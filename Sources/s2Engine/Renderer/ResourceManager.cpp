// ResourceManager.cpp
//
#include "ResourceManager.h"

#include "Resources/ImageLoader.h"

using namespace s2;
using namespace s2::Renderer;

// ------------------------------------------------------------------------------------------------
MeshID ResourceManager::registerMesh( const std::string& name, const MeshData3D& mesh )
{
    auto it = _nameToHandle.find( name );
    if( it != _nameToHandle.end() )
        return MeshID{ it->second };

    uint64_t handle = _nextHandle++;
    _nameToHandle[name] = handle;
    _meshes[handle] = RenderCore::VertexData::New( mesh );
    return MeshID{ handle };
}


// ------------------------------------------------------------------------------------------------
//MeshID ResourceManager::registerMesh( const std::string& name, const MeshData2D& mesh )
//{
//    auto it = _nameToHandle.find( name );
//    if( it != _nameToHandle.end() )
//        return MeshID{ it->second };
//
//	auto vtx = RenderCore::VertexData::New();	
//    vtx->setVertices     ( vector_cast<Math::dvec3,Math::vec2>( mesh.vertices ) );
//	vtx->setNormals      ( vector_cast<Math::dvec3,Math::vec3>( mesh.normals ) );
//	vtx->setTextureCoords( vector_cast<Math::dvec2,Math::vec2>( mesh.uvCoords ) );
//    vtx->setIndices      ( mesh.indices );
//
//    uint64_t handle = _nextHandle++;
//    _nameToHandle[name] = handle;
//    _meshes[handle] = vtx;
//}

// ------------------------------------------------------------------------------------------------
MeshID ResourceManager::registerMesh( const std::string& name, const RenderCore::VertexDataPtr &mesh )
{
    auto it = _nameToHandle.find( name );
    if( it != _nameToHandle.end() )
        return MeshID{ it->second };

    uint64_t handle = _nextHandle++;
    _nameToHandle[name] = handle;
    _meshes[handle] = mesh;
    return MeshID{ handle };
}

// ------------------------------------------------------------------------------------------------
TextureID ResourceManager::registerTexture( const std::string& name, const RenderCore::Texture2DPtr& texture )
{
    auto it = _nameToHandle.find( name );
    if( it != _nameToHandle.end() )
        return TextureID{ it->second };
    
    uint64_t handle = _nextHandle++;
    _nameToHandle[name] = handle;
    _textures[handle] = texture;
	return TextureID{ handle };
}

// ------------------------------------------------------------------------------------------------
TextureID ResourceManager::registerTexture( const std::string& name, const std::filesystem::path& filePath)
{
    // Check if already loaded
    auto it = _nameToHandle.find( name );
    if( it != _nameToHandle.end() )
        return TextureID{ it->second };

    // Load image from file
    auto imageData = Resources::ImageLoader::loadFromFile( filePath.string() );
    if( !imageData || imageData->pixmap.isEmpty() )
        return TextureInvalidID;

    // Determine texture format based on channels
    RenderCore::TextureFormat format = RenderCore::TextureFormat::RedGreenBlueAlpha8;
    switch( imageData->pixmap.numChannels() )
    {
    case 1: format = RenderCore::TextureFormat::Red8; break;
    case 3: format = RenderCore::TextureFormat::RedGreenBlue8; break;
    case 4: format = RenderCore::TextureFormat::RedGreenBlueAlpha8; break;
    }

    // Create and register texture
    auto texture = RenderCore::Texture2D::New(
        RenderCore::TextureDescription(
        imageData->pixmap.width(),
        imageData->pixmap.height(),
        format ),
        (void*) imageData->pixmap.pixels() );

    return registerTexture( name, texture );
}

// ------------------------------------------------------------------------------------------------
ShaderID ResourceManager::registerShader( const std::string& name, const RenderCore::ShaderPtr& shader )
{
    auto it = _nameToHandle.find( name );
    if( it != _nameToHandle.end() )
        return ShaderID{ it->second };
    
    uint64_t handle = _nextHandle++;
    _nameToHandle[name] = handle;
    _shaders[handle] = shader;
	return ShaderID{ handle };
}

// ------------------------------------------------------------------------------------------------
RenderCore::VertexDataPtr ResourceManager::mesh( MeshID id ) const
{
    auto it = _meshes.find( id.handle );
    return ( it != _meshes.end() ) ? it->second : nullptr;
}

// ------------------------------------------------------------------------------------------------
MeshID ResourceManager::mesh( const std::string& name ) const
{
	auto it = _nameToHandle.find( name );
	return it == _nameToHandle.end() ? MeshInvalidID : MeshID{ it->second };
}

// ------------------------------------------------------------------------------------------------
RenderCore::Texture2DPtr ResourceManager::texture( TextureID id ) const
{
    auto it = _textures.find( id.handle );
	return ( it != _textures.end() ) ? it->second : nullptr;
}

// ------------------------------------------------------------------------------------------------
TextureID ResourceManager::texture( const std::string &name ) const
{
    auto it = _nameToHandle.find( name );
    return it == _nameToHandle.end() ? TextureInvalidID : TextureID{ it->second };
}

// ------------------------------------------------------------------------------------------------
RenderCore::ShaderPtr ResourceManager::shader( ShaderID id ) const
{
    auto it = _shaders.find( id.handle );
	return ( it != _shaders.end() ) ? it->second : nullptr;
}

// ------------------------------------------------------------------------------------------------
ShaderID ResourceManager::shader( const std::string& name ) const
{
    auto it = _nameToHandle.find( name );
    return it == _nameToHandle.end() ? ShaderInvalidID : ShaderID{ it->second };
}