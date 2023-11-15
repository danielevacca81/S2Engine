// StateManager.h
//
#ifndef STATEMANAGER_ONCE
#define STATEMANAGER_ONCE

#include "RenderSystem_API.h"

#include "DrawState.h"
#include "ClearState.h"
#include "RenderState.h"

#include "Core/Color.h"

#include "Program.h"
#include "FrameBuffer.h"

namespace RenderSystem {

/* There shall be only one state manager per context.
   Useful to have a Context::map to handle context switches?
*/
class RENDERSYSTEM_API StateManager
{
public:
	StateManager();
	void disableShadowingOneShot() { _disableDrawStateShadowingOneShot = _disableClearStateShadowingOneShot = true; }

	void setClearState( const ClearState &cs );
	void setDrawState( const DrawState &ds );

	// for debugging purpose
	void debugState( const bool drawStateCheck = true, const bool clearStateCheck = true ) const;

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
	void applyDepthMask         ( const DepthMask& depthMask );
	void applyStencilMask       ( const StencilMask &stencilMask );
	void applyShaderProgram     ( const ProgramPtr  &program );
	void applyViewport          ( const Viewport &viewport );
	void applyClearColorSeparate( const ClearColorSeparate &clearColorSeparate );

private:
	bool         _disableDrawStateShadowingOneShot;
	bool         _disableClearStateShadowingOneShot;
	bool         _shadowingCurrentlyEnabled;
				 
	Color        _clearColor;
	float        _clearDepth;
	int          _clearStencil;
				 
	RenderState  _renderState;
	ProgramPtr   _currentShaderProgram;
};


}

#endif