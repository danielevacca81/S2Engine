// Selection.h
//
#ifndef SELECTION_H
#define SELECTION_H

#include "S2RendererAPI.h"


#include "Math/Math.h"

#include <vector>

namespace OpenGL{

//---------------------------------------------------------------------------------------------
/**
  * Selection class.
  * This class wraps the OpenGL selection mechanism.
  * It gives an abstract way to select objects using std::vectors of unsigned int.
  * It DOES NOT store pointers and DOES NOT keep track of current selected objects.
  *
  * @author D. Vacca
  * @updated 30-jul-2009
  */

class S2RENDERER_API Selection
{
private:
	enum {MinBufferSize = 4096}; // 4 ^ 6  (1024 different entries in the selection buffer)

	std::vector<unsigned int> buffer;
	std::vector<unsigned int> currSelection;
	int numRecords;
	int pixelSizeW;
	int pixelSizeH;

	void bindBuffer();
	void resizeBuffer();

	bool open;
	bool fail;

	//void getNearest();
	//void getAll();

public:
	class Name
	{
	public:
		explicit Name(unsigned int id);
		~Name();
	};

	enum SelectionMode {Nearest, All};

	Selection( unsigned int windowSizeW=9, unsigned int windowSizeH=9 );
	~Selection();

	bool isEmpty() const;	// true if no object has been selected
	bool failed() const;    // true if the selection buffer was not big enough
	//bool pick(const int &x, const int &y, double pickedPoint[3]);

	// -------------------------------------------------------------------------
	Math::dmat4 openSelectionMode( const Math::ivec2 &xy, const Math::dmat4 &prjMatrix, const Math::ivec4 &viewport );
	void closeSelectionMode();
	const std::vector<unsigned int> &getObjects();
	bool getNearestObject( unsigned int &id );
};

}
#endif 
