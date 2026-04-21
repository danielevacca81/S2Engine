// Device.h
//
#ifndef S2_RENDERCORE_DEVICE_H
#define S2_RENDERCORE_DEVICE_H

#include "s2Engine_API.h"

namespace s2 {
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

	static int    availableGPUMemory();
	static int    totalGPUMemory();
	static int    dedicatedGPUTotalMemory();


public:
	Device() = delete;
};

} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_DEVICE_H