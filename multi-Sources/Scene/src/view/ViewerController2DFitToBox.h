// ViewerController2DFitToBox.h
//
#ifndef RENDERENGINE_VIEWERCONTROLLER2DFITTOBOX_H
#define RENDERENGINE_VIEWERCONTROLLER2DFITTOBOX_H

#include "RenderPipeline_API.h"

#include "ViewerController.h"

namespace RenderPipeline {

// ------------------------------------------------------------------------------------------------
class RENDERPIPELINE_API ViewerController2DFitToBox : public ViewerController2D
{
public:
	bool    start( const Math::dbox3 &box );

	bool    updateBox( const Math::dbox3 &box );

	bool    doResize( int w, int h ) override;

protected:
	Math::dbox3 _box;
};


}

#endif // RENDERENGINE_VIEWERCONTROLLER2DFITTOBOX_H



