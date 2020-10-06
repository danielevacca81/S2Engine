// Device.h
//
#ifndef DEVICE_ONCE
#define DEVICE_ONCE

#include "s2Renderer_API.h"

#include <string>

namespace Renderer {

class S2RENDERER_API Device
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
#endif