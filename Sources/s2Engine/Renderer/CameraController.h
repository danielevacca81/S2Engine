// CameraController.h
//
#ifndef GRAPHICS_CAMERACONTROLLER_H
#define GRAPHICS_CAMERACONTROLLER_H

#include "s2Engine_API.h"

#include "Camera.h"

#include "Math/Math.h"
#include "Math/Box.h"

#include <functional>
#include <vector>

namespace Graphics {

// ------------------------------------------------------------------------------------------------
class S2ENGINE_API AnimatedController
{
public:
	enum class Status
	{
		Idle,
		InProgress,
		Done,
	};


	virtual ~AnimatedController() = default;

	virtual void update( Camera& camera, double deltaTime ) = 0;

	virtual bool dragBegin ( const Math::ivec2& screenPos ) = 0;
	virtual bool dragUpdate( const Math::ivec2& screenPos ) = 0;
	virtual bool dragEnd   ( const Math::ivec2& screenPos ) = 0;

};

// ------------------------------------------------------------------------------------------------
class S2ENGINE_API OrthographicCamera
{
public:
	static void  zoomToFit( Camera& c, const Math::dbox3& b );
	static void  zoomToBeFilledBy( Camera& c, const Math::dbox3& b );
};

}

#endif // !GRAPHICS_CAMERACONTROLLER_H