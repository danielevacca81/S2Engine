// RenderState.h
// 
#ifndef RENDERSTATE_ONCE
#define RENDERSTATE_ONCE

#include "Common/Color.h"
#include "Math/Rectangle.h"

namespace OpenGL {

enum PrimitiveType
{
	Points,
	Lines,
	LineStrip,
	LineLoop,
	Triangles,
	TriangleStrip,
	TriangleFan,
	Quads,
	QuadStrip
};


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
struct FaceCulling
{
	enum Face
	{
		Front,
		Back,
		FrontAndBack,
	};

	enum WindingOrder 
	{
		ClockWise, 
		CounterClockWise, 
	};

	FaceCulling() 
	: enabled(true)
	, cullFace(Back)
	, frontFaceWindingOrder(CounterClockWise) 
	{}


	bool         enabled;
	Face         cullFace;
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
/*                            SCISSORTEST                               */
/************************************************************************/
struct ScissorTest
{
	ScissorTest()
	: rect( 0,0,0,0 )
	, enabled(false) 
	{}

	bool            enabled;
	Math::Rectangle rect;
};

/************************************************************************/
/*                              STENCILTEST                             */
/************************************************************************/
struct StencilTestFace
{
	enum StencilOperation
	{
		Zero,
		Invert,
		Keep,
		Replace,
		Increment,
		Decrement,     
		IncrementWrap,
		DecrementWrap,
	};

	enum StencilTestFunction
	{
		Never,
		Less,
		Equal,
		LessThanOrEqual,
		Greater,
		NotEqual,
		GreaterThanOrEqual,
		Always,
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
		Never, 
		Less, 
		Equal, 
		LessThanOrEqual, 
		Greater, 
		NotEqual, 
		GreaterThanOrEqual, 
		Always,
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
		Zero,
		One,
		SourceAlpha,
		OneMinusSourceAlpha,
		SourceColor,
		OneMinusSourceColor,
		DestinationAlpha,
		OneMinusDestinationAlpha,
		DestinationColor,
		OneMinusDestinationColor,
		SourceAlphaSaturate,
		ConstantColor,
		OneMinusConstantColor,
		ConstantAlpha,
		OneMinusConstantAlpha,
	};

	enum BlendEquation
	{
		Add,
		Minimum,
		Maximum,
		Subtract,
		ReverseSubtract,
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
	s2::Color      color;
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
	ColorBuffer,
	DepthBuffer, 
	StencilBuffer,
	ColorAndDepthBuffer,
	All,
};

/************************************************************************/
/*                             RENDERSTATE                              */
/************************************************************************/
struct RenderState
{
	enum RasterizationMode
	{ 
		Point,
		Line, 
		Fill,
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

}
#endif