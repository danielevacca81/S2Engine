// MaterialToGPUStateMapper.h
//
#ifndef S2_RENDERER_GPUSTATEMAPPER_H
#define S2_RENDERER_GPUSTATEMAPPER_H

#include "s2Engine_API.h"

#include "RenderCore/ClearState.h"
#include "RenderCore/RenderState.h"

namespace s2 {
namespace Renderer {

RenderCore::ClearState   getClearState ( const ClearCommand &clearCmd );
RenderCore::RenderState  getRenderState( const RenderCommand &renderCmd );


} // namespace Renderer
} // namespace s2


#endif // !S2_RENDERER_GPUSTATEMAPPER_H