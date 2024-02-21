// Device.h
//
#ifndef DEVICE_ONCE
#define DEVICE_ONCE

#include "RenderSystem_API.h"

#include <string>
#include <set>

namespace RenderSystem {

class RENDERSYSTEM_API Device
{
public:
	enum class Vendor
	{
		Intel,
		AMD,
		Nvidia,
		Unknown,
	};

public:
	static Vendor vendor();
	static int    maxAttribPerVertex();
	static int    numberOfTextureUnits();
	static int    maxColorAttachmentPoints();
	static float  maxLinesWidth();
	static float  minLinesWidth();

public:
	Device() = delete;
};

}
#endif