// Shader.cpp
//
#include "Shader.h"

#include "OpenGL.h"
#include "OpenGLCheck.h"
#include "OpenGLWrap.h"

#include <vector>
#include <cassert>

using namespace s2::RenderCore;

#pragma region Helpers
// ------------------------------------------------------------------------------------------------
static inline bool attach( GLenum shaderAttachType, unsigned int objID, unsigned int shaderHandle, ShaderStageType shaderType )
{
    if( objID == 0 || shaderHandle == 0 )
        return false;

    if( glWrap( shaderType ) != shaderAttachType )
        return false;

    glAttachShader( objID, shaderHandle );
    glCheck;
    return true;
}

// ------------------------------------------------------------------------------------------------
static inline Uniform* createUniform( const std::string& name, unsigned int loc, unsigned int type )
{
    switch( type )
    {
    case GL_FLOAT:           return new Uniform( loc, name, float { 0.f } );
    case GL_FLOAT_VEC2:      return new Uniform( loc, name, Math::fvec2 { 0.f, 0.f } );
    case GL_FLOAT_VEC3:      return new Uniform( loc, name, Math::fvec3 { 0.f, 0.f, 0.f } );
    case GL_FLOAT_VEC4:      return new Uniform( loc, name, Math::fvec4 { 0.f, 0.f, 0.f, 0.f } );

    case GL_INT:             return new Uniform( loc, name, int { 0 } );
    case GL_INT_VEC2:        return new Uniform( loc, name, Math::ivec2 { 0, 0 } );
    case GL_INT_VEC3:        return new Uniform( loc, name, Math::ivec3 { 0, 0, 0 } );
    case GL_INT_VEC4:        return new Uniform( loc, name, Math::ivec4 { 0, 0, 0, 0 } );

    case GL_BOOL:            return new Uniform( loc, name, bool { false } );

    case GL_FLOAT_MAT2:      return new Uniform( loc, name, Math::fmat2( 1.f ) );
    case GL_FLOAT_MAT3:      return new Uniform( loc, name, Math::fmat3( 1.f ) );
    case GL_FLOAT_MAT4:      return new Uniform( loc, name, Math::fmat4( 1.f ) );

    case GL_SAMPLER_2D:
    case GL_INT_SAMPLER_2D:
    case GL_SAMPLER_CUBE:    return new Uniform( loc, name, int { 0 } );

    // OpenGL 4.0+
    case GL_DOUBLE:          return new Uniform( loc, name, double { 0.0 } );
    case GL_DOUBLE_VEC2:     return new Uniform( loc, name, Math::dvec2 { 0.0, 0.0 } );
    case GL_DOUBLE_VEC3:     return new Uniform( loc, name, Math::dvec3 { 0.0, 0.0, 0.0 } );
    case GL_DOUBLE_VEC4:     return new Uniform( loc, name, Math::dvec4 { 0.0, 0.0, 0.0, 0.0 } );

    case GL_DOUBLE_MAT2:     return new Uniform( loc, name, Math::dmat2( 1.0 ) );
    case GL_DOUBLE_MAT3:     return new Uniform( loc, name, Math::dmat3( 1.0 ) );
    case GL_DOUBLE_MAT4:     return new Uniform( loc, name, Math::dmat4( 1.0 ) );

    // OpenGL 4.6+ Bindless
    case GL_UNSIGNED_INT64_ARB: return new Uniform( loc, name, uint64_t { 0 } );

    default:
        assert( false && "Uniform type not supported" );
        return nullptr;
    }

    return nullptr;
}
#pragma endregion

// ------------------------------------------------------------------------------------------------
ShaderPtr Shader::New()
{
    return std::make_shared<Shader>();
}

// ------------------------------------------------------------------------------------------------
Shader::Shader()
{
    create();
}

// ------------------------------------------------------------------------------------------------
Shader::~Shader()
{
    destroy();
}

// -------------------------------------------------------------------------------------------------
void Shader::reset()
{
    OpenGLObject::reset();

    _linked = false;
    _name = "";

    for( auto& it : _uniforms )
        delete it.second;

    _uniforms.clear();
    _attributes.clear();
    _residentTextures.clear();
}

// ------------------------------------------------------------------------------------------------
void Shader::create()
{
    if( isValid() )
        return;

    OpenGLObject::create();

    _objectID = glCreateProgram();
    glCheck;

    if( _name.empty() )
        _name = "Shader";

}

// ------------------------------------------------------------------------------------------------
void Shader::destroy()
{
    if( !isValid() )
        return;

    // Make textures non-resident
    for( auto& texture : _residentTextures )
    {
        if( texture && texture->isResident() )
            texture->makeNonResident();
    }

    _vshd = nullptr;
    _fshd = nullptr;
    _gshd = nullptr;
    _cshd = nullptr;
    _tshd = nullptr;
    _teshd = nullptr;

    glDeleteProgram( _objectID );
    glCheck;

    reset();
}

// ------------------------------------------------------------------------------------------------
std::string Shader::name() const
{
    return _name;
}

// ------------------------------------------------------------------------------------------------
bool Shader::attachVertexShaderStage( const ShaderStagePtr& shader )
{
    if( !isValid() || !shader )
        return false;

    if( !attach( GL_VERTEX_SHADER, _objectID, shader->id(), shader->type() ) )
        return false;

    _vshd = shader;
    return true;
}

// ------------------------------------------------------------------------------------------------
bool Shader::attachFragmentShaderStage( const ShaderStagePtr& shader )
{
    if( !isValid() || !shader )
        return false;

    if( !attach( GL_FRAGMENT_SHADER, _objectID, shader->id(), shader->type() ) )
        return false;

    _fshd = shader;
    return true;
}

// ------------------------------------------------------------------------------------------------
bool Shader::attachGeometryShaderStage( const ShaderStagePtr& shader )
{
    if( !isValid() || !shader )
        return false;

    if( !attach( GL_GEOMETRY_SHADER, _objectID, shader->id(), shader->type() ) )
        return false;

    _gshd = shader;
    return true;
}

// ------------------------------------------------------------------------------------------------
bool Shader::attachComputeShaderStage( const ShaderStagePtr& shader )
{
    if( !isValid() || !shader )
        return false;

    if( shader->type() != ShaderStageType::Compute )
        return false;

    if( !attach( GL_COMPUTE_SHADER, _objectID, shader->id(), shader->type() ) )
        return false;

    _cshd = shader;
    return true;
}

// ------------------------------------------------------------------------------------------------
bool Shader::attachTessellationControlShaderStage( const ShaderStagePtr& shader )
{
    if( !isValid() || !shader )
        return false;

    if( shader->type() != ShaderStageType::TessellationControl )
        return false;

    if( !attach( GL_TESS_CONTROL_SHADER, _objectID, shader->id(), shader->type() ) )
        return false;

    _tshd = shader;
    return true;
}

// ------------------------------------------------------------------------------------------------
bool Shader::attachTessellationEvaluationShaderStage( const ShaderStagePtr& shader )
{
    if( !isValid() || !shader )
        return false;

    if( shader->type() != ShaderStageType::TessellationEvaluation )
        return false;

    if( !attach( GL_TESS_EVALUATION_SHADER, _objectID, shader->id(), shader->type() ) )
        return false;

    _teshd = shader;
    return true;
}

// ------------------------------------------------------------------------------------------------
bool Shader::isLinked() const 
{ 
    return _linked; 
}

// ------------------------------------------------------------------------------------------------
void Shader::bind() const
{
    glUseProgram( _objectID );
    glCheck;
}

// ------------------------------------------------------------------------------------------------
void Shader::unbind() const
{
    glUseProgram( 0 );
    glCheck;
}

// ------------------------------------------------------------------------------------------------
// DSA: Set uniform and apply immediately (OpenGL 4.1+)
// ------------------------------------------------------------------------------------------------
void Shader::setUniform( const std::string& uniformName, const UniformValue& value )
{
    auto it = _uniforms.find( uniformName );
    if( it == _uniforms.end() )
        return;

    // Set value and apply immediately using DSA (no binding required)
    it->second->setValue( value );
    it->second->setDSA( _objectID );
}

// ------------------------------------------------------------------------------------------------
// DSA: Apply all changed uniforms (OpenGL 4.1+)
// ------------------------------------------------------------------------------------------------
void Shader::applyUniforms()
{
    for( auto& [name, uniform] : _uniforms )
    {
        if( uniform->isChanged() )
            uniform->setDSA( _objectID );
    }
}

// ------------------------------------------------------------------------------------------------
const Uniform* Shader::uniform( const std::string& name ) const
{
    auto it = _uniforms.find( name );
    if( it == _uniforms.end() )
        return nullptr;

    return it->second;
}

// ------------------------------------------------------------------------------------------------
void Shader::findUniforms()
{
    // Clear previous uniforms
    for( auto& it : _uniforms )
        delete it.second;
    _uniforms.clear();

    int numberOfUniforms;
    glGetProgramiv( _objectID, GL_ACTIVE_UNIFORMS, &numberOfUniforms );

    int uniformNameMaxLength;
    glGetProgramiv( _objectID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformNameMaxLength );
    glCheck;

    for( int i = 0; i < numberOfUniforms; ++i )
    {
        int uniformNameLength;
        int uniformSize;
        GLenum uniformType;
        std::vector<GLchar> uniformInternalName( uniformNameMaxLength );
        
        glGetActiveUniform( 
            _objectID, 
            i, 
            uniformNameMaxLength, 
            &uniformNameLength, 
            &uniformSize, 
            &uniformType, 
            &uniformInternalName[0] 
        );
        glCheck;

        const std::string uniformName( uniformInternalName.begin(), uniformInternalName.begin() + uniformNameLength );

        // Skip built-in uniforms
        if( uniformName.find( "gl_" ) == 0 )
            continue;

        // TODO: Support uniform arrays
        if( uniformSize != 1 )
            continue;

        int uniformLocation = glGetUniformLocation( _objectID, uniformName.c_str() );
        glCheck;

        _uniforms[uniformName] = createUniform( uniformName, uniformLocation, uniformType );
    }
}

// -------------------------------------------------------------------------------------------------
int Shader::objectLabelIdentifier() const
{
    return GL_PROGRAM;
}

// ------------------------------------------------------------------------------------------------
// Bindless Texture Support
// ------------------------------------------------------------------------------------------------

void Shader::setTextureHandle( const std::string& uniformName, uint64_t handle )
{
    auto it = _uniforms.find( uniformName );
    if( it == _uniforms.end() )
        return;

    // Set uint64 uniform value and apply immediately
    it->second->setValue( handle );
    it->second->setDSA( _objectID );
}

// ------------------------------------------------------------------------------------------------
void Shader::setTexture( const std::string& uniformName, const Texture2DPtr& texture )
{
    assert( texture && texture->isValid() && "Texture must be valid" );
    
    // Make resident if not already
    if( !texture->isResident() )
    {
        texture->makeResident();
        _residentTextures.push_back( texture ); // Track for cleanup
    }
    
    // Set bindless handle
    setTextureHandle( uniformName, texture->bindlessHandle() );
}