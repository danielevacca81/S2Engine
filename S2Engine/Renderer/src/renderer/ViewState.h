// ViewState.h
// 
#ifndef VIEWSTATE_ONCE
#define VIEWSTATE_ONCE

#include "Renderer_API.h"

#include "View/View.h"

#include "Math/Math.h"
#include "Math/Rectangle.h"
#include "Math/Ray.h"

namespace OpenGL {

class RENDERER_API ViewState
{
public:
	enum ProjectionMode { Perspective,Orthographic };

protected:
	ProjectionMode _projectionMode;

public:
	s2::View        view;
	Math::Rectangle viewport;
	Math::dmat4     model;

	// ------------------------------------------------------------------------------------------------
	ViewState( const ProjectionMode &pm = Perspective )
	: _projectionMode(pm)
	{}

	// ------------------------------------------------------------------------------------------------
	virtual ~ViewState()
	{}

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
	
	// ------------------------------------------------------------------------------------------------
	//Math::dmat4 viewMatrix()                              const { return Math::lookAt( view.eye(), view.target(), view.up()); }
	Math::dmat4 viewMatrix()                              const { return view.matrix(); }
	Math::dmat4 perspectiveMatrix()                       const { return Math::perspective( view.fieldOfViewY(), view.aspectRatio(), view.perspectiveNearPlaneDistance(), view.perspectiveFarPlaneDistance() ); }
	Math::dmat4 projectionMatrix()                        const { return isOrthographic() ? orthographicMatrix() : perspectiveMatrix(); }
	Math::dmat4 modelViewMatrix()                         const { return viewMatrix() * model; }
	Math::dmat4 modelViewPerspectiveMatrix()              const { return perspectiveMatrix() * modelViewMatrix(); }
	Math::dmat4 modelViewOrthographicMatrix()             const { return orthographicMatrix() * modelViewMatrix(); }

	// ------------------------------------------------------------------------------------------------
	double      pixelSize( const Math::dvec3 &p )                     const;
	Math::dray  rayAt( int pxlX, int pxlY )                           const;
	Math::dvec3 worldPoint( int pxlX, int pxlY, bool flipY = true )  const;
};

}


#endif