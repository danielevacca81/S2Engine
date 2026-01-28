// PickerConstants.h
//
#ifndef PICKERCONSTANTS_H
#define PICKERCONSTANTS_H

#include "s2Engine_API.h"

#include "Math/Math.h"

#include <limits>
#include <cstdint>

namespace RenderCore {


struct S2ENGINE_API PickerConstants
{
	using Value = int32_t;

	static constexpr Value		kMaxValue { std::numeric_limits<Value>::max() };
	static constexpr Value		kInvalidValueBegin { (kMaxValue / 4) * 3 };
	static constexpr Value      kClearValue { kMaxValue };
	static const Math::ivec4    kClearColor; 
};

}
#endif