// ViewerController2DGoTo.h
//
#ifndef ViewerController2DGoTo_ONCE
#define ViewerController2DGoTo_ONCE

#include "s2Scene_API.h"

#include "ViewerController.h"

#include <chrono>

namespace Scene {

// ------------------------------------------------------------------------------------------------
class S2SCENE_API ViewerController2DGoTo : public ViewerController2D
{
private:
	using time_point = std::chrono::time_point< std::chrono::high_resolution_clock >;

public:
	bool    start( const Math::dvec3 &destPos, const double zoomFactor, const double animationDurationSec );
	bool    start( const Math::ivec2 &screenPos, const double zoomMultiplier, const double animationDurationSec );

	bool    doAnimate() override;


protected:
	Math::dvec3 _startPos;
	double      _startZoomFactor;
	double      _startZoomFactorInv;

	time_point  _startTime;	
	double      _durationSec;		

	Math::dvec3 _endPos;
	double      _endZoomFactor;
	double      _endZoomFactorInv;
};


}

#endif // ViewerController2DGoTo_ONCE



