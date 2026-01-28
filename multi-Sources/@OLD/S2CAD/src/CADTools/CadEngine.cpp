// CadEngine.cpp
//
#include "CadEngine.h"

#include "CadTool.h"

#include "VObjects/VPoint.h"
#include "VObjects/VLine.h"
#include "VObjects/VPolyLine.h"
#include "VObjects/VRectangle.h"
#include "VObjects/VCircle.h"
#include "VObjects/VPolygon.h"
#include "VObjects/VSplineStrip.h"

#include "CadToolCircle.h"
#include "CadToolLine.h"
#include "CadToolPoint.h"
#include "CadToolPolygon.h"
#include "CadToolPolyLine.h"
#include "CadToolRectangle.h"

#include "OpenGL/OpenGL.h"
#include "OpenGL/Renderer.h"

#include <iostream>

#include "Utils/StringTools.h"
#include "Utils/MemCheck.h"

// ------------------------------------------------------------------------------------------------
CadEngine::CadEngine( VObjectManager *objManager )
: _objManager(objManager),
  _currTool(0),
  _pixelSize(1.0),
  _worldSize(1.0),
  _rubberband(false),
  _activateTool(0),
  _toolMan(0)
{
	setDefaultSettings();
}

// ------------------------------------------------------------------------------------------------
CadEngine::~CadEngine()
{
	destroyTools();

	deleteHistory();
}

// ------------------------------------------------------------------------------------------------
void CadEngine::setDefaultSettings()
{
	_settings = CadSettings::defaults();

	// @todo: load from settings
	//_settings.setDefaultObjColor( OpenGL::Color::blue().lighter(.25f) );
	//_settings.setHilightedColor ( OpenGL::Color::yellow().lighter() );
	//_settings.setPreviewColor   ( OpenGL::Color::white() );
	//_settings.setSelectedColor  ( OpenGL::Color::magenta() );
	//_settings.setSnapPointColor ( OpenGL::Color::green().lighter(.25f) );

	//VObject::setHilightColor( _settings.hilightedColor() );
	//VObject::setSelectionColor( _settings.selectedColor() );
	_snapEngine.setColor( _settings.snapStyle().baseColor() );
}

// ------------------------------------------------------------------------------------------------
void CadEngine::clear()
{
	deleteHistory();
	
	_commands.clear();
	_snapEngine.clear();
}

// ------------------------------------------------------------------------------------------------
int CadEngine::addTool( CadTool *tool )
{
	static unsigned int id = 0;

	if( !tool )
		return -1;

	tool->setManager(this);
	tool->setID(id);

	_tools[id] = tool;
	
	return id++;
}

// ------------------------------------------------------------------------------------------------
CadTool *CadEngine::removeTool( unsigned int toolID )
{
	CadTools::const_iterator it = _tools.find( toolID );

	if( it == _tools.end() )      return 0; // invalid tool ?
	if( it->second == _currTool ) return 0; // unable to remove current tool

	CadTool *t = it->second;
	_tools.erase(it);

	return t;
}

// ------------------------------------------------------------------------------------------------
void CadEngine::destroyTool( unsigned int toolID )
{
	CadTools::const_iterator it = _tools.find( toolID );

	if( it == _tools.end() )      return;  // invalid tool ?
	if( it->second == _currTool ) return;  // unable to remove current tool

	CadTool *t = it->second;
	_tools.erase(it);

	delete t;
}

// ------------------------------------------------------------------------------------------------
void CadEngine::destroyTools()
{
	for( CadTools::const_iterator it = _tools.begin();
		 it != _tools.end();
		 ++it )
	{
		delete it->second;
	}

	_tools.clear();
	_currTool = 0;
}

// ------------------------------------------------------------------------------------------------
void CadEngine::draw( OpenGL::Renderer *r )
{
	if( !_currTool )
		return;

	updateSizes( r->worldSizeToPixels(1), r->pixelToWorldSize(1) );

	glPushAttrib( GL_ALL_ATTRIB_BITS );

	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 		

	if( _currTool->hasPoints() )
		_currTool->draw( r );

	if( _currTool->canSnap() )
		_snapEngine.draw( r );

	glPopAttrib();
}

// ------------------------------------------------------------------------------------------------
CadTool *CadEngine::tool( int toolID ) const
{
	CadTools::const_iterator it = _tools.find( toolID );

	// invalid tool ?
	if( it == _tools.end() )	return 0;

	return it->second;
}

// ------------------------------------------------------------------------------------------------
CadTool *CadEngine::tool() const
{
	return _currTool;
}

// ------------------------------------------------------------------------------------------------
std::vector<unsigned int> CadEngine::getSelectedObjects() const
{
	return _objManager->getSelectedObjects();
}

// ------------------------------------------------------------------------------------------------
VObjectPtr CadEngine::getObject( unsigned int id ) const
{
	return _objManager->getObject( id );
}

// ------------------------------------------------------------------------------------------------
// Activate GUI Tool (calls activateTool callback, witch shall call enableTool)
void CadEngine::activateGUITool( int toolID )
{
	CadTools::iterator it = _tools.find( toolID );
	if( it == _tools.end() )
		return;

	if( _activateTool == 0 || _toolMan == 0 )
		return;

	_activateTool( _toolMan, toolID );
}

// ------------------------------------------------------------------------------------------------
void CadEngine::executeTool()
{
	if( _currTool )
	{
		_currTool->execute();
		executeCommands();
	}
}

// ------------------------------------------------------------------------------------------------
void CadEngine::activateTool( int toolID )
{
	_objManager->hilightObject(-1);

	CadTools::iterator it = _tools.find( toolID );

	if( it == _tools.end() )
	{
		//if( _currTool )
		//	_currTool->reset();

		// invalidate tool
		//_currTool = 0;
		if( _currTool )
			_currTool->setEnabled(false);
		return;
	}

	if( it->second->canCollectPoints() )
	{
		_currTool = it->second; // set this tool as current in order to collect points
		_currTool->setEnabled(true);

		if( !_currTool->hasTarget() && !_currTool->hasPoints() )
			_currTool->reset();
	}
	else
	{
		// no points to be collected
		// just execute this tool
		it->second->execute();
		executeCommands();
	}
}

// ------------------------------------------------------------------------------------------------
bool CadEngine::isEnabled() const
{
	if( !_currTool )
		return false;

	return _currTool->isEnabled();
}

// ------------------------------------------------------------------------------------------------
//void CadEngine::hilightObject( const Math::dvec3 &p )
//{
//	if( !isEnabled() )
//		return;
//
//	if( _currTool->canHilight() )
//		_objManager->hilightObjectAt(p);
//}


// ------------------------------------------------------------------------------------------------
void CadEngine::updateSizes( double worldSize, double pixelSize )
{
	_worldSize = worldSize;
	_pixelSize = pixelSize;
}

// ------------------------------------------------------------------------------------------------
void CadEngine::startRubberBand( const Math::dvec3 &p )
{
	if( !isEnabled() )
		return;

	_rubberband = true;

	_snapEngine.updateSnapHint(p,40*_pixelSize);


	Math::dvec3 closestPoint = p;
	
	if( _currTool->canSnap() )
		_snapEngine.getHint(closestPoint);

	if( _currTool->canHilight() )
		_objManager->hilightObjectAt(closestPoint);

	_currTool->collectPoint( closestPoint );
}

// ------------------------------------------------------------------------------------------------
void CadEngine::setCurrentPoint( const Math::dvec3 &p )
{
	if( !isEnabled() )
		return;

	_snapEngine.updateSnapHint(p,40*_pixelSize);

	if( _rubberband )
	{
		Math::dvec3 closestPoint = p;
		
		if( _currTool->canSnap() )
			_snapEngine.getHint( closestPoint );
	
		_currTool->dragPoint( closestPoint );

		if( _currTool->canHilight() )
			_objManager->hilightObjectAt(closestPoint);
	}
}

// ------------------------------------------------------------------------------------------------
void CadEngine::endRubberBand( const Math::dvec3 &p )
{
	if( !isEnabled() )
		return;
	setCurrentPoint(p);
	
	_snapEngine.updateSnapHint(p,40*_pixelSize);

	if( _rubberband )
	{
		Math::dvec3 closestPoint = p;
		
		if( _currTool->canSnap() )
			_snapEngine.getHint(closestPoint);
		
		if( _currTool->canHilight() )
			_objManager->hilightObjectAt(closestPoint);

		_currTool->done(closestPoint);
		_rubberband = false;
	}
}

// ------------------------------------------------------------------------------------------------
void CadEngine::confirmTool()
{
	if( !isEnabled() )
		return;
	
	_rubberband = false;

	_currTool->confirm();
}

// ------------------------------------------------------------------------------------------------
void CadEngine::issueCommand( const Command &c, bool queued )
{
	if( queued ) _commands.push_back( c );
	else		 executeCommand( c );
}

// ------------------------------------------------------------------------------------------------
void CadEngine::executeCommands(  )
{
	for( size_t i=0; i<_commands.size(); ++i )
		executeCommand( _commands[i] );

	_commands.clear();
}

// ------------------------------------------------------------------------------------------------
void CadEngine::executeCommand( const Command &c )
{
	std::cout << "[CAD Engine] Command";

	switch( c.command )
	{
	//
	case Select:	selectGeometry( c );	break;
	case StartEdit:	startEdit( c );	        break;
	// 
	case Create:  createGeometry( c );  break;
	case Destroy: destroyGeometry( c ); break;
	case Edit:		editGeometry( c );		break;
	// Special cases:
	case Undo:		undo(); break;
	case Redo:		redo(); break;
	}
}

// ------------------------------------------------------------------------------------------------
VObjectPtr CadEngine::createObject( const Command &c )
{	
	VObjectPtr obj = nullptr;
	
	switch( c.primitive )
	{
	case Point:    obj = std::make_shared<VPoint>( c.points[0] );                                           std::cout << " CREATE POINT ";    break;	
	case Line:     obj = std::make_shared<VLine>(  c.points[0],c.points[1] );                               std::cout << " CREATE LINE ";     break;
	case PolyLine: obj = std::make_shared<VPolyLine>( c.points );                                         std::cout << " CREATE POLYLINE "; break;	
	//case PolyLine: obj = std::make_shared<VSplineStrip>( c.points,2,8 );									std::cout << " CREATE STRIP ";    break;	
	case Quad:     obj = std::make_shared<VRectangle>( Math::box3::createAABB(c.points[0],c.points[1]));    std::cout << " CREATE QUAD ";     break;
	case Circle:   obj = std::make_shared<VCircle>( c.points[0], Math::distance(c.points[0],c.points[1]) ); std::cout << " CREATE CIRCLE ";   break;
	case Polygon:  obj = std::make_shared<VPolygon>(c.points );                                             std::cout << " CREATE POLYGON ";  break;
	}

	if( obj )
	{
		setAttributes(obj,c);
		obj->setStyle( c.color );
	}
		
	return obj;
}

// ------------------------------------------------------------------------------------------------
void CadEngine::createGeometry( const Command &c )
{	
	bool ret     = false;	
	VObjectPtr obj = nullptr;

	if( c.target >= 0 && !c.undoredo )
	{
		editGeometry(c);
		return;
	}

	if( c.object == nullptr )
	{
		obj = createObject( c );
		ret = _objManager->addObject( obj );
	}
	else // c.object not null
	{
		obj = c.object;
		ret = _objManager->addObject( obj );
	}

	if( obj )
	{
		_snapEngine.addPoints( obj->snapPoints(), obj->id() );
		obj->setSelected(false);

		// Build undo command only if command is not undo or redo
		// This allows undo() and redo() methods to manage the undo and redo lists
		if( !c.undoredo )
		{
			Command undoCommand = c.inverse();
			undoCommand.target  =  obj->id();
			undoCommand.object  = obj;
			_undo.push_back( undoCommand );
		}
	}

	if( !c.undoredo )
	{
		// Delete redo history
		deleteHistory(true);
	}

	std::cout << (ret ? "completed":"failed") << std::endl;
}

// ------------------------------------------------------------------------------------------------
void CadEngine::selectGeometry( const Command &c )
{
	bool ret = false;
	_objManager->selectArea( c.points[0],c.points[1]  );	ret = true; std::cout << " SELECT QUAD ";

	std::cout << (ret ? "completed":"failed") << std::endl;
}

// ------------------------------------------------------------------------------------------------
void CadEngine::destroyGeometry( const Command &c ) 
{
	VObjectPtr obj = nullptr;

	// Build undo command only if command is not undo or redo
	// This allows undo() and redo() methods to manage the undo and redo lists
	if( !c.undoredo )
	{
		obj = _objManager->getObject( c.target );
		if( !obj )
			return;
		Command undoCommand= c.inverse();
		undoCommand.object= obj;
		_undo.push_back( undoCommand );
	}
	else
	{
		obj = c.object;
	}
	
	if( !obj )
		return;

	_snapEngine.removePoints( c.target );
	/*VObject *o = */_objManager->removeObject( obj->id() );

	if( !c.undoredo )
	{
		// Delete redo history
		deleteHistory(true);
	}

	std::cout << " DELETE completed"<< std::endl;
}

// ------------------------------------------------------------------------------------------------
CadEngine::PrimitiveType GetObjectPrimitive( VObject* obj )
{
	if( dynamic_cast<VPoint*>(obj)     ) return CadEngine::Point;
	if( dynamic_cast<VLine*>(obj)      ) return CadEngine::Line;
	if( dynamic_cast<VPolyLine*>(obj)  ) return CadEngine::PolyLine;
	if( dynamic_cast<VRectangle*>(obj) ) return CadEngine::Quad;
	if( dynamic_cast<VCircle*>(obj)    ) return CadEngine::Circle;
	if( dynamic_cast<VPolygon*>(obj)   ) return CadEngine::Polygon;
	return CadEngine::None;
}

// ------------------------------------------------------------------------------------------------
int CadEngine::getToolFromObject( VObjectPtr obj ) const
{
	PrimitiveType primitive = GetObjectPrimitive( obj.get() );
	for( CadTools::const_iterator it= _tools.begin(); it != _tools.end(); ++it )
	{
		if( dynamic_cast<CadToolCircle*>(it->second)    && std::dynamic_pointer_cast<VCircle>(obj)    ) return it->first;
		if( dynamic_cast<CadToolLine*>(it->second)      && std::dynamic_pointer_cast<VLine>(obj)      ) return it->first;
		if( dynamic_cast<CadToolPoint*>(it->second)     && std::dynamic_pointer_cast<VPoint>(obj)     ) return it->first;
		if( dynamic_cast<CadToolPolygon*>(it->second)   && std::dynamic_pointer_cast<VPolygon>(obj)   ) return it->first;
		if( dynamic_cast<CadToolPolyLine*>(it->second)  && std::dynamic_pointer_cast<VPolyLine>(obj)  ) return it->first;
		if( dynamic_cast<CadToolRectangle*>(it->second) && std::dynamic_pointer_cast<VRectangle>(obj) ) return it->first;
		// VSplineStrip
	}
	return -1;
}

// ------------------------------------------------------------------------------------------------
void CadEngine::startEdit( const Command &c ) 
{
	const int toolID = getToolFromObject( _objManager->getObject(c.target) );
	if( toolID < 0 )
		return;

	if( _activateTool && _toolMan )
		_activateTool( _toolMan, toolID ); // <- refresh gui. this also calls a cadtool reset() 
	
	_tools[ toolID ]->setTarget( c.target );
	
	activateTool( toolID );
//	_tools[ toolID ]->done();
}

// ------------------------------------------------------------------------------------------------
void CadEngine::editGeometry( const Command &c ) 
{
	// obj is the object to be changed, while newObj is the new value (ID shall be the same)
	VObjectPtr obj    = _objManager->getObject( c.target );
	VObjectPtr newObj = nullptr;

	if( !c.object ) // Edit from GUI shall build the new object from command, as c.object is null
	{
		newObj = createObject( c );
		obj->copyDataIn( newObj.get() );
	}
	else // while (re)edit via undo/redo has a non-null c.object 
	{
		newObj= c.object;
	}

	if( !obj || !newObj)
		return;

	// Build undo command only if command is not undo or redo
	// This allows undo() and redo() methods to manage the undo and redo lists
	if( !c.undoredo )
	{
		Command undoCommand= c.inverse();
		undoCommand.object= obj;
		_undo.push_back( undoCommand );
	}
	
	// Remove old obj from obj manager
	_snapEngine.removePoints( obj->id() );
	/*VObject *o = */_objManager->removeObject( obj->id() );

	// Add new object to the obj manager
	////setAttributes( newObj, c );
	_objManager->addObject( newObj );
	_snapEngine.addPoints( newObj->snapPoints(), newObj->id() );

	if( !c.undoredo )
	{
		// Delete redo history
		deleteHistory(true);
	}
		
	std::cout << " DELETE completed"<< std::endl;
}

// ------------------------------------------------------------------------------------------------
void CadEngine::setAttributes( VObjectPtr obj, const Command &c )
{
	std::vector<std::string> attributes = split(c.attributes,";");

	for( size_t i=0; i<attributes.size(); ++i )
	{
		if	   ( startsWith(attributes[i], "Name:")  ) { obj->setName( after(attributes[i],":") );}
		else {}
	}

	//obj->setColor( c.color );
}


// ------------------------------------------------------------------------------------------------
void CadEngine::undo()
{
	if( _undo.size() > 0 )
	{
		VObjectPtr oldObj= nullptr;

		// Get latest undo
		Command undo= _undo.back();

		// Get the old object if Edit
		if( undo.command == Edit )
			oldObj = _objManager->getObject( undo.target );

		// Create a redo command, inverse of the last undo command
		Command redo= undo.inverse();

		// Store the old object in the redo if Edit
		if( undo.command == Edit )
			redo.object= oldObj;

		// Push the redo command in the redo command list
		_redo.push_back( redo );
		
		// Remove the undo command
		_undo.pop_back();

		// Set the last undo command as undoredo
		undo.undoredo= true;

		// Execute the last undo command
		executeCommand( undo );
	}
}

// ------------------------------------------------------------------------------------------------
void CadEngine::redo()
{
	if( _redo.size() > 0 )
	{
		VObjectPtr oldObj= nullptr;

		// Get latest redo
		Command redo= _redo.back();

		// Get the old object if Edit
		if( redo.command == Edit )
			oldObj= _objManager->getObject( redo.target );

		// Create an undo command, inverse of the last redo command
		Command undo= redo.inverse();

		// Store the old object in the undo if Edit
		if( redo.command == Edit )
			undo.object= oldObj;

		// Push the redo command in the redo command list
		_undo.push_back( undo );

		// Remove redo command
		_redo.pop_back();
		
		// Set the last redo command as undoredo
		redo.undoredo= true;

		// Execute redo command
		executeCommand( redo );
	}
}

#include <set>
// ------------------------------------------------------------------------------------------------
void CadEngine::deleteHistory( bool redoOnly )
{
	std::set<VObjectPtr> delenda;
	// Destroy redo history
	for( std::list<Command>::iterator it= _redo.begin(); it != _redo.end(); ++it )
	{
		if( it->object )
		{
			if( it->command == Edit || it->command == Create )
				delenda.insert( it->object );
			else if( it->command == Destroy ) // <-- Destroy means object is in the domain
				delenda.erase( it->object );
		}
	}
	_redo.clear();

	// Destroy undo history
	if( !redoOnly )
	{
		for( std::list<Command>::iterator it= _undo.begin(); it != _undo.end(); ++it )
		{
			if( it->object )
			{
				if( it->command == Edit || it->command == Create )
					delenda.insert( it->object );
				else if( it->command == Destroy ) // <-- Destroy means object is in the domain
					delenda.erase( it->object );
			}
		}
		_undo.clear();
	}

	for( std::set<VObjectPtr>::iterator it= delenda.begin(); it != delenda.end(); ++it )
	{
		//delete (*it);
	}
}