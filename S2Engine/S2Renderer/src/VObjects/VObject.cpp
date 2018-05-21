// VObject.cpp
//
#include "VObject.h"

#include "VObjectManager.h"

static unsigned int IDPool = 0;


VStyle  VObject::_defaultStyle;

// ------------------------------------------------------------------------------------------------
VObject::VObject()
: _manager(0)
, _selected(false)
, _hilighted(false)
, _styled(false)
, _selectionEnabled(true)
, _visible(true)
, _name("")
{
	_id = IDPool++;
}

// ------------------------------------------------------------------------------------------------
void VObject::setDefaultStyle( const VStyle &style )
{
	_defaultStyle = style;
}

// ------------------------------------------------------------------------------------------------
void VObject::setStyle( const VStyle &s )
{
	_style  = s;
	_styled = true;
}


// ------------------------------------------------------------------------------------------------
void VObject::setName( const std::string &name )	{ _name             = name; }
void VObject::enableSelection( bool on )            { _selectionEnabled = on;   }

// ------------------------------------------------------------------------------------------------
void VObject::select( bool on )  { _selected  = _selectionEnabled && on; }
void VObject::hilight( bool on)  { _hilighted = _selectionEnabled && on; }

// ------------------------------------------------------------------------------------------------
void VObject::setSelected( bool on )
{ 
	_selected = _selectionEnabled && on; 

	if( _manager )
		_manager->notifyObjectSelected( shared_from_this() );
}

// ------------------------------------------------------------------------------------------------
void VObject::setHilighted( bool on )
{ 
	_hilighted = _selectionEnabled && on; 
	
	if( _manager )
		_manager->notifyObjectHilighted( shared_from_this()  );
}

// ------------------------------------------------------------------------------------------------
//void VObject::setSelectionColor( const OpenGL::Color &c )	{ _selectionColor = c;   }
//void VObject::setHilightColor( const OpenGL::Color &c )		{ _hilightColor   = c;   }
void VObject::setManager( VObjectManager *mgr )             { _manager        = mgr; }


// ------------------------------------------------------------------------------------------------
unsigned int VObject::id() const                { return _id;}
std::string VObject::name() const           	{ return _name; }
bool VObject::isSelected() const	            { return _selected;}
bool VObject::isHilighted() const               { return _hilighted;}
bool VObject::isVisible() const				    { return _visible; }
bool VObject::isSelectionEnabled() const		{ return _selectionEnabled; }
Math::box3 VObject::boundingBox() const { return _boundingBox; }
VObject::ObjectType VObject::type() const		{ return Unknown; }

// ------------------------------------------------------------------------------------------------
void VObject::draw( OpenGL::Renderer *r ) const {}
void VObject::draw( OpenGL::Renderer *r,  const VStyle &style ) const 
{
	VStyle backup = _style;
	
	_style = style;
	draw(r);
	_style = backup;	
}

// ------------------------------------------------------------------------------------------------
void VObject::drawForSelection( OpenGL::Renderer *r ) const {}

// ------------------------------------------------------------------------------------------------
void VObject::setColor( const OpenGL::Color &c )
{
	VStyle style;
	style.setBaseColor(c);

	setStyle(style);
}


// ------------------------------------------------------------------------------------------------
VStyle &VObject::style() const
{
	return _styled ? _style : _defaultStyle;
}

// ------------------------------------------------------------------------------------------------
OpenGL::Color VObject::color() const
{
	OpenGL::Color c;

	switch( ((int)_selected)<<1 | (int)_hilighted )
	{
	case 0:	c = style().baseColor();				    break;
	case 1:	c = style().baseColor().lighter(0.25);		break;	// hilighted color
	case 2: c = style().selectedColor();			    break;	// selected color
	case 3:	c = style().selectedColor().lighter(0.25);	break;	// selected and hilighted
	}

	return c;
}

// ------------------------------------------------------------------------------------------------
bool VObject::intersects( const Math::box3 &b )  const
{
	return false;
}

// ------------------------------------------------------------------------------------------------
void VObject::copyDataIn( VObject* newObj ) const
{
	//memcpy( (void*)newObj, (void*)this, sizeof(VObject) );
	newObj->_id= _id;
	newObj->_selected= false;
	newObj->_hilighted= false;
}