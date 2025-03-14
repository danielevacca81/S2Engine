// Scene.h
//
#ifndef SCENE_H
#define SCENE_H

#include "RenderPipeline_API.h"

#include "Camera.h"

namespace RenderPipeline {

class RENDERPIPELINE_API Scene //@todo add entity handling methods based on enTT
{
public:
	virtual void init()   = 0;
	virtual void deinit() = 0;
	virtual void update() = 0;
	virtual void clear()  = 0;
	virtual void draw()   = 0;

protected:							     
	Camera _camera;
};

}
#endif