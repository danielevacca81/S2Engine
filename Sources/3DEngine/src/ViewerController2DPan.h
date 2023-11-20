// ViewerController2DPan.h
//
#ifndef VIEWERCONTROLLER2DPAN_ONCE
#define VIEWERCONTROLLER2DPAN_ONCE

#include "s2Scene_API.h"

#include "ViewerController.h"

namespace Scene {

// ------------------------------------------------------------------------------------------------
class S2SCENE_API ViewerController2DPan : public ViewerController2D
{
public:
	bool    start(); 

	bool    doDragBegin ( const Math::ivec2 &screenPos )  override;
	bool    doDragUpdate( const Math::ivec2 &screenPos )  override;
	bool    doDragEnd   ( const Math::ivec2 &screenPos )  override;

protected:
	void    updatePos( const Math::ivec2 &screenPos );

protected:
	bool         _dragInProgress = false;
	Math::ivec2  _prevScreenPos;

};


}

#endif // VIEWERCONTROLLER2DPAN_ONCE



