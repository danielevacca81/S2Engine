// RenderState.h
// 
#ifndef RENDERSTATE_ONCE
#define RENDERSTATE_ONCE

#include "Core/Color.h"
#include "Core/Rectangle.h"

namespace Renderer {

/************************************************************************/
/*                            PRIMITIVERESTART                          */
/************************************************************************/
struct PrimitiveRestart
{
	PrimitiveRestart()
	: enabled( false )
	, index( 0xffff )
	{}

	bool enabled;
	int  index;
};

/************************************************************************/
/*                               FACECULLING                            */
/************************************************************************/
struct FaceCulling
{
	enum class Face
	{
		Front,
		Back,
		FrontAndBack,
	};

	enum class WindingOrder
	{
		ClockWise,
		CounterClockWise,
	};

	FaceCulling()
	: enabled( true )
	, cullFace( Face::Back )
	, frontFaceWindingOrder( WindingOrder::CounterClockWise )
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
	: enabled( false )
	{}
};

/************************************************************************/
/*                            SCISSORTEST                               */
/************************************************************************/
struct ScissorTest
{
	ScissorTest()
	: rect( 0, 0, 0, 0 )
	, enabled( false )
	{}

	bool            enabled;
	Math::Rectangle rect;
};

/************************************************************************/
/*                             VIEWPORT                                 */
/************************************************************************/
struct Viewport
{
	Viewport()
	: Viewport( 0,0,0,0 )
	{}

	Viewport( int x, int y, int width, int height )
	: rect( x, y, width, height )
	{}

	Math::Rectangle rect;
};

/************************************************************************/
/*                              STENCILTEST                             */
/************************************************************************/
struct StencilTestFace
{
	enum class StencilOperation
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

	enum class StencilTestFunction
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
	: stencilFailOperation( StencilOperation::Keep )
	, depthFailStencilPassOperation( StencilOperation::Keep )
	, depthPassStencilPassOperation( StencilOperation::Keep )
	, function( StencilTestFunction::Always )
	, referenceValue( 0 )
	, mask( ~0 )
	{}

	StencilOperation    stencilFailOperation;
	StencilOperation    depthFailStencilPassOperation;
	StencilOperation    depthPassStencilPassOperation;
	StencilTestFunction function;
	int                 referenceValue;
	int                 mask;	// read/comparison mask
};


struct StencilTest
{
	StencilTest()
	: enabled( false )
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
	enum class DepthTestFunction
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
	: enabled( true )
	, function( DepthTestFunction::Less )
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
	: nearValue( 0.0 )
	, farValue( 1.0 )
	{}

	double nearValue;
	double farValue;
};

/************************************************************************/
/*                                BLENDING                              */
/************************************************************************/
struct Blending
{
	enum class Factor
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

	enum class Equation
	{
		Add,
		Minimum,
		Maximum,
		Subtract,
		ReverseSubtract,
	};


	Blending()
	: enabled( false )
	, sourceRGBFactor( Factor::One )
	, sourceAlphaFactor( Factor::One )
	, destinationRGBFactor( Factor::Zero )
	, destinationAlphaFactor( Factor::Zero )
	, rgbEquation( Equation::Add )
	, alphaEquation( Equation::Add )
	, color( 0.f, 0, 0, 0 )
	{}

	bool     enabled;
	Factor   sourceRGBFactor;
	Factor   sourceAlphaFactor;
	Factor   destinationRGBFactor;
	Factor   destinationAlphaFactor;
	Equation rgbEquation;
	Equation alphaEquation;
	Color    color;
};


/************************************************************************/
/*                              COLORMASK                               */
/************************************************************************/
struct ColorMask
{
	ColorMask( bool redBit, bool greenBit, bool blueBit, bool alphaBit )
	: r( redBit )
	, g( greenBit )
	, b( blueBit )
	, a( alphaBit )
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
/*                              STENCILMASK                             */
/************************************************************************/
struct StencilMask
{
	StencilMask( const unsigned fornMask, const unsigned backMask )
	: front( fornMask )
	, back( backMask )
	{}

	unsigned front;
	unsigned back;

	bool equals( const StencilMask &other )
	{
		return 
			front == other.front &&
			back == other.back;
	}
};

/************************************************************************/
/*                            CLEARBUFFERS                              */
/************************************************************************/
enum class ClearBuffers
{
	ColorBuffer             = 1,
	DepthBuffer             = 2,
	StencilBuffer           = 4,
	ColorAndDepthBuffer     = ColorBuffer | DepthBuffer,
	ColorAndStencilBuffer   = ColorBuffer | StencilBuffer,
	StencilAndDepthBuffer   = StencilBuffer | DepthBuffer,
	All = ColorBuffer | DepthBuffer | StencilBuffer,
};

/************************************************************************/
/*                             RENDERSTATE                              */
/************************************************************************/
struct RenderState
{
	enum class RasterizationMode
	{
		Point,
		Line,
		Fill,
	};

	RenderState()
	: primitiveRestart()
	, faceCulling()
	, programPointSize()
	, rasterizationMode( RasterizationMode::Fill )
	, lineWidth( 1.0f )
	, scissorTest()
	, stencilTest()
	, depthTest()
	, depthRange()
	, blending()
	, colorMask( true, true, true, true )
	, depthMask( true )
	, stencilMask( ~0u, ~0u )
	{}

	Viewport          viewport;
	PrimitiveRestart  primitiveRestart;
	FaceCulling       faceCulling;
	ProgramPointSize  programPointSize;
	RasterizationMode rasterizationMode;
	float             lineWidth;
	ScissorTest       scissorTest;
	StencilTest       stencilTest;
	DepthTest         depthTest;
	DepthRange        depthRange;
	Blending          blending;
	ColorMask         colorMask;
	bool              depthMask;
	StencilMask       stencilMask;
};

}
#endif