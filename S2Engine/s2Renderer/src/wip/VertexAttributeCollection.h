// VertexAttributeCollection.h
//
#ifndef VERTEXATTRIBUTECOLLECTION_ONCE
#define VERTEXATTRIBUTECOLLECTION_ONCE

#include "s2Renderer_API.h"

#include "VertexAttribute.h"
#include "VertexAttributeType.h"

#include <map>
#include <string>

//namespace Renderer {
//
//class S2RENDERER_API VertexAttributeCollection
//{
//public:
//	void add   ( const VertexAttribute &vertexAttribute );
//	bool remove( const VertexAttribute &item );
//	bool remove( const std::string &vertexAttributeName );
//	void clear ();
//
//	bool contains( const VertexAttribute &vertexAttribute ) const;
//	bool contains( const std::string &vertexAttributeName ) const;
//	int  size() const;
//
//    //bool ICollection<VertexAttribute>.IsReadOnly
//    //{
//    //    get { return false; }
//    //}
//
//
//
//    //public IEnumerator<VertexAttribute> GetEnumerator()
//    //{
//    //    ICollection<VertexAttribute> values = m_collection.Values;
//    //    return values.GetEnumerator();
//    //}
//
//    VertexAttribute operator []( const std::string &vertexAttributeName ) const
//    {
//		return _collection.at( vertexAttributeName );
//    }
//
//private:
//	std::map< std::string, VertexAttribute > _collection;  
//};

//}
#endif