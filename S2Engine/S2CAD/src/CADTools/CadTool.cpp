// CadTool.cpp
//
#include "CadTool.h"

#include "CadEngine.h"
#include "VObjects/VObject.h"

#include "OpenGL/OpenGL.h"
#include "OpenGL/Renderer.h"

#include "Utils/StringTools.h"

#include <iostream>


#include "Utils/MemCheck.h"

// ------------------------------------------------------------------------------------------------
CadTool::CadTool( const std::string &name, int numPoints ) 
: _id(-1)
, _target(-1)
, _selIndex(-1)
, _manager(0)
, _name(name)
, _totPoints(numPoints)
, _nPoints(-1)
, _waitConfirm(false)
, _completed(false)
{}

// ------------------------------------------------------------------------------------------------
CadTool::~CadTool()
{}

// ------------------------------------------------------------------------------------------------
int CadTool::selectionThreshold() const { return 64; }
int CadTool::collapseThreshold()  const { return 16; }
int CadTool::discardThreshold()   const { return 64; }

// ------------------------------------------------------------------------------------------------
int CadTool::setPoint( int index, const Math::dvec3 &p,bool force )
{
	int idx = index >=0 ? index : _nPoints;

	if( idx == _totPoints && _totPoints > 0 )
		idx--;

	if( idx < 0 )
		return index<0 ? _nPoints-1 : index;

 	const int closest = getClosestPoint( p, collapseThreshold() );

	if( force       || 
		closest < 0 /*||
   	  ( closest >=0 && 
	    Math::distance(p,_points[closest]) > _manager->pixelSize() * DISCARD_THRESHOLD )*/)
		_points[idx]      = p;

	if( index <0 ) // want to insert a new point
	{

		// insert only if its distance is big enough from other points
		if( force || closest < 0 )
			_nPoints++;

		if( _nPoints > _totPoints )
			_nPoints = _totPoints;
	}

	return index<0 ? _nPoints-1 : index;
}

// ------------------------------------------------------------------------------------------------
void CadTool::setPoint( int index, const Math::dvec3 &point )
{
	if( index < 0 )
		return;
	
	if( _nPoints == 0 )
		_nPoints = _totPoints;

	_points[index] = point;
	_capturedPoint = point;
	_selectedPoint = point;
	_selIndex      = index;
}

// ------------------------------------------------------------------------------------------------
void CadTool::setAttributes( const std::string &attrib )
{
	_attributes.clear();
	const std::vector<std::string> attributes = split( attrib, ";" );

	for( size_t i=0; i<attributes.size(); ++i )
	{
		const std::vector<std::string> attrib = split( attributes[i], "=" );

		if( attrib.size() >= 2 )
			_attributes[ attrib[0] ]= attrib[1];
	}
}

// ------------------------------------------------------------------------------------------------
std::string CadTool::attributes() const
{
	// pack attributes into a single formatted string
	std::string a;
	for( Attributes::const_iterator it = _attributes.begin();
		it != _attributes.end();
		++it )
		a += it->first + "=" + it->second + ";";

	return a;
}
	
// ------------------------------------------------------------------------------------------------
// Set object attribute by key and value
void CadTool::setAttribute( const std::string &key, const std::string &value )
{
	_attributes[ key ]= value;
}


// ------------------------------------------------------------------------------------------------
// Get object attribute value by key
std::string CadTool::attribute( const std::string &key ) const
{
	Attributes::const_iterator it = _attributes.find( key );
	
	if( it != _attributes.end() )
		return it->second;
	
	return std::string("");
}


// ------------------------------------------------------------------------------------------------
// given P and a range, get the closest collected point to P (excluding itself)
int CadTool::getClosestPoint(const Math::dvec3 &p, int pxlThreshold ) const
{
	int k = -1; // return -1 if each point is outside a certain threshold
	
	const double minSize = _manager->pixelSize() * pxlThreshold;
	
	for( int i = 0; i<_nPoints; ++i )
	{
		double size    = Math::distance(p,_points[i]);

		// p is the closest point IF:
		if( size < minSize &&		// is the closest
			i != _selIndex )		// is not itself
			
			k = i;
	}

	return k;
}


// ------------------------------------------------------------------------------------------------
// collect a point
void CadTool::collectPoint(const Math::dvec3 &p)
{
	//std::cout << "COLLECT_POINT" << _status << std::endl;

	_selectedPoint = p;

	if( canStayOnScreen() && _completed )
		reset();

	switch(_status)
	{
	case Drag:
	case Idle:
		if( _nPoints == 0 )
		{
			setPoint(-1,p, false);
			_selIndex      = -1;
		}
		else
		{
			// store prev selected point
			int oldSelection = _selIndex;

			// reset selIndex will allow to select an internal point more than once
			_selIndex = -1;
			_selIndex = getClosestPoint(p,selectionThreshold());

			if( _selIndex >= 0 )
				_capturedPoint = _points[_selIndex];
			else
			{
				// add a new point only if none was selected
				if( oldSelection < 0 )
					setPoint( -1,p,false );
			}
		}
		break;
	
	case Complete:
		break;
	}

	validate();
}

// ------------------------------------------------------------------------------------------------
// Drag selected point if any, insert and drag a new one otherwise
void CadTool::dragPoint(const Math::dvec3& point)
{
	//std::cout << "DRAG_POINT" << std::endl;

	_selectedPoint = point;
	
	if (canStayOnScreen() && _completed)
		reset();

	switch(_status)
	{
	case Idle:
		if( _totPoints == 1 ) // dragging the first point
		{
			_selIndex = setPoint(-1,point,false);
		}
		else if( _nPoints < _totPoints )
		{
			if(_selIndex < 0)
				_selIndex = setPoint(-1,point,true);
			else
			{
				if( Math::distance(_capturedPoint,point) > _manager->pixelSize() * selectionThreshold() )
				{
					collectPoint(point); // 
				}
				else
					setPoint(_selIndex,point,false);
			}
		}
		else if( _nPoints == _totPoints )
		{
			if(_selIndex < 0)
				_selIndex = getClosestPoint( point,selectionThreshold() );
		}

		if( _selIndex >= 0 )
			_capturedPoint = _points[_selIndex];
		else
		{
			_selIndex      = _nPoints-1;
			_capturedPoint = _points[_selIndex];
			//std::cout << "00" << std::endl;
		}

		_status   = Drag;
		break;

	case Drag:
		if( _selIndex >= 0 )
		{
			setPoint(_selIndex,point,false);
			_capturedPoint = _points[_selIndex];
		}

		_status = Drag;
		break;

	case Complete:
		_status = Idle;
		break;
	}

	validate();
}


// ------------------------------------------------------------------------------------------------
// Done!
void CadTool::done(const Math::dvec3 &p)
{
	//std::cout << "DONE_POINT" << std::endl;

	_selectedPoint = p;

	switch(_status)
	{
	case Idle:
		{
			if( Math::isinf( Math::length( _capturedPoint ) ) )
			{
				// already collected all needed points ?
				for(int i=_nPoints;i<_totPoints;++i)
					setPoint(-1,p,canOverrideSingleClick()); // override remaining points (i.e. single click selection tool)

				if( _nPoints == _totPoints && !isWaitingConfirm() )
					_status = Complete;
			}
			else
			{
				// if clicked on a prev point
				if( _selIndex >= 0 )
					return;
			}
		}
		break;

	case Drag:
		{
			_status = Idle;
			
			// discard points only if action can collect more than one
			if( _totPoints > 1 ) //&&   )
			{
				//// if dragging a new point 
				//if( _selIndex < 0 )
				//{
				//	// discard only if is too close to the last one
				//	const double d = Math::distance(_points[_nPoints-1],p);
				//
				//	if( d >= _manager->pixelSize() * discardThreshold() )
				//	{
				//		//if( _selIndex >= 0 )
				//		//	_points[_selIndex] = p; 
				//		//else 
				//		//if( _nPoints > 0 )
				//			_points[_nPoints-1] = p;
				//	}
				//	else
				//	{
				//		// discard p
				//		if( _nPoints > 0 )
				//			_nPoints--;
				//	
				//		if( _nPoints == 1 )
				//			reset();
				//	}
				//}
				//else
				{
					int k = -1;
					// if dragging internal point find 
					for( int i=0; i<_nPoints; ++i )
					{
						const Math::dvec3 pt = _points[i];
						const double d               = Math::distance(pt,p);

						if( d < _manager->pixelSize() * discardThreshold() && i != _selIndex )
						{	
							k=i;
							break;
						}
					}

					if( k>=0 )
					{
						// discard point at _selIndex
						_points.erase( _points.begin() + _selIndex );

						// discard p
						if( _totPoints > _nPoints )
						{   
							_totPoints = _nPoints;
							_nPoints--;
							_selIndex = -1;
							_points.resize( _points.size() -1 );
							
							if( _nPoints == 1 )
								reset();
						}
						else
						{
							_nPoints--;
							_selIndex = -1;
						}
					}
					else
						_points[_selIndex] = p;					
				}
			}
	
			// collected all needed points?
			if( _nPoints == _totPoints || (_nPoints == 1 && canOverrideSingleClick()) )
			{
				if( !isWaitingConfirm() )
					_status = Complete;
			}
		}
		break;

	case Complete:
		break;
	}

	done();
}

// ------------------------------------------------------------------------------------------------
// Done!
void CadTool::done()
{		
	if( canChainPoints() && isWaitingConfirm() )
		chainPoint();
	else if( _status == Complete )
	{
		//_nPoints   = _totPoints;

		validate();
		_completed = true;

		execute();

		if( !canStayOnScreen() )
			reset();
	}
}

//-------------------------------------------------------------------------------------------------
void CadTool::confirm()
{
	_waitConfirm = false;
	_status      = Complete;
	done();
}

//-------------------------------------------------------------------------------------------------
// Reset
void CadTool::reset()
{
	_completed     = false;
	_nPoints       = 0;
	_selIndex      = -1;
	_status		   = Idle;
	_capturedPoint = Math::dvec3( std::numeric_limits<double>::infinity() );
	_target        = -1;
	_selectedPoint = Math::dvec3(0.0);
	
	_points.clear();
	_points.resize(_totPoints);

	for( int i=0;i<_totPoints;++i )
		_points[i]		= Math::dvec3(0.0); 
}

//-------------------------------------------------------------------------------------------------
void CadTool::chainPoint()
{
	_completed = false;
	_status    = Idle;
//	_selIndex  = -1;
	if( _totPoints == _nPoints )
		_totPoints++;

	_points.resize( _totPoints );
	
	if( _selIndex < 0 ) _capturedPoint = Math::dvec3( std::numeric_limits<double>::infinity() );
	else                _capturedPoint = _points[_selIndex];
}

//-------------------------------------------------------------------------------------------------
void CadTool::setTarget( int t )
{
	reset();

	VObjectPtr obj = _manager->getObject( t );

	if( !obj )
		return;

	_target    = t;
	_points    = obj->getPoints();
	_nPoints   = _points.size();
	_totPoints = _points.size();
	_selIndex  = -1;
		
	// when set a target on chaining actions
	// we need to take care of of one more point
	if( canChainPoints() )
		chainPoint();
	
	setAttribute( "Name", obj->name() );
}

//-------------------------------------------------------------------------------------------------
int CadTool::target() const { return _target; }

//-------------------------------------------------------------------------------------------------
bool CadTool::hasTarget() const { return _target >= 0; }

//-------------------------------------------------------------------------------------------------
//Math::dvec3 CadTool::selectedPoint() const { return _selectedPoint; }
Math::dvec3 CadTool::selectedPoint() const { return _capturedPoint; }

//-------------------------------------------------------------------------------------------------
void CadTool::drawControlPoints( OpenGL::Renderer *r ) const
{
	const VStyle style          = _manager->settings().previewStyle();
	const float borderSize      = style.penSize()+style.borderSize();

	r->beginRendering();
	
	
	if( _selIndex >= 0 )
	{
		glPointSize((float)selectionThreshold());
		
		glColor4fv( style.baseColor().transparent() );		 

		std::vector<Math::dvec3> pts;
		pts.push_back( _points[_selIndex] );
		r->setVertex3DArray( pts );

		glDrawArrays(GL_POINTS,0,1);
	}

	r->setVertex3DArray( _points );

	glPointSize(borderSize);
	glColor4fv( style.borderColor() );	
	glDrawArrays(GL_POINTS, 0, _nPoints );

	glPointSize( style.penSize() );
	glColor4fv( style.baseColor() );
	glDrawArrays(GL_POINTS, 0, _nPoints );

	r->endRendering();
}

//-------------------------------------------------------------------------------------------------
void CadTool::applyStyle() const
{
	const VStyle style = _manager->settings().previewStyle();

	glColor4fv( style.baseColor() );
	glLineWidth( style.borderSize() );	
	switch( style.penStyle() )
	{
	case VStyle::SolidPen: break;
	case VStyle::DottedPen:
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(1, 0x000F);
		break;
	case VStyle::DashedPen:
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(1, 0x00FF);
		break;
	}
}