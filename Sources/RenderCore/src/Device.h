// Device.h
//
#ifndef DEVICE_H
#define DEVICE_H

#include "RenderCore_API.h"

#include <string>
#include <set>

namespace RenderCore {

class RENDERCORE_API Device
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