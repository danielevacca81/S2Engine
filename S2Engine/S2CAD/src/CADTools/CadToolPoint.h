// CadToolPoint.h
//
#ifndef CADTOOLPOINT_ONCE
#define CADTOOLPOINT_ONCE

#include "Framework_Exports.h"


#include "CadTool.h"
#include "OpenGL/Color.h"

namespace OpenGL {
	class Renderer; 
}

class FWK_API CadToolPoint : public CadTool
{
private:
	OpenGL::Color _creationColor;

public:
	CadToolPoint();
	~CadToolPoint();

	bool canSnap() const		 { return true;  }
	bool canHilight() const		 { return false; }
	bool canStayOnScreen() const { return false; } 
	bool canChainPoints() const  { return false; }
	bool canOverrideSingleClick() const { return false; }


	void setCreationColor( const OpenGL::Color &c );
	OpenGL::Color creationColor() const;

	void execute();
	void draw( OpenGL::Renderer *r );
};

#endif