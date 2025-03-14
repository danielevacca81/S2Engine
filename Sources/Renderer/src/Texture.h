// Texture.h
//
#ifndef RENDERER_TEXTURE_H
#define RENDERER_TEXTURE_H

#include "Resource.h"

namespace Renderer{

class Texture : public Resource 
{
public:
	Texture() {}
};

class Texture2D : public Texture 
{
public:
	Texture2D();

	virtual int width() const;
	virtual int height() const;
	virtual bool hasAlpha() const;
};

}

#endif // !RENDERER_TEXTURE_H