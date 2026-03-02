// StateManager.h
//
#ifndef S2_RENDERCORE_STATEMANAGER_H
#define S2_RENDERCORE_STATEMANAGER_H

#include "s2Engine_API.h"

#include "RenderState.h"
#include "ClearState.h"
#include "DrawState.h"

namespace s2 {
namespace RenderCore {

/************************************************************************************************/
/*                                      StateManager                                            */
/************************************************************************************************/
// Manages OpenGL state with shadowing to minimize redundant state changes
// Modernized for DSA and bindless textures
class S2ENGINE_API StateManager
{
public:
    StateManager();

    // ===== State Application =====
    
    // Apply clear state and perform clear
    void setClearState( const ClearState& clearState );
    
    // Apply draw state (DSA-aware)
    void setDrawState( const DrawState& drawState );

    // ===== State Shadowing Control =====
    
    // Disable shadowing for next draw (forces all state updates)
    void disableDrawStateShadowing() { /*_disableDrawStateShadowingOneShot = true; */}
    
    // Disable shadowing for next clear (forces all state updates)
    void disableClearStateShadowing() { /*_disableClearStateShadowingOneShot = true;*/ }
    
    // Get current shader
    const ShaderPtr& currentShader() const { return _currentShader; }

    // ===== Debug =====
    
    // Validate current state against OpenGL (debug only)
    void validateState( bool drawState = true, bool clearState = false ) const;

private:
    // Apply individual state components
    void applyRenderState( const RenderState& renderState );
    void applyViewport( const ViewportState& viewport );
    void applyShaderProgram( const ShaderPtr& shader );

    // Individual state setters
    void applyPrimitiveRestart( const PrimitiveRestart& pr );
    void applyFaceCulling( const FaceCulling& fc );
    void applyProgramPointSize( const ProgramPointSize& pps );
    void applyRasterizationMode( RenderState::RasterizationMode mode );
    void applyLineWidth( float lineWidth );
    void applyScissorTest( const ScissorTest& scissorTest );
    void applyStencilTest( const StencilTest& stencilTest );
    void applyStencil( FaceCulling::Face face, StencilTestFace& current, const StencilTestFace& target );
    void applyDepthTest( const DepthTest& depthTest );
    void applyDepthRange( const DepthRange& depthRange );
    void applyBlending( const Blending& blending );
    void applyColorMask( const ColorMask& colorMask );
    void applyDepthMask( const DepthMask& depthMask );
    void applyStencilMask( const StencilMask& stencilMask );
    void applyClearColorSeparate( const ClearColorSeparate& clearColorSeparate );

private:
    // Cached state
    RenderState    _renderState;
    ViewportState  _viewportState;
    ShaderPtr      _currentShader;

    // Clear state cache
    Color    _clearColor { 0.f, 0.f, 0.f, 0.f };
    float    _clearDepth { 1.0f };
    int32_t  _clearStencil { 0 };
};

} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_STATEMANAGER_H