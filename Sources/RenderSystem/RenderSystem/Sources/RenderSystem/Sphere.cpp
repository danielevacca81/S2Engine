#include <math.h>
#include "Sphere.h"

Sphere::Sphere( RenderDevice* pRenderDevice, float radius, DWORD slices )
{
	m_dwSlices = slices;
	m_fRadius = radius;

	// Create VB
	//--
	m_pVertexBuffer = pRenderDevice->CreateVertexBuffer(
		2*(m_dwSlices+1)*m_dwSlices,
		RSVF_XYZ | RSVF_TEXCOORD | RSVF_NORMAL, 
		false );

	// Fill VB (build triangle stripes)
	// I know we could use fans at the poles to reduce
	// vertex count but I don't care for this sample.
	//--
	DWORD i=0, j=0;
	float Phi=0, Theta=2*3.1415926f;
	float PhiInc   = 3.1415926f/m_dwSlices;
	float ThetaInc = -2*3.1415926f/m_dwSlices;
	float Pos[3]; // Position of current point
	float Norm;   // Norm of position vector (used to compute normal)

	m_pVertexBuffer->Lock();

	for ( i=0; i<m_dwSlices; i++ )
	{
		Theta = 0;

		for ( j=0; j<=m_dwSlices; j++ )
		{			
			// Position and normal computation for 1st point of strip
			//--
			Pos[0] = radius*cosf(Theta)*sinf(Phi);
			Pos[1] = radius*cosf(Phi);
			Pos[2] = radius*sinf(Theta)*sinf(Phi);
			Norm   = sqrtf( Pos[0]*Pos[0] + Pos[1]*Pos[1] + Pos[2]*Pos[2] );

			m_pVertexBuffer->Normal( Pos[0]/Norm, Pos[1]/Norm, Pos[2]/Norm  );
			m_pVertexBuffer->TexVertex( Theta/(2*3.1415926f), Phi/3.1415926f );
			m_pVertexBuffer->Vertex( Pos[0], Pos[1], Pos[2] );

			// Position and normal computation for 2nd point of strip
			//--
			Pos[0] = radius*cosf(Theta)*sinf(Phi+PhiInc);
			Pos[1] = radius*cosf(Phi+PhiInc);
			Pos[2] = radius*sinf(Theta)*sinf(Phi+PhiInc);
			Norm   = sqrtf( Pos[0]*Pos[0] + Pos[1]*Pos[1] + Pos[2]*Pos[2] );

			m_pVertexBuffer->Normal( Pos[0]/Norm, Pos[1]/Norm, Pos[2]/Norm  );
			m_pVertexBuffer->TexVertex( Theta/(2*3.1415926f), (Phi+PhiInc)/3.1415926f );
			m_pVertexBuffer->Vertex( Pos[0], Pos[1], Pos[2] );

			Theta += ThetaInc;
		}

		Phi += PhiInc;
	}

	m_pVertexBuffer->Unlock();
}

Sphere::~Sphere()
{
	// Release VB memory
	//--
	SAFE_DELETE( m_pVertexBuffer );
}

void Sphere::Draw( RenderDevice* pRenderDevice )
{
	DWORD i=0;
	
	for ( i=0; i<m_dwSlices; i++ )
	{
		pRenderDevice->DrawPrimitives(
			RSPT_TRIANGLESTRIP,
			m_pVertexBuffer,
			i*2*(m_dwSlices+1),
			2*m_dwSlices );
	}

}