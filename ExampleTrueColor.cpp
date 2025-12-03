// TrueColor Example
// How to open a display in truecolor mode and work with 32 bit integer pixels.
// Part of the PixelToaster Framebuffer Library - http://www.pixeltoaster.com

#include "PixelToaster.h"

#ifdef PIXELTOASTER_NO_STL
#    include <vector>
using std::vector;
#endif

using namespace PixelToaster;

int main()
{
    try
    {
        const int width  = 320;
        const int height = 240;

        Display display("TrueColor Example", width, height, Output::Default, Mode::TrueColor);

        vector<TrueColorPixel> pixels(width * height);

        while (display.open())
        {
            unsigned int index = 0;

            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width; ++x)
                {
                    pixels[index].r = (integer8)(x < 255 ? x : 255);
                    pixels[index].g = (integer8)(y < 255 ? y : 255);
                    pixels[index].b = (integer8)(x + y < 255 ? x + y : 255);

                    ++index;
                }
            }

#ifdef PIXELTOASTER_NO_STL
            display.update(pixels.data());
#else
            display.update(pixels);
#endif
        }
    }
    catch (const PixelToasterException& e)
    {
        // Print error message and exit with failure
        printf("PixelToaster Error: %s (Error Code: %d)\n", e.what(), e.errorCode());
        return 1;
    }
    catch (const std::exception& e)
    {
        // Print general error message and exit with failure
        printf("Error: %s\n", e.what());
        return 1;
    }
    catch (...)
    {
        // Print unknown error message and exit with failure
        printf("Unknown Error\n");
        return 1;
    }

    return 0;
}
