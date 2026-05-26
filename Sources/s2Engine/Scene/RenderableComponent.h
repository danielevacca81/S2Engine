// RenderableComponent.h
//
#ifndef S2_SCENE_RENDERABLECOMPONENT_H
#define S2_SCENE_RENDERABLECOMPONENT_H

#include "s2Engine_API.h"
#include "Component.h"

#include "Renderer/ResourceManager.h"
#include "Renderer/RenderMaterial.h"

#include <unordered_map>
#include <string>

namespace s2 {
namespace Scene {

class S2ENGINE_API RenderableComponent : public Component
{
public:
    RenderableComponent() = default;
    RenderableComponent( Renderer::MeshID meshID );

    void setMesh( Renderer::MeshID meshID ) { _meshID = meshID; }
    Renderer::MeshID meshID() const { return _meshID; }

    // Overrides default material properties
    void setMaterialProperty( const std::string& propertyName, const Renderer::MaterialDefinition::Property& value );
    const Renderer::MaterialDefinition::Property* getMaterialProperty( const std::string& propertyName ) const;

    bool isCastingShadow() const { return _castShadow; }
    void setCastShadow( bool cast ) { _castShadow = cast; }

    bool isVisible() const { return _visible; }
    void setVisible( bool visible ) { _visible = visible; }

private:
    Renderer::MeshID _meshID;
    
    bool _visible = true;
    bool _castShadow = true;

    std::unordered_map<std::string, Renderer::MaterialDefinition::Property> _materialOverrides;
};

} // namespace Scene
} // namespace s2

#endif // !S2_SCENE_RENDERABLECOMPONENT_H
