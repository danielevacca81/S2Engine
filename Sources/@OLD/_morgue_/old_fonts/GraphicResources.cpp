// GraphicResources.cpp
//
#include "GraphicResources.h"

#include "Fonts/FontNew.h"

using namespace s2;

int GraphicResources::initialized = 0;

// ------------------------------------------------------------------------------------------------
bool GraphicResources::init( const std::wstring &resourcePath  )
{
	initialized++;

	if( initialized > 1 )
		return true;

	std::wstring path = resourcePath;

	if( path.find_last_of(L"/\\") == path.npos )
		path += L"/";

	//FontsLibrary::init( resourcePath );

	//fonts = OpenGL::FontTexture("Arial small");
	//bool fnt = fonts.isLoaded() ? true : fonts.load(path + "arial_small.fnt");

	return true;
}

// ------------------------------------------------------------------------------------------------
bool GraphicResources::destroy()
{
	initialized--;

	if( initialized > 0 )
		return false;
	return true;
}