// Skybox.cpp
//
#include "Skybox.h"

#include "Math/Math.h"

using namespace Renderer;

// -------------------------------------------------------------------------------------------------
SkyboxPtr Skybox::New( const std::string& name )
{
	return std::make_shared<Skybox>( name );
}

// -------------------------------------------------------------------------------------------------
SkyboxPtr Skybox::NewSolidColor( const std::string& name, const Color& color )
{
	auto skybox = std::make_shared<Skybox>( name );
	skybox->setSolidColor( color );
	return skybox;
}

// -------------------------------------------------------------------------------------------------
SkyboxPtr Skybox::NewGradient( const std::string& name,
							   const Color& topColor,
							   const Color& bottomColor )
{
	auto skybox = std::make_shared<Skybox>( name );
	skybox->setGradient( topColor, bottomColor );
	return skybox;
}

// -------------------------------------------------------------------------------------------------
SkyboxPtr Skybox::NewCubemap( const std::string& name, CubemapTexturePtr cubemap )
{
	auto skybox = std::make_shared<Skybox>( name );
	skybox->setCubemap( cubemap );
	return skybox;
}

// -------------------------------------------------------------------------------------------------
Skybox::Skybox( const std::string& name )
	: _name( name )
{
}

// -------------------------------------------------------------------------------------------------
void Skybox::setSolidColor( const Color& color )
{
	_type = Type::SolidColor;
	_solidColor = color;
	_cubemap = nullptr;
}

// -------------------------------------------------------------------------------------------------
void Skybox::setGradient( const Color& topColor, const Color& bottomColor )
{
	_type = Type::Gradient;
	_topColor = topColor;
	_bottomColor = bottomColor;
	_cubemap = nullptr;
}

// -------------------------------------------------------------------------------------------------
void Skybox::setCubemap( CubemapTexturePtr cubemap )
{
	if( !cubemap || !cubemap->isValid() )
	{
		_type = Type::None;
		_cubemap = nullptr;
		return;
	}

	_type = Type::Cubemap;
	_cubemap = cubemap;
}

// -------------------------------------------------------------------------------------------------
bool Skybox::isValid() const
{
	switch( _type )
	{
	case Type::None:
		return true;

	case Type::SolidColor:
	case Type::Gradient:
		return true;

	case Type::Cubemap:
		return _cubemap && _cubemap->isValid();

	default:
		return false;
	}
}