// VertexAttributeLocation.h
//
#ifndef VERTEXATTRIBUTELOCATION_ONCE
#define VERTEXATTRIBUTELOCATION_ONCE

namespace Renderer {

enum VertexAttributeLocation: unsigned char
{
	VA_Position           = 0,
	VA_Color              = 1,
	VA_Normal             = 2,
	VA_UVCoords           = 3,
	VA_CustomInt          = 4,
	VA_CustomFloat        = 5,
	VA_CustomFloatVector2 = 6,
	VA_CustomFloatVector3 = 7,
	VA_CustomFloatVector4 = 8,
	VA_CustomIntVector2   = 9,
	VA_CustomIntVector3   = 10,
	VA_CustomIntVector4   = 11,
};

}
#endif
