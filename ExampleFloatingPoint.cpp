// Floating Point Color.
// Pixels are float r,g,b,a values automatically clamped in range [0,1]
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

        Display display("Floating Point Example", width, height);

        vector<Pixel> pixels(width * height);

        while (display.open())
        {
            unsigned int index = 0;

            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width; ++x)
                {
                    pixels[index].r = 0.1f + (x + y) * 0.0015f;
                    pixels[index].g = 0.5f + (x + y) * 0.001f;
                    pixels[index].b = 0.7f + (x + y) * 0.0005f;

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
