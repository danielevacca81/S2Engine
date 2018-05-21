// CadToolRectangle.h
//
#ifndef CADTOOLRECTANGLE_ONCE
#define CADTOOLRECTANGLE_ONCE

#include "Framework_Exports.h"


#include "CadTool.h"
#include "OpenGL/Color.h"


namespace OpenGL {
	class Renderer;
}

class FWK_API CadToolRectangle : public CadTool
{
private:
	OpenGL::Color _creationColor;

	bool _square;

	bool _byRectSize;

public:
	CadToolRectangle();
	~CadToolRectangle();

	bool canSnap() const		 { return true;  }
	bool canHilight() const		 { return false; }
	bool canStayOnScreen() const { return false; }
	bool canChainPoints() const  { return false; }
	bool canOverrideSingleClick() const { return false; }



	void setSquare( bool square ) { _square = square; }
	
	void setByRectSize( bool byRectSize ) { _byRectSize = byRectSize; }
	bool byRectSize() { return _byRectSize; }

	void setCreationColor( const OpenGL::Color &c );
	OpenGL::Color creationColor() const;

	void execute();
	void draw( OpenGL::Renderer *r );
};

#endif 