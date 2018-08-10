// Device.h
//
#ifndef DEVICE_ONCE
#define DEVICE_ONCE

#include "s2OpenGL_API.h"

#include <string>

namespace s2{
namespace OpenGL {

class S2OPENGL_API Device
{
public:
	static Device &get();

public:
	std::string info() const;

	int maxAttribPerVertex() const;
	int numberOfTextureUnits() const;

private:
	Device();

private:
};

}
}

#endif