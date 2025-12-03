#pragma once

#include <chrono>
#include <stdexcept>
#include <thread>

namespace PixelToaster
{
    /// rief Base exception class for iterator-related errors
    class IteratorException : public std::runtime_error
    {
    public:
        explicit IteratorException(const std::string& message) : std::runtime_error(message) {}
    };

    /// rief Exception thrown when an iterator operation times out
    class IteratorTimeoutException : public IteratorException
    {
    public:
        explicit IteratorTimeoutException(const std::string& message) : IteratorException(message) {}
    };

    /// rief Base class for all iterators in PixelToaster
    /// 	param T The type of elements being iterated
    template <typename T>
    class BaseIterator
    {
    public:
        /// rief Default constructor
        BaseIterator() : currentPosition(0), iteratedCount(0), startTime(std::chrono::steady_clock::now()) {}

        /// rief Virtual destructor
        virtual ~BaseIterator() = default;

        /// rief Check if there are more elements to iterate
        /// eturns true if there are more elements, false otherwise
        virtual bool hasNext() const = 0;

        /// rief Get the next element
        /// \param timeout Maximum time to wait for the next element in milliseconds (default: no timeout)
        /// eturns The next element
        /// 	hrows IteratorTimeoutException if the operation times out
        /// 	hrows IteratorException if an error occurs
        virtual T next(unsigned int timeout = 0) = 0;

        /// rief Reset the iterator to the beginning
        virtual void reset()
        {
            currentPosition = 0;
            iteratedCount = 0;
            startTime = std::chrono::steady_clock::now();
        }

        /// rief Get the current position in the iteration
        /// eturns The current position
        size_t getCurrentPosition() const { return currentPosition; }

        /// rief Get the number of elements iterated so far
        /// eturns The number of elements iterated
        size_t getIteratedCount() const { return iteratedCount; }

        /// rief Get the time elapsed since the iterator was created or reset
        /// eturns The elapsed time in milliseconds
        double getElapsedTime() const
        {
            auto now = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
            return duration.count();
        }

    protected:
        size_t currentPosition; ///< Current position in the iteration
        size_t iteratedCount;   ///< Number of elements iterated so far
        std::chrono::steady_clock::time_point startTime; ///< Time when the iterator was created or reset

        /// rief Check if the operation has timed out
        /// \param timeout Maximum time to wait in milliseconds
        /// \param start Time when the operation started
        /// eturns true if timed out, false otherwise
        bool isTimedOut(unsigned int timeout, const std::chrono::steady_clock::time_point& start) const
        {
            if (timeout == 0)
                return false;

            auto now = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
            return duration.count() >= timeout;
        }

        /// rief Update the iterator state after iterating an element
        void updateState()
        {
            currentPosition++;
            iteratedCount++;
        }
    };

    /// rief Raw iterator for accessing raw memory
    /// 	param T The type of elements being iterated
    template <typename T>
    class RawIterator : public BaseIterator<T>
    {
    public:
        /// rief Constructor
        /// \param data Pointer to the raw data
        /// \param size Number of elements in the data
        RawIterator(const T* data, size_t size) : BaseIterator<T>(), data(data), size(size) {}

        /// rief Check if there are more elements to iterate
        /// eturns true if there are more elements, false otherwise
        bool hasNext() const override
        {
            return this->currentPosition < size;
        }

        /// rief Get the next element
        /// \param timeout Maximum time to wait for the next element in milliseconds (default: no timeout)
        /// eturns The next element
        /// 	hrows IteratorTimeoutException if the operation times out
        /// 	hrows IteratorException if an error occurs
        T next(unsigned int timeout = 0) override
        {
            auto start = std::chrono::steady_clock::now();

            // Wait until there is a next element or timeout
            while (!hasNext())
            {
                if (this->isTimedOut(timeout, start))
                    throw IteratorTimeoutException("RawIterator::next() timed out");

                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

            if (data == nullptr)
                throw IteratorException("RawIterator::next() - data pointer is null");

            T element = data[this->currentPosition];
            this->updateState();
            return element;
        }

        /// rief Reset the iterator to the beginning
        void reset() override
        {
            BaseIterator<T>::reset();
        }

    private:
        const T* data; ///< Pointer to the raw data
        size_t size;   ///< Number of elements in the data
    };

    /// rief Iterator for accessing elements from a vector
    /// 	param T The type of elements being iterated
    template <typename T>
    class VectorIterator : public BaseIterator<T>
    {
    public:
        /// rief Constructor
        /// \param vector Reference to the vector
        explicit VectorIterator(const std::vector<T>& vector) : BaseIterator<T>(), vector(&vector) {}

        /// rief Check if there are more elements to iterate
        /// eturns true if there are more elements, false otherwise
        bool hasNext() const override
        {
            return this->currentPosition < vector->size();
        }

        /// rief Get the next element
        /// \param timeout Maximum time to wait for the next element in milliseconds (default: no timeout)
        /// eturns The next element
        /// 	hrows IteratorTimeoutException if the operation times out
        /// 	hrows IteratorException if an error occurs
        T next(unsigned int timeout = 0) override
        {
            auto start = std::chrono::steady_clock::now();

            // Wait until there is a next element or timeout
            while (!hasNext())
            {
                if (this->isTimedOut(timeout, start))
                    throw IteratorTimeoutException("VectorIterator::next() timed out");

                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

            T element = (*vector)[this->currentPosition];
            this->updateState();
            return element;
        }

        /// rief Reset the iterator to the beginning
        void reset() override
        {
            BaseIterator<T>::reset();
        }

    private:
        const std::vector<T>* vector; ///< Pointer to the vector
    };

    /// rief Factory function to create a RawIterator
    /// 	param T The type of elements being iterated
    /// \param data Pointer to the raw data
    /// \param size Number of elements in the data
    /// eturns A unique_ptr to the created RawIterator
    template <typename T>
    std::unique_ptr<BaseIterator<T>> createRawIterator(const T* data, size_t size)
    {
        return std::make_unique<RawIterator<T>>(data, size);
    }

    /// rief Factory function to create a VectorIterator
    /// 	param T The type of elements being iterated
    /// \param vector Reference to the vector
    /// eturns A unique_ptr to the created VectorIterator
    template <typename T>
    std::unique_ptr<BaseIterator<T>> createVectorIterator(const std::vector<T>& vector)
    {
        return std::make_unique<VectorIterator<T>>(vector);
    }
} // namespace PixelToaster