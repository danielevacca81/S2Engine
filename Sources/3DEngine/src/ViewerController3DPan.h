// ViewerController3DPan.h
//
#ifndef VIEWERCONTROLLER3DPAN_ONCE
#define VIEWERCONTROLLER3DPAN_ONCE

#include "s2Scene_API.h"

#include "ViewerController.h"

namespace Scene {

// ------------------------------------------------------------------------------------------------
class S2SCENE_API ViewerController3DPan : public ViewerController3D
{
public:
	bool    start(); 

	bool    doDragBegin ( const Math::ivec2 &screenPos )  override;
	bool    doDragUpdate( const Math::ivec2 &screenPos )  override;
	bool    doDragEnd   ( const Math::ivec2 &screenPos )  override;

protected:
	bool    updatePos( const Math::ivec2 &screenPos );

protected:
	bool         _dragInProgress = false;
	Math::ivec2  _screenPickPos;
	Math::dvec3  _pickWorldPos;
};


}

#endif // VIEWERCONTROLLER3DPAN_ONCE



