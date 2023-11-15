#ifndef _TEXTURE_H_INCLUDED_
#define _TEXTURE_H_INCLUDED_

#include "RenderTypeDefs.h"

/**
 * This is an abstract class that provides an interface on top of various
 * implementations of textures.
 *
 */
class Texture
{
public:
    virtual ~Texture() {}
    
	/**
      * Locks the texture (usually for writing)...      
      * @return True if the lock request succeeded. False else.
      */
    virtual bool Lock() = 0;

    /**
      * Unlocks the texture
      * @return True if the unlock request succeeded. False else.
      */
    virtual bool Unlock() = 0;

    /**
      * Determines if the texture is locked or not.      
      * @return boolean value specifying if texture is locked or not.
      */
    virtual const bool IsLocked() const = 0;

    /**
      * Returns a pointer to the raw texture data.      
      * This should only be called when texture is locked.      
      * @return pointer to raw vertex data.
      */
    virtual void* GetData() = 0;
    
    /**
      * Get width of the texture
      */
    virtual const DWORD GetWidth() const = 0;

    /**
      * Get height of the texture
      */
    virtual const DWORD GetHeight() const = 0;
    
    /**
      * Get pitch of the texture (length of a row of pixels)
	  * This might differ from the width of the texture. For instance, a
	  * non-power of two texture might have a width of 120 and a pitch of
	  * 128.
      */
    virtual const DWORD GetPitch() const = 0;

	/**
      * returns the format of the texture. (see RS_PIXELFORMAT type)
      * @return Pixel format of the texture. 
      */
    virtual const RS_FORMAT GetFormat() const = 0;

	/**
	  * This method is called to activate the texture before rendering
	  * textured primitives.
	  */
	virtual bool Use() = 0;

	/**
	  * Use this to set current pixel in the texture (helps to solve
	  * Api dependant ordering of rgba components)
	  */
	virtual void SetPixel4f( float a, float r, float g, float b ) = 0;

	/**
	  * Use this to set current pixel in the texture (helps to solve
	  * Api dependant ordering of rgba components)
	  */
	virtual void SetPixel4uc( unsigned char a, unsigned char r, unsigned char g, unsigned char b ) = 0;
};

#endif