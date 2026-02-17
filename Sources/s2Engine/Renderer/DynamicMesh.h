// DynamicMesh.h
//
#ifndef S2_RENDERER_DYNAMICMESH_H
#define S2_RENDERER_DYNAMICMESH_H

#include "s2Engine_API.h"

#include "RenderCore/VertexData.h"

namespace s2 {
namespace Renderer {

class S2ENGINE_API DynamicMesh
{
//public:
//    // Crea una mesh dinamica con capacità massima
//    DynamicMesh( uint32_t maxVertices, uint32_t maxIndices = 0 );
//
//    // Aggiorna i vertici (fino a maxVertices)
//    void updateVertices( const std::vector<Math::fvec3>& vertices );
//    void updateColors( const std::vector<Color>& colors );
//    void updateNormals( const std::vector<Math::fvec3>& normals );
//    void updateIndices( const std::vector<uint32_t>& indices );
//
//    // Accesso diretto ai buffer per aggiornamenti parziali
//    void updateVerticesPartial( const Math::fvec3* vertices, uint32_t count, uint32_t offset = 0 );
//
//    RenderCore::VertexDataPtr getVertexData() const { return _vertexData; }
//
//    uint32_t vertexCount() const { return _currentVertexCount; }
//    uint32_t maxVertexCount() const { return _maxVertices; }
//
//private:
//    RenderCore::VertexDataPtr _vertexData;
//    uint32_t _maxVertices;
//    uint32_t _maxIndices;
//    uint32_t _currentVertexCount = 0;
//    uint32_t _currentIndexCount = 0;
};

} // namespace Renderer
} // namespace s2



#endif // !S2_RENDERER_DYNAMICMESH_H