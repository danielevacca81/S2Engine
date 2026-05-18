// Transform.cpp
//
#include "Transform.h"
#include "Entity.h"

#include "Math/Math.h"

using namespace s2::Scene;

// ------------------------------------------------------------------------------------------------
Transform::Transform()
    : _localPosition(0, 0, 0)
    , _localRotation(0, 0, 0)
    , _localScale(1, 1, 1)
{
}

// ------------------------------------------------------------------------------------------------
Transform::Transform( const Math::dvec3& position, const Math::dvec3& rotation, const Math::dvec3& scale )
    : _localPosition( position )
    , _localRotation( rotation )
    , _localScale( scale )
{
}

// ------------------------------------------------------------------------------------------------
void Transform::setLocalPosition( const Math::dvec3& pos )
{
    if( _localPosition != pos )
    {
        _localPosition = pos;
        markDirty();
    }
}

// ------------------------------------------------------------------------------------------------
void Transform::setLocalRotation( const Math::dvec3& rot )
{
    if( _localRotation != rot )
    {
        _localRotation = rot;
        markDirty();
    }
}

// ------------------------------------------------------------------------------------------------
void Transform::setLocalScale( const Math::dvec3& scl )
{
    if( _localScale != scl )
    {
        _localScale = scl;
        markDirty();
    }
}

// ------------------------------------------------------------------------------------------------
Math::dvec3 Transform::worldPosition() const
{
    return Math::dvec3( worldMatrix()[3] );
}

// ------------------------------------------------------------------------------------------------
Math::dvec3 Transform::worldRotation() const
{
    // Extract Euler angles from world matrix (simplified)
    if( _parent )
        return _parent->worldRotation() + _localRotation;
    return _localRotation;
}

// ------------------------------------------------------------------------------------------------
Math::dvec3 Transform::worldScale() const
{
    if( _parent )
    {
        auto parentScale = _parent->worldScale();
        return Math::dvec3(
            _localScale.x * parentScale.x,
            _localScale.y * parentScale.y,
            _localScale.z * parentScale.z
        );
    }
    return _localScale;
}

// ------------------------------------------------------------------------------------------------
void Transform::setParent( Transform* parent )
{
    if( _parent == parent )
        return;

    // Remove from old parent
    if( _parent )
    {
        auto& siblings = _parent->_children;
        siblings.erase( std::find( siblings.begin(), siblings.end(), this ) );
    }

    // Add to new parent
    _parent = parent;
    if( _parent )
        _parent->_children.push_back( this );

    markDirty();
}

// ------------------------------------------------------------------------------------------------
Math::dmat4 Transform::localMatrix() const
{
    if( _localMatrixDirty )
        updateMatrices();
    return _cachedLocalMatrix;
}

// ------------------------------------------------------------------------------------------------
Math::dmat4 Transform::worldMatrix() const
{
    if( _worldMatrixDirty )
        updateMatrices();
    return _cachedWorldMatrix;
}

// ------------------------------------------------------------------------------------------------
Math::dvec3 Transform::forward() const
{
    auto mat = worldMatrix();
    return -Math::dvec3( mat[2].x, mat[2].y, mat[2].z );  // -Z axis
}

// ------------------------------------------------------------------------------------------------
Math::dvec3 Transform::right() const
{
    auto mat = worldMatrix();
    return Math::dvec3( mat[0].x, mat[0].y, mat[0].z );  // X axis
}

// ------------------------------------------------------------------------------------------------
Math::dvec3 Transform::up() const
{
    auto mat = worldMatrix();
    return Math::dvec3( mat[1].x, mat[1].y, mat[1].z );  // Y axis
}

// ------------------------------------------------------------------------------------------------
void Transform::markDirty()
{
    _localMatrixDirty = true;
    _worldMatrixDirty = true;
    notifyChildrenDirty();
}

// ------------------------------------------------------------------------------------------------
void Transform::updateMatrices() const
{
    // Build local matrix: Translation * RotationX * RotationY * RotationZ * Scale
    // Using Euler angles (roll, pitch, yaw) in radians
    
    // Start with identity
    Math::dmat4 translation = Math::translate( Math::dmat4(1.0), _localPosition );
    Math::dmat4 rotation = Math::dmat4(1.0);
    Math::dmat4 scale = Math::scale( Math::dmat4(1.0), _localScale );
    
    // Apply rotations in ZYX order (yaw, pitch, roll)
    if( _localRotation != Math::dvec3(0) )
    {
        rotation = Math::rotate( rotation, _localRotation.z, Math::dvec3(0, 0, 1) );  // Roll (Z)
        rotation = Math::rotate( rotation, _localRotation.y, Math::dvec3(0, 1, 0) );  // Pitch (Y)
        rotation = Math::rotate( rotation, _localRotation.x, Math::dvec3(1, 0, 0) );  // Yaw (X)
    }
    
    // Combine: TRS (Translation * Rotation * Scale)
    _cachedLocalMatrix = translation * rotation * scale;
    _localMatrixDirty = false;

    // World matrix = parent world matrix * local matrix
    _cachedWorldMatrix = _parent 
        ? _parent->worldMatrix() * _cachedLocalMatrix
        : _cachedLocalMatrix;

    _worldMatrixDirty = false;
}


// ------------------------------------------------------------------------------------------------
void Transform::notifyChildrenDirty()
{
    for( auto child : _children )
        child->markDirty();
}

// ------------------------------------------------------------------------------------------------
void Transform::onAttach()
{
    // Transform attached to entity - already initialized
}

void Transform::onDetach()
{
    // When detached, clear parent references
    for( auto child : _children )
        child->_parent = nullptr;

    //  _children.clear();
}