// ViewerController2DFillByBox.cpp
// 
#include "ViewerController2DFillByBox.h"

using namespace Scene;

// ------------------------------------------------------------------------------------------------
bool ViewerController2DFillByBox::start( const Math::box3 &b )
{
	if( !canStart() )  return false;

	_box = b;
	viewer()->fillByBox( _box ); 

	doStart();  return true;
}

// ------------------------------------------------------------------------------------------------
bool ViewerController2DFillByBox::updateBox( const Math::box3 &box )
{
	if( status() != Status::InProgress )
		return false;

	if( _box.equals( box ) )
		return true;
	
	_box = box;
	viewer()->fillByBox( _box ); 
	return true;
}

// ------------------------------------------------------------------------------------------------
bool ViewerController2DFillByBox::doResize( int w, int h ) 
{
	if( _box.isEmpty() )
		return false;

	viewer()->setViewport( Math::Rectangle( 0, 0, w, h ) );
	viewer()->fillByBox( _box );
	return true;
}








				     