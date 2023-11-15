// VObjectStyle.h
// 
#ifndef VOBJECTSTYLE_ONCE
#define VOBJECTSTYLE_ONCE

#include "s2Scene_API.h"

#include "Core/Color.h"

namespace Scene {

class S2SCENE_API VObjectStyle
{
public:
	enum DrawStyle { WireFrame, Points, Fill, Outline };
	enum PenStyle  { SolidPen, DashedPen, DottedPen };

public:
	VObjectStyle();
	virtual ~VObjectStyle();

	void setPenSize( float pixelWidth );
	void setBorderSize( float pixelWidth );

	void setDrawStyle( const DrawStyle &style );
	void setPenStyle( const PenStyle &style );
	void setBaseColor( const Color &color );
	void setSelectedColor( const Color &color );
	void setHilightedColor( const Color &color );
	void setBorderColor( const Color &color );

	float penSize() const;
	float borderSize() const;

	DrawStyle drawStyle()      const;
	PenStyle  penStyle()       const;
	Color     baseColor()      const;
	Color     selectedColor()  const;
	Color     hilightedColor() const;
	Color     borderColor()    const;

protected:
	float _penSize;
	float _borderSize;

	DrawStyle _drawStyle;
	PenStyle _penStyle;
	Color    _baseColor;
	Color    _selectedColor;
	Color    _hilightedColor;
	Color    _borderColor;
};


}
#endif