// Font.cpp
//
#if 0

#include "FontNew.h"

// FreeType Headers
#include "ft2build.h"
#include FT_FREETYPE_H

#include "ftglyph.h"
#include "ftoutln.h"
#include "fttrigon.h"

using namespace s2;

// Global fonts collection
FontsLibrary::FontCollection FontsLibrary::_fonts;

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
FT_Library  library;

bool FontsLibrary::init( const std::wstring &path )
{
	FT_Error error = FT_Init_FreeType( &library );
	
	if( error != FT_Err_Ok )
		return false;

	// @TODO: for each file in path ?
	FontPtr defaultFont = std::make_shared<Font>("Arial.ttf",16);

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

#if 0

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
Font::Font( const std::string &fontName, unsigned int fontSize )
: _name(fontName)
{
	_face = new FT_Face;

	FT_New_Face( library, 
		         fontName.c_str(), 
				 0,
				 (FT_Face*)&_face );

	FT_Set_Char_Size(
		(FT_Face)_face,   // handle to face object
		0,                // char_width in 1/64th of points (0 means same as height)
		fontSize*64,      // char_height in 1/64th of points
		0,                // horizontal device resolution
		0 );              // vertical device resolution      (if both are 0 then 72 dpi is used)

	//FT_Set_Pixel_Sizes(
	//	face,   // handle to face object
	//	0,      // pixel_width
	//	16 );   // pixel_height
}

// -------------------------------------------------------------------------------------------------
Font::~Font()
{
	delete _face;
}

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
#define WIDTH   640
#define HEIGHT  480

/* origin is the upper left corner */
unsigned char image[HEIGHT][WIDTH];

static void draw_bitmap( FT_Bitmap*  bitmap, FT_Int x, FT_Int y)
{
	FT_Int  i, j, p, q;
	FT_Int  x_max = x + bitmap->width;
	FT_Int  y_max = y + bitmap->rows;


	for ( i = x, p = 0; i < x_max; i++, p++ )
	{
		for ( j = y, q = 0; j < y_max; j++, q++ )
		{
			if ( i < 0      || j < 0      ||
				 i >= WIDTH || j >= HEIGHT )
				continue;

			image[j][i] |= bitmap->buffer[q * bitmap->width + p];
		}
	}
}
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------
std::vector<Font::Glyph> Font::rasterizeText( const std::wstring &text )
{
	std::vector<Glyph> out;

	int           target_height = HEIGHT;
	FT_Face       face = static_cast<FT_Face>(_face);
	FT_GlyphSlot  slot = face->glyph;

	for( size_t i=0; i<text.length(); ++i )
	{
		/* load glyph image into the slot (erase previous one) */
		FT_Error error = FT_Load_Char( face, text[i], FT_LOAD_RENDER );

		if ( error )
			continue;

		/* now, draw to our target surface (convert position) */
		draw_bitmap( &slot->bitmap,
			slot->bitmap_left,
			target_height - slot->bitmap_top );
	}


	//if( text.empty() )
	//	return out;

	//int advance_x = 0;

	//for( size_t i=0; i<text.length(); ++i )
	//{
	//	texture_glyph_t *glyph = (texture_glyph_t *) texture_font_get_glyph( (texture_font_t*)_font, text[i] );

	//	float kerning = 0;
	//	if( i > 0 )
	//		kerning = texture_glyph_get_kerning( glyph, text[i-1] );

	//	//glCallList( glyph->id );
	//	Glyph g;
	//	g.offset_x = advance_x + glyph->offset_x;
	//	g.offset_y =      0    + glyph->offset_y;
	//	g.width    = glyph->width;
	//	g.height   = glyph->height;
	//	g.s0       = glyph->s0;
	//	g.t0       = glyph->t0;
	//	g.s1       = glyph->s1;
	//	g.t1       = glyph->t1;

	//	out.push_back(g);

	//	advance_x += (int)glyph->advance_x;
	//}

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
#else
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
Font::Font( const std::string &fontName, unsigned int fontSize )
: _name(fontName)
{
	_face = new FT_Face;

	FT_New_Face( library, 
		fontName.c_str(), 
		0,
		(FT_Face*)&_face );

	FT_Set_Char_Size(
		(FT_Face)_face,   // handle to face object
		0,                // char_width in 1/64th of points (0 means same as height)
		fontSize*64,      // char_height in 1/64th of points
		96,                // horizontal device resolution
		96 );              // vertical device resolution      (if both are 0 then 72 dpi is used)

	buildGlyphs();
}

// -------------------------------------------------------------------------------------------------
Font::~Font()
{
	delete _face;
}

// -------------------------------------------------------------------------------------------------
void Font::buildGlyphs()
{
	//Now we just setup some texture paramaters.
	GLuint tex;
	glActiveTexture( GL_TEXTURE0 );
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glEnableVertexAttribArray(attribute_coord);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer( attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0 );
}

// -------------------------------------------------------------------------------------------------
std::vector<Font::Glyph> Font::rasterizeText( const std::wstring &text )
{
	FT_Face face  = static_cast<FT_Face>(_face);

	FT_GlyphSlot g = face->glyph;
	const float sx = 1;
	const float sy = 1;
	
	for( size_t i=0; i<text.length(); ++i )
	{
		if( FT_Load_Char( _face, text[i], FT_LOAD_RENDER ) )
			continue;

		glTexImage2D( GL_TEXTURE_2D, 0, GL_RED, g->bitmap.width, g->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer );
		
		const float x2 = x + g->bitmap_left * sx;
		const float y2 = -y - g->bitmap_top * sy;
		const float w  = g->bitmap.width * sx;
		const float h  = g->bitmap.rows * sy;

		GLfloat box[4][4] = {
			{x2,     -y2    , 0, 0},
			{x2 + w, -y2    , 1, 0},
			{x2,     -y2 - h, 0, 1},
			{x2 + w, -y2 - h, 1, 1},
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		x += (g->advance.x >> 6) * sx;
		y += (g->advance.y >> 6) * sy;
	}
}

#endif

#endif