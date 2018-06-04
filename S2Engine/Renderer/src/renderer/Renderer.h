// Renderer.h
//
#ifndef RENDERER_ONCE
#define RENDERER_ONCE

#include "Renderer_API.h"

#include "DrawState.h"
#include "ClearState.h"
#include "RenderState.h"
#include "ViewState.h"

#include "graphics/Color.h"

#include "OpenGL/Program.h"
#include "OpenGL/VertexArray.h"
#include "OpenGL/Mesh.h"

//#include "Math/Rectangle.h"

//namespace OpenGL {
//	class FrameBuffer;
//}

namespace OpenGL {

class RENDERER_API Renderer
{
private:
	Color          _clearColor;
	float              _clearDepth;
	int                _clearStencil;
	//Math::Rectangle  _viewport;
	RenderState        _renderState;
	ViewState          _viewState;
	OpenGL::ProgramPtr _currentShaderProgram;
	
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
	void applyShaderProgram     ( const OpenGL::ProgramPtr &program );
	void applyViewState         ( const ViewState &viewState );
	//void applyFramebuffer();
	void setViewport            ( const Math::Rectangle &r );

public:
	Renderer();
	~Renderer();

	//Math::Rectangle viewport() const;
	
	//void setFramebuffer( const FrameBuffer &fbo );
	//void setViewMatrix( const Math::dmat4 &vm );
	//void setModelMatrix( const Math::dmat4 &mm );
	//void setProjectionMatrix( const Math::dmat4 &pm );

	void clear( const ClearState &cs = ClearState() );
	
	//void draw( PrimitiveType primitive, const OpenGL::VertexBuffer &vb, const ViewState &vs = ViewState(), const DrawState &ds = DrawState() );
	void draw( PrimitiveType primitive, const VertexArray &va, const ViewState &vs = ViewState(), const DrawState &ds = DrawState() );
	void draw( PrimitiveType primitive, const Mesh &m, const ViewState &vs = ViewState(), const DrawState &ds = DrawState() );

	//void drawText();
	//void addText( const Math::dvec3 &worldPos, const std::wstring &text, const Color &color,  const FontPtr &font = FontsLibrary::font("Default",12), TextRenderer::TextAlignment alignment = TextRenderer::AlignLeft );
	//void addText( const Math::ivec2 &screenPos, const std::wstring &text, const Color &color, const FontPtr &font = FontsLibrary::font("Default",12), TextRenderer::TextAlignment alignment = TextRenderer::AlignLeft );

/*
#ifdef EXPERIMENTAL
	void setProjection( const Projection &prj );
	void setView( const View &view );
	void setModelPipeline( const Pipeline &matrix );
	void setDrawState( const DrawState &ds );
	void setBackground( const Background &bkg );
#endif

	Math::dvec3 normalizedDeviceCoordinates( const Math::ivec2 &pixelCoordinates, bool flipY = true ) const;
	
	Math::dvec3 convertWindowTo3D(int x, int y, bool flipY = true ) const ;
	void        convertWindowTo3D(int x, int y, Math::dvec3& near, Math::dvec3& far, bool flipY = true )const;
	Math::dvec2 convertWindowTo2D(int x, int y, bool flipY = true ) const ;
	Math::dvec3 convertWorldToWindow(const Math::dvec3 &p, bool flipY = true ) const ;
	Math::dvec3 convertWorldToWindow(const Math::dvec2 &p, bool flipY = true ) const ;
	double	    pixelToWorldSize( float pixels = 1.f) const;
	int         worldSizeToPixels( double worldSize = 1.0) const;
	
	Selection  selection()  const;
	Projection projection() const;
	Pipeline   pipeline()   const;
	View       view()       const;
	Frustum    frustum()    const;
	
	void setViewport(int x, int y, int w, int h );

	void computeClipMatrix();
	void computeClipMatrix( const Math::dmat4 &modelView, const Math::dmat4 &projectionMatrix );

	Math::dvec3 eyePosition() const;
	Math::dvec3 viewDirection() const;

	void enableFloatingOrigin( bool on = true );
	void setZoom( double zoomValue );
	void zoomInOut( double step );
	void fitView( const Math::box3 &box = Math::box3() );
	
	void setFont( const FontPtr &f );
	FontPtr font() const {return _font; }

	void drawText();
	void addText( const Math::dvec3 &worldPos, const std::string &text, const Color &color, double size = 12.0, TextRenderer::TextAlignment alignment = TextRenderer::AlignLeft );
	void addText( const Math::ivec2 &screenPos, const std::string &text, const Color &color, double size = 12.0, TextRenderer::TextAlignment alignment = TextRenderer::AlignLeft );
	
	bool       isVisible( const Math::dvec3 &p ) const;
	bool       isVisible( const Math::dvec3 &p0,const Math::dvec3 &p1  ) const;
	bool       isVisible( const Math::box3  &b ) const;
	
	Math::ivec4 viewport()			const   { return _viewport; }
	double      zoomFactor()	    const   { return _zoomFactor; }
*/
	/************************************************************************/
	/*  FLOATING ORIGIN                                                     */
	/************************************************************************/
	//void beginRendering();
	//void setVertex3DArray( const std::vector<Math::dvec3> &vertices, int stride = 0 );
	//void setVertex2DArray( const std::vector<Math::dvec3> &vertices, int stride = 0 );
	//void setVertex( const Math::ivec2 &v );
	//void setVertex( const Math::ivec3 &v );
	//void setVertex( const Math::vec2 &v );
	//void setVertex( const Math::vec3 &v );
	//void setVertex( const Math::dvec2 &v );
	//void setVertex( const Math::dvec3 &v );
	//void endRendering();
	/************************************************************************/
	/*  FLOATING ORIGIN                                                     */
	/************************************************************************/ 
};


}

#endif