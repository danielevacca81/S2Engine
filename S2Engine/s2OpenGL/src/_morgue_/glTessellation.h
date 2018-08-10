// glTesselation.h
//
#ifndef GLTESSELATION_H
#define GLTESSELATION_H

#include <vector>
#include "Math.h"

//struct IDSReal2D
//{
//	double v[2];
//	double& operator[](int i) { return v[i]; }
//	IDSReal2D() { v[0] = 0; v[1] = 0; }
//	IDSReal2D(double x,double y) { v[0] = x; v[1] = y; }
//};

/**
 * @param vertices [in out]: array di vertici; questa funzione aggiunge dei
 *            vertici alla fine dell'array in caso di intersezione tra
 *            i segmenti del bordo.
 * @param triangles [out]: array di triangoli; ogni triangolo è costituito
 *            da tre indici che si riferiscono all'array dei vertici.
 * @return true in caso di successo, false in caso di errore
 */
namespace OpenGL{

bool glTessellation(std::vector<Math::vec2> &vertices, std::vector<int> &triangles);


}

#endif