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

// ------------------------------------------------------------------------------------------------
// Typed resource handle — compile-time distinction between Mesh, Texture, Shader IDs.
// ------------------------------------------------------------------------------------------------
template<typename Tag>
struct TypedResourceID
{
    uint64_t handle { 0 };

    bool isValid() const { return handle != 0; }
    explicit operator bool() const { return isValid(); }

    bool operator==( const TypedResourceID& o ) const { return handle == o.handle; }
    bool operator!=( const TypedResourceID& o ) const { return handle != o.handle; }
    bool operator< ( const TypedResourceID& o ) const { return handle <  o.handle; }
};

struct MeshTag {};
struct TextureTag {};
struct ShaderTag {};

using MeshID    = TypedResourceID<MeshTag>;
using TextureID = TypedResourceID<TextureTag>;
using ShaderID  = TypedResourceID<ShaderTag>;

static constexpr MeshID    MeshInvalidID {};
static constexpr TextureID TextureInvalidID {};
static constexpr ShaderID  ShaderInvalidID {};

// high-level resource manager for the renderer. Manages GPU resources like textures, buffers, shaders, and programs.
// allows loading resources from the application layer and caching them for efficient reuse.
class S2ENGINE_API ResourceManager
{
public:
    MeshID    registerMesh( const std::string& name,    const RenderCore::VertexDataPtr &mesh );
    MeshID    registerMesh( const std::string& name,    const MeshData3D &mesh );
    //MeshID  registerMesh( const std::string& name,    const MeshData2D &mesh );
    TextureID registerTexture( const std::string& name, const RenderCore::Texture2DPtr  &texture );
	TextureID registerTexture( const std::string& name, const std::filesystem::path &path );
    ShaderID  registerShader( const std::string& name,  const RenderCore::ShaderPtr    &shader );

    RenderCore::VertexDataPtr mesh( MeshID id ) const;
	MeshID                    mesh( const std::string& name ) const;

    RenderCore::Texture2DPtr  texture( TextureID id ) const;
    TextureID                 texture( const std::string& name ) const;

    RenderCore::ShaderPtr shader( ShaderID id ) const;
    ShaderID              shader( const std::string& name ) const;


private:
    std::atomic<uint64_t> _nextHandle { 1 };

    std::unordered_map<std::string, uint64_t>                 _nameToHandle;
    std::unordered_map<uint64_t, RenderCore::VertexDataPtr>   _meshes;
    std::unordered_map<uint64_t, RenderCore::Texture2DPtr>    _textures;
    std::unordered_map<uint64_t, RenderCore::ShaderPtr>       _shaders;
};

} // namespace Renderer
} // namespace s2

// std::hash specialization for TypedResourceID
template<typename Tag>
struct std::hash<s2::Renderer::TypedResourceID<Tag>>
{
    size_t operator()( const s2::Renderer::TypedResourceID<Tag>& id ) const noexcept
    {
        return std::hash<uint64_t>{}( id.handle );
    }
};

#endif // !S2_RENDERER_RESOURCEMANAGER_H