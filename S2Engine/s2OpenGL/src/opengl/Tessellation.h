// Tesselation.h
//
#ifndef TESSELATION_H
#define TESSELATION_H

#include "s2OpenGL_API.h"


#include "Math/Math.h"

#include <vector>

namespace OpenGL{

/**
 * @param vertices [in out]: array di vertici; questa funzione aggiunge dei
 *        vertici alla fine dell'array in caso di intersezione tra
 *        i segmenti del bordo.
 *
 * @param triangles [out]: array di triangoli; ogni triangolo è costituito
 *        da tre indici che si riferiscono all'array dei vertici.
 *
 * @return true in caso di successo, false in caso di errore
 */

bool S2OPENGL_API Tessellation(std::vector<Math::dvec3> &vertices, std::vector<int> &triangles);

}

#endif