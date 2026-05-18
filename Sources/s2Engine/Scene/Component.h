// Component.h
//
#ifndef S2_SCENE_COMPONENT_H
#define S2_SCENE_COMPONENT_H

#include "s2Engine_API.h"
#include <memory>

namespace s2 {
namespace Scene {

class Entity;

// Base class for all components
class S2ENGINE_API Component
{
public:
    virtual ~Component() = default;

    Entity* owner() const { return _owner; }

    virtual void onAttach() {}    // Called when attached to entity
    virtual void onDetach() {}    // Called when detached from entity

protected:
    friend class Entity;
    Entity* _owner = nullptr;

private:
    void setOwner( Entity* entity ) { _owner = entity; }
};

using ComponentPtr = std::shared_ptr<Component>;

} // namespace Scene
} // namespace s2

#endif // !S2_SCENE_COMPONENT_H
