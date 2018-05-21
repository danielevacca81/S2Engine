// Renderer.cpp
//
#include "Renderer.h"

#include "OpenGL.h"
#include "Math/Box.h"
#include "Color.h"
#include "FloatingOrigin.h"
#include "Font.h"
#include "TextRenderer.h"
#include "Image.h"
#include "Geometry/Intersection.h"

#include <iostream>

#include "Utils/MemCheck.h"
using namespace OpenGL;

// ------------------------------------------------------------------------------------------------
Renderer::Renderer()
: _zoomFactor(1.0)
//, _useGradient(false)
, _textRenderer(this)
, fonts(0)
{}

// ------------------------------------------------------------------------------------------------
Renderer::~Renderer()
{}

// ------------------------------------------------------------------------------------------------
void Renderer::setFont( Font *f )
{
	fonts = f;
}

// ------------------------------------------------------------------------------------------------
void Renderer::setClearColor(const Color &gradientTop, const Color &gradientBottom )
{
	bkTop        = gradientTop;
	bkBottom     = gradientBottom;
	_useGradient = true;
}

// ------------------------------------------------------------------------------------------------
void Renderer::setClearColor(const Color &flatColor )
{
	bkTop        = flatColor;
	bkBottom     = flatColor;
	_useGradient = false;
}

// ------------------------------------------------------------------------------------------------
void Renderer::clearBackground()
{
	glClearColor( bkTop.r(),bkTop.g(),bkTop.b(),bkTop.a() );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if( _useGradient )
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-1,1, -1,1, -1,1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		glBegin(GL_TRIANGLE_STRIP); // Z
			// Top
			glColor4fv( bkTop );
			glVertex2i(-1,+1);
			glVertex2i(+1,+1);

			// Bottom
			glColor4fv( bkBottom );
			glVertex2i(-1,-1);
			glVertex2i(+1,-1);
		glEnd();
		glPopAttrib();	
	}
}

// ------------------------------------------------------------------------------------------------
void Renderer::zoomInOut( double factor )
{
	if( Math::abs(factor) < 1e-8 )
		factor = 1.0;

	_zoomFactor *= factor;
}

// ------------------------------------------------------------------------------------------------
void Renderer::setZoom( double zoomValue )
{
	if( zoomValue > 0.0 )
		_zoomFactor = zoomValue;

	//std::cout << zoomFactor << std::endl;
}

// -------------------------------------------------------------------------------------------------
void Renderer::fitView( const Math::box3 &b )
{
	Math::box3 box(b);

	// center in 0,0,0 if no box
	//if( box.empty() )
	//{
	//	if( !_sceneBox.empty() )
	//		box = _sceneBox;
	//	else
	//	{
	//		_zoomFactor = 1.0;
	//		//setCenter( Math::dvec3(0) );
	//		updateViewBox();
	//		//_origin.set(_center);
	//		//glGetDoublev( GL_MODELVIEW_MATRIX, (double*)&(_modelViewMatrix)[0] );
	//		return;
	//	}
	//}

	// center in box.minpoint if box is too small
	if( box.empty() || box.diag() < 1e-8 )
	{
		_zoomFactor = 1.0;
		//setCenter( box.minPoint() );
		//updateViewBox();
		//_origin.set(_center);
		//glGetDoublev( GL_MODELVIEW_MATRIX, (double*)&(_modelViewMatrix)[0] );
		return;
	}

	// resize only if the desired box is big enough
	const double bw = box.sizes().x;
	const double bh = box.sizes().y;

	// height * bw < width * bh
	if( _viewport.w * bw < _viewport.z * bh )	_zoomFactor = 2. * _viewport.w / ( (double)_viewport.z * bh );// scale by height
	else										_zoomFactor = 2. / bw;										 // scale by width

	//setCenter( box.center() );
	//updateViewBox();
	//_origin.set( _center );
	//glGetDoublev( GL_MODELVIEW_MATRIX, (double*)&(_modelViewMatrix)[0] );
}

// ------------------------------------------------------------------------------------------------
void Renderer::enableFloatingOrigin( bool on )
{
	_origin.enable(on);
}

// ------------------------------------------------------------------------------------------------
void Renderer::setViewport(int x, int y, int w, int h )
{
	_viewport.x = x;
	_viewport.y = y;
	_viewport.z = w;
	_viewport.w = h;
	
	glViewport( _viewport.x, _viewport.y, _viewport.z, _viewport.w );
}

// ------------------------------------------------------------------------------------------------
void Renderer::setModelView( const Math::dvec3 &eye,
							 const Math::dvec3 &target,
							 const Math::dvec3 &up )
{
	setModelView( Math::lookAt(eye,target,up) );
}

// ------------------------------------------------------------------------------------------------
void Renderer::setModelView( const Math::dmat4 &m )
{
	_modelViewMatrix = m;
	_eyePosition     = computeEyePosition( m );
	_viewDirection   = Math::dvec3( _modelViewMatrix[2][0], _modelViewMatrix[2][1], _modelViewMatrix[2][2] );
	
	computeClipMatrix();
	
	glPushAttrib( GL_TRANSFORM_BIT );
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd( (const double*)&(_modelViewMatrix)[0] );
	glPopAttrib();
}

// ------------------------------------------------------------------------------------------------
Math::dmat4 Renderer::modelViewMatrix() const { return _modelViewMatrix; }

// ------------------------------------------------------------------------------------------------
Math::dvec3 Renderer::eyePosition() const { return _eyePosition; }

// ------------------------------------------------------------------------------------------------
Math::dvec3 Renderer::viewDirection() const { return _viewDirection; }

// ------------------------------------------------------------------------------------------------
Math::dvec3 Renderer::computeEyePosition( const Math::dmat4 &modelView ) const
{
	// Get the 3 basis vector planes at the camera origin and transform them into model space.
	//  
	// NOTE: Planes have to be transformed by the inverse transpose of a matrix
	//       Nice reference here: http://www.opengl.org/discussion_boards/showthread.php/159564-Clever-way-to-transform-plane-by-matrix
	//
	//       So for a transform to model space we need to do:
	//            inverse(transpose(inverse(MV)))
	//       This equals : transpose(MV) - see Lemma 5 in http://mathrefresher.blogspot.com.au/2007/06/transpose-of-matrix.html
	//
	// As each plane is simply (1,0,0,0), (0,1,0,0), (0,0,1,0) we can pull the data directly from the transpose matrix.
	//  
	Math::dmat4 modelViewT = Math::transpose(modelView);

	// Get plane normals 
	Math::dvec3 n1(modelViewT[0]);
	Math::dvec3 n2(modelViewT[1]);
	Math::dvec3 n3(modelViewT[2]);

	// Get plane distances
	const double d1(modelViewT[0].w);
	const double d2(modelViewT[1].w);
	const double d3(modelViewT[2].w);

	// Get the intersection of these 3 planes
	// http://paulbourke.net/geometry/3planes/
	Math::dvec3 n2n3 = Math::cross(n2, n3);
	Math::dvec3 n3n1 = Math::cross(n3, n1);
	Math::dvec3 n1n2 = Math::cross(n1, n2);

	const Math::dvec3 top = (n2n3 * d1) + (n3n1 * d2) + (n1n2 * d3);
	double denom = Math::dot(n1, n2n3);

	return top / -denom;
#if 0
	// Get the intersection of these 3 planes 
	// (using math from RealTime Collision Detection by Christer Ericson)
	Math::dvec3 n2n3   = Math::cross(n2, n3);
	const double denom = Math::dot(n1, n3);

	Math::dvec3 top = (n2n3 * d1) + Math::cross(n1, (d3*n2) - (d2*n3));
	return top / -denom;
#endif
}


// ------------------------------------------------------------------------------------------------
void Renderer::setPerspectiveProjection( double fovY, double aspect, double zNear, double zFar )
{
#if 0
	double zoomFOV = fovY * _zoomFactor;

	setProjection(		
		Math::perspective(zoomFOV,aspect,zNear,zFar) 
		);
#else
	const double tangent = Math::tan( Math::radians(fovY*0.5) )/_zoomFactor;
	const double height  = zNear * tangent;
	const double width   = height * aspect;

	setProjection( 
		Math::frustum( -width,width,-height,height, zNear, zFar )
		);
#endif
}

// ------------------------------------------------------------------------------------------------
void Renderer::setOrthographicProjection( double left, double right, double bottom, double top, double zNear, double zFar )
{
	setProjection( 
		Math::ortho( left,
		             right,
		             bottom,
		             top,
		             zNear,
		             zFar)
		);
}

// ------------------------------------------------------------------------------------------------
void Renderer::setOrthographicProjection()
{
	if( _viewport.z > 0 && _viewport.w > 0 )
	{
		const double aspect = _viewport.z / (double)_viewport.w;
		const double a      = 1.0 / _zoomFactor;
		const double b      = (a / aspect);

		const Math::box3 _viewBox = Math::box3( Math::dvec3(-a,-b,-50.0), Math::dvec3(a,b,50.0) );

		setProjection( 
		Math::ortho( _viewBox.minPoint().x,
		             _viewBox.maxPoint().x,
		             _viewBox.minPoint().y,
		             _viewBox.maxPoint().y,
		             -_viewBox.minPoint().z,
		             -_viewBox.maxPoint().z)
				  );
	}
}

// ------------------------------------------------------------------------------------------------
void Renderer::setScreenSpaceProjection()
{
	setOrthographicProjection( _viewport.x, _viewport.z, _viewport.y, _viewport.w, -50.0, 50.0 );
}

// ------------------------------------------------------------------------------------------------
void Renderer::setProjection( const Math::dmat4 &m )
{
	_projectionMatrix = m;

	glPushAttrib( GL_TRANSFORM_BIT );
	glMatrixMode( GL_PROJECTION );
	glLoadMatrixd( (const double*)&(_projectionMatrix)[0] );
	glPopAttrib();
}

// ------------------------------------------------------------------------------------------------
Math::dmat4 Renderer::projectionMatrix() const { return _projectionMatrix; }

// ------------------------------------------------------------------------------------------------
void Renderer::computeClipMatrix()
{
	_frustum.update(_modelViewMatrix,_projectionMatrix);
}

// ------------------------------------------------------------------------------------------------
void Renderer::computeClipMatrix( const Math::dmat4 &modelViewMatrix, const Math::dmat4 &projectionMatrix )
{
	_frustum.update( modelViewMatrix,projectionMatrix );
}

// ------------------------------------------------------------------------------------------------
void Renderer::drawText()
{
	if( !fonts || _textRenderer.isEmpty() )
		return;

	_textRenderer.update();
	_textRenderer.draw();
}

// -------------------------------------------------------------------------------------
void Renderer::addText( const Math::dvec3 &worldPos, const std::string &text, const Color &color, double size, TextRenderer::TextAlignment alignment )
{
	if( !text.empty() )
		_textRenderer.addTag( TextTag(text,worldPos,color,size,0.0,alignment) );
}

// -------------------------------------------------------------------------------------
void Renderer::addText( const Math::ivec2 &screenPos, const std::string &text, const Color &color, double size, TextRenderer::TextAlignment alignment )
{
	if( !text.empty() )
	{
		Math::dvec2 wp = convertWindowTo2D( screenPos.x,screenPos.y ,false);
		_textRenderer.addTag( TextTag(text,Math::dvec3( wp,0.0 ),color,size,0.0,alignment) );
	}
}

// -------------------------------------------------------------------------------------
Math::dvec3 Renderer::normalizedDeviceCoordinates( const Math::ivec2 &pixelCoordinates, bool flipY ) const
{
	const double x = 2.0 * pixelCoordinates.x / _viewport.z  - 1.0;
	const double y = 2.0 * pixelCoordinates.y / _viewport.w;
	const double z = 1.0;

	return Math::dvec3(x, flipY ? 1.0-y : y-1.0,z);
}

// -------------------------------------------------------------------------------------
//Convert a point from world coords in a window pixel 
Math::dvec3 Renderer::convertWorldToWindow(const Math::dvec3 &p, bool flipY) const
{
	Math::dvec3 q      = p;
	Math::dvec3 winPos = Math::project( q, _modelViewMatrix, _projectionMatrix, _viewport );

	if( flipY )
		winPos.y = _viewport.w - winPos.y - 1;
	
	winPos.z = 0;
	
	return winPos;
}

// -------------------------------------------------------------------------------------
Math::dvec3 Renderer::convertWorldToWindow(const Math::dvec2 &p, bool flipY) const 
{
	Math::dvec3 q      = Math::dvec3(p,0);
	Math::dvec3 winPos = Math::project( q, _modelViewMatrix, _projectionMatrix, _viewport );

	if( flipY )
		winPos.y = _viewport.w - winPos.y - 1;
	
	winPos.z = 0;
	
	return winPos;
}

// ------------------------------------------------------------------------------------------------
//Math::box3 Renderer::convertWorldToWindow(const Math::box3 &b, bool flipY ) const
//{
//	Math::dvec3 minP = b.minPoint();
//	Math::dvec3 maxP = b.maxPoint();
//
//	Math::dvec3 p0 = Math::project( minP, _modelViewMatrix, _projectionMatrix, _viewport );
//	Math::dvec3 p1 = Math::project( maxP, _modelViewMatrix, _projectionMatrix, _viewport );
//
//	if( flipY )
//	{
//		p0.y = _viewport.w - p0.y - 1;
//		p1.y = _viewport.w - p1.y - 1;
//	}
//	
//	p0.z = 0;
//	p1.z = 0;
//
//	return Math::box3( p0,p1 );
//}

// -------------------------------------------------------------------------------------
Math::dvec3 Renderer::convertWindowTo3D(int x, int y, bool flipY) const
{
	Math::dvec3 t(x,y,0);

	if( flipY )
		t.y = _viewport.w - y - 1;

	Math::dvec3 out = Math::unProject( t, _modelViewMatrix, _projectionMatrix, _viewport );
	
	return out;
}

// -------------------------------------------------------------------------------------
void Renderer::convertWindowTo3D(int x, int y, Math::dvec3& n, Math::dvec3& f, bool flipY) const
{
	Math::dvec3 t(x,y,0);
	if( flipY )
		t.y = _viewport.w - y - 1;
	n= Math::unProject( t, _modelViewMatrix, _projectionMatrix, _viewport );
	t.z= 1.;
	f=  Math::unProject( t, _modelViewMatrix, _projectionMatrix, _viewport );
}

// -------------------------------------------------------------------------------------------------
Math::dvec2 Renderer::convertWindowTo2D(int x, int y, bool flipY) const
{
	Math::dvec3 t(x,y,0);

	if( flipY )
		t.y = _viewport.w - y - 1;

	Math::dvec3 out = Math::unProject( t, _modelViewMatrix, _projectionMatrix, _viewport );
	
	return Math::dvec2(out.x,out.y);
}

// ------------------------------------------------------------------------------------------------
bool Renderer::isVisible( const Math::dvec3 &p ) const
{
	return _frustum.isInside(p) != Frustum::FrustumOutside;
}

// ------------------------------------------------------------------------------------------------
bool Renderer::isVisible( const Math::box3  &b ) const
{
	return _frustum.isInside(b) != Frustum::FrustumOutside;
}

// ------------------------------------------------------------------------------------------------
bool Renderer::isVisible( const Math::dvec3 &p0,const Math::dvec3 &p1  ) const
{
	//return Math::lineBoxIntersection( p0,p1, _viewBox.translated(_center) );
	return true;
}

// ------------------------------------------------------------------------------------------------
double Renderer::pixelToWorldSize( float pixels ) const
{
	return ((double)pixels) * 1.0/(_zoomFactor * ((double)_viewport.z)); // tested and working
}

// -------------------------------------------------------------------------------------------------
int Renderer::worldSizeToPixels(double distance) const
{
	const Math::dvec3 t(0,distance,0);

	Math::dvec3 out = Math::project( t, Math::dmat4(1), _projectionMatrix, _viewport );
	out.z = 0.0;

	if( Math::any( Math::isnan(out) ) ||
		Math::any( Math::isinf(out) ) )
		return 0;

	return (int)Math::length(out);
}

// ------------------------------------------------------------------------------------------------
Math::dmat4 Renderer::openSelectionMode(int x, int y)
{
	return _selection.openSelectionMode( Math::ivec2(x,y), _projectionMatrix, _viewport );
}

// ------------------------------------------------------------------------------------------------
void Renderer::closeSelectionMode()
{
	_selection.closeSelectionMode();
}

// ------------------------------------------------------------------------------------------------
bool Renderer::selectionFailed() const
{
	return _selection.failed();
}

// ------------------------------------------------------------------------------------------------
std::vector<unsigned int> Renderer::getSelectionNames( Selection::SelectionMode mode )
{
	std::vector<unsigned int> v;

	switch( mode )
	{
		case Selection::Nearest:
			{
			unsigned int id;
			if(  _selection.getNearestObject(id) )
				v.push_back(id);
			}
			break;

		case Selection::All:
			v = _selection.getObjects();
			break;
	}

	 return v;
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
void Renderer::beginRendering()
{
	glEnableClientState( GL_VERTEX_ARRAY );
	_origin.bind();
	_origin.setModelViewMatrix( _projectionMatrix*_modelViewMatrix );
	_origin.setCameraPosition( _eyePosition );
}

// ------------------------------------------------------------------------------------------------
void Renderer::endRendering()
{
	_origin.unbind();
	glDisableClientState( GL_VERTEX_ARRAY );
}

// ------------------------------------------------------------------------------------------------
void Renderer::setVertex3DArray( const std::vector<Math::dvec3> &vertices, int stride )
{
	_origin.set3DPositions( vertices );
	glVertexPointer( 3, GL_DOUBLE, stride, &vertices[0] );
}

// ------------------------------------------------------------------------------------------------
void Renderer::setVertex2DArray( const std::vector<Math::dvec3> &vertices, int stride )
{
	_origin.set2DPositions( vertices );
	glVertexPointer( 3, GL_DOUBLE, stride, &vertices[0] );
}


// ------------------------------------------------------------------------------------------------
Image *Renderer::readBuffer( unsigned int id, int x, int y, int w, int h)
{
	//@TODO: frame buffer object
	//switch(id) 
	//{}
    
	unsigned char *buffer = new unsigned char[w*h*4];
	Image *target         = new Image(w,h,4,buffer);

	glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	return target;
}
