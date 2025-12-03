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

        if (!load("ExampleImage.tga", width, height, pixels) &&
            !load("../ExampleImage.tga", width, height, pixels))
        {
            printf("failed to load image\n");
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
}

bool load(const char filename[], int& width, int& height, vector<Pixel>& pixels)
{
    // 参数校验
    if (!filename || filename[0] == '\0')
    {
        printf("invalid filename\n");
        return false;
    }

    unsigned int index = 0;
    vector<unsigned char> buffer;
    FILE* file = nullptr;

    try
    {
        // 打开文件
        file = fopen(filename, "rb");
        if (!file)
        {
            printf("failed to open file: %s\n", filename);
            return false;
        }

        // 读取TGA头
        unsigned char header[18];
        if (fread(header, 1, 18, file) != 18)
        {
            printf("failed to read TGA header\n");
            throw std::runtime_error("failed to read TGA header");
        }

        // 检查TGA格式
        if (header[2] != 2)
        {
            printf("tga must be uncompressed rgb format\n");
            throw std::runtime_error("unsupported TGA format");
        }

        if (header[16] != 24)
        {
            printf("tga must be 24 bits per pixel\n");
            throw std::runtime_error("unsupported TGA bit depth");
        }

        // 获取图像尺寸
        width  = (header[13] << 8) | header[12];
        height = (header[15] << 8) | header[14];

        if (width <= 0 || height <= 0)
        {
            printf("invalid image dimensions: %dx%d\n", width, height);
            throw std::runtime_error("invalid image dimensions");
        }

        // 读取像素数据
        size_t pixelDataSize = width * height * 3;
        buffer.resize(pixelDataSize);

        if (fread(buffer.data(), 1, pixelDataSize, file) != pixelDataSize)
        {
            printf("failed to read image pixel data\n");
            throw std::runtime_error("failed to read pixel data");
        }

        // 关闭文件
        fclose(file);
        file = nullptr;

        // 转换为浮点像素格式
        pixels.resize(width * height);

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                Pixel& pixel = pixels[index];
                const unsigned char* bgr = &buffer[index * 3];

                pixel.r = bgr[2] / 255.0f;
                pixel.g = bgr[1] / 255.0f;
                pixel.b = bgr[0] / 255.0f;

                ++index;
            }
        }

        return true;
    }
    catch (const std::exception& e)
    {
        printf("error loading image: %s\n", e.what());
        if (file)
        {
            fclose(file);
        }
        return false;
    }
    catch (...)
    {
        printf("unknown error loading image\n");
        if (file)
        {
            fclose(file);
        }
        return false;
    }
}

    return true;

failure:

    // onoes somebody call the whaaaambulance...

    width  = 0;
    height = 0;
    fclose(file);

    return false;
}
