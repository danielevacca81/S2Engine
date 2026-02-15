// ResourceManager.cpp
//
#include "ResourceManager.h"

using namespace s2::Renderer;

// ------------------------------------------------------------------------------------------------
bool ResourceManager::loadMesh( const std::string& name, const MeshData3D& meshData )
{
    // Check if mesh is already cached
    if ( _meshCache.find( name ) != _meshCache.end() )
    {
        return true; // Already loaded
    }

    // Create and load mesh buffer
    RenderCore::BufferObjectPtr meshBuffer = std::make_shared<RenderCore::BufferObject>();
    if ( !meshBuffer || !meshBuffer->create( meshData ) )
    {
        return false;
    }

    // Cache the mesh
    _meshCache[ name ] = meshBuffer;
    return true;
}

// ------------------------------------------------------------------------------------------------
bool ResourceManager::loadTexture( const std::string& name, const RenderCore::TextureDescription& desc, const void* data )
{
    // Check if texture is already cached
    if ( _textureCache.find( name ) != _textureCache.end() )
    {
        return true; // Already loaded
    }

    // Create and load texture
    RenderCore::Texture2DPtr texture = std::make_shared<RenderCore::Texture2D>();
    if ( !texture || !texture->create( desc, data ) )
    {
        return false;
    }

    // Cache the texture
    _textureCache[ name ] = texture;
    return true;
}

// ------------------------------------------------------------------------------------------------
bool ResourceManager::loadShader( const std::string& name, const RenderCore::ShaderType& type, const std::string& source )
{
    // Check if shader is already cached
    if ( _shaderCache.find( name ) != _shaderCache.end() )
    {
        return true; // Already loaded
    }

    // Create and compile shader
    RenderCore::ShaderPtr shader = std::make_shared<RenderCore::Shader>();
    if ( !shader || !shader->compile( type, source ) )
    {
        return false;
    }

    // Cache the shader
    _shaderCache[ name ] = shader;
    return true;
}

} // namespace Renderer
} // namespace s2