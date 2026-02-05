// ViewerController3DOrbitAnimation.h
//
#ifndef RENDERENGINE_VIEWERCONTROLLER3DORBITANIMATION_H
#define RENDERENGINE_VIEWERCONTROLLER3DORBITANIMATION_H

#include "RenderPipeline_API.h"

#include "ViewerController.h"

#include <chrono>

namespace RenderPipeline {

// ------------------------------------------------------------------------------------------------
class RENDERPIPELINE_API ViewerController3DOrbitAnimation : public ViewerController3D
{
private:
	using time_point = std::chrono::time_point< std::chrono::high_resolution_clock >;

public:
	bool    start( const Math::dvec3 &origin,
				   const double startTheta, const double startPhi, const double startRho,
				   const double endTheta, const double endPhi, const double endRho,
				   const double animationDurationSec );
	bool    start( const double endTheta, const double endPhi, const double endRho,
				   const double animationDurationSec );

	bool    doAnimate() override;

protected:
	bool        _validOrigin = false;
	Math::dvec3 _origin;

	double      _startTheta;
	double      _startPhi;
	double      _startRho;

	double      _endTheta;
	double      _endPhi;
	double      _endRho;

	time_point  _startTime;	
	double      _durationSec;	
};


}

#endif // RENDERENGINE_VIEWERCONTROLLER3DORBITANIMATION_H



