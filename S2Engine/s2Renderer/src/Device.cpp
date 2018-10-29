// Device.cpp
//
#include "Device.h"

#include "OpenGL.h"

using namespace s2::Renderer;

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
	return value;
}

// ------------------------------------------------------------------------------------------------
int Device::numberOfTextureUnits() const
{
	int value = 0;
	glGetIntegerv( GL_MAX_TEXTURE_UNITS, &value);
	return value;
}