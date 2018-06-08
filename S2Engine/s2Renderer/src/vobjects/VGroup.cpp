// VGroup.cpp
// 
#include "VGroup.h"

#include "OpenGL/Renderer.h"

using namespace s2;
using namespace s2::Renderer;

// ------------------------------------------------------------------------------------------------
VGroup::VGroup()
{}

// ------------------------------------------------------------------------------------------------
VGroup::~VGroup()
{}

// ------------------------------------------------------------------------------------------------
void VGroup::addObject( const VObjectPtr &obj )
{
	_objects[ obj->id() ] = obj;
	
	_boundingBox.extend( obj->boundingBox() );
}

// ------------------------------------------------------------------------------------------------
std::map<unsigned int, VObjectPtr> &VGroup::objects()
{
	return _objects;
}

// ------------------------------------------------------------------------------------------------
void VGroup::select( bool on )
{
	VObject::select(on);

	for( auto it = _objects.begin();
		it != _objects.end();
		++it )
	{
		it->second->setSelected(_selected);
	}
}

// ------------------------------------------------------------------------------------------------
void VGroup::hilight( bool on)
{
	VObject::hilight(on);

	for( auto it = _objects.begin();
		it != _objects.end();
		++it )
	{
		it->second->setHilighted(_hilighted);
	}
}

// ------------------------------------------------------------------------------------------------
void VGroup::setName( const std::string &name )
{
	VObject::setName(name);
}

// ------------------------------------------------------------------------------------------------
void VGroup::setSelected( bool on )
{
	VObject::setSelected(on);
	
	for( auto it = _objects.begin();
		it != _objects.end();
		++it )
	{
		it->second->setSelected(on);
	}
}

// ------------------------------------------------------------------------------------------------
void VGroup::setHilighted( bool on )
{
	VObject::setHilighted(on);

	for( auto it = _objects.begin();
		it != _objects.end();
		++it )
	{
		it->second->setHilighted(on);
	}
}

// ------------------------------------------------------------------------------------------------
void VGroup::setStyle( const VStyle &s)
{
	VObject::setStyle(s);

	for( auto it = _objects.begin();
		it != _objects.end();
		++it )
	{
		it->second->setStyle(s);
	}
}

// ------------------------------------------------------------------------------------------------
void VGroup::setColor( const OpenGL::Color &c )
{
	VObject::setColor(c);

	for( auto it = _objects.begin();
		it != _objects.end();
		++it )
	{
		it->second->setColor(c);
	}
}

// ------------------------------------------------------------------------------------------------
void VGroup::enableSelection( bool enable )
{
	VObject::enableSelection(enable);

	for( auto it = _objects.begin();
		it != _objects.end();
		++it )
	{
		it->second->enableSelection(enable);
	}
}

// ------------------------------------------------------------------------------------------------
std::vector<Math::dvec3> VGroup::snapPoints() const
{
	std::vector<Math::dvec3> ret;
	for( auto it = _objects.begin();
		it != _objects.end();
		++it )
	{
		auto pts = it->second->snapPoints();

		ret.insert(ret.end(), pts.begin(),pts.end() );
	}
	return ret;
}

// ------------------------------------------------------------------------------------------------
Math::dvec3 VGroup::center() const
{
	return _boundingBox.center();
}

// ------------------------------------------------------------------------------------------------
void VGroup::draw( OpenGL::Renderer *r ) const
{
	for( auto it = _objects.begin();
		 it != _objects.end();
		 ++it )
	{
		it->second->draw(r);
	}
}

// ------------------------------------------------------------------------------------------------
void VGroup::drawForSelection( OpenGL::Renderer *r ) const
{
	for( auto it = _objects.begin();
		it != _objects.end();
		++it )
	{
		it->second->drawForSelection(r);
	}
}

// ------------------------------------------------------------------------------------------------
bool VGroup::intersects( const Math::box3 &b ) const
{
	bool intersection = false;
	for( auto it = _objects.begin();
		it != _objects.end();
		++it )
	{
		if( it->second->intersects(b) )
		{
			intersection = true;
			break;
		}
	}

	return intersection;	
}

// ------------------------------------------------------------------------------------------------
std::vector<Math::dvec3> VGroup::getPoints() const
{
	std::vector<Math::dvec3> ret;
	for( auto it = _objects.begin();
		it != _objects.end();
		++it )
	{
		auto pts = it->second->getPoints();

		ret.insert(ret.end(), pts.begin(),pts.end() );
	}
	return ret;
}

// ------------------------------------------------------------------------------------------------
VObject* VGroup::clone() const
{
	return 0;
}
