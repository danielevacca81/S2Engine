// Properties.h
//
#ifndef S2_ECS_PROPERTIES
#define S2_ECS_PROPERTIES

#include "Entity.h"
#include "Math/Math.h"

#include <string>

namespace s2{
namespace ECS{

struct Name
{
    std::string value;
};

struct Hierarchy
{
    Entity::EntityID parent { Entity::EntityInvalidID };
    Entity::EntityID firstChild { Entity::EntityInvalidID };
    Entity::EntityID nextSibling { Entity::EntityInvalidID };
    Entity::EntityID prevSibling { Entity::EntityInvalidID };
};

struct Transform
{
    Math::dvec3 position { 0.0, 0.0, 0.0 };
    Math::dvec3 rotation { 0.0, 0.0, 0.0 };
    Math::dvec3 scale { 1.0, 1.0, 1.0 };
};

struct WorldTransform
{
    Math::dmat4 matrix { 1.0 };
};

}
}

#endif //!S2_ECS_PROPERTIES