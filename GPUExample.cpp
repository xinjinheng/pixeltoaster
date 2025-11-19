// PixelToaster GPU Example
// Shows how to use the new GPU acceleration features

#include "PixelToaster.h"
#include "PixelToasterGPU.h"
#include "PixelToasterVulkan.h"

using namespace PixelToaster;

class GPUExample {
public:
    GPUExample() : m_display(Display::Mode::windowed, 800, 600, "PixelToaster GPU Example"), m_running(true)
    {
        // Initialize the GPU context
        m_gpuContext = createGPUContext();
        if (m_gpuContext && m_gpuContext->isAvailable()) {
            std::cout << "GPU context initialized successfully: " << m_gpuContext->deviceName() << std::endl;
        } else {
            std::cout << "Failed to initialize GPU context, falling back to CPU" << std::endl;
        }
    }

    ~GPUExample()
    {
        if (m_gpuContext) {
            destroyGPUContext(m_gpuContext);
        }
    }

    void run()
    {
        // Open the display with GPU rendering
        m_display.open(RenderMode::GPU);

        // Create a framebuffer for GPU rendering
        Framebuffer* gpuFramebuffer = nullptr;
        if (m_gpuContext) {
            gpuFramebuffer = m_gpuContext->createFramebuffer(800, 600, Format::RGBA32F);
        }

        // Create a simple animation timeline
        Timeline* timeline = createTimeline();
        TransformAnimation* transformAnim = timeline->createTransformAnimation();
        transformAnim->setDuration(2.0f);
        transformAnim->setLoop(true);
        transformAnim->setEasingFunction(Easing::easeInOutQuad);

        // Start the animation
        timeline->play();

        // Main loop
        while (m_running) {
            m_display.update();

            // Update animation timeline
            timeline->update();

            // Get the frame buffer
            void* buffer = m_display.buffer();

            if (m_gpuContext && gpuFramebuffer) {
                // Use GPU to render
                m_gpuContext->setFramebuffer(gpuFramebuffer);
                m_gpuContext->clear(0.2f, 0.3f, 0.4f, 1.0f);

                // Draw a rectangle with animation
                float scale = 0.5f + 0.5f * transformAnim->progress();
                m_gpuContext->drawRectangle(400 - 100 * scale, 300 - 100 * scale, 200 * scale, 200 * scale);

                // Draw a triangle
                m_gpuContext->drawTriangle(400, 100, 200, 500, 600, 500);

                // Flush GPU commands
                m_gpuContext->flush();

                // Read back to CPU buffer if needed
                gpuFramebuffer->readPixels(buffer, Format::XRGB8888);
            } else {
                // Fall back to CPU rendering
                auto* pixels = static_cast<TrueColorPixel*>(buffer);
                for (int y = 0; y < 600; ++y) {
                    for (int x = 0; x < 800; ++x) {
                        int index = y * 800 + x;
                        pixels[index].r = (x / 800.0f) * 255;
                        pixels[index].g = (y / 600.0f) * 255;
                        pixels[index].b = 128;
                    }
                }
            }

            m_display.swap();
        }

        if (m_gpuContext && gpuFramebuffer) {
            m_gpuContext->destroyFramebuffer(gpuFramebuffer);
        }
        if (timeline) {
            destroyTimeline(timeline);
        }
    }

    void onKeyDown(Key key)
    {
        if (key == Key::escape) {
            m_running = false;
        }
    }

private:
    Display m_display;
    GPU::Context* m_gpuContext;
    bool m_running;
};

int main(int argc, char** argv)
{
    try {
        GPUExample example;
        example.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
