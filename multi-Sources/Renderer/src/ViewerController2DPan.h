// ViewerController2DPan.h
//
#ifndef RENDERENGINE_VIEWERCONTROLLER2DPAN_H
#define RENDERENGINE_VIEWERCONTROLLER2DPAN_H

#include "RenderPipeline_API.h"

#include "ViewerController.h"

namespace RenderPipeline {

// ------------------------------------------------------------------------------------------------
class RENDERPIPELINE_API ViewerController2DPan : public ViewerController2D
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

#endif // RENDERENGINE_VIEWERCONTROLLER2DPAN_H



