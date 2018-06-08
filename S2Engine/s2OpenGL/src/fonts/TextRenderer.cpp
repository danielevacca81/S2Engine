// TextRenderer.cpp
//
#include "TextRenderer.h"

#include "Renderer/Renderer.h"
#include "Fonts/FontNew.h"

#include "utils/String.h"

#include <iostream>

using namespace s2;

// ------------------------------------------------------------------------------------------------
TextRenderer::TextRenderer()
{}

// ------------------------------------------------------------------------------------------------
TextRenderer::~TextRenderer()
{}

// ------------------------------------------------------------------------------------------------
void TextRenderer::addTag( const TextTag &tag )
{
	if( !tag._text.empty() )
		_tags.push_back(tag);
}

// ------------------------------------------------------------------------------------------------
bool TextRenderer::isEmpty() const
{
	return _tags.empty();
}

// ------------------------------------------------------------------------------------------------
bool TextRenderer::init()
{
	_shader = OpenGL::Program::New();

	_shader->attachVertexShader( 
		STRINGIFY (
	varying vec2 vUV;
	void main()
	{
		gl_Position   = gl_Vertex;
		vUV           = gl_MultiTexCoord0.xy;
		gl_FrontColor = gl_Color;
	}
	));

	_shader->attachFragmentShader( 
		STRINGIFY (

	uniform sampler2D uAtlas;
	varying vec2      vUV;

	void main()
	{
		gl_FragColor = texture2D( uAtlas,vUV );
		gl_FragColor = vec4(1,0,1,1);
	}
	));

	_shader->link("TextRenderer");
	std::cout << _shader->name() << ":" << _shader->info() << std::endl;

	OpenGL::Blending b;
	b.enabled = true;
	
	_renderState.blending = b;
	_drawState   = OpenGL::DrawState( _shader, _renderState );
	_viewState   = OpenGL::ViewState( OpenGL::ViewState::Orthographic );
	return true;
}

// ------------------------------------------------------------------------------------------------
void TextRenderer::update()
{
	for( std::list<TextTag>::reverse_iterator it = _tags.rbegin();
		it != _tags.rend();
		++it )
	{
		// the first is always visible
		it->_visible = true;
	}

#if 0
	std::vector< TextTag* > lbl;

	const Math::ivec4 vp = _renderer->viewport();
	Math::box3  vb( Math::dvec3(vp.x,vp.y,0.0),Math::dvec3(vp.z,vp.w,0.0) );

	_box = Math::box3();

	for( std::list<TextTag>::reverse_iterator it = _tags.rbegin();
		it != _tags.rend();
		++it )
	{
		// compute overlapping with other tags
		const double strH = it->_fonts->textSize(it->_text).height;
		const double strW = it->_fonts->textSize(it->_text).width;
		
		const Math::dvec3 p0( 0.0, strH, 0.0 );
		const Math::dvec3 p1( strW, 0.0, 0.0 );

		double xOffset = 0.0;

		switch( it->_alignment )
		{
		case TextRenderer::AlignRight:		xOffset = -strW*1.0; break;
		case TextRenderer::AlignCenter:     xOffset = -strW*0.5; break;
		case TextRenderer::AlignLeft:		break;	
		}

		const Math::dvec3 offset(xOffset,0,0);
		const Math::dvec3 p = Math::dvec3(it->_screenPos,0) + offset;

		//it->_screenPos = p+offset;
		it->_box       = Math::box3::createAABB( p-p0, p+p1 );

		// the first is always visible
		if( lbl.empty() )
		{
			it->_visible = true;
			lbl.push_back(&(*it));
		}
		else
		{
			// tag is visible if not overlaps other tags
			for( size_t j=0; j<lbl.size(); ++j )
			{
				it->_visible = true;
				if( lbl[j]->_box.overlaps2D(it->_box) )
				{
					it->_visible = false;
					break;
				}
			}

			// add to the visible list
			if(it->_visible)
				lbl.push_back( &(*it) );
		}

		// eventually cull not visible tags
		if( it->_visible && !vb.contains2D(it->_box) && !vb.overlaps2D(it->_box) )
			it->_visible = false;		
	}
#endif
}


// ------------------------------------------------------------------------------------------------
void TextRenderer::draw( OpenGL::Renderer *r )
{
	if( _tags.empty() )
		return;

	update();

	/*
	OpenGL::VertexBuffer vb;

	for( std::list<TextTag>::iterator jt = _tags.begin();
		jt != _tags.end();
		++jt )
	{
		if( !jt->_visible )
			continue;

		vb << buildVertexBuffer( *jt );
	}

	r->draw( OpenGL::Quads, vb, _viewState, _drawState );
	*/

	//OpenGL::VertexBuffer vb;
	//for( int i=-10; i<10; ++i )
	//{
	//	vb << OpenGL::Vertex( Math::dvec3( i, -10, 0 ), Math::vec3(), Math::vec2(), i>=0 ? Color::white():Color::blue() );
	//	vb << OpenGL::Vertex( Math::dvec3( i,  10, 0 ), Math::vec3(), Math::vec2(), i>=0 ? Color::white():Color::blue() );
	//}

	//for( int i=-10; i<10; ++i )
	//{
	//	vb << OpenGL::Vertex( Math::dvec3( -10, i, 0 ), Math::vec3(), Math::vec2(), i>=0 ? Color::white():Color::blue() );
	//	vb << OpenGL::Vertex( Math::dvec3(  10, i, 0 ), Math::vec3(), Math::vec2(), i>=0 ? Color::white():Color::blue() );
	//}

	//_renderer.draw( OpenGL::Lines,vb, _viewState, ds);

#if 0
	const Math::dmat4 pm = r.projectionMatrix();
	const Math::dmat4 mm = r.modelViewMatrix();

	_renderer->setScreenSpaceProjection();
	_renderer->setModelView( Math::dmat4(1) );

	glPushAttrib( GL_COLOR_BUFFER_BIT );
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	VertexBuffer vb;

	for( std::list<TextTag>::iterator jt = _tags.begin();
		 jt != _tags.end();
		 ++jt )
	{
		if( !jt->_visible )
			continue;
		
		//glColor4fv( jt->_color );

		//glPushMatrix();
		//glTranslated( jt->_screenPos.x, jt->_screenPos.y, 0 );

		vb << jt->_fonts->buildVertexBuffer( jt->_screenPos.x, jt->_screenPos.y, jt->_text, jt->_color );

		//glPopMatrix();

#if DEBUGBOX
		// debug Box
		glDisable(GL_TEXTURE_2D);
		glColor4f(0,0,1,1);
		glBegin( GL_LINE_LOOP );
			glVertex2d( jt->_box.minPoint().x, jt->_box.minPoint().y );
			glVertex2d( jt->_box.maxPoint().x, jt->_box.minPoint().y );
			glVertex2d( jt->_box.maxPoint().x, jt->_box.maxPoint().y );
			glVertex2d( jt->_box.minPoint().x, jt->_box.maxPoint().y );
		glEnd();
		glEnable(GL_TEXTURE_2D);
#endif
	}
	
	r.draw( Quads, vb );

	//glPopAttrib();
	_tags.clear();

	_renderer->setProjection(pm);
	_renderer->setModelView(mm);
#endif
}

// ------------------------------------------------------------------------------------------------
OpenGL::VertexBuffer TextRenderer::buildVertexBuffer( const TextTag &tag )
{
	OpenGL::VertexBuffer out;
/*
	FontPtr face = FontsLibrary::font( tag._fontFace,tag._size );
	//face->atlas();

	std::vector<Font::Glyph> glyphs = face->rasterizeText( tag._text );
	
	for( auto it = glyphs.begin();
		 it     != glyphs.end();
		 ++it )
	{
		// shortcuts
		const int x    = it->offset_x;
		const int y    = it->offset_y;
		const int w    = it->width;
		const int h    = it->height;
		const float s0 = it->s0;
		const float t0 = it->t0;
		const float s1 = it->s1;
		const float t1 = it->t1;

		out << OpenGL::Vertex( Math::dvec3( x+0, y+0, 0.0 ), Math::vec3(), Math::vec2( s0,t0 ), tag._color )
			<< OpenGL::Vertex( Math::dvec3( x+0, y-h, 0.0 ), Math::vec3(), Math::vec2( s0,t1 ), tag._color )
			<< OpenGL::Vertex( Math::dvec3( x+w, y-h, 0.0 ), Math::vec3(), Math::vec2( s1,t1 ), tag._color )
			<< OpenGL::Vertex( Math::dvec3( x+0, y+0, 0.0 ), Math::vec3(), Math::vec2( s0,t0 ), tag._color )
			<< OpenGL::Vertex( Math::dvec3( x+w, y-h, 0.0 ), Math::vec3(), Math::vec2( s1,t1 ), tag._color )
			<< OpenGL::Vertex( Math::dvec3( x+w, y+0, 0.0 ), Math::vec3(), Math::vec2( s1,t0 ), tag._color )
			;
	}
	*/
	return out;
}
