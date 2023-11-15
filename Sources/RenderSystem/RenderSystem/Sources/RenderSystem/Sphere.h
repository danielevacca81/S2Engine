#ifndef _Sphere_h_
#define _Sphere_h_

#include "RenderDevice.h"
#include "VertexBuffer.h"

/// This class is only a helper to fill a vertex buffer. In a real engine,
/// your sphere class could have many other responsabilities such as to render
/// by itself, its own transformation matrix, ...
class Sphere 
{
public:
	Sphere( RenderDevice* pRenderDevice, float radius, DWORD slices );
	~Sphere();
	void Draw( RenderDevice* pRenderDevice );

public:
	VertexBuffer*	m_pVertexBuffer;
	float m_fRadius;
	DWORD m_dwSlices;
};

#endif
