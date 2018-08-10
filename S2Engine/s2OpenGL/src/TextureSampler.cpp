// TextureSampler.cpp
//
#include "TextureSampler.h"

#include "OpenGL.h"
#include "OpenGLWrap.h"

using namespace s2::OpenGL;

// ------------------------------------------------------------------------------------------------
TextureSamplerPtr TextureSampler::New(const MinificationFilter &minFilter, const MagnificationFilter &magFilter, const Wrap &wrapS, const Wrap &wrapT, float maxAnisotropy)
{
	return std::make_shared<TextureSampler>( minFilter,magFilter,wrapS,wrapT,maxAnisotropy );
}

// ------------------------------------------------------------------------------------------------
TextureSamplerPtr TextureSampler::nearestClamp()
{
	static TextureSamplerPtr s;
	if (!s)
		s = New( MinificationFilter::Nearest, MagnificationFilter::Nearest, Wrap::Clamp, Wrap::Clamp, 1);
	return s;
}

// ------------------------------------------------------------------------------------------------
TextureSamplerPtr TextureSampler::linearClamp()
{
	static TextureSamplerPtr s;
	if (!s)
		s = New(MinificationFilter::Linear, MagnificationFilter::Linear, Wrap::Clamp, Wrap::Clamp, 1);
	return s;
}

// ------------------------------------------------------------------------------------------------
TextureSamplerPtr TextureSampler::nearestRepeat()
{
	static TextureSamplerPtr s;
	if (!s)
		s = New(MinificationFilter::Nearest, MagnificationFilter::Nearest, Wrap::Repeat, Wrap::Repeat, 1);
	return s;
}

// ------------------------------------------------------------------------------------------------
TextureSamplerPtr TextureSampler::linearRepeat()
{
	static TextureSamplerPtr s;
	if (!s)
		s = New(MinificationFilter::Linear, MagnificationFilter::Linear, Wrap::Repeat, Wrap::Repeat, 1);
	return s;
}


// ------------------------------------------------------------------------------------------------
TextureSampler::TextureSampler(const MinificationFilter &minFilter, const MagnificationFilter &magFilter, const Wrap &wrapS, const Wrap &wrapT, float maxAnisotropy)
	: _minificationFilter(minFilter)
	, _magnificationFilter(magFilter)
	, _wrapS(wrapS)
	, _wrapT(wrapT)
	, _maximumAnistropy(maxAnisotropy)
{
	glGenSamplers( 1, &_id);
	glSamplerParameteri(_id, GL_TEXTURE_MIN_FILTER, glWrap(minFilter));
	glSamplerParameteri(_id, GL_TEXTURE_MAG_FILTER, glWrap(magFilter));
	glSamplerParameteri(_id, GL_TEXTURE_WRAP_S, glWrap(wrapS));
	glSamplerParameteri(_id, GL_TEXTURE_WRAP_T, glWrap(wrapT));

	//if (Device.Extensions.AnisotropicFiltering)
	//{
	//	glSamplerParameter(_id, (ArbSamplerObjects)All.TextureMaxAnisotropyExt, maximumAnistropy);
	//}
	//else
	//{
	//	if (maximumAnistropy != 1)
	//	{
	//		throw new InsufficientVideoCardException("Anisotropic filtering is not supported.  The extension GL_EXT_texture_filter_anisotropic was not found.");
	//	}
	//}
}

// ------------------------------------------------------------------------------------------------
TextureSampler::~TextureSampler()
{
	
	// need a valid context to be performed correctly.
	// remove static allocation and prefer late initialization
	glDeleteSamplers( 1, &_id );
	glCheck;
}


// ------------------------------------------------------------------------------------------------
void TextureSampler::bind(int unit)
{
	glBindSampler( unit, _id );
}

// ------------------------------------------------------------------------------------------------
void TextureSampler::unbind(int unit)
{
	glBindSampler(unit, 0);
}
