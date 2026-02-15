// MaterialToGPUStateMapper.cpp
// Implementazione pratica del mapping Material → GPU RenderState

#include "MaterialToGPUStateMapper.h"
#include "Math/MathCommon.h"

namespace s2 {
namespace Renderer {

// ================================================================================================
// PUBLIC INTERFACE
// ================================================================================================

void MaterialToGPUStateMapper::mapMaterialToRenderState(
    const Resources::Material& material,
    RenderCore::DrawState& drawState)
{
    auto& renderState = drawState.renderState;

    // Applicare mappature in ordine di importanza
    mapOpacityToBlending(material, renderState);
    mapDepthState(material, renderState);
    mapFaceCulling(material, renderState);
    mapColorMask(material, renderState);
    mapStencilState(material, renderState);
}

// ================================================================================================
// MAPPING IMPLEMENTATION
// ================================================================================================

/**
 * OPACITÀ → BLENDING
 * 
 * Questa è la mappatura più comune:
 * - Se opacity < 1.0 → abilitare alpha blending
 * - Se trasparente → disabilitare depth write (per evitare artifact di sorting)
 * - Se trasparente → mantenere depth test (per correct ordering)
 */
void MaterialToGPUStateMapper::mapOpacityToBlending(
    const Resources::Material& material,
    RenderCore::RenderState& renderState)
{
    const float opacity = material.opacity();
    const bool isTransparent = opacity < 1.0f;

    if (isTransparent)
    {
        // Abilitare blending per trasparenza
        renderState.blending.enabled[0] = true;

        // Standard alpha blending: output = src.rgb * src.a + dst.rgb * (1 - src.a)
        renderState.blending.sourceRGBFactor = RenderCore::Blending::Factor::SrcAlpha;
        renderState.blending.sourceAlphaFactor = RenderCore::Blending::Factor::One;
        renderState.blending.destinationRGBFactor = RenderCore::Blending::Factor::OneMinusSrcAlpha;
        renderState.blending.destinationAlphaFactor = RenderCore::Blending::Factor::Zero;

        renderState.blending.rgbEquation = RenderCore::Blending::Equation::Add;
        renderState.blending.alphaEquation = RenderCore::Blending::Equation::Add;

        // IMPORTANTE: Disabilitare depth write per materiali trasparenti
        // Questo è cruciale per il correct compositing di layer trasparenti
        renderState.depthMask.enabled = false;
    }
    else
    {
        // Disabilitare blending per materiali opachi
        renderState.blending.enabled[0] = false;

        // Riscrivere depth per materiali opachi
        renderState.depthMask.enabled = true;
    }
}

/**
 * PROFONDITÀ → DEPTH TEST/WRITE
 * 
 * Esempi:
 * - Oggetto opaco normale: depth test = Less, depth write = true
 * - Skybox: depth test = Always, depth write = false
 * - Acqua/vetro: depth test = Less, depth write = false
 * - Shadow mapping (back faces): depth test = Greater, depth write = true
 */
void MaterialToGPUStateMapper::mapDepthState(
    const Resources::Material& material,
    RenderCore::RenderState& renderState)
{
    // Se il material ha proprietà custom depth, usarle
    // Per questo esempio, utilizziamo logica basata su proprietà comuni

    // Default: profondità normale
    bool isTransparent = material.opacity() < 1.0f;

    // Profondità test
    renderState.depthTest.enabled = true;
    renderState.depthTest.function = RenderCore::DepthTest::DepthTestFunction::Less;

    // Profondità write (già impostato da mapOpacityToBlending per trasparenti)
    if (!isTransparent) {
        renderState.depthMask.enabled = true;
    }

    // Range di profondità (default è corretto nella maggior parte dei casi)
    // renderState.depthRange.nearValue = 0.0f;
    // renderState.depthRange.farValue = 1.0f;
}

/**
 * FACE CULLING
 * 
 * Esempi di mappatura:
 * - Material solido opaco: cullare back faces (default)
 * - Superficie trasparente (foglia, finestra): no culling
 * - Due-faccie (doppie normali): no culling
 * - Materiale disegnato "da dietro": cullare front faces
 */
void MaterialToGPUStateMapper::mapFaceCulling(
    const Resources::Material& material,
    RenderCore::RenderState& renderState)
{
    // Disabilitare culling per materiali trasparenti
    // (come foglie, vetro, fluidi)
    if (material.opacity() < 1.0f)
    {
        renderState.faceCulling.enabled = false;
        return;
    }

    // Default: cullare back faces
    renderState.faceCulling.enabled = true;
    renderState.faceCulling.cullFace = RenderCore::FaceCulling::Face::Back;
    renderState.faceCulling.frontFaceWindingOrder = RenderCore::FaceCulling::WindingOrder::CounterClockWise;

    // Nota: se il material ha una proprietà custom cullMode, si potrebbe fare:
    // switch (material.cullMode()) {
    //     case Material::CullMode::None:
    //         renderState.faceCulling.enabled = false;
    //         break;
    //     case Material::CullMode::Front:
    //         renderState.faceCulling.cullFace = FaceCulling::Face::Front;
    //         break;
    //     case Material::CullMode::Back:
    //         renderState.faceCulling.cullFace = FaceCulling::Face::Back;
    //         break;
    // }
}

/**
 * COLOR MASK
 * 
 * Questo è usato per:
 * - Selezionare quali canali di colore scrivere (R, G, B, A)
 * - Utile per rendering in layer specifici (normal map buffer, ecc.)
 * - Compositing e post-processing
 * 
 * Esempio: material che renderizza solo nel canale Alpha
 * (per costruire una mappa di silhouette per ambient occlusion)
 */
void MaterialToGPUStateMapper::mapColorMask(
    const Resources::Material& material,
    RenderCore::RenderState& renderState)
{
    // Default: scrivere tutti i canali
    renderState.colorMask.red = true;
    renderState.colorMask.green = true;
    renderState.colorMask.blue = true;
    renderState.colorMask.alpha = true;

    // Se il material è marcato come "write only alpha", eseguire questo:
    // (questo permetterebbe di usare un material per scrivere solo in alpha channel)
    // if (material.hasCustomColorMask())
    // {
    //     if (!material.writeRed()) renderState.colorMask.red = false;
    //     if (!material.writeGreen()) renderState.colorMask.green = false;
    //     if (!material.writeBlue()) renderState.colorMask.blue = false;
    //     if (!material.writeAlpha()) renderState.colorMask.alpha = false;
    // }
}

/**
 * STENCIL STATE
 * 
 * Usato per:
 * - Shadow volumes (incrementare/decrementare stencil per back/front faces)
 * - UI masking (disegnare forme nel buffer stencil, poi usare per maskare UI)
 * - Mirror effects (disegnare il mirror nel stencil, poi disegnare solo dentro quel'area)
 * 
 * Questo è un mapping più avanzato:
 */
void MaterialToGPUStateMapper::mapStencilState(
    const Resources::Material& material,
    RenderCore::RenderState& renderState)
{
    // Per default: stencil disabilitato
    renderState.stencilTest.enabled = false;

    // Se il material è marcato come "shadow caster" (volume shadow):
    // Questo è un esempio avanzato
    // if (material.isShadowCaster())
    // {
    //     renderState.stencilTest.enabled = true;
    //     renderState.stencilMask.enabled = true;
    //
    //     // Front faces: scrivere nel stencil
    //     renderState.stencilTest.frontFace.function = StencilTestFace::StencilTestFunction::Always;
    //     renderState.stencilTest.frontFace.referenceValue = 0;
    //     renderState.stencilTest.frontFace.mask = 0xFF;
    //     renderState.stencilTest.frontFace.stencilOnStencilPass = 
    //         StencilTestFace::StencilOperation::IncrementWrap;
    //     renderState.stencilTest.frontFace.stencilOnDepthPass = 
    //         StencilTestFace::StencilOperation::Keep;
    //     renderState.stencilTest.frontFace.stencilOnDepthFail = 
    //         StencilTestFace::StencilOperation::Keep;
    //
    //     // Back faces: operazione inversa
    //     renderState.stencilTest.backFace = renderState.stencilTest.frontFace;
    //     renderState.stencilTest.backFace.stencilOnStencilPass = 
    //         StencilTestFace::StencilOperation::DecrementWrap;
    // }
    //
    // // Per UI masking:
    // else if (material.usesStencilMask())
    // {
    //     renderState.stencilTest.enabled = true;
    //     renderState.stencilMask.enabled = false;  // Non scrivere, solo leggere
    //
    //     renderState.stencilTest.frontFace.function = 
    //         StencilTestFace::StencilTestFunction::NotEqual;
    //     renderState.stencilTest.frontFace.referenceValue = 0;
    //     renderState.stencilTest.frontFace.mask = 0xFF;
    // }
}

// ================================================================================================
// ESEMPIO DI USO / USAGE EXAMPLE
// ================================================================================================

/*
 * Scenario: Renderizzare un materiale trasparente (vetro, acqua)
 *
 * Material properties:
 *   - opacity: 0.7
 *   - baseColor: #4488FF (blu)
 *   - roughness: 0.2
 *   - metallic: 0.0
 *
 * Mappatura che avviene:
 *   1. mapOpacityToBlending:
 *      - renderState.blending.enabled = true
 *      - renderState.blending.sourceRGBFactor = SrcAlpha
 *      - renderState.depthMask.enabled = false  ← cruciale!
 *
 *   2. mapDepthState:
 *      - renderState.depthTest.enabled = true
 *      - renderState.depthTest.function = Less
 *
 *   3. mapFaceCulling:
 *      - renderState.faceCulling.enabled = false  ← perché trasparente
 *
 *   4. mapColorMask:
 *      - tutti i canali abilitati (default)
 *
 *   5. mapStencilState:
 *      - stencil disabilitato (default)
 *
 * Risultato GPU:
 *   - Schermi trasparenti con alpha blending
 *   - Nessun depth write → evita z-fighting per layer trasparenti
 *   - Nessun face culling → shader deve gestire normali invertite se necessario
 *   - Depth test abilitato → correct interleaving con geometria opaca
 */

} // namespace Renderer
} // namespace s2
