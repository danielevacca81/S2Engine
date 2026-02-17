// ResourceManager.cpp
//
#include "ResourceManager.h"

using namespace s2::Renderer;

// ------------------------------------------------------------------------------------------------
ResourceHandle ResourceManager::registerMesh( const std::string& name, const RenderCore::VertexDataPtr &mesh )
{
    auto it = _nameToHandle.find( name );
    if( it != _nameToHandle.end() )
        return it->second;

    ResourceHandle handle = _nextHandle++;
    _nameToHandle[name] = handle;
    _meshes[handle] = mesh;
    return handle;
}

// ------------------------------------------------------------------------------------------------
ResourceHandle ResourceManager::registerTexture( const std::string& name, const RenderCore::Texture2DPtr& texture )
{
    auto it = _nameToHandle.find( name );
    if( it != _nameToHandle.end() )
        return it->second;
    
    ResourceHandle handle = _nextHandle++;
    _nameToHandle[name] = handle;
    _textures[handle] = texture;
	return handle;
}

// ------------------------------------------------------------------------------------------------
ResourceHandle ResourceManager::registerShader( const std::string& name, const RenderCore::ProgramPtr& shader )
{
    auto it = _nameToHandle.find( name );
    if( it != _nameToHandle.end() )
        return it->second;
    
    ResourceHandle handle = _nextHandle++;
    _nameToHandle[name] = handle;
    _shaders[handle] = shader;
	return handle;
}

// ------------------------------------------------------------------------------------------------
RenderCore::VertexDataPtr ResourceManager::getMesh( const ResourceHandle &handle ) const
{
    auto it = _meshes.find( handle );
    return ( it != _meshes.end() ) ? it->second : nullptr;
}

// ------------------------------------------------------------------------------------------------
RenderCore::Texture2DPtr  ResourceManager::getTexture( const ResourceHandle& handle ) const
{
    auto it = _textures.find( handle );
	return ( it != _textures.end() ) ? it->second : nullptr;
}

// ------------------------------------------------------------------------------------------------
RenderCore::ProgramPtr ResourceManager::getShader( const ResourceHandle& handle ) const
{
    auto it = _shaders.find( handle );
	return ( it != _shaders.end() ) ? it->second : nullptr;
}

