// Painter2D.h
//
#ifndef RENDERER_PAINTER2D_H
#define RENDERER_PAINTER2D_H

#include "Renderer_API.h"

#include "RenderingContext.h"

#include "Math/Math.h"
#include "Core/Color.h"

#include <vector>
#include <string>

namespace RenderCore { class Surface; }

namespace Renderer {

//struct RenderBudgetSpecification
//{
//	uint32_t vertexBudgetSize {};
//	uint32_t textureBudgetSize_byte{};
//};

class RENDERER_API Painter2D
{
public:
	Painter2D();
	~Painter2D();

	uint32_t width()  const;
	uint32_t height() const;

	void resize( int32_t width, int32_t height );
	void blit();


	void clear( /*clearbrush*/);
	void drawPoint   ( const Math::vec2& position, float size, const Color& color );
	void drawLine    ( const Math::vec2& p0, const Math::vec2& p1, const Color& color );
	void drawRect    ( const Math::vec2& bottomLeft, const Math::vec2& topRight, const Color &color );
	void drawCircle  ( const Math::vec2& center, float radius, const Color &color );
	
	void drawPoints  ( const std::vector<Math::vec2>& positions, const std::vector<float> &sizes, const std::vector<Color>& colors );
	void drawPolyLine( const std::vector<Math::vec2>& vertices,  const Color& color );
	void drawPolygon ( const std::vector<Math::vec2>& vertices,  const Color& color );

	void drawText( const std::string &text );


private:
	RenderCore::Surface* _renderSurface { nullptr };
};

}

#endif // !RENDERER_PAINTER2D_H
