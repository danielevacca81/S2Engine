// VObject.cpp
//
#include "VObject.h"

#include "VObjectManager.h"


using namespace s2;
using namespace s2::SceneGraph;

static uint64_t IDPool = 0;


VObjectStyle  VObject::_defaultStyle;

// ------------------------------------------------------------------------------------------------
VObject::VObject()
: _selected( false )
, _hilighted( false )
, _styled( false )
, _selectable( true )
, _visible( true )
, _name( "" )
, _id( IDPool ++)
{}

// ------------------------------------------------------------------------------------------------
void VObject::setDefaultStyle( const VObjectStyle &style )
{
	_defaultStyle = style;
}

// ------------------------------------------------------------------------------------------------
void VObject::setName( const std::string &name )     { _name       = name; }
void VObject::setSelectable( bool on )               { _selectable = on; }
void VObject::setMatrix( const Math::dmat4 &matrix ) { _matrix     = matrix; }
void VObject::setUserData( const ObjectData &data )  { _userData   = data; }
void VObject::setStyle( const VObjectStyle &s )      { _style  = s; _styled = true; }

// ------------------------------------------------------------------------------------------------
void VObject::setSelected( bool on )
{
	const bool prev = _selected;
	_selected = _selectable && on;
	if( prev ^ _selected )
		notifyObservers( std::make_pair("selected", _selected) );
}

// ------------------------------------------------------------------------------------------------
void VObject::setHilighted( bool on )
{
	const bool prev = _hilighted;
	_hilighted = _selectable && on;
	if( prev ^ _hilighted )
		notifyObservers( std::make_pair("hilighted", _hilighted) );
}

// ------------------------------------------------------------------------------------------------
uint64_t                      VObject::id()           const { return _id; }
std::string                   VObject::name()         const { return _name; }
bool                          VObject::isSelected()   const { return _selected; }
bool                          VObject::isHilighted()  const { return _hilighted; }
bool                          VObject::isVisible()    const { return _visible; }
bool                          VObject::isSelectable() const { return _selectable; }
Math::box3                    VObject::boundingBox()  const { return _boundingBox; }
VObject::ObjectType           VObject::type()         const { return Unknown; }
VObjectStyle                 &VObject::style()        const { return _styled ? _style : _defaultStyle; }
bool                          VObject::hasStyle()     const { return _styled; }
//std::weak_ptr<VObjectManager> VObject::manager()      const { return _manager; }
VObject::ObjectData           VObject::userData()     const { return _userData; }
Math::dmat4                   VObject::matrix()       const { return _matrix; }

// ------------------------------------------------------------------------------------------------
void VObject::draw( const Renderer::SurfacePtr &surface, const Renderer::DrawingState &ds ) const
{}

// ------------------------------------------------------------------------------------------------
Color VObject::color() const
{
	Color c;

	switch( ( (int) _selected ) << 1 | (int) _hilighted )
	{
	case 0:	c = style().baseColor();                     break;
	case 1:	c = style().hilightedColor();                break; // hilighted color
	case 2: c = style().selectedColor();                 break; // selected color
	case 3:	c = style().selectedColor().lighter( 0.25 ); break; // selected and hilighted
	}

	return c;
}

// ------------------------------------------------------------------------------------------------
bool VObject::intersects( const Math::box3 &b )  const
{
	return false;
}

// ------------------------------------------------------------------------------------------------
void VObject::VObjectBuffer::operator+=( const VObjectBuffer &other )
{
	if( !other.indices.empty() )
	{
		indices.push_back( PrimitiveRestartIndex );
		for( auto &i : other.indices )
		{
			if( i == PrimitiveRestartIndex )
				continue;
			
			indices.push_back( i + vertices.size() );
		}
	}
		
	vertices.insert     ( vertices.end(),      other.vertices.begin(),      other.vertices.end() );
	colors.insert       ( colors.end(),        other.colors.begin(),        other.colors.end() );
	normals.insert      ( normals.end(),       other.normals.begin(),       other.normals.end() );
	textureCoords.insert( textureCoords.end(), other.textureCoords.begin(), other.textureCoords.end() );
}
