// Array2D.hpp
// 
#include "Array2D.h"

#include <iterator>
#include <cassert>
#include <algorithm>

// ------------------------------------------------------------------------------------------------
template<typename T>
Array2D<T>::Array2D()
: _rows( 0 )
, _cols( 0 )
{}

// ------------------------------------------------------------------------------------------------
template<typename T>
Array2D<T>::Array2D( int size )
: _rows( size )
, _cols( 1 )
{
	_values = std::vector<T>( size, 0 );
}

// ------------------------------------------------------------------------------------------------
template<typename T>
Array2D<T>::Array2D( int rows, int cols )
: _rows( rows )
, _cols( cols )
{
	_values = std::vector<T>( rows*cols, 0 );
}

// ------------------------------------------------------------------------------------------------
template<typename T>
Array2D<T>::Array2D( const std::vector<T> &data, int rows, int cols )
: _values( data )
, _rows( rows )
, _cols( cols )
{}


// ------------------------------------------------------------------------------------------------
template<typename T>
Array2D<T>::~Array2D()
{}

// ------------------------------------------------------------------------------------------------
template<typename T>
void Array2D<T>::setValues( const std::vector<T> &data, int rows, int cols )
{
	_rows   = rows;
	_cols   = cols;
	_values = data; // copy data
}

// ------------------------------------------------------------------------------------------------
template<typename T>
void Array2D<T>::setColumnValues( const std::vector<T>& values, int c )
{
	assert(c < _cols);

	const int size = values.size();

	assert(size <= _rows);

	std::copy_n(values.begin(), size, _values.begin() + c*_rows);
}

// ------------------------------------------------------------------------------------------------
template<typename T>
void Array2D<T>::setRowValues( const std::vector<T>& values, int r )
{
	assert( r < _rows );
	assert( (int) values.size() == _cols );

	for( int i = 0; i < _cols; ++i )
		_values[i*_rows + r] = values[i];
}

// ------------------------------------------------------------------------------------------------
template<typename T>
void Array2D<T>::clear()
{
	_values.clear();
	_rows = 0;
	_cols = 0;
}

// ------------------------------------------------------------------------------------------------
template<typename T>
bool Array2D<T>::isEmpty() const
{
	return _values.empty();
}

// ------------------------------------------------------------------------------------------------
template<typename T>
std::vector<T> Array2D<T>::values() const
{
	return _values;
}

// ------------------------------------------------------------------------------------------------
template<typename T>
std::vector<T> &Array2D<T>::values()
{
	return _values;
}

// ------------------------------------------------------------------------------------------------
template<typename T>
int Array2D<T>::size()   const
{
	return static_cast<int>( _values.size() );
}

// ------------------------------------------------------------------------------------------------
template<typename T>
Array2D<T> Array2D<T>::column( int c ) const
{
	assert( c < _cols );

	Array2D col( _rows );

	std::copy_n( _values.begin() + c*_rows, _rows, col._values.begin() );

	return std::move( col );
}

// ------------------------------------------------------------------------------------------------
template<typename T>
Array2D<T> Array2D<T>::row( int r ) const
{
	assert( r < _rows );

	Array2D row( 1, _cols );

	for( int i = 0; i < _cols; ++i )
		row( 0, i ) = _values[i*_rows + r];

	return std::move( row );
}

// ------------------------------------------------------------------------------------------------
template<typename T>
Array2D<T> Array2D<T>::columns( int startColunm, int endColunm ) const
{
	assert( startColunm <= endColunm );
	assert( endColunm < _cols );

	const int n = endColunm - startColunm + 1;

	Array2D cols( _rows, n );
	std::copy_n( _values.begin() + startColunm*_rows, n*_rows, cols._values.begin() );

	return std::move( cols );
}

// ------------------------------------------------------------------------------------------------
template<typename T>
Array2D<T> Array2D<T>::rows( int startRow, int endRow ) const
{
	assert( startRow <= endRow );
	assert( endRow < _rows );

	const int n = endRow - startRow + 1;

	Array2D rows( n, _cols );

	for( int i = 0; i < _cols; ++i ) {
		std::copy_n( _values.begin() + i*_rows + startRow, n, rows._values.begin() + i * n );
	}

	return std::move( rows );
}

// ------------------------------------------------------------------------------------------------
template<typename T>
void Array2D<T>::removeColumns( int startColunm, int endColunm )
{
	assert( startColunm <= endColunm );
	assert( endColunm < _cols );

	const int n = endColunm - startColunm + 1;

	_values.erase( _values.begin() + startColunm*_rows, _values.begin() + ( startColunm + n )*_rows );

	_cols -= n;
}

// ------------------------------------------------------------------------------------------------
template<typename T>
void Array2D<T>::removeRows( int startRow, int endRow )
{
	assert( startRow <= endRow );
	assert( endRow < _rows );

	const int n = endRow - startRow + 1;

	for( int i=_cols - 1; i >= 0; --i ) {
		typename std::vector< T >::iterator itCol = _values.begin() + i*_rows;
		_values.erase( itCol + startRow, itCol + endRow + 1 );
	}

	_rows -= n;
}

// ------------------------------------------------------------------------------------------------
template<typename T>
Array2D<T> Array2D<T>::sub( int startRow, int endRow, int startColunm, int endColunm ) const
{
	assert( startColunm <= endColunm );
	assert( endColunm < _cols );
	assert( startRow <= endRow );
	assert( endRow < _rows );

	const int nRows = endRow - startRow + 1;
	const int nCols = endColunm - startColunm + 1;

	Array2D sub( nRows, nCols );

	for( int i = 0; i < nCols; ++i ) {
		for( int j = 0; j < nRows; ++j ) {
			sub( j, i ) = _values[( startColunm + i )*_rows + startRow + j];
		}
	}

	return std::move( sub );
}

// ------------------------------------------------------------------------------------------------
template<typename T>
void Array2D<T>::pushColumnFront( const std::vector<T> &data )
{
	if( data.empty() )
		return;

	if( _rows == 0 ) _rows = (int)data.size();
	else             assert( _rows == (int)data.size() );

	_values.insert( _values.begin(), data.begin(), data.end() );
	++_cols;
}

// ------------------------------------------------------------------------------------------------
template<typename T>
void Array2D<T>::appendColumn( const std::vector<T> &data )
{
	if( data.empty() )
		return;

	if( _rows == 0 ) _rows = (int)data.size();
	else             assert( _rows == (int)data.size() );

	_values.insert( _values.end(), data.begin(), data.end() );
	++_cols;
}

// ------------------------------------------------------------------------------------------------
template<typename T>
void Array2D<T>::appendRow( const std::vector<T> &data )
{
	if( _cols == 0 )
		_cols = data.size();

	int r = _rows + 1;

	typename std::vector<T> tmp( _cols*r );

	for( int i=0; i < _cols; ++i )
	{
		for( int j=0; j < _rows; ++j )
		{
			const int k1 = i*r + j;
			const int k2 = i*_rows + j;

			tmp[k1] = _values[k2];
		}
	}

	for( int i=0; i < _cols; ++i )
	{
		for( size_t j=0; j < data.size(); ++j )
		{
			const size_t k1 = ( _rows + j ) + r*i;
			const size_t k2 = j + data.size()*i;

			if( k2 >= data.size() )
				tmp[k1] = data[0];	// !!!!!
			else
				tmp[k1] = data[k2];
		}
	}

	_rows = r;
	tmp.swap( _values );
}

// ------------------------------------------------------------------------------------------------
template<typename T>
void Array2D<T>::fill( T value )
{
	std::fill( _values.begin(), _values.end(), value );
}

// ------------------------------------------------------------------------------------------------
template<typename T>
Array2D<T> Array2D<T>::transposed() const
{
	Array2D<T> out( columnsCount(), rowsCount() );

	for( int i = 0; i < rowsCount(); ++i )
		for( int j = 0; j < columnsCount(); ++j )
			out( j, i ) = (*this)( i, j );

	return std::move( out );
}

// ------------------------------------------------------------------------------------------------
template<typename T>
Array2D<T> Array2D<T>::zeroPaddedRows(int numZeroPaddingRows) const
{
	if (numZeroPaddingRows < 1)
		return (*this);

	Array2D<T> out( rowsCount() + numZeroPaddingRows, columnsCount());
	for (int j = 0; j < columnsCount(); ++j)
		for (int i = 0; i < rowsCount(); ++i)
			out(i, j) = (*this)(i, j);

	return std::move(out);
}