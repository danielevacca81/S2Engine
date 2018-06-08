// RenderState.cpp
// 
#include "RenderState.h"

#include "graphics/Color.h"
#include "OpenGL/OpenGL.h"

namespace s2{
namespace OpenGL{

/************************************************************************/
/*                            PRIMITIVERESTART                          */
/************************************************************************/
struct PrimitiveRestart
{
	PrimitiveRestart()
	: enabled(false)
	, index(0) 
	{}

	bool enabled;
	int  index;
};

/************************************************************************/
/*                               FACECULLING                            */
/************************************************************************/
enum Face
{
	Front        = GL_FRONT,
	Back         = GL_BACK,
	FrontAndBack = GL_FRONT_AND_BACK 
};

struct FaceCulling
{
	enum WindingOrder 
	{
		ClockWise        = GL_CW, 
		CounterClockWise = GL_CCW 
	};
	
	FaceCulling() 
	: enabled(true)
	, cullFace(Back)
	, frontFaceWindingOrder(CounterClockWise) 
	{}


	bool         enabled;
	OpenGL::Face cullFace;
	WindingOrder frontFaceWindingOrder;
};

/************************************************************************/
/*                            PROGRAMPOINTSIZE                          */
/************************************************************************/
struct ProgramPointSize
{
	bool enabled;

	ProgramPointSize()
	: enabled(false) 
	{}
};

/************************************************************************/
/*                          RASTERIZATIONMODE                           */
/************************************************************************/
struct Rectangle
{
	int left,bottom;
	int width,height;

	Rectangle( int l, int b, int w, int h ) 
	: left(l), bottom(b), width(w), height(h)
	{}

	Rectangle() 
	: left(0), bottom(0), width(0), height(0)
	{}

	bool equals( const Rectangle &other ) const 
	{ 
		return left   == other.left && 
			   bottom == other.bottom &&
			   width  == other.width &&
			   height == other.height;
	}
};

/************************************************************************/
/*                            SCISSORTEST                               */
/************************************************************************/
struct ScissorTest
{
	ScissorTest()
	: rect( 0,0,0,0 )
	, enabled(false) 
	{}

	bool      enabled;
	Rectangle rect;
};

/************************************************************************/
/*                              STENCILTEST                             */
/************************************************************************/
struct StencilTestFace
{
	enum StencilOperation
	{
		Zero          = GL_ZERO,
		Invert        = GL_INVERT,
		Keep          = GL_KEEP,
		Replace       = GL_REPLACE,
		Increment     = GL_INCR,
		Decrement     = GL_DECR,
		IncrementWrap = GL_INCR_WRAP,
		DecrementWrap = GL_DECR_WRAP
	};

	enum StencilTestFunction
	{
		Never              = GL_NEVER,
		Less               = GL_LESS,
		Equal              = GL_EQUAL,
		LessThanOrEqual    = GL_LEQUAL,
		Greater            = GL_GREATER,
		NotEqual           = GL_NOTEQUAL,
		GreaterThanOrEqual = GL_GEQUAL,
		Always             = GL_ALWAYS,
	};

	StencilTestFace()
	: stencilFailOperation(Keep)
	, depthFailStencilPassOperation(Keep)
	, depthPassStencilPassOperation(Keep)
	, function(Always)
	, referenceValue(0)
	, mask(~0)
	{}

	StencilOperation    stencilFailOperation;
	StencilOperation    depthFailStencilPassOperation;
	StencilOperation    depthPassStencilPassOperation;
	StencilTestFunction function;
	int                 referenceValue;		
	int                 mask;
};


struct StencilTest
{
	StencilTest()
	: enabled(false)
	{}

	bool            enabled;
	StencilTestFace frontFace;
	StencilTestFace backFace;
};

/************************************************************************/
/*                               DEPTHTEST                              */
/************************************************************************/
struct DepthTest
{
	enum DepthTestFunction
	{
		Never              = GL_NEVER, 
		Less               = GL_LESS, 
		Equal              = GL_EQUAL, 
		LessThanOrEqual    = GL_LEQUAL, 
		Greater            = GL_GREATER, 
		NotEqual           = GL_NOTEQUAL, 
		GreaterThanOrEqual = GL_GEQUAL, 
		Always             = GL_ALWAYS
	};

	DepthTest() 
	: enabled(true)
	, function(Less) 
	{}

	bool              enabled;
	DepthTestFunction function;
};

/************************************************************************/
/*                             DEPTHRANGE                               */
/************************************************************************/
struct DepthRange
{
	DepthRange()
	: nearValue(0.0)
	, farValue(1.0) 
	{}

	double nearValue;
	double farValue;
};

/************************************************************************/
/*                                BLENDING                              */
/************************************************************************/
struct Blending
{
	enum BlendingFactor
	{ 
		Zero                     = GL_ZERO,
		One                      = GL_ONE,
		SourceAlpha              = GL_SRC_ALPHA,
		OneMinusSourceAlpha      = GL_ONE_MINUS_SRC_ALPHA,
		SourceColor              = GL_SRC_COLOR,
		OneMinusSourceColor      = GL_ONE_MINUS_SRC_COLOR,
		DestinationAlpha         = GL_DST_ALPHA,
		OneMinusDestinationAlpha = GL_ONE_MINUS_DST_ALPHA,
		DestinationColor         = GL_DST_COLOR,
		OneMinusDestinationColor = GL_ONE_MINUS_DST_COLOR,
		SourceAlphaSaturate      = GL_SRC_ALPHA_SATURATE,
		ConstantColor            = GL_CONSTANT_COLOR,
		OneMinusConstantColor    = GL_ONE_MINUS_CONSTANT_COLOR,
		ConstantAlpha            = GL_CONSTANT_ALPHA,
		OneMinusConstantAlpha    = GL_ONE_MINUS_CONSTANT_ALPHA
	};

	enum BlendEquation
	{
		Add             = GL_FUNC_ADD,
		Minimum         = GL_MIN,
		Maximum         = GL_MAX,
		Subtract        = GL_FUNC_SUBTRACT,
		ReverseSubtract = GL_FUNC_REVERSE_SUBTRACT
	};


	Blending() 
	: enabled(false)
	, sourceRGBFactor(One)
	, sourceAlphaFactor(One)
	, destinationRGBFactor(Zero)
	, destinationAlphaFactor(Zero)
	, rgbEquation(Add)
	, alphaEquation(Add)
	, color(0.f,0,0,0)
	{}

	bool           enabled;
	BlendingFactor sourceRGBFactor;
	BlendingFactor sourceAlphaFactor;
	BlendingFactor destinationRGBFactor;
	BlendingFactor destinationAlphaFactor;
	BlendEquation  rgbEquation;
	BlendEquation  alphaEquation;
	Color          color;
};


/************************************************************************/
/*                              COLORMASK                               */
/************************************************************************/
struct ColorMask
{
	ColorMask( bool redBit, bool greenBit, bool blueBit, bool alphaBit )
	: r(redBit)
	, g(greenBit)
	, b(blueBit)
	, a(alphaBit)
	{}

	bool r;
	bool g;
	bool b;
	bool a;

	bool equals( const ColorMask &other )
	{
		return 
			r == other.r &&
			g == other.g &&
			b == other.b &&
			a == other.a;
	}
};

/************************************************************************/
/*                            CLEARBUFFERS                              */
/************************************************************************/
enum ClearBuffers
{ 
	ColorBuffer         = GL_COLOR_BUFFER_BIT/*1*/,
	DepthBuffer         = GL_DEPTH_BUFFER_BIT/*2*/, 
	StencilBuffer       = GL_STENCIL_BUFFER_BIT/*4*/,
	ColorAndDepthBuffer = ColorBuffer | DepthBuffer,
	All                 = ColorBuffer | DepthBuffer | StencilBuffer
};

/************************************************************************/
/*                             RENDERSTATE                              */
/************************************************************************/
struct RenderState
{
	enum RasterizationMode
	{ 
		Point = GL_POINT,
		Line  = GL_LINE, 
		Fill  = GL_FILL
	};

	RenderState() 
	: primitiveRestart()
	, faceCulling()
	, programPointSize()
	, rasterizationMode(Fill)
	, scissorTest()
	, stencilTest()
	, depthTest()
	, depthRange()
	, blending()
	, colorMask(true,true,true,true)
	, depthMask(true)
	{}

	PrimitiveRestart  primitiveRestart;
	FaceCulling       faceCulling;
	ProgramPointSize  programPointSize;
	RasterizationMode rasterizationMode;
	ScissorTest       scissorTest;
	StencilTest       stencilTest;
	DepthTest         depthTest;
	DepthRange        depthRange;
	Blending          blending;
	ColorMask         colorMask;
	bool              depthMask;
};

}}