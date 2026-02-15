// MaterialToGPUStateMapper.h
// Esempio pratico di come mappare Material properties a GPU RenderState

#pragma once

#include "s2Engine_API.h"
#include "Resources/Material.h"
#include "RenderCore/DrawState.h"
#include "RenderCore/RenderState.h"

namespace s2 {
namespace Renderer {

/**
 * Helper class per traslare Material properties a GPU RenderState
 * Questo è quello che il MaterialBinder dovrebbe fare quando attivato
 */
class MaterialToGPUStateMapper
{
public:
    /**
     * Mappa le proprietà di un material sul render state di un DrawState
     * @param material - Il material con le proprietà da mappare
     * @param drawState - Il DrawState dove impostare il render state
     */
    static void mapMaterialToRenderState(
        const Resources::Material& material,
        RenderCore::DrawState& drawState);

private:
    // Helper functions per mapping specifico
    
    /**
     * Mappa l'opacity del material al blending state della GPU
     * Se opacity < 1.0, abilita blending alpha
     */
    static void mapOpacityToBlending(
        const Resources::Material& material,
        RenderCore::RenderState& renderState);

    /**
     * Mappa face culling mode
     * Back face culling è il default, ma transparent materials spesso lo disabilitano
     */
    static void mapFaceCulling(
        const Resources::Material& material,
        RenderCore::RenderState& renderState);

    /**
     * Mappa depth test/write basato su transparency e tipo di material
     */
    static void mapDepthState(
        const Resources::Material& material,
        RenderCore::RenderState& renderState);

    /**
     * Mappa color write mask (utile per compositing e effects)
     */
    static void mapColorMask(
        const Resources::Material& material,
        RenderCore::RenderState& renderState);

    /**
     * Mappa stencil buffer usage (shadow volumes, UI masking, ecc.)
     */
    static void mapStencilState(
        const Resources::Material& material,
        RenderCore::RenderState& renderState);
};

} // namespace Renderer
} // namespace s2
