// Sampler.h
//
#ifndef TEXTURE_SAMPLER_ONCE
#define TEXTURE_SAMPLER_ONCE

#include "s2Renderer_API.h"

#include "OpenGLObject.h"

#include <memory>

namespace Renderer{

/************************************************************************************************/
/*                                           Sampler                                            */
/************************************************************************************************/
class Sampler;
typedef std::shared_ptr<Sampler> SamplerPtr;

class S2RENDERER_API Sampler : public OpenGLObject
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
	static SamplerPtr New(const MinificationFilter &minFilter, const MagnificationFilter &magFilter, const Wrap &wrapS, const Wrap &wrapT, float maxAnisotropy);

public:
	//OBJECT_DECLARE_MOVEABLE( Sampler )
	//OBJECT_DISABLE_COPY( Sampler )

	//Sampler();
	Sampler( const MinificationFilter &minFilter, const MagnificationFilter &magFilter, const Wrap &wrapS, const Wrap &wrapT, float maxAnisotropy );
	~Sampler();

	MinificationFilter  minificationFilter()  const { return _minificationFilter; }
	MagnificationFilter magnificationFilter() const { return _magnificationFilter; }
	Wrap wrapS()                              const { return _wrapS; }
	Wrap wrapT()                              const { return _wrapT; }
	float maximumAnistropy()                  const { return _maximumAnistropy; }


	bool create()  override;
	void destroy() override;

	void bind( int unit ) const;
	void unbind( int unit ) const;
	
	static void unbindAll( int unit );

private:
	void reset() override;
	void bind()    const override;
	void unbind()  const override;

private:
	MinificationFilter  _minificationFilter;
	MagnificationFilter _magnificationFilter;
	Wrap                _wrapS;
	Wrap                _wrapT;
	float               _maximumAnistropy;
};

}
#endif // !TEXTURE_SAMPLER_ONCE
