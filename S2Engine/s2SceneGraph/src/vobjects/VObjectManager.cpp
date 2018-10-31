// VObjectManager.cpp
//	
#include "VObjectManager.h"

#include "SelectionSet.h"

#include <assert.h>

#include <iostream>

using namespace s2;
using namespace s2::SceneGraph;

// ------------------------------------------------------------------------------------------------
VObjectManager::VObjectManager()
: _visible( true )
, _selectionEnabled( true)
, _hilightEnabled( true )
{}

// ------------------------------------------------------------------------------------------------
VObjectManager::~VObjectManager()
{
	removeAllObjects();
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::enableSelection( bool enable )                               { _selectionEnabled = enable; if( !enable ) clearSelection(); }
void VObjectManager::enableHilight( bool enable )                                 { _hilightEnabled   = enable; if( !enable ) clearHilight();   }
void VObjectManager::setVisible( bool enable )                                    { _visible          = enable; }
void VObjectManager::setSelectionPolicy( const SelectionSet::Policy &policyMask ) { _selectionSet.setPolicy( policyMask ); }

// ------------------------------------------------------------------------------------------------
bool VObjectManager::isSelectionEnabled()              const { return _selectionEnabled; }
bool VObjectManager::isHilightEnabled()                const { return _hilightEnabled;   }
bool VObjectManager::isVisible()                       const { return _visible;          }
bool VObjectManager::isEmpty()                         const { return _objects.empty();  }
SelectionSet::Policy VObjectManager::selectionPolicy() const { return _selectionSet.policy(); }


// ------------------------------------------------------------------------------------------------
bool VObjectManager::addObject( const VObjectPtr &obj )
{
	// Null objects are discarded
	if( !obj )
		return false;


	// @todo: recursive addObject if object is GROUP?

	// If object with the same id exists, this means that object is already inserted
	if( objectByID( obj->id() ) )
		removeObject( obj );

	_objects.push_back( obj );

	createIndex( obj );

	obj->_manager = shared_from_this(); // weak reference
	obj->registerObserver( this );

	return true;
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::removeObject( const VObjectPtr &obj )
{
	VObjectList::const_iterator it = findObjectIndex( obj );

	if( it == _objects.end() )
		return; // object not found

	removeIndex( ( *it ) );
	_selectionSet.removeItem( ( *it )->id() );
	( *it )->unregisterObserver( this );

	_objects.erase( it );
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::removeAllObjects()
{
	for( auto &obj : _objects )
		obj->unregisterObserver( this );
	//for( VObjectList::iterator it = _objects.begin();
	//	 it != _objects.end();
	//	 ++it )
	//{
	//	delete (*it);
	//}

	_selectionSet.clear( SelectionSet::IgnorePolicy );
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
	// @tbd
	return (unsigned int) _objects.size();
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::createIndex( const VObjectPtr &obj )
{
	if( !obj )
		return;

	_objectIndex[obj->type()].push_back( obj );
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::removeIndex( const VObjectPtr &obj )
{
	if( !obj )
		return;

	VObjectIndex::iterator it = _objectIndex.find( obj->type() );

	assert( it != _objectIndex.end() );

	it->second.remove( obj );
}

// ------------------------------------------------------------------------------------------------
VObjectManager::VObjectList::const_iterator VObjectManager::findObjectIndex( const VObjectPtr &key ) const
{

	for( VObjectList::const_iterator it = _objects.begin();
		 it != _objects.end();
		 ++it )
	{
		if( ( *it )->id() == key->id() )
			return it; // object found
	}

	return _objects.end();
}


// ------------------------------------------------------------------------------------------------
Math::box3 VObjectManager::boundingBox() const
{
	Math::box3 box;

	for( auto &obj : _objects )
		box.extend( obj->boundingBox() );

	return box;
}

// ------------------------------------------------------------------------------------------------
VObjectPtr VObjectManager::objectByID( unsigned int id ) const
{
	for( VObjectList::const_iterator it= _objects.begin();
		 it != _objects.end();
		 ++it )
	{
		if( ( *it )->id() == id )
			return ( *it ); // object found
	}
	return nullptr;
}

// ------------------------------------------------------------------------------------------------
std::vector<VObjectPtr> VObjectManager::objects() const
{
	return std::vector<VObjectPtr>( _objects.begin(), _objects.end() );
}

// ------------------------------------------------------------------------------------------------
VObjectPtr VObjectManager::hilightedObject() const
{
	return _hilightedObject;
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::hilightObject( const VObjectPtr &obj )
{
	if( !_hilightEnabled )
		return;

	if( _hilightedObject == obj )
		return;

	// undo highlighting on the current object
	if( _hilightedObject )
		_hilightedObject->setHilighted( false );
		//_hilightedObject->setState(_hilightedObject->state() -= VObject::State::Hilighted);

	// set highlighting to the hovered object (if any)
	if( obj )
		obj->setHilighted( true );
		//obj->setState(_hilightedObject->state() += VObject::State::Hilighted);

	// get new highlighted object (it may be NULL)
	_hilightedObject = obj;
}

// ------------------------------------------------------------------------------------------------
VObjectPtr VObjectManager::hilightObjectAt( const Math::ivec2 &screenCoord )
{
	if( !_hilightEnabled )
		return nullptr;

	VObjectPtr obj = extractObjectAt( screenCoord );

	hilightObject( obj );
	return obj;
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::selectObject( const VObjectPtr &obj, bool select )
{
	if( !_selectionEnabled )
		return;	

	if( !obj )
		return;

	if( obj->isSelectable() )
	{
		if( select ) _selectionSet.addItem( obj->id() );
		else         _selectionSet.removeItem( obj->id() ); // does nothing if not found
	}

	updateSelection();
}

// ------------------------------------------------------------------------------------------------
VObjectPtr VObjectManager::selectObjectAt( const Math::ivec2 &p, bool select )
{
	if( !_selectionEnabled )
		return nullptr;

	VObjectPtr obj = extractObjectAt( p );

	if( !obj )
	{
		if( !(_selectionSet.policy() & SelectionSet::NotEmpty ) )
			_selectionSet.clear( SelectionSet::IgnorePolicy );

		updateSelection();
		return nullptr;
	}

	if( obj->isSelectable() )
	{
		if( select ) _selectionSet.addItem( obj->id() );
		else         _selectionSet.removeItem( obj->id() );
	}

	hilightObject( obj );
	updateSelection();
	return obj;
}

// ------------------------------------------------------------------------------------------------
std::vector<VObjectPtr> VObjectManager::selectObjectsInArea( const Math::ivec2 &p0, const Math::ivec2 &p1 )
{
	const float threshold = 4.f; // pixels // _renderer->pixelToWorldSize( 4 ); // four pixel threshold

	// distance accepts only floating points
	if( Math::distance( Math::vec2(p0), Math::vec2(p1) ) < threshold )
		return { selectObjectAt( p0 ) };
	else
	{
		//Math::box3 selectionArea;
		//selectionArea.extend( p0 );
		//selectionArea.extend( p1 );

		return selectArea( Math::Rectangle( p0,p1 ) );
	}
}


// ------------------------------------------------------------------------------------------------
std::vector<VObjectPtr> VObjectManager::selectArea( const Math::Rectangle &area )
{
	if( !_selectionEnabled )
		return {};
	
	std::vector<VObjectPtr> selectedObjects;
	// todo: cull objects (do not check culled objects)
	for( auto &obj : _objects )
	{
		if( !obj->isSelectable() )
			continue;

		//const Rectangle r = obj->boundingBox().transformed().toRectangle();

		//if( area.overlaps( r ) )
		//	if( area.contains( r ) || area->intersects( r ) )
		//		_selectionSet.addItem( obj->id() );
	}
	
	if( !(_selectionSet.policy() & SelectionSet::NotEmpty ) )
		_selectionSet.clear( SelectionSet::IgnorePolicy );

	updateSelection();
	return selectedObjects;
}


// ------------------------------------------------------------------------------------------------
VObjectPtr VObjectManager::extractObjectAt( const Math::ivec2 &screenCoords )
{
	// @todo: 
	//auto rgb = _surface->readPixelAt( screenCoords.x, screenCoords.y, Renderer::FrameBuffer::ColorAttachment0 );
	// SelectionName name = SelectionName::fromRGB(rgb);
	const int id = 0;


	VObjectPtr obj = objectByID( id ); // obj is either a leaf or the root of a tree-group
	
	if( !obj )
		return nullptr; // assert ?


	//if( obj->isGroup() )
	//	obj->dynamicCastTo<VGroup>()->setChangedLeaf( id );

	return obj;
}

// ------------------------------------------------------------------------------------------------
VObjectPtr VObjectManager::findObjectData( const VObject::ObjectData &data ) const
{
	auto it = std::find_if( _objects.begin(), _objects.end(), 
							[data] ( const VObjectPtr &a ) 
	{ return a->userData() == data; } 
	);

	if( it == _objects.end() )
		return nullptr; // not found

	return *it;
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::clearSelection()
{
	if( !_selectionEnabled )
		return;

	_selectionSet.clear( SelectionSet::IgnorePolicy );

	for( auto &obj : _objects )
		obj->setSelected( false );
		//obj->setState( (*it)->state() -= VObject::State::Selected );
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::clearHilight()
{
	// undo highlighting on the current object
	if( _hilightedObject )
        _hilightedObject->setHilighted( false );
	
	_hilightedObject = nullptr;
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::notify( Observable *o, const std::any &message )
{
	using ObjectUpdateMessage = std::pair<std::string, bool>; // @todo: add object ID
 
	try
    {
		const ObjectUpdateMessage msg = std::any_cast<ObjectUpdateMessage>( message );

		if      ( msg.first == "selected"  ) {}
		else if ( msg.first == "hilighted" ) {}
		//else if ( msg.first == "hilighted" ) {}
		//else if ( msg.first == "hilighted" ) {}
		//else if ( msg.first == "hilighted" ) {}
    }
	catch( const std::bad_any_cast& e )
    {
        std::cout << "Malformed VObject message" << '\n';
    }
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::updateSelection()
{
	if( !_selectionEnabled )
		return;

	SelectionSet::Items selected = _selectionSet.items();

	// ultra inefficient two pass selection update
	
	// deselect all
	for( auto &obj : _objects )
		 obj->setSelected( false );
		//obj->setState( (*it)->state() -= VObject::State::Selected );

	// set selected only those in the selectionset (if any)
	for( auto &itemID : selected )
		objectByID( itemID )->setSelected( true );
		//objectByID( itemID )->setState( obj->state() += VObject::State::Selected );
}

// ------------------------------------------------------------------------------------------------
std::vector<VObjectPtr> VObjectManager::selectedObjects() const
{
	SelectionSet::Items list = _selectionSet.items();

	std::vector<VObjectPtr> selectedObjects;

	for( auto &itemID : list )
		selectedObjects.push_back( objectByID( itemID ) );

	return selectedObjects;
}

// ------------------------------------------------------------------------------------------------
VObjectManager::VObjectList VObjectManager::cullObjects() const
{
	// use transform feedback to cull object on GPU
	return { };
}

// ------------------------------------------------------------------------------------------------
VObjectManager::VObjectList VObjectManager::sortObjects( const VObjectList &objList ) const
{
	struct
	{
		bool operator()( const VObjectPtr &a, const VObjectPtr &b )
		{
			return a->boundingBox().diag() > b->boundingBox().diag();
		}
	} compareBBox;

	VObjectList sortedList(objList);
	sortedList.sort( compareBBox );

	return sortedList;
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::updateBuffers()
{
	// compute VBOs for drawing
	// 1 vbo per object type?
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::draw( const Renderer::SurfacePtr &surface, const Renderer::DrawingState &drawState ) const
{
	if( !_visible || _objects.empty() )
		return;

	// no more need to cull objects on CPU if use transform feedback
	//VObjectList visibleObjs = cullObjects();

	for( auto &b : _primitiveBuffers )
	{
		Renderer::Primitive p;
		switch( b.first )
		{
		case 0: p = Renderer::Primitive::Points; break;
		case 1: p = Renderer::Primitive::Lines; break;
		case 2: p = Renderer::Primitive::Triangles; break;
		case 3: p = Renderer::Primitive::TriangleStrip; break;
		}

		surface->draw( p, b.second, drawState );
	}

#if 0
	glPushAttrib( GL_ALL_ATTRIB_BITS );
	{
		glEnable( GL_POINT_SMOOTH );
		glEnable( GL_LINE_SMOOTH );
		glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );
		glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
		glDepthFunc( GL_LEQUAL );

		glEnable( GL_DEPTH_TEST );

		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

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
				if( ( *jt )->isHilighted() )
					continue;

				if( ( *jt )->isSelected() )
				{
					selectedList.push_back( jt );
					continue;
				}

				( *jt )->draw( _renderer );

				if( textEnabled && ( *jt )->isVisible() )
					_renderer->addText( ( *jt )->center(), ( *jt )->name(), textColor, textSize, OpenGL::TextRenderer::AlignCenter );
			}
		}

		glDisable( GL_DEPTH_TEST );
		// draw selected objects
		for( size_t i=0; i < selectedList.size(); ++i )
			( *selectedList[i] )->draw( _renderer );

		// at the end draw the hilighted (if any)
		if( _hilightedObject )
		{
			_hilightedObject->draw( _renderer );
			_renderer->addText( _hilightedObject->center(), _hilightedObject->name(), _hilightedObject->color(), textSize, OpenGL::TextRenderer::AlignCenter );
		}
	}
	glPopAttrib();

	drawForSelection();
	glCheck;
#endif
}

// ------------------------------------------------------------------------------------------------
void VObjectManager::drawForSelection() const
{
	//glPushAttrib( GL_ALL_ATTRIB_BITS );
	//glDisable( GL_TEXTURE_2D );
	//glColor3ub( 255, 255, 255 );

	//do
	//{
	//	Math::dmat4 selMatrix = _renderer->openSelectionMode( (int) _pickPoint.x, (int) _pickPoint.y );
	//	_renderer->computeClipMatrix( _renderer->modelViewMatrix(), selMatrix );
	//	//_renderer->computeClipMatrix(Math::dmat4(1),selMatrix);

	//	const int typesCount  = (int) _objectIndex.size();
	//	const float depthStep = 1.f / typesCount;
	//	float currDepth = 0.f;

	//	// draw objects by type (shapes, lines, points)
	//	for( VObjectIndex::const_reverse_iterator it = _objectIndex.rbegin();
	//		 it != _objectIndex.rend();
	//		 ++it )
	//	{
	//		glPushMatrix();
	//		glTranslatef( 0, 0, currDepth );
	//		for( VObjectList::const_iterator jt = it->second.begin();
	//			 jt != it->second.end();
	//			 ++jt )
	//		{
	//			if( !_renderer->isVisible( ( *jt )->boundingBox() ) )
	//				continue; //culling

	//			if( ( *jt )->isVisible() && ( *jt )->isSelectable() )
	//				( *jt )->drawForSelection( _renderer );
	//		}
	//		glPopMatrix();

	//		currDepth += depthStep;
	//	}

	//	_renderer->closeSelectionMode();
	//} while( _renderer->selectionFailed() );

	//glPopAttrib();
}