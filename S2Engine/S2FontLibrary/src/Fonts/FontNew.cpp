// Font.cpp
//
#include "FontNew.h"

#include "freetype-gl.h"

//#include "Common/StringTools.h"

using namespace s2;

// Global fonts collection
FontsLibrary::FontCollection FontsLibrary::_fonts;

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
bool FontsLibrary::init( const std::wstring &path )
{
	//FT_Error error = FT_Init_FreeType( &library );
	
	//if( error != FT_Err_Ok )
	//	return false;

	// @TODO: for each file in path
	FontPtr defaultFont = std::make_shared<Font>("Arial.ttf",16);

	//const std::string defaultFontFile("arial.ttf");

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
// ------------------------------------------------------------------------------------------------
static wchar_t* cachedGlyphs = L"";

Font::Font( const std::string &fontName, unsigned int fontSize )
: _name(fontName)
//, _size(fontSize)
{
	//if( FontsLibrary::contains(fontName) )
	//{
	//	FontPtr fnt = FontsLibrary::font(fontName,fontSize);

	//	//_face = fnt->_face;
	//	//_texture = fnt->_texture;
	//	//_glyphs  = fnt->_glyphs;
	//}
	
	_atlas   = (texture_atlas_t*)texture_atlas_new( 512,512,1 );
	_font    = texture_font_new_from_file( (texture_atlas_t*)_atlas, fontSize, fontName.c_str() );

	size_t t = texture_font_load_glyphs( (texture_font_t*)_font, cachedGlyphs );
}

// -------------------------------------------------------------------------------------------------
Font::~Font()
{
	texture_font_delete( (texture_font_t*)_font );
	texture_atlas_delete( (texture_atlas_t*)_atlas );
}

// -------------------------------------------------------------------------------------------------
std::vector<Font::Glyph> Font::rasterizeText( const std::wstring &text )
{
	std::vector<Glyph> out;

	if( text.empty() )
		return out;

	int advance_x = 0;

	for( size_t i=0; i<text.length(); ++i )
	{
		texture_glyph_t *glyph = (texture_glyph_t *) texture_font_get_glyph( (texture_font_t*)_font, text[i] );

		float kerning = 0;
		if( i > 0 )
			kerning = texture_glyph_get_kerning( glyph, text[i-1] );

		//glCallList( glyph->id );
		Glyph g;
		g.offset_x = advance_x + glyph->offset_x;
		g.offset_y =      0    + glyph->offset_y;
		g.width    = glyph->width;
		g.height   = glyph->height;
		g.s0       = glyph->s0;
		g.t0       = glyph->t0;
		g.s1       = glyph->s1;
		g.t1       = glyph->t1;

		out.push_back(g);

		advance_x += (int)glyph->advance_x;
	}

	return out;
}


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