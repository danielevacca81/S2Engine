// CadToolCircle.h
//
#ifndef CADTOOLCIRCLE_ONCE
#define CADTOOLCIRCLE_ONCE

#include "Framework_Exports.h"

#include "CadTool.h"
#include "OpenGL/Color.h"

namespace OpenGL {
	class Renderer;
}

class FWK_API CadToolCircle : public CadTool
{
private:
	OpenGL::Color _creationColor;
	bool          _useRadius;

public:
	CadToolCircle();
	~CadToolCircle();

	bool canSnap() const		 { return true;  }
	bool canHilight() const		 { return false; }
	bool canStayOnScreen() const { return false; }
	bool canChainPoints() const  { return false; }
	bool canOverrideSingleClick() const { return false; }


	void setCreationColor( const OpenGL::Color &c );
	OpenGL::Color creationColor() const;

	void setUseRadius(bool useRadius) { _useRadius= useRadius; }
	bool useRadius() const { return _useRadius; }

	void execute();
	void draw( OpenGL::Renderer *r );
};

#endif 