// Properties.h
//
#ifndef S2_ECS_PROPERTIES
#define S2_ECS_PROPERTIES

#include "Entity.h"

namespace s2{
namespace ECS{

struct Hierarchy
{
    Entity::EntityID parent;
    Entity::EntityID firstChild;
    Entity::EntityID nextSibling;
    Entity::EntityID prevSibling;
};

struct Transform 
{
    //float x = 0.0f, y = 0.0f; 
};

struct WorldTransform 
{
    //float x = 0.0f, y = 0.0f;
};

}
}

#endif //!S2_ECS_PROPERTIES