// ViewState.h
// 
#ifndef VIEWSTATE_H
#define VIEWSTATE_H

#include "RenderCore_API.h"

#include "Math/Rectangle.h"

namespace RenderCore  {

struct ViewState
{
	Math::irect viewport { 0,0,0,0 };
};

}

#endif