// Array3D.h
// 
#ifndef CORE_ARRAY3D_ONCE
#define CORE_ARRAY3D_ONCE

#include "Array2D.h"

#include <vector>
#include <string>

namespace s2 {

template< typename T >
class Array3D
{
public:
	Array3D();
	Array3D( int planes, int rows, int cols );

	~Array3D();

	void setValues( const std::vector<T> &data, int plane, int rows, int cols );
	void setPlane( const Array2D<T> &plane, int p );
	void clear();
	void fill( T value );

	int rowsCount()    const { return _rows; }
	int columnsCount() const { return _cols; }
	int planesCount()  const { return _planes; }

	std::vector<T>    values()    const;
	std::vector<T>    &values();

	bool               isEmpty()         const;
	int                size()            const;

	Array2D<T> sectionHorizontal( int row )    const;
	Array2D<T> sectionVertical( int column ) const;
	Array2D<T> sectionTransversal( int plane )  const;

	inline T &operator()( int i ) { return _values[i]; }
	inline T operator()( int i ) const { return _values[i]; }

	inline T &operator() ( int p, int r, int c ) { return _values[p*_rows*_cols + c*_rows + r]; }
	inline T operator()  ( int p, int r, int c ) const { return _values[p*_rows*_cols + c*_rows + r]; }

private:
	/**
		Array3D elements layout is y*x*z (rows*cols*planes)

				  /
			   z /
				/
			   /     x
			  +--------------
			  |
			  |
			y |
			  |
			  |
	*/
	std::vector<T>  _values; // y*x*z
	int _planes;
	int _rows;
	int _cols;
};

}

#include "Array3D.hpp"

#endif