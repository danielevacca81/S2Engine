// ColorMap.cpp
//
#include "ColorMap.h"

using namespace Scene;

// ------------------------------------------------------------------------------------------------
ColorMap::ColorMap( const std::vector< Color > &colors )
: _colors( colors )
{}

// ------------------------------------------------------------------------------------------------
void                        ColorMap::setColors( const std::vector< Color > &colors ) { _colors = colors; }
const std::vector< Color > &ColorMap::colors()       const { return _colors; }
int                         ColorMap::colorsCount()  const { return (int) _colors.size(); }


// ------------------------------------------------------------------------------------------------
ColorMap ColorMap::build( const std::vector< ColorEntry > &colors, const Size &size )
{
	const int ncol = (int) colors.size();

	std::vector< Color > _colors;
	int _size = static_cast<int>( size );

	for( int i = 0; i < _size; ++i )
	{
		const float f = float( i ) / ( _size - 1 );

		int currPos = 0;
		for( int j = 0; j < ncol; ++j )
		{
			if( colors[j].position >= f )
			{
				currPos = j > 1 ? j - 1 : 0;
				break;
			}
		}

		if( currPos < ( ncol - 1 ) )
		{
			const float u = ( f - colors[currPos].position ) / ( colors[currPos + 1].position - colors[currPos].position );

			Color currColor;

			if( colors[currPos].gradient )
				currColor = colors[currPos].color.blend( colors[currPos + 1].color, u );
			else
				currColor = colors[currPos].color;

			_colors.push_back( currColor );
		}
		else
		{
			_colors.push_back( colors[ncol - 1].color );
		}
	}

	ColorMap out;
	out.setColors( _colors );

	return out;
}

//// ------------------------------------------------------------------------------------------------
//ColorMap ColorMapFactory::fromCSV( const std::string &commaSeparatedValues )
//{
//	const std::vector<std::string> rgbEntries = split( commaSeparatedValues, "\n\r" );
//
//	std::vector< Color > colors;
//	for( auto &entry : rgbEntries )
//	{
//		std::vector<std::string> rgb = split( entry, "," );
//
//		if( rgb.size() < 3 )
//			break;
//
//		// ?
//		//assert rgb.size == 3
//
//		const unsigned char R = std::stoi( rgb[0] );
//		const unsigned char G = std::stoi( rgb[1] );
//		const unsigned char B = std::stoi( rgb[2] );
//		
//		colors.push_back( Color((unsigned char)R,G,B) );
//	}
//
//	
//	ColorMap out;
//	out.setColors( colors );
//
//	return out;
//}

// ------------------------------------------------------------------------------------------------
//OpenGL::Texture1DPtr ColorMap::toTexture() const
//{
//    std::vector<unsigned char> buf( _colors.size()*4 );
//    for( size_t i=0; i<_colors.size(); ++i )
//    {
//        buf[i * 4 + 0] = _colors[i].r() * 255;
//        buf[i * 4 + 1] = _colors[i].g() * 255;
//        buf[i * 4 + 2] = _colors[i].b() * 255;
//        buf[i * 4 + 3] = _colors[i].a() * 255;
//    }
//    
//    OpenGL::Texture1DPtr tex = OpenGL::Texture1D::New( GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, false, false, _colors.size(), &buf[0] );
//    tex->setParameter( GL_TEXTURE_MAG_FILTER, GL_LINEAR );
//    tex->setParameter( GL_TEXTURE_MIN_FILTER, GL_LINEAR );
//    return tex;
//}

