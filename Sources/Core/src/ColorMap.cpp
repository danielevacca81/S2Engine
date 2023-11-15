// ColorMap.cpp
//
#include "ColorMap.h"

// ------------------------------------------------------------------------------------------------
ColorMap::ColorMap( const std::vector< Color > &colors )
: _colors( colors )
{}

// ------------------------------------------------------------------------------------------------
void                        ColorMap::setColors( const std::vector< Color > &colors ) { _colors = colors; }
const std::vector< Color > &ColorMap::colors()       const { return _colors; }
int                         ColorMap::colorsCount()  const { return (int) _colors.size(); }


// ------------------------------------------------------------------------------------------------
ColorMap ColorMap::build( const std::vector< ColorEntry > &colorEntries, const uint32_t &resolution)
{
	const int ncol = (int) colorEntries.size();

	std::vector< Color > colors;
	const int size = resolution;

	for( int i = 0; i < size; ++i )
	{
		const float f = float( i ) / ( size - 1 );

		int currPos = 0;
		for( int j = 0; j < ncol; ++j )
		{
			if( colorEntries[j].position >= f )
			{
				currPos = j > 1 ? j - 1 : 0;
				break;
			}
		}

		if( currPos < ( ncol - 1 ) )
		{
			const float u = ( f - colorEntries[currPos].position ) / ( colorEntries[currPos + 1].position - colorEntries[currPos].position );

			Color currColor;

			if( colorEntries[currPos].gradient )
				currColor = colorEntries[currPos].color.blend( colorEntries[currPos + 1].color, u );
			else
				currColor = colorEntries[currPos].color;

			colors.push_back( currColor );
		}
		else
		{
			colors.push_back( colorEntries[ncol - 1].color );
		}
	}

	ColorMap out;
	out.setColors( colors );

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

