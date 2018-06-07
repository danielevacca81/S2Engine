// Array2D.h
// 
#ifndef CORE_ARRAY2D_ONCE
#define CORE_ARRAY2D_ONCE

#include <vector>
#include <string>

template< typename T >
class Array2D
{
public:
	Array2D();
	Array2D( int size );
	Array2D( const std::vector<T> &data, int rows, int cols );
	Array2D( int rows, int cols );

	~Array2D();

    void setValues       ( const std::vector<T> &data, int rows, int cols );
    void setColumnValues ( const std::vector<T> &values, int col );
	void setRowValues	 ( const std::vector<T> &values, int row);
	void appendRow		 ( const std::vector<T> &row );
    void pushColumnFront ( const std::vector<T> &col );
    void appendColumn	 ( const std::vector<T> &col );
	void removeRows      ( int startRow, int endRow );
	void removeColumns   ( int startColunm, int endColunm );
	void clear           ();
    void fill            ( T value );

	inline int rowsCount()     const { return _rows; }
	inline int columnsCount()  const { return _cols; }
	
	std::vector<T>    values()    const;
	std::vector<T>    &values();

	bool    isEmpty()      const;
	int     size()         const;
	Array2D column( int c ) const;
	Array2D row( int r )    const;
	Array2D columns( int startColunm, int endColunm ) const;
	Array2D rows( int startRow, int endRow ) const;
	Array2D sub( int startRow, int endRow, int startColunm, int endColunm ) const;
	Array2D transposed() const;
	Array2D zeroPaddedRows(int numZeroPaddingRows) const;

	inline T &operator()( int i )      { return _values[i]; }
	inline T operator()( int i ) const { return _values[i]; }
    
    inline T &operator() ( int r, int c )       { return _values[c*_rows +r]; }
    inline T operator()  ( int r, int c ) const { return _values[c*_rows +r]; }

private:
	std::vector<T>  _values; // y*x
	int _rows;
	int _cols;
};

#include "Array2D.hpp"

#endif