// TextureSampler.h
//
#ifndef TEXTURE_SAMPLER_ONCE
#define TEXTURE_SAMPLER_ONCE

#include "s2OpenGL_API.h"

#include <memory>

namespace s2{
namespace OpenGL{

class TextureSampler;
typedef std::shared_ptr<TextureSampler> TextureSamplerPtr;

class S2OPENGL_API TextureSampler
{
public:
	enum class Wrap 
	{
		Clamp,
		Repeat,
		MirroredRepeat
	};

	enum class MinificationFilter
	{
		Nearest,
		Linear,
		NearestMipmapNearest,
		LinearMipmapNearest,
		NearestMipmapLinear,
		LinearMipmapLinear,
	};

	enum class MagnificationFilter
	{
		Nearest,
		Linear
	};

public:
	static TextureSamplerPtr New(const MinificationFilter &minFilter, const MagnificationFilter &magFilter, const Wrap &wrapS, const Wrap &wrapT, float maxAnisotropy);

	static TextureSamplerPtr nearestClamp();
	static TextureSamplerPtr linearClamp();
	static TextureSamplerPtr nearestRepeat();
	static TextureSamplerPtr linearRepeat();

public:
	TextureSampler()
	: _minificationFilter( MinificationFilter::Nearest )
	, _magnificationFilter( MagnificationFilter::Nearest)
	, _wrapS( Wrap::Clamp )
	, _wrapT( Wrap::Clamp )
	, _maximumAnistropy(1)
	, _id(0)
	{}

	TextureSampler( const MinificationFilter &minFilter, const MagnificationFilter &magFilter, const Wrap &wrapS, const Wrap &wrapT, float maxAnisotropy );
	~TextureSampler();

	MinificationFilter  minificationFilter() const { return _minificationFilter; }
	MagnificationFilter magnificationFilter() const { return _magnificationFilter; }
	Wrap wrapS() const { return _wrapS; }
	Wrap wrapT()  const { return _wrapT; }
	float maximumAnistropy() const { return _maximumAnistropy; }

	void bind( int unit );
	static void unbind(int unit);

private:
	MinificationFilter _minificationFilter;
	MagnificationFilter _magnificationFilter;
	Wrap _wrapS;
	Wrap _wrapT;
	float _maximumAnistropy;
	unsigned int _id;
};

}}

#endif // !TEXTURE_SAMPLER_ONCE
