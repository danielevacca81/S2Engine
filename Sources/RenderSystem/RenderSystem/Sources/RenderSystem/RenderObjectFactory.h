#ifndef _RENDEROBJECTFACTORY_H_INCLUDED_
#define _RENDEROBJECTFACTORY_H_INCLUDED_

#include "VertexBuffer.h"
#include "Texture.h"

/**
  * This is an abstract class that provides an interface on top of various
  * implementations of render object (textures, vertex buffers, ...) 
  * factories.
  *
  * @author Vincent Prat
  */
class RenderObjectFactory
{
//----------------------------------------------------------------------
// Public methods
//--
public:

	/**
	  * Default object constructor
	  */
	RenderObjectFactory() {}

	/**
	  * Object destructor
	  */
	virtual ~RenderObjectFactory() {}

	//------------------------------------------------------------------
	// Object creation functions
	//--

	/**
	  * This function should be used to allocate memory for a new vertex
	  * buffer.
      * @param count Number of vertex to be created
      * @param format Format of vertex buffer ( See format flags in VertexBuffer)
      * @param isDynamic Specifies that vertex buffer is dynamic. [default: true]
	  * @return A pointer to the new vertex buffer or NULL if it could not be created.
	  */
	virtual VertexBuffer* CreateVertexBuffer( int count, DWORD format, bool isDynamic = true ) = 0;

	/**
	  * This function should be used to allocate memory for a new texture
      * @param Width width of the texture
	  * @param Height height of the texture
      * @param format format of the texture
      * @param isDynamic specifies that texture is dynamic. [default: false]
	  * @param dwMipMaLevels Number of mipmap levels to generate. Value of 
	  *        0 will generate all the levels till 1x1. [default: 0]
	  * @return A pointer to the new texture or NULL if it could not be created.
	  */
	virtual Texture* CreateTexture( DWORD Width, DWORD Height, RS_FORMAT format, bool isDynamic = false, DWORD dwMipMapLevelNum = 0 ) = 0;
};

#endif