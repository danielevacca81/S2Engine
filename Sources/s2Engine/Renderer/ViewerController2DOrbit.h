// ViewerController2DOrbit.h
//
#ifndef RENDERENGINE_VIEWERCONTROLLER2DORBIT_H
#define RENDERENGINE_VIEWERCONTROLLER2DORBIT_H

#include "RenderPipeline_API.h"

#include "ViewerController.h"

namespace RenderPipeline {

// ------------------------------------------------------------------------------------------------
class RENDERPIPELINE_API ViewerController2DOrbit: public ViewerController2D
{
public:
	ViewerController2DOrbit();

	void    setMinDragThreshold( const int thresholdInPixel ) { _minDragThreshold= thresholdInPixel; }
	//void    setRotationInverted( const bool invert );
	void    setMinZoomFactor( const double zoomFactor );

	bool    start();

	bool    doDragBegin( const Math::ivec2 &screenPos )  override;
	bool    doDragUpdate( const Math::ivec2 &screenPos )  override;
	bool    doDragEnd( const Math::ivec2 &screenPos )  override;

protected:
	void    updatePos( const Math::ivec2 &screenPos );

protected:
	int           _minDragThreshold;
	double        _rotationSpeed;
	double        _minZoomFactor;

	bool          _dragInProgress;
	Math::ivec2   _screenPickPos;
	Math::dvec3   _rotationOrigin;

	Math::dquat   _startQ;
	Math::dquat   _currQ;
	Math::dmat4   _startOrientation;

};


}

#endif // RENDERENGINE_VIEWERCONTROLLER3DORBIT_H



