/*
  PixelToaster Framebuffer Library.

  Copyright © 2004-2007 Glenn Fiedler
  Copyright © 2024 Your Name

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.

  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.

  3. This notice may not be removed or altered from any source distribution.

  Glenn Fiedler
  gaffer@gaffer.org
*/

#ifndef PIXELTOASTER_EXCEPTION_H
#define PIXELTOASTER_EXCEPTION_H

#include <stdexcept>
#include <string>

namespace PixelToaster {

/**
 * @brief Base exception class for all PixelToaster exceptions.
 */
class PixelToasterException : public std::runtime_error {
public:
    /**
     * @brief Constructs a PixelToasterException.
     * @param message The error message.
     * @param errorCode The error code (optional).
     */
    explicit PixelToasterException(const std::string& message, int errorCode = 0)
        : std::runtime_error(message), m_errorCode(errorCode) {}

    /**
     * @brief Gets the error code.
     * @return The error code.
     */
    int getErrorCode() const { return m_errorCode; }

private:
    int m_errorCode; ///< The error code.
};

/**
 * @brief Exception thrown when a null pointer is encountered.
 */
class NullPointerException : public PixelToasterException {
public:
    /**
     * @brief Constructs a NullPointerException.
     * @param message The error message.
     * @param errorCode The error code (optional).
     */
    explicit NullPointerException(const std::string& message, int errorCode = 0)
        : PixelToasterException(message, errorCode) {}
};

/**
 * @brief Exception thrown when a resource operation fails.
 */
class ResourceException : public PixelToasterException {
public:
    /**
     * @brief Constructs a ResourceException.
     * @param message The error message.
     * @param errorCode The error code (optional).
     */
    explicit ResourceException(const std::string& message, int errorCode = 0)
        : PixelToasterException(message, errorCode) {}
};

/**
 * @brief Exception thrown when an invalid parameter is provided.
 */
class InvalidParameterException : public PixelToasterException {
public:
    /**
     * @brief Constructs an InvalidParameterException.
     * @param message The error message.
     * @param errorCode The error code (optional).
     */
    explicit InvalidParameterException(const std::string& message, int errorCode = 0)
        : PixelToasterException(message, errorCode) {}
};

/**
 * @brief Exception thrown when a platform-specific error occurs.
 */
class PlatformException : public PixelToasterException {
public:
    /**
     * @brief Constructs a PlatformException.
     * @param message The error message.
     * @param errorCode The error code (optional).
     */
    explicit PlatformException(const std::string& message, int errorCode = 0)
        : PixelToasterException(message, errorCode) {}
};

} // namespace PixelToaster

#endif // PIXELTOASTER_EXCEPTION_H
