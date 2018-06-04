// Texture.cpp
//
#include "Texture.h"

#include "OpenGL.h"
#include "OpenGLWrap.h"

namespace OpenGL {

/************************************************************************************************/
/*                                             Texture                                          */
/************************************************************************************************/
// -----------------------------------------------------------------------------------------------
Texture::Texture() 
: _textureID(0)
, _valid(false)
, _dataFormat(InvalidFormat)
, _dataType(InvalidDataType)
, _minFilter(InvalidMinFilter)
, _magFilter(InvalidMagFilter)
, _unit(-1)
, _wrap(Clamp)
, _channelCount(0)
{}

// -----------------------------------------------------------------------------------------------
Texture::~Texture() 
{
	destroy(); 
}

// -----------------------------------------------------------------------------------------------
void Texture::create()
{
	destroy();
	glGenTextures(1, &_textureID);
	glBindTexture(glWrap(type()),_textureID);
}

// -----------------------------------------------------------------------------------------------
void Texture::destroy()
{
	if(_textureID != 0)
	{
		glDeleteTextures(1, &_textureID);
		_textureID = 0;
	}
	_valid = false;
}

// -----------------------------------------------------------------------------------------------
void Texture::setMinFilter( const TextureMinFilter &minFilter )
{
	_minFilter = minFilter; 
	
	bind();
	glTexParameteri( glWrap(type()),GL_TEXTURE_MIN_FILTER, _minFilter);
	unbind();
}

// -----------------------------------------------------------------------------------------------
void Texture::setMagFilter( const TextureMagFilter &magFilter )
{ 
	_magFilter = magFilter; 
	
	bind();
	glTexParameteri( glWrap(type()),GL_TEXTURE_MAG_FILTER, _magFilter);
	unbind();
}

// -----------------------------------------------------------------------------------------------
void Texture::setWrapMode ( const WrapMode &wrap )
{ 
	_wrap = wrap;
	
	bind();
	glTexParameteri( glWrap(type()),GL_TEXTURE_WRAP_R, _wrap); 
	glTexParameteri( glWrap(type()),GL_TEXTURE_WRAP_S, _wrap); 
	glTexParameteri( glWrap(type()),GL_TEXTURE_WRAP_T, _wrap); 
	unbind();
}

// -----------------------------------------------------------------------------------------------
void Texture::bind(unsigned int unit ) const { _unit = unit; glActiveTexture( GL_TEXTURE0+_unit ); glBindTexture(glWrap(type()),_textureID); glActiveTexture( GL_TEXTURE0 ); }
void Texture::unbind()	               const { glActiveTexture( GL_TEXTURE0+_unit ); glBindTexture(glWrap(type()),0); glActiveTexture( GL_TEXTURE0 ); _unit = -1;            }

// -----------------------------------------------------------------------------------------------
bool Texture::isValid()	const { return _valid; }
bool Texture::isBound() const { return _unit>=0;}

// -----------------------------------------------------------------------------------------------
unsigned int Texture::unit()   const { return _unit; }
int          Texture::width()  const { return -1;    }
int          Texture::height() const { return -1;    }

// -----------------------------------------------------------------------------------------------
unsigned int	          Texture::id()	          const { return _textureID;  }
unsigned int	          Texture::channelCount() const { return _channelCount;  }
Texture::DataFormat       Texture::dataFormat()   const { return _dataFormat; }
Texture::DataType         Texture::dataType()     const { return _dataType;   }
Texture::TextureMinFilter Texture::minFilter()    const { return _minFilter;  }
Texture::TextureMagFilter Texture::magFilter()    const { return _magFilter;  }
Texture::WrapMode         Texture::wrap()         const { return _wrap;       }


/************************************************************************************************/
/*                                           Texture1D                                          */
/************************************************************************************************/
Texture1D::Texture1D()
: Texture()
, _textureWidth(0)
{}

// -----------------------------------------------------------------------------------------------
Texture::TextureType	Texture1D::type() const { return Texture_1D; }

// -----------------------------------------------------------------------------------------------
void Texture1D::create( int channelCount, const DataFormat &format, const DataType &type, bool mipmaps, int   width, void *pixels)
{
	Texture::create();

	_textureWidth = width;
	_channelCount = channelCount;
	_dataFormat   = format;
	_dataType     = type;

	if( mipmaps )	gluBuild1DMipmaps(GL_TEXTURE_1D,_channelCount, _textureWidth, format, type, pixels);
	else			glTexImage1D(GL_TEXTURE_1D, 0, format, width, 0, format, type, pixels);

	_valid = glValidate;
}

// -----------------------------------------------------------------------------------------------
void Texture1D::clear()
{
	glBindTexture(GL_TEXTURE_1D, _textureID);

	unsigned char *pxl = new unsigned char[_textureWidth*1*_channelCount];
	memset(pxl,0,_textureWidth*1*_channelCount);
	glTexSubImage1D(GL_TEXTURE_1D, 0,0, _textureWidth, _dataFormat, _dataType,pxl);
	delete [] pxl;

	_valid = glValidate;
}


/************************************************************************************************/
/*                                          Texture2D                                           */
/************************************************************************************************/
Texture2D::Texture2D()
: Texture()
, _textureWidth(0)
, _textureHeight(0)
, _u(1.f)
, _v(1.f)
{}

// -----------------------------------------------------------------------------------------------
void Texture2D::setDefaultFilterParameters()
{
	setMinFilter(MinFilterNearest);
	setMagFilter(MagFilterNearest);
	setWrapMode(Clamp);
}

// -----------------------------------------------------------------------------------------------
float                Texture2D::u()      const { return _u; }
float                Texture2D::v()      const { return _v; }
int                  Texture2D::width()  const { return _textureWidth; }
int                  Texture2D::height() const { return _textureHeight; }
Texture::TextureType Texture2D::type()   const { return Texture_2D; }

// -----------------------------------------------------------------------------------------------
void Texture2D::create( int channelCount, const DataFormat &format, const DataType &type, bool mipmaps, int width, int height, void *pixels )
{
	Texture::create();

	_textureWidth  = width;
	_textureHeight = height;
	_channelCount  = channelCount;
	_dataFormat    = format;
	_dataType      = type;

	if( mipmaps )	gluBuild2DMipmaps(GL_TEXTURE_2D,channelCount, width, height, /* */ _dataFormat, _dataType, pixels);
	else			glTexImage2D(GL_TEXTURE_2D, 0,  channelCount, width, height, 0,    _dataFormat, _dataType, pixels);

	_valid = glValidate;

	if( _valid )
		setDefaultFilterParameters();
}

// -----------------------------------------------------------------------------------------------
void Texture2D::createFromFrameBuffer( int width, int height )
{
	// copy the framebuffer pixels to a texture
	Texture::create();

	_textureWidth  = width;
	_textureHeight = height;
	_channelCount  = 4;
	_dataFormat    = RGBA;
	_dataType      = UByte_8;

	glCopyTexImage2D(GL_TEXTURE_2D, 0, _dataFormat, 0, 0, _textureWidth, _textureHeight, 0 );
	_valid = glValidate;

	setDefaultFilterParameters();
}

// -----------------------------------------------------------------------------------------------
void Texture2D::update( int xOffset, int yOffset, int width, int height, void *pixels)
{
	bind();
	glTexSubImage2D(GL_TEXTURE_2D,0, xOffset, yOffset, width, height, _dataFormat, _dataType, pixels);
	_valid = glValidate;
	unbind();
}

// -----------------------------------------------------------------------------------------------
void Texture2D::clear()
{
	unsigned char *pxl = new unsigned char[_textureWidth*_textureHeight*_channelCount];
	memset(pxl,0,_textureWidth*_textureHeight*_channelCount);
	update(0,0,_textureWidth,_textureHeight,pxl);
	delete [] pxl;

	_u      = 1.f;
	_v      = 1.f;
}

}