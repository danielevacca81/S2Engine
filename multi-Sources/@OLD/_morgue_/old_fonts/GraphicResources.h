// GraphicResources.h
//
#ifndef GRAPHICRESOURCES_ONCE
#define GRAPHICRESOURCES_ONCE

#include "S2RendererAPI.h"

#include <string>

namespace s2{

class S2RENDERER_API GraphicResources
{
private:
	static int initialized;

public:
	static bool init( const std::wstring &resoucePath = std::wstring(L"") );
	static bool destroy();
};

}


#endif