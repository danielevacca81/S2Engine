// ViewerController3DGoTo.h
//
#ifndef ViewerController3DGoTo_ONCE
#define ViewerController3DGoTo_ONCE

#include "s2Scene_API.h"

#include "ViewerController.h"

#include <chrono>

namespace Scene {

// ------------------------------------------------------------------------------------------------
class S2SCENE_API ViewerController3DGoTo : public ViewerController3D
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

#endif // ViewerController3DGoTo_ONCE



