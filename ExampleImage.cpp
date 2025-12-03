// Image example
// Demonstrates how to load and display a TGA image
// Part of the PixelToaster Framebuffer Library - http://www.pixeltoaster.com

#include <cstdio>
#include "PixelToaster.h"

#ifdef PIXELTOASTER_NO_STL
#    include <vector>
using std::vector;
#endif

using namespace PixelToaster;

bool load(const char filename[], int& width, int& height, vector<Pixel>& pixels);

int main()
{
    try
    {
        // load image and show it on the screen

        vector<Pixel> pixels;

        int width  = 0;
        int height = 0;

        bool loaded = false;
        try
        {
            loaded = load("ExampleImage.tga", width, height, pixels);
        }
        catch (const PixelToaster::PixelToasterException& e)
        {
            printf("Failed to load image from current directory: %s\n", e.what());
        }

        if (!loaded)
        {
            try
            {
                loaded = load("../ExampleImage.tga", width, height, pixels);
            }
            catch (const PixelToaster::PixelToasterException& e)
            {
                printf("Failed to load image from parent directory: %s\n", e.what());
            }
        }

        if (!loaded)
        {
            printf("Failed to load image from both locations\n");
            return 1;
        }

        Display display("Image Example", width, height);

        while (display.open())
        {
#ifdef PIXELTOASTER_NO_STL
            display.update(pixels.data());
#else
            display.update(pixels);
#endif
        }

        return 0;
    }
    catch (const PixelToaster::PixelToasterException& e)
    {
        printf("PixelToaster error: %s (Error code: %d)\n", e.what(), e.getErrorCode());
        return 1;
    }
    catch (const std::exception& e)
    {
        printf("Standard library error: %s\n", e.what());
        return 1;
    }
    catch (...)
    {
        printf("Unknown error occurred\n");
        return 1;
    }
}

bool load(const char filename[], int& width, int& height, vector<Pixel>& pixels)
{
    if (filename == nullptr)
    {
        throw PixelToaster::NullPointerException("Filename parameter cannot be null");
    }

    unsigned int index = 0;
    FILE* file = nullptr;

    try
    {
        vector<unsigned char> buffer;

        // open file for binary reading
        file = fopen(filename, "rb");
        if (!file)
        {
            throw PixelToaster::FileOperationException(
                std::string("Failed to open file: ") + filename,
                errno);
        }

        // read 18 byte TGA header
        unsigned char header[18];
        if (fread(header, 18, 1, file) != 1)
        {
            throw PixelToaster::FileOperationException(
                std::string("Failed to read TGA header from file: ") + filename,
                ferror(file) ? ferror(file) : EIO);
        }

        // verify TGA format
        if (header[2] != 2)
        {
            throw PixelToaster::InvalidParameterException(
                "Unsupported TGA format: must be uncompressed RGB format (type 2)");
        }

        if (header[16] != 24)
        {
            throw PixelToaster::InvalidParameterException(
                "Unsupported TGA bit depth: must be 24 bits per pixel");
        }

        // get image dimensions
        width  = (header[13] << 8) | header[12];
        height = (header[15] << 8) | header[14];

        if (width <= 0 || height <= 0)
        {
            throw PixelToaster::InvalidParameterException(
                std::string("Invalid image dimensions: ") + 
                std::to_string(width) + "x" + std::to_string(height));
        }

        // read image pixel data
        const size_t bufferSize = width * height * 3;
        buffer.resize(bufferSize, 0);
        
        if (fread(&buffer[0], bufferSize, 1, file) != 1)
        {
            throw PixelToaster::FileOperationException(
                std::string("Failed to read pixel data from file: ") + filename,
                ferror(file) ? ferror(file) : EIO);
        }

        // close file
        fclose(file);
        file = nullptr;

        // convert 24 bit image pixels to floating point color
        pixels.resize(width * height);
        
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                Pixel& pixel = pixels[index];

                pixel.b = buffer[index * 3 + 0] * 1.0f / 255.0f;
                pixel.g = buffer[index * 3 + 1] * 1.0f / 255.0f;
                pixel.r = buffer[index * 3 + 2] * 1.0f / 255.0f;

                ++index;
            }
        }

        return true;
    }
    catch (...)
    {
        // clean up resources on failure
        if (file != nullptr)
        {
            fclose(file);
        }
        
        width  = 0;
        height = 0;
        pixels.clear();
        
        // rethrow the exception
        throw;
    }
}
