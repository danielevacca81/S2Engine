// VObjectManager.h
//
#ifndef VOBJECTMANAGER_ONCE
#define VOBJECTMANAGER_ONCE

#include "s2Scene_API.h"

#include "VObject.h"
#include "SelectionSet.h"

#include "Core/Observer.h"
#include "Core/Math.h"
#include "Core/Box.h"

#include "s2Renderer/Surface.h"
#include "s2Renderer/PrimitiveBuffer.h"

#include <map>
#include <unordered_map>
#include <vector>
#include <list>

namespace Scene {

class S2SCENE_API VObjectManager : public Observer, std::enable_shared_from_this<VObjectManager>
{
public: 
	typedef std::list< VObjectPtr >              VObjectList;
	typedef std::map<unsigned int, VObjectList>  VObjectIndex;	// map from object types to objectList
	//typedef std::unordered_map<unsigned int, Renderer::PrimitiveBufferPtr> VObjectPrimitive;

public:
	VObjectManager();
	~VObjectManager();

	// void                 setView...

	bool                    addObject( const VObjectPtr &obj );
	void                    removeObject( const VObjectPtr &obj );
	void                    removeAllObjects();

	void                    enableSelection( bool enable );
	void                    enableHilight( bool enable );
	void                    setVisible( bool enable );

	void                    hilightObject( const VObjectPtr &obj );
	VObjectPtr              hilightObjectAt( const Math::ivec2 &screenCoord );
	void                    selectObject( const VObjectPtr &obj, bool select = true );
	VObjectPtr              selectObjectAt( const Math::ivec2 &screenCoord, bool select = true );
	std::vector<VObjectPtr> selectObjectsInArea( const Math::ivec2 &p0, const Math::ivec2 &p1 );

	void                    setSelectionPolicy( const SelectionSet::Policy &policyMask );
	void                    clearSelection();
	void                    clearHilight();

	VObjectPtr              objectByID( unsigned int id ) const;
    VObjectPtr              findObjectData( const VObject::ObjectData &data  ) const;
	std::vector<VObjectPtr> objects()                     const;
	VObjectPtr              hilightedObject()     const;
	unsigned int            objectCount()         const;
	unsigned int            selectedObjectCount() const;
	std::vector<VObjectPtr> selectedObjects()     const;
    bool                    isSelectionEnabled()  const;
    bool                    isHilightEnabled()    const;
    bool                    isVisible()           const;
	bool                    isEmpty()             const;
	Math::box3              boundingBox()         const;
	SelectionSet::Policy    selectionPolicy()     const;

	void draw( const Renderer::SurfacePtr &surface, const Renderer::DrawingState &drawState ) const;
	void notify( Observable *o, const std::any &message ) override;

private:
    VObjectList                 cullObjects() const;
    VObjectList                 sortObjects(const VObjectList &) const;
    void                        drawForSelection() const;
	void                        createIndex( const VObjectPtr &obj );
	void                        removeIndex( const VObjectPtr &obj );
	VObjectList::const_iterator findObjectIndex( const VObjectPtr &obj ) const;
	
	std::vector<VObjectPtr>     selectArea( const Math::Rectangle &area );
	void                        updateSelection();

	VObjectPtr                  extractObjectAt( const Math::ivec2 &screenCoords );

	void                        updateBuffers();

private:
	Renderer::SurfacePtr         _surface;
	
	Renderer::PrimitiveBufferPtr _pointsBuffer;
	Renderer::PrimitiveBufferPtr _polylinesBuffer;
	//Renderer::PrimitiveBufferPtr _polygonsBuffer;
	std::vector<Renderer::PrimitiveBufferPtr> _meshesBuffers;

	SelectionSet        _selectionSet;
	
	VObjectPtr          _hilightedObject; // weakPtr?

    bool                _selectionEnabled;
    bool                _hilightEnabled;
    bool                _visible;

	bool                _needUpdate;

	VObjectList         _objects;
	VObjectIndex        _objectIndex;

	//friend class VObject;
};

}
#endif