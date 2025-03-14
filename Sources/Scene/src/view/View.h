// View.h
// 
#ifndef RENDERENGINE_VIEW_H
#define RENDERENGINE_VIEW_H

#include "RenderPipeline_API.h"

#include "Frustum.h"

#include "Math/Math.h"
#include "Math/Rectangle.h"

namespace RenderPipeline {

class RENDERPIPELINE_API View
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



/*
const T pixelSizeAtPosition(const Foundation::Vector3<T>& v) const
		{
			T factor;
			if(flags(Ortho))
			{
				Foundation::Vector2<T> scrPixels((T)mSize.x, (T)mSize.y);//TBD: len() does not compile with int32 on vc10
				factor = mOrthoSize.len() / scrPixels.len();
			}
			else
			{
				Foundation::Vector3<T> pointToCamera;
				Foundation::Vector3<T>::sub(mFrom, v, pointToCamera);
				factor = pointToCamera.len() * mPerspectiveScale;
			}
			return factor;
		}

		const T pixelSizeAtPosition(const Foundation::Vector3<T>& v, Foundation::Vector2<T>& s) const
		{
			T factor;
			if(flags(Ortho))
			{
				Foundation::Vector2<T> scrPixels((T)mSize.x, (T)mSize.y);
				factor = mOrthoSize.len() / ((T) scrPixels.len());
				T v = mOrthoSize.x * mOrthoSize.x;
				if (v)
					v = sqrt(v);
				s.x = v / (T) mSize.x;
				v = mOrthoSize.y * mOrthoSize.y;
				if (v)
					v = sqrt(v);
				s.y = v / (T) mSize.y;
			}
			else
			{
				Foundation::Vector3<T> pointToCamera;
				Foundation::Vector3<T>::sub(mFrom, v, pointToCamera);
				factor = pointToCamera.len()* mPerspectiveScale;
			}
			return factor;
		}


*/


private:
	void update();

private:
	Math::irect _viewport;
	Math::dmat4 _projectionMatrix;
	Math::dmat4 _viewMatrix;
	Math::dmat4 _modelMatrix;
	Math::dmat4 _normalMatrix;
	Math::dvec4 _cameraEye;
	Frustum     _frustum;
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

#endif