// VObjectManager.cpp
//	
#include "VObjectManager.h"

#include "OpenGL/Renderer.h"
#include "OpenGL/OpenGL.h"

#include "Geometry/SelectionSet.h"

#include <assert.h>

#include <iostream>

#include "Utils/MemCheck.h"

// ------------------------------------------------------------------------------------------------
VObjectManager::VObjectManager( OpenGL::Renderer *r )
: _renderer(r)
, _hilightedObject(0)
, _selectionSet( new SelectionSet )
{}

// ------------------------------------------------------------------------------------------------
VObjectManager::~VObjectManager()
{
	removeAllObjects();

	delete _selectionSet;
}

// ------------------------------------------------------------------------------------------------
bool VObjectManager::addObject( VObjectPtr obj )
{
	// Null objects are discarded
	if( !obj )
		return false;

	// If object with the same id exists, this means that object is already inserted
	if( getObject( obj->id() ) )
		removeObject( obj->id() );

	_objects.push_back( obj );

	createIndex(obj);

	obj->setManager(this);

	return true;
}

// ------------------------------------------------------------------------------------------------
bool VObjectManager::addObject( VObject *obj )
{
	return addObject( std::shared_ptr<VObject>(obj) );
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::removeObject( unsigned int id )
{
	VObjectList::const_iterator it = findObject( id );

	if( it == _objects.end() )
		return; // object not found

	removeIndex( (*it) );
	_selectionSet->removeObject( (*it)->id() );
	(*it)->setManager(0);
	 
	_objects.erase(it);



	//return (*it).get();
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::removeAllObjects()
{
	//for( VObjectList::iterator it = _objects.begin();
	//	 it != _objects.end();
	//	 ++it )
	//{
	//	delete (*it);
	//}

	_selectionSet->reset();
	_objects.clear();
	_objectIndex.clear();
}

// ------------------------------------------------------------------------------------------------
unsigned int VObjectManager::objectCount() const
{
	return (unsigned int) _objects.size();
}

// ------------------------------------------------------------------------------------------------
unsigned int VObjectManager::selectedObjectCount() const
{
	return (unsigned int) _objects.size();
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::createIndex( VObjectPtr obj )
{
	if( !obj )
		return;

	_objectIndex[ obj->type() ].push_back( obj );
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::removeIndex( VObjectPtr obj )
{
	if( !obj )
		return;

	VObjectIndex::iterator it = _objectIndex.find( obj->type() );

	assert( it != _objectIndex.end() );
	
	it->second.remove(obj);
}

// ------------------------------------------------------------------------------------------------
VObjectManager::VObjectList::const_iterator VObjectManager::findObject( unsigned int id ) const
{
	for( VObjectList::const_iterator it = _objects.begin();
		 it != _objects.end();
		 ++it )
	{
		if( (*it)->id() == id )
			return it; // object found
	}
	
	return _objects.end();
}


// ------------------------------------------------------------------------------------------------
Math::box3 VObjectManager::boundingBox() const
{
	Math::box3 box;

	for( VObjectList::const_iterator it = _objects.begin();
		 it != _objects.end();
		 ++it )
	{
		box.extend( (*it)->boundingBox() );
	}
	
	return box;
}

// ------------------------------------------------------------------------------------------------
VObjectPtr VObjectManager::getObject( unsigned int id ) const
{
	for( VObjectList::const_iterator it= _objects.begin();
		it != _objects.end();
		++it )
	{
		if( (*it)->id() == id )
			return (*it); // object found
	}
	return nullptr;
}

// ------------------------------------------------------------------------------------------------
std::vector<VObjectPtr> VObjectManager::getObjects() const
{
	std::vector<VObjectPtr> objs;
	
	for( VObjectList::const_iterator it= _objects.begin();
		it != _objects.end();
		++it )
		objs.push_back(*it);

	return objs;
}

// ------------------------------------------------------------------------------------------------
VObjectPtr VObjectManager::getHilightedObject() const
{
	return _hilightedObject;
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::hilightObject( VObjectPtr obj )
{
	if(_hilightedObject == obj )
		return;

	// undo highlighting on the current object
	if(_hilightedObject)
		_hilightedObject->hilight(false);

	// get new highlighted object (it may be NULL)
	_hilightedObject = obj;

	// set highlighting to the hovered object (if any)
	if( _hilightedObject )	
		_hilightedObject->hilight(true);
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::hilightObject( int id )
{
	if(_hilightedObject && _hilightedObject->id() == id )
		return;

	// undo highlighting on the current object
	if(_hilightedObject)
		_hilightedObject->hilight(false);

	// get new highlighted object (it may be NULL)
	_hilightedObject = nullptr;
	VObjectList::const_iterator it = findObject(id);
	if( it == _objects.end() )
		return;
	_hilightedObject = *it;

	// set highlighting to the hovered object (if any)
	if(_hilightedObject)	
		_hilightedObject->hilight(true);
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::hilightObjectAt( const Math::dvec3 &p )
{
	_pickPoint = _renderer->convertWorldToWindow(p,false);
	
	std::vector<unsigned int> v = _renderer->getSelectionNames( OpenGL::Selection::All );

	if( v.empty() )
	{
		hilightObject( nullptr );
		return;
	}

	// loop through objects list
	VObjectPtr obj = nullptr;
	size_t i = 0;
	do 
	{
		// hilight the first hilightable object in the list
		obj = getObject( v[i++] );
		
		if( obj && obj->isSelectionEnabled() )
		{
			hilightObject( obj );
			break;
		}
	} 
	while( i<v.size() );
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::setSelectionPolicy(  unsigned int policyMask )
{
	_selectionSet->setPolicy( (SelectionSet::SelectionPolicy) policyMask );
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::selectArea( const Math::dvec3 &p0, const Math::dvec3 &p1 )
{
	const double threshold = _renderer->pixelToWorldSize(4); // four pixel threshold
	
	if( Math::distance( p0,p1 ) < threshold )
		selectObjectAt(p0);
	else
	{
		Math::box3 selectionArea;
		selectionArea.extend(p0);
		selectionArea.extend(p1);

		selectArea(selectionArea);
	}
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::selectArea( const Math::box3 &area )
{
	_selectionSet->reset();

	bool added = false;
	for( VObjectList::iterator it = _objects.begin();
		it != _objects.end();
		++it )
	{
		if( area.overlaps2D( (*it)->boundingBox() ) )
			if( area.contains2D( (*it)->boundingBox() ) || (*it)->intersects( area ) )
				if( (*it)->isSelectionEnabled() )
					_selectionSet->addObject((*it)->id());
	}

	updateSelection();
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::selectObject( int id )
{
	_selectionSet->reset();

	if( id>=0 )
	{
		VObjectList::const_iterator it = findObject( id );
		
		if( it != _objects.end() )
			if( (*it)->isSelectionEnabled() )_selectionSet->addObject( (*it)->id() );
	}

	updateSelection();
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::selectObjectAt( const Math::dvec3 &p )
{
	_selectionSet->reset();

	_pickPoint = _renderer->convertWorldToWindow(p,false);
	
	std::vector<unsigned int> v = _renderer->getSelectionNames();

	VObjectPtr obj = nullptr;
	if( v.empty() )
		updateSelection();
	else
	{
		//_selectionSet->setPolicy( SelectionSet::PolicyExclusive );

		obj = getObject(v[0]);
		if( obj )
			if( obj->isSelectionEnabled() )_selectionSet->addObject( obj->id() );

		//const VObjectPtr 
		//if( obj )
		//	obj->setSelected( _selectionSet->addObject(v[0]) );	
	}


	hilightObject( obj );
	updateSelection();
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::unselectObject( int id )
{
	if( id<0 )
		_selectionSet->reset();
	else
	{
		VObjectList::const_iterator it = findObject( id );

		if( it != _objects.end() )
			_selectionSet->removeObject( (*it)->id() );
	}

	updateSelection();
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::clearSelection()
{
	_selectionSet->reset();

	for( VObjectList::iterator it = _objects.begin();
		it != _objects.end();
		++it )
		(*it)->select(false);
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::notifyObjectSelected( const VObjectPtr &obj )
{
	if( obj->isSelected() )
		selectObject( obj->id() ); 
	else
		unselectObject( obj->id() );
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::notifyObjectHilighted( const VObjectPtr &obj )
{
	if( obj->isHilighted() )
		hilightObject(obj);  
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::updateSelection()
{
	SelectionSet::SelectionList selected = _selectionSet->get();

	// deselect all
	for( VObjectList::iterator it = _objects.begin();
		it != _objects.end();
		++it )
		(*it)->select(false);


	for( SelectionSet::SelectionList::iterator it = selected.begin();
		 it != selected.end();
		 ++it )
		getObject( *it )->select(true);	
}

// ------------------------------------------------------------------------------------------------
std::vector<unsigned int> VObjectManager::getSelectedObjects() const
{
	std::vector<unsigned int> selectedObjects;

	SelectionSet::SelectionList list = _selectionSet->get();

	for( SelectionSet::SelectionList::iterator it = list.begin();
		 it != list.end();
		 ++it )
		selectedObjects.push_back( *it );

	return selectedObjects;
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::drawObjects() const
{
	glPushAttrib( GL_ALL_ATTRIB_BITS );
	{
		//glEnable(GL_POLYGON_SMOOTH);
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_POINT_SMOOTH_HINT,   GL_NICEST);
		glHint(GL_LINE_SMOOTH_HINT,    GL_NICEST);
		glDepthFunc(GL_LEQUAL);
		//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

		//glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 

		const bool textEnabled  = true;
		const int  textSize     = 12;
		OpenGL::Color textColor = OpenGL::Color::white();

		std::vector<VObjectList::const_iterator> selectedList;

		// draw objects by type (shapes, lines, points)
		for( VObjectIndex::const_reverse_iterator it = _objectIndex.rbegin();
			 it != _objectIndex.rend();
			 ++it )
		{
			for( VObjectList::const_iterator jt = it->second.begin();
				 jt != it->second.end();
				 ++jt )
			{
				// skip the hilighted obj
				if( (*jt)->isHilighted() )
					continue;

				if( (*jt)->isSelected() )
				{
					selectedList.push_back(jt);
					continue;
				}

				(*jt)->draw(_renderer);

				if( textEnabled && (*jt)->isVisible() )
					_renderer->addText( (*jt)->center(), (*jt)->name(), textColor, textSize, OpenGL::TextRenderer::AlignCenter );
			}
		}

		glDisable(GL_DEPTH_TEST);
		// draw selected objects
		for( size_t i=0; i<selectedList.size(); ++i )
			(*selectedList[i])->draw( _renderer );

		// at the end draw the hilighted (if any)
		if( _hilightedObject )
		{
			_hilightedObject->draw(_renderer);
			_renderer->addText( _hilightedObject->center(), _hilightedObject->name(), _hilightedObject->color() , textSize, OpenGL::TextRenderer::AlignCenter );
		}
	}
	glPopAttrib();

	//drawForSelection();
	
	glCheck;
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::drawForSelection( ) const
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_TEXTURE_2D);
	glColor3ub(255,255,255);

	do
	{
		Math::dmat4 selMatrix = _renderer->openSelectionMode((int)_pickPoint.x,(int)_pickPoint.y);
		_renderer->computeClipMatrix(_renderer->modelViewMatrix(),selMatrix);
		//_renderer->computeClipMatrix(Math::dmat4(1),selMatrix);

		const int typesCount  = (int)_objectIndex.size();
		const float depthStep = 1.f / typesCount;
		float currDepth = 0.f;

		// draw objects by type (shapes, lines, points)
		for( VObjectIndex::const_reverse_iterator it = _objectIndex.rbegin();
			 it != _objectIndex.rend();
			 ++it )
		{
			glPushMatrix();
			glTranslatef(0,0,currDepth);
			for( VObjectList::const_iterator jt = it->second.begin();
				 jt != it->second.end();
				 ++jt )
			{
				if( !_renderer->isVisible( (*jt)->boundingBox() ) )
					continue; //culling

				if( (*jt)->isVisible() && (*jt)->isSelectionEnabled() )
					(*jt)->drawForSelection(_renderer);			
			}
			glPopMatrix();

			currDepth += depthStep;
		}

		_renderer->closeSelectionMode();
	}
	while( _renderer->selectionFailed() );

	glPopAttrib();
}