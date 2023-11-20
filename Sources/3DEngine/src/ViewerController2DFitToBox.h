// ViewerController2DFitToBox.h
//
#ifndef VIEWERCONTROLLER2DFITTOBOX_ONCE
#define VIEWERCONTROLLER2DFITTOBOX_ONCE

#include "s2Scene_API.h"

#include "ViewerController.h"

namespace Scene {

// ------------------------------------------------------------------------------------------------
class S2SCENE_API ViewerController2DFitToBox : public ViewerController2D
{
public:
	bool    start( const Math::box3 &box );

	bool    updateBox( const Math::box3 &box );

	bool    doResize( int w, int h ) override;

protected:
	Math::box3			 _box;
};


}

#endif // VIEWERCONTROLLER2DFITTOBOX_ONCE



