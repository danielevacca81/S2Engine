// CadToolSelect.h
//
#ifndef CADTOOLSELECT_ONCE
#define CADTOOLSELECT_ONCE

#include "Framework_Exports.h"


#include "CadTool.h"

namespace OpenGL {
	class Renderer;
}

class FWK_API CadToolSelect : public CadTool
{
public:
	CadToolSelect();
	~CadToolSelect();

	bool canSnap() const		 { return false; }
	bool canHilight() const		 { return true;  }
	bool canStayOnScreen() const { return false; }
	bool canChainPoints() const  { return false; }
	bool canOverrideSingleClick() const { return true; }

	int selectionThreshold() const { return 0; }
	int collapseThreshold()  const { return 0; }
	int discardThreshold()   const { return 0; }

	void execute();
	void draw( OpenGL::Renderer *r );
};

#endif 