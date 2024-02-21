// Device.cpp
//
#include "Device.h"

#include "OpenGL.h"
#include "OpenGLCheck.h"

#include <sstream>
#include <optional>
#include <algorithm>

// comment this to have static a MaxVertexAttrib value
//#define QUERY_GL_MAX_VERTEX_ATTRIBS

using namespace RenderSystem;

// ------------------------------------------------------------------------------------------------
Device::Vendor Device::vendor()
{
	static std::optional<Vendor> v;
	if( !v.has_value() )
	{
		const auto glVendorStringLowercase = [] ()
		{
			auto s = std::string( (char*) glGetString( GL_VENDOR ) );
			std::transform( s.begin(), s.end(), s.begin(), ::tolower );
			return s;
		};

		const auto vendorString = glVendorStringLowercase();

		if     ( vendorString.find( "intel" )  != vendorString.npos ) v = Vendor::Intel;
		else if( vendorString.find( "amd" )    != vendorString.npos ) v = Vendor::AMD;
		else if( vendorString.find( "nvidia" ) != vendorString.npos ) v = Vendor::Nvidia;
		else
			v = Vendor::Unknown;

	}
	return *v;
}


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