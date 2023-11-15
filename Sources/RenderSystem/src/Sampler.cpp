// Sampler.cpp
//
#include "Sampler.h"

#include "OpenGL.h"
#include "OpenGLWrap.h"
#include "OpenGLCheck.h"

using namespace RenderSystem;

// ------------------------------------------------------------------------------------------------
SamplerPtr Sampler::New(const MinificationFilter &minFilter, const MagnificationFilter &magFilter, const Wrap &wrapS, const Wrap &wrapT, float maxAnisotropy)
{
	return std::make_shared<Sampler>( minFilter,magFilter,wrapS,wrapT,maxAnisotropy );
}

// ------------------------------------------------------------------------------------------------
//Sampler::Sampler()
//: Sampler( MinificationFilter::Nearest, MagnificationFilter::Nearest, Wrap::Clamp, Wrap::Clamp, 1 )
//{}

// ------------------------------------------------------------------------------------------------
Sampler::Sampler( const MinificationFilter &minFilter, const MagnificationFilter &magFilter, const Wrap &wrapS, const Wrap &wrapT, float maxAnisotropy )
: _minificationFilter( minFilter )
, _magnificationFilter( magFilter )
, _wrapS( wrapS )
, _wrapT( wrapT )
, _maximumAnistropy( maxAnisotropy )
{
	
	//if (Device.Extensions.AnisotropicFiltering)
	//{
	//	glSamplerParameter(_objectID, (ArbSamplerObjects)All.TextureMaxAnisotropyExt, maximumAnistropy);
	//}
	//else
	//{
	//	if (maximumAnistropy != 1)
	//	{
	//		throw new InsufficientVideoCardException("Anisotropic filtering is not supported.  The extension GL_EXT_texture_filter_anisotropic was not found.");
	//	}
	//}
	create();
}

// ------------------------------------------------------------------------------------------------
//Sampler::Sampler( Sampler &&other )
//: Sampler()
//{
//	std::swap( _minificationFilter , other._minificationFilter  );
//	std::swap( _magnificationFilter, other._magnificationFilter );
//	std::swap( _wrapS              , other._wrapS               );
//	std::swap( _wrapT              , other._wrapT               );
//	std::swap( _maximumAnistropy   , other._maximumAnistropy    );
//
//	std::swap( _created,   other._created);
//	std::swap( _objectID,  other._objectID);
//}

// ------------------------------------------------------------------------------------------------
Sampler::~Sampler()
{
	destroy();
}

// ------------------------------------------------------------------------------------------------
//Sampler &Sampler::operator=( Sampler &&other )
//{
//	reset();
//
//	std::swap( _minificationFilter , other._minificationFilter  );
//	std::swap( _magnificationFilter, other._magnificationFilter );
//	std::swap( _wrapS              , other._wrapS               );
//	std::swap( _wrapT              , other._wrapT               );
//	std::swap( _maximumAnistropy   , other._maximumAnistropy    );
//
//	std::swap( _created,   other._created);
//	std::swap( _objectID,  other._objectID);
//	return *this;
//}

// -------------------------------------------------------------------------------------------------
void Sampler::reset()
{
	OpenGLObject::reset();

	_minificationFilter  = MinificationFilter::Nearest;
	_magnificationFilter = MagnificationFilter::Nearest; 
	_wrapS               = Wrap::Clamp; 
	_wrapT               = Wrap::Clamp; 
	_maximumAnistropy    = 1;
}

// ------------------------------------------------------------------------------------------------
void Sampler::create()
{
	destroy();
	OpenGLObject::create();
	
	glGenSamplers( 1, &_objectID );
	glCheck;
	glSamplerParameteri( _objectID, GL_TEXTURE_MIN_FILTER, glWrap( _minificationFilter ) );
	glSamplerParameteri( _objectID, GL_TEXTURE_MAG_FILTER, glWrap( _magnificationFilter ) );
	glSamplerParameteri( _objectID, GL_TEXTURE_WRAP_S,     glWrap( _wrapS ) );
	glSamplerParameteri( _objectID, GL_TEXTURE_WRAP_T,     glWrap( _wrapT ) );
	glCheck;

	_created = _objectID != 0;
}

// ------------------------------------------------------------------------------------------------
void Sampler::destroy()
{
	if( !isCreated() )
		return;

	// need a valid context to be performed correctly.
	// remove static allocation and prefer late initialization
	
	glDeleteSamplers( 1, &_objectID );
	glCheck;
	reset();
}

// ------------------------------------------------------------------------------------------------
void Sampler::bind()           const {}
void Sampler::bind( int unit ) const {  glBindSampler( unit, _objectID ); glCheck; }

// ------------------------------------------------------------------------------------------------
void Sampler::unbind()           const {}
void Sampler::unbind( int unit ) const {  glBindSampler( unit, 0 ); glCheck; }

// ------------------------------------------------------------------------------------------------
void Sampler::unbindAll( int unit )
{
	glBindSampler( unit, 0 );
	glCheck;
}