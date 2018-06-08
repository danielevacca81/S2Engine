// VObjectManager.h
//
#ifndef VOBJECTMANAGER_ONCE
#define VOBJECTMANAGER_ONCE

#include "s2Renderer_API.h"

#include "Math/Math.h"
#include "Math/Box.h"

#include "VObject.h"

#include <map>
#include <vector>
#include <list>

namespace s2 {
namespace Renderer {

class SelectionSet;

class S2RENDERER_API VObjectManager
{
public: 
	enum SelectionPolicy 
	{ 
		SelectionNone     = 0x0000, 
		SelectionSingle   = 0x0001, 
		SelectionMultiple = 0x0002, 
		SelectionNotEmpty = 0x0010,
		SelectionDefault  = SelectionMultiple,
	};

private:
	typedef std::list< VObjectPtr >             VObjectList;
	typedef std::map<unsigned int, VObjectList> VObjectIndex;	// map from object types to objectList

	OpenGL::Renderer   *_renderer; // not owned
	SelectionSet       *_selectionSet;
	Math::dvec3 _pickPoint;
	VObjectPtr          _hilightedObject;


	VObjectList  _objects;
	VObjectIndex _objectIndex;

	void VObjectManager::drawForSelection() const;
	void createIndex( VObjectPtr obj );
	void removeIndex( VObjectPtr obj );
	VObjectList::const_iterator findObject( unsigned int id ) const;
	void hilightObject( VObjectPtr obj );
	void selectArea( const Math::box3 &area );
	void updateSelection();

	void notifyObjectSelected( const VObjectPtr &obj );
	void notifyObjectHilighted( const VObjectPtr &obj );

	friend class VObject;

public:
	explicit VObjectManager( OpenGL::Renderer *r );
	~VObjectManager();

	bool addObject( VObject *obj );
	bool addObject( VObjectPtr obj );

	std::vector<VObjectPtr> getObjects() const;
	VObjectPtr getObject( unsigned int id ) const;
	VObjectPtr getHilightedObject() const;

	void hilightObject( int id );
	void hilightObjectAt( const Math::dvec3 &p );
	void selectObjectAt( const Math::dvec3 &p );
	void setSelectionPolicy( unsigned int policyMask );
	void selectObject( int id );
	void unselectObject( int id );
	void selectArea( const Math::dvec3 &p0, const Math::dvec3 &p1 );
	void clearSelection();
	
	unsigned int objectCount() const;
	unsigned int selectedObjectCount() const;
	std::vector<unsigned int> getSelectedObjects()const; // TODO: return std::vector<VObject*>

	void removeObject( unsigned int id );
	void removeAllObjects();
	

	Math::box3 boundingBox() const;

	void drawObjects() const;
};

}}
#endif