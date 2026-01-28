// VObject.cpp
//
#include "VObject.h"

static unsigned int IDPool = 0;

using namespace s2;

// ------------------------------------------------------------------------------------------------
VObject::VObject()
: _name("")
{
	_id = IDPool++;
}

// ------------------------------------------------------------------------------------------------
VObject::~VObject()
{}

// ------------------------------------------------------------------------------------------------
bool VObject::operator==( const VObjectPtr &other )
{
	return _id == other->_id;
}

// ------------------------------------------------------------------------------------------------
void VObject::setName( const std::string &name )
{ 
	_name = name;
	notifyObservers();
}

// ------------------------------------------------------------------------------------------------
void VObject::setState( const State &state )
{
	_state = state;
	notifyObservers();
}

// ------------------------------------------------------------------------------------------------
unsigned int VObject::id()          const { return _id;}
std::string VObject::name()         const { return _name; }
Math::box3 VObject::boundingBox()   const { return _boundingBox; }
VObject::ObjectType VObject::type() const { return Unknown; }

// ------------------------------------------------------------------------------------------------
void VObject::draw( OpenGL::Renderer *r ) const 
{}

// ------------------------------------------------------------------------------------------------
bool VObject::intersects( const Math::box3 &b )  const
{
	return false;
}