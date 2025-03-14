// Painter2D.cpp
//
#include "Painter2D.h"

#include "RenderCore/DrawState.h"
#include "RenderCore/Surface.h"


using namespace Renderer;

//// -------------------------------------------------------------------------------------------------
//void Painter2D::init( uint32_t memoryBudgetSize_byte )
//{}
//
//// -------------------------------------------------------------------------------------------------
//void Painter2D::destroy()
//{}

// -------------------------------------------------------------------------------------------------
Painter2D::Painter2D()
	: _renderSurface ( new RenderCore::Surface )
{}

// -------------------------------------------------------------------------------------------------
Painter2D::~Painter2D() { delete _renderSurface; }

// -------------------------------------------------------------------------------------------------
uint32_t Painter2D::width()  const { return _renderSurface->width(); }
uint32_t Painter2D::height() const { return _renderSurface->height(); }

// -------------------------------------------------------------------------------------------------
void Painter2D::blit()
{
	_renderSurface->blit();
}


// -------------------------------------------------------------------------------------------------
void Painter2D::resize( int32_t width, int32_t height )
{
	_renderSurface->resize( width, height );
}

// -------------------------------------------------------------------------------------------------
void Painter2D::clear()
{
	_renderSurface->clear();
	
	auto quad = RenderCore::PrimitiveBuffer::New();
	quad->setVertices( { {-1,1,0},{-1,-1,0},{1,1,0},{1,-1,0} } );
	quad->setColors( { Color::red(),Color::blue(),Color::green(),Color::yellow() } );

	//DrawState ds;// ( Resources::DefaultShaders.Simple );
	_renderSurface->draw( RenderCore::PrimitiveType::TriangleStrip, quad );
}

// -------------------------------------------------------------------------------------------------
void Painter2D::drawPoint( const Math::vec2& position, float size, const Color& color )
{
	RenderCore::RenderState rs
	{
		.programPointSize{ false }, //tbd
	};


	RenderCore::DrawState ds;

	auto point = RenderCore::PrimitiveBuffer::New();
	point->setVertices( { position } );
	point->setColors( { color } );

	_renderSurface->draw( RenderCore::PrimitiveType::Points, point );
}

// -------------------------------------------------------------------------------------------------
void Painter2D::drawLine( const Math::vec2& p0, const Math::vec2& p1, const Color& color )
{}

// -------------------------------------------------------------------------------------------------
void Painter2D::drawRect( const Math::vec2& bottomLeft, const Math::vec2& topRight, const Color& color )
{}

// -------------------------------------------------------------------------------------------------
void Painter2D::drawCircle( const Math::vec2& center, float radius, const Color& color )
{}

// -------------------------------------------------------------------------------------------------
void Painter2D::drawPoints( const std::vector<Math::vec2>& positions, const std::vector<float>& sizes, const std::vector<Color>& colors )
{
	RenderCore::RenderState rs
	{
		.programPointSize{ false }, //tbd
	};


	RenderCore::DrawState ds;

	auto points = RenderCore::PrimitiveBuffer::New();
	points->setVertices( positions );
	points->setColors( colors );

	_renderSurface->draw( RenderCore::PrimitiveType::Points, points );
}

// -------------------------------------------------------------------------------------------------
void Painter2D::drawPolyLine( const std::vector<Math::vec2>& vertices, const Color& color )
{}

// -------------------------------------------------------------------------------------------------
void Painter2D::drawPolygon( const std::vector<Math::vec2>& vertices, const Color& color )
{}

// -------------------------------------------------------------------------------------------------
void Painter2D::drawText( const std::string& text )
{}