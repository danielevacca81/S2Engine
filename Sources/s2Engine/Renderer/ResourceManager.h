// ResourceManager.h
//
#ifndef S2_RENDERER_RESOURCEMANAGER_H
#define S2_RENDERER_RESOURCEMANAGER_H

#include "s2Engine_API.h"

#include "RenderCore/VertexData.h"
#include "RenderCore/Texture.h"
#include "RenderCore/Program.h"

#include <unordered_map>
#include <string>
#include <memory>

namespace s2 {
namespace Renderer {

using ResourceHandle = uint64_t;

static constexpr ResourceHandle InvalidHandle = 0;
// static constexpr ResourceHandle TransientHandleStart = 1ull << 63; // Handles >= this value are transient and valid only for the current frame

// high-level resource manager for the renderer. Manages GPU resources like textures, buffers, shaders, and programs.
// allows loading resources from the application layer and caching them for efficient reuse.
class S2ENGINE_API ResourceManager
{
public:
    ResourceHandle registerMesh( const std::string& name,    const RenderCore::VertexDataPtr &mesh );
    ResourceHandle registerTexture( const std::string& name, const RenderCore::Texture2DPtr  &texture );
    ResourceHandle registerShader( const std::string& name,  const RenderCore::ProgramPtr    &shader );

    RenderCore::VertexDataPtr getMesh( const ResourceHandle &handle ) const;
    RenderCore::Texture2DPtr  getTexture( const ResourceHandle &handle ) const;
    RenderCore::ProgramPtr    getShader( const ResourceHandle &handle ) const;


private:
    std::atomic<uint64_t> _nextHandle { 1 };

    std::unordered_map<std::string, ResourceHandle> _nameToHandle;
    std::unordered_map<ResourceHandle, RenderCore::VertexDataPtr> _meshes;
    std::unordered_map<ResourceHandle, RenderCore::Texture2DPtr> _textures;
    std::unordered_map<ResourceHandle, RenderCore::ProgramPtr> _shaders;
};

} // namespace Renderer
} // namespace s2
#endif // !S2_RENDERER_RESOURCEMANAGER_H