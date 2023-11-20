// SceneEvent.cpp
//
#include "SceneEvent.h"

#include <chrono>

using namespace Scene;

// ------------------------------------------------------------------------------------------------
SceneEvent::SceneEvent()
{
	using namespace std::chrono;
	timeMs = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

