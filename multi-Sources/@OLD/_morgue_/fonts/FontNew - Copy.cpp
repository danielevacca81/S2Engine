// Font.cpp
//
#include "FontNew.h"

#include "ft2build.h"
#include FT_FREETYPE_H

//#include "Common/StringTools.h"

using namespace s2;

static FT_Library library;

// Global fonts collection
FontsLibrary::FontCollection FontsLibrary::_fonts;

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
bool FontsLibrary::init( const std::wstring &path )
{
	FT_Error error = FT_Init_FreeType( &library );
	
	if( error != FT_Err_Ok )
		return false;

	// @TODO: for each file in path
	FontPtr defaultFont = std::make_shared<Font>("Arial");

	const std::string defaultFontFile("arial.ttf");


	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	error = FT_New_Face( library,
		                 defaultFontFile.c_str(),
						 0,
						 (FT_Face*)defaultFont->_face);

	if ( error == FT_Err_Unknown_File_Format )
	{
		// the font file could be opened and read, but it appears
		// that its font format is unsupported
		return false;
	}
	else if ( error )
	{
		// another error code means that the font file could not
		// be opened or read, or that it is broken...
		return false;
	}
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/

	_fonts[ defaultFont->name() ] = defaultFont;

	return true;
}

// -------------------------------------------------------------------------------------------------
FontPtr FontsLibrary::font( const std::string &fontName, int size )
{
	FontCollection::iterator it = _fonts.find(fontName);
	if( it == _fonts.end() )
		return nullptr;
	
	return it->second;
}

// -------------------------------------------------------------------------------------------------
bool FontsLibrary::contains( const std::string &fontName )
{
	return _fonts.find(fontName) != _fonts.end();
}


// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
Font::Font( const std::string &fontName, unsigned int fontSize )
: _name(fontName)
//, _size(fontSize)
{
	if( FontsLibrary::contains(fontName) )
	{
		FontPtr fnt = FontsLibrary::font(fontName,fontSize);

		_face = fnt->_face;
		//_texture = fnt->_texture;
		//_glyphs  = fnt->_glyphs;
	}
}

// -------------------------------------------------------------------------------------------------
Font::~Font()
{}

// ------------------------------------------------------------------------------------------------
//Font::TextSize Font::textSize( const std::wstring &str ) const
//{
//	float w = 0.f;
//
//	for( size_t j=0; j<str.size(); ++j )
//	{
//		Char ch = _glyphs.at( str.at(j) );
//
//		w += ch.xAdv-ch.xOff;
//
//		// handle kernings
//		//if( j+1<str.size() )
//		//{
//		//	std::map<int,Char>::const_iterator jt = chars.find( str.at(j+1) );
//
//		//	if( jt == chars.end() )
//		//		continue; // unknown char ?
//
//		//	w += adjustForKerningPairs( it->second, jt->second );
//		//}
//	}
//
//	TextSize sz;
//	sz.height = (_size * 1.0f);
//	sz.width  =  w * _size;
//
//	return sz;
//}

// -------------------------------------------------------------------------------------------------
//float Font::adjustForKerningPairs( const Char &ch1, const Char &ch2 ) const
//{
//	int amount = 0;
//	for( size_t i=0; i<ch1.kernings.size(); i+=2 )
//		if( ch1.kernings[i] == ch2.id )
//		{
//			amount = ch1.kernings[i+1];
//			break;
//		}
//
//	return (float)amount;
//}

// ------------------------------------------------------------------------------------------------
//void Font::draw( const std::wstring &text )
//{
//	std::vector<Math::vec2> uv;
//	std::vector<Math::dvec3> xy;
//
//	int xOffset = 0;
//	int yOffset = 0;
//
//	OpenGL::VertexBuffer vb;
//	for( size_t i=0; i<text.size(); ++i )
//	{
//		const unsigned int t =  text[i];
//		const Char ch        = _glyphs[t];
//
//		if( ch.w<=0 || ch.h<=0 )
//			continue;
//
//		int u[4],v[4];
//		int x[4],y[4];
//
//		u[0] = (ch.x + 0   );
//		u[1] = (ch.x + ch.w);
//		u[2] = (ch.x + ch.w);
//		u[3] = (ch.x + 0   );
//
//		v[0] = (ch.y + ch.h);
//		v[1] = (ch.y + ch.h);
//		v[2] = (ch.y + 0   );
//		v[3] = (ch.y + 0   );
//		uv.push_back( Math::vec2(u[0],v[0]) );
//		uv.push_back( Math::vec2(u[1],v[1]) );
//		uv.push_back( Math::vec2(u[2],v[2]) );
//		uv.push_back( Math::vec2(u[3],v[3]) );
//
//		x[0] = xOffset + 0;
//		x[1] = xOffset + ch.w;
//		x[2] = xOffset + ch.w;
//		x[3] = xOffset + 0;
//		
//		y[0] = yOffset + ch.yOff + ch.h;
//		y[1] = yOffset + ch.yOff + ch.h;
//		y[2] = yOffset + ch.yOff + 0;
//		y[3] = yOffset + ch.yOff + 0;
//		xy.push_back( Math::dvec3(x[0],y[0],0.0) );
//		xy.push_back( Math::dvec3(x[1],y[1],0.0) );
//		xy.push_back( Math::dvec3(x[2],y[2],0.0) );
//		xy.push_back( Math::dvec3(x[3],y[3],0.0) );
//
//		vb << OpenGL::Vertex(xy[0],Math::vec3(),uv[0]) 
//		   << OpenGL::Vertex(xy[1],Math::vec3(),uv[1]) 
//		   << OpenGL::Vertex(xy[2],Math::vec3(),uv[2]) 
//		   << OpenGL::Vertex(xy[3],Math::vec3(),uv[3]);
//
//		xOffset = (ch.xAdv - ch.xOff);
//		yOffset = 0;
//
//		//glTexCoord2d(u,1.0-v);
//		//glVertex2d(x,-y);
//
//		//glTexCoord2d(u,1.0-v);
//		//glVertex2d(x,-y);
//
//		//glTexCoord2d(u,1.0-v);
//		//glVertex2d(x,-y);
//
//		//glTexCoord2d(u,1.0-v);
//		//glVertex2d(x,-y);
//	}
//
//	//RenderState rs;
//
//	//r.draw( OpenGL::Quads, rs );
//}