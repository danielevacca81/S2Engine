// RenderableComponent.cpp
//
#include "RenderableComponent.h"

using namespace s2;
using namespace s2::Scene;

// ------------------------------------------------------------------------------------------------
RenderableComponent::RenderableComponent( Renderer::MeshID meshID )
    : _meshID( meshID )
{
}

// ------------------------------------------------------------------------------------------------
void RenderableComponent::setMaterialProperty( const std::string& propertyName, const Renderer::MaterialDefinition::Property& value )
{
    _materialOverrides[propertyName] = value;
}

// ------------------------------------------------------------------------------------------------
const Renderer::MaterialDefinition::Property* RenderableComponent::getMaterialProperty( const std::string& propertyName ) const
{
    auto it = _materialOverrides.find( propertyName );
    if( it != _materialOverrides.end() )
        return &it->second;
    return nullptr;
}