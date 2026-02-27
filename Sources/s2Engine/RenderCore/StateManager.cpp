// StateManager.cpp
//
#include "StateManager.h"

#include "OpenGL.h"
#include "OpenGLWrap.h"
#include "OpenGLCheck.h"
#include "Device.h"
#include "Shader.h"

#include "Math/Rectangle.h"

#include <cassert>

using namespace s2::RenderCore;

static const bool _shadowingCurrentlyEnabled = false;

// ------------------------------------------------------------------------------------------------
static inline void enable( GLenum cap, bool enabled )
{
    if( enabled ) glEnable( cap ); 
    else          glDisable( cap );
}

// ------------------------------------------------------------------------------------------------
static inline void enable( GLenum cap, bool enabled, int index )
{
    if( enabled ) glEnablei( cap, index ); 
    else          glDisablei( cap, index );
}

// ------------------------------------------------------------------------------------------------
StateManager::StateManager()
    //: _disableDrawStateShadowingOneShot{ true }
    //, _disableClearStateShadowingOneShot{ true }
    //, _shadowingCurrentlyEnabled{ true }
    : _clearColor( 0.f, 0.f, 0.f, 0.f )
    , _clearDepth( 1.0f )
    , _clearStencil( 0 )
{
    _renderState.depthTest.enabled = false;
    _renderState.faceCulling.enabled = false;
}

// ------------------------------------------------------------------------------------------------
void StateManager::setClearState( const ClearState& cs )
{
    //_shadowingCurrentlyEnabled = !( _disableClearStateShadowingOneShot || !cs.shadowingEnabled );

    applyScissorTest( cs.scissorTest );
    applyColorMask( cs.colorMask );
    applyDepthMask( cs.depthMask );
    applyStencilMask( cs.stencilMask );

    int buffers = static_cast<int>( cs.buffers );

    // Handle separate color clear
    if( (buffers & static_cast<int>( ClearBuffers::ColorBuffer )) && cs.colorSeparate.enabled )
    {
        buffers = buffers & ~static_cast<int>( ClearBuffers::ColorBuffer );
        applyClearColorSeparate( cs.colorSeparate );
    }

    // Set clear color
    if( (buffers & static_cast<int>( ClearBuffers::ColorBuffer )) && 
        (_clearColor != cs.color || !_shadowingCurrentlyEnabled) )
    {
        glClearColor( cs.color.r(), cs.color.g(), cs.color.b(), cs.color.a() );
        glCheck;
        _clearColor = cs.color;
    }

    // Set clear depth
    if( (buffers & static_cast<int>( ClearBuffers::DepthBuffer )) && 
        (_clearDepth != cs.depth || !_shadowingCurrentlyEnabled) )
    {
        glClearDepth( static_cast<double>( cs.depth ) );
        glCheck;
        _clearDepth = cs.depth;
    }

    // Set clear stencil
    if( (buffers & static_cast<int>( ClearBuffers::StencilBuffer )) && 
        (_clearStencil != cs.stencil || !_shadowingCurrentlyEnabled) )
    {
        glClearStencil( cs.stencil );
        glCheck;
        _clearStencil = cs.stencil;
    }

    // Perform clear
    if( buffers )
    {
        glClear( glWrap( static_cast<ClearBuffers>( buffers ) ) );
        glCheck;
    }

    //_disableClearStateShadowingOneShot = false;
}

// ------------------------------------------------------------------------------------------------
void StateManager::setDrawState( const DrawState& ds )
{
    //_shadowingCurrentlyEnabled = !( _disableDrawStateShadowingOneShot || !ds.shadowingEnabled );

    // Apply viewport and scissor
    applyViewport( ds.viewport );

    // Apply render state
    applyRenderState( ds.renderState );

    // Apply shader program (DSA-aware)
    applyShaderProgram( ds.shader );

    //_disableDrawStateShadowingOneShot = false;
}

// ------------------------------------------------------------------------------------------------
void StateManager::applyRenderState( const RenderState& rs )
{
    applyPrimitiveRestart ( rs.primitiveRestart );
    applyFaceCulling      ( rs.faceCulling );
    applyProgramPointSize ( rs.programPointSize );
    applyRasterizationMode( rs.rasterizationMode );
    applyLineWidth        ( rs.lineWidth );  
    applyStencilTest      ( rs.stencilTest );
    applyDepthTest        ( rs.depthTest );
    applyDepthRange       ( rs.depthRange );
    applyBlending         ( rs.blending );
    applyColorMask        ( rs.colorMask );
    applyDepthMask        ( rs.depthMask );
    applyStencilMask      ( rs.stencilMask );
}

// ------------------------------------------------------------------------------------------------
void StateManager::applyPrimitiveRestart( const PrimitiveRestart& pr )
{
    if( _renderState.primitiveRestart.enabled != pr.enabled || !_shadowingCurrentlyEnabled )
    {
        enable( GL_PRIMITIVE_RESTART, pr.enabled );
        glCheck;
        _renderState.primitiveRestart.enabled = pr.enabled;
    }

    if( pr.enabled && 
        (_renderState.primitiveRestart.index != pr.index || !_shadowingCurrentlyEnabled) )
    {
        glPrimitiveRestartIndex( pr.index ); 
        glCheck;
        _renderState.primitiveRestart.index = pr.index;
    }
}

// ------------------------------------------------------------------------------------------------
void StateManager::applyFaceCulling( const FaceCulling& fc )
{
    if( _renderState.faceCulling.enabled != fc.enabled || !_shadowingCurrentlyEnabled )
    {
        enable( GL_CULL_FACE, fc.enabled );
        glCheck;
        _renderState.faceCulling.enabled = fc.enabled;
    }

    if( fc.enabled && 
        (_renderState.faceCulling.cullFace != fc.cullFace || !_shadowingCurrentlyEnabled) )
    {
        glCullFace( glWrap( fc.cullFace ) );
        glCheck;
        _renderState.faceCulling.cullFace = fc.cullFace;
    }

    if( _renderState.faceCulling.frontFaceWindingOrder != fc.frontFaceWindingOrder || 
        !_shadowingCurrentlyEnabled )
    {
        glFrontFace( glWrap( fc.frontFaceWindingOrder ) );
        glCheck;
        _renderState.faceCulling.frontFaceWindingOrder = fc.frontFaceWindingOrder;
    }
}

// ------------------------------------------------------------------------------------------------
void StateManager::applyProgramPointSize( const ProgramPointSize& programPointSize )
{
    if( _renderState.programPointSize.enabled != programPointSize.enabled || 
        !_shadowingCurrentlyEnabled )
    {
        enable( GL_PROGRAM_POINT_SIZE, programPointSize.enabled );
        glCheck;
        _renderState.programPointSize = programPointSize;
    }
}

// ------------------------------------------------------------------------------------------------
void StateManager::applyRasterizationMode( RenderState::RasterizationMode rasterizationMode )
{
    if( _renderState.rasterizationMode != rasterizationMode || !_shadowingCurrentlyEnabled )
    {
        glPolygonMode( GL_FRONT_AND_BACK, glWrap( rasterizationMode ) );
        glCheck;
        _renderState.rasterizationMode = rasterizationMode;
    }
}

// ------------------------------------------------------------------------------------------------
void StateManager::applyLineWidth( float lineWidth )
{
    if( _renderState.lineWidth != lineWidth || !_shadowingCurrentlyEnabled )
    {
        const float w = Math::clamp( lineWidth, Device::minLinesWidth(), Device::maxLinesWidth() );
        glLineWidth( w );
        glCheck;
        _renderState.lineWidth = w;
    }
}

// ------------------------------------------------------------------------------------------------
void StateManager::applyScissorTest( const ScissorTest& scissorTest )
{
    const Math::irect& rectangle = scissorTest.rect;

    const bool enabled = scissorTest.enabled && 
                        !rectangle.isEmpty() && 
                        rectangle.width() > 0 && 
                        rectangle.height() > 0;

    if( _renderState.scissorTest.enabled != enabled || !_shadowingCurrentlyEnabled )
    {
        enable( GL_SCISSOR_TEST, enabled );
        glCheck;
        _renderState.scissorTest.enabled = enabled;
    }

    if( enabled && 
        ( _renderState.scissorTest.rect != scissorTest.rect || !_shadowingCurrentlyEnabled) )
    {
        glScissor( rectangle.left(), rectangle.bottom(), rectangle.width(), rectangle.height() );
        glCheck;
        _renderState.scissorTest.rect = scissorTest.rect;
    }
}

// ------------------------------------------------------------------------------------------------
void StateManager::applyStencilTest( const StencilTest& stencilTest )
{
    if( _renderState.stencilTest.enabled != stencilTest.enabled || !_shadowingCurrentlyEnabled )
    {
        enable( GL_STENCIL_TEST, stencilTest.enabled );
        glCheck;
        _renderState.stencilTest.enabled = stencilTest.enabled;
    }

    if( stencilTest.enabled || !_shadowingCurrentlyEnabled )
    {
        applyStencil( FaceCulling::Face::Front, 
                     _renderState.stencilTest.frontFace, 
                     stencilTest.frontFace );
        applyStencil( FaceCulling::Face::Back, 
                     _renderState.stencilTest.backFace, 
                     stencilTest.backFace );
    }
}

// ------------------------------------------------------------------------------------------------
void StateManager::applyStencil( 
    FaceCulling::Face face, 
    StencilTestFace& currentTest, 
    const StencilTestFace& test )
{
    if( (currentTest.stencilFailOperation != test.stencilFailOperation) ||
        (currentTest.depthFailStencilPassOperation != test.depthFailStencilPassOperation) ||
        (currentTest.depthPassStencilPassOperation != test.depthPassStencilPassOperation) ||
        !_shadowingCurrentlyEnabled )
    {
        glStencilOpSeparate( 
            glWrap( face ),
            glWrap( test.stencilFailOperation ),
            glWrap( test.depthFailStencilPassOperation ),
            glWrap( test.depthPassStencilPassOperation ) 
        );
        glCheck;

        currentTest.stencilFailOperation = test.stencilFailOperation;
        currentTest.depthFailStencilPassOperation = test.depthFailStencilPassOperation;
        currentTest.depthPassStencilPassOperation = test.depthPassStencilPassOperation;
    }

    if( (currentTest.function != test.function) ||
        (currentTest.referenceValue != test.referenceValue) ||
        (currentTest.mask != test.mask) ||
        !_shadowingCurrentlyEnabled )
    {
        glStencilFuncSeparate( 
            glWrap( face ),
            glWrap( test.function ),
            test.referenceValue,
            test.mask 
        );
        glCheck;

        currentTest.function = test.function;
        currentTest.referenceValue = test.referenceValue;
        currentTest.mask = test.mask;
    }
}

// ------------------------------------------------------------------------------------------------
void StateManager::applyDepthTest( const DepthTest& depthTest )
{
    if( _renderState.depthTest.enabled != depthTest.enabled || !_shadowingCurrentlyEnabled )
    {
        enable( GL_DEPTH_TEST, depthTest.enabled );
        glCheck;
        _renderState.depthTest.enabled = depthTest.enabled;
    }

    if( depthTest.enabled && 
        (_renderState.depthTest.function != depthTest.function || !_shadowingCurrentlyEnabled) )
    {
        glDepthFunc( glWrap( depthTest.function ) );
        glCheck;
        _renderState.depthTest.function = depthTest.function;
    }
}

// ------------------------------------------------------------------------------------------------
void StateManager::applyDepthRange( const DepthRange& depthRange )
{
    if( (_renderState.depthRange.nearValue != depthRange.nearValue) ||
        (_renderState.depthRange.farValue != depthRange.farValue) ||
        !_shadowingCurrentlyEnabled )
    {
        glDepthRange( depthRange.nearValue, depthRange.farValue );
        glCheck;
        _renderState.depthRange.nearValue = depthRange.nearValue;
        _renderState.depthRange.farValue = depthRange.farValue;
    }
}

// ------------------------------------------------------------------------------------------------
void StateManager::applyBlending( const Blending& blending )
{
    bool blendingEnabled = false;
    for( size_t i = 0; i < blending.enabled.size(); ++i ) 
    {
        if( _renderState.blending.enabled[i] != blending.enabled[i] || 
            !_shadowingCurrentlyEnabled )
        {
            enable( GL_BLEND, blending.enabled[i], static_cast<int>( i ) );
            glCheck;
            _renderState.blending.enabled[i] = blending.enabled[i];
        }

        blendingEnabled = blendingEnabled || blending.enabled[i]; 
    }

    if( blendingEnabled || !_shadowingCurrentlyEnabled )
    {
        if( (_renderState.blending.sourceRGBFactor != blending.sourceRGBFactor) ||
            (_renderState.blending.destinationRGBFactor != blending.destinationRGBFactor) ||
            (_renderState.blending.sourceAlphaFactor != blending.sourceAlphaFactor) ||
            (_renderState.blending.destinationAlphaFactor != blending.destinationAlphaFactor) ||
            !_shadowingCurrentlyEnabled )
        {
            glBlendFuncSeparate(
                glWrap( blending.sourceRGBFactor ),
                glWrap( blending.destinationRGBFactor ),
                glWrap( blending.sourceAlphaFactor ),
                glWrap( blending.destinationAlphaFactor )
            );
            glCheck;

            _renderState.blending.sourceRGBFactor = blending.sourceRGBFactor;
            _renderState.blending.destinationRGBFactor = blending.destinationRGBFactor;
            _renderState.blending.sourceAlphaFactor = blending.sourceAlphaFactor;
            _renderState.blending.destinationAlphaFactor = blending.destinationAlphaFactor;
        }

        if( (_renderState.blending.rgbEquation != blending.rgbEquation) ||
            (_renderState.blending.alphaEquation != blending.alphaEquation) ||
            !_shadowingCurrentlyEnabled )
        {
            glBlendEquationSeparate(
                glWrap( blending.rgbEquation ),
                glWrap( blending.alphaEquation )
            );
            glCheck;

            _renderState.blending.rgbEquation = blending.rgbEquation;
            _renderState.blending.alphaEquation = blending.alphaEquation;
        }

        if( _renderState.blending.color != blending.color || !_shadowingCurrentlyEnabled )
        {
            glBlendColor( 
                blending.color.r(), 
                blending.color.g(), 
                blending.color.b(), 
                blending.color.a() 
            );
            glCheck;
            _renderState.blending.color = blending.color;
        }
    }
}

// ------------------------------------------------------------------------------------------------
void StateManager::applyColorMask( const ColorMask& colorMask )
{
    if( !_renderState.colorMask.equals( colorMask ) || !_shadowingCurrentlyEnabled )
    {
        glColorMask( colorMask.r, colorMask.g, colorMask.b, colorMask.a );
        glCheck;
        _renderState.colorMask = colorMask;
    }
}

// ------------------------------------------------------------------------------------------------
void StateManager::applyDepthMask( const DepthMask& depthMask )
{
    if( _renderState.depthMask.enabled != depthMask.enabled || !_shadowingCurrentlyEnabled )
    {
        glDepthMask( depthMask.enabled );
        glCheck;
        _renderState.depthMask = depthMask;
    }
}

// ------------------------------------------------------------------------------------------------
void StateManager::applyStencilMask( const StencilMask& stencilMask )
{
    if( _renderState.stencilMask.front != stencilMask.front || !_shadowingCurrentlyEnabled )
    {
        glStencilMaskSeparate( GL_FRONT, stencilMask.front );
        glCheck;
        _renderState.stencilMask.front = stencilMask.front;
    }

    if( _renderState.stencilMask.back != stencilMask.back || !_shadowingCurrentlyEnabled )
    {
        glStencilMaskSeparate( GL_BACK, stencilMask.back );
        glCheck;
        _renderState.stencilMask.back = stencilMask.back;
    }
}

// ------------------------------------------------------------------------------------------------
void StateManager::applyShaderProgram( const ShaderPtr& shader )
{
    // Validate shader
    ShaderPtr newShader = (!shader || shader->isCreated()) ? shader : _currentShader;
    assert( newShader == shader && "Shader must be created before use" );

    // Apply shader if changed or shadowing disabled
    if( newShader != _currentShader || !_shadowingCurrentlyEnabled )
    {
        if( newShader )
            newShader->bind();
        else
        {
            glUseProgram( 0 );
            glCheck;
        }

        _currentShader = newShader;
    }

    // Apply uniforms (DSA - no binding required, but apply only if shader is current)
    if( _currentShader )
        _currentShader->applyUniforms();
}

// ------------------------------------------------------------------------------------------------
void StateManager::applyClearColorSeparate( const ClearColorSeparate& clearColorSeparate )
{
    if( !clearColorSeparate.enabled )
        return;

    for( size_t i = 0; i < clearColorSeparate.color.size(); ++i )
    {
        const auto& c = clearColorSeparate.color[i];
        
        if( std::holds_alternative<Color>( c ) )
        {
            glClearBufferfv( GL_COLOR, static_cast<int>( i ), std::get<Color>( c ).rgba() );
            glCheck;
        }
        else if( std::holds_alternative<Math::ivec4>( c ) )
        {
            const auto& color = std::get<Math::ivec4>( c );
            glClearBufferiv( GL_COLOR, static_cast<int>( i ), &color.r );
            glCheck;
        }
    }
}

// ------------------------------------------------------------------------------------------------
void StateManager::applyViewport( const ViewportState& vs )
{
    //if( vs.rect != _viewportState.rect || !_shadowingCurrentlyEnabled )
    {
        glViewport( vs.rect.left(), vs.rect.bottom(), vs.rect.width(), vs.rect.height() );
        glCheck;
        _viewportState.rect = vs.rect;
    }
}

// ------------------------------------------------------------------------------------------------
void StateManager::validateState( bool drawStateCheck, bool clearStateCheck ) const
{
#ifdef _DEBUG
    GLint   val[4];
    GLfloat valf[4];

    if( drawStateCheck )
    {
        assert( static_cast<bool>( glIsEnabled( GL_PRIMITIVE_RESTART ) ) == _renderState.primitiveRestart.enabled );
        glGetIntegerv( GL_PRIMITIVE_RESTART_INDEX, val );
        assert( val[0] == static_cast<int>( _renderState.primitiveRestart.index ) );

        assert( static_cast<bool>( glIsEnabled( GL_CULL_FACE ) ) == _renderState.faceCulling.enabled );
        glGetIntegerv( GL_CULL_FACE_MODE, val );
        assert( val[0] == glWrap( _renderState.faceCulling.cullFace ) );
        glGetIntegerv( GL_FRONT_FACE, val );
        assert( val[0] == glWrap( _renderState.faceCulling.frontFaceWindingOrder ) );

        assert( static_cast<bool>( glIsEnabled( GL_PROGRAM_POINT_SIZE ) ) == _renderState.programPointSize.enabled );

        glGetIntegerv( GL_POLYGON_MODE, val );
        assert( val[0] == glWrap( _renderState.rasterizationMode ) );

        assert( static_cast<bool>( glIsEnabled( GL_DEPTH_TEST ) ) == _renderState.depthTest.enabled );
        glGetIntegerv( GL_DEPTH_FUNC, val );
        assert( val[0] == glWrap( _renderState.depthTest.function ) );

        glGetFloatv( GL_DEPTH_RANGE, valf );
        assert( valf[0] == _renderState.depthRange.nearValue );
        assert( valf[1] == _renderState.depthRange.farValue );

        glGetIntegerv( GL_CURRENT_PROGRAM, val );
        if( _currentShader )
            assert( val[0] == static_cast<int>( _currentShader->id() ) );
        else
            assert( val[0] == 0 );

        glGetIntegerv( GL_VIEWPORT, val );
        assert( val[0] == _viewportState.rect.left() );
        assert( val[1] == _viewportState.rect.bottom() );
        assert( val[2] == _viewportState.rect.width() );
        assert( val[3] == _viewportState.rect.height() );
    }

    if( drawStateCheck || clearStateCheck )
    {
        assert( static_cast<bool>( glIsEnabled( GL_SCISSOR_TEST ) ) == _renderState.scissorTest.enabled );
        
        glGetIntegerv( GL_DEPTH_WRITEMASK, val );
        assert( static_cast<bool>( val[0] ) == _renderState.depthMask.enabled );
    }

    if( clearStateCheck )
    {
        glGetFloatv( GL_COLOR_CLEAR_VALUE, valf );
        for( int i = 0; i < 4; ++i )
            assert( valf[i] == _clearColor.rgba()[i] );

        glGetFloatv( GL_DEPTH_CLEAR_VALUE, valf );
        assert( valf[0] == _clearDepth );

        glGetIntegerv( GL_STENCIL_CLEAR_VALUE, val );
        assert( val[0] == _clearStencil );
    }
#endif // _DEBUG
}