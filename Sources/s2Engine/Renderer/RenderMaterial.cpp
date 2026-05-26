// RenderMaterial.cpp
//
#include "RenderMaterial.h"

#include "RenderCore/Shader.h"
#include "RenderCore/Uniform.h"
#include "RenderCore/Texture.h"

#include <optional>
#include <cassert>

using namespace s2::Renderer;

// --------------------------------------------------------------------------------------------
// Helper: Convert material property to uniform value with type conversion
// --------------------------------------------------------------------------------------------
static inline std::optional<RenderCore::UniformValue> convertProperty(
    const MaterialDefinition::Property& property,
    const RenderCore::UniformValue& targetUniformType )
{
    return std::visit( [&] ( auto&& targetType ) -> std::optional<RenderCore::UniformValue>
    {
        using TargetType = std::decay_t<decltype( targetType )>;

        return std::visit( [&]<typename T>( const T & value ) -> std::optional<RenderCore::UniformValue>
        {
            using ValueType = std::decay_t<T>;

            // 1. Exact type match (no conversion)
            if constexpr( std::is_same_v<ValueType, TargetType> ) { return value; }
            // 2. Numeric conversions
            else if constexpr( std::is_arithmetic_v<ValueType> && std::is_arithmetic_v<TargetType> ) { return static_cast<TargetType>( value ); }
            // 3. Color -> vec4/vec3
            else if constexpr( std::is_same_v<ValueType, Color> && std::is_same_v<TargetType, Math::fvec4> ) { return Math::fvec4( value.r(), value.g(), value.b(), value.a() ); }
            else if constexpr( std::is_same_v<ValueType, Color> && std::is_same_v<TargetType, Math::fvec3> ) { return Math::fvec3( value.r(), value.g(), value.b() ); }
            // 4. Vector expansions
            else if constexpr( std::is_same_v<ValueType, Math::fvec3> && std::is_same_v<TargetType, Math::fvec4> ) { return Math::fvec4( value.x, value.y, value.z, 1.0f ); }
            else if constexpr( std::is_same_v<ValueType, Math::fvec2> && std::is_same_v<TargetType, Math::fvec3> ) { return Math::fvec3( value.x, value.y, 0.0f ); }
            else if constexpr( std::is_same_v<ValueType, Math::fvec2> && std::is_same_v<TargetType, Math::fvec4> ) { return Math::fvec4( value.x, value.y, 0.0f, 1.0f ); }
            // 5. Vector reductions
            else if constexpr( std::is_same_v<ValueType, Math::fvec4> && std::is_same_v<TargetType, Math::fvec3> ) { return Math::fvec3( value.x, value.y, value.z ); }
            // 6. Double -> Float vector conversions
            else if constexpr( std::is_same_v<ValueType, Math::dvec2> && std::is_same_v<TargetType, Math::fvec2> ) { return Math::fvec2( static_cast<float>( value.x ), static_cast<float>( value.y ) ); }
            else if constexpr( std::is_same_v<ValueType, Math::dvec3> && std::is_same_v<TargetType, Math::fvec3> ) { return Math::fvec3( static_cast<float>( value.x ), static_cast<float>( value.y ), static_cast<float>( value.z ) ); }
            else if constexpr( std::is_same_v<ValueType, Math::dvec4> && std::is_same_v<TargetType, Math::fvec4> ) { return Math::fvec4( static_cast<float>( value.x ), static_cast<float>( value.y ), static_cast<float>( value.z ), static_cast<float>( value.w ) ); }
            else if constexpr( std::is_same_v<ValueType, Math::fvec2> && std::is_same_v<TargetType, Math::dvec2> ) { return Math::dvec2( value.x, value.y ); }
            else if constexpr( std::is_same_v<ValueType, Math::fvec3> && std::is_same_v<TargetType, Math::dvec3> ) { return Math::dvec3( value.x, value.y, value.z ); }
            else if constexpr( std::is_same_v<ValueType, Math::fvec4> && std::is_same_v<TargetType, Math::dvec4> ) { return Math::dvec4( value.x, value.y, value.z, value.w ); }
            // 7. Matrix conversions (double -> float)
            else if constexpr( std::is_same_v<ValueType, Math::dmat2> && std::is_same_v<TargetType, Math::fmat2> ) { return Math::fmat2( value ); }
            else if constexpr( std::is_same_v<ValueType, Math::dmat3> && std::is_same_v<TargetType, Math::fmat3> ) { return Math::fmat3( value ); }
            else if constexpr( std::is_same_v<ValueType, Math::dmat4> && std::is_same_v<TargetType, Math::fmat4> ) { return Math::fmat4( value ); }
            else if constexpr( std::is_same_v<ValueType, Math::fmat2> && std::is_same_v<TargetType, Math::dmat2> ) { return Math::dmat2( value ); }
            else if constexpr( std::is_same_v<ValueType, Math::fmat3> && std::is_same_v<TargetType, Math::dmat3> ) { return Math::dmat3( value ); }
            else if constexpr( std::is_same_v<ValueType, Math::fmat4> && std::is_same_v<TargetType, Math::dmat4> ) { return Math::dmat4( value ); }
            else
            {
                // Unsupported conversion
                return std::nullopt;
            }
        }, property );
    }, targetUniformType );
}



// ------------------------------------------------------------------------------------------------
Material MaterialDefinition::createMaterial()
{
    return Material( this );
}

// ------------------------------------------------------------------------------------------------
MaterialDefinition::Property Material::property( const MaterialDefinition::PropertyID &id ) const
{
    if( auto it = _overrides.find( id ); it != _overrides.end() )
        return it->second;
    return _definition->defaultProperties.at( id );
}

// ------------------------------------------------------------------------------------------------
void Material::apply( const ResourceManager& rm ) const
{
	// shader object from resource manager
	auto shader = rm.shader( _definition->shader );
    if( !shader )
    {
        assert( false && "Material::apply: Shader resource not found" );
        return;
	}

	auto applyProperty = [&]( const std::string& id, const MaterialDefinition::Property& val )
	{
		if( auto* texID = std::get_if<TextureID>( &val ) )
		{
			// Texture property: resolve handle, ensure residency, set bindless handle
			auto tex = rm.texture( *texID );
			if( tex && tex->isValid() )
			{
				if( !tex->isResident() )
					tex->makeResident();
				shader->setUniform( id, tex->bindlessHandle() );
			}
		}
		else
		{
			auto* uniform = shader->uniform( id );
			if( uniform )
				shader->setUniform( id, *convertProperty( val, uniform->value() ) );
		}
	};

	// first apply properties from the material definition, then apply overrides from the instance
	for( const auto& [id, val] : _definition->defaultProperties ) 
	{
		if( _overrides.find( id ) == _overrides.end() )
			applyProperty( id, val );
	}
	// then apply instance overrides
	for( const auto& [id, val] : _overrides ) 
		applyProperty( id, val );
}







#if 0

// --------------------------------------------------------------------------------------------
// Helper: Convert material property to uniform value with type conversion
// --------------------------------------------------------------------------------------------
static inline std::optional<RenderCore::UniformValue> convertProperty( 
    const RenderMaterial::Property& property, 
    const RenderCore::UniformValue& targetUniformType )
{
    return std::visit( [&]( auto&& targetType ) -> std::optional<RenderCore::UniformValue>
    {
        using TargetType = std::decay_t<decltype( targetType )>;

        return std::visit( [&]<typename T>( const T& value ) -> std::optional<RenderCore::UniformValue>
        {
            using ValueType = std::decay_t<T>;

            // 1. Exact type match (no conversion)
            if constexpr( std::is_same_v<ValueType, TargetType> )                                                  { return value;}
            // 2. Numeric conversions
            else if constexpr( std::is_arithmetic_v<ValueType> && std::is_arithmetic_v<TargetType> )               { return static_cast<TargetType>( value ); }
            // 3. Color -> vec4/vec3
            else if constexpr( std::is_same_v<ValueType, Color> && std::is_same_v<TargetType, Math::fvec4> )       {return Math::fvec4( value.r(), value.g(), value.b(), value.a() );}
            else if constexpr( std::is_same_v<ValueType, Color> && std::is_same_v<TargetType, Math::fvec3> )       { return Math::fvec3( value.r(), value.g(), value.b() ); }
            // 4. Vector expansions
            else if constexpr( std::is_same_v<ValueType, Math::fvec3> && std::is_same_v<TargetType, Math::fvec4> ) { return Math::fvec4( value.x, value.y, value.z, 1.0f );}
            else if constexpr( std::is_same_v<ValueType, Math::fvec2> && std::is_same_v<TargetType, Math::fvec3> ) { return Math::fvec3( value.x, value.y, 0.0f );}
            else if constexpr( std::is_same_v<ValueType, Math::fvec2> && std::is_same_v<TargetType, Math::fvec4> ) { return Math::fvec4( value.x, value.y, 0.0f, 1.0f );}
            // 5. Vector reductions
            else if constexpr( std::is_same_v<ValueType, Math::fvec4> && std::is_same_v<TargetType, Math::fvec3> ) { return Math::fvec3( value.x, value.y, value.z ); }
            // 6. Double -> Float vector conversions
            else if constexpr( std::is_same_v<ValueType, Math::dvec2> && std::is_same_v<TargetType, Math::fvec2> ) { return Math::fvec2( static_cast<float>( value.x ), static_cast<float>( value.y ) );}
            else if constexpr( std::is_same_v<ValueType, Math::dvec3> && std::is_same_v<TargetType, Math::fvec3> ) { return Math::fvec3( static_cast<float>( value.x ), static_cast<float>( value.y ), static_cast<float>( value.z ) );}
            else if constexpr( std::is_same_v<ValueType, Math::dvec4> && std::is_same_v<TargetType, Math::fvec4> ) { return Math::fvec4( static_cast<float>( value.x ), static_cast<float>( value.y ), static_cast<float>( value.z ), static_cast<float>( value.w ) ); }
            else if constexpr( std::is_same_v<ValueType, Math::fvec2> && std::is_same_v<TargetType, Math::dvec2> ) { return Math::dvec2( value.x, value.y ); }
            else if constexpr( std::is_same_v<ValueType, Math::fvec3> && std::is_same_v<TargetType, Math::dvec3> ) { return Math::dvec3( value.x, value.y, value.z ); }
            else if constexpr( std::is_same_v<ValueType, Math::fvec4> && std::is_same_v<TargetType, Math::dvec4> ) { return Math::dvec4( value.x, value.y, value.z, value.w ); }
            // 7. Matrix conversions (double -> float)
            else if constexpr( std::is_same_v<ValueType, Math::dmat2> && std::is_same_v<TargetType, Math::fmat2> ) { return Math::fmat2( value ); }
            else if constexpr( std::is_same_v<ValueType, Math::dmat3> && std::is_same_v<TargetType, Math::fmat3> ) { return Math::fmat3( value ); }
            else if constexpr( std::is_same_v<ValueType, Math::dmat4> && std::is_same_v<TargetType, Math::fmat4> ) { return Math::fmat4( value ); }
            else if constexpr( std::is_same_v<ValueType, Math::fmat2> && std::is_same_v<TargetType, Math::dmat2> ) { return Math::dmat2( value ); }
            else if constexpr( std::is_same_v<ValueType, Math::fmat3> && std::is_same_v<TargetType, Math::dmat3> ) { return Math::dmat3( value ); }
            else if constexpr( std::is_same_v<ValueType, Math::fmat4> && std::is_same_v<TargetType, Math::dmat4> ) { return Math::dmat4( value ); }
            else
            {
                // Unsupported conversion
                return std::nullopt;
            }
        }, property );
    }, targetUniformType );
}

// --------------------------------------------------------------------------------------------
// Apply material properties to shader uniforms (DSA)
// --------------------------------------------------------------------------------------------
void RenderMaterial::applyPropertiesToShader( RenderCore::Shader& shader ) const
{
    for( const auto& [name, property] : _properties )
    {
        const auto* uniform = shader.uniform( name );
        if( !uniform )
            continue; // Uniform not found in shader, skip

        // Convert property to uniform type
        auto convertedValue = convertProperty( property, uniform->value() );
        
        if( convertedValue )
        {
            // Set uniform immediately (no binding required)
            shader.setUniform( name, *convertedValue );
        }
        else
        {
            // Conversion failed - type incompatible
#ifdef _DEBUG
            // TODO: Log warning about incompatible types
#endif
        }
    }
}

// --------------------------------------------------------------------------------------------
// Apply textures to shader (Bindless - DSA)
// --------------------------------------------------------------------------------------------
void RenderMaterial::applyTexturesToShader( 
    RenderCore::Shader& shader, 
    const ResourceManager& resourceManager ) const
{
    // Set textures directly by uniform name (bindless)
    for( const auto& [uniformName, textureHandle] : _textures )
    {
        auto texture = resourceManager.texture( textureHandle );
        if( !texture )
            continue; // Texture not found, skip

        // Bindless: Set texture (makes resident automatically)
        shader.setTexture( uniformName, texture );
    }
}

// ------------------------------------------------------------------------------------------------
void RenderMaterial::set        ( const std::string& name, const Property& value )    { _properties[name] = value; }
void RenderMaterial::setBool    ( const std::string& name, bool value )               { _properties[name] = value; }
void RenderMaterial::setInt     ( const std::string& name, int value )                { _properties[name] = value; }
void RenderMaterial::setFloat   ( const std::string& name, float value )              { _properties[name] = value; }
void RenderMaterial::setColor   ( const std::string& name, const Color& value )       { _properties[name] = value; }
void RenderMaterial::setVec2    ( const std::string& name, const Math::fvec2& value ) { _properties[name] = value; }
void RenderMaterial::setVec3    ( const std::string& name, const Math::fvec3& value ) { _properties[name] = value; }
void RenderMaterial::setVec4    ( const std::string& name, const Math::fvec4& value ) { _properties[name] = value; }
void RenderMaterial::setMatrix3 ( const std::string& name, const Math::fmat3& value ) { _properties[name] = value; }
void RenderMaterial::setMatrix4 ( const std::string& name, const Math::fmat4& value ) { _properties[name] = value; }

// Set texture by uniform name (not unit index!)
void RenderMaterial::setTexture( const std::string& uniformName, ResourceID textureHandle )
{
    _textures[uniformName] = textureHandle;
}

// ------------------------------------------------------------------------------------------------
bool RenderMaterial::operator<( const RenderMaterial& other ) const
{
    // @todo
	// Sort by blend mode first (opaque before transparent)

    // Opaque materials rendered before transparent (front-to-back)
    if( blendMode != other.blendMode )
        return blendMode == BlendMode::Opaque && other.blendMode != BlendMode::Opaque;

    // Then sort by shader (minimize shader switches)
    if( shader != other.shader )
        return shader < other.shader;

    // Then by material properties (arbitrary but consistent)
    return _properties.size() < other._properties.size();
}

#endif