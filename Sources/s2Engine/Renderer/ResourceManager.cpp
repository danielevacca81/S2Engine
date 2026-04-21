// ResourceManager.cpp
//
#include "ResourceManager.h"

#include "Resources/ImageLoader.h"

using namespace s2::Renderer;


// ------------------------------------------------------------------------------------------------
ResourceID ResourceManager::registerMesh( const std::string& name, const MeshData3D& mesh )
{
    auto it = _nameToHandle.find( name );
    if( it != _nameToHandle.end() )
        return it->second;

    ResourceID handle = _nextHandle++;
    _nameToHandle[name] = handle;
    _meshes[handle] = RenderCore::VertexData::New( mesh );
    return handle;
}


// ------------------------------------------------------------------------------------------------
//ResourceID ResourceManager::registerMesh( const std::string& name, const MeshData2D& mesh )
//{
//    auto it = _nameToHandle.find( name );
//    if( it != _nameToHandle.end() )
//        return it->second;
//
//	auto vtx = RenderCore::VertexData::New();	
//    vtx->setVertices     ( vector_cast<Math::dvec3,Math::vec2>( mesh.vertices ) );
//	vtx->setNormals      ( vector_cast<Math::dvec3,Math::vec3>( mesh.normals ) );
//	vtx->setTextureCoords( vector_cast<Math::dvec2,Math::vec2>( mesh.uvCoords ) );
//    vtx->setIndices      ( mesh.indices );
//
//    ResourceID handle = _nextHandle++;
//    _nameToHandle[name] = handle;
//    _meshes[handle] = vtx;
//}

// ------------------------------------------------------------------------------------------------
ResourceID ResourceManager::registerMesh( const std::string& name, const RenderCore::VertexDataPtr &mesh )
{
    auto it = _nameToHandle.find( name );
    if( it != _nameToHandle.end() )
        return it->second;

    ResourceID handle = _nextHandle++;
    _nameToHandle[name] = handle;
    _meshes[handle] = mesh;
    return handle;
}

// ------------------------------------------------------------------------------------------------
ResourceID ResourceManager::registerTexture( const std::string& name, const RenderCore::Texture2DPtr& texture )
{
    auto it = _nameToHandle.find( name );
    if( it != _nameToHandle.end() )
        return it->second;
    
    ResourceID handle = _nextHandle++;
    _nameToHandle[name] = handle;
    _textures[handle] = texture;
	return handle;
}

// ------------------------------------------------------------------------------------------------
ResourceID ResourceManager::registerTexture( const std::string& name, const std::filesystem::path& filePath)
{
    // Check if already loaded
    auto it = _nameToHandle.find( name );
    if( it != _nameToHandle.end() )
        return it->second;

    // Load image from file
    auto imageData = Resources::ImageLoader::loadFromFile( filePath.string() );
    if( !imageData || imageData->pixmap.isEmpty() )
        return ResourceInvalidID;

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
ResourceID ResourceManager::registerShader( const std::string& name, const RenderCore::ShaderPtr& shader )
{
    auto it = _nameToHandle.find( name );
    if( it != _nameToHandle.end() )
        return it->second;
    
    ResourceID handle = _nextHandle++;
    _nameToHandle[name] = handle;
    _shaders[handle] = shader;
	return handle;
}

// ------------------------------------------------------------------------------------------------
RenderCore::VertexDataPtr ResourceManager::mesh( const ResourceID &handle ) const
{
    auto it = _meshes.find( handle );
    return ( it != _meshes.end() ) ? it->second : nullptr;
}

// ------------------------------------------------------------------------------------------------
ResourceID ResourceManager::mesh( const std::string& name ) const
{
	auto it = _nameToHandle.find( name );
	return it == _nameToHandle.end() ? ResourceInvalidID : it->second;
}

// ------------------------------------------------------------------------------------------------
RenderCore::Texture2DPtr ResourceManager::texture( const ResourceID& handle ) const
{
    auto it = _textures.find( handle );
	return ( it != _textures.end() ) ? it->second : nullptr;
}

// ------------------------------------------------------------------------------------------------
ResourceID ResourceManager::texture( const std::string &name ) const
{
    auto it = _nameToHandle.find( name );
    return it == _nameToHandle.end() ? ResourceInvalidID : it->second;
}

// ------------------------------------------------------------------------------------------------
RenderCore::ShaderPtr ResourceManager::shader( const ResourceID& handle ) const
{
    auto it = _shaders.find( handle );
	return ( it != _shaders.end() ) ? it->second : nullptr;
}

// ------------------------------------------------------------------------------------------------
ResourceID ResourceManager::shader( const std::string& name ) const
{
    auto it = _nameToHandle.find( name );
    return it == _nameToHandle.end() ? ResourceInvalidID : it->second;
}