// Transform.h
//
#ifndef S2_SCENE_TRANSFORM_H
#define S2_SCENE_TRANSFORM_H

#include "s2Engine_API.h"
#include "Component.h"
#include "Math/Math.h"

#include <vector>

namespace s2 {
namespace Scene {

class S2ENGINE_API Transform : public Component
{
public:
    Transform();
    Transform( const Math::dvec3& position, const Math::dvec3& rotation = Math::dvec3(0), const Math::dvec3& scale = Math::dvec3(1) );

    // Position (local space)
    void setLocalPosition( const Math::dvec3& pos );
    const Math::dvec3& localPosition() const { return _localPosition; }
    Math::dvec3 worldPosition() const;

    // Rotation (Euler angles in radians, local space)
    void setLocalRotation( const Math::dvec3& rot );  // rot in radians (roll, pitch, yaw)
    const Math::dvec3& localRotation() const { return _localRotation; }
    Math::dvec3 worldRotation() const;

    // Scale (local space)
    void setLocalScale( const Math::dvec3& scl );
    const Math::dvec3& localScale() const { return _localScale; }
    Math::dvec3 worldScale() const;

    // Hierarchy
    void setParent( Transform* parent );
    Transform* parent() const { return _parent; }
    const std::vector<Transform*>& children() const { return _children; }

    // Matrices
    Math::dmat4 localMatrix() const;      // TRS in local space
    Math::dmat4 worldMatrix() const;      // TRS in world space

    // Direction vectors (from world matrix)
    Math::dvec3 forward() const;  // -Z axis
    Math::dvec3 right() const;    // X axis
    Math::dvec3 up() const;       // Y axis

    // Mark matrix as dirty (called automatically on changes)
    void markDirty();

protected:
    void onAttach() override;
    void onDetach() override;

private:
    void updateMatrices() const;
    void notifyChildrenDirty();

private:
    Math::dvec3 _localPosition { 0, 0, 0 };
    Math::dvec3 _localRotation { 0, 0, 0 };  // Euler angles in radians
    Math::dvec3 _localScale { 1, 1, 1 };

    Transform* _parent = nullptr;
    std::vector<Transform*> _children;

    mutable Math::dmat4 _cachedLocalMatrix;
    mutable Math::dmat4 _cachedWorldMatrix;
    mutable bool _localMatrixDirty = true;
    mutable bool _worldMatrixDirty = true;


    friend class Entity;
};

} // namespace Scene
} // namespace s2

#endif // !S2_SCENE_TRANSFORM_H
