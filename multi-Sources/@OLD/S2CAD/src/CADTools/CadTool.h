// CadTool.h
//
#ifndef CADTOOL_ONCE
#define CADTOOL_ONCE

#include "Framework_Exports.h"


#include "Math/Math.h"

#include <map>
#include <string>
#include <vector>

namespace OpenGL{
	class Renderer;
}


class CadEngine;

// Abstract cad tool
class FWK_API CadTool
{
	friend class CadEngine;

public:
	//typedef std::map<std::string, double> Attributes;
	typedef std::map<std::string, std::string> Attributes;

protected:
	enum Status { Idle, Drag, Complete };
	
	Status   _status;

	CadEngine   *_manager;

	std::string  _name;
	int          _id;
	int          _totPoints;
	int          _selIndex;     // index of the selected (working) point [-1 if none]
	int          _nPoints;
	bool         _waitConfirm;
	bool         _completed;
	bool         _enabled;
	int          _target;       // some tools ( like measure or edit tools ) may have target objects 
	Attributes   _attributes;	// attributes

	std::vector<Math::dvec3> _points;
	Math::dvec3              _capturedPoint;
	Math::dvec3              _selectedPoint;

	void chainPoint();

	void setCompleted(bool b)             { _completed = b;}
	void setManager( CadEngine *manager ) { _manager = manager; }
	void setID(unsigned int id)           { _id = id;}
	int	 setPoint(int index, const Math::dvec3 &p,bool force );
	int	 getClosestPoint(const Math::dvec3 &p, int pxlThreshold ) const;
	
	virtual int selectionThreshold() const;
	virtual int collapseThreshold()  const;
	virtual int discardThreshold()   const;

	virtual void drawControlPoints( OpenGL::Renderer *r ) const;
	virtual void applyStyle() const;

public:
	CadTool( const std::string &name, int numPoints );
	virtual ~CadTool();

	void setEnabled( bool on ) { _enabled = on; _selIndex = -1; }
	bool isEnabled() const     { return _enabled; }

	// -----------------------------------------------------------------------------------------------
	bool isRunning() const
	{
		return 
		((_status == Idle||_status == Complete) && (_nPoints == _totPoints) )
		|| _waitConfirm
		|| (_nPoints > 0 && _nPoints <= _totPoints);
	}

	int  id()                                           const   { return _id; }
	bool isCompleted()                                  const	{ return (_status == Complete);}
	int  collectedPointCount()							const	{ return _nPoints; }
	int  selectedPointID()								const   { return _selIndex; }
	Math::dvec3 selectedPoint()                 const;
	std::string name()									const   { return _name; }	
	std::vector<Math::dvec3> &getPoints()				{ return _points;  }

	void setAttributes( const std::string &attr );                         /// Set attributes by formatted string "key1=value1;key2=value2"
	void setAttribute( const std::string &key, const std::string &value ); /// Set attribute by key and value	
	std::string attributes() const;
	std::string attribute( const std::string &key ) const;
	
	void setWaitConfirm(bool b)       { _waitConfirm = b;  }
	bool isWaitingConfirm()		const {return _waitConfirm;}
	void confirm();
	
	bool hasPoints() const { return _nPoints>0; }
	void setPoint( int index, const Math::dvec3 &point );
	void collectPoint(const Math::dvec3 &p );
	void dragPoint( const Math::dvec3 &p );
	void done( const Math::dvec3 &p );
	void done();

	void setTarget( int t );
	int target() const;
	bool hasTarget() const;
	
	virtual void execute()                      = 0;
	virtual void draw( OpenGL::Renderer *r )    = 0;
	virtual bool canHilight() const             = 0;
	virtual bool canSnap() const                = 0;
	virtual bool canChainPoints() const         = 0;
	virtual bool canStayOnScreen() const        = 0;
	virtual bool canOverrideSingleClick() const = 0;
	virtual bool canCollectPoints() const     { return _totPoints > 0; }

	virtual void validate(){}
	virtual void updatePreview() {}
	virtual void reset();
};


#endif