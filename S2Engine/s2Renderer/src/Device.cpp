// Device.cpp
//
#include "Device.h"

#include "OpenGL.h"

using namespace Renderer;

// ------------------------------------------------------------------------------------------------
Device &Device::get()
{
	static Device d;
	return d;
}

// ------------------------------------------------------------------------------------------------
Device::Device()
{}

// ------------------------------------------------------------------------------------------------
std::string Device::info() const
{
	return "";
}

// ------------------------------------------------------------------------------------------------
int Device::maxAttribPerVertex() const
{
	int value = 0;
	glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, &value);
	glCheck;
	return value;
}

// ------------------------------------------------------------------------------------------------
int Device::numberOfTextureUnits() const
{
	int value = 0;
	glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS, &value);
	glCheck;
	return value;
}