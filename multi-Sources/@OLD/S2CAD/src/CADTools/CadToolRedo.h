// CadToolRedo.h
//
#ifndef CADTOOLREDO_ONCE
#define CADTOOLREDO_ONCE

#include "Framework_Exports.h"


#include "CadTool.h"

namespace OpenGL {
	class Renderer;
}

class FWK_API CadToolRedo : public CadTool
{
public:
	CadToolRedo();
	~CadToolRedo();

	bool canSnap() const		 { return false; }
	bool canHilight() const		 { return false; }
	bool canStayOnScreen() const { return false; }
	bool canChainPoints() const  { return false; }
	bool canOverrideSingleClick() const { return false; }


	void execute();
	void draw( OpenGL::Renderer *r );
};

#endif 