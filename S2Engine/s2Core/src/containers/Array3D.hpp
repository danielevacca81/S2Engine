// Array3D.hpp
// 
#include "Array3D.h"

#include <cassert>
#include <algorithm>

// ------------------------------------------------------------------------------------------------
template<typename T>
Array3D<T>::Array3D()
: _rows( 0 )
, _cols( 0 )
, _planes( 0 )
{}

// ------------------------------------------------------------------------------------------------
template<typename T>
Array3D<T>::Array3D( int planes, int rows, int cols )
: _rows( rows )
, _cols( cols )
, _planes( planes )
{
	_values = std::vector<T>( planes*rows*cols, 0 );
}

// ------------------------------------------------------------------------------------------------
template<typename T>
Array3D<T>::~Array3D()
{}

// ------------------------------------------------------------------------------------------------
template<typename T>
void Array3D<T>::setValues( const std::vector<T> &data, int planes, int rows, int cols )
{
	assert( planes > 0 );
	assert( rows   > 0 );
	assert( cols   > 0 );

	_rows   = rows;
	_cols   = cols;
	_planes = planes;
	_values = data; // copy data
}

// ------------------------------------------------------------------------------------------------
template<typename T>
void Array3D<T>::setPlane( const Array2D<T> &plane, int p )
{
	if( plane.isEmpty() )
		return;

	assert( p < _planes );
	assert( plane.columnsCount() == _cols );
	assert( plane.rowsCount()    == _rows );

	const int k = p*_rows*_cols; // compute plane index within the array
	for( int i=0; i < plane.size(); ++i )
		_values[k + i] = plane( i );
}

// ------------------------------------------------------------------------------------------------
template<typename T>
void Array3D<T>::clear()
{
	_values.clear();
	_rows = 0;
	_cols = 0;
	_planes = 0;
}

// ------------------------------------------------------------------------------------------------
template<typename T>
bool Array3D<T>::isEmpty() const
{
	return _values.empty();
}

// ------------------------------------------------------------------------------------------------
template<typename T>
std::vector<T> Array3D<T>::values() const
{
	return _values;
}

// ------------------------------------------------------------------------------------------------
template<typename T>
std::vector<T> &Array3D<T>::values()
{
	return _values;
}

// ------------------------------------------------------------------------------------------------
template<typename T>
int Array3D<T>::size()   const
{
	return static_cast<int>( _values.size() );
}

// ------------------------------------------------------------------------------------------------
template<typename T> 
Array2D<T> Array3D<T>::sectionHorizontal( int row )    const
{
	assert( row < _rows);

	// cut the volume into a z-x slice
	Array2D<T> dest( _planes, _cols );
	
	for( int j=0; j < _cols; ++j )
		for( int i=0; i < _planes; ++i )
			dest( i, j ) = (*this)( i, row, j );

	return std::move( dest );
}

// ------------------------------------------------------------------------------------------------
template<typename T>
Array2D<T> Array3D<T>::sectionVertical( int column ) const
{
	assert( column < _cols);

	// cut the volume into a y-z slice
	Array2D<T> dest( _rows, _planes );
	
	for( int j=0; j < _planes; ++j )
		for( int i=0; i < _rows; ++i )
			dest( i, j ) = (*this)( j, i, column );

	return std::move( dest );
}

// ------------------------------------------------------------------------------------------------
template<typename T>
Array2D<T> Array3D<T>::sectionTransversal( int plane )  const
{
	assert( plane < _planes );

	Array2D<T> dest( _rows, _cols );

	const int planeSize = _rows*_cols;
	const int k         = plane * planeSize; // compute plane index within the array

	for( int i=0; i < planeSize; ++i )
		dest( i ) = _values[k + i] ;

	return std::move( dest );
}

// ------------------------------------------------------------------------------------------------
template<typename T>
void Array3D<T>::fill( T value )
{
	std::fill( _values.begin(), _values.end(), value );
}
