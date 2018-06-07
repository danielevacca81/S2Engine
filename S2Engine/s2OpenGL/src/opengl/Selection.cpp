// Selection.cpp
//
#include "Selection.h"

#include "OpenGL.h"

#include <iostream>

using namespace s2;

using namespace s2::OpenGL;

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Selection::Name::Name( unsigned int id )	{ glPushName(id); }
Selection::Name::~Name( )					{ glPopName(); }
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
Selection::Selection( unsigned int windowSizeW, unsigned int windowSizeH )
: pixelSizeW(windowSizeW),
  pixelSizeH(windowSizeH),
  open(false),
  fail(false),
  numRecords(0)
{}


//-------------------------------------------------------------------------------------------------
Selection::~Selection()
{}

//-------------------------------------------------------------------------------------------------
// Resize the selection buffer
void Selection::resizeBuffer()
{
	if(buffer.size() < MinBufferSize)	buffer.resize(MinBufferSize);
	
	if(fail) //if failed increase the current buffersize by 3/2
	{
		buffer.resize((int)(buffer.size()*1.5f));
		fail = false;
	}
}

//-------------------------------------------------------------------------------------------------
// Send the selection buffer to OpenGL
void Selection::bindBuffer()
{
	resizeBuffer();
	glSelectBuffer((GLsizei)buffer.size(), &buffer[0]);
}

//---------------------------------------------------------------------------------
// Select Nearest Object
//void Selection::getNearest()
//{
//	unsigned int dist;
//	unsigned int minDist= -1; // 0xfff...fff,  2^32-1 or 2^64-1
//	int idx = -1;			  // indice dell'oggetto con z minore
//	
//	int base = 0;
//	
//	for(int i=0; i<numRecords; base+= this->buffer[base]+3, ++i)
//	{
//		if(this->buffer[base]>0)
//		{
//			// look for the minimum Z value
//			dist = this->buffer[base+1];
//
//			if(dist < minDist)
//			{
//				minDist = dist;
//				idx = base+3; // Indice dell'oggetto con z minore	
//			}
//		}
//	}
//	
//	// returns empty array if no names
//	if(idx>0)
//	{
//		std::cout << idx << std::endl;
//
//		for(int i=0; i<buffer[idx-3]; ++i)
//			currSelection.push_back(buffer[idx+i]); //fill up the array
//	}
//}

//---------------------------------------------------------------------------------
// Select all objects 
//void Selection::getAll()
//{
//	int base = 0;
//	for(int i=0; i<numRecords; base+= this->buffer[base]+3, ++i)
//	{
//		if(this->buffer[base]>0)
//		{
//			for(int j= 0; j<buffer[base]; ++j)
//				currSelection.push_back(buffer[base+3+j]);
//		}
//	}
//}

//---------------------------------------------------------------------------------
// Check if there is no object selected
bool Selection::isEmpty() const
{
	return numRecords == 0; //return selected.empty();
}

//---------------------------------------------------------------------------------
// Return true if the selection has failed, i.e. glRenderMode(GL_RENDER) returned a negative value
bool Selection::failed() const
{
	return fail;
}

//---------------------------------------------------------------------------------
/** Prepara la selezione nell'area specificata.
	Premoltiplica la matrice di proiezione per la matrice di picking
	e attiva la modalita' GL_SELECT.
*/
Math::dmat4 Selection::openSelectionMode( const Math::ivec2 &xy, const Math::dmat4 &prjMatrix, const Math::ivec4 &viewport )
{
	const Math::dmat4 pickMatrix     = Math::pickMatrix(Math::dvec2(xy), Math::dvec2(pixelSizeW,pixelSizeW), viewport );
	const Math::dmat4 pickProjection = pickMatrix*prjMatrix;

	//GLfloat mp[16];
	//glGetFloatv(GL_PROJECTION_MATRIX,mp); // stores current proj matrix

	glMatrixMode(GL_PROJECTION);
	{
		glPushMatrix(); // saves projection into the matrix stack
		glLoadMatrixd( Math::value_ptr(pickProjection) );
	}
	glMatrixMode(GL_MODELVIEW);

	// Bind the selection buffer
	bindBuffer();
	
	// Activate selection
	glRenderMode(GL_SELECT);
	glInitNames();

	open = true;
	// We're ready for the draw procedure (do NOT swap buffers!)
	return pickProjection;
}

//---------------------------------------------------------------------------------
/** Esce dalla modalita' selection e riprisitina la matrice di proiezione.
	Questo metodo deve essere invocato DOPO la draw().
*/
void Selection::closeSelectionMode()
{
	numRecords = glRenderMode(GL_RENDER);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	
	open = false;
	fail = numRecords < 0;
}

//---------------------------------------------------------------------------------
const std::vector<unsigned int> &Selection::getObjects()
{
	currSelection.clear();

	if(numRecords <= 0 || open)
		return currSelection;
	
	int base = 0;
	for(int i=0; i<numRecords; base+= this->buffer[base]+3, ++i)
	{
		if(this->buffer[base]>0)
		{
			for(size_t j= 0; j<buffer[base]; ++j)
				currSelection.push_back(buffer[base+3+j]);
		}
	}

	return currSelection;
}

//---------------------------------------------------------------------------------
bool Selection::getNearestObject( unsigned int &id )
{
	currSelection.clear();

	unsigned int dist;
	unsigned int minDist = -1; // 0xfff...fff,  2^32-1 or 2^64-1
	int idx              = -1; // indice dell'oggetto con z minore
	
	int base = 0;
	for(int i=0; i<numRecords; base+= this->buffer[base]+3, ++i)
	{
		if(this->buffer[base]>0)
		{
			// look for the minimum Z value
			dist = this->buffer[base+1];

			if(dist < minDist)
			{
				minDist = dist;
				idx = base+3; // Indice dell'oggetto con z minore	
			}
		}
	}

	if( idx<0 )
		return false;
	
	// returns empty array if no names
	for(int i=0; i<(int)buffer[idx-3]; ++i)
		currSelection.push_back(buffer[idx+i]); //fill up the array
		
	id = currSelection[0];
	return true;
}

//---------------------------------------------------------------------------------
// Reads the depth buffer
// to pick the shallowest object
//bool Selection::pick(const int &x, const int &y, double pickedPoint[3])
//{
//	float   pix;
//	glReadPixels(x,y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&pix);
//
//	GLfloat depthrange[2]={0.f,0.f};
//	glGetFloatv(GL_DEPTH_RANGE,depthrange);
//	if(pix==depthrange[1]) return false;
//	
//	GLdouble mm[16],pm[16]; GLint vp[4];
//	glGetDoublev(GL_MODELVIEW_MATRIX,mm);
//	glGetDoublev(GL_PROJECTION_MATRIX,pm);
//	glGetIntegerv(GL_VIEWPORT,vp);
//	gluUnProject(x,y,pix,mm,pm,vp,&pickedPoint[0],&pickedPoint[1],&pickedPoint[2]);
//	return true;
//}


//---------------------------------------------------------------------------------
// Functions for int names
//void Selection::PushName(unsigned int name)	{glPushName(name);}
//void Selection::LoadName(unsigned int name)	{glLoadName(name);}
//void Selection::PopName()                   {glPopName();}