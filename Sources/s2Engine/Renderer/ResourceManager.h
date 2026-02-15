// ResourceManager.h
//
#ifndef S2_RENDERER_RESOURCEMANAGER_H
#define S2_RENDERER_RESOURCEMANAGER_H

#include "s2Engine_API.h"

#include "RenderCore/BufferObject.h"
#include "RenderCore/Texture.h"
#include "RenderCore/Shader.h"

#include <unordered_map>
#include <string>
#include <memory>

namespace s2 {
namespace Renderer {

// high-level resource manager for the renderer. Manages GPU resources like textures, buffers, shaders, and programs.
// allows loading resources from the application layer and caching them for efficient reuse.
class S2ENGINE_API ResourceManager
{
public:
    bool loadMesh( const std::string& name, const MeshData3D& meshData );
    bool loadTexture( const std::string& name, const RenderCore::TextureDescription& desc, const void* data );
    bool loadShader( const std::string& name, const  RenderCore::ShaderType& type, const std::string& source );

    RenderCore::BufferObjectPtr getMesh( const std::string& name );
    RenderCore::Texture2DPtr getTexture( const std::string& name );
    RenderCore::ShaderPtr getShader( const std::string& name );

private:
    // Cache per risorse GPU
    std::unordered_map<std::string, RenderCore::BufferObjectPtr> _meshCache;
    std::unordered_map<std::string, RenderCore::Texture2DPtr> _textureCache;
    std::unordered_map<std::string, RenderCore::ShaderPtr> _shaderCache;
};

} // namespace Renderer
} // namespace s2
#endif // !S2_RENDERER_RESOURCEMANAGER_H