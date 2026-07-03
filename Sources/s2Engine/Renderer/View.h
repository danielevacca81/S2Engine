// View.h
// 
#ifndef RENDERER_VIEW_H
#define RENDERER_VIEW_H

#include "s2Engine_API.h"

#include "Math/Math.h"
#include "Math/ProjectionTransform.h"
#include "Math/Rectangle.h"
#include "Math/Ray.h"
#include "Math/Plane.h"

#include <array>

namespace s2 {
namespace Renderer {

class S2ENGINE_API View
{
public:
	void setViewport           ( const Math::irect &viewport )                          { _viewport            = viewport;            computeFrustum(); }
	void setViewMatrix         ( const Math::dmat4 &viewMatrix )                        { _viewMatrix          = viewMatrix;          computeFrustum(); }
	void setProjectionTransform( const Math::ProjectionTransform &projectionTransform ) { _projectionTransform = projectionTransform; computeFrustum(); }

	inline Math::irect                        viewport()              const { return _viewport; }
	inline Math::dmat4                        viewMatrix()            const { return _viewMatrix; }
	inline Math::dmat4                        projectionMatrix()      const { return _projectionTransform.matrix(); }
	inline Math::dmat4                        viewProjectionMatrix()  const { return projectionMatrix() * viewMatrix(); }
	inline Math::dvec4                        cameraEye()             const { return _cameraEye; } // camera position in worldspace
	inline const std::array<Math::dplane, 6>& frustumPlanes()         const { return _frustumPlanes; }

	Math::dray  rayAt( const Math::ivec2 &screenCoord ) const;
	Math::dvec3 worldPoint( const Math::ivec2 &screenCoord ) const { return worldPointF( Math::dvec2(screenCoord) ); }
	Math::dvec3 worldPointF( const Math::dvec2 &screenCoord ) const;
	Math::ivec2 screenPoint( const Math::dvec3 &worldCoord ) const;

	double      pixelToWorldSize( int pixels )       const;
	int         worldToPixelSize( double worldSize ) const;
	Math::dvec3 normalizedDeviceCoordinates( const Math::ivec2 &screenCoord ) const;
			    
	bool        isOrthographic() const;
	bool        isPerspective()  const;
			    
	bool        operator==( const View &otherView ) const;
	bool        operator!=( const View &otherView ) const { return !operator==( otherView ); }

private:
	void computeFrustum();

private:
	Math::irect               _viewport;
	Math::ProjectionTransform _projectionTransform;
	Math::dmat4               _viewMatrix = Math::dmat4( 1.0 );
	
	// computed values
	Math::dvec4                 _cameraEye { 0.0, 0.0, 0.0, 1.0 };
	std::array<Math::dplane, 6> _frustumPlanes { Math::dplane( 0.0, 0.0, 0.0, 0.0 ),
											   Math::dplane( 0.0, 0.0, 0.0, 0.0 ),
											   Math::dplane( 0.0, 0.0, 0.0, 0.0 ),
											   Math::dplane( 0.0, 0.0, 0.0, 0.0 ),
											   Math::dplane( 0.0, 0.0, 0.0, 0.0 ),
											   Math::dplane( 0.0, 0.0, 0.0, 0.0 ) };
};

}
}
#endif // !RENDERER_VIEW_H