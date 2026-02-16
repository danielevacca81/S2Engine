// GPUStateMapper.cpp
//
#include "GPUStateMapper.h"

#include "RenderCommand.h"

#include "RenderCore/RenderState.h"
#include "RenderCore/ClearState.h"

using namespace s2;
using namespace s2::Renderer;

#pragma region Helper Functions
static inline void mapOpacityToBlending( const RenderMaterial& material, RenderCore::RenderState& renderState)
{
    /**
     * OPACITÀ → BLENDING
     * 
     * Questa è la mappatura più comune:
     * - Se opacity < 1.0 → abilitare alpha blending
     * - Se trasparente → disabilitare depth write (per evitare artifact di sorting)
     * - Se trasparente → mantenere depth test (per correct ordering)
     */
    const float opacity = material.opacity;
    const bool isTransparent = opacity < 1.0f;

    if (isTransparent)
    {
        // Abilitare blending per trasparenza
        renderState.blending.enabled[0] = true;

        // Standard alpha blending: output = src.rgb * src.a + dst.rgb * (1 - src.a)
        renderState.blending.sourceRGBFactor = RenderCore::Blending::Factor::SourceAlpha;
        renderState.blending.sourceAlphaFactor = RenderCore::Blending::Factor::One;
        renderState.blending.destinationRGBFactor = RenderCore::Blending::Factor::OneMinusSourceAlpha;
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

// ------------------------------------------------------------------------------------------------
static inline void mapDepthState( const RenderMaterial& material, RenderCore::RenderState& renderState)
{
/**
 * PROFONDITÀ → DEPTH TEST/WRITE
 * 
 * Esempi:
 * - Oggetto opaco normale: depth test = Less, depth write = true
 * - Skybox: depth test = Always, depth write = false
 * - Acqua/vetro: depth test = Less, depth write = false
 * - Shadow mapping (back faces): depth test = Greater, depth write = true
 */


    // Se il material ha proprietà custom depth, usarle
    // Per questo esempio, utilizziamo logica basata su proprietà comuni

    // Default: profondità normale
    bool isTransparent = material.opacity < 1.0f;

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

// ------------------------------------------------------------------------------------------------
static inline void mapFaceCulling( const RenderMaterial& material, RenderCore::RenderState& renderState)
{
/**
 * FACE CULLING
 * 
 * Esempi di mappatura:
 * - Material solido opaco: cullare back faces (default)
 * - Superficie trasparente (foglia, finestra): no culling
 * - Due-faccie (doppie normali): no culling
 * - Materiale disegnato "da dietro": cullare front faces
 */


    // Disabilitare culling per materiali trasparenti
    // (come foglie, vetro, fluidi)
    if (material.opacity < 1.0f)
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

// ------------------------------------------------------------------------------------------------
static inline void mapColorMask(const RenderMaterial& material,RenderCore::RenderState& renderState)
{
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


    // Default: scrivere tutti i canali
    renderState.colorMask.r = true;
    renderState.colorMask.g = true;
    renderState.colorMask.b = true;
    renderState.colorMask.a = true;

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

// ------------------------------------------------------------------------------------------------
static inline void mapStencilState( const RenderMaterial& material, RenderCore::RenderState& renderState)
{
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

#pragma endregion

// ------------------------------------------------------------------------------------------------
 RenderCore::ClearState   GPUStateMapper::map( const ClearCommand &clearCmd )
 {
    RenderCore::ClearState cs;

    switch( clearCmd.mode )
    {
    case s2::Renderer::ClearMode::ColorOnly:
        cs.buffers = RenderCore::ClearBuffers::ColorBuffer;
        cs.color = clearCmd.color;
        break;

    case s2::Renderer::ClearMode::DepthOnly:
        cs.buffers = RenderCore::ClearBuffers::DepthBuffer;
        cs.depth = clearCmd.depth;
        break;

    case s2::Renderer::ClearMode::ColorAndDepth:
        cs.buffers = RenderCore::ClearBuffers::ColorAndDepthBuffer;
        cs.color = clearCmd.color;
        cs.depth = clearCmd.depth;
        break;

    case s2::Renderer::ClearMode::StencilOnly:
        cs.buffers = RenderCore::ClearBuffers::StencilBuffer;
        cs.stencil = clearCmd.stencil;
        break;

    case s2::Renderer::ClearMode::DepthAndStencil:
        cs.buffers = RenderCore::ClearBuffers::StencilAndDepthBuffer;
        cs.depth = clearCmd.depth;
        cs.stencil = clearCmd.stencil;
        break;

    case s2::Renderer::ClearMode::AllBuffers:
        cs.buffers = RenderCore::ClearBuffers::All;
        cs.color = clearCmd.color;
        cs.depth = clearCmd.depth;
        cs.stencil = clearCmd.stencil;
        break;
    }

    return cs;
 }
 
// ------------------------------------------------------------------------------------------------
 RenderCore::RenderState  GPUStateMapper::map( const RenderCommand &renderCmd )
 {
    RenderCore::RenderState rs;

    // Mappare proprietà del material al render state
    mapOpacityToBlending ( renderCmd.material, rs );
    mapDepthState        ( renderCmd.material, rs );
    mapFaceCulling       ( renderCmd.material, rs );
    mapColorMask         ( renderCmd.material, rs );
    mapStencilState      ( renderCmd.material, rs );

    return rs;
 }