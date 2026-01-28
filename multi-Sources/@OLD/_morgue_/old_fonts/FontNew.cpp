// Font.cpp
//
#include "FontNew.h"


#include "Math/Math.h"

#include "OpenGL/VertexBuffer.h"
#include "Renderer/RenderState.h"
#include "Renderer/Renderer.h"

#include "Common/StringTools.h"

#include "FTGL/ftgl.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

typedef std::map<std::string, FTFont*> FontList;
typedef FontList::const_iterator FontIter;


static FontList fonts;



using namespace s2;


FTGLFontManager& FTGLFontManager::Instance()
{
	static FTGLFontManager tm;
	return tm;
}

FTGLFontManager::~FTGLFontManager()
{
    FontIter font;
    for(font = fonts.begin(); font != fonts.end(); font++)
    {
        delete (*font).second;
    }

    fonts.clear();
}


FTFont* FTGLFontManager::GetFont(const char *filename, int size)
{
    char buf[256];
    sprintf(buf, "%s%i", filename, size);
    std::string fontKey = std::string(buf);

    FontIter result = fonts.find(fontKey);
    if(result != fonts.end())
    {
        //LOGMSG("Found font %s in list", filename);
        return result->second;
    }

    std::string fullname = /*path + */std::string(filename);
    
	FTFont* font = new FTTextureFont(fullname.c_str());

    if(!font)
    {
        //LOGERROR("Font %s failed to open", fullname.c_str());
        delete font;
        return NULL;
    }

    if(!font->FaceSize(size))
    {
        //LOGERROR("Font %s failed to set size %i", filename, size);
        delete font;
        return NULL;
    }

    fonts[fontKey] = font;

	font->Render(

    return font;
}

#if 0

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// Global fonts collection
FontsLibrary::FontCollection FontsLibrary::_fonts;
FontsLibrary                *FontsLibrary::_library = 0;
void                        *FontsLibrary::_fontManager = 0;


// ------------------------------------------------------------------------------------------------
FontsLibrary::FontsLibrary()
{
	_fontManager = (void*)font_manager_new( 512,512,1 );
}

// ------------------------------------------------------------------------------------------------
FontsLibrary::~FontsLibrary()
{
	font_manager_delete((font_manager_t*)_fontManager);
}

// ------------------------------------------------------------------------------------------------
// preload fonts
bool FontsLibrary::init( const std::wstring &path )
{
	if( !_library )
	{
		_library = new FontsLibrary();
	}

	std::string defaultFontFile(toStdString(path)+"/arial.ttf");

	texture_font_t *font = font_manager_get_from_filename((font_manager_t*)_fontManager,defaultFontFile.c_str(),8);
	
	FontPtr fnt = std::make_shared<Font>();
	fnt->_font = font;
	fnt->_name = "Arial";

	_fonts[ fnt->_name ] = fnt;


	//// Build fonts and corresponding display lists
 //   vector_t        *fonts = vector_new( sizeof(texture_font_t) );
 //   texture_atlas_t *atlas = texture_atlas_new( 512, 512, 1 );

 //   for( int i=7; i < 27; ++i)
 //   {
 //       texture_font_t *font = texture_font_new( atlas, defaultFontFile, i );
 //       texture_font_load_glyphs( font, text );
 //       //make_display_lists( font );
 //       vector_push_back( fonts, font );
 //       free( font ); // We're only freeing the local copy of the font
 //   }
 //   
	//texture_atlas_delete( atlas );
	return true;
}

// -------------------------------------------------------------------------------------------------
FontPtr FontsLibrary::font( const std::string &fontName )
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
Font::Font()
{}

// -------------------------------------------------------------------------------------------------
Font::~Font()
{}

// ------------------------------------------------------------------------------------------------
//Font::TextSize    Font::textSize( const std::wstring &str ) const
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
void Font::draw( const std::wstring &text, OpenGL::Renderer *r )
{
	if( text.empty() )
		return;

	texture_font_t* f = (texture_font_t*) _font;

	int advance_x = 0;
	OpenGL::VertexBuffer vb;

	for( size_t i=0; i<text.length(); ++i)
	{
		texture_glyph_t *glyph = (texture_glyph_t *) texture_font_get_glyph( f, text[i] );

		float kerning = 0;
		if( i > 0 )
			kerning = texture_glyph_get_kerning( glyph, text[i-1] );

		//glCallList( glyph->id );
		const int x  = glyph->offset_x;
		const int y  = glyph->offset_y;
		const int w  = glyph->width;
		const int h  = glyph->height;
		const float s0 = glyph->s0;
		const float t0 = glyph->t0;
		const float s1 = glyph->s1;
		const float t1 = glyph->t1;

		vb 
			<< OpenGL::Vertex(Math::dvec3(advance_x +kerning+ x  , y  , 0),Math::vec3(),Math::vec2(s0, t0) )
			<< OpenGL::Vertex(Math::dvec3(advance_x +kerning+ x  , y-h, 0),Math::vec3(),Math::vec2(s0, t1) )
			<< OpenGL::Vertex(Math::dvec3(advance_x +kerning+ x+w, y-h, 0),Math::vec3(),Math::vec2(s1, t1) )
			<< OpenGL::Vertex(Math::dvec3(advance_x +kerning+ x  , y  , 0),Math::vec3(),Math::vec2(s0, t0) )
			<< OpenGL::Vertex(Math::dvec3(advance_x +kerning+ x+w, y-h, 0),Math::vec3(),Math::vec2(s1, t1) )
			<< OpenGL::Vertex(Math::dvec3(advance_x +kerning+ x+w, y  , 0),Math::vec3(),Math::vec2(s1, t0) )
		;

		//glBegin( GL_TRIANGLES );
		//glTexCoord2f( s0, t0 ); glVertex2i( x,   y   );
		//glTexCoord2f( s0, t1 ); glVertex2i( x,   y-h );
		//glTexCoord2f( s1, t1 ); glVertex2i( x+w, y-h );
		//glTexCoord2f( s0, t0 ); glVertex2i( x,   y   );
		//glTexCoord2f( s1, t1 ); glVertex2i( x+w, y-h );
		//glTexCoord2f( s1, t0 ); glVertex2i( x+w, y   );
		//glEnd();
		//glEndList();

		advance_x += (int)glyph->advance_x;
	}
	
	
	r->draw( OpenGL::Triangles, vb );
	
	
/*	std::vector<Math::vec2> uv;
	std::vector<Math::dvec3> xy;

	int xOffset = 0;
	int yOffset = 0;

	OpenGL::VertexBuffer vb;
	for( size_t i=0; i<text.size(); ++i )
	{
		const unsigned int t =  text[i];
		const Char ch        = _glyphs[t];

		if( ch.w<=0 || ch.h<=0 )
			continue;

		int u[4],v[4];
		int x[4],y[4];

		u[0] = (ch.x + 0   );
		u[1] = (ch.x + ch.w);
		u[2] = (ch.x + ch.w);
		u[3] = (ch.x + 0   );

		v[0] = (ch.y + ch.h);
		v[1] = (ch.y + ch.h);
		v[2] = (ch.y + 0   );
		v[3] = (ch.y + 0   );
		uv.push_back( Math::vec2(u[0],v[0]) );
		uv.push_back( Math::vec2(u[1],v[1]) );
		uv.push_back( Math::vec2(u[2],v[2]) );
		uv.push_back( Math::vec2(u[3],v[3]) );

		x[0] = xOffset + 0;
		x[1] = xOffset + ch.w;
		x[2] = xOffset + ch.w;
		x[3] = xOffset + 0;
		
		y[0] = yOffset + ch.yOff + ch.h;
		y[1] = yOffset + ch.yOff + ch.h;
		y[2] = yOffset + ch.yOff + 0;
		y[3] = yOffset + ch.yOff + 0;
		xy.push_back( Math::dvec3(x[0],y[0],0.0) );
		xy.push_back( Math::dvec3(x[1],y[1],0.0) );
		xy.push_back( Math::dvec3(x[2],y[2],0.0) );
		xy.push_back( Math::dvec3(x[3],y[3],0.0) );

		vb << OpenGL::Vertex(xy[0],Math::vec3(),uv[0]) 
		   << OpenGL::Vertex(xy[1],Math::vec3(),uv[1]) 
		   << OpenGL::Vertex(xy[2],Math::vec3(),uv[2]) 
		   << OpenGL::Vertex(xy[3],Math::vec3(),uv[3]);

		xOffset = (ch.xAdv - ch.xOff);
		yOffset = 0;

		//glTexCoord2d(u,1.0-v);
		//glVertex2d(x,-y);

		//glTexCoord2d(u,1.0-v);
		//glVertex2d(x,-y);

		//glTexCoord2d(u,1.0-v);
		//glVertex2d(x,-y);

		//glTexCoord2d(u,1.0-v);
		//glVertex2d(x,-y);
	}

	//RenderState rs;

	//r.draw( OpenGL::Quads, rs );
	*/
}

#if 0
//-------------------------------------------------------------------------------------------------
// Font
//-------------------------------------------------------------------------------------------------
void   Font::setSize(double pixels){_size = pixels;}
double Font::getSize() const	{return _size;}
double Font::getHeight() const	{return _size;}
void   Font::pushSize()	{_stackSize.push(_size);}
void   Font::popSize()	{_size = _stackSize.top(); _stackSize.pop();}

//-------------------------------------------------------------------------------------------------
// FontTexture
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
FontTexture::FontTexture( const std::string &fontName )
: Font(fontName)
{
	fontDL	            = -1;
	_size               = 1.f;
	internalScaleFactor = 1.f;
	loaded              = false;
}

//-------------------------------------------------------------------------------------------------
FontTexture::~FontTexture()
{
	destroy();
}

//-------------------------------------------------------------------------------------------------
void FontTexture::destroy()
{
	texture.destroy();
	glDeleteLists( fontDL, chars.size() );

	chars.clear();
	loaded = false;
	fontDL = -1;
}

//-------------------------------------------------------------------------------------------------
void FontTexture::setSize(double pixels)
{
	_size = ((double)pixels * internalScaleFactor);
}

//-------------------------------------------------------------------------------------------------
bool FontTexture::loadTexture(const std::string &filename)
{
	Image img;
	if( !ImageLoader::load( filename, img ) )
		return false;

	int numCh = img.numChannels();

	if(numCh != 3 && numCh != 4)
		return false;

	GLenum fmt = (numCh == 3) ? GL_RGB : GL_RGBA;
	GLenum t   = GL_UNSIGNED_BYTE;

	//texture.createMipMap(numCh,fmt,t,img.width(), img.height(),img.pixels());
	texture.create(numCh,fmt,t,false,false,img.width(),img.height(),img.pixels());
	//texture.setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//texture.setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//texture.setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	texture.setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texture.setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}



//-------------------------------------------------------------------------------------------------
void FontTexture::build(int pixelSize)
{
	const double invW    = 1.0 / texture.width();
	const double invH    = 1.0 / texture.height();
	internalScaleFactor = fabs(1.0 / (double) pixelSize);

	fontDL   = glGenLists(chars.size());

	for( std::map<int,Char>::iterator it=chars.begin();
		 it != chars.end();
		 ++it )
	{
		double x,y,u,v;
		glNewList(/*fontDL+*/it->first, GL_COMPILE);

		const Char &ch = it->second;

		if( ch.w )
		{
			glBegin(GL_TRIANGLE_STRIP); //inv N
			// 0]
			u = (ch.x + 0) * invW;
			v = (ch.y + ch.h) * invH;
			x = 0;			//x = ch.xOff + 0;
			/*y = ch.h;*/ y =ch.yOff + ch.h;
			glTexCoord2d(u,1.0-v);
			glVertex2d(x,-y);

			// 3]
			u = (ch.x + 0) * invW;
			v = (ch.y + 0) * invH;
			x = 0;	//x = ch.xOff + 0;
			/*y = 0;*/	y = ch.yOff + 0;
			glTexCoord2d(u,1.0-v);
			glVertex2d(x,-y);

			// 1]
			u = (ch.x + ch.w) * invW;
			v = (ch.y + ch.h) * invH;
			x = ch.w; //x = ch.xOff + ch.w;
			/*y = ch.h;*/ y = ch.yOff + ch.h;
			glTexCoord2d(u,1.0-v);
			glVertex2d(x,-y);

			// 2]
			u = (ch.x + ch.w) * invW;
			v = (ch.y + 0) * invH;
			x = ch.w;	//x = ch.xOff + ch.w;
			/*y = 0;*/		y = ch.yOff + 0;
			glTexCoord2d(u,1.0-v);
			glVertex2d(x,-y);

			glEnd();
			glTranslatef((ch.xAdv - ch.xOff), 0,0);
		}
		else
			glTranslated( 1.0/internalScaleFactor, 0,0);

		glEndList();
	}
}


//-------------------------------------------------------------------------------------------------
void FontTexture::bind()
{
	glPushAttrib( GL_ENABLE_BIT );
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	texture.bind();
}

//-------------------------------------------------------------------------------------------------
void FontTexture::unbind()
{
	glPopAttrib();
	texture.unbind();
}

// -------------------------------------------------------------------------------------------------
void FontTexture::drawText(const std::string &str)
{
	//switch(overlayMode)
	//{
	//case OVERLAY_MASK:			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	break;
	//case OVERLAY_BLEND:			glBlendFunc(GL_SRC_ALPHA, GL_ONE);					break;
	//}
	glPushAttrib( GL_LIST_BIT );

	glPushMatrix();
		glScaled(_size,_size,1.0);
		glCallLists((GLsizei)str.size(),GL_UNSIGNED_BYTE,(const unsigned char *)str.c_str());
	glPopMatrix();

	glPopAttrib();
}


// -------------------------------------------------------------------------------------------------
double FontTexture::getTextWidth( const std::string &str, double size, bool monospaced ) const
{
	double w = 0.f;

	if( monospaced )
		w = getHeight()*str.size()*0.5f;
	else
	{
		for( size_t j=0; j<str.size(); ++j )
		{
			std::map<int,Char>::const_iterator it = chars.find( str.at(j) );

			if( it == chars.end() )
				continue; // unknown char ?

			const Char &ch = it->second;

			w += ch.xAdv-ch.xOff;

			// handle kernings
			//if( j+1<str.size() )
			//{
			//	std::map<int,Char>::const_iterator jt = chars.find( str.at(j+1) );

			//	if( jt == chars.end() )
			//		continue; // unknown char ?

			//	w += adjustForKerningPairs( it->second, jt->second );
			//}
		}
	}

	return w * size * internalScaleFactor;
}

//-------------------------------------------------------------------------------------------------
double FontTexture::getHeight() const
{
	return (_size * 1.0f) / internalScaleFactor;
}
#endif

#endif