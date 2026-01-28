// Device.h
//
#ifndef DEVICE_H
#define DEVICE_H

#include "s2Engine_API.h"

namespace RenderCore {

class S2ENGINE_API Device
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
	static int    maxTextureSize();
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