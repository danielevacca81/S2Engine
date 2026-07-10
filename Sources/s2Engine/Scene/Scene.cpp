// Scene.cpp
//
#include "Scene.h"

#include "ECS/World.h"

#include <utility>
#include <variant>

using namespace s2::Scene;
using namespace s2::ECS;

namespace {

template<typename T>
T& ensureProperty(World& world, Entity entity)
{
    if (!world.hasProperty<T>(entity))
    {
        return world.addProperty<T>(entity);
    }

    return world.property<T>(entity);
}

} // namespace

// ------------------------------------------------------------------------------------------------
Scene::Scene()
: _world(std::make_unique<World>())
{}

// ------------------------------------------------------------------------------------------------
Scene::~Scene() = default;

// ------------------------------------------------------------------------------------------------
SceneObject Scene::createObject(const std::string& name)
{
    SceneObject object(this, _world->createEntity());
    object.setName(name);
    return object;
}

// ------------------------------------------------------------------------------------------------
Camera Scene::createCamera(const std::string& name)
{
    Camera camera(this, _world->createEntity());
    camera.setName(name);
    return camera;
}

// ------------------------------------------------------------------------------------------------
Light Scene::createLight(const std::string& name)
{
    Light light(this, _world->createEntity());
    light.setName(name);
    return light;
}

// ------------------------------------------------------------------------------------------------
Body Scene::createBody(const std::string& name)
{
    Body body(this, _world->createEntity());
    body.setName(name);
    return body;
}

// ------------------------------------------------------------------------------------------------
std::vector<SceneObject> Scene::objects() const
{
    std::vector<SceneObject> result;
    auto& world = const_cast<World&>(this->world());
    world.each<s2::ECS::Transform>([&](Entity entity, s2::ECS::Transform&)
    {
        result.emplace_back(const_cast<Scene*>(this), entity);
    });
    return result;
}

// ------------------------------------------------------------------------------------------------
std::vector<Camera> Scene::cameras() const
{
    std::vector<Camera> result;
    auto& world = const_cast<World&>(this->world());
    world.each<ECS::CameraData>([&](Entity entity, ECS::CameraData&)
    {
        result.emplace_back(const_cast<Scene*>(this), entity);
    });
    return result;
}

// ------------------------------------------------------------------------------------------------
std::vector<Light> Scene::lights() const
{
    std::vector<Light> result;
    auto& world = const_cast<World&>(this->world());
    world.each<ECS::LightData>([&](Entity entity, ECS::LightData&)
    {
        result.emplace_back(const_cast<Scene*>(this), entity);
    });
    return result;
}

// ------------------------------------------------------------------------------------------------
std::vector<Body> Scene::bodies() const
{
    std::vector<Body> result;
    auto& world = const_cast<World&>(this->world());
    world.each<ECS::BodyData>([&](Entity entity, ECS::BodyData&)
    {
        result.emplace_back(const_cast<Scene*>(this), entity);
    });
    return result;
}

// ------------------------------------------------------------------------------------------------
SceneObject Scene::findByName(const std::string& name) const
{
    auto& world = const_cast<World&>(this->world());
    for (auto object : objects())
    {
        if (object.name() == name)
        {
            return object;
        }
    }

    return {};
}

// ------------------------------------------------------------------------------------------------
std::vector<SceneObject> Scene::findAllByName(const std::string& name) const
{
    std::vector<SceneObject> result;
    for (auto object : objects())
    {
        if (object.name() == name)
        {
            result.push_back(object);
        }
    }
    return result;
}

// ------------------------------------------------------------------------------------------------
void Scene::clear()
{
    _world->clear();
}

// ------------------------------------------------------------------------------------------------
World& Scene::world()
{
    return *_world;
}

// ------------------------------------------------------------------------------------------------
const World& Scene::world() const
{
    return *_world;
}

// ------------------------------------------------------------------------------------------------
void Scene::setActiveCamera( const Camera &camera )
{
    // find any other active camera and set remove the ActiveTag from it
    _world->each<ECS::ActiveCameraTag>([this](ECS::Entity e)
                                       { _world->removeProperty<ECS::ActiveCameraTag>(e); });

    if( !camera.isValid() )
        return;
    
    _world->addProperty<ECS::ActiveCameraTag>(camera.entity());
}

// ------------------------------------------------------------------------------------------------
Camera Scene::activeCamera() const
{
    Camera cam;
    _world->each<ECS::ActiveCameraTag>([&](ECS::Entity e)
                                        { cam = Camera(const_cast<Scene*>(this), e); });
    return cam;
}

// ------------------------------------------------------------------------------------------------
SceneObject::SceneObject(Scene* scene, ECS::Entity entity)
: _scene(scene)
, _entity(entity)
{
    ensureProperties();
}

// ------------------------------------------------------------------------------------------------
bool SceneObject::isValid() const
{
    return _scene != nullptr && _entity.isValid();
}

// ------------------------------------------------------------------------------------------------
void SceneObject::destroy()
{
    if (isValid())
    {
        _entity.destroy();
        _scene = nullptr;
    }
}

// ------------------------------------------------------------------------------------------------
std::string SceneObject::name() const
{
    if (!isValid())
    {
        return {};
    }

    ensureProperties();
    auto& world = const_cast<World&>(_scene->world());
    if (!world.hasProperty<Name>(_entity))
    {
        return {};
    }

    return world.property<Name>(_entity).value;
}

// ------------------------------------------------------------------------------------------------
void SceneObject::setName(const std::string& name)
{
    if (!isValid())
    {
        return;
    }

    ensureProperties();
    auto& world = _scene->world();
    auto& data = ensureProperty<Name>(world, _entity);
    data.value = name;
}

// ------------------------------------------------------------------------------------------------
Math::dvec3 SceneObject::position() const
{
    return transform().position;
}

// ------------------------------------------------------------------------------------------------
void SceneObject::setPosition(const Math::dvec3& position)
{
    transform().position = position;
}

// ------------------------------------------------------------------------------------------------
Math::dvec3 SceneObject::rotation() const
{
    return transform().rotation;
}

// ------------------------------------------------------------------------------------------------
void SceneObject::setRotation(const Math::dvec3& rotation)
{
    transform().rotation = rotation;
}

// ------------------------------------------------------------------------------------------------
Math::dvec3 SceneObject::scale() const
{
    return transform().scale;
}

// ------------------------------------------------------------------------------------------------
void SceneObject::setScale(const Math::dvec3& scale)
{
    transform().scale = scale;
}

// ------------------------------------------------------------------------------------------------
Math::dmat4 SceneObject::worldTransform() const
{
    if (!isValid())
    {
        return Math::dmat4(1.0);
    }

    updateWorldTransform();
    return _scene->world().property<s2::ECS::WorldTransform>(_entity).matrix;
}

// ------------------------------------------------------------------------------------------------
Math::dvec3 SceneObject::worldPosition() const
{
    const auto matrix = worldTransform();
    return Math::dvec3(matrix[3]);
}

// ------------------------------------------------------------------------------------------------
Math::dvec3 SceneObject::worldRotation() const
{
    const auto matrix = worldTransform();
    Math::dquat rotationQuat;
    Math::dvec3 scale;
    Math::dvec3 translation;
    Math::dvec3 skew;
    Math::dvec4 perspective;
    Math::decompose(matrix, scale, rotationQuat, translation, skew, perspective);
    return Math::eulerAngles(rotationQuat);
}

// ------------------------------------------------------------------------------------------------
Math::dvec3 SceneObject::worldScale() const
{
    const auto matrix = worldTransform();
    Math::dquat rotationQuat;
    Math::dvec3 scale;
    Math::dvec3 translation;
    Math::dvec3 skew;
    Math::dvec4 perspective;
    Math::decompose(matrix, scale, rotationQuat, translation, skew, perspective);
    return scale;
}

// ------------------------------------------------------------------------------------------------
bool SceneObject::hasParent() const
{
    return hierarchy().parent != ECS::Entity::EntityInvalidID;
}

// ------------------------------------------------------------------------------------------------
SceneObject SceneObject::parent() const
{
    if (!isValid() || !hasParent())
    {
        return {};
    }

    return SceneObject(_scene, Entity(hierarchy().parent, &_scene->world()));
}

// ------------------------------------------------------------------------------------------------
std::vector<SceneObject> SceneObject::children() const
{
    std::vector<SceneObject> result;
    if (!isValid())
    {
        return result;
    }

    auto& world = const_cast<World&>(_scene->world());
    world.each<Hierarchy>([&](Entity child, Hierarchy& hierarchy)
    {
        if (hierarchy.parent == _entity.id())
        {
            result.emplace_back(_scene, child);
        }
    });

    return result;
}

// ------------------------------------------------------------------------------------------------
void SceneObject::setParent(const SceneObject& parent)
{
    if (!isValid())
    {
        return;
    }

    detachFromParent();
    if (parent.isValid())
    {
        detachFromParent();
        hierarchy().parent = parent.entity().id();
        auto& siblingHierarchy = parent._scene->world().property<s2::ECS::Hierarchy>(parent.entity());
        if (siblingHierarchy.firstChild == ECS::Entity::EntityInvalidID)
        {
            siblingHierarchy.firstChild = _entity.id();
            hierarchy().prevSibling = ECS::Entity::EntityInvalidID;
            hierarchy().nextSibling = ECS::Entity::EntityInvalidID;
        }
        else
        {
            auto current = siblingHierarchy.firstChild;
            auto previous = ECS::Entity::EntityInvalidID;
            while (current != ECS::Entity::EntityInvalidID)
            {
                const auto next = _scene->world().property<s2::ECS::Hierarchy>(Entity(current, &_scene->world())).nextSibling;
                if (next == ECS::Entity::EntityInvalidID)
                {
                    auto& tailHierarchy = _scene->world().property<s2::ECS::Hierarchy>(Entity(current, &_scene->world()));
                    tailHierarchy.nextSibling = _entity.id();
                    hierarchy().prevSibling = current;
                    hierarchy().nextSibling = ECS::Entity::EntityInvalidID;
                    break;
                }
                previous = current;
                current = next;
            }
        }
    }
    else
    {
        hierarchy().parent = ECS::Entity::EntityInvalidID;
    }
}

// ------------------------------------------------------------------------------------------------
void SceneObject::removeParent()
{
    detachFromParent();
    hierarchy().parent = ECS::Entity::EntityInvalidID;
}

// ------------------------------------------------------------------------------------------------
bool SceneObject::isAncestorOf(const SceneObject& object) const
{
    if (!isValid() || !object.isValid())
    {
        return false;
    }

    return object.isDescendantOf(_entity.id());
}

// ------------------------------------------------------------------------------------------------
void SceneObject::updateWorldTransform() const
{
    if (!isValid())
    {
        return;
    }

    auto& world = const_cast<World&>(_scene->world());
    if (!world.hasProperty<s2::ECS::WorldTransform>(_entity))
    {
        world.addProperty<s2::ECS::WorldTransform>(_entity);
    }

    auto& local = transform();
    auto& worldTransform = world.property<s2::ECS::WorldTransform>(_entity);
    worldTransform.matrix = Math::dmat4(1.0);

    const auto translation = Math::translate(Math::dmat4(1.0), local.position);
    const auto rotation = Math::rotate(Math::dmat4(1.0), local.rotation.x, Math::dvec3(1.0, 0.0, 0.0));
    const auto rotationY = Math::rotate(rotation, local.rotation.y, Math::dvec3(0.0, 1.0, 0.0));
    const auto rotationZ = Math::rotate(rotationY, local.rotation.z, Math::dvec3(0.0, 0.0, 1.0));
    const auto scaling = Math::scale(Math::dmat4(1.0), local.scale);
    worldTransform.matrix = translation * rotationZ * scaling;

    if (hasParent())
    {
        const auto parentWorld = parent().worldTransform();
        worldTransform.matrix = parentWorld * worldTransform.matrix;
    }
}

// ------------------------------------------------------------------------------------------------
void SceneObject::detachFromParent()
{
    if (!isValid() || !hasParent())
    {
        return;
    }

    auto& parentHierarchy = _scene->world().property<s2::ECS::Hierarchy>(Entity(hierarchy().parent, &_scene->world()));
    if (parentHierarchy.firstChild == _entity.id())
    {
        parentHierarchy.firstChild = hierarchy().nextSibling;
    }
    else
    {
        auto current = parentHierarchy.firstChild;
        while (current != ECS::Entity::EntityInvalidID)
        {
            auto currentEntity = Entity(current, &_scene->world());
            auto& currentHierarchy = _scene->world().property<s2::ECS::Hierarchy>(currentEntity);
            if (currentHierarchy.nextSibling == _entity.id())
            {
                currentHierarchy.nextSibling = hierarchy().nextSibling;
                break;
            }
            current = currentHierarchy.nextSibling;
        }
    }

    hierarchy().nextSibling = ECS::Entity::EntityInvalidID;
    hierarchy().prevSibling = ECS::Entity::EntityInvalidID;
}

// ------------------------------------------------------------------------------------------------
bool SceneObject::isDescendantOf(const ECS::Entity::EntityID ancestorId) const
{
    if (!isValid())
    {
        return false;
    }

    auto current = hierarchy().parent;
    while (current != ECS::Entity::EntityInvalidID)
    {
        if (current == ancestorId)
        {
            return true;
        }
        current = _scene->world().property<s2::ECS::Hierarchy>(Entity(current, &_scene->world())).parent;
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
void SceneObject::ensureProperties() const
{
    if (!_scene || !_entity.isValid())
    {
        return;
    }

    auto& world = const_cast<World&>(_scene->world());
    if (!world.hasProperty<s2::ECS::Transform>(_entity))
    {
        world.addProperty<s2::ECS::Transform>(_entity);
    }
    if (!world.hasProperty<s2::ECS::Hierarchy>(_entity))
    {
        world.addProperty<s2::ECS::Hierarchy>(_entity);
    }
    if (!world.hasProperty<s2::ECS::Name>(_entity))
    {
        world.addProperty<s2::ECS::Name>(_entity);
    }
    if (!world.hasProperty<s2::ECS::WorldTransform>(_entity))
    {
        world.addProperty<s2::ECS::WorldTransform>(_entity);
    }
}

// ------------------------------------------------------------------------------------------------
s2::ECS::Transform& SceneObject::transform()
{
    ensureProperties();
    return _scene->world().property<s2::ECS::Transform>(_entity);
}

// ------------------------------------------------------------------------------------------------
const s2::ECS::Transform& SceneObject::transform() const
{
    ensureProperties();
    return const_cast<SceneObject*>(this)->_scene->world().property<s2::ECS::Transform>(_entity);
}

// ------------------------------------------------------------------------------------------------
s2::ECS::Hierarchy& SceneObject::hierarchy()
{
    ensureProperties();
    return _scene->world().property<s2::ECS::Hierarchy>(_entity);
}

// ------------------------------------------------------------------------------------------------
const s2::ECS::Hierarchy& SceneObject::hierarchy() const
{
    ensureProperties();
    return const_cast<SceneObject*>(this)->_scene->world().property<s2::ECS::Hierarchy>(_entity);
}

// ------------------------------------------------------------------------------------------------
Camera::Camera(Scene* scene, ECS::Entity entity)
: SceneObject(scene, entity)
{}

// ------------------------------------------------------------------------------------------------
bool Camera::isPerspective() const
{
    return std::holds_alternative<ECS::CameraData::Perspective>(cameraData().projection);
}

// ------------------------------------------------------------------------------------------------
bool Camera::isOrthographic() const
{
    return std::holds_alternative<ECS::CameraData::Orthographic>(cameraData().projection);
}

// ------------------------------------------------------------------------------------------------
double Camera::fov() const
{
    if (const auto *p = std::get_if<ECS::CameraData::Perspective>(&cameraData().projection))
        return p->fov;

    return 0.0;
}

// ------------------------------------------------------------------------------------------------
void Camera::setPerspective(double value)
{
    cameraData().projection = ECS::CameraData::Perspective{ value };
}

// ------------------------------------------------------------------------------------------------
double Camera::orthoHeight() const
{
    if (const auto* o = std::get_if<ECS::CameraData::Orthographic>(&cameraData().projection))
        return o->orthoHeight;
    
    return 0.0;
}

// ------------------------------------------------------------------------------------------------
void Camera::setOrthographic(double value)
{
    cameraData().projection = ECS::CameraData::Orthographic{ value };
}

// ------------------------------------------------------------------------------------------------
double Camera::nearPlane() const
{
    return cameraData().nearPlane;
}

// ------------------------------------------------------------------------------------------------
void Camera::setNearPlane(double value)
{
    cameraData().nearPlane = value;
}

// ------------------------------------------------------------------------------------------------
double Camera::farPlane() const
{
    return cameraData().farPlane;
}

// ------------------------------------------------------------------------------------------------
void Camera::setFarPlane(double value)
{
    cameraData().farPlane = value;
}

// ------------------------------------------------------------------------------------------------
Math::ivec2 Camera::viewportSize() const
{
    return cameraData().viewportSize;
}

// ------------------------------------------------------------------------------------------------
void Camera::setViewportSize(const Math::ivec2& size)
{
    cameraData().viewportSize = size;
}

// ------------------------------------------------------------------------------------------------
s2::ECS::CameraData& Camera::cameraData()
{
    if (!isValid())
    {
        static CameraData fallback;
        return fallback;
    }

    auto& world = scene()->world();
    if (!world.hasProperty<CameraData>(entity()))
        world.addProperty<CameraData>(entity());

    return world.property<CameraData>(entity());
}

// ------------------------------------------------------------------------------------------------
const s2::ECS::CameraData& Camera::cameraData() const
{
    return const_cast<Camera*>(this)->cameraData();
}

// ------------------------------------------------------------------------------------------------
Light::Light(Scene* scene, ECS::Entity entity)
: SceneObject(scene, entity)
{
    //lightData();
}

// ------------------------------------------------------------------------------------------------
Light::Type Light::type() const
{
    return lightType;
}

// ------------------------------------------------------------------------------------------------
void Light::setType(Type value)
{
    lightType = value;
}

// ------------------------------------------------------------------------------------------------
Math::dvec3 Light::color() const
{
    return _lightData.color;
}

// ------------------------------------------------------------------------------------------------
void Light::setColor(const Math::dvec3& value)
{
    _lightData.color = value;
}

// ------------------------------------------------------------------------------------------------
double Light::intensity() const
{
    return _lightData.intensity;
}

// ------------------------------------------------------------------------------------------------
void Light::setIntensity(double value)
{
    _lightData.intensity = value;
}

// ------------------------------------------------------------------------------------------------
double Light::range() const
{
    return _lightData.range;
}

// ------------------------------------------------------------------------------------------------
void Light::setRange(double value)
{
    _lightData.range = value;
}

// ------------------------------------------------------------------------------------------------
s2::ECS::LightData& Light::lightData()
{
    if (!isValid())
    {
        static LightData fallback;
        return fallback;
    }

    auto& world = scene()->world();
    if (!world.hasProperty<LightData>(entity()))
    {
        world.addProperty<LightData>(entity());
    }

    return world.property<LightData>(entity());
}

// ------------------------------------------------------------------------------------------------
const s2::ECS::LightData& Light::lightData() const
{
    return const_cast<Light*>(this)->lightData();
}

// ------------------------------------------------------------------------------------------------
Body::Body(Scene* scene, ECS::Entity entity)
: SceneObject(scene, entity)
{
    bodyData();
}

// ------------------------------------------------------------------------------------------------
double Body::mass() const
{
    return bodyData().mass;
}

// ------------------------------------------------------------------------------------------------
void Body::setMass(double value)
{
    bodyData().mass = value;
}

// ------------------------------------------------------------------------------------------------
bool Body::isStatic() const
{
    return bodyData().isStatic;
}

// ------------------------------------------------------------------------------------------------
void Body::setStatic(bool value)
{
    bodyData().isStatic = value;
}

// ------------------------------------------------------------------------------------------------
s2::ECS::BodyData& Body::bodyData()
{
    if (!isValid())
    {
        static BodyData fallback;
        return fallback;
    }

    auto& world = scene()->world();
    if (!world.hasProperty<BodyData>(entity()))
    {
        world.addProperty<BodyData>(entity());
    }

    return world.property<BodyData>(entity());
}

// ------------------------------------------------------------------------------------------------
const s2::ECS::BodyData& Body::bodyData() const
{
    return const_cast<Body*>(this)->bodyData();
}
