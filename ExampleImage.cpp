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

void load(const char filename[], int& width, int& height, vector<Pixel>& pixels);

int main()
{
    try {
        // load image and show it on the screen

        vector<Pixel> pixels;

        int width  = 0;
        int height = 0;

        try {
            load("ExampleImage.tga", width, height, pixels);
        } catch (const PixelToasterException&) {
            // Try loading from parent directory
            load("../ExampleImage.tga", width, height, pixels);
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

    } catch (const PixelToasterException& e) {
        printf("Error: %s (code: %d)\n", e.what(), e.getErrorCode());
        return 1;
    }

    return 0;
}

void load(const char filename[], int& width, int& height, vector<Pixel>& pixels)
{
    unsigned int index = 0;

    vector<unsigned char> buffer;

    // open file for binary reading

    FILE* file = fopen(filename, "rb");

    if (!file)
    {
        throw PixelToaster::ResourceException("Failed to open file: " + std::string(filename));
    }

    // read 18 byte TGA header

    unsigned char header[18];

    if (!fread(header, 18, 1, file))
    {
        fclose(file);
        throw PixelToaster::ResourceException("Failed to read TGA header from file: " + std::string(filename));
    }

    // fail if not uncompressed rgb format

    if (header[2] != 2)
    {
        fclose(file);
        throw PixelToaster::InvalidParameterException("TGA file must be in uncompressed RGB format: " + std::string(filename));
    }

    // fail if not 24 bits per pixel

    if (header[16] != 24)
    {
        fclose(file);
        throw PixelToaster::InvalidParameterException("TGA file must be 24 bits per pixel: " + std::string(filename));
    }

    // read image pixels

    width  = (header[13] << 8) | header[12];
    height = (header[15] << 8) | header[14];

    if (width <= 0 || height <= 0)
    {
        fclose(file);
        throw PixelToaster::InvalidParameterException("Invalid image dimensions in TGA file: " + std::string(filename));
    }

    buffer.resize(width * height * 3, 0);

    if (!fread(&buffer[0], buffer.size(), 1, file))
    {
        fclose(file);
        throw PixelToaster::ResourceException("Failed to read image pixel data from file: " + std::string(filename));
    }

    fclose(file);

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
}
