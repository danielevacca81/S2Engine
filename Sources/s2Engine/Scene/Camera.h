// Camera.h
//
#ifndef SCENE_CAMERA_H
#define SCENE_CAMERA_H

#include "s2Engine_API.h"


namespace s2 {
namespace Scene {

class S2ENGINE_API Camera
{
public:
	Camera() = default;


protected:
	// @todo: wrap Entity with Camera properties
	// 
	// 	struct CameraProperty {
	// 	double fov;
	// 	double nearPlane;
	// 	double farPlane;
	// 	Math::irect viewport;
	// };
	//
	// struct TransformProperty {
	//     Math::dvec3 position;
	//     Math::dvec3 rotation; // o Quaternion
	// };

};

}
}

#endif // !SCENE_CAMERA_H