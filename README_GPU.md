# PixelToaster GPU Extension

## Overview

PixelToaster GPU is an extension to the PixelToaster framebuffer library that adds GPU acceleration capabilities using Vulkan API. This allows for real-time rendering and complex dynamic effects while maintaining compatibility with the existing CPU rendering functionality.

## Features

### Hybrid Rendering Architecture
- **Dynamic Mode Switching**: Seamlessly switch between CPU software rendering and GPU hardware acceleration
- **Unified API**: Single interface for both CPU and GPU rendering
- **Pixel Format Compatibility**: Supports existing `FloatingPointPixel` and `TrueColorPixel` formats
- **GPU-optimized Formats**: New formats including FP16, RGBA32F for high-quality GPU rendering

### GPU Accelerated Rendering Module
- **Vulkan Backend**: Cross-platform GPU support for Windows, Linux, macOS
- **Basic 2D Geometry**: Hardware-accelerated rectangle and triangle drawing with anti-aliasing
- **Pixel Filters**: GPU implementations of Gaussian blur, edge detection, and tone mapping
- **Compute Shaders**: Support for custom pixel processing logic
- **Framebuffer Operations**: Efficient read/write between CPU and GPU memory

### Dynamic Effects Engine
- **Keyframe Animation**: Timeline-based animation system
- **Transformations**: Scale, rotate, and distort pixel regions
- **Color Gradients**: Smooth HSL space color transitions
- **Particle System**: Support for 10,000+ particles with real-time rendering
- **Timeline Management**: Animation composition, looping, and transitions

### Performance Optimization
- **Task Scheduler**: Automatic CPU/GPU load balancing
- **Framebuffer Compression**: ASTC-based compression to reduce memory bandwidth
- **Performance Monitoring**: Real-time CPU/GPU profiling and heatmap visualization

## Getting Started

### Prerequisites
- Vulkan SDK installed (version 1.0 or later)
- C++17 compatible compiler
- Windows, Linux, or macOS operating system

### Building

#### Using CMake (Recommended)
```bash
mkdir build
cd build
cmake ..
make
```

#### Using Visual Studio
- Open the PixelToaster.sln file
- Build the solution

#### Using Build Script (Windows)
```bash
build.bat
```

## Usage

### Basic Example

```cpp
#include "PixelToaster.h"
#include "PixelToasterGPU.h"

using namespace PixelToaster;

int main() {
    // Create a display with windowed mode
    Display display(Display::Mode::windowed, 800, 600, "GPU Example");
    
    // Initialize GPU context
    GPU::Context* gpuContext = createGPUContext();
    
    // Open display with GPU rendering
    display.open(RenderMode::GPU);
    
    bool running = true;
    while (running) {
        display.update();
        
        void* buffer = display.buffer();
        
        if (gpuContext && gpuContext->isAvailable()) {
            // GPU rendering
            gpuContext->clear(0.2f, 0.3f, 0.4f, 1.0f);
            gpuContext->drawRectangle(100, 100, 200, 200);
            gpuContext->flush();
        } else {
            // CPU fallback
            auto* pixels = static_cast<TrueColorPixel*>(buffer);
            // ... CPU rendering ...
        }
        
        display.swap();
    }
    
    destroyGPUContext(gpuContext);
    return 0;
}
```

### Animation Example

```cpp
// Create timeline
Timeline* timeline = createTimeline();

// Create transformation animation
TransformAnimation* transformAnim = timeline->createTransformAnimation();
transformAnim->setDuration(2.0f);
transformAnim->setLoop(true);
transformAnim->setEasingFunction(Easing::easeInOutQuad);

// Play animation
timeline->play();

while (running) {
    // Update animation
timeline->update();
    
    // Apply transformation
    float scale = 0.5f + 0.5f * transformAnim->progress();
    gpuContext->drawRectangle(400 - 100 * scale, 300 - 100 * scale, 200 * scale, 200 * scale);
    
    // ...
}
```

## API Reference

### RenderMode
```cpp
enum class RenderMode {
    CPU,    // Use CPU software rendering
    GPU,    // Use GPU hardware acceleration
    Hybrid  // Automatic switching based on load
};
```

### GPU Context
```cpp
// Create and destroy GPU context
GPU::Context* createGPUContext();
void destroyGPUContext(GPU::Context* context);

// Context methods
bool isAvailable() const;
const char* deviceName() const;
Shader* createShader(const char* vertexShader, const char* fragmentShader);
ComputeShader* createComputeShader(const char* computeShader);
Framebuffer* createFramebuffer(int width, int height, Format format);
// ... rendering methods ...
```

### Shaders
```cpp
// Create shader
Shader* shader = gpuContext->createShader(vertexCode, fragmentCode);

// Set uniforms
shader->setUniform("color", 1.0f, 0.0f, 0.0f, 1.0f);

// Apply shader
gpuContext->setShader(shader);
```

### Compute Shaders
```cpp
// Create compute shader
ComputeShader* computeShader = gpuContext->createComputeShader(computeCode);

// Set buffers
computeShader->setBuffer("inputBuffer", inputData, size);
computeShader->setImage("outputImage", framebuffer, 0);

// Dispatch compute
computeShader->dispatch(16, 16, 1);
```

## Configuration

### Build Options
- `PIXELTOASTER_USE_VULKAN`: Enable Vulkan GPU support (default: ON)
- `PIXELTOASTER_GPU_DEBUG`: Enable GPU debug messages (default: OFF)
- `PIXELTOASTER_ENABLE_PERFORMANCE_MONITOR`: Enable performance monitoring (default: ON)

### Runtime Options
- `gpu_render_mode`: Global setting for default render mode
- `gpu_device_index`: Select specific GPU device (0 for default)
- `gpu_memory_limit`: Maximum GPU memory to use (in MB)

## Performance Considerations

1. **Use GPU Formats**: Prefer `RGBA16F` or `RGBA32F` for GPU rendering to avoid format conversion overhead
2. **Minimize Data Transfers**: Reduce CPU-GPU data transfers by batching operations
3. **Use Hybrid Mode**: Let the system automatically balance CPU and GPU load
4. **Enable Compression**: Use ASTC framebuffer compression for memory bandwidth savings
5. **Profile Regularly**: Monitor performance stats to identify bottlenecks

## Supported Platforms

- **Windows**: Windows 10/11 with Vulkan 1.0+ support
- **Linux**: X11 or Wayland with Vulkan 1.0+ support
- **macOS**: macOS 10.13+ with Vulkan 1.0+ support (via MoltenVK)

## Compatibility

- **PixelToaster 1.5.0**: Fully compatible with existing API
- **Backward Compatible**: Applications without GPU code will continue to work
- **Cross-Compiler**: Support for MSVC, GCC, Clang

## License

PixelToaster GPU is released under the same MIT license as the original PixelToaster library.

## Credits

- Original PixelToaster: Matthew Conte
- GPU Extension: Trae AI
- Vulkan API: Khronos Group

## Support

For issues and feature requests, please visit the GitHub repository.
