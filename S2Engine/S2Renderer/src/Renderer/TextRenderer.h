// TextRenderer.h
//
#ifndef TEXTRENDERER_ONCE
#define TEXTRENDERER_ONCE

#include "S2RendererAPI.h"

#include "Common/Color.h"

#include "Renderer/Renderer.h"

#include "Math/Math.h"
#include "Math/Rectangle.h"

#include <list>
#include <string>

//namespace OpenGL {
//	class VertexBuffer;
//}

namespace s2 {


class S2RENDERER_API TextRenderer
{
public:
	enum TextAlignment { AlignLeft=0, AlignCenter, AlignRight };

	struct TextTag
	{
		std::wstring    _text;
		Math::ivec2     _screenPos;
		Math::Rectangle _box;
		Color		    _color;
		std::string     _fontFace;
		double          _angle;
		double          _size;
		bool            _visible;
		int             _alignment;

		TextTag( const Math::ivec2 &screenPos, 
		         const std::wstring &text, 
				 const Color &color, 
				 const std::string &fontFace, 
				 double angle, 
				 int alignment )
		: _text(text)
		, _screenPos(screenPos)
		, _color(color)
		, _angle(angle)
		, _fontFace(fontFace)
		, _alignment(alignment)
		, _visible(false) 
		{}
	};


private:
	std::list<TextTag>	_tags;
	Math::Rectangle     _area;
	OpenGL::ProgramPtr  _shader;
	OpenGL::RenderState _renderState;
	OpenGL::DrawState   _drawState;
	OpenGL::ViewState   _viewState;

	void update();
	OpenGL::VertexBuffer buildVertexBuffer( const TextTag &tag );

public:
	TextRenderer();
	~TextRenderer();

	bool init();

	void addTag( const TextTag &tag );
	void draw( OpenGL::Renderer *r );

	Math::Rectangle area() const { return _area; }

	bool isEmpty() const;
};

}



#endif