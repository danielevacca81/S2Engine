// ViewerController3DGoTo.h
//
#ifndef RENDERENGINE_VIEWERCONTROLLER3DGOTO_H
#define RENDERENGINE_VIEWERCONTROLLER3DGOTO_H

#include "RenderPipeline_API.h"

#include "ViewerController.h"

#include <chrono>

namespace RenderPipeline {

// ------------------------------------------------------------------------------------------------
class RENDERPIPELINE_API ViewerController3DGoTo : public ViewerController3D
{
private:
	using time_point = std::chrono::time_point< std::chrono::high_resolution_clock >;

public:
	bool    start( const Math::ivec2 &screenPos, const double zoomMultiplier, const double animationDurationSec );

	bool    doAnimate() override;

protected:
	bool        _startPosOk = false;
	Math::dvec3 _startTargetPos;
	Math::dvec3 _startPos;
	double      _startTargetDist;

	bool        _endPosOk = false;
	Math::dvec3 _endTargetPos;
	Math::dvec3 _endPos;
	double      _endTargetDist;

	Math::dmat4 _cameraOrientation;
	double      _durationSec;		

	time_point  _startTime;	
};


}

#endif // RENDERENGINE_VIEWERCONTROLLER3DGOTO_H



