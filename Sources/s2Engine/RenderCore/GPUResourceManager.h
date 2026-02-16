// GPUResourceManager.h
//
#ifndef S2_RENDERCORE_GPURESOURCEMANAGER_H
#define S2_RENDERCORE_GPURESOURCEMANAGER_H

#include "s2Engine_API.h"

#include "BufferObject.h"
#include "Texture.h"
#include "Shader.h"
#include "Program.h"

#include <memory>
#include <unordered_map>
#include <typeinfo>
#include <functional>

namespace s2 {
    namespace RenderCore {
        
// Manages GPU resources (textures, buffers, shaders, programs) with caching and reference counting.
//
// Resource lifecycle: Load → Upload → Render → Unload
// GPU is a limited and expensive resource.
// This manager:
// 1. Caches resources to avoid duplicate uploads
// 2. Automatically tracks reference counts (shared_ptr)
// 3. Supports asynchronous unload when ref count returns to 0
// 4. Provides GPU memory consumption statistics
class S2ENGINE_API GPUResourceManager
{
public:
    enum class ResourceType
    {
        Texture,
        VertexBuffer,
        IndexBuffer,
        Shader,
        Program,
    };

    struct ResourceStats
    {
        ResourceType type;
        size_t sizeBytes = 0;
        int refCount = 0;
        std::string name;
        bool isCached = false;
    };

    struct MemoryStats
    {
        size_t totalTextureMemory = 0;
        size_t totalBufferMemory = 0;
        size_t totalShaderMemory = 0;
        size_t cachedResourceCount = 0;
        
        size_t getTotalMemory() const 
        { 
            return totalTextureMemory + totalBufferMemory + totalShaderMemory; 
        }
    };

public:
    // Singleton instance
    static GPUResourceManager& instance();

    ~GPUResourceManager();

    // ---- TEXTURE CACHE ----
    /**
     * Load or retrieve a texture from cache
     * @param name Unique identifier
     * @param descriptionLoader Function that loads description data
     * @param dataLoader Function that loads pixel data (optional)
     * @return Texture2D shared_ptr already uploaded to GPU
     */
    Texture2DPtr getOrLoadTexture(
        const std::string& name,
        std::function<TextureDescription()> descriptionLoader,
        std::function<void*()> dataLoader = nullptr
    );

    Texture2DPtr getTexture(const std::string& name);
    bool hasTexture(const std::string& name) const;
    void unloadTexture(const std::string& name);

    // ---- BUFFER CACHE ----
    BufferObjectPtr getOrLoadBuffer(
        const std::string& name,
        int sizeBytes,
        const BufferObject::Type& type,
        const BufferObject::UsageHint& hint,
        void* data = nullptr
    );

    BufferObjectPtr getBuffer(const std::string& name);
    bool hasBuffer(const std::string& name) const;
    void unloadBuffer(const std::string& name);

    // ---- SHADER CACHE ----
    ShaderPtr getOrLoadShader(
        const std::string& name,
        const ShaderType& type,
        std::function<std::string()> sourceLoader
    );

    ShaderPtr getShader(const std::string& name);
    bool hasShader(const std::string& name) const;
    void unloadShader(const std::string& name);

    // ---- PROGRAM CACHE ----
    ProgramPtr getOrLoadProgram(
        const std::string& name,
        const std::vector<ShaderPtr>& shaders
    );

    ProgramPtr getProgram(const std::string& name);
    bool hasProgram(const std::string& name) const;
    void unloadProgram(const std::string& name);

    // ---- MEMORY MANAGEMENT ----
    /**
     * Garbage collection: releases resources with refCount == 1
     * (only referenced by the manager)
     */
    void collectGarbage();

    /**
     * Force unload of all resources
     * Useful for cleanup before shutdown
     */
    void clear();

    // ---- STATISTICS ----
    MemoryStats getMemoryStats() const;
    std::vector<ResourceStats> getResourceStats() const;
    void printStats() const;

private:
    GPUResourceManager();

    // Non-copyable
    GPUResourceManager(const GPUResourceManager&) = delete;
    GPUResourceManager& operator=(const GPUResourceManager&) = delete;

private:
    // Three-level maps: name → shared_ptr
    std::unordered_map<std::string, Texture2DPtr>    _textureCache;
    std::unordered_map<std::string, BufferObjectPtr> _bufferCache;
    std::unordered_map<std::string, ShaderPtr>       _shaderCache;
    std::unordered_map<std::string, ProgramPtr>      _programCache;

    // Thread-safe access (future improvement)
    // std::shared_mutex mCacheMutex;
};

} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_GPURESOURCEMANAGER_H