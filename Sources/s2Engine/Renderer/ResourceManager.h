// ResourceManager.h
//
#ifndef S2_RENDERER_RESOURCEMANAGER_H
#define S2_RENDERER_RESOURCEMANAGER_H

#include "s2Engine_API.h"


#include "Geometry/MeshData.h"

#include "RenderCore/VertexData.h"
#include "RenderCore/Texture.h"
#include "RenderCore/Shader.h"

#include <unordered_map>
#include <string>
#include <filesystem>


namespace s2 {
namespace Renderer {

using ResourceID = uint64_t;

static constexpr ResourceID ResourceInvalidID = 0;
// static constexpr ResourceID TransientHandleStart = 1ull << 63; // Handles >= this value are transient and valid only for the current frame

// high-level resource manager for the renderer. Manages GPU resources like textures, buffers, shaders, and programs.
// allows loading resources from the application layer and caching them for efficient reuse.
class S2ENGINE_API ResourceManager
{
public:
    ResourceID registerMesh( const std::string& name,    const RenderCore::VertexDataPtr &mesh );
    ResourceID registerMesh( const std::string& name,    const MeshData3D &mesh );
    //ResourceID registerMesh( const std::string& name,    const MeshData2D &mesh );
    ResourceID registerTexture( const std::string& name, const RenderCore::Texture2DPtr  &texture );
	ResourceID registerTexture( const std::string& name, const std::filesystem::path &path );
    ResourceID registerShader( const std::string& name,  const RenderCore::ShaderPtr    &shader );

    RenderCore::VertexDataPtr mesh( const ResourceID &handle ) const;
	ResourceID                mesh( const std::string& name ) const;

    RenderCore::Texture2DPtr  texture( const ResourceID &handle ) const;
    ResourceID                texture( const std::string& name ) const;

    RenderCore::ShaderPtr shader( const ResourceID &handle ) const;
    ResourceID            shader( const std::string& name ) const;


private:
    std::atomic<uint64_t> _nextHandle { 1 };

    std::unordered_map<std::string, ResourceID> _nameToHandle;
    std::unordered_map<ResourceID, RenderCore::VertexDataPtr> _meshes;
    std::unordered_map<ResourceID, RenderCore::Texture2DPtr>  _textures;
    std::unordered_map<ResourceID, RenderCore::ShaderPtr>     _shaders;
};

} // namespace Renderer
} // namespace s2
#endif // !S2_RENDERER_RESOURCEMANAGER_H