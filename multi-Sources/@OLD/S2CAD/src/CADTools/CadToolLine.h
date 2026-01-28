// CadToolLine.h
//
#ifndef CADTOOLLINE_ONCE
#define CADTOOLLINE_ONCE

#include "Framework_Exports.h"

#include "CadTool.h"
#include "OpenGL/Color.h"


namespace OpenGL {
	class Renderer;
}

class FWK_API CadToolLine : public CadTool
{
private:
	OpenGL::Color _creationColor;
	bool _useLen;
	double _length;
	double _angle;

public:
	CadToolLine();
	~CadToolLine();

	bool canSnap() const		 { return true;  }
	bool canHilight() const		 { return false; }
	bool canStayOnScreen() const { return false; }
	bool canChainPoints() const  { return false; }
	bool canOverrideSingleClick() const { return false; }



	void setUseLen( bool useLen ) { this->_useLen= useLen; }
	bool useLen() { return this->_useLen; }

	void setLength( double length ) { this->_length= length; }
	double length() { return this->_length; }

	void setAngle( double angle ) { this->_angle= angle; }
	double angle() { return this->_angle; }

	void setCreationColor( const OpenGL::Color &c );
	OpenGL::Color creationColor() const;
	
	void execute();
	void draw( OpenGL::Renderer *r );

	virtual void validate();
};

#endif 