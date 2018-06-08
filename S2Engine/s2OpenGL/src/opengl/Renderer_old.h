// Renderer.h
//
#ifndef RENDERER_ONCE
#define RENDERER_ONCE

#include "CoreAPI.h"

#include "Color.h"
#include "Math/Math.h"
#include "Math/Box.h"
#include "Selection.h"
#include "FloatingOrigin.h"
#include "TextRenderer.h"

#include "Rendering/Camera.h"
#include "Rendering/Frustum.h"

namespace OpenGL{
class Image;
class Font;

class S2OPENGL_API Renderer
{
private:
	Color bkTop;
	Color bkBottom;
	bool _useGradient;

	double      _zoomFactor;
	Math::dvec3 _eyePosition;
	Math::dvec3 _viewDirection;
	Math::dmat4 _projectionMatrix;
	Math::dmat4 _modelViewMatrix;
	Math::ivec4 _viewport;
	
	Font           *fonts;
	FloatingOrigin _origin;
	Frustum        _frustum;
	Selection      _selection;
	TextRenderer   _textRenderer;
	
	Math::dvec3 computeEyePosition( const Math::dmat4 &modelView ) const ;
	void updateViewBox();

public:
	Renderer();
	~Renderer();

	void setClearColor(const Color &flatColor );
	void setClearColor(const Color &gradientTop, const Color &gradientBottom );	

	void clearBackground();

	Math::dvec3 normalizedDeviceCoordinates( const Math::ivec2 &pixelCoordinates, bool flipY = true ) const;
	
	Math::dvec3 convertWindowTo3D(int x, int y, bool flipY = true ) const ;
	void        convertWindowTo3D(int x, int y, Math::dvec3& near, Math::dvec3& far, bool flipY = true )const;
	Math::dvec2 convertWindowTo2D(int x, int y, bool flipY = true ) const ;
	Math::dvec3 convertWorldToWindow(const Math::dvec3 &p, bool flipY = true ) const ;
	Math::dvec3 convertWorldToWindow(const Math::dvec2 &p, bool flipY = true ) const ;
	//Math::box3  convertWorldToWindow(const Math::box3 &b, bool flipY = true )  const ;
	double	   pixelToWorldSize( float pixels = 1.f) const;
	int        worldSizeToPixels( double worldSize = 1.0) const;
	
	Math::dmat4 openSelectionMode(int x, int y);
	std::vector<unsigned int> getSelectionNames( Selection::SelectionMode mode = Selection::Nearest);
	void closeSelectionMode();
	bool selectionFailed() const;

	void setViewport(int x, int y, int w, int h );
	void setPerspectiveProjection( double fovY, double aspect, double zNear, double zFar );
	void setOrthographicProjection( double left, double right, double bottom, double top, double zNear, double zFar );
	void setOrthographicProjection();
	void setScreenSpaceProjection();
	void setProjection( const Math::dmat4 &m );
	Math::dmat4 projectionMatrix() const;
	
	void setModelView( const Math::dvec3 &eye, const Math::dvec3 &center,const Math::dvec3 &up );
	void setModelView( const Math::dmat4 &m );

	void computeClipMatrix();
	void computeClipMatrix( const Math::dmat4 &modelView, const Math::dmat4 &projectionMatrix );

	Math::dvec3 eyePosition() const;
	Math::dvec3 viewDirection() const;
	Math::dmat4 modelViewMatrix() const;

	void enableFloatingOrigin( bool on = true );
	void setZoom( double zoomValue );
	void zoomInOut( double step/*, const Math::dvec3 &target*/ );
	void fitView( const Math::box3 &box = Math::box3() );
	
	void setFont( Font *f );
	Font *getFont() const {return fonts; }

	void drawText();
	void addText( const Math::dvec3 &worldPos, const std::string &text, const Color &color, double size = 12.0, TextRenderer::TextAlignment alignment = TextRenderer::AlignLeft );
	void addText( const Math::ivec2 &screenPos, const std::string &text, const Color &color, double size = 12.0, TextRenderer::TextAlignment alignment = TextRenderer::AlignLeft );
	
	bool       isVisible( const Math::dvec3 &p ) const;
	bool       isVisible( const Math::dvec3 &p0,const Math::dvec3 &p1  ) const;
	bool       isVisible( const Math::box3  &b ) const;
	
	bool isFloatingOriginEnabled()	const   { return _origin.isEnabled(); }
	Math::ivec4 viewport()			const   { return _viewport; }
	double      zoomFactor()	    const   { return _zoomFactor; }

	// -----------------------------------------------------------------------------------------------
	void beginRendering();
	void setVertex3DArray( const std::vector<Math::dvec3> &vertices, int stride = 0 );
	void setVertex2DArray( const std::vector<Math::dvec3> &vertices, int stride = 0 );
	void setVertex( const Math::ivec2 &v );
	void setVertex( const Math::ivec3 &v );
	void setVertex( const Math::vec2 &v );
	void setVertex( const Math::vec3 &v );
	void setVertex( const Math::dvec2 &v );
	void setVertex( const Math::dvec3 &v );
	void endRendering();

	Image *readBuffer( unsigned int id, int x, int y, int w, int h );
};


}

#endif