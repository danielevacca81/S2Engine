// ArcBall.h
//
// ArcBall class
//
// Thanks to Terence J. Grant for his ArcBall Tutorial
// and to Paolo Cignoni for his TrackBall code
//
//
// Arcball sphere constants:
// Diameter is       2.0f
// Radius is         1.0f
// Radius squared is 1.0f


#pragma once

#include "../math/Matrix.h"
#include "../math/Vector.h"
#include "../math/Quaternion.h"

namespace s2Lib {

class ArcBall
{
	private:
		float		sizeW,sizeH;			// Dimensioni
		int			mouseDownX,mouseDownY;	// Coordinate del click
		
		Quaternion	prevQ,currQ;	// Calcolano le rotazioni
		Vector3		currP,prevP;	// Calcolano le traslazioni
		
		Matrix4		rotMatrix;		// 


		bool	lmbDown;
		bool	rmbDown;

		// Mappa il punto (x,y) su P appartenente alla sfera
		void	mapToSphere(int x, int y,Vector3 &P);
	
	public:
		ArcBall();

		void	resize(int Width,int Height);
		void	reset();

		void	onLMouseDown(int x,int y);
		void	onLMouseUp();

		void	onRMouseDown(int x,int y);
		void	onRMouseUp();

		void	onMouseMove(int x,int y);

		Matrix4	getRotationMatrix()
		{
			return currQ.toMatrix();
			
			//rotMatrix = currQ.toMatrix();
			//rotMatrix.setTransposed();

			//return rotMatrix;
		}

		Vector3		&getPosition()	{return currP;}


		//void	draw(); -- ??
};

////////////////////////////////////
// Costruttore
//
// Inizializza i valori dell'arcBall
// (Default values)
////////////////////////////////////
inline ArcBall::ArcBall()
{
	sizeW = sizeH = 1;

	mouseDownX = mouseDownY = 0;

	rmbDown = false;
	lmbDown = false;

	rotMatrix.setIdentity();
	
	currP.reset();
	prevP.reset();
	
	currQ.reset();
	prevQ.reset();
}


/////////////////////////////////
// Reset
// Ripristina i valori di default
/////////////////////////////////
inline void ArcBall::reset()
{
	mouseDownX = mouseDownY = 0;

	rmbDown = false;
	lmbDown = false;

	rotMatrix.setIdentity();
	
	currP.reset();
	prevP.reset();
	
	currQ.reset();
	prevQ.reset();
}

////////////////////////////////////
// Resize
//
// Adatta le dimensioni dell'arcball
// alle dimensioni dell'area in cui 
// si muove il mouse
////////////////////////////////////
inline void ArcBall::resize(int Width,int Height)
{
	if(Width <= 1)
		Width = 2;

	if(Height <= 1)
		Height = 2;

    sizeW = 1.0f / ((Width  - 1.0f) * 0.5f);
    sizeH = 1.0f / ((Height - 1.0f) * 0.5f);
}



//////////////////////////////////////
// Mappa le coordinate del punto (x,y)
// in coordinate appartenenti alla
// sfera
//////////////////////////////////////
inline void ArcBall::mapToSphere(int x,int y,Vector3 &P)
{
	// Adjust point coords and scale down to range of [-1 ... 1]
	float X =	  x * sizeW - 1;
	float Y = 1 - y * sizeH;
	
	float lenSqr = X*X + Y*Y;

	// Se il punto rimappato e'
	// fuori dalla sfera (lenSqr > r*r)
	if(lenSqr > 1.f)
	{
		float iLen = 1.f /sqrtf(lenSqr);
		
		P.set(X*iLen,Y*iLen,0);
	}
	else
	{
		// Il punto e' all'interno della sfera
		P.set(X,Y,sqrtf(1-lenSqr));
	}
}

////////////////////////////////////
// LeftClick
// Momorizza le coordinate del click
////////////////////////////////////
inline void ArcBall::onLMouseDown(int x,int y)
{
	lmbDown = true;
	mouseDownX = x;
	mouseDownY = y;
}


inline void ArcBall::onLMouseUp()
{
	lmbDown = false;
	prevQ = currQ;
}


////////////////////////////////////
// RightClick
// Momorizza le coordinate del click
////////////////////////////////////
inline void ArcBall::onRMouseDown(int x,int y)
{
	rmbDown = true;
	mouseDownX = x;
	mouseDownY = y;
}

inline void ArcBall::onRMouseUp()
{
	rmbDown = false;
	prevP = currP;
}

//////////////////////////////////////////////////
// OnMouseMove
//
// Processa il movimento del mouse:
//
//  - Se e' premuto il bottone SX ruota l'arcball
//    in funzione delle coordinate attuali
//
//  - Se e' premuto il bottone DX trasla l'arcball
//    in funzione delle coordinate attuali
//////////////////////////////////////////////////
inline void ArcBall::onMouseMove(int x,int y)
{
	if(lmbDown)	// Rotatzione?
	{
		Vector3 start,end,cross;
		
		// Converto le coordinate del mouse
		// in punti appartenenti alla sfera
		mapToSphere(mouseDownX,mouseDownY,start);
		mapToSphere(x,y,end);

		cross = start^end;

		// Se e' stato effettuato un movimento
		// devo calcolare la rotazione
		if(cross.getLengthSqr() > EPS)
		{
			currQ.set(cross[0],cross[1],cross[2],start|end);
			currQ.normalize();
		}
		else
		{
			// Non e' stato effettuato alcun movimento
			// Rotazione nulla
			currQ.reset();
		}

		// Concateno la rotazione corrente con
		// la rotazione precedente
		currQ *= prevQ;
	}
	else if(rmbDown)	// Traslazione ?
	{
		// Se e' stato effettuato un movimento
		// la traslazione non e' nulla
		if (x != mouseDownX || y != mouseDownY)
		{
			currP[0] = (x-mouseDownX)*sizeW;
			currP[1] = (y-mouseDownY)*sizeH;

			// Concateno la traslazione corrente con
			// la traslazione precedente
			currP += prevP;
		}
	}
}


}	// End of namespace