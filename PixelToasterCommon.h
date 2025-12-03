// Common code across all platforms
// Copyright © 2004-2007 Glenn Fiedler
// Part of the PixelToaster Framebuffer Library - http://www.pixeltoaster.com

#ifndef PIXELTOASTER_NO_CRT
#    include <ctime>
#endif

namespace PixelToaster {
// glenn's magical strcpy replacement with bram's template touch ...
template <int n>
inline void magical_strcpy(char (&dest)[n], const char src[])
{
    unsigned int i = 0;
    while (i < n - 1)
    {
        if (src[i] == 0)
            break;
        dest[i] = src[i];
        i++;
    }
    dest[i] = 0;
}

// derive your platform's display implementation from this and it will handle all the mundane details for you

class DisplayAdapter : public DisplayInterface
{
public:
    DisplayAdapter()
    {
        _listener = nullptr;
        _wrapper  = nullptr;
        try
        {
            defaults();
        }
        catch (const std::exception& e)
        {
            throw PixelToaster::ResourceException(std::string("Failed to initialize display adapter: ") + e.what());
        }
    }

    ~DisplayAdapter()
    {
        try
        {
            close();
            _listener = nullptr;
            _wrapper  = nullptr;
        }
        catch (const std::exception& e)
        {
            // 析构函数不应该抛出异常，所以我们只记录错误
            (void)e; // 避免未使用变量警告
        }
    }

    bool open(const char title[], int width, int height, Output output, Mode mode) override
    {
        // 参数范围检查
        if (width <= 0 || width > 8192)
        {
            throw PixelToaster::InvalidParameterException(
                std::string("Invalid width parameter: ") + std::to_string(width) + 
                ". Width must be between 1 and 8192.");
        }
        
        if (height <= 0 || height > 8192)
        {
            throw PixelToaster::InvalidParameterException(
                std::string("Invalid height parameter: ") + std::to_string(height) + 
                ". Height must be between 1 and 8192.");
        }
        
        if (title == nullptr)
        {
            throw PixelToaster::NullPointerException("Title parameter cannot be null");
        }
        
        close();

        magical_strcpy(_title, title);
        _width  = width;
        _height = height;
        _output = output;
        _mode   = mode;
        _open   = true;

        return true;
    }

    void close() override
    {
        defaults();
    }

    bool open() const override
    {
        return _open;
    }

    bool update(const TrueColorPixel pixels[], const Rectangle* dirtyBox) override
    {
        if (pixels == nullptr)
        {
            throw PixelToaster::NullPointerException("TrueColorPixel pixels array cannot be null");
        }
        
        if (dirtyBox != nullptr)
        {
            if (dirtyBox->x < 0 || dirtyBox->y < 0 || 
                dirtyBox->width <= 0 || dirtyBox->height <= 0)
            {
                throw PixelToaster::InvalidParameterException(
                    std::string("Invalid dirtyBox parameters: x=") + std::to_string(dirtyBox->x) +
                    ", y=" + std::to_string(dirtyBox->y) +
                    ", width=" + std::to_string(dirtyBox->width) +
                    ", height=" + std::to_string(dirtyBox->height));
            }
        }
        
        return update(pixels, 0, dirtyBox);
    }

    bool update(const FloatingPointPixel pixels[], const Rectangle* dirtyBox) override
    {
        if (pixels == nullptr)
        {
            throw PixelToaster::NullPointerException("FloatingPointPixel pixels array cannot be null");
        }
        
        if (dirtyBox != nullptr)
        {
            if (dirtyBox->x < 0 || dirtyBox->y < 0 || 
                dirtyBox->width <= 0 || dirtyBox->height <= 0)
            {
                throw PixelToaster::InvalidParameterException(
                    std::string("Invalid dirtyBox parameters: x=") + std::to_string(dirtyBox->x) +
                    ", y=" + std::to_string(dirtyBox->y) +
                    ", width=" + std::to_string(dirtyBox->width) +
                    ", height=" + std::to_string(dirtyBox->height));
            }
        }
        
        return update(0, pixels, dirtyBox);
    }

    const char* title() const override
    {
        return _title;
    }

    void title(const char title[]) override
    {
        magical_strcpy(_title, title);
    }

    int width() const override
    {
        return _width;
    }

    int height() const override
    {
        return _height;
    }

    Mode mode() const override
    {
        return _mode;
    }

    Output output() const override
    {
        return _output;
    }

    void listener(Listener* listener) override
    {
        _listener = listener;
    }

    Listener* listener() const override
    {
        return _listener;
    }

    void wrapper(DisplayInterface* display) override
    {
        _wrapper = display;
    }

    DisplayInterface* wrapper() override
    {
        return _wrapper;
    }

protected:
    // note: override this "unified" update to implement your display update.
    // only one of the pointers will be non-null, this allows you to avoid
    // duplicating update code between truecolor and floating point update methods.

    virtual bool update(const TrueColorPixel* trueColorPixels, const FloatingPointPixel* floatingPointPixels, const Rectangle* dirtyBox) { return true; }

    // this defaults is virtual, override it to add your own defaults
    // but make sure you always call the superclass defaults in your overridden function!
    // note: due to c++ constructor oddities, make sure you also call defaults in your own
    // display ctor, otherwise it will only call this base version because the vtable is not
    // setup yet. see the windows display for an example of this.

    virtual void defaults()
    {
        _title[0] = 0;
        _width    = 0;
        _height   = 0;
        _mode     = Mode::FloatingPoint;
        _output   = Output::Default;
        _open     = false;
    }

    // switch to windowed output.
    // override this to implement your own switch.

    virtual bool windowed()
    {
        _output = Output::Windowed;
        return true;
    }

    // switch to fullscreen output.
    // override this to implement your own switch.

    virtual bool fullscreen()
    {
        _output = Output::Fullscreen;
        return true;
    }

private:
    char              _title[256];
    int               _width;
    int               _height;
    Mode              _mode;
    Output            _output;
    bool              _open;
    Listener*         _listener;
    DisplayInterface* _wrapper; // required for listener callbacks
};

#ifndef PIXELTOASTER_NO_CRT

// portable timer implementation for platforms without a specific high res timer

class PortableTimer : public PixelToaster::TimerInterface
{
public:
    PortableTimer()
    {
        try
        {
            _resolution = 1.0 / CLOCKS_PER_SEC;
            reset();
        }
        catch (const std::exception& e)
        {
            throw PixelToaster::ResourceException(std::string("Failed to initialize portable timer: ") + e.what());
        }
    }

    void reset()
    {
        _time         = 0;
        _timeCounter  = clock();
        _deltaCounter = _timeCounter;
    }

    double time()
    {
        clock_t counter = std::clock();
        double  delta   = (counter - _timeCounter) * _resolution;
        _timeCounter    = counter;
        _time += delta;
        return _time;
    }

    double delta()
    {
        clock_t counter = std::clock();
        double  delta   = (counter - _deltaCounter) * _resolution;
        _deltaCounter   = counter;
        return delta;
    }

    double resolution()
    {
        return _resolution;
    }

    void wait(double seconds)
    {
        clock_t start  = std::clock();
        clock_t finish = start + clock_t(seconds / _resolution);
        while (std::clock() < finish)
            ;
    }

private:
    double  _time;         ///< current time in seconds
    double  _resolution;   ///< timer resolution in seconds
    clock_t _timeCounter;  ///< time counter in clocks
    clock_t _deltaCounter; ///< delta counter in clocks
};

#else

// dummy timer if we are not using CRT

class PortableTimer : public PixelToaster::TimerInterface
{
public:
    PortableTimer() {}
    void   reset() {}
    double time() { return 0.0f; }
    double delta() { return 0.0f; }
    double resolution() { return 0.0f; }
    void   wait(double seconds) {}
};

#endif
} // namespace PixelToaster
