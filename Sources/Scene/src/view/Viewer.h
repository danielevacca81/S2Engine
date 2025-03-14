// Viewer.h
//  @author Ivan Miraglia
//
#ifndef RENDERENGINE_VIEWER_H
#define RENDERENGINE_VIEWER_H

#include "RenderPipeline_API.h"

#include "Camera.h"
#include "ViewOrthographic.h"
#include "ViewPerspective.h"
#include "View.h"

#include "Math/Math.h"
#include "Math/Rectangle.h"

#include <functional>
#include <vector>

namespace RenderPipeline {

// ------------------------------------------------------------------------------------------------
class RENDERPIPELINE_API Viewer
{
public:
	using Constraint = std::function< void( Viewer * ) >;

public:
	virtual ~Viewer() = default;

	virtual void      reset() { *this = Viewer(); }
					    
	void              addConstraint( const Constraint &constraint )      { _constraints.push_back( constraint ); }
	void              clearConstraints()                                 { _constraints.clear(); }
					    
	// setter   		 			  
	void			  setViewport( const Math::irect  &viewportRect ) { _viewport = viewportRect; applyConstraints(); }
	void			  setCamera	( const Camera       &camera       ) { _camera   = camera;       applyConstraints();}
					    
	void			  setCameraDirectMatrix( const Math::dmat4 &matrix );
	void              moveToLookAt( const Math::dvec3 &pos )             { _camera.moveToLookAt( pos ); applyConstraints(); }
	void			  setLocalSphericalCoordinate( const Math::dvec3 &origin, 
												   const double theta, const double phi, const double rho );

	// getter			   
	const Math::irect &viewport     () const { return _viewport; }
	const Camera	  &camera       () const { return _camera; }
						   
	Math::dvec2       viewportSize () const;
	Math::dvec2       viewportLocalCenter() const { return viewportSize() * 0.5; }
	Math::dmat4		  cameraDirectMatrix() const { return Math::inverse( _camera.matrix() ); }

	// operation
	bool			  equals( const Viewer& o ) const;

protected:
	void              applyConstraints();

protected:
	std::vector< Constraint > _constraints;
	bool	                  _applyConstraintsInProgress {false};

	Math::irect _viewport;
	Camera		_camera;
};


// ------------------------------------------------------------------------------------------------
class RENDERPIPELINE_API ViewerOrthographic : public Viewer
{
public:
	void                     reset() override { *this = ViewerOrthographic(); }

	// setter   						  
	void					 setViewOrthographic( const ViewOrthographic &viewOrtho ) { _ortho = viewOrtho; applyConstraints(); }

	// getter
	const ViewOrthographic	&viewOrthographic          () const { return _ortho; } 

	View				     view                      () const;

	// operation
	bool					 equals( const ViewerOrthographic& o ) const;

protected:
	ViewOrthographic _ortho;
};


// ------------------------------------------------------------------------------------------------
class RENDERPIPELINE_API Viewer2D : public ViewerOrthographic
{
public: 
	void                     reset() override { *this = Viewer2D(); }

	// setter   
	void                     setZoomFactor( double factor );
	bool                     fitToBox( const Math::dbox3 &b );
	bool                     fillByBox( const Math::dbox3 &b );

	// getter
	double                   zoomFactor()     const { return 2.0 / (_ortho.rightPlane()-_ortho.leftPlane()); }
	Math::dbox3               viewBox()        const;
	Math::dbox3               rotatedViewBox() const;
	Math::dbox3               viewAABB()       const;
};


// ------------------------------------------------------------------------------------------------
class RENDERPIPELINE_API ViewerPerspective : public Viewer
{
public:
	enum class FovId
	{
		Horizontal,
		Vertical,
		Min,
		Max,
	};

public:
	void                     reset() override { *this = ViewerPerspective(); }

	// setter   						  
	void					 setViewPerspective( const ViewPerspective &viewPerspective ) { _perspective = viewPerspective; applyConstraints(); }

	void                     setNearFarPlanes( const double nearPlane, const double farPlane ) { _perspective.setPlanes( nearPlane, farPlane ); applyConstraints(); }
	void                     setViewportAndFov( const int width, const int height, const FovId fovId, const double fov );

	// getter
	const ViewPerspective	&viewPerspective          () const { return _perspective; } 

	View				     view                     () const;
	double					 focalLengthPixel         () const;

	// operation
	bool					 equals( const ViewerPerspective& o ) const;

protected:
	ViewPerspective		_perspective;
};


// ------------------------------------------------------------------------------------------------
class RENDERPIPELINE_API Viewer3D : public ViewerPerspective
{
public:
	enum class BoxAdaptationPolicy { Fill, Fit };

public: 
	void  reset() override { *this = Viewer3D(); }

	// setter   
	void  setZoomFactor( double factor );
	void  setCameraTopDownAdaptToBox( const Math::dbox3 &b, const BoxAdaptationPolicy &adaptPolicy );

	// getter
	double zoomFactor() const;
	double zoomDist( const double zoomFactor ) const;
	bool   depth0PlaneAtScreenPos( const Math::ivec2 &screenCoord, Math::dvec3 &depth0WorldPoint ) const;
	bool   depth0PlaneAtScreenPos( const Math::dvec2 &screenCoord, Math::dvec3 &depth0WorldPoint ) const;
	bool   depth0LocalSphericalCoordinate( Math::dvec3 &depth0Origin, 
															 double &theta, double &phi, double &rho ) const;

protected:
	Math::dbox3 trueBoxByAdaptationPolicy( const Math::dbox3 &b, const BoxAdaptationPolicy adaptPolicy ) const;
};

}

#endif // RENDERENGINE_VIEWER_H