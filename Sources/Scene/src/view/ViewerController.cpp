// ViewerController.cpp
// 
#include "ViewerController.h"

using namespace RenderPipeline;

// ------------------------------------------------------------------------------------------------
bool ViewerController::acquireViewer( Viewer *viewer )
{
	assert( viewer );
	assert( _viewer == nullptr );

	if( _viewer )
		return false;

	_viewer = viewer;
	_status = Status::Completed;
	return true;
}

// ------------------------------------------------------------------------------------------------
/*
// TO BE CUSTOMIZED
bool ViewerController::start( ... )
{
	if( !canStart() )  return false;


	some_code;
	if( some_code )
	{
		doReleaseViewer( false );
		return false;
	}	
	some_code;


	doStart();  return true;
}
*/

// ------------------------------------------------------------------------------------------------
void ViewerController::doStart()
{
	assert( _viewer );
	assert( _status != Status::InProgress );

	_status = Status::InProgress;
}

// ------------------------------------------------------------------------------------------------
bool ViewerController::doReleaseViewer( bool completedSuccessfully )
{
	if( _viewer )
	{
		//releasingViewerEvent();
		_viewer = nullptr;

		_status = completedSuccessfully ? Status::CompletedSuccessfully : Status::Completed;
		return true;
	}
	else
	{
		assert( _status != Status::InProgress );
		return false;
	}
}				     