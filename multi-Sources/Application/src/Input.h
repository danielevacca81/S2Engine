// Input.h
//
#ifndef APPLICATION_INPUT_H
#define APPLICATION_INPUT_H

#include "Application_API.h"

#include "MouseState.h"
#include "KeyboardState.h"

namespace s2 {

namespace Input{

// @todo: make it configurable
class APPLICATION_API Settings
{
public:
	static constexpr uint32_t kDragThreshold_px    { 2 };
	static constexpr uint32_t kDoubleClickTime_ms  { 200 }; // 200 ms for double click
};

}

}
#endif