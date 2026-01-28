// CadToolUndo.h
//
#ifndef CADTOOLUNDO_ONCE
#define CADTOOLUNDO_ONCE

#include "Framework_Exports.h"


#include "CadTool.h"

namespace OpenGL {
	class Renderer;
}

class FWK_API CadToolUndo : public CadTool
{
public:
	CadToolUndo();
	~CadToolUndo();

	bool canSnap() const		        { return false; }
	bool canHilight() const		        { return false; }
	bool canStayOnScreen() const        { return false; }
	bool canChainPoints() const         { return false; }
	bool canOverrideSingleClick() const { return false; }


	void execute();
	void draw( OpenGL::Renderer *r );
};

#endif 