// VObjectManager.h
//
#ifndef VOBJECTMANAGER_ONCE
#define VOBJECTMANAGER_ONCE

#include "S2CAD_API.h"

#include "Math/Math.h"
#include "Math/Box.h"

#include "OpenGL/VertexArray.h"

#include "Design/Observer.h"

#include "VObject.h"

#include <map>
#include <vector>
#include <list>

namespace OpenGL {
	class Renderer;
}

namespace s2 {


class SelectionSet;

class S2CAD_API VObjectManager : private IObserver
{
public:
	typedef std::list< VObjectPtr > VObjectList;

	VObjectManager();
	~VObjectManager();

	int         addObject   ( const VObjectPtr &obj );
	int         removeObject( const VObjectPtr &obj );
	void        removeAllObjects();

	VObjectList objects() const;
	bool        containsObject( const VObjectPtr &obj ) const;
	VObjectPtr  hilightedObject() const;

	void hilightObject( const VObjectPtr &obj );
	void hilightObjectAt( const Math::dvec3 &p );

	void selectObject( const VObjectPtr &obj );
	void selectObjectAt( const Math::dvec3 &p );
	void selectArea( const Math::dvec3 &p0, const Math::dvec3 &p1 );

	void setSelectionPolicy( unsigned int policyMask );
	void unselectObject( const VObjectPtr &obj );
	void clearSelection();

	int objectCount()              const;
	int selectedObjectCount()      const;
	VObjectList  selectedObjects() const;



	Math::box3 boundingBox() const;

	void drawObjects( OpenGL::Renderer *r ) const;

	virtual void notify( IObservable *o );

private:
	typedef std::map<unsigned int, VObjectList> VObjectIndex;	// map from object types to objectList

	SelectionSet       *_selectionSet;
	Math::dvec3         _pickPoint;
	VObjectPtr          _hilightedObject;
	Math::box3          _boundingBox;

	OpenGL::VertexArray _pointsBuffer;
	OpenGL::VertexArray _linesBuffer;
	OpenGL::VertexArray _polygonsBuffer;


	VObjectList  _objects;
	VObjectIndex _objectIndex;

	void VObjectManager::drawForSelection() const;

    VObjectList::const_iterator findObject( const VObjectPtr &obj ) const;
	void                        createIndex( const VObjectPtr &obj );
	void                        removeIndex( const VObjectPtr &obj );
	void                        selectArea( const Math::box3 &area );
	void                        updateSelection();
	void                        updateBoundingBox();
	void                        updateBuffers();
};

}

#endif