// Device.cpp
//
#include "Device.h"

#include "OpenGL.h"
#include "OpenGLCheck.h"

#include <sstream>

// comment this to have static a MaxVertexAttrib value
//#define QUERY_GL_MAX_VERTEX_ATTRIBS

using namespace RenderSystem;

// ------------------------------------------------------------------------------------------------
int Device::maxAttribPerVertex()
{
#if defined QUERY_GL_MAX_VERTEX_ATTRIBS
	static int value = 0;
	if( value == 0 )
	{
		glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, &value );
		glCheck;
	}
#else
	static int value = 16;
#endif

	return value;
}

// ------------------------------------------------------------------------------------------------
int Device::numberOfTextureUnits()
{
	static int value = 0;
	if( value == 0 )
	{
		glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS, &value );
		glCheck;
	}
	return value;
}


// ------------------------------------------------------------------------------------------------
int Device::maxColorAttachmentPoints()
{
	static int value = 0;
	if( value == 0 )
	{
		glGetIntegerv( GL_MAX_COLOR_ATTACHMENTS, &value );
		glCheck;
	}
	return value;
}

// ------------------------------------------------------------------------------------------------
float Device::maxLinesWidth()
{
	static float maxWidth { -1.f };
	if( maxWidth < 0.f )
	{
		GLfloat lineWidthRange[2] = { 0.0f, 0.0f };
		glGetFloatv( GL_ALIASED_LINE_WIDTH_RANGE, lineWidthRange );
		maxWidth = lineWidthRange[1];
	}
	return maxWidth;
}

// ------------------------------------------------------------------------------------------------
float Device::minLinesWidth()
{
	static float minWidth { -1.f };
	if( minWidth < 0.f )
	{
		GLfloat lineWidthRange[2] = { 0.0f, 0.0f };
		glGetFloatv( GL_ALIASED_LINE_WIDTH_RANGE, lineWidthRange );
		minWidth = lineWidthRange[0];
	}

	return minWidth;
}