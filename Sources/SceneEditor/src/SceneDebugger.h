// SceneDebugger.h
//
#ifndef SCENE_H
#define SCENE_H

#include "SceneManager_API.h"



#include <memory>

namespace SceneManager {

class RENDERPIPELINE_API SceneDebugger
{
public:
	static void logEvent( std::shared_ptr<ObjectInteraction> &o, const SceneEvent    &ev, const char *str );
	static void logEvent( std::shared_ptr<ObjectInteraction> &o, const MouseEventObj &ev, const char *str );

};

}
#endif