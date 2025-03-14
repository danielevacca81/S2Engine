// Brush.h
//
#ifndef RENDERER_BRUSH_H
#define RENDERER_BRUSH_H

#include "Core/Color.h"

namespace Renderer {

struct FillBrush
{
	Color color;
	//pattern ?
};

struct BorderBrush
{
	Color color;
	float thickness { 1.f };
};

}

#endif //!RENDERER_BRUSH_H