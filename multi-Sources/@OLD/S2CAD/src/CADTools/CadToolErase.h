// CadToolErase.h
//
#ifndef CADTOOLERASE_ONCE
#define CADTOOLERASE_ONCE

#include "Framework_Exports.h"


#include "CadTool.h"

namespace OpenGL {
	class Renderer; 
}

class FWK_API CadToolErase : public CadTool
{
private:
public:
	CadToolErase();
	~CadToolErase();

	bool canSnap() const		 { return false; }
	bool canHilight() const		 { return false; }
	bool canStayOnScreen() const { return false; }
	bool canChainPoints() const  { return false; }
	bool canOverrideSingleClick() const { return false; }


	void execute();
	void draw( OpenGL::Renderer *r );
};

#endif