// RenderState.h
// 
#ifndef RENDERSTATE_ONCE
#define RENDERSTATE_ONCE

#include "Core/Color.h"
#include "Math/Rectangle.h"

#include <array>

namespace RenderSystem {
	
static constexpr int kMaxRenderTargets = 2;
	
/************************************************************************/
/*                            PRIMITIVERESTART                          */
/************************************************************************/
struct PrimitiveRestart
{
	bool enabled { false };
	int  index { 0xffff };
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

	bool         enabled { true };
	Face         cullFace { Face::Back };
	WindingOrder frontFaceWindingOrder { WindingOrder::CounterClockWise };
};

/************************************************************************/
/*                            PROGRAMPOINTSIZE                          */
/************************************************************************/
struct ProgramPointSize
{
	bool enabled { false };
};

/************************************************************************/
/*                                DEPTHMASK                             */
/* enable or disable writing into the depth buffer                      */
/************************************************************************/
struct DepthMask
{
	bool enabled { true };
};

/************************************************************************/
/*                            SCISSORTEST                               */
/************************************************************************/
struct ScissorTest
{
	bool        enabled { false };
	Math::irect rect {0,0,0,0};
};

///************************************************************************/
///*                             VIEWPORT                                 */
///************************************************************************/
//struct Viewport
//{
//	Viewport()
//	: Viewport( 0,0,0,0 )
//	{}
//
//	Viewport( int x, int y, int width, int height )
//	: rect( x, y, width, height )
//	{}
//
//	Math::irect rect { 0,0,0,0 };
//};

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

	StencilOperation    stencilFailOperation { StencilOperation::Keep };
	StencilOperation    depthFailStencilPassOperation { StencilOperation::Keep };
	StencilOperation    depthPassStencilPassOperation { StencilOperation::Keep };
	StencilTestFunction function { StencilTestFunction::Always };
	int                 referenceValue {0};
	int                 mask {~0};	// read/comparison mask
};


struct StencilTest
{
	bool            enabled { false };    // todo: scissor test can be enabled/disabled individually for each viewport. 
								          // other notes:
								          // see https://www.khronos.org/opengl/wiki/Scissor_Test
								          // the maximum number of simultaneous viewports that are supported is implementation dependent and can be queried by calling glGet with the argument GL_MAX_VIEWPORTS. The value must be at least 16
								          // (Ivan: is ARB_viewport_array extension needed? )
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

	bool              enabled { true };
	DepthTestFunction function { DepthTestFunction::Less };
};

/************************************************************************/
/*                             DEPTHRANGE                               */
/************************************************************************/
struct DepthRange
{
	double nearValue { 0.0 };
	double farValue  { 1.0 };
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
	: enabled( {false} ) // initialize enabled[0] to false and the following ones to false as well
	, sourceRGBFactor( Factor::One )
	, sourceAlphaFactor( Factor::One )
	, destinationRGBFactor( Factor::Zero )
	, destinationAlphaFactor( Factor::Zero )
	, rgbEquation( Equation::Add )
	, alphaEquation( Equation::Add )
	, color( 0.f, 0, 0, 0 )
	{}

	void enableAll( const bool val ) { enabled.fill( val ); }

	std::array<bool, kMaxRenderTargets> enabled { {false} }; 	// blendig can be enabled/disabled individually for each render target (multiple render targets can be written by a fragment shader).
	 												            // Currently other blend parameters cannot be specified individually and are shared among all render target writes.
	 												            // Other notes:
	 												            // The maximum number of draw buffers supported is implementation dependent and can be queried by calling glGet with the argument GL_MAX_DRAW_BUFFERS. The value must be at least 8
	 												            // The ability to specify different blend parameters for different buffers relies on the OpenGL ARB_draw_buffers_blend extension
	 												            // and it is not one of the requirements which the s2Renderer library is based on.
	 												            // See https://www.khronos.org/opengl/wiki/Blending for further dettails
	Factor   sourceRGBFactor        { Factor::One };
	Factor   sourceAlphaFactor      { Factor::One };
	Factor   destinationRGBFactor   { Factor::Zero };
	Factor   destinationAlphaFactor { Factor::Zero};
	Equation rgbEquation            { Equation::Add };
	Equation alphaEquation          { Equation::Add };
	Color    color                  { 0.f,0.f, 0.f, 0.f };
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

	bool r{true};
	bool g{true};
	bool b{true};
	bool a{true};

	bool equals( const ColorMask &other ) const
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
	StencilMask( unsigned int frontMask, unsigned int backMask )
	: front( frontMask )
	, back( backMask )
	{}

	unsigned int front { ~0u };
	unsigned int back { ~0u };

	bool equals( const StencilMask &other ) const
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
	ColorBuffer           = 1,
	DepthBuffer           = 2,
	StencilBuffer         = 4,
	ColorAndDepthBuffer   = ColorBuffer | DepthBuffer,
	ColorAndStencilBuffer = ColorBuffer | StencilBuffer,
	StencilAndDepthBuffer = StencilBuffer | DepthBuffer,
	All                   = ColorBuffer | DepthBuffer | StencilBuffer,
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

	PrimitiveRestart  primitiveRestart;
	FaceCulling       faceCulling;
	ProgramPointSize  programPointSize;
	RasterizationMode rasterizationMode { RasterizationMode::Fill };
	float             lineWidth {1.0};
	ScissorTest       scissorTest;
	StencilTest       stencilTest;
	DepthTest         depthTest;
	DepthRange        depthRange;
	Blending          blending;
	ColorMask         colorMask {true, true, true, true};
	DepthMask         depthMask;
	StencilMask       stencilMask { ~0u,~0u };
};

}
#endif