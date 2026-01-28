// CadSettings.h
//
#ifndef CADSETTINGS_ONCE
#define CADSETTINGS_ONCE

#include "Framework_Exports.h"

#include "VObjects/VStyle.h"

class FWK_API CadSettings
{
private:
	VStyle        _pointStyle;
	VStyle        _lineStyle;
	VStyle        _shapeStyle;
	VStyle        _previewStyle;
	VStyle        _snapStyle;
	//OpenGL::Color _defaultObjColor;
	//OpenGL::Color _selectedColor;
	//OpenGL::Color _hilightedColor;
	//OpenGL::Color _previewColor;
	//OpenGL::Color _snapPointColor;

public:
	CadSettings();
	~CadSettings();

	static CadSettings defaults();
	
	VStyle pointStyle() const;
	VStyle lineStyle()  const;
	VStyle shapeStyle() const;
	VStyle previewStyle() const;
	VStyle snapStyle()    const;

	void setPointStyle( const VStyle &style );
	void setLineStyle( const VStyle &style );
	void setShapeStyle( const VStyle &style );
	void setPreviewStyle( const VStyle &style );
	void setSnapStyle( const VStyle &style );


	//OpenGL::Color defaultObjColor() const;
	//OpenGL::Color selectedColor()   const;
	//OpenGL::Color hilightedColor()  const;
	//OpenGL::Color previewColor()    const;
	//OpenGL::Color snapPointColor()  const;

	//void setDefaultObjColor ( const OpenGL::Color &c );
	//void setSelectedColor   ( const OpenGL::Color &c );
	//void setHilightedColor  ( const OpenGL::Color &c );
	//void setPreviewColor    ( const OpenGL::Color &c );
	//void setSnapPointColor  ( const OpenGL::Color &c );
};

#endif