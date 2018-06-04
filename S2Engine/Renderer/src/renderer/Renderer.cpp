// Renderer.cpp
//
#include "Renderer.h"

#include "OpenGL/OpenGL.h"
#include "OpenGL/OpenGLWrap.h"

#include "Math/Rectangle.h"

using namespace OpenGL;

#define SHADOWING 0
#define OPENGL_DEPRECATED 

static void enable( GLenum cap, bool enabled )
{
	if( enabled ) glEnable(cap);
	else          glDisable(cap);
}

// ------------------------------------------------------------------------------------------------
Renderer::Renderer()
{}

// ------------------------------------------------------------------------------------------------
Renderer::~Renderer()
{}

// ------------------------------------------------------------------------------------------------
void Renderer::applyRenderState( const RenderState &rs )
{
	applyPrimitiveRestart( rs.primitiveRestart );
	applyFaceCulling( rs.faceCulling );
	applyProgramPointSize( rs.programPointSize );
	applyRasterizationMode( rs.rasterizationMode );
	applyScissorTest( rs.scissorTest );
	applyStencilTest( rs.stencilTest );
	applyDepthTest( rs.depthTest );
	applyDepthRange( rs.depthRange );
	applyBlending( rs.blending );
	applyColorMask( rs.colorMask );
	applyDepthMask( rs.depthMask );
}

// ------------------------------------------------------------------------------------------------
void Renderer::applyPrimitiveRestart( const PrimitiveRestart &pr )
{
	//if( rs.primitiveRestart.Enabled != primitiveRestart.Enabled)
	//{
	//	Enable(EnableCap.PrimitiveRestart, primitiveRestart.Enabled);
	//	_renderState.PrimitiveRestart.Enabled = primitiveRestart.Enabled;
	//}

	//if (primitiveRestart.Enabled)
	//{
	//	if (_renderState.PrimitiveRestart.Index != primitiveRestart.Index)
	//	{
	//		GL.PrimitiveRestartIndex(primitiveRestart.Index);
	//		_renderState.PrimitiveRestart.Index = primitiveRestart.Index;
	//	}
	//}
}

// ------------------------------------------------------------------------------------------------
void Renderer::applyFaceCulling( const FaceCulling &fc )
{
#if SHADOWING
	if( _renderState.faceCulling.enabled != fc.enabled )
#endif
	{
		enable( GL_CULL_FACE, fc.enabled);
		_renderState.faceCulling.enabled =  fc.enabled;
	}

	if( fc.enabled )
	{
#if SHADOWING
		if( _renderState.faceCulling.cullFace != fc.cullFace )
#endif
		{
			glCullFace( glWrap(fc.cullFace) );
			_renderState.faceCulling.cullFace = fc.cullFace;
		}
#if SHADOWING
		if( _renderState.faceCulling.frontFaceWindingOrder != fc.frontFaceWindingOrder )
#endif
		{
			glFrontFace( glWrap(fc.frontFaceWindingOrder) );
			_renderState.faceCulling.frontFaceWindingOrder = fc.frontFaceWindingOrder;
		}
	}

}

// ------------------------------------------------------------------------------------------------
void Renderer::applyProgramPointSize( const ProgramPointSize &programPointSize )
{
#if SHADOWING
	if( _renderState.programPointSize.enabled != programPointSize.enabled )
#endif
	{
		enable( GL_PROGRAM_POINT_SIZE, programPointSize.enabled );
		_renderState.programPointSize = programPointSize;
	}
}

// ------------------------------------------------------------------------------------------------
void Renderer::applyRasterizationMode( const RenderState::RasterizationMode &rasterizationMode )
{
#if SHADOWING
	if( _renderState.rasterizationMode != rasterizationMode )
#endif
	{
		glPolygonMode( glWrap(FaceCulling::FrontAndBack), glWrap(rasterizationMode) );
		_renderState.rasterizationMode = rasterizationMode;
	}
}

// ------------------------------------------------------------------------------------------------
void Renderer::applyScissorTest( const ScissorTest &scissorTest )
{
	Math::Rectangle rectangle = scissorTest.rect;

	if( rectangle.width() < 0)
	{
#if 0
		throw new ArgumentOutOfRangeException(
			"renderState.ScissorTest.Rectangle.Width must be greater than or equal to zero.", 
			"renderState");
#endif
	}

	if( rectangle.height() < 0)
	{
#if 0
		throw new ArgumentOutOfRangeException(
			"renderState.ScissorTest.Rectangle.Height must be greater than or equal to zero.",
			"renderState");
#endif
	}
#if SHADOWING
	if( _renderState.scissorTest.enabled != scissorTest.enabled )
#endif
	{
		enable( GL_SCISSOR_TEST, scissorTest.enabled );
		_renderState.scissorTest.enabled = scissorTest.enabled;
	}

	if( scissorTest.enabled )
	{
#if SHADOWING
		if( !_renderState.scissorTest.rect.equals(scissorTest.rect) )
#endif
		{
			glScissor( rectangle.left(), rectangle.bottom(), rectangle.width(), rectangle.height() );
			_renderState.scissorTest.rect = scissorTest.rect;
		}
	}
}

// ------------------------------------------------------------------------------------------------
void Renderer::applyStencilTest( const StencilTest &stencilTest )
{
#if SHADOWING
	if( _renderState.stencilTest.enabled != stencilTest.enabled )
#endif
	{
		enable( GL_STENCIL_TEST, stencilTest.enabled );
		_renderState.stencilTest.enabled = stencilTest.enabled;
	}

	if( stencilTest.enabled )
	{
		applyStencil( FaceCulling::Front, _renderState.stencilTest.frontFace, stencilTest.frontFace);
		applyStencil( FaceCulling::Back,  _renderState.stencilTest.backFace,  stencilTest.backFace);
	}
}

// ------------------------------------------------------------------------------------------------
void Renderer::applyStencil( const FaceCulling::Face &face, StencilTestFace &currentTest, const StencilTestFace &test)
{
#if SHADOWING
	if ((currentTest.stencilFailOperation          != test.stencilFailOperation) ||
		(currentTest.depthFailStencilPassOperation != test.depthFailStencilPassOperation) ||
		(currentTest.depthPassStencilPassOperation != test.depthPassStencilPassOperation))
#endif
	{
		glStencilOpSeparate( glWrap(face),
			glWrap(test.stencilFailOperation),
			glWrap(test.depthFailStencilPassOperation),
			glWrap(test.depthPassStencilPassOperation));

		currentTest.stencilFailOperation          = test.stencilFailOperation;
		currentTest.depthFailStencilPassOperation = test.depthFailStencilPassOperation;
		currentTest.depthPassStencilPassOperation = test.depthPassStencilPassOperation;
	}
#if SHADOWING
	if ((currentTest.function       != test.function) ||
		(currentTest.referenceValue != test.referenceValue) ||
		(currentTest.mask           != test.mask))
#endif
	{
		glStencilFuncSeparate( glWrap(face),
			glWrap(test.function),
			test.referenceValue,
			test.mask);

		currentTest.function       = test.function;
		currentTest.referenceValue = test.referenceValue;
		currentTest.mask           = test.mask;
	}
}

// ------------------------------------------------------------------------------------------------
void Renderer::applyDepthTest( const DepthTest &depthTest)
{
#if SHADOWING
	if( _renderState.depthTest.enabled != depthTest.enabled )
#endif
	{
		enable( GL_DEPTH_TEST, depthTest.enabled );
		_renderState.depthTest.enabled = depthTest.enabled;
	}

	if( depthTest.enabled )
	{
#if SHADOWING
		if( _renderState.depthTest.function != depthTest.function )
#endif
		{
			glDepthFunc( glWrap(depthTest.function) );
			_renderState.depthTest.function = depthTest.function;
		}
	}
}

// ------------------------------------------------------------------------------------------------
void Renderer::applyDepthRange( const DepthRange &depthRange)
{
#if 0
	if( depthRange.near() < 0.0 || depthRange.near() > 1.0)
	{
		throw new ArgumentOutOfRangeException(
			"renderState.DepthRange.Near must be between zero and one.",
			"depthRange");
	}

	if (depthRange.far() < 0.0 || depthRange.far() > 1.0)
	{
		throw new ArgumentOutOfRangeException(
			"renderState.DepthRange.Far must be between zero and one.",
			"depthRange");
	}
#endif
#if SHADOWING
	if( (_renderState.depthRange.nearValue != depthRange.nearValue) ||
		(_renderState.depthRange.farValue  != depthRange.farValue) )
#endif
	{
		glDepthRange( depthRange.nearValue, depthRange.farValue );
		_renderState.depthRange.nearValue = depthRange.nearValue;
		_renderState.depthRange.farValue  = depthRange.farValue;
	}
}

// ------------------------------------------------------------------------------------------------
void Renderer::applyBlending( const Blending &blending )
{
#if SHADOWING
	if( _renderState.blending.enabled != blending.enabled )
#endif
	{
		enable( GL_BLEND, blending.enabled );
		_renderState.blending.enabled = blending.enabled;
	}

	if( blending.enabled )
	{
#if SHADOWING
		if( (_renderState.blending.sourceRGBFactor        != blending.sourceRGBFactor)      ||
			(_renderState.blending.destinationRGBFactor   != blending.destinationRGBFactor) ||
			(_renderState.blending.sourceAlphaFactor      != blending.sourceAlphaFactor)    ||
			(_renderState.blending.destinationAlphaFactor != blending.destinationAlphaFactor))
#endif
		{
			glBlendFuncSeparate(
				glWrap(blending.sourceRGBFactor),
				glWrap(blending.destinationRGBFactor),
				glWrap(blending.sourceAlphaFactor),
				glWrap(blending.destinationAlphaFactor));

			_renderState.blending.sourceRGBFactor        = blending.sourceRGBFactor;
			_renderState.blending.destinationRGBFactor   = blending.destinationRGBFactor;
			_renderState.blending.sourceAlphaFactor      = blending.sourceAlphaFactor;
			_renderState.blending.destinationAlphaFactor = blending.destinationAlphaFactor;
		}

#if SHADOWING
		if( (_renderState.blending.rgbEquation   != blending.rgbEquation)  ||
			(_renderState.blending.alphaEquation != blending.alphaEquation) )
#endif
		{
			glBlendEquationSeparate(
				glWrap(blending.rgbEquation),
				glWrap(blending.alphaEquation));

			_renderState.blending.rgbEquation   = blending.rgbEquation;
			_renderState.blending.alphaEquation = blending.alphaEquation;
		}

#if SHADOWING
		if( !_renderState.blending.color.equals( blending.color ) )
#endif
		{
			glBlendColor( blending.color.r(),blending.color.g(),blending.color.b(),blending.color.a() );
			_renderState.blending.color = blending.color;
		}
	}
}

// ------------------------------------------------------------------------------------------------
void Renderer::applyColorMask( const ColorMask &colorMask )
{
#if SHADOWING
	if( !_renderState.colorMask.equals(colorMask) )
#endif
	{
		glColorMask( colorMask.r, colorMask.g, colorMask.b, colorMask.a );
		_renderState.colorMask = colorMask;
	}
}

// ------------------------------------------------------------------------------------------------
void Renderer::applyDepthMask(bool depthMask)
{
#if SHADOWING
	if( _renderState.depthMask != depthMask )
#endif
	{
		glDepthMask(depthMask);
		_renderState.depthMask = depthMask;
	}
}

// ------------------------------------------------------------------------------------------------
void Renderer::applyShaderProgram( const OpenGL::ProgramPtr &program )
{
	if( !program->isValid() )
		return;

#if SHADOWING
	if( _currentShaderProgram != program )
#endif
	{
		program->bind();
		_currentShaderProgram = program;
	}
	
	//_currentShaderProgram.Clean(this, drawState, sceneState);
}

// ------------------------------------------------------------------------------------------------
void Renderer::applyViewState( const ViewState &vs )
{
	//setViewport( vs.viewport );
#if SHADOWING
	if( !_viewState.viewport.equals(vs.viewport) )
#endif
	glViewport( vs.viewport.left(), vs.viewport.bottom(), vs.viewport.width(), vs.viewport.height() );

#ifdef OPENGL_DEPRECATED
	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixd( Math::value_ptr( vs.modelViewMatrix() )  );
	glMatrixMode( GL_PROJECTION );
	glLoadMatrixd( Math::value_ptr( vs.projectionMatrix() ) );
	glMatrixMode( GL_MODELVIEW );

	_viewState = vs;
#endif
}

// ------------------------------------------------------------------------------------------------
//void Renderer::applyFramebuffer()
//{
//	if (_boundFramebuffer != _setFramebuffer)
//	{
//		if (_setFramebuffer != null)
//		{
//			_setFramebuffer.Bind();
//		}
//		else
//		{
//			FramebufferGL3x.UnBind();
//		}
//
//		_boundFramebuffer = _setFramebuffer;
//	}
//
//	if (_setFramebuffer != null)
//		_setFramebuffer.Clean();
//}

// ------------------------------------------------------------------------------------------------
void Renderer::setViewport( const Math::Rectangle &r )
{
	if( r.width() < 0 || r.height() < 0 )
	{
#if 0
		throw new ArgumentOutOfRangeException("Viewport", "The viewport width and height must be greater than or equal to zero.");
#endif
	}
#if SHADOWING
	if( !_viewState.viewport.equals(r) )
#endif
	{
		_viewState.viewport = r;
		glViewport( r.left(), r.bottom(), r.width(), r.height() );
	}
}

// ------------------------------------------------------------------------------------------------
//Math::Rectangle Renderer::viewport() const { return _viewport; }

// ------------------------------------------------------------------------------------------------
void Renderer::clear( const ClearState &cs )
{
	//applyFramebuffer();

	applyScissorTest( cs.scissorTest );
	applyColorMask(   cs.colorMask    );
	applyDepthMask(   cs.depthMask    );
	// TODO: StencilMaskSeparate

#if SHADOWING
	if( !_clearColor.equals( cs.color ) )
#endif
	{
		glClearColor( cs.color.r(), cs.color.g(), cs.color.b(), cs.color.a() );
		_clearColor = cs.color ;
	}

#if SHADOWING
	if( _clearDepth != cs.depth )
#endif
	{
		glClearDepth((double)cs.depth );
		_clearDepth = cs.depth;
	}

#if SHADOWING
	if( _clearStencil != cs.stencil )
#endif
	{
		glClearStencil( cs.stencil );
		_clearStencil = cs.stencil;
	}

	glClear( glWrap(cs.buffers) );
}

// ------------------------------------------------------------------------------------------------
void Renderer::draw( PrimitiveType primitive, const VertexArray &va, const ViewState &vs, const DrawState &ds )
{
	//VerifyDraw(drawState, sceneState);
	//ApplyBeforeDraw(drawState, sceneState);
	applyRenderState( ds.renderState );
	applyShaderProgram( ds.program );
	applyViewState( vs );

	va.bind();
	
	if( va.isIndexed() )
	{
		glDrawElements( glWrap(primitive), 
			            0, 
						glWrap(va.indexBuffer()->dataType()), 
						BUFFER_OFFSET(0) );
		
		//glDrawRangeElements( glWrap(primitive),
		//	0, va.MaximumArrayIndex(), va.indices()->count(),
		//	TypeConverterGL3x.To(indexBuffer.Datatype), new IntPtr());
	}
	else
	{
		glDrawArrays( glWrap(primitive), 0, va.maxArrayIndex() + 1);
	}

	glCheck;
}

// ------------------------------------------------------------------------------------------------
void Renderer::draw( PrimitiveType primitive, const Mesh &m, const ViewState &vs, const DrawState &ds )
{
	draw( primitive, m._va, vs, ds );
}


// ------------------------------------------------------------------------------------------------
//void Renderer::draw( PrimitiveType primitive, const VertexBuffer &vb, const ViewState &vs, const DrawState &ds )
//{
//	//VerifyDraw(drawState, sceneState);
//	//ApplyBeforeDraw(drawState, sceneState);
//	applyRenderState( ds.renderState );
//	applyShaderProgram( ds.program );
//	applyViewState( vs );
//		
//	glEnableClientState( GL_VERTEX_ARRAY );
//	glEnableClientState( GL_NORMAL_ARRAY );
//	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
//	glEnableClientState( GL_COLOR_ARRAY );
//
//	glVertexPointer  ( 3, GL_DOUBLE, sizeof(Vertex), &vb.vertices()[0].position );
//	glColorPointer   ( 4, GL_FLOAT,  sizeof(Vertex), &vb.vertices()[0].color    );
//	glNormalPointer  (    GL_FLOAT,  sizeof(Vertex), &vb.vertices()[0].normal   );
//	glTexCoordPointer( 2, GL_FLOAT,  sizeof(Vertex), &vb.vertices()[0].texCoord );
//
//	//if( vb.hasIndexBuffer() )
//		//glDrawRangeElements( glWrap(primitive), 0
//	//else
//		glDrawArrays( glWrap(primitive), 0, vb.size() );
//	
//	glDisableClientState( GL_VERTEX_ARRAY );
//	glDisableClientState( GL_NORMAL_ARRAY );
//	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
//	glDisableClientState( GL_COLOR_ARRAY );
//
//	//VertexArrayGL3x vertexArray = (VertexArrayGL3x)drawState.VertexArray;
//	//IndexBufferGL3x indexBuffer = vertexArray.IndexBuffer as IndexBufferGL3x;
//
//	//if( indexBuffer != null )
//	//{
//	//	GL.DrawRangeElements(TypeConverterGL3x.To(primitiveType),
//	//		0, vertexArray.MaximumArrayIndex(), indexBuffer.Count,
//	//		TypeConverterGL3x.To(indexBuffer.Datatype), new IntPtr());
//	//}
//	//else
//	//{
//	//	GL.DrawArrays(TypeConverterGL3x.To(primitiveType), 0,
//	//		vertexArray.MaximumArrayIndex() + 1);
//	//}
//}

// ------------------------------------------------------------------------------------------------
//void Renderer::drawText()
//{
//	_textRenderer.draw(*this);
//}
//
//// ------------------------------------------------------------------------------------------------
//void Renderer::addText( const Math::dvec3 &worldPos, const std::wstring &text, const Color &color,  const FontPtr &font, TextRenderer::TextAlignment alignment )
//{
//	if( text.empty() )
//		return;
//	
//	const Math::ivec2 screenPos = convertWorldToWindow( worldPos , false );
//	
//	_textRenderer.addTag( TextRenderer::TextTag(text,screenPos,color,font,0.0, alignment) );
//}
//
//// ------------------------------------------------------------------------------------------------
//void Renderer::addText( const Math::ivec2 &screenPos, const std::wstring &text, const Color &color, const FontPtr &font, TextRenderer::TextAlignment alignment )
//{
//	if( text.empty() )
//		return;
//
//	_textRenderer.addTag( TextRenderer::TextTag(text,screenPos,color,font,0.0, alignment) );
//}