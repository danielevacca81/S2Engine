// StateManager.h
//
#ifndef STATEMANAGER_ONCE
#define STATEMANAGER_ONCE

#include "s2Renderer_API.h"

#include "DrawingState.h"
#include "ClearState.h"
#include "RenderState.h"

#include "Core/Color.h"

#include "Program.h"
#include "FrameBuffer.h"

namespace Renderer {

/* There shall be only one state manager per context.
   Useful to have a Context::map to handle context switches?
*/

class S2RENDERER_API StateManager
{
public:
	//void setFrameBuffer( const FrameBufferPtr &fb );
	void setClearState( const ClearState &cs );
	void setDrawState( const DrawingState &ds );
	void setViewport ( const Viewport &vp );

private:
	void applyRenderState       ( const RenderState &rs );
	void applyPrimitiveRestart  ( const PrimitiveRestart &pr );
	void applyFaceCulling       ( const FaceCulling &fc );
	void applyProgramPointSize  ( const ProgramPointSize &programPointSize );
	void applyRasterizationMode ( const RenderState::RasterizationMode &rasterizationMode );
	void applyLineWidth         ( const float lineWidth );
	void applyScissorTest       ( const ScissorTest &scissorTest );
	void applyStencilTest       ( const StencilTest &stencilTest );
	void applyStencil           ( const FaceCulling::Face &face, StencilTestFace &currentTest, const StencilTestFace &test );
	void applyDepthTest         ( const DepthTest &depthTest );
	void applyDepthRange        ( const DepthRange &depthRange );
	void applyBlending          ( const Blending &blending );
	void applyColorMask         ( const ColorMask &colorMask );
	void applyDepthMask         ( bool depthMask );
	void applyStencilMask       ( const StencilMask &stencilMask );
	void applyShaderProgram     ( const ProgramPtr  &program );
	void applyViewport          ( const Viewport &viewport );
	//void applyViewState         ( const ViewState &viewState );
	void applyFramebuffer();	

private:
	Color              _clearColor;
	float              _clearDepth;
	int                _clearStencil;

	Viewport           _fullViewport;
	RenderState        _renderState;
	ProgramPtr         _currentShaderProgram;
	FrameBufferPtr     _currentFrameBuffer;
};


}

#endif