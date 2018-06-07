// VStyle.cpp
// 
#include "VStyle.h"

// ------------------------------------------------------------------------------------------------
VStyle::VStyle()
{
	_baseColor     = OpenGL::Color::blue().lighter(.25f);
	_borderColor   = OpenGL::Color::black();
	_selectedColor = OpenGL::Color::magenta();

	_penSize    = 1.0;
	_borderSize = 0.0;

	_drawStyle = Fill;
	_penStyle  = SolidPen;
}

// ------------------------------------------------------------------------------------------------
VStyle::~VStyle()
{}

// ------------------------------------------------------------------------------------------------
void VStyle::setPenSize( float pixelWidth )    { _penSize    = pixelWidth; }
void VStyle::setBorderSize( float pixelWidth ) { _borderSize = pixelWidth; }

// ------------------------------------------------------------------------------------------------
void VStyle::setDrawStyle( const DrawStyle &style ) { _drawStyle = style; }
void VStyle::setPenStyle( const PenStyle &style )   { _penStyle  = style; }

// ------------------------------------------------------------------------------------------------
void VStyle::setBaseColor( const OpenGL::Color &color )      { _baseColor      = color; }
void VStyle::setSelectedColor( const OpenGL::Color &color )  { _selectedColor  = color; }
//void VStyle::setHilightedColor( const OpenGL::Color &color ) { _hilightedColor = color; }
void VStyle::setBorderColor( const OpenGL::Color &color )    { _borderColor    = color; }


// ------------------------------------------------------------------------------------------------
float VStyle::penSize()    const { return _penSize; }
float VStyle::borderSize() const { return _borderSize; }

// ------------------------------------------------------------------------------------------------
VStyle::DrawStyle VStyle::drawStyle() const { return _drawStyle; }
VStyle::PenStyle  VStyle::penStyle() const  { return _penStyle;  }

// ------------------------------------------------------------------------------------------------
OpenGL::Color VStyle::baseColor() const      { return _baseColor; }
OpenGL::Color VStyle::selectedColor() const  { return _selectedColor; }
//OpenGL::Color VStyle::hilightedColor() const { return _hilightedColor; }
OpenGL::Color VStyle::borderColor() const    { return _borderColor; }