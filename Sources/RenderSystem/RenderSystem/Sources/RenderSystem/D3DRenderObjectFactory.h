#ifndef _D3DRENDEROBJECTFACTORY_H_INCLUDED_
#define _D3DRENDEROBJECTFACTORY_H_INCLUDED_

#include "RenderObjectFactory.h"
#include <d3d9.h>

/**
  * This is the implementation of the D3D version of render object
  * factory.
  *
  * @author Vincent Prat
  */
class D3DRenderObjectFactory : public RenderObjectFactory
{
//----------------------------------------------------------------------
// Public methods
//--
public:

	/**
	  * Default object constructor
	  */
	D3DRenderObjectFactory( LPDIRECT3DDEVICE9 pD3DDevice );

	/**
	  * Object destructor
	  */
	~D3DRenderObjectFactory();

	//------------------------------------------------------------------
	// Object creation functions (see base class documentation for this)
	//--
	VertexBuffer* CreateVertexBuffer( int count, DWORD format, bool isDynamic = true );
	Texture* CreateTexture( DWORD Width, DWORD Height, RS_FORMAT format, bool isDynamic = false, DWORD dwMipMapLevelNum = 0 );

protected:
	/**
	  * Pointer to the d3d device object
	  */
	LPDIRECT3DDEVICE9 m_pD3DDevice;
};


#endif