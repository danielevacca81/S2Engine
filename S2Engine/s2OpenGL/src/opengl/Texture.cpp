// Texture.cpp
//
#include "Texture.h"

#include "OpenGL.h"
#include "OpenGLWrap.h"

#include "TextureSampler.h"

using namespace s2::OpenGL;

//-------------------------------------------------------------------------------------------------
Texture2DPtr Texture2D::New( const Texture2DDescription &desc)
{
	return std::make_shared<Texture2D>(desc);
}

//-------------------------------------------------------------------------------------------------
Texture2D::Texture2D()
: _id(0)
, _description()
{}

//-------------------------------------------------------------------------------------------------
Texture2D::Texture2D(const Texture2DDescription &description, void* data )
: _description(description)
{
	glGenTextures( 1, &_id );

	if (_description.isRectagle())
	{
		//glBindTexture(GL_TEXTURE_2D); bind rectangle

	}
	else
	{
		//WritePixelBuffer::unbind();
		glActiveTexture( GL_TEXTURE0 ); //bind to last?
		bind();
		glTexImage2D( 
			GL_TEXTURE_2D, 
			0, 
			glWrap(_description.textureFormat()),
			_description.width(),
			_description.height(),
			0, /*border*/
			glWrapTextureFormatToPixelFormat( _description.textureFormat()),
			glWrapTextureFormatToPixelType( _description.textureFormat()),
			data);
	}

	setDefaultSampler();
	generateMipmaps();
}

//-------------------------------------------------------------------------------------------------
Texture2D::~Texture2D()
{
	if (_id != 0)
	{
		glDeleteTextures(1, &_id);
		_id = 0;
	}
}

//-------------------------------------------------------------------------------------------------
unsigned int Texture2D::id() const { return _id; }
Texture2DDescription Texture2D::description() const { return _description; }

//-------------------------------------------------------------------------------------------------
void Texture2D::bind()   { glBindTexture(GL_TEXTURE_2D, _id); }
void Texture2D::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

//-------------------------------------------------------------------------------------------------
void Texture2D::setDefaultSampler()
{
	const TextureSamplerPtr sampler = TextureSampler::linearClamp();

	/*todo : handle gl texture rectangle in case of description.isRectangle*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glWrap( sampler->minificationFilter() ));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glWrap(sampler->magnificationFilter() ));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glWrap(sampler->wrapS() ));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glWrap(sampler->wrapT() ));
}

//-------------------------------------------------------------------------------------------------
void Texture2D::generateMipmaps()
{
	if( _description.isGenerateMipmapsEnabled() )
		glGenerateMipmap( GL_TEXTURE_2D ); // dv: to be tested 
}

