// Scene.h
//
#ifndef S2_SCENE_SCENE_H
#define S2_SCENE_SCENE_H

#include "s2Engine_API.h"

#include "ECS/Entity.h"
#include "ECS/Properties.h"
#include "Math/Math.h"

#include <memory>
#include <string>
#include <vector>

namespace s2 {
namespace ECS {
class World;
}

namespace Scene {

class S2ENGINE_API Scene;

class S2ENGINE_API SceneObject
{
public:
    SceneObject() = default;
    SceneObject(Scene* scene, ECS::Entity entity);

    bool isValid() const;
    void destroy();

    Scene* scene() const { return _scene; }
    ECS::Entity entity() const { return _entity; }

    std::string name() const;
    void setName(const std::string& name);

    Math::dvec3 position() const;
    void setPosition(const Math::dvec3& position);

    Math::dvec3 rotation() const;
    void setRotation(const Math::dvec3& rotation);

    Math::dvec3 scale() const;
    void setScale(const Math::dvec3& scale);

    Math::dmat4 worldTransform() const;
    Math::dvec3 worldPosition() const;
    Math::dvec3 worldRotation() const;
    Math::dvec3 worldScale() const;

    bool hasParent() const;
    SceneObject parent() const;
    std::vector<SceneObject> children() const;
    void setParent(const SceneObject& parent);
    void removeParent();
    bool isAncestorOf(const SceneObject& object) const;

protected:
    void ensureProperties() const;
    ECS::Transform& transform();
    const ECS::Transform& transform() const;
    ECS::Hierarchy& hierarchy();
    const ECS::Hierarchy& hierarchy() const;
    void updateWorldTransform() const;
    void detachFromParent();
    bool isDescendantOf(const ECS::Entity::EntityID ancestorId) const;

private:
    Scene* _scene { nullptr };
    ECS::Entity _entity;
};

class S2ENGINE_API Camera : public SceneObject
{
public:
    Camera() = default;
    Camera(Scene* scene, ECS::Entity entity);

    bool isPerspective() const;
    bool isOrthographic() const;

    double fov() const;
    void setPerspective(double fov);

    double orthoHeight() const;
    void setOrthographic(double orthoHeight);

    double nearPlane() const;
    void setNearPlane(double value);

    double farPlane() const;
    void setFarPlane(double value);

    Math::ivec2 viewportSize() const;
    void setViewportSize(const Math::ivec2& size);

private:
    ECS::CameraData& cameraData();
    const ECS::CameraData& cameraData() const;
};

class S2ENGINE_API Light : public SceneObject
{
public:
    enum class Type
    {
        Directional,
        Point,
        Spot
    };

    Light() = default;
    Light(Scene* scene, ECS::Entity entity);

    Type type() const;
    void setType(Type value);

    Math::dvec3 color() const;
    void setColor(const Math::dvec3& value);

    double intensity() const;
    void setIntensity(double value);

    double range() const;
    void setRange(double value);

public:
    Type lightType;
    ECS::LightData _lightData;

private:
    ECS::LightData& lightData();
    const ECS::LightData& lightData() const;
};

class S2ENGINE_API Body : public SceneObject
{
public:
    Body() = default;
    Body(Scene* scene, ECS::Entity entity);

    double mass() const;
    void setMass(double value);

    bool isStatic() const;
    void setStatic(bool value);

public:
    ECS::BodyData _bodyData;

private:
    ECS::BodyData& bodyData();
    const ECS::BodyData& bodyData() const;
};

class S2ENGINE_API Scene
{
public:
    Scene();
    ~Scene();

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    SceneObject createObject(const std::string& name = {});
    Camera createCamera(const std::string& name = {});
    Light createLight(const std::string& name = {});
    Body createBody(const std::string& name = {});

    void setActiveCamera( const Camera &camera );
    Camera activeCamera() const;

    std::vector<SceneObject> objects() const;
    std::vector<Camera> cameras() const;
    std::vector<Light> lights() const;
    std::vector<Body> bodies() const;
    SceneObject findByName(const std::string& name) const;
    std::vector<SceneObject> findAllByName(const std::string& name) const;

    void clear();

    ECS::World& world();
    const ECS::World& world() const;

private:
    std::unique_ptr<ECS::World> _world;
};

} // namespace Scene
} // namespace s2

#endif // !S2_SCENE_SCENE_H