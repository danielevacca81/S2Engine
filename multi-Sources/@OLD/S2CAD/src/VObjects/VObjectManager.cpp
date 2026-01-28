// VObjectManager.cpp
//	
#include "VObjectManager.h"

#include "Renderer/Renderer.h"

#include "Selection/SelectionSet.h"

#include <assert.h>
#include <algorithm>

//#include "Utils/MemCheck.h"

using namespace s2;

// ------------------------------------------------------------------------------------------------
VObjectManager::VObjectManager()
: _hilightedObject(0)
, _selectionSet( new SelectionSet )
{}

// ------------------------------------------------------------------------------------------------
VObjectManager::~VObjectManager()
{
	removeAllObjects();

	delete _selectionSet;
}

// ------------------------------------------------------------------------------------------------
int VObjectManager::addObject( const VObjectPtr &obj )
{
	// Null objects are discarded
	if( !obj )
		return objectCount();

	// check if an object with the same id already exists
	if( containsObject( obj ) )
		removeObject( obj );

	_objects.push_back( obj );

	createIndex( obj );

	obj->registerObserver( this );

	updateBoundingBox();

	return objectCount();
}

// ------------------------------------------------------------------------------------------------
int VObjectManager::removeObject( const VObjectPtr &obj )
{
	auto it = findObject( obj );

	if( it == _objects.end() )
		return objectCount(); // object not found

	removeIndex( (*it) );
	_selectionSet->removeEntry( (*it)->id() );
	(*it)->unregisterObserver(this);
	 
	_objects.erase(it);

	updateBoundingBox();
	
	return objectCount();
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::removeAllObjects()
{
	_selectionSet->reset();
	_objects.clear();
	_objectIndex.clear();
	
	updateBoundingBox();
}

// ------------------------------------------------------------------------------------------------
bool VObjectManager::containsObject( const VObjectPtr &obj ) const
{
	auto found = std::find( _objects.begin(), _objects.end(), obj );

	return found != _objects.end();
}

// ------------------------------------------------------------------------------------------------
int VObjectManager::objectCount() const
{
	return (int) _objects.size();
}

// ------------------------------------------------------------------------------------------------
int VObjectManager::selectedObjectCount() const
{
	return (int) _objects.size();
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::createIndex( const VObjectPtr &obj )
{
	_objectIndex[ obj->type() ].push_back( obj );
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::removeIndex( const VObjectPtr &obj )
{
	auto it = _objectIndex.find( obj->type() );

	assert( it != _objectIndex.end() );
	
	it->second.remove(obj);
}

// ------------------------------------------------------------------------------------------------
VObjectManager::VObjectList::const_iterator VObjectManager::findObject( const VObjectPtr &obj ) const
{
	return std::find( _objects.begin(), _objects.end(), obj );

	//for( VObjectList::const_iterator it = _objects.begin();
	//	 it != _objects.end();
	//	 ++it )
	//{
	//	if( (*it) == obj )
	//		return it; // object found
	//}
	//
	//return _objects.end();
}

// ------------------------------------------------------------------------------------------------
Math::box3 VObjectManager::boundingBox() const
{
	return _boundingBox;
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::updateBoundingBox()
{
	Math::box3 box;
	for( auto it = _objects.begin();
		it !=      _objects.end();
		++it )
	{
		box.extend( (*it)->boundingBox() );
	}

	_boundingBox = box;
}


// ------------------------------------------------------------------------------------------------
VObjectManager::VObjectList VObjectManager::objects() const
{
	return _objects;
}

// ------------------------------------------------------------------------------------------------
VObjectPtr VObjectManager::hilightedObject() const
{
	return _hilightedObject;
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::hilightObject( const VObjectPtr &obj )
{
	if(_hilightedObject == obj )
		return;

	//// undo highlighting on the current object
	//if( _hilightedObject )
	//	_hilightedObject->hilight(false);

	//// get new highlighted object (it may be NULL)
	//_hilightedObject = obj;

	//// set highlighting to the hovered object (if any)
	//if( _hilightedObject )	
	//	_hilightedObject->hilight(true);
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::hilightObjectAt( const Math::dvec3 &p )
{
	//_pickPoint = _renderer->convertWorldToWindow(p,false);
	//
	//std::vector<unsigned int> v = _renderer->getSelectionNames( OpenGL::Selection::All );

	//if( v.empty() )
	//{
	//	hilightObject( nullptr );
	//	return;
	//}

	//// loop through objects list
	//VObjectPtr obj = nullptr;
	//size_t i = 0;
	//do 
	//{
	//	// hilight the first hilightable object in the list
	//	obj = getObject( v[i++] );
	//	
	//	if( obj && obj->isSelectionEnabled() )
	//	{
	//		hilightObject( obj );
	//		break;
	//	}
	//} 
	//while( i<v.size() );
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::setSelectionPolicy(  unsigned int policyMask )
{
	_selectionSet->setPolicy( (SelectionSet::SelectionPolicy) policyMask );
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::selectArea( const Math::dvec3 &p0, const Math::dvec3 &p1 )
{
	//const double threshold = _renderer->pixelToWorldSize(4); // four pixel threshold
	//
	//if( Math::distance( p0,p1 ) < threshold )
	//	selectObjectAt(p0);
	//else
	//{
	//	Math::box3 selectionArea;
	//	selectionArea.extend(p0);
	//	selectionArea.extend(p1);

	//	selectArea(selectionArea);
	//}
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::selectArea( const Math::box3 &area )
{
	//_selectionSet->reset();

	//bool added = false;
	//for( VObjectList::iterator it = _objects.begin();
	//	it != _objects.end();
	//	++it )
	//{
	//	if( area.overlaps2D( (*it)->boundingBox() ) )
	//		if( area.contains2D( (*it)->boundingBox() ) || (*it)->intersects( area ) )
	//			if( (*it)->isSelectionEnabled() )
	//				_selectionSet->addObject((*it)->id());
	//}

	//updateSelection();
}

// ------------------------------------------------------------------------------------------------
//void VObjectManager::selectObject( int id )
//{
//	_selectionSet->reset();
//
//	if( id>=0 )
//	{
//		VObjectList::const_iterator it = findObject( id );
//		
//		if( it != _objects.end() )
//			if( (*it)->isSelectionEnabled() )_selectionSet->addObject( (*it)->id() );
//	}
//
//	updateSelection();
//}

// ------------------------------------------------------------------------------------------------
//void VObjectManager::selectObjectAt( const Math::dvec3 &p )
//{
//	_selectionSet->reset();
//
//	_pickPoint = _renderer->convertWorldToWindow(p,false);
//	
//	std::vector<unsigned int> v = _renderer->getSelectionNames();
//
//	VObjectPtr obj = nullptr;
//	if( v.empty() )
//		updateSelection();
//	else
//	{
//		//_selectionSet->setPolicy( SelectionSet::PolicyExclusive );
//
//		obj = getObject(v[0]);
//		if( obj )
//			if( obj->isSelectionEnabled() )_selectionSet->addObject( obj->id() );
//
//		//const VObjectPtr 
//		//if( obj )
//		//	obj->setSelected( _selectionSet->addObject(v[0]) );	
//	}
//
//
//	hilightObject( obj );
//	updateSelection();
//}

// ------------------------------------------------------------------------------------------------
void VObjectManager::unselectObject( const VObjectPtr &obj )
{
	//if( id<0 )
	//	_selectionSet->reset();
	//else
	//{
	//	VObjectList::const_iterator it = findObject( id );

	//	if( it != _objects.end() )
	//		_selectionSet->removeObject( (*it)->id() );
	//}

	//updateSelection();
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::clearSelection()
{
	//_selectionSet->reset();

	//for( VObjectList::iterator it = _objects.begin();
	//	it != _objects.end();
	//	++it )
	//	(*it)->select(false);
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::notify( IObservable *o )
{
	VObject* obj = static_cast<VObject*>(o);

	const VObject::State s = obj->state();

	if( s.Culled ) {}
	if( s.Hilighted ) {}
	if( s.Selected ) {}
	if( s.Selectable) {}
	if( s.Styled) {}
	if( s.Visible) {}
}


//// ------------------------------------------------------------------------------------------------
//void VObjectManager::notifyObjectSelected( const VObjectPtr &obj )
//{
//	if( obj->isSelected() )
//		selectObject( obj->id() ); 
//	else
//		unselectObject( obj->id() );
//}
//
//// ------------------------------------------------------------------------------------------------
//void VObjectManager::notifyObjectHilighted( const VObjectPtr &obj )
//{
//	if( obj->isHilighted() )
//		hilightObject(obj);  
//}

// ------------------------------------------------------------------------------------------------
void VObjectManager::updateSelection()
{
	//SelectionSet::SelectionList selected = _selectionSet->get();

	//// deselect all
	//for( VObjectList::iterator it = _objects.begin();
	//	it != _objects.end();
	//	++it )
	//	(*it)->select(false);


	//for( SelectionSet::SelectionList::iterator it = selected.begin();
	//	 it != selected.end();
	//	 ++it )
	//	getObject( *it )->select(true);	
}

// ------------------------------------------------------------------------------------------------
VObjectManager::VObjectList VObjectManager::selectedObjects() const
{
	VObjectList objs;

	//SelectionSet::SelectionList list = _selectionSet->get();

	//for( auto it = list.begin();
	//	 it != list.end();
	//	 ++it )
	//	objs.push_back( it-> );

	return objs;
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::drawObjects( OpenGL::Renderer *r ) const
{
	r->draw( OpenGL::Points,    _pointsBuffer   );
	r->draw( OpenGL::Lines,     _linesBuffer    );
	r->draw( OpenGL::Triangles, _polygonsBuffer );

	//glPushAttrib( GL_ALL_ATTRIB_BITS );
	//{
	//	//glEnable(GL_POLYGON_SMOOTH);
	//	glEnable(GL_POINT_SMOOTH);
	//	glEnable(GL_LINE_SMOOTH);
	//	glHint(GL_POINT_SMOOTH_HINT,   GL_NICEST);
	//	glHint(GL_LINE_SMOOTH_HINT,    GL_NICEST);
	//	glDepthFunc(GL_LEQUAL);
	//	//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	//	//glDepthMask(GL_TRUE);
	//	glEnable(GL_DEPTH_TEST);

	//	glEnable(GL_BLEND);
	//	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//	const bool textEnabled  = true;
	//	const int  textSize     = 12;
	//	OpenGL::Color textColor = OpenGL::Color::white();

	//	std::vector<VObjectList::const_iterator> selectedList;

	//	// draw objects by type (shapes, lines, points)
	//	for( VObjectIndex::const_reverse_iterator it = _objectIndex.rbegin();
	//		 it != _objectIndex.rend();
	//		 ++it )
	//	{
	//		for( VObjectList::const_iterator jt = it->second.begin();
	//			 jt != it->second.end();
	//			 ++jt )
	//		{
	//			// skip the hilighted obj
	//			if( (*jt)->isHilighted() )
	//				continue;

	//			if( (*jt)->isSelected() )
	//			{
	//				selectedList.push_back(jt);
	//				continue;
	//			}

	//			(*jt)->draw(_renderer);

	//			if( textEnabled && (*jt)->isVisible() )
	//				_renderer->addText( (*jt)->center(), (*jt)->name(), textColor,true, 0, textSize );//, OpenGL::TextRenderer::AlignCenter );
	//		}
	//	}

	//	glDisable(GL_DEPTH_TEST);
	//	// draw selected objects
	//	for( size_t i=0; i<selectedList.size(); ++i )
	//		(*selectedList[i])->draw( _renderer );

	//	// at the end draw the hilighted (if any)
	//	if( _hilightedObject )
	//	{
	//		_hilightedObject->draw(_renderer);
	//		//_renderer->addText( _hilightedObject->center(), _hilightedObject->name(), _hilightedObject->color() ,0, textSize, OpenGL::TextRenderer::AlignCenter );
	//		_renderer->addText( _hilightedObject->center(), _hilightedObject->name(), _hilightedObject->color(),true,0, textSize );
	//	}
	//}
	//glPopAttrib();

	//drawForSelection();
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::drawForSelection( ) const
{
	//glPushAttrib(GL_ALL_ATTRIB_BITS);
	//glDisable(GL_TEXTURE_2D);
	//glColor3ub(255,255,255);

	//do
	//{
	//	Math::dmat4 selMatrix = _renderer->openSelectionMode((int)_pickPoint.x,(int)_pickPoint.y);
	//	_renderer->computeClipMatrix(_renderer->modelViewMatrix(),selMatrix);
	//	//_renderer->computeClipMatrix(Math::dmat4(1),selMatrix);

	//	const int typesCount  = (int)_objectIndex.size();
	//	const float depthStep = 1.f / typesCount;
	//	float currDepth = 0.f;

	//	// draw objects by type (shapes, lines, points)
	//	for( VObjectIndex::const_reverse_iterator it = _objectIndex.rbegin();
	//		 it != _objectIndex.rend();
	//		 ++it )
	//	{
	//		glPushMatrix();
	//		glTranslatef(0,0,currDepth);
	//		for( VObjectList::const_iterator jt = it->second.begin();
	//			 jt != it->second.end();
	//			 ++jt )
	//		{

	//			if( !_renderer->isVisible( (*jt)->boundingBox() ) )
	//				continue; //culling
	//			if( (*jt)->isVisible() && (*jt)->isSelectionEnabled() )
	//				(*jt)->drawForSelection(_renderer);			
	//		}
	//		glPopMatrix();

	//		currDepth += depthStep;
	//	}

	//	_renderer->closeSelectionMode();
	//}
	//while( _renderer->selectionFailed() );

	//glPopAttrib();
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::updateBuffers()
{
	//std::vector<VObjectList::const_iterator> selectedList;

	std::vector<Math::dvec3> points;
	std::vector<Math::dvec3> lines;
	std::vector<Math::dvec3> triangles;

	for( auto it = _objects.begin();
		 it     != _objects.end();
		 ++it )
	{
		const std::vector<Math::dvec3> pts( (*it)->points() );
		const VObject::ObjectType      type( (*it)->type()  );

		if     ( type == VObject::Point    ) std::copy( pts.begin(), pts.end(), points.end()    );
		else if( type == VObject::Line     ) std::copy( pts.begin(), pts.end(), lines.end()     );
		else if( type == VObject::PolyLine ) std::copy( pts.begin(), pts.end(), lines.end()     );
		else if( type == VObject::Shape    ) std::copy( pts.begin(), pts.end(), triangles.end() );
		else if( type == VObject::Mesh     ) std::copy( pts.begin(), pts.end(), triangles.end() );	
	}
	
	{
		OpenGL::VertexBuffer buf( points.size() * sizeof(Math::dvec3), OpenGL::BufferObject::StaticDraw );
		buf.sendData( (void*)(&points[0]), points.size() * sizeof(Math::dvec3), 0 );

		_pointsBuffer.attribute(0)->set( buf, OpenGL::AttributeBuffer::Double,3, false,0,0 );
	}

	{
		OpenGL::VertexBuffer buf( lines.size() * sizeof(Math::dvec3), OpenGL::BufferObject::StaticDraw );
		buf.sendData( (void*)(&lines[0]), lines.size() * sizeof(Math::dvec3), 0 );

		_linesBuffer.attribute(0)->set( buf, OpenGL::AttributeBuffer::Double,3, false,0,0 );	
	}

	{
		OpenGL::VertexBuffer buf( triangles.size() * sizeof(Math::dvec3), OpenGL::BufferObject::StaticDraw );
		buf.sendData( (void*)(&triangles[0]), triangles.size() * sizeof(Math::dvec3), 0 );

		_polygonsBuffer.attribute(0)->set( buf, OpenGL::AttributeBuffer::Double,3, false,0,0 );	
	}
}