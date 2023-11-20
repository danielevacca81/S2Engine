// StateManager.cpp
//
#include "StateManager.h"

#include "OpenGL.h"
#include "OpenGLWrap.h"
#include "OpenGLCheck.h"
#include "Device.h"


#include "Math/Rectangle.h"

using namespace RenderSystem;

//#define OPENGL_DEPRECATED
#define CHECK_SHADOWING

static bool gShadowingAlwaysDisabled = false;

// ------------------------------------------------------------------------------------------------
static inline void enable( GLenum cap, bool enabled )
{
	if( enabled ) glEnable(cap); 
	else          glDisable(cap);
}

// ------------------------------------------------------------------------------------------------
static inline void enable( GLenum cap, bool enabled, int index )
{
	if( enabled ) glEnablei(cap, index); 
	else          glDisablei(cap, index);
}

// ------------------------------------------------------------------------------------------------
StateManager::StateManager()
: _disableDrawStateShadowingOneShot{ true }
, _disableClearStateShadowingOneShot { true }
, _shadowingCurrentlyEnabled { true }
, _clearColor( 0.f, 0.f, 0.f, 0.f )
, _clearDepth( 1.0f )
, _clearStencil( 0 )
{
	_renderState.depthTest.enabled   = false;
	_renderState.faceCulling.enabled = false;
}

// ------------------------------------------------------------------------------------------------
void StateManager::setClearState( const ClearState &cs )
{
	_shadowingCurrentlyEnabled = !( gShadowingAlwaysDisabled || _disableClearStateShadowingOneShot || !cs.shadowingEnabled );

	applyScissorTest( cs.scissorTest );
	applyColorMask(   cs.colorMask    );
	applyDepthMask(   cs.depthMask    );
	applyStencilMask( cs.stencilMask );

	int buffers = int(cs.buffers );

	if( buffers & int(ClearBuffers::ColorBuffer) && cs.colorSeparate.enabled )
	{
		buffers = buffers & ~int(ClearBuffers::ColorBuffer);
		applyClearColorSeparate( cs.colorSeparate );
	}

	if( (buffers & int(ClearBuffers::ColorBuffer)) && _clearColor != cs.color ||
		!_shadowingCurrentlyEnabled )
	{
		glClearColor( cs.color.r(), cs.color.g(), cs.color.b(), cs.color.a() );
		glCheck;
		_clearColor = cs.color ;
	}

	if( (buffers & int(ClearBuffers::DepthBuffer)) && _clearDepth != cs.depth || 
		!_shadowingCurrentlyEnabled )
	{
		glClearDepth((double)cs.depth );
		glCheck;
		_clearDepth = cs.depth;
	}

	if( (buffers & int(ClearBuffers::StencilBuffer)) && _clearStencil != cs.stencil || 
		!_shadowingCurrentlyEnabled )
	{
		glClearStencil( cs.stencil );
		glCheck;
		_clearStencil = cs.stencil;
	}

	if( buffers )
	{
		glClear( glWrap( ClearBuffers(buffers) ) );
		glCheck;
	}

#if defined( CHECK_SHADOWING )
	debugState( false, true );
#endif

	_disableClearStateShadowingOneShot = false;
}

// ------------------------------------------------------------------------------------------------
void StateManager::setDrawState( const DrawState &ds )
{
	_shadowingCurrentlyEnabled = !( gShadowingAlwaysDisabled || _disableDrawStateShadowingOneShot || !ds.shadowingEnabled );

	// apply before draw:
	applyViewState( ds.viewState );
	applyRenderState( ds.renderState );
	applyShaderProgram( ds.shaderProgram );
	ds.textureUnits.set();

	// add: apply texture units

#if defined( CHECK_SHADOWING ) 
	debugState( true, false );
#endif

	_disableDrawStateShadowingOneShot = false;
}

// ------------------------------------------------------------------------------------------------
inline void StateManager::applyRenderState( const RenderState &rs )
{
	applyPrimitiveRestart ( rs.primitiveRestart );
	applyFaceCulling      ( rs.faceCulling );
	applyProgramPointSize ( rs.programPointSize );
	applyRasterizationMode( rs.rasterizationMode );
	applyLineWidth        ( rs.lineWidth );
	applyScissorTest      ( rs.scissorTest );
	applyStencilTest      ( rs.stencilTest );
	applyDepthTest        ( rs.depthTest );
	applyDepthRange       ( rs.depthRange );
	applyBlending         ( rs.blending );
	applyColorMask        ( rs.colorMask );
	applyDepthMask        ( rs.depthMask );
	applyStencilMask      ( rs.stencilMask );	
}

// ------------------------------------------------------------------------------------------------
inline void StateManager::applyPrimitiveRestart( const PrimitiveRestart &pr )
{
	if( _renderState.primitiveRestart.enabled != pr.enabled ||
		!_shadowingCurrentlyEnabled )
	{
		enable( GL_PRIMITIVE_RESTART, pr.enabled );
		glCheck;
		_renderState.primitiveRestart.enabled = pr.enabled;
	}

	if( pr.enabled && _renderState.primitiveRestart.index != pr.index ||
		!_shadowingCurrentlyEnabled )
	{
		glPrimitiveRestartIndex( pr.index ); 
		glCheck;
		_renderState.primitiveRestart.index = pr.index;
	}
}

// ------------------------------------------------------------------------------------------------
inline void StateManager::applyFaceCulling( const FaceCulling &fc )
{
	if( _renderState.faceCulling.enabled != fc.enabled ||
		!_shadowingCurrentlyEnabled )
	{
		enable( GL_CULL_FACE, fc.enabled );
		glCheck;
		_renderState.faceCulling.enabled =  fc.enabled;
	}

	if( fc.enabled && _renderState.faceCulling.cullFace != fc.cullFace || 
		!_shadowingCurrentlyEnabled )
	{
		glCullFace( glWrap(fc.cullFace) );
		glCheck;
		_renderState.faceCulling.cullFace = fc.cullFace;
	}

	if( _renderState.faceCulling.frontFaceWindingOrder != fc.frontFaceWindingOrder ||
		!_shadowingCurrentlyEnabled )
	{
		glFrontFace( glWrap(fc.frontFaceWindingOrder) );
		glCheck;
		_renderState.faceCulling.frontFaceWindingOrder = fc.frontFaceWindingOrder;
	}
}

// ------------------------------------------------------------------------------------------------
inline void StateManager::applyProgramPointSize( const ProgramPointSize &programPointSize )
{
	if( _renderState.programPointSize.enabled != programPointSize.enabled ||
		!_shadowingCurrentlyEnabled )
	{
#if 0		// GL_POINT_SPRITE requires OpenGL Compatibility context Profile
		enable( GL_POINT_SPRITE, programPointSize.enabled ); // available only in OpenGL < 3.2 
		glCheck;
#endif
		
		enable( GL_PROGRAM_POINT_SIZE, programPointSize.enabled );
		glCheck;
		_renderState.programPointSize = programPointSize;
	}
}

// ------------------------------------------------------------------------------------------------
inline void StateManager::applyRasterizationMode( const RenderState::RasterizationMode &rasterizationMode )
{
	if( _renderState.rasterizationMode != rasterizationMode ||
		!_shadowingCurrentlyEnabled )
	{
		glPolygonMode( glWrap(FaceCulling::Face::FrontAndBack), glWrap(rasterizationMode) );
		glCheck;
		_renderState.rasterizationMode = rasterizationMode;
	}
}

// ------------------------------------------------------------------------------------------------
inline void StateManager::applyLineWidth( const float lineWidth )
{
	if( _renderState.lineWidth != lineWidth || 
		!_shadowingCurrentlyEnabled )
	{
		const float w = Math::clamp( lineWidth, Device::minLinesWidth(), Device::maxLinesWidth() );

		glLineWidth(w);
		glCheck;
		_renderState.lineWidth = w;
	}
}

// ------------------------------------------------------------------------------------------------
inline void StateManager::applyScissorTest( const ScissorTest &scissorTest )
{
	Math::irect rectangle = scissorTest.rect;

	if( rectangle.width() < 0 )
	{
#if 0
		throw new ArgumentOutOfRangeException(
			"renderState.ScissorTest.Rectangle.Width must be greater than or equal to zero.", 
			"renderState");
#endif
	}

	if( rectangle.height() < 0 )
	{
#if 0
		throw new ArgumentOutOfRangeException(
			"renderState.ScissorTest.Rectangle.Height must be greater than or equal to zero.",
			"renderState");
#endif
	}

	bool enabled =	scissorTest.enabled && 
					!rectangle.isEmpty() && rectangle.width()>0 && rectangle.height()>0;

	if(  _renderState.scissorTest.enabled != enabled ||
		!_shadowingCurrentlyEnabled )
	{
		enable( GL_SCISSOR_TEST, enabled );
		glCheck;
		_renderState.scissorTest.enabled = enabled;
	}

	if( enabled && !_renderState.scissorTest.rect.equals(scissorTest.rect) ||
		!_shadowingCurrentlyEnabled )
	{
		glScissor( rectangle.left(), rectangle.bottom(), rectangle.width(), rectangle.height() );
		glCheck;
		_renderState.scissorTest.rect = scissorTest.rect;
	}
}

// ------------------------------------------------------------------------------------------------
inline void StateManager::applyStencilTest( const StencilTest &stencilTest )
{
	if( _renderState.stencilTest.enabled != stencilTest.enabled ||
		!_shadowingCurrentlyEnabled )
	{
		enable( GL_STENCIL_TEST, stencilTest.enabled );
		glCheck;
		_renderState.stencilTest.enabled = stencilTest.enabled;
	}

	if( stencilTest.enabled ||
		!_shadowingCurrentlyEnabled  )
	{
		applyStencil( FaceCulling::Face::Front, _renderState.stencilTest.frontFace, stencilTest.frontFace );
		applyStencil( FaceCulling::Face::Back,  _renderState.stencilTest.backFace,  stencilTest.backFace  );
	}
}

// ------------------------------------------------------------------------------------------------
inline void StateManager::applyStencil( const FaceCulling::Face &face, StencilTestFace &currentTest, const StencilTestFace &test)
{
	if ((currentTest.stencilFailOperation          != test.stencilFailOperation) ||
		(currentTest.depthFailStencilPassOperation != test.depthFailStencilPassOperation) ||
		(currentTest.depthPassStencilPassOperation != test.depthPassStencilPassOperation) ||
		 !_shadowingCurrentlyEnabled )
	{
		glStencilOpSeparate( glWrap( face ),
							 glWrap( test.stencilFailOperation ),
							 glWrap( test.depthFailStencilPassOperation ),
							 glWrap( test.depthPassStencilPassOperation ) );
		glCheck;

		currentTest.stencilFailOperation          = test.stencilFailOperation;
		currentTest.depthFailStencilPassOperation = test.depthFailStencilPassOperation;
		currentTest.depthPassStencilPassOperation = test.depthPassStencilPassOperation;
	}
	if ((currentTest.function       != test.function) ||
		(currentTest.referenceValue != test.referenceValue) ||
		(currentTest.mask           != test.mask) ||
		 !_shadowingCurrentlyEnabled)
	{
		glStencilFuncSeparate( glWrap( face ),
							   glWrap( test.function ),
							   test.referenceValue,
							   test.mask );
		glCheck;

		currentTest.function       = test.function;
		currentTest.referenceValue = test.referenceValue;
		currentTest.mask           = test.mask;
	}
}

// ------------------------------------------------------------------------------------------------
inline void StateManager::applyDepthTest( const DepthTest &depthTest)
{
	if( _renderState.depthTest.enabled != depthTest.enabled ||
		!_shadowingCurrentlyEnabled )
	{
		enable( GL_DEPTH_TEST, depthTest.enabled );
		glCheck;
		_renderState.depthTest.enabled = depthTest.enabled;
	}

	if( depthTest.enabled && _renderState.depthTest.function != depthTest.function ||
		!_shadowingCurrentlyEnabled )
	{
		glDepthFunc( glWrap( depthTest.function ) );
		glCheck;
		_renderState.depthTest.function = depthTest.function;
	}
}

// ------------------------------------------------------------------------------------------------
inline void StateManager::applyDepthRange( const DepthRange &depthRange )
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

	if( (_renderState.depthRange.nearValue != depthRange.nearValue) ||
		(_renderState.depthRange.farValue  != depthRange.farValue) ||
		!_shadowingCurrentlyEnabled )
	{
		glDepthRange( depthRange.nearValue, depthRange.farValue );
		glCheck;
		_renderState.depthRange.nearValue = depthRange.nearValue;
		_renderState.depthRange.farValue  = depthRange.farValue;
	}
}

// ------------------------------------------------------------------------------------------------
inline void StateManager::applyBlending( const Blending &blending )
{
	bool blendingEnabled = false;
	for( size_t i= 0; i< blending.enabled.size(); ++i ) 
	{
		if( _renderState.blending.enabled[i] != blending.enabled[i] ||
			!_shadowingCurrentlyEnabled )
		{
			enable( GL_BLEND, blending.enabled[i], i );
			glCheck;
			_renderState.blending.enabled[i] = blending.enabled[i];
		}

		blendingEnabled = blendingEnabled || blending.enabled[i]; 
	}

	if( blendingEnabled || 
		!_shadowingCurrentlyEnabled )
	{
		if( (_renderState.blending.sourceRGBFactor        != blending.sourceRGBFactor)        ||
			(_renderState.blending.destinationRGBFactor   != blending.destinationRGBFactor)   ||
			(_renderState.blending.destinationRGBFactor   != blending.destinationRGBFactor)   ||
			(_renderState.blending.sourceAlphaFactor      != blending.sourceAlphaFactor)      ||
			(_renderState.blending.destinationAlphaFactor != blending.destinationAlphaFactor) ||
			!_shadowingCurrentlyEnabled )
		{
			glBlendFuncSeparate(
				glWrap(blending.sourceRGBFactor),
				glWrap(blending.destinationRGBFactor),
				glWrap(blending.sourceAlphaFactor),
				glWrap(blending.destinationAlphaFactor));
			glCheck;

			_renderState.blending.sourceRGBFactor        = blending.sourceRGBFactor;
			_renderState.blending.destinationRGBFactor   = blending.destinationRGBFactor;
			_renderState.blending.sourceAlphaFactor      = blending.sourceAlphaFactor;
			_renderState.blending.destinationAlphaFactor = blending.destinationAlphaFactor;
		}

		if( (_renderState.blending.rgbEquation   != blending.rgbEquation)   ||
			(_renderState.blending.alphaEquation != blending.alphaEquation) ||
			!_shadowingCurrentlyEnabled )
		{
			glBlendEquationSeparate(
				glWrap(blending.rgbEquation),
				glWrap(blending.alphaEquation));
			glCheck;

			_renderState.blending.rgbEquation   = blending.rgbEquation;
			_renderState.blending.alphaEquation = blending.alphaEquation;
		}

		if( _renderState.blending.color != blending.color ||
			!_shadowingCurrentlyEnabled )
		{
			glBlendColor( blending.color.r(),blending.color.g(),blending.color.b(),blending.color.a() );
			glCheck;
			_renderState.blending.color = blending.color;
		}
	}
}

// ------------------------------------------------------------------------------------------------
inline void StateManager::applyColorMask( const ColorMask &colorMask )
{
	if( !_renderState.colorMask.equals(colorMask) ||
		!_shadowingCurrentlyEnabled )
	{
		glColorMask( colorMask.r, colorMask.g, colorMask.b, colorMask.a );
		glCheck;
		_renderState.colorMask = colorMask;
	}
}

// ------------------------------------------------------------------------------------------------
inline void StateManager::applyDepthMask( const DepthMask &depthMask )
{
	if( _renderState.depthMask.enabled != depthMask.enabled || 
		!_shadowingCurrentlyEnabled )
	{
		glDepthMask( depthMask.enabled );
		glCheck;
		_renderState.depthMask = depthMask;
	}
}

// ------------------------------------------------------------------------------------------------
inline void StateManager::applyStencilMask( const StencilMask &stencilMask )
{
	if( _renderState.stencilMask.front != stencilMask.front || 
		!_shadowingCurrentlyEnabled )
	{
		glStencilMaskSeparate( GL_FRONT, stencilMask.front );
		glCheck;
		_renderState.stencilMask.front = stencilMask.front;
	}

	if( _renderState.stencilMask.back != stencilMask.back ||
		!_shadowingCurrentlyEnabled )
	{
		glStencilMaskSeparate( GL_BACK, stencilMask.back );
		glCheck;
		_renderState.stencilMask.back = stencilMask.back;
	}
}

// ------------------------------------------------------------------------------------------------
inline void StateManager::applyShaderProgram( const ProgramPtr &program )
{
	auto doApplyProgram = []( const ProgramPtr & p ){
		if( p )
			p->bind();
		else
		{
			glUseProgram( 0 );
			glCheck;
		}
	};

	ProgramPtr newProgram = (!program || program->isCreated()) ? program : _currentShaderProgram;
	assert( newProgram == program );

	if( newProgram != _currentShaderProgram ||
		!_shadowingCurrentlyEnabled )
	{
		doApplyProgram( newProgram );
		_currentShaderProgram = newProgram;
	}

	if( _currentShaderProgram )
		_currentShaderProgram->applyUniforms();
}

// ------------------------------------------------------------------------------------------------
inline void StateManager::applyClearColorSeparate( const ClearColorSeparate& clearColorSeparate )
{
	if( !clearColorSeparate.enabled )
		return;

	for( size_t i= 0; i< clearColorSeparate.color.size(); ++i )
	{
		const auto &c= clearColorSeparate.color[i];
		if( std::holds_alternative<Color>( c ) )
		{
			glClearBufferfv( GL_COLOR, i, std::get<Color>( c ).rgba() );
			glCheck;
		}
		else if( std::holds_alternative<Math::ivec4>( c ) )
		{
			const auto &color = std::get<Math::ivec4>( c );
			glClearBufferiv( GL_COLOR, i, &( color.r ) );
			glCheck;
		}
	}
}

// ------------------------------------------------------------------------------------------------
inline void StateManager::applyViewState( const ViewState &vs )
{
	if( !vs.viewport.equals( _viewState.viewport ) ||
		!_shadowingCurrentlyEnabled )
	{
		glViewport( vs.viewport.left(), vs.viewport.bottom(), vs.viewport.width(), vs.viewport.height() );
		glCheck;
		_viewState.viewport = vs.viewport;
	}
}

// ------------------------------------------------------------------------------------------------
void StateManager::debugState( const bool drawStateCheck , const bool clearStateCheck ) const
{
	GLint   val[4];
	GLfloat valf[4];

	if( drawStateCheck )
	{
		assert( bool(glIsEnabled( GL_PRIMITIVE_RESTART )  ) == _renderState.primitiveRestart.enabled );
		glGetIntegerv( GL_PRIMITIVE_RESTART_INDEX, val );
		assert( val[0] == _renderState.primitiveRestart.index );
	

		assert( bool(glIsEnabled( GL_CULL_FACE )          ) == _renderState.faceCulling.enabled );
		glGetIntegerv( GL_CULL_FACE_MODE, val );
		assert( val[0] == glWrap(_renderState.faceCulling.cullFace) );
		glGetIntegerv( GL_FRONT_FACE, val );
		assert( val[0] == glWrap(_renderState.faceCulling.frontFaceWindingOrder) );


		assert( bool(glIsEnabled( GL_POINT_SPRITE ) )       == _renderState.programPointSize.enabled );
		assert( bool(glIsEnabled( GL_PROGRAM_POINT_SIZE ) ) == _renderState.programPointSize.enabled );


		glGetIntegerv( GL_POLYGON_MODE, val );
		assert( val[0] == glWrap(_renderState.rasterizationMode) );
		assert( val[1] == glWrap(_renderState.rasterizationMode) );


		glGetIntegerv( GL_LINE_WIDTH, val );
		assert( val[0] == _renderState.lineWidth );
	
		assert( bool(glIsEnabled( GL_STENCIL_TEST )       ) == _renderState.stencilTest.enabled );

		glGetIntegerv( GL_STENCIL_BACK_FAIL, val );
		assert( val[0] == glWrap( _renderState.stencilTest.backFace.stencilFailOperation ) );
		glGetIntegerv( GL_STENCIL_BACK_PASS_DEPTH_FAIL, val );
		assert( val[0] == glWrap( _renderState.stencilTest.backFace.depthFailStencilPassOperation ) );
		glGetIntegerv( GL_STENCIL_BACK_PASS_DEPTH_PASS, val );
		assert( val[0] == glWrap( _renderState.stencilTest.backFace.depthPassStencilPassOperation ) );
		glGetIntegerv( GL_STENCIL_BACK_FUNC, val );
		assert( val[0] == glWrap( _renderState.stencilTest.backFace.function ) );
		glGetIntegerv( GL_STENCIL_BACK_REF, val );
		assert( val[0] == _renderState.stencilTest.backFace.referenceValue );
		glGetIntegerv( GL_STENCIL_BACK_VALUE_MASK, val );
		assert( val[0] == _renderState.stencilTest.backFace.mask );

		glGetIntegerv( GL_STENCIL_FAIL, val );
		assert( val[0] == glWrap( _renderState.stencilTest.frontFace.stencilFailOperation ) );
		glGetIntegerv( GL_STENCIL_PASS_DEPTH_FAIL, val );
		assert( val[0] == glWrap( _renderState.stencilTest.frontFace.depthFailStencilPassOperation ) );
		glGetIntegerv( GL_STENCIL_PASS_DEPTH_PASS, val );
		assert( val[0] == glWrap( _renderState.stencilTest.frontFace.depthPassStencilPassOperation ) );
		glGetIntegerv( GL_STENCIL_FUNC, val );
		assert( val[0] == glWrap( _renderState.stencilTest.frontFace.function ) );
		glGetIntegerv( GL_STENCIL_REF, val );
		assert( val[0] == _renderState.stencilTest.frontFace.referenceValue );
		glGetIntegerv( GL_STENCIL_VALUE_MASK, val );
		assert( val[0] == _renderState.stencilTest.frontFace.mask );


		assert( bool(glIsEnabled( GL_DEPTH_TEST )         ) == _renderState.depthTest.enabled );
		glGetIntegerv( GL_DEPTH_FUNC, val );
		assert( val[0] == glWrap( _renderState.depthTest.function ) );


		glGetFloatv( GL_DEPTH_RANGE, valf );
		assert( valf[0] == _renderState.depthRange.nearValue );
		assert( valf[1] == _renderState.depthRange.farValue );


		for( int i= 0; i< _renderState.blending.enabled.size(); ++i ) 
			assert( bool(glIsEnabledi( GL_BLEND, i )      ) == _renderState.blending.enabled[i] );

		glGetIntegerv( GL_BLEND_SRC_RGB, val );
		assert( val[0] == glWrap( _renderState.blending.sourceRGBFactor ) );
		glGetIntegerv( GL_BLEND_DST_RGB, val );
		assert( val[0] == glWrap( _renderState.blending.destinationRGBFactor ) );
		glGetIntegerv( GL_BLEND_SRC_ALPHA, val );
		assert( val[0] == glWrap( _renderState.blending.sourceAlphaFactor ) );
		glGetIntegerv( GL_BLEND_DST_ALPHA, val );
		assert( val[0] == glWrap( _renderState.blending.destinationAlphaFactor ) );

		glGetIntegerv( GL_BLEND_EQUATION_RGB, val );
		assert( val[0] == glWrap( _renderState.blending.rgbEquation ) );
		glGetIntegerv( GL_BLEND_EQUATION_ALPHA, val );
		assert( val[0] == glWrap( _renderState.blending.alphaEquation ) );


		glGetIntegerv( GL_CURRENT_PROGRAM, val );
		if( _currentShaderProgram )
			assert( val[0] == _currentShaderProgram->id() );
		else
			assert( val[0] == 0 );


		glGetIntegerv( GL_VIEWPORT, val );
		assert( val[0] == _viewState.viewport.left()   );
		assert( val[1] == _viewState.viewport.bottom() );
		assert( val[2] == _viewState.viewport.width()  );
		assert( val[3] == _viewState.viewport.height() );
	}
	if( drawStateCheck || clearStateCheck )
	{
		assert( bool(glIsEnabled( GL_SCISSOR_TEST )       ) == _renderState.scissorTest.enabled );
		glGetFloatv( GL_SCISSOR_BOX, valf );
		assert( valf[0] == _renderState.scissorTest.rect.left()   );
		assert( valf[1] == _renderState.scissorTest.rect.bottom() );
		assert( valf[2] == _renderState.scissorTest.rect.width()  );
		assert( valf[3] == _renderState.scissorTest.rect.height() );


		glGetFloatv( GL_BLEND_COLOR, valf );
		for( int i= 0; i< 4; ++i )
			assert( valf[i] == _renderState.blending.color.rgba()[i] );


		glGetIntegerv( GL_DEPTH_WRITEMASK, val );
		assert( bool(val[0]) == _renderState.depthMask.enabled );


		glGetIntegerv( GL_STENCIL_BACK_WRITEMASK, val );
		assert( val[0] == _renderState.stencilMask.back );
		glGetIntegerv( GL_STENCIL_WRITEMASK, val );
		assert( val[0] == _renderState.stencilMask.front );
	}
	if( clearStateCheck )
	{
		glGetFloatv( GL_COLOR_CLEAR_VALUE, valf );
		for( int i= 0; i< 4; ++i )
			assert( valf[i] == _clearColor.rgba()[i] );


		glGetFloatv( GL_DEPTH_CLEAR_VALUE, valf );
		assert( valf[0] == _clearDepth );


		glGetIntegerv( GL_STENCIL_CLEAR_VALUE, val );
		assert( val[0] == _clearStencil );
	}
}