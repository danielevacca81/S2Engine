// VStyle.cpp
// 
#include "VStyle.h"

using namespace s2;
using namespace s2::Renderer;

// ------------------------------------------------------------------------------------------------
VStyle::VStyle()
{
	_baseColor     = Color::blue().lighter(.25f);
	_borderColor   = Color::black();
	_selectedColor = Color::magenta();

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
void VStyle::setBaseColor( const Color &color )      { _baseColor      = color; }
void VStyle::setSelectedColor( const Color &color )  { _selectedColor  = color; }
//void VStyle::setHilightedColor( const Color &color ) { _hilightedColor = color; }
void VStyle::setBorderColor( const Color &color )    { _borderColor    = color; }


// ------------------------------------------------------------------------------------------------
float VStyle::penSize()    const { return _penSize; }
float VStyle::borderSize() const { return _borderSize; }

// ------------------------------------------------------------------------------------------------
VStyle::DrawStyle VStyle::drawStyle() const { return _drawStyle; }
VStyle::PenStyle  VStyle::penStyle() const  { return _penStyle;  }

// ------------------------------------------------------------------------------------------------
Color VStyle::baseColor() const      { return _baseColor; }
Color VStyle::selectedColor() const  { return _selectedColor; }
//Color VStyle::hilightedColor() const { return _hilightedColor; }
Color VStyle::borderColor() const    { return _borderColor; }