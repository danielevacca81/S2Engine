// RenderModel.h
//
#ifndef S2_RENDERER_RENDERMODEL_H
#define S2_RENDERER_RENDERMODEL_H

#include "s2Engine_API.h"

#include "RenderCore/VertexData.h"

namespace s2 {
namespace Renderer {

struct S2ENGINE_API RenderModel
{
	RenderCore::VertexDataPtr vertexData;

	// Optional: add bounding box, material reference, etc. for culling and sorting
};

} // namespace Renderer
} // namespace s2



#endif // !S2_RENDERER_RENDERMODEL_H