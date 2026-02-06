// MaterialBinder.cpp
//
#include "MaterialBinder.h"

namespace s2 {
namespace Renderer {

void MaterialBinder::bind( const Resources::Material& material, RenderCore::DrawState& drawState )
{
	// Check if material changed
	if( _currentMaterial != &material )
	{
		_currentMaterial = &material;
		_materialSwitches++;

		// Bind material properties to shader uniforms
		bindMaterialProperties( material, drawState );

		// Bind textures to texture units
		bindTextures( material, drawState );
	}
}

void MaterialBinder::bindMaterialProperties( const Resources::Material& material, RenderCore::DrawState& drawState )
{
	if( !drawState.shader )
		return;

	auto& program = drawState.shader;

	// Bind material color properties
	program->setUniformValue( "u_Material.baseColor", material.baseColor().toVec4() );
	program->setUniformValue( "u_Material.emissiveColor", material.emissiveColor().toVec4() );
	program->setUniformValue( "u_Material.roughness", material.roughness() );
	program->setUniformValue( "u_Material.metallic", material.metallic() );
	program->setUniformValue( "u_Material.opacity", material.opacity() );
}

void MaterialBinder::bindTextures( const Resources::Material& material, RenderCore::DrawState& drawState )
{
	using TextureType = Resources::Material::TextureType;

	if( !drawState.shader )
		return;

	auto& program = drawState.shader;
	auto& textureUnits = drawState.textureUnits;

	int textureUnit = 0;

	// Diffuse/Albedo texture
	if( material.hasTexture( TextureType::Diffuse ) )
	{
		auto tex = material.getTexture( TextureType::Diffuse );

		// Set texture in DrawState's texture units
		textureUnits[textureUnit].setTexture( tex );

		// Set uniform for texture sampler
		program->setUniformValue( "u_DiffuseTexture", textureUnit );
		program->setUniformValue( "u_HasDiffuseTexture", 1 );
		textureUnit++;
	}
	else
	{
		program->setUniformValue( "u_HasDiffuseTexture", 0 );
	}

	// Normal map
	if( material.hasTexture( TextureType::Normal ) )
	{
		auto tex = material.getTexture( TextureType::Normal );
		textureUnits[textureUnit].setTexture( tex );

		program->setUniformValue( "u_NormalTexture", textureUnit );
		program->setUniformValue( "u_HasNormalTexture", 1 );
		textureUnit++;
	}
	else
	{
		program->setUniformValue( "u_HasNormalTexture", 0 );
	}

	// Roughness texture
	if( material.hasTexture( TextureType::Roughness ) )
	{
		auto tex = material.getTexture( TextureType::Roughness );
		textureUnits[textureUnit].setTexture( tex );

		program->setUniformValue( "u_RoughnessTexture", textureUnit );
		program->setUniformValue( "u_HasRoughnessTexture", 1 );
		textureUnit++;
	}
	else
	{
		program->setUniformValue( "u_HasRoughnessTexture", 0 );
	}

	// Metallic texture
	if( material.hasTexture( TextureType::Metallic ) )
	{
		auto tex = material.getTexture( TextureType::Metallic );
		textureUnits[textureUnit].setTexture( tex );

		program->setUniformValue( "u_MetallicTexture", textureUnit );
		program->setUniformValue( "u_HasMetallicTexture", 1 );
		textureUnit++;
	}
	else
	{
		program->setUniformValue( "u_HasMetallicTexture", 0 );
	}

	// Ambient Occlusion texture
	if( material.hasTexture( TextureType::AmbientOcclusion ) )
	{
		auto tex = material.getTexture( TextureType::AmbientOcclusion );
		textureUnits[textureUnit].setTexture( tex );

		program->setUniformValue( "u_AOTexture", textureUnit );
		program->setUniformValue( "u_HasAOTexture", 1 );
		textureUnit++;
	}
	else
	{
		program->setUniformValue( "u_HasAOTexture", 0 );
	}

	// Emissive texture
	if( material.hasTexture( TextureType::Emissive ) )
	{
		auto tex = material.getTexture( TextureType::Emissive );
		textureUnits[textureUnit].setTexture( tex );

		program->setUniformValue( "u_EmissiveTexture", textureUnit );
		program->setUniformValue( "u_HasEmissiveTexture", 1 );
		textureUnit++;
	}
	else
	{
		program->setUniformValue( "u_HasEmissiveTexture", 0 );
	}

	// Opacity/Alpha texture
	if( material.hasTexture( TextureType::Opacity ) )
	{
		auto tex = material.getTexture( TextureType::Opacity );
		textureUnits[textureUnit].setTexture( tex );

		program->setUniformValue( "u_OpacityTexture", textureUnit );
		program->setUniformValue( "u_HasOpacityTexture", 1 );
		textureUnit++;
	}
	else
	{
		program->setUniformValue( "u_HasOpacityTexture", 0 );
	}

	// Height/Displacement texture
	if( material.hasTexture( TextureType::Height ) )
	{
		auto tex = material.getTexture( TextureType::Height );
		textureUnits[textureUnit].setTexture( tex );

		program->setUniformValue( "u_HeightTexture", textureUnit );
		program->setUniformValue( "u_HasHeightTexture", 1 );
		textureUnit++;
	}
	else
	{
		program->setUniformValue( "u_HasHeightTexture", 0 );
	}
}

void MaterialBinder::reset()
{
	_currentMaterial = nullptr;
}

}
}