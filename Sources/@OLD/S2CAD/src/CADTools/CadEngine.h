// CadEngine.h
//
#ifndef CADENGINE_ONCE
#define CADENGINE_ONCE

#include "Framework_Exports.h"


#include "SnapEngine.h"
#include "CadSettings.h"

#include "VObjects/VObjectManager.h"

#include "Math/Math.h"

#include <map>
#include <vector>


namespace OpenGL{
	class Renderer;
}

class CadTool;

typedef void (*ActivateToolCB)(void* o, int toolID );
typedef void* ToolMan;

class FWK_API CadEngine
{
	friend class CadTool;
public:
	typedef std::map<unsigned int,CadTool*> CadTools;

	enum CommandType{ Undefined, Create, Destroy, Select, StartEdit, Edit, Undo, Redo, Measure };
	enum PrimitiveType{ None, Point, Line, PolyLine, Polygon, Triangle, Quad, Circle };
	
	struct Command
	{
		PrimitiveType                       primitive;
		VStyle                              color;
		CommandType			                command;
		std::vector< Math::dvec3 >	points;
		std::string		                    attributes;
		int	                                target;
		VObjectPtr                          object;
		bool			                    undoredo;

		// --------------------------------------
		Command()
		{
			primitive = None;
			command   = Undefined;
			target    = -1;
			object    = 0;
			undoredo  = false;
		}

		// --------------------------------------	
		Command inverse() const
		{
			Command c = *this;
			switch( this->command )
			{
				case Create:  c.command = Destroy; break;
				case Destroy: c.command = Create; break;
			}
			return c;
		}
	};

private:
	double _worldSize;
	double _pixelSize;
	
	bool            _rubberband;
	CadTools        _tools;
	CadTool        *_currTool;
	VObjectManager *_objManager;
	SnapEngine      _snapEngine;
	CadSettings     _settings;
	ActivateToolCB  _activateTool;
	ToolMan			_toolMan;
	
	std::vector<Command> _commands;
	std::list<Command> _undo;
	std::list<Command> _redo;

	void createGeometry ( const Command &c );
	void selectGeometry ( const Command &c );
	void startEdit      ( const Command &c );
	void destroyGeometry( const Command &c );
	void editGeometry   ( const Command &c );
	
	void setAttributes( VObjectPtr obj, const Command &c );
	int getToolFromObject( VObjectPtr obj ) const; 

	VObjectPtr createObject( const Command &c );

public:
	CadEngine( VObjectManager *objManager );
	~CadEngine();

	void setDefaultSettings();

	int      addTool( CadTool *t );
	CadTool *removeTool( unsigned int toolID );
	void     destroyTool( unsigned int toolID );
	void     destroyTools();
	//void     setCurrentTool( unsigned int toolID );

	std::vector<unsigned int> getSelectedObjects() const;
	VObjectPtr CadEngine::getObject( unsigned int id ) const;

	//void hilightObject   ( const Math::dvec3 &p );
	void startRubberBand ( const Math::dvec3 &p );
	void endRubberBand   ( const Math::dvec3 &p );
	void setCurrentPoint ( const Math::dvec3 &p );
	void confirmTool();

	void updateSizes( double worldSize, double pixelSize );
	void issueCommand( const Command &c, bool queued = false );
	void executeTool();
	void executeCommands();
	void executeCommand( const Command &c );
	void draw( OpenGL::Renderer *r );

	void undo();
	void redo();
	void clear();
	void deleteHistory( bool redoOnly= false );

	
	void   activateGUITool( int toolID ); /// Activate GUI Tool (calls activateTool callback, witch shall call enableTool)	
	void   activateTool( int toolID ); /// Enable Tool

	bool   isEnabled()         const;
	double pixelSize()         const { return _pixelSize; }
	double worldSize()         const { return _worldSize; }
	CadTools tools()           const { return _tools; }
	CadTool  *tool()           const;
	CadTool  *tool(int toolID) const;
	CadSettings settings()     const { return _settings; }

	// --------------------------------------------------------------------------------------------
	template<typename T>
	CadTool  *toolByType() const
	{
		CadTool  *t = 0;
	
		for( CadTools::const_iterator it = _tools.begin();
			 it != _tools.end();
			 ++it )
		{
			if( dynamic_cast<T*>(it->second) )
			{
				t = it->second;
				break;
			}
		}
		
		return t;
	}

	// --------------------------------------------------------------------------------------------
	template<typename T>
	void   activateTool() const
	{
		//CadTool  *t = 0;

		for( CadTools::const_iterator it = _tools.begin();
			it != _tools.end();
			++it )
		{
			if( dynamic_cast<T*>(it->second) )
			{
				activateTool(it->first);
				break;
			}
		}
	}


	// --------------------------------------------------------------------------------------------
	void setActivateTool( ActivateToolCB cb, ToolMan tm ) { _activateTool= cb; _toolMan= tm; }
};

#endif