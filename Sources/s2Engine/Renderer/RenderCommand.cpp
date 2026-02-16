// RenderCommand.cpp
//
#include "RenderCommand.h"

namespace s2 {
namespace Renderer {

#pragma region Helper Functions
static inline void mapOpacityToBlending( const RenderMaterial& material, RenderCore::RenderState& renderState )
{
	const bool isTransparent = material.blendMode != BlendMode::Opaque;

	if( isTransparent )
	{
		renderState.blending.enabled[0] = true;

		// Standard alpha blending: output          = src.rgb * src.a + dst.rgb * (1 - src.a)
		renderState.blending.sourceRGBFactor        = RenderCore::Blending::Factor::SourceAlpha;
		renderState.blending.sourceAlphaFactor      = RenderCore::Blending::Factor::One;
		renderState.blending.destinationRGBFactor   = RenderCore::Blending::Factor::OneMinusSourceAlpha;
		renderState.blending.destinationAlphaFactor = RenderCore::Blending::Factor::Zero;

		// @todo: support other blend modes (additive, multiplicative, etc.)
		renderState.blending.rgbEquation   = RenderCore::Blending::Equation::Add;
		renderState.blending.alphaEquation = RenderCore::Blending::Equation::Add;

		renderState.depthMask.enabled = false;
	}
	else
	{
		renderState.blending.enabled[0] = false;
		renderState.depthMask.enabled = true;
	}
}

// ------------------------------------------------------------------------------------------------
static inline void mapDepthState( const RenderMaterial& material, RenderCore::RenderState& renderState )
{
	const bool isTransparent = material.blendMode != BlendMode::Opaque;

	// Depth test
	renderState.depthTest.enabled = true;
	renderState.depthTest.function = RenderCore::DepthTest::DepthTestFunction::Less;

	// Depth write
	renderState.depthMask.enabled = !isTransparent; // Depth write disabled for transparent materials, enabled for opaque

	// renderState.depthRange.nearValue = 0.0f;
	// renderState.depthRange.farValue = 1.0f;
}

// ------------------------------------------------------------------------------------------------
static inline void mapFaceCulling( const RenderMaterial& material, RenderCore::RenderState& renderState )
{
	// always disable face culling for transparent materials
	if( material.blendMode == BlendMode::AlphaBlend )
	{
		renderState.faceCulling.enabled = false;
		return;
	}

	// For opaque materials, use cull mode from material
	switch( material.cullMode )
	{
	case CullMode::None:  renderState.faceCulling.enabled  = false; break;
	case CullMode::Front: renderState.faceCulling.cullFace = RenderCore::FaceCulling::Face::Front; break;
	case CullMode::Back:  renderState.faceCulling.cullFace = RenderCore::FaceCulling::Face::Back; break;
	}

	//renderState.faceCulling.frontFaceWindingOrder = RenderCore::FaceCulling::WindingOrder::CounterClockWise;
}

// ------------------------------------------------------------------------------------------------
static inline void mapColorMask( const RenderMaterial& material, RenderCore::RenderState& renderState )
{
	// default: use all channels for writing.
	// In a more complex implementation, you could have material properties that specify which channels to write to.
	renderState.colorMask.r = true;
	renderState.colorMask.g = true;
	renderState.colorMask.b = true;
	renderState.colorMask.a = true;
}

// ------------------------------------------------------------------------------------------------
static inline void mapStencilState( const RenderMaterial& material, RenderCore::RenderState& renderState )
{
	// no stencil operations by default
	renderState.stencilTest.enabled = false;

	// Example of stencil setup for shadow casting (if material.shadowCaster is true):
	 //if( material.shadowCaster )
	 //{
	 //    renderState.stencilTest.enabled = true;
	 //    renderState.stencilMask.front   = true;
	
		// // Front faces write +1 on stencil pass, keep on depth pass/fail
	 //    renderState.stencilTest.frontFace.function             = RenderCore::StencilTestFace::StencilTestFunction::Always;
	 //    renderState.stencilTest.frontFace.referenceValue       = 0;
	 //    renderState.stencilTest.frontFace.mask                 = 0xFF;
	 //    renderState.stencilTest.frontFace.stencilOnStencilPass = RenderCore::StencilTestFace::StencilOperation::IncrementWrap;
	 //    renderState.stencilTest.frontFace.depthPassStencilPassOperation = RenderCore::StencilTestFace::StencilOperation::Keep;
	 //    renderState.stencilTest.frontFace.depthFailStencilPassOperation = RenderCore::StencilTestFace::StencilOperation::Keep;

		// // Back faces: invert operation (decrement stencil on pass)
	 //    renderState.stencilTest.backFace                      = renderState.stencilTest.frontFace;
	 //    renderState.stencilTest.backFace.stencilOnStencilPass = RenderCore::StencilTestFace::StencilOperation::DecrementWrap;
	 //}
	//
	// // example for ui masking:
	// else if (material.usesStencilMask())
	// {
	//     renderState.stencilTest.enabled = true;
	//     renderState.stencilMask.enabled = false; // dont write, only test
	//
	//     renderState.stencilTest.frontFace.function = 
	//         StencilTestFace::StencilTestFunction::NotEqual;
	//     renderState.stencilTest.frontFace.referenceValue = 0;
	//     renderState.stencilTest.frontFace.mask = 0xFF;
	// }
}

#pragma endregion

// ------------------------------------------------------------------------------------------------
RenderCore::ClearState   getClearState( const ClearCommand& clearCmd )
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
RenderCore::RenderState  getRenderState( const RenderCommand& renderCmd )
{
	RenderCore::RenderState rs;

	// translate material properties to render state
	mapOpacityToBlending( renderCmd.material, rs );
	mapDepthState       ( renderCmd.material, rs );
	mapFaceCulling      ( renderCmd.material, rs );
	mapColorMask        ( renderCmd.material, rs );
	mapStencilState     ( renderCmd.material, rs );

	return rs;
}

} // namespace Renderer
} // namespace s2