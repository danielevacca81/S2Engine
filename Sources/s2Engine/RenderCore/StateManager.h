// StateManager.h
//
#ifndef S2_RENDERCORE_STATEMANAGER_H
#define S2_RENDERCORE_STATEMANAGER_H

#include "s2Engine_API.h"

#include "DrawState.h"
#include "ClearState.h"
#include "RenderState.h"

#include "Graphics/Color.h"

#include "Shader.h"

namespace s2 {
namespace RenderCore {

/* There shall be only one state manager per context.
   Useful to have a Context::map to handle context switches?
*/
class StateManager
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
	void applyShaderProgram     ( const ShaderPtr  &shader);
	void applyViewportAndScissor( const ViewportState &vs);
	void applyClearColorSeparate( const ClearColorSeparate &clearColorSeparate );

private:
	bool         _disableDrawStateShadowingOneShot;
	bool         _disableClearStateShadowingOneShot;
	bool         _shadowingCurrentlyEnabled;

	// shadowed clear state to avoid redundant state changes. 
	Color        _clearColor;
	float        _clearDepth;
	int          _clearStencil;

	// shadowed state to avoid redundant state changes.
	RenderState   _renderState;
	ViewportState _viewportState;
	ShaderPtr     _currentShader;
};


} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_STATEMANAGER_H