// Texture.cpp
//
#include "Texture.h"

#include "OpenGL.h"
#include "OpenGLWrap.h"

#include "Sampler.h"

#include "BuiltIn.h" //samplers

using namespace s2::Renderer;

// -------------------------------------------------------------------------------------------------
Texture2DPtr Texture2D::New( const TextureDescription &desc, void *data )
{
	return std::make_shared<Texture2D>( desc, data );
}

// -------------------------------------------------------------------------------------------------
Texture2D::Texture2D( const TextureDescription &description, void* data )
: _description( description )
{
	create();

	if( data )
		setData( data );
}

// -------------------------------------------------------------------------------------------------
//Texture2D::Texture2D( Texture2D &&other )
//{
//	std::swap( _format     , other._format      );
//	std::swap( _description, other._description );
//
//	std::swap( _created,   other._created);
//	std::swap( _objectID,  other._objectID);
//}

// -------------------------------------------------------------------------------------------------
Texture2D::~Texture2D()
{
	destroy();
}

// -------------------------------------------------------------------------------------------------
//Texture2D &Texture2D::operator=( Texture2D &&other )
//{
//	reset();
//
//	std::swap( _format     , other._format      );
//	std::swap( _description, other._description );
//
//	std::swap( _created,   other._created);
//	std::swap( _objectID,  other._objectID);
//	return *this;
//}

// -------------------------------------------------------------------------------------------------
void Texture2D::reset()
{
	OpenGLObject::reset();

	_description = TextureDescription( 0,0, TextureFormat::RedGreenBlueAlpha8 );
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
		return;

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
	const SamplerPtr sampler = BuiltIn::samplerLinearClamp;

	/*todo : handle gl texture rectangle in case of description.isRectangle*/
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glWrap( sampler->minificationFilter() ) );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glWrap( sampler->magnificationFilter() ) );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     glWrap( sampler->wrapS() ) );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     glWrap( sampler->wrapT() ) );
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
s2::ImageBufferPtr<unsigned char> Texture2D::readData() const
{
 //public override ReadPixelBuffer CopyToBuffer(
	// ImageFormat format,
	// ImageDatatype dataType,
	// int rowAlignment )
 //{
	// if( format == ImageFormat.StencilIndex )
	// {
	//	 throw new ArgumentException( "StencilIndex is not supported by CopyToBuffer.  Try DepthStencil instead.", "format" );
	// }

	// VerifyRowAlignment( rowAlignment );

	// ReadPixelBufferGL3x pixelBuffer = new ReadPixelBufferGL3x( PixelBufferHint.Stream,
	//															TextureUtility.RequiredSizeInBytes( _description.Width, _description.Height, format, dataType, rowAlignment ) );

	// pixelBuffer.Bind();
	// BindToLastTextureUnit();
	// GL.PixelStore( PixelStoreParameter.PackAlignment, rowAlignment );
	// GL.GetTexImage( _target, 0,
	//				 TypeConverterGL3x.To( format ),
	//				 TypeConverterGL3x.To( dataType ),
	//				 new IntPtr() );

	// return pixelBuffer;
 //}

	// @todo: make user selectable
	const int rowAlignment       = 4;
	const ImageFormat format     = ImageFormat::RedGreenBlueAlpha;
	const ImageDataType dataType = ImageDataType::UnsignedByte;
	const int sizeInBytes        = computeRequiredSizeInBytes( _description.width(), _description.height(), format, dataType, rowAlignment );

	glActiveTexture( GL_TEXTURE0 );
	bind();
	glPixelStorei( GL_PACK_ALIGNMENT, rowAlignment );
	
	ReadPixelBuffer pixelBuffer = ReadPixelBuffer( sizeInBytes, ReadPixelBuffer::UsageHint::Stream );
	pixelBuffer.bind();
	
	glGetTexImage( GL_TEXTURE_2D, 0, glWrap( format ), glWrap( dataType ), BUFFER_OFFSET(0) );
	glCheck;
	//unbind();
	//glCheck;

	s2::ImageBufferPtr<unsigned char> img = s2::ImageBuffer<unsigned char>::New( _description.width(), _description.height(), 4, (unsigned char*)pixelBuffer.receiveData( sizeInBytes ) );
	unbind();

	return img;
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