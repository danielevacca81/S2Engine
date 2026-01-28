// ViewerController3DOrbit.h
//
#ifndef RENDERENGINE_VIEWERCONTROLLER3DORBIT_H
#define RENDERENGINE_VIEWERCONTROLLER3DORBIT_H

#include "RenderPipeline_API.h"

#include "ViewerController.h"

namespace RenderPipeline {

// ------------------------------------------------------------------------------------------------
class RENDERPIPELINE_API ViewerController3DOrbit : public ViewerController3D
{
public:
	ViewerController3DOrbit();

	void    setMinDragThreshold( const int thresholdInPixel ) { _minDragThreshold= thresholdInPixel; }
	void    setHorizontalRotInverted( const bool invert );
	void    setVerticalRotInverted  ( const bool invert );
	void    setMinTiltAngle         ( const double angle );
	void    setMinZoomFactor        ( const double zoomFactor ); 

	bool    start();

	bool    doDragBegin ( const Math::ivec2 &screenPos )  override;
	bool    doDragUpdate( const Math::ivec2 &screenPos )  override;
	bool    doDragEnd   ( const Math::ivec2 &screenPos )  override;

protected:
	void    updatePos( const Math::ivec2 &screenPos );

protected:
	bool		  _pivotPointUnderDragBegin; // if it is false, rotation is relative to the terrain position at the center of the screen 
	int           _minDragThreshold;
	double        _horizontalRotSpeed;
	double        _verticalRotSpeed;
	double        _minTiltAngle;
	double        _minZoomFactor;

	bool          _dragInProgress;
	Math::ivec2   _screenPickPos;
	Math::dvec3   _rotationOrigin;
	double	      _minPhiAngle;  

	/*

            z
			|
            |    /|
            |   / |
            |  /  |
            | /p  |
            |/_)__|______y
           / \ )  |
          /___\)  |
         /  t  \  |
        /       \ |
       /         \|
      x

	*/
	double        _startTheta;    // 
	double		  _startPhi;	  // in degrees [-90, +90]
	Math::dvec3	  _cameraOffset;  // in camera local coordinates

	double        _lastTheta;  // 
	double		  _lastPhi;	   // in degrees [-90, +90]
};


}

#endif // RENDERENGINE_VIEWERCONTROLLER3DORBIT_H



