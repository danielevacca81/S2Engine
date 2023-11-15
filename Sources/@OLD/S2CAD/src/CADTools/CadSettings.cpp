// CadSettings.cpp
//
#include "CadSettings.h"

#include "OpenGL/Color.h"

//#include "Geometry/VStyle.h"


// ------------------------------------------------------------------------------------------------
CadSettings::CadSettings()
{
	_pointStyle.setBaseColor( OpenGL::Color::yellow() );
	_pointStyle.setSelectedColor( OpenGL::Color::magenta() );
	_pointStyle.setPenSize( 6 );
	_pointStyle.setBorderSize( 2 );

	_lineStyle.setBaseColor( OpenGL::Color::red() );
	_lineStyle.setSelectedColor( OpenGL::Color::magenta() );
	_lineStyle.setPenSize( 1 );

	_shapeStyle.setBaseColor( OpenGL::Color::blue().lighter(.25).transparent() );
	_shapeStyle.setSelectedColor( OpenGL::Color::magenta() );
	_shapeStyle.setPenSize( 2 );
	//_shapeStyle.setBorderSize( 3 );

	_previewStyle.setBaseColor( OpenGL::Color::white() );
	_previewStyle.setPenSize( 6 );
	_previewStyle.setBorderSize( 2 );
	_previewStyle.setPenStyle( VStyle::DashedPen );

	_snapStyle.setBaseColor( OpenGL::Color::green().lighter() );
	_snapStyle.setPenSize( 2 );
}

// ------------------------------------------------------------------------------------------------
CadSettings::~CadSettings()
{}

// ------------------------------------------------------------------------------------------------
CadSettings CadSettings::defaults() // [static]
{
	return CadSettings();
}

// ------------------------------------------------------------------------------------------------
VStyle CadSettings::pointStyle() const { return _pointStyle; }
VStyle CadSettings::lineStyle()  const { return _lineStyle;  }
VStyle CadSettings::shapeStyle() const { return _shapeStyle; }
VStyle CadSettings::previewStyle() const { return _previewStyle; }
VStyle CadSettings::snapStyle()    const { return _snapStyle; }
//OpenGL::Color CadSettings::defaultObjColor() const	{ return _defaultObjColor; }
//OpenGL::Color CadSettings::selectedColor()   const	{ return _selectedColor;   }
//OpenGL::Color CadSettings::hilightedColor()  const	{ return _hilightedColor;  }
//OpenGL::Color CadSettings::previewColor()    const	{ return _previewColor;    }
//OpenGL::Color CadSettings::snapPointColor()  const	{ return _snapPointColor;  }

// ------------------------------------------------------------------------------------------------
void CadSettings::setPointStyle( const VStyle &style )   { _pointStyle   = style; }
void CadSettings::setLineStyle( const VStyle &style )    { _lineStyle    = style; }
void CadSettings::setShapeStyle( const VStyle &style )   { _shapeStyle   = style; }
void CadSettings::setPreviewStyle( const VStyle &style ) { _previewStyle = style; }
void CadSettings::setSnapStyle( const VStyle &style )    { _snapStyle    = style; }

//void CadSettings::setDefaultObjColor ( const OpenGL::Color &c ) { _defaultObjColor = c; }
//void CadSettings::setSelectedColor   ( const OpenGL::Color &c ) { _selectedColor   = c; }
//void CadSettings::setHilightedColor  ( const OpenGL::Color &c ) { _hilightedColor  = c; }
//void CadSettings::setPreviewColor    ( const OpenGL::Color &c ) { _previewColor    = c; }
//void CadSettings::setSnapPointColor  ( const OpenGL::Color &c ) { _snapPointColor  = c; }