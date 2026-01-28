// ViewerController.h
// @author Ivan Miraglia
//
#ifndef RENDERENGINE_VIEWERCONTROLLER_H
#define RENDERENGINE_VIEWERCONTROLLER_H

#include "RenderPipeline_API.h"

#include "Viewer.h"

#include "sigslot/signal.hpp"

namespace RenderPipeline {

// ------------------------------------------------------------------------------------------------
class RENDERPIPELINE_API ViewerController
{
//public:
//	sigslot::signal<> releasingViewerEvent;

public:
	enum class Status 
	{ 
		InProgress, 
		Completed,		// alias: not in progress, stopped, still
		CompletedSuccessfully 
	};

public:
	virtual ~ViewerController() { doReleaseViewer( false ); }

	bool            acquireViewer( Viewer *viewer );              // it does not start
	//bool          start( ... );
	void            releaseViewer() { doReleaseViewer( false ); } // alias: end, complete
	Status			status() const { return _status; }

	bool            resize    ( int w, int h )                 { return _status == Status::InProgress ? doResize    ( w,  h )     : false; }
	bool            animate   ()                               { return _status == Status::InProgress ? doAnimate   ()            : false; }
	bool            dragBegin ( const Math::ivec2 &screenPos ) { return _status == Status::InProgress ? doDragBegin ( screenPos ) : false; }
	bool            dragUpdate( const Math::ivec2 &screenPos ) { return _status == Status::InProgress ? doDragUpdate( screenPos ) : false; }
	bool            dragEnd   ( const Math::ivec2 &screenPos ) { return _status == Status::InProgress ? doDragEnd   ( screenPos ) : false; }

protected:
	Viewer         *viewer() const { return _viewer; }
	bool            canStart() { return _viewer && _status != Status::InProgress; }
	void            doStart();
	bool            doReleaseViewer( bool completedSuccessfully );

	virtual bool    doResize    ( int w, int h )                  { return false; }
	virtual bool    doAnimate   ()                                { return false; }
	virtual bool    doDragBegin ( const Math::ivec2 &screenPos )  { return false; }
	virtual bool    doDragUpdate( const Math::ivec2 &screenPos )  { return false; }
	virtual bool    doDragEnd   ( const Math::ivec2 &screenPos )  { return false; }

private:
	Status   _status { Status::Completed };	// \invariant _status==InProgress => _viewer != nullptr
	Viewer* _viewer  { nullptr };
};


// ------------------------------------------------------------------------------------------------
template <class VIEWER_T >
class ViewerControllerCast : public ViewerController
{
public:
	VIEWER_T       *viewer() { return static_cast<VIEWER_T*>( ViewerController::viewer() ); }
};


// ------------------------------------------------------------------------------------------------
using ViewerControllerOrthographic = ViewerControllerCast< ViewerOrthographic >;
using ViewerController2D           = ViewerControllerCast< Viewer2D >;
using ViewerControllerPerspective  = ViewerControllerCast< ViewerPerspective >;
using ViewerController3D           = ViewerControllerCast< Viewer3D >;


}

#endif // RENDERENGINE_VIEWERCONTROLLER_H



