// Texture.h
//
#ifndef RENDERER_TEXTURE_H
#define RENDERER_TEXTURE_H

#include "Resource.h"

#include "RenderCore/Texture.h"

namespace Renderer{

class Texture : public Resource 
{
public:
	Texture() : Resource() {}
};

class Texture2D : public Texture 
{
public:
	Texture2D();

	virtual int width() const;
	virtual int height() const;
	virtual bool hasAlpha() const;

protected:
	RenderCore::Texture2DPtr _texture;
};

}

#endif // !RENDERER_TEXTURE_H