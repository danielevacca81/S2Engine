// ViewState.h
// 
#ifndef VIEWSTATE_ONCE
#define VIEWSTATE_ONCE

#include "s2SceneGraph_API.h"

#include "Math/Math.h"
#include "Math/Rectangle.h"

namespace s2 {
namespace SceneGraph {

class S2SCENEGRAPH_API ViewState
{
public:
	ViewState( 
		const Math::Rectangle &viewport   = Math::Rectangle() ,
		const Math::dmat4     &model      = Math::dmat4( 1.0 ),
		const Math::dmat4     &view       = Math::dmat4( 1.0 ),
		const Math::dmat4     &projection = Math::dmat4( 1.0 ) )
	: _viewport( viewport )
	, _modelMatrix( model )
	, _viewMatrix( view )
	, _projectionMatrix( projection )
	{}

	// ------------------------------------------------------------------------------------------------
	~ViewState()
	{}

	// ------------------------------------------------------------------------------------------------
	void setViewport        ( const Math::Rectangle &viewport )     { _viewport         = viewport; }
	void setViewMatrix      ( const Math::dmat4 &viewMatrix )       { _viewMatrix       = viewMatrix; }
	void setModelMatrix     ( const Math::dmat4 &modelMatrix )      { _modelMatrix      = modelMatrix; }
	void setProjectionMatrix( const Math::dmat4 &projectionMatrix ) { _projectionMatrix = projectionMatrix; }
	
	// ------------------------------------------------------------------------------------------------
	Math::Rectangle viewport()                    const { return _viewport; }
	Math::dmat4     viewMatrix()                  const { return _viewMatrix; }
	Math::dmat4     modelMatrix()                 const { return _modelMatrix; }
	Math::dmat3     normalMatrix()                const { return Math::inverseTranspose( modelViewMatrix() ); }
	Math::dmat4     projectionMatrix()            const { return _projectionMatrix; }
	Math::dmat4     modelViewMatrix()             const { return _viewMatrix * _modelMatrix; }
	Math::dmat4     modelViewProjectionMatrix()   const { return projectionMatrix() * modelViewMatrix(); }


private:
	Math::Rectangle _viewport;
	Math::dmat4     _projectionMatrix;
	Math::dmat4     _viewMatrix;
	Math::dmat4     _modelMatrix;
};

/****
	enum ProjectionMode { Perspective,Orthographic };

	// ------------------------------------------------------------------------------------------------
	double      pixelSize( const Math::dvec3 &p ) const;
	Math::dray  rayAt( int pxlX, int pxlY )       const;
	Math::dvec3 worldPoint( int pxlX, int pxlY )  const;


	Math::dmat4 perspectiveMatrix()                       const { return Math::perspective( view.fieldOfViewY(), view.aspectRatio(), view.perspectiveNearPlaneDistance(), view.perspectiveFarPlaneDistance() ); }

	// ------------------------------------------------------------------------------------------------
	static Math::dmat4 computeViewportOrthographicMatrix( const Math::Rectangle &vp )
	{
		return Math::ortho<double>( vp.left(), vp.right(), vp.bottom(), vp.top(), 0.0, 1.0 );
	}

	// ------------------------------------------------------------------------------------------------
	static Math::dmat4 computeViewportTransformationMatrix( const Math::Rectangle &viewport, double nearDepthRange, double farDepthRange)
	{
		const double halfWidth  = viewport.width() * 0.5;
		const double halfHeight = viewport.height() * 0.5;
		const double halfDepth  = (farDepthRange - nearDepthRange) * 0.5;

		// Bottom and top swapped:  MS -> OpenGL
		//
		return Math::dmat4(
			halfWidth, 0.0,        0.0,       viewport.left() + halfWidth,
			0.0,       halfHeight, 0.0,       viewport.top() + halfHeight,
			0.0,       0.0,        halfDepth, nearDepthRange + halfDepth,
			0.0,       0.0,        0.0,       1.0);	
	}
	
	// ------------------------------------------------------------------------------------------------
	bool        isOrthographic()                          const { return _projectionMode == Orthographic; }
	bool        isPerspective()                           const { return _projectionMode == Perspective; }

	// ------------------------------------------------------------------------------------------------
	Math::dmat4 orthographicMatrix()                      const 
	{
		return Math::ortho( view.orthographicLeft(), 
		view.orthographicRight(), 
		view.orthographicTop(), 
		view.orthographicBottom(), 
		view.orthographicNearPlaneDistance(), 
		view.orthographicFarPlaneDistance()); 
	}

	*/

}

}

#endif