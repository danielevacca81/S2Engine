// VObjectStyle.cpp
// 
#include "VObjectStyle.h"


using namespace Scene;

// ------------------------------------------------------------------------------------------------
VObjectStyle::VObjectStyle()
{
	_baseColor      = Color::blue().lighter(.25f);
	_hilightedColor = _baseColor.lighter(.25f);
	_borderColor    = Color::black();
	_selectedColor  = Color::magenta();

	_penSize    = 1.0;
	_borderSize = 0.0;

	_drawStyle = Fill;
	_penStyle  = SolidPen;
}

// ------------------------------------------------------------------------------------------------
VObjectStyle::~VObjectStyle()
{}

// ------------------------------------------------------------------------------------------------
void VObjectStyle::setPenSize( float pixelWidth )    { _penSize    = pixelWidth; }
void VObjectStyle::setBorderSize( float pixelWidth ) { _borderSize = pixelWidth; }

// ------------------------------------------------------------------------------------------------
void VObjectStyle::setDrawStyle( const DrawStyle &style ) { _drawStyle = style; }
void VObjectStyle::setPenStyle( const PenStyle &style )   { _penStyle  = style; }

// ------------------------------------------------------------------------------------------------
void VObjectStyle::setBaseColor( const Color &color )      { _baseColor      = color; }
void VObjectStyle::setSelectedColor( const Color &color )  { _selectedColor  = color; }
void VObjectStyle::setHilightedColor( const Color &color ) { _hilightedColor = color; }
void VObjectStyle::setBorderColor( const Color &color )    { _borderColor    = color; }


// ------------------------------------------------------------------------------------------------
float VObjectStyle::penSize()    const { return _penSize; }
float VObjectStyle::borderSize() const { return _borderSize; }

// ------------------------------------------------------------------------------------------------
VObjectStyle::DrawStyle VObjectStyle::drawStyle() const { return _drawStyle; }
VObjectStyle::PenStyle  VObjectStyle::penStyle() const  { return _penStyle;  }

// ------------------------------------------------------------------------------------------------
Color VObjectStyle::baseColor() const      { return _baseColor; }
Color VObjectStyle::selectedColor() const  { return _selectedColor; }
Color VObjectStyle::hilightedColor() const { return _hilightedColor; }
Color VObjectStyle::borderColor() const    { return _borderColor; }