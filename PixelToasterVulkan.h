// PixelToaster Vulkan Implementation
// Copyright (c) 2024
// Part of the PixelToaster Framebuffer Library

#ifndef PIXELTOASTER_VULKAN_H
#define PIXELTOASTER_VULKAN_H

#include "PixelToasterGPU.h"

#ifdef PIXELTOASTER_PLATFORM_WINDOWS
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>
#endif

#ifdef PIXELTOASTER_PLATFORM_LINUX
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_xlib.h>
#endif

#ifdef PIXELTOASTER_PLATFORM_APPLE
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_macos.h>
#endif

namespace PixelToaster {
namespace GPU {

class VulkanContext : public Context {
public:
    VulkanContext();
    ~VulkanContext() override;
    
    const char* deviceName() const override;
    bool isAvailable() const override;
    
    Shader* createShader(const char* vertexShader, const char* fragmentShader) override;
    ComputeShader* createComputeShader(const char* computeShader) override;
    void destroyShader(Shader* shader) override;
    void destroyComputeShader(ComputeShader* shader) override;
    
    Framebuffer* createFramebuffer(int width, int height, Format format) override;
    void destroyFramebuffer(Framebuffer* framebuffer) override;
    
    bool setShader(Shader* shader) override;
    bool setComputeShader(ComputeShader* shader) override;
    bool setFramebuffer(Framebuffer* framebuffer) override;
    
    void clear(float r, float g, float b, float a) override;
    void drawRectangle(float x, float y, float width, float height) override;
    void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3) override;
    
    void dispatchCompute(int x, int y, int z) override;
    
    void flush() override;
    void finish() override;
    
    // Vulkan-specific methods
    VkInstance instance() const;
    VkPhysicalDevice physicalDevice() const;
    VkDevice device() const;
    VkQueue graphicsQueue() const;
    VkQueue computeQueue() const;
    VkCommandPool commandPool() const;
    
private:
    bool initialize();
    bool createInstance();
    bool selectPhysicalDevice();
    bool createDevice();
    bool createCommandPool();
    
    VkInstance m_instance;
    VkPhysicalDevice m_physicalDevice;
    VkDevice m_device;
    
    uint32_t m_graphicsQueueFamilyIndex;
    uint32_t m_computeQueueFamilyIndex;
    
    VkQueue m_graphicsQueue;
    VkQueue m_computeQueue;
    
    VkCommandPool m_commandPool;
    VkCommandBuffer m_currentCommandBuffer;
    
    bool m_initialized;
    char m_deviceName[256];
};

class VulkanShader : public Shader {
public:
    VulkanShader(VulkanContext* context, const char* vertexShader, const char* fragmentShader);
    ~VulkanShader() override;
    
    bool setUniform(const char* name, float value) override;
    bool setUniform(const char* name, const float* values, int count) override;
    bool setUniform(const char* name, int value) override;
    bool setUniform(const char* name, const int* values, int count) override;
    
    VkPipeline pipeline() const;
    VkPipelineLayout pipelineLayout() const;
    
private:
    bool compileShader(const char* shaderCode, VkShaderStageFlagBits stage, VkShaderModule& module);
    bool createPipeline();
    
    VulkanContext* m_context;
    const char* m_vertexShader;
    const char* m_fragmentShader;
    
    VkShaderModule m_vertexModule;
    VkShaderModule m_fragmentModule;
    VkPipeline m_pipeline;
    VkPipelineLayout m_pipelineLayout;
    VkDescriptorSetLayout m_descriptorSetLayout;
};

class VulkanComputeShader : public ComputeShader {
public:
    VulkanComputeShader(VulkanContext* context, const char* computeShader);
    ~VulkanComputeShader() override;
    
    bool setUniform(const char* name, float value) override;
    bool setUniform(const char* name, const float* values, int count) override;
    bool setUniform(const char* name, int value) override;
    bool setUniform(const char* name, const int* values, int count) override;
    
    bool setImage(const char* name, Framebuffer* framebuffer, int binding) override;
    bool setBuffer(const char* name, void* buffer, size_t size, int binding) override;
    
    void dispatch(int x, int y, int z) override;
    
private:
    bool compileShader(const char* shaderCode, VkShaderModule& module);
    bool createPipeline();
    
    VulkanContext* m_context;
    const char* m_computeShader;
    
    VkShaderModule m_computeModule;
    VkPipeline m_pipeline;
    VkPipelineLayout m_pipelineLayout;
    VkDescriptorSetLayout m_descriptorSetLayout;
    VkDescriptorSet m_descriptorSet;
    
    std::vector<VkDescriptorBufferInfo> m_bufferInfos;
    std::vector<VkDescriptorImageInfo> m_imageInfos;
};

class VulkanFramebuffer : public Framebuffer {
public:
    VulkanFramebuffer(VulkanContext* context, int width, int height, Format format);
    ~VulkanFramebuffer() override;
    
    int width() const override;
    int height() const override;
    Format format() const override;
    
    bool readPixels(void* pixels, Format format) override;
    bool writePixels(const void* pixels, Format format, const Rectangle* dirtyBox = nullptr) override;
    
    void* handle() const override;
    
    VkImage image() const;
    VkImageView imageView() const;
    VkFramebuffer framebuffer() const;
    VkRenderPass renderPass() const;
    
private:
    bool createImage();
    bool createImageView();
    bool createFramebuffer();
    bool createRenderPass();
    
    VulkanContext* m_context;
    int m_width;
    int m_height;
    Format m_format;
    
    VkImage m_image;
    VkDeviceMemory m_imageMemory;
    VkImageView m_imageView;
    VkFramebuffer m_framebuffer;
    VkRenderPass m_renderPass;
    
    // Conversion helper
    VkFormat pixelToasterFormatToVulkanFormat(Format format);
};

} // namespace GPU
} // namespace PixelToaster

#endif // PIXELTOASTER_VULKAN_H