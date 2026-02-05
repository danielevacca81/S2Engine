// CubemapTexture.cpp
//
#include "CubemapTexture.h"

#include <stdexcept>

using namespace s2::Resources;

// -------------------------------------------------------------------------------------------------
CubemapTexturePtr CubemapTexture::New( const std::string& name, int faceSize, int channels )
{
	return std::make_shared<CubemapTexture>( name, faceSize, channels );
}

// -------------------------------------------------------------------------------------------------
CubemapTexturePtr CubemapTexture::New( const std::string& name,
									   const std::array<s2::Pixmap<uint8_t>, 6>& faces )
{
	return std::make_shared<CubemapTexture>( name, faces );
}

// -------------------------------------------------------------------------------------------------
CubemapTexture::CubemapTexture( const std::string& name, int faceSize, int channels )
	: _name( name )
	, _faceSize( faceSize )
	, _channels( channels )
{
	// Initialize empty faces
	const char* faceNames[] = { "Right", "Left", "Top", "Bottom", "Front", "Back" };

	for( int i = 0; i < 6; ++i )
	{
		std::string faceName = name + "_" + faceNames[i];
		_faces[i] = Texture::New( faceName, faceSize, faceSize, channels );
	}
}

// -------------------------------------------------------------------------------------------------
CubemapTexture::CubemapTexture( const std::string& name,
								const std::array<s2::Pixmap<uint8_t>, 6>& faces )
	: _name( name )
{
	// Validate and set faces
	if( faces[0].isEmpty() )
		throw std::runtime_error( "CubemapTexture: First face cannot be empty" );

	_faceSize = faces[0].width();
	_channels = faces[0].numChannels();

	const char* faceNames[] = { "Right", "Left", "Top", "Bottom", "Front", "Back" };

	for( int i = 0; i < 6; ++i )
	{
		validateFaceSize( faces[i] );

		std::string faceName = name + "_" + faceNames[i];
		_faces[i] = Texture::New( faceName, faces[i] );
	}
}

// -------------------------------------------------------------------------------------------------
TexturePtr CubemapTexture::getFace( Face face ) const
{
	return _faces[static_cast<int>( face )];
}

// -------------------------------------------------------------------------------------------------
void CubemapTexture::setFace( Face face, TexturePtr texture )
{
	if( !texture )
		return;

	// Validate dimensions
	if( texture->width() != _faceSize || texture->height() != _faceSize )
		throw std::runtime_error( "CubemapTexture: Face size mismatch" );

	if( texture->channels() != _channels )
		throw std::runtime_error( "CubemapTexture: Face channels mismatch" );

	_faces[static_cast<int>( face )] = texture;
}

// -------------------------------------------------------------------------------------------------
void CubemapTexture::setFace( Face face, const s2::Pixmap<uint8_t>& pixmap )
{
	validateFaceSize( pixmap );

	auto texture = Texture::New( _name + "_" + faceToString( face ), pixmap );
	_faces[static_cast<int>( face )] = texture;
}

// -------------------------------------------------------------------------------------------------
bool CubemapTexture::isValid() const
{
	for( const auto& face : _faces )
	{
		if( !face || face->isEmpty() )
			return false;
	}
	return true;
}

// -------------------------------------------------------------------------------------------------
bool CubemapTexture::hasFace( Face face ) const
{
	auto faceTexture = _faces[static_cast<int>( face )];
	return faceTexture && !faceTexture->isEmpty();
}

// -------------------------------------------------------------------------------------------------
void CubemapTexture::validateFaceSize( const s2::Pixmap<uint8_t>& pixmap ) const
{
	if( pixmap.width() != _faceSize || pixmap.height() != _faceSize )
		throw std::runtime_error( "CubemapTexture: Face must be square and match cubemap size" );

	if( pixmap.numChannels() != _channels )
		throw std::runtime_error( "CubemapTexture: Face channels mismatch" );
}

// -------------------------------------------------------------------------------------------------
const char* CubemapTexture::faceToString( Face face )
{
	switch( face )
	{
	case Face::PositiveX: return "Right";
	case Face::NegativeX: return "Left";
	case Face::PositiveY: return "Top";
	case Face::NegativeY: return "Bottom";
	case Face::PositiveZ: return "Front";
	case Face::NegativeZ: return "Back";
	default:              return "Unknown";
	}
}

// -------------------------------------------------------------------------------------------------
CubemapTexture::Face CubemapTexture::stringToFace( const std::string& str )
{
	if( str == "Right" || str == "+X" )  return Face::PositiveX;
	if( str == "Left" || str == "-X" )   return Face::NegativeX;
	if( str == "Top" || str == "+Y" )    return Face::PositiveY;
	if( str == "Bottom" || str == "-Y" ) return Face::NegativeY;
	if( str == "Front" || str == "+Z" )  return Face::PositiveZ;
	if( str == "Back" || str == "-Z" )   return Face::NegativeZ;

	throw std::runtime_error( "Invalid cubemap face name: " + str );
}