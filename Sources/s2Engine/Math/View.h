// View.h
// 
#ifndef MATH_VIEW_H
#define MATH_VIEW_H

#include "s2Engine_API.h"

#include "Math/Math.h"
#include "Rectangle.h"
#include "Plane.h"

#include <array>

namespace glm{

class S2ENGINE_API View
{
public:
	View( 
		const Math::irect &viewport   = {} ,
		const Math::dmat4 &model      = Math::dmat4( 1.0 ),
		const Math::dmat4 &view       = Math::dmat4( 1.0 ),
		const Math::dmat4 &projection = Math::dmat4( 1.0 ) )
	: _viewport( viewport )
	, _modelMatrix( model )
	, _viewMatrix( view )
	, _projectionMatrix( projection )
	{
		update();
	}

	void setViewport        ( const Math::irect &viewport )         { _viewport         = viewport;         update(); }
	void setViewMatrix      ( const Math::dmat4 &viewMatrix )       { _viewMatrix       = viewMatrix;       update(); }
	void setModelMatrix     ( const Math::dmat4 &modelMatrix )      { _modelMatrix      = modelMatrix;      update(); }
	void setProjectionMatrix( const Math::dmat4 &projectionMatrix ) { _projectionMatrix = projectionMatrix; update(); }
	
	inline Math::dvec4 cameraEye()                             const { return _cameraEye; } // camera position in worldspace
	inline Math::irect viewport()                              const { return _viewport; }
	inline Math::dmat4 viewMatrix()                            const { return _viewMatrix; }
	inline Math::dmat4 modelMatrix()                           const { return _modelMatrix; }
	inline Math::dmat3 normalMatrix()                          const { return _normalMatrix; }
	inline Math::dmat4 projectionMatrix()                      const { return _projectionMatrix; }
	inline Math::dmat4 modelViewMatrix()                       const { return _viewMatrix * _modelMatrix; }
	inline Math::dmat4 modelViewProjectionMatrix()             const { return projectionMatrix() * modelViewMatrix(); }
	inline Math::dmat4 screenSpaceProjectionMatrix()           const;
	inline Math::dmat4 modelViewMatrixRelativeToEye()          const;
	inline Math::dmat4 modelViewMatrixProjectionRelatveToEye() const;
	
	Math::dvec3 worldPoint( const Math::ivec2 &screenCoord ) const { return worldPointF( Math::dvec2(screenCoord) ); }
	Math::dvec3 worldPointF( const Math::dvec2 &screenCoord ) const;
	Math::ivec2 screenPoint( const Math::dvec3 &worldCoord ) const;

	double      pixelToWorldSize( int pixels )       const;
	int         worldToPixelSize( double worldSize ) const;
	Math::dvec3 normalizedDeviceCoordinates( const Math::ivec2 &screenCoord ) const;
			    
	bool        isOrthographic() const;
	bool        isPerspective()  const;
			    
	bool        equals( const View &otherView ) const;

private:
	void update();

private:
	Math::irect _viewport;
	Math::dmat4 _projectionMatrix;
	Math::dmat4 _viewMatrix;
	Math::dmat4 _modelMatrix;
	Math::dmat4 _normalMatrix;
	Math::dvec4 _cameraEye;
	
	std::array<Math::dplane, 6> _frustumPlanes;
};

}

#endif