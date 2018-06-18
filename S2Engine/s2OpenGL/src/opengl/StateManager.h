// StateManager.h
//
#ifndef STATEMANAGER_ONCE
#define STATEMANAGER_ONCE

#include "s2OpenGL_API.h"

#include "DrawState.h"
#include "ClearState.h"
#include "RenderState.h"
#include "ViewState.h"

#include "graphics/Color.h"

#include "Program.h"
//#include "VertexArray.h"
//#include "Mesh.h"

namespace s2 {
namespace OpenGL {

/* There shall be only one state manager per context.
   Useful to have a Context::map to handle context switches?
*/

class S2OPENGL_API StateManager
{
public:
	StateManager();
	~StateManager();

	void applyClearState( const ClearState &cs );
	void applyDrawState( const DrawState &ds );
	//void reset(); // @todo: clears internal status and 


	//void clear( const ClearState &cs = ClearState() );
	//
	//void draw( Primitive primitive, const VertexArray &va, const ViewState &vs = ViewState(), const DrawState &ds = DrawState() );
	//void draw( Primitive primitive, const Mesh &m, const ViewState &vs = ViewState(), const DrawState &ds = DrawState() );

private:
	void applyRenderState       ( const RenderState &rs );
	void applyPrimitiveRestart  ( const PrimitiveRestart &pr );
	void applyFaceCulling       ( const FaceCulling &fc );
	void applyProgramPointSize  ( const ProgramPointSize &programPointSize );
	void applyRasterizationMode ( const RenderState::RasterizationMode &rasterizationMode );
	void applyScissorTest       ( const ScissorTest &scissorTest );
	void applyStencilTest       ( const StencilTest &stencilTest );
	void applyStencil           ( const FaceCulling::Face &face, StencilTestFace &currentTest, const StencilTestFace &test );
	void applyDepthTest         ( const DepthTest &depthTest );
	void applyDepthRange        ( const DepthRange &depthRange );
	void applyBlending          ( const Blending &blending );
	void applyColorMask         ( const ColorMask &colorMask );
	void applyDepthMask         ( bool depthMask );
	void applyShaderProgram     ( const ProgramPtr &program );
	void applyViewState         ( const ViewState &viewState );
	//void applyFramebuffer();
	void setViewport            ( const Math::Rectangle &r );

private:
	Color              _clearColor;
	float              _clearDepth;
	int                _clearStencil;
	//Math::Rectangle  _viewport;
	RenderState        _renderState;
	ViewState          _viewState;
	ProgramPtr         _currentShaderProgram;
};


}
}

#endif