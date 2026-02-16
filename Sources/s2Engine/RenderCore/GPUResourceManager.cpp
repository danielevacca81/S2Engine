// GPUResourceManager.cpp
//
#include "GPUResourceManager.h"
#include "Program.h"
#include "ShaderCompiler.h"

namespace s2 {
namespace RenderCore {

// ------------------------------------------------------------------------------------------------
GPUResourceManager& GPUResourceManager::instance()
{
    static GPUResourceManager s_instance;
    return s_instance;
}

// ------------------------------------------------------------------------------------------------
GPUResourceManager::GPUResourceManager() = default;

// ------------------------------------------------------------------------------------------------
GPUResourceManager::~GPUResourceManager()
{
    clear();
}

// ------------------------------------------------------------------------------------------------
Texture2DPtr GPUResourceManager::getOrLoadTexture(
    const std::string& name,
    std::function<TextureDescription()> descriptionLoader,
    std::function<void*()> dataLoader
)
{
    // Check cache
    auto it = _textureCache.find(name);
    if (it != _textureCache.end()) {
        return it->second;
    }

    // Load from RAM
    TextureDescription desc = descriptionLoader();
    void* data = dataLoader ? dataLoader() : nullptr;

    // Upload to GPU
    auto texture = Texture2D::New(desc, data);
    _textureCache[name] = texture;

    // Free RAM data if loader allocated with malloc
    // (depends on who loads — ideally the loader cleans up)

    return texture;
}

// ------------------------------------------------------------------------------------------------
Texture2DPtr GPUResourceManager::getTexture(const std::string& name)
{
    auto it = _textureCache.find(name);
    return (it != _textureCache.end()) ? it->second : nullptr;
}

// ------------------------------------------------------------------------------------------------
bool GPUResourceManager::hasTexture(const std::string& name) const
{
    return _textureCache.find(name) != _textureCache.end();
}

// ------------------------------------------------------------------------------------------------
void GPUResourceManager::unloadTexture(const std::string& name)
{
    _textureCache.erase(name);
}

// ------------------------------------------------------------------------------------------------
BufferObjectPtr GPUResourceManager::getOrLoadBuffer(
    const std::string& name,
    int sizeBytes,
    const BufferObject::Type& type,
    const BufferObject::UsageHint& hint,
    void* data
)
{
    auto it = _bufferCache.find(name);
    if (it != _bufferCache.end())
        return it->second;

    auto buffer = BufferObject::New(sizeBytes, type, hint);
    if (data)
        buffer->sendData(data, sizeBytes);

    _bufferCache[name] = buffer;
    return buffer;
}

// ------------------------------------------------------------------------------------------------
BufferObjectPtr GPUResourceManager::getBuffer(const std::string& name)
{
    auto it = _bufferCache.find(name);
    return (it != _bufferCache.end()) ? it->second : nullptr;
}

// ------------------------------------------------------------------------------------------------
bool GPUResourceManager::hasBuffer(const std::string& name) const
{
    return _bufferCache.find(name) != _bufferCache.end();
}

// ------------------------------------------------------------------------------------------------
void GPUResourceManager::unloadBuffer(const std::string& name)
{
    _bufferCache.erase(name);
}


// ------------------------------------------------------------------------------------------------
ShaderPtr GPUResourceManager::getOrLoadShader(
    const std::string& name,
    const ShaderType& type,
    std::function<std::string()> sourceLoader
)
{
    auto it = _shaderCache.find(name);
    if (it != _shaderCache.end()) {
        return it->second;
    }

    const std::string source = sourceLoader();
    auto result = ShaderCompiler::compile(type, source);

    if( !result ) 
    {
        // Log error
        return nullptr;
    }

    _shaderCache[name] = result.shader;
    return result.shader;
}

// ------------------------------------------------------------------------------------------------
ShaderPtr GPUResourceManager::getShader(const std::string& name)
{
    auto it = _shaderCache.find(name);
    return (it != _shaderCache.end()) ? it->second : nullptr;
}

// ------------------------------------------------------------------------------------------------
bool GPUResourceManager::hasShader(const std::string& name) const
{
    return _shaderCache.find(name) != _shaderCache.end();
}

// ------------------------------------------------------------------------------------------------
void GPUResourceManager::unloadShader(const std::string& name)
{
    _shaderCache.erase(name);
}

// ------------------------------------------------------------------------------------------------
ProgramPtr GPUResourceManager::getOrLoadProgram(
    const std::string& name,
    const std::vector<ShaderPtr>& shaders
)
{
    auto it = _programCache.find(name);
    if (it != _programCache.end()) {
        return it->second;
    }

    auto program = Program::New();
    for (const auto& shader : shaders) 
    {
        switch ( shader->type() )
        {
        case ShaderType::Vertex:                 program->attachVertexShader(shader);    break;
        case ShaderType::Fragment:               program->attachFragmentShader(shader);  break;
        case ShaderType::Geometry:               program->attachGeometryShader(shader);  break;
        case ShaderType::Compute:                program->attachComputeShader(shader);   break;
        case ShaderType::TessellationControl:    program->attachTessellationControlShader(shader); break;
        case ShaderType::TessellationEvaluation: program->attachTessellationEvaluationShader(shader); break;
        default: break;            
        }
    }
    auto result = ShaderCompiler::linkProgram(program);
    if( !result ) 
    {
        // Log error
        return nullptr;
    }

    _programCache[name] = program;
    return program;
}

// ------------------------------------------------------------------------------------------------
ProgramPtr GPUResourceManager::getProgram(const std::string& name)
{
    auto it = _programCache.find(name);
    return (it != _programCache.end()) ? it->second : nullptr;
}

// ------------------------------------------------------------------------------------------------
bool GPUResourceManager::hasProgram(const std::string& name) const
{
    return _programCache.find(name) != _programCache.end();
}

// ------------------------------------------------------------------------------------------------
void GPUResourceManager::unloadProgram(const std::string& name)
{
    _programCache.erase(name);
}

// ------------------------------------------------------------------------------------------------
void GPUResourceManager::collectGarbage()
{
    auto clearContainer = [](auto& cache) 
    {
        for (auto it = cache.begin(); it != cache.end(); ) 
        {
            if (it->second.use_count() == 1)   // Only referenced by map
                it = cache.erase(it);
            else
                ++it;
        }
    };

    clearContainer(_textureCache);
    clearContainer(_bufferCache);
    clearContainer(_shaderCache);
    clearContainer(_programCache);
}

// ------------------------------------------------------------------------------------------------
void GPUResourceManager::clear()
{
    _textureCache.clear();
    _bufferCache.clear();
    _shaderCache.clear();
    _programCache.clear();
}

// ------------------------------------------------------------------------------------------------
    GPUResourceManager::MemoryStats GPUResourceManager::getMemoryStats() const
{
    MemoryStats stats;
    stats.cachedResourceCount = _textureCache.size() + _bufferCache.size() + 
                                _shaderCache.size() + _programCache.size();
    // TODO: Accurate memory measurement
    return stats;
}

std::vector<GPUResourceManager::ResourceStats> GPUResourceManager::getResourceStats() const
{
    std::vector<ResourceStats> stats;
    
    for (const auto& [name, tex] : _textureCache)
     {
        ResourceStats s;
        s.type = ResourceType::Texture;
        s.name = name;
        s.refCount = static_cast<int>(tex.use_count());
        s.isCached = true;
        // s.sizeBytes = tex->estimateMemoryUsage(); // Future
        stats.push_back(s);
    }

    return stats;
}

// ------------------------------------------------------------------------------------------------
void GPUResourceManager::printStats() const
{
    auto stats = getMemoryStats();
    // TODO: Print formatted stats
}

} // namespace RenderCore
} // namespace s2