// Texture.cpp
//
#include "Texture.h"

#include "OpenGL.h"
#include "OpenGLWrap.h"

#include "Sampler.h"

#include "BuiltIn.h"
using namespace s2::OpenGL;

// -------------------------------------------------------------------------------------------------
//Texture2DPtr Texture2D::New( const TextureDescription &desc, void *data )
//{
//	return std::make_shared<Texture2D>( desc );
//}

// -------------------------------------------------------------------------------------------------
Texture2D::Texture2D( const TextureDescription &description /*, void* data*/ )
: _description( description )
{}

// -------------------------------------------------------------------------------------------------
Texture2D::Texture2D( Texture2D &&other )
{
	std::swap( _format     , other._format      );
	std::swap( _description, other._description );

	std::swap( _created,   other._created);
	std::swap( _objectID,  other._objectID);
}

// -------------------------------------------------------------------------------------------------
Texture2D::~Texture2D()
{
	destroy();
}

// -------------------------------------------------------------------------------------------------
Texture2D &Texture2D::operator=( Texture2D &&other )
{
	reset();

	std::swap( _format     , other._format      );
	std::swap( _description, other._description );

	std::swap( _created,   other._created);
	std::swap( _objectID,  other._objectID);
	return *this;
}

// -------------------------------------------------------------------------------------------------
void Texture2D::reset()
{
	OpenGLObject::reset();

	_description = {};
	_format      = {};
}

// -------------------------------------------------------------------------------------------------
TextureDescription Texture2D::description() const { return _description; }

// -------------------------------------------------------------------------------------------------
bool Texture2D::create()
{
	destroy();
	glGenTextures( 1, &_objectID );

	if( _description.isRectagle() )
	{
		//glBindTexture(GL_TEXTURE_2D); bind rectangle
	}
	else
	{
		setData( nullptr );
	}

	setDefaultSampler();
	generateMipmaps();


	_created = true;
	return _created;
}

// -------------------------------------------------------------------------------------------------
void Texture2D::destroy()
{
	if( !isCreated() )

	glDeleteTextures( 1, &_objectID );
	reset();
}

// -------------------------------------------------------------------------------------------------
void Texture2D::bind()   const { glBindTexture( GL_TEXTURE_2D, _objectID ); }
void Texture2D::unbind() const { glBindTexture( GL_TEXTURE_2D, 0 ); }
void Texture2D::unbindAll()    { glBindTexture( GL_TEXTURE_2D, 0 ); }

// -------------------------------------------------------------------------------------------------
void Texture2D::setDefaultSampler()
{
	const Sampler &sampler = BuiltIn::samplerLinearClamp;

	/*todo : handle gl texture rectangle in case of description.isRectangle*/
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glWrap( sampler.minificationFilter() ) );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glWrap( sampler.magnificationFilter() ) );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     glWrap( sampler.wrapS() ) );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     glWrap( sampler.wrapT() ) );
}

// -------------------------------------------------------------------------------------------------
void Texture2D::generateMipmaps()
{
	if( _description.isGenerateMipmapsEnabled() )
		glGenerateMipmap( GL_TEXTURE_2D ); // dv: to be tested 
}

// -------------------------------------------------------------------------------------------------
void Texture2D::setData( void* pixels /*, int rowAlignment = 4 */ )
{
	glActiveTexture( GL_TEXTURE0 ); //bind to last?
	bind();
	glTexImage2D( GL_TEXTURE_2D,
			      0,
			      glWrap( _description.textureFormat() ),
			      _description.width(),
			      _description.height(),
			      0, /*border*/
			      glWrapTextureFormatToPixelFormat( _description.textureFormat() ),
			      glWrapTextureFormatToPixelType( _description.textureFormat() ),
			      pixels );
	unbind();
}


// -------------------------------------------------------------------------------------------------
void Texture2D::update(int xOffset, int yOffset, 
				 int width, int height, 
				 const ImageFormat &imgFormat,
				 const ImageDataType &imgDataType,
				 void* pixels/*, int rowAlignment = 4 */)
{
	bind();
	glTexSubImage2D( GL_TEXTURE_2D,
					 0,
					 xOffset,
					 yOffset,
					 width,
					 height,
					 glWrap( imgFormat ),
					 glWrap( imgDataType ),
					 pixels );
	unbind();
}

// -------------------------------------------------------------------------------------------------
void Texture2D::update(int xOffset, int yOffset, 
				 int width, int height, 
				 const ImageFormat &imgFormat,
				 const ImageDataType &imgDataType,
				 const ReadPixelBuffer &gpuBuffer/*, int rowAlignment = 4 */)
{
	bind();
	gpuBuffer.bind();
	glTexSubImage2D( GL_TEXTURE_2D,
					 0,
					 xOffset,
					 yOffset,
					 width,
					 height,
					 glWrap( imgFormat ),
					 glWrap( imgDataType ),
					 nullptr );
	unbind();
}


// -------------------------------------------------------------------------------------------------
void Texture2D::validateAlignment( int rowAlignment )
{
	if( ( rowAlignment != 1 ) &&
		( rowAlignment != 2 ) &&
		( rowAlignment != 4 ) &&
		( rowAlignment != 8 ) )
		assert( ("Invalid rowAlignment", 0) );
}