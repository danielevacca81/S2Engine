#ifndef _RENDERTYPEDEFS_H_INCLUDED_
#define _RENDERTYPEDEFS_H_INCLUDED_

#include <windows.h>
#include <windef.h>

/**
  * Base Api used to render primitives
  */
typedef enum _BASE_API {
	RSAPI_DIRECT3D,   /** Direct3D is used as the base API */
	RSAPI_OPENGL,		/** OpenGL is used as the base API */
} RS_BASEAPI;


/**
  * Formats to use for surfaces
  */
typedef enum _PIXELFORMAT {
	RSFMT_A8R8G8B8,	/** 32 bits per pixel (ARGB) */
} RS_FORMAT;


/**
  * Current matrix mode in the render device (indicate which
  * matrix is modified by matrix function calls).
  */
typedef enum _MATRIX_MODE {
	RSMM_PROJECTION = 0, /** Projection matrix mode */
	RSMM_MODELVIEW  = 1, /** Model-view matrix mode */
} RS_MATRIXMODE;


/**
  * Fill mode used to render primitives on screen
  */
typedef enum _FILLMODE {
	RSFILL_WIREFRAME = 0x0000, /** Render primitives using wireframe mode */
	RSFILL_SOLID     = 0x0001, /** Render filled primitives */
} RS_FILLMODE;


/**
  * List of the render states that can be set on a device.
  */
typedef enum _RENDERSTATETYPE {
	RSRS_FILLMODE,		 /** Set to one of the _FILLMODE values */
	RSRS_ZENABLE,		 /** Enable z-buffer testing. Set to 1 (True) or 0 (False) */
	RSRS_CULLINGENABLE,  /** Enable CCW backface culling. Set to 1 (True) or 0 (False) */
	RSRS_LIGHTINGENABLE, /** Enable lighting. Set to 1 (True) or 0 (False) */
} RS_RENDERSTATETYPE;


/**
  * Flags used to describe the format of a vertex in a vertex buffer
  */
typedef enum __VERTEXFORMATFLAGS
{
	RSVF_XYZ           = 0x0001, /** Untransformed XYZ value set. */
    RSVF_XYZRHW        = 0x0002, /** transformed XYZRHW value set. */
    RSVF_NORMAL        = 0x0004, /** XYZ normal value set */
    RSVF_DIFFUSE       = 0x0008, /** RGBA diffuse color value set */
    RSVF_TEXCOORD      = 0x0010, /** Texture vertex flag. */
} RS_VERTEXFORMATFLAGS;


/**
  * Supported primitive types
  */
typedef enum __PRIMITIVETYPE
{
    RSPT_POINTLIST,     /** Specifies a point list  */
    RSPT_LINELIST,      /** Specifies a line list   */
    RSPT_LINESTRIP,     /** Specifies a line strip  */
    RSPT_TRIANGLELIST,  /** Specifies a triangle list  */
    RSPT_TRIANGLESTRIP, /** Specifies a triangle strip */
    RSPT_TRIANGLEFAN,   /** Specifies a triangle fan   */
} RS_PRIMITIVETYPE;

/**
  * Valid clear flags
  */
typedef enum _CLEAR_FLAG
{
	RSCLR_COLOR   = 0x0001, /** Clear color buffer */
	RSCLR_ZBUFFER = 0x0002, /** Clear z-buffer */
	RSCLR_STENCIL = 0x0004, /** Clear stencil buffer */
} RS_CLEAR_FLAG;

/**
  * A handy macro to release a pointer
  */
#ifndef SAFE_RELEASE
#define SAFE_RELEASE( ptr ) { if (ptr!=NULL) { ptr->Release(); ptr = NULL; } }
#endif

/**
  * A handy macro to delete a pointer
  */
#ifndef SAFE_DELETE
#define SAFE_DELETE( ptr ) { if (ptr!=NULL) { delete ptr; ptr = NULL; } }
#endif

/**
  * A handy macro to delete a pointer to an array
  */
#ifndef SAFE_DELETEARRAY
#define SAFE_DELETEARRAY( ptr ) { if (ptr!=NULL) { delete [] ptr; ptr = NULL; } }
#endif


#endif