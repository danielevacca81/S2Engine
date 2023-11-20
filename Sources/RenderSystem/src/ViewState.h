// ViewState.h
// 
#ifndef VIEWSTATE_ONCE
#define VIEWSTATE_ONCE

#include "RenderSystem_API.h"

#include "Math/Rectangle.h"

struct ViewState
{
	Math::irect viewport { 0,0,0,0 };
};


#endif