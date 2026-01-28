// CadToolPolyLine.h
//
#ifndef CADTOOLPOLYLINE_ONCE
#define CADTOOLPOLYLINE_ONCE

#include "Framework_Exports.h"


#include "CadTool.h"
#include "OpenGL/Color.h"


namespace OpenGL {
	class Renderer;
}

class FWK_API CadToolPolyLine : public CadTool
{
private:
	OpenGL::Color _creationColor;

public:
	CadToolPolyLine();
	~CadToolPolyLine();

	bool canSnap() const		 {return true;}
	bool canHilight() const		 {return false;}
	bool canStayOnScreen() const {return false;}
	bool canChainPoints() const  {return true;}
	bool canOverrideSingleClick() const { return false; }



	void setCreationColor( const OpenGL::Color &c );
	OpenGL::Color creationColor() const;
	
	void reset();
	void execute();
	void draw( OpenGL::Renderer *r );
};

#endif 