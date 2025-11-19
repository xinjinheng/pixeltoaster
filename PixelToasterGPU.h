// PixelToaster GPU Accelerated Rendering
// Copyright (c) 2024
// Part of the PixelToaster Framebuffer Library

#ifndef PIXELTOASTER_GPU_H
#define PIXELTOASTER_GPU_H

#include "PixelToaster.h"

namespace PixelToaster {
namespace GPU {

// Forward declarations
class Context;
class Shader;
class ComputeShader;
class Framebuffer;

// GPU Context class
class PIXELTOASTER_API Context {
public:
    virtual ~Context() = default;
    
    // GPU device information
    virtual const char* deviceName() const = 0;
    virtual bool isAvailable() const = 0;
    
    // Shader management
    virtual Shader* createShader(const char* vertexShader, const char* fragmentShader) = 0;
    virtual ComputeShader* createComputeShader(const char* computeShader) = 0;
    virtual void destroyShader(Shader* shader) = 0;
    virtual void destroyComputeShader(ComputeShader* shader) = 0;
    
    // Framebuffer management
    virtual Framebuffer* createFramebuffer(int width, int height, Format format) = 0;
    virtual void destroyFramebuffer(Framebuffer* framebuffer) = 0;
    
    // Render commands
    virtual bool setShader(Shader* shader) = 0;
    virtual bool setComputeShader(ComputeShader* shader) = 0;
    virtual bool setFramebuffer(Framebuffer* framebuffer) = 0;
    virtual void clear(float r, float g, float b, float a) = 0;
    
    // Draw commands
    virtual void drawRectangle(float x, float y, float width, float height) = 0;
    virtual void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3) = 0;
    
    // Compute commands
    virtual void dispatchCompute(int x, int y, int z) = 0;
    
    // Synchronization
    virtual void flush() = 0;
    virtual void finish() = 0;
};

// Shader base class
class PIXELTOASTER_API Shader {
public:
    virtual ~Shader() = default;
    
    virtual bool setUniform(const char* name, float value) = 0;
    virtual bool setUniform(const char* name, const float* values, int count) = 0;
    virtual bool setUniform(const char* name, int value) = 0;
    virtual bool setUniform(const char* name, const int* values, int count) = 0;
};

// Compute Shader class
class PIXELTOASTER_API ComputeShader : public Shader {
public:
    virtual ~ComputeShader() = default;
    
    virtual bool setImage(const char* name, Framebuffer* framebuffer, int binding) = 0;
    virtual bool setBuffer(const char* name, void* buffer, size_t size, int binding) = 0;
};

// Framebuffer class
class PIXELTOASTER_API Framebuffer {
public:
    virtual ~Framebuffer() = default;
    
    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual Format format() const = 0;
    
    virtual bool readPixels(void* pixels, Format format) = 0;
    virtual bool writePixels(const void* pixels, Format format, const Rectangle* dirtyBox = nullptr) = 0;
    
    // GPU-specific framebuffer handle access
    virtual void* handle() const = 0;
};

// Dynamic Effects Engine classes
class PIXELTOASTER_API Animation {
public:
    virtual ~Animation() = default;
    
    enum Type {
        Transform,
        ColorGradient,
        ParticleSystem
    };
    
    virtual Type type() const = 0;
    virtual bool isPlaying() const = 0;
    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
    virtual void setTime(float time) = 0;
    virtual float time() const = 0;
    virtual void setLoop(bool loop) = 0;
    virtual bool loop() const = 0;
};

class PIXELTOASTER_API TransformAnimation : public Animation {
public:
    enum TransformType {
        Scale,
        Rotate,
        Translate,
        Skew
    };
    
    virtual ~TransformAnimation() = default;
    
    virtual Type type() const override { return Transform; }
    
    virtual void addKeyframe(float time, TransformType type, float value1, float value2 = 0.0f, float value3 = 0.0f) = 0;
    virtual void setTransformCenter(float x, float y) = 0;
    virtual void getTransform(float time, float& scaleX, float& scaleY, float& rotation, float& translateX, float& translateY, float& skewX, float& skewY) = 0;
};

class PIXELTOASTER_API ColorGradientAnimation : public Animation {
public:
    virtual ~ColorGradientAnimation() = default;
    
    virtual Type type() const override { return ColorGradient; }
    
    virtual void addKeyframe(float time, float r, float g, float b, float a = 1.0f) = 0;
    virtual void addKeyframeHSL(float time, float h, float s, float l, float a = 1.0f) = 0;
    virtual void getColor(float time, float& r, float& g, float& b, float& a) = 0;
};

class PIXELTOASTER_API Particle {
public:
    float x, y;
    float vx, vy;
    float r, g, b, a;
    float size;
    float life;
    float maxLife;
    
    Particle() : x(0), y(0), vx(0), vy(0), r(1), g(1), b(1), a(1), size(1), life(1), maxLife(1) {}
};

class PIXELTOASTER_API ParticleSystem : public Animation {
public:
    virtual ~ParticleSystem() = default;
    
    virtual Type type() const override { return ParticleSystem; }
    
    virtual void emit(int count, const Particle& particle) = 0;
    virtual void setEmitRate(float rate) = 0;
    virtual float emitRate() const = 0;
    virtual void setParticleLifetime(float min, float max) = 0;
    virtual void getParticleLifetime(float& min, float& max) const = 0;
    virtual int particleCount() const = 0;
    virtual const Particle* particles() const = 0;
};

class PIXELTOASTER_API Timeline {
public:
    virtual ~Timeline() = default;
    
    virtual void addAnimation(Animation* animation, float startTime = 0.0f) = 0;
    virtual void removeAnimation(Animation* animation) = 0;
    virtual void setTime(float time) = 0;
    virtual float time() const = 0;
    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
    virtual bool isPlaying() const = 0;
    virtual void setLoop(bool loop) = 0;
    virtual bool loop() const = 0;
};

// Performance monitoring
struct PIXELTOASTER_API PerformanceStats {
    float cpuTime;    // CPU rendering time in seconds
    float gpuTime;    // GPU rendering time in seconds
    float frameTime;  // Total frame time in seconds
    int   frameRate;  // Current frame rate
    
    // Memory usage
    size_t cpuMemory; // CPU memory used in bytes
    size_t gpuMemory; // GPU memory used in bytes
    
    // Pixel update statistics
    int   totalPixels;      // Total pixels in frame
    int   updatedPixels;    // Pixels updated in this frame
    float updateRatio;      // Ratio of updated pixels (0-1)
    
    PerformanceStats() : cpuTime(0), gpuTime(0), frameTime(0), frameRate(0),
                        cpuMemory(0), gpuMemory(0),
                        totalPixels(0), updatedPixels(0), updateRatio(0) {}
};

class PIXELTOASTER_API PerformanceMonitor {
public:
    virtual ~PerformanceMonitor() = default;
    
    virtual void beginFrame() = 0;
    virtual void endFrame() = 0;
    virtual void beginCPURender() = 0;
    virtual void endCPURender() = 0;
    virtual void beginGPURender() = 0;
    virtual void endGPURender() = 0;
    
    virtual const PerformanceStats& getStats() const = 0;
    virtual void reset() = 0;
};

// Factory functions
PIXELTOASTER_API Context* createGPUContext();
PIXELTOASTER_API Timeline* createTimeline();
PIXELTOASTER_API PerformanceMonitor* createPerformanceMonitor();
PIXELTOASTER_API TransformAnimation* createTransformAnimation();
PIXELTOASTER_API ColorGradientAnimation* createColorGradientAnimation();
PIXELTOASTER_API ParticleSystem* createParticleSystem();

} // namespace GPU
} // namespace PixelToaster

#endif // PIXELTOASTER_GPU_H