// VStyle.h
// 
#ifndef VSTYLE_ONCE
#define VSTYLE_ONCE

#include "CoreAPI.h"

#include "OpenGL/Color.h"

class S2CORE_API VStyle
{
public:
	enum DrawStyle { WireFrame, Points, Fill, Outline };
	enum PenStyle  { SolidPen, DashedPen, DottedPen };

protected:
	float _penSize;
	float _borderSize;

	DrawStyle     _drawStyle;
	PenStyle      _penStyle;
	OpenGL::Color _baseColor;
	OpenGL::Color _selectedColor;
	//OpenGL::Color _hilightedColor;
	OpenGL::Color _borderColor;

public:
	VStyle();
	virtual ~VStyle();

	void setPenSize( float pixelWidth );
	void setBorderSize( float pixelWidth );

	void setDrawStyle( const DrawStyle &style );
	void setPenStyle( const PenStyle &style );
	void setBaseColor( const OpenGL::Color &color );
	void setSelectedColor( const OpenGL::Color &color );
	//void setHilightedColor( const OpenGL::Color &color );
	void setBorderColor( const OpenGL::Color &color );
	
	float penSize() const;
	float borderSize() const;

	DrawStyle     drawStyle() const;
	PenStyle      penStyle() const;
	OpenGL::Color baseColor() const;
	OpenGL::Color selectedColor() const;
	//OpenGL::Color hilightedColor() const;
	OpenGL::Color borderColor() const;
};


#endif