// Properties.h
//
#ifndef S2_ECS_PROPERTIES
#define S2_ECS_PROPERTIES

#include "Entity.h"

#include "Math/Math.h"
#include "Math/Box.h"
#include "Graphics/Color.h"

#include <string>
#include <variant>

namespace s2{
namespace ECS{

struct Name { std::string value;};

struct Spatial // spatial hierarchy? consider adding also parent-child relationships
{
    Math::dmat4 localTransform;
    Math::dmat4 worldTransform;

    Math::dbox3 localBounds;
    Math::dbox3 WorldBounds;
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

struct LightData
{
    //Type type { Type::Directional };
    Math::dvec3 color { 1.0, 1.0, 1.0 };
    double intensity { 1.0 };
    double range { 10.0 };
};


struct CameraData
{
    struct Perspective   { double fov { 60.0 }; };
    struct Orthographic  { double orthoHeight{ 10.0 }; };
    enum class ClearMode { ColorAndDepth, DepthOnly, None };

    ClearMode clearMode{ ClearMode::ColorAndDepth };
    Color  clearColor {0.1f, 0.1f, 0.1f, 0.1f };
    double othoHeight { 10.0 };
    double nearPlane { 0.1 };
    double farPlane { 1000.0 };
    Math::ivec2 viewportSize { 1920, 1080 };
    std::variant<Perspective,Orthographic> projection = Perspective{};
};


struct ActiveCameraTag {};

struct BodyData
{
    double mass { 1.0 };
    bool isStatic { false };
};




}
}

#endif //!S2_ECS_PROPERTIES