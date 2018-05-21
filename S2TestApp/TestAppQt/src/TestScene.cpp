// TestScene.cpp
//
#include "TestScene.h"

#include "VObjects/VLine.h"
#include "VObjects/VCircle.h"

#include "S2Qt/UserInteractionManager.h"
#include "S2Qt/MouseStatus.h"

#include "Renderer/TextRenderer.h"
//#include "Fonts/FontNew.h"

//#include "OpenGL/Uniform.h"

//#include "Common/StringTools.h"
#include "Resources/R.h"

#include "Math/Ray.h"
#include "Math/Plane.h"

//#include "Graphics/Font.h"

#include <QPainter>

// ------------------------------------------------------------------------------------------------
TestScene::TestScene( QWidget *parent )
: GLGraphicsScene(parent)
, _wireframe(false)
, _viewState( OpenGL::ViewState::Perspective )
//, _fonts("")
{
	postInitialization();
}

// ------------------------------------------------------------------------------------------------
TestScene::~TestScene()
{
	//delete _objectManager;
	//delete _cadEngine;
}

// ------------------------------------------------------------------------------------------------
void TestScene::postInitialization()
{
	makeCurrent();

	initContext();
	initShaders();
	initFonts();

//	s2::FontsLibrary::init(L"./fonts");
	
	_textRenderer.init();

	//_textRenderer.addTag( s2::TextRenderer::TextTag( Math::ivec2(0,0),L"Watashi: わたし - 私",s2::Color::red(),"Arial.ttf",0, s2::TextRenderer::AlignLeft) );
	
	_center = Math::dvec3(0,0,0);
	_camera.set( Math::dvec3(0,0,4), Math::dvec3(0,0,0), Math::dvec3(0,1,0));

	createMeshes();
	createObjects();
}

// ------------------------------------------------------------------------------------------------
void TestScene::createObjects()
{
	s2::VLinePtr line = s2::VLine::New( Math::dvec3(0,0,0), Math::dvec3(5,5,5) );
	
	line->setName( "Original" );

	_objectManager.addObject(line);

	line->setName( "Renamed" );
	line->setState( s2::VObject::State() );
}

// ------------------------------------------------------------------------------------------------
void TestScene::createMeshes()
{
	std::vector< Math::vec3 > pts;
	std::vector< s2::Color  > colors;
	std::vector< Math::vec3 > normals;
	
	// left
	pts.push_back( Math::vec3(-1.0f,-1.0f,-1.0f) ); colors.push_back( s2::Color::red().darker() ); normals.push_back( Math::vec3(-1.0f, 0.0f, 0.0f) );
	pts.push_back( Math::vec3(-1.0f,-1.0f, 1.0f) ); colors.push_back( s2::Color::red().darker() ); normals.push_back( Math::vec3(-1.0f, 0.0f, 0.0f) );
	pts.push_back( Math::vec3(-1.0f, 1.0f, 1.0f) ); colors.push_back( s2::Color::red().darker() ); normals.push_back( Math::vec3(-1.0f, 0.0f, 0.0f) );
	pts.push_back( Math::vec3(-1.0f,-1.0f,-1.0f) ); colors.push_back( s2::Color::red().darker() ); normals.push_back( Math::vec3(-1.0f, 0.0f, 0.0f) );
	pts.push_back( Math::vec3(-1.0f, 1.0f, 1.0f) ); colors.push_back( s2::Color::red().darker() ); normals.push_back( Math::vec3(-1.0f, 0.0f, 0.0f) );
	pts.push_back( Math::vec3(-1.0f, 1.0f,-1.0f) ); colors.push_back( s2::Color::red().darker() ); normals.push_back( Math::vec3(-1.0f, 0.0f, 0.0f) );

	// back
	pts.push_back( Math::vec3( 1.0f, 1.0f,-1.0f) ); colors.push_back( s2::Color::blue().darker() ); normals.push_back( Math::vec3( 0.0f, 0.0f,-1.0f) );
	pts.push_back( Math::vec3(-1.0f,-1.0f,-1.0f) ); colors.push_back( s2::Color::blue().darker() );	normals.push_back( Math::vec3( 0.0f, 0.0f,-1.0f) );
	pts.push_back( Math::vec3(-1.0f, 1.0f,-1.0f) ); colors.push_back( s2::Color::blue().darker() );	normals.push_back( Math::vec3( 0.0f, 0.0f,-1.0f) );
	pts.push_back( Math::vec3( 1.0f, 1.0f,-1.0f) ); colors.push_back( s2::Color::blue().darker() );	normals.push_back( Math::vec3( 0.0f, 0.0f,-1.0f) );
	pts.push_back( Math::vec3( 1.0f,-1.0f,-1.0f) ); colors.push_back( s2::Color::blue().darker() );	normals.push_back( Math::vec3( 0.0f, 0.0f,-1.0f) );
	pts.push_back( Math::vec3(-1.0f,-1.0f,-1.0f) ); colors.push_back( s2::Color::blue().darker() );	normals.push_back( Math::vec3( 0.0f, 0.0f,-1.0f) );

	// bottom
	pts.push_back( Math::vec3( 1.0f,-1.0f, 1.0f) ); colors.push_back( s2::Color::green().darker() ); normals.push_back( Math::vec3( 0.0f,-1.0f, 0.0f) );
	pts.push_back( Math::vec3(-1.0f,-1.0f,-1.0f) ); colors.push_back( s2::Color::green().darker() ); normals.push_back( Math::vec3( 0.0f,-1.0f, 0.0f) );
	pts.push_back( Math::vec3( 1.0f,-1.0f,-1.0f) ); colors.push_back( s2::Color::green().darker() ); normals.push_back( Math::vec3( 0.0f,-1.0f, 0.0f) );
	pts.push_back( Math::vec3( 1.0f,-1.0f, 1.0f) ); colors.push_back( s2::Color::green().darker() ); normals.push_back( Math::vec3( 0.0f,-1.0f, 0.0f) );
	pts.push_back( Math::vec3(-1.0f,-1.0f, 1.0f) ); colors.push_back( s2::Color::green().darker() ); normals.push_back( Math::vec3( 0.0f,-1.0f, 0.0f) );
	pts.push_back( Math::vec3(-1.0f,-1.0f,-1.0f) ); colors.push_back( s2::Color::green().darker() ); normals.push_back( Math::vec3( 0.0f,-1.0f, 0.0f) );

	// front
	pts.push_back( Math::vec3(-1.0f, 1.0f, 1.0f) ); colors.push_back( s2::Color::blue().lighter() ); normals.push_back( Math::vec3( 0.0f, 0.0f, 1.0f) );
	pts.push_back( Math::vec3(-1.0f,-1.0f, 1.0f) ); colors.push_back( s2::Color::blue().lighter() ); normals.push_back( Math::vec3( 0.0f, 0.0f, 1.0f) );
	pts.push_back( Math::vec3( 1.0f,-1.0f, 1.0f) ); colors.push_back( s2::Color::blue().lighter() ); normals.push_back( Math::vec3( 0.0f, 0.0f, 1.0f) );
	pts.push_back( Math::vec3( 1.0f, 1.0f, 1.0f) ); colors.push_back( s2::Color::blue().lighter() ); normals.push_back( Math::vec3( 0.0f, 0.0f, 1.0f) );
	pts.push_back( Math::vec3(-1.0f, 1.0f, 1.0f) ); colors.push_back( s2::Color::blue().lighter() ); normals.push_back( Math::vec3( 0.0f, 0.0f, 1.0f) );
	pts.push_back( Math::vec3( 1.0f,-1.0f, 1.0f) ); colors.push_back( s2::Color::blue().lighter() ); normals.push_back( Math::vec3( 0.0f, 0.0f, 1.0f) );

	// right
	pts.push_back( Math::vec3( 1.0f, 1.0f, 1.0f) ); colors.push_back( s2::Color::red().lighter() );	normals.push_back( Math::vec3( 1.0f, 0.0f, 0.0f) );
	pts.push_back( Math::vec3( 1.0f,-1.0f,-1.0f) ); colors.push_back( s2::Color::red().lighter() );	normals.push_back( Math::vec3( 1.0f, 0.0f, 0.0f) );
	pts.push_back( Math::vec3( 1.0f, 1.0f,-1.0f) ); colors.push_back( s2::Color::red().lighter() );	normals.push_back( Math::vec3( 1.0f, 0.0f, 0.0f) );
	pts.push_back( Math::vec3( 1.0f,-1.0f,-1.0f) ); colors.push_back( s2::Color::red().lighter() );	normals.push_back( Math::vec3( 1.0f, 0.0f, 0.0f) );
	pts.push_back( Math::vec3( 1.0f, 1.0f, 1.0f) ); colors.push_back( s2::Color::red().lighter() );	normals.push_back( Math::vec3( 1.0f, 0.0f, 0.0f) );
	pts.push_back( Math::vec3( 1.0f,-1.0f, 1.0f) ); colors.push_back( s2::Color::red().lighter() );	normals.push_back( Math::vec3( 1.0f, 0.0f, 0.0f) );

	// top 
	pts.push_back( Math::vec3( 1.0f, 1.0f, 1.0f) ); colors.push_back( s2::Color::green().lighter() ); normals.push_back( Math::vec3( 0.0f, 1.0f, 0.0f) );
	pts.push_back( Math::vec3( 1.0f, 1.0f,-1.0f) ); colors.push_back( s2::Color::green().lighter() ); normals.push_back( Math::vec3( 0.0f, 1.0f, 0.0f) );
	pts.push_back( Math::vec3(-1.0f, 1.0f,-1.0f) ); colors.push_back( s2::Color::green().lighter() ); normals.push_back( Math::vec3( 0.0f, 1.0f, 0.0f) );
	pts.push_back( Math::vec3( 1.0f, 1.0f, 1.0f) ); colors.push_back( s2::Color::green().lighter() ); normals.push_back( Math::vec3( 0.0f, 1.0f, 0.0f) );
	pts.push_back( Math::vec3(-1.0f, 1.0f,-1.0f) ); colors.push_back( s2::Color::green().lighter() ); normals.push_back( Math::vec3( 0.0f, 1.0f, 0.0f) );
	pts.push_back( Math::vec3(-1.0f, 1.0f, 1.0f) ); colors.push_back( s2::Color::green().lighter() ); normals.push_back( Math::vec3( 0.0f, 1.0f, 0.0f) );

	_cubeMesh.setVertices( pts );
	_cubeMesh.setColors( colors );
	_cubeMesh.setNormals( normals );
}


// ------------------------------------------------------------------------------------------------
void TestScene::initContext()
{
	makeCurrent();
	_context.initExtensions();
	std::cout << _context.info() << std::endl;
}

// ------------------------------------------------------------------------------------------------
void TestScene::initShaders()
{
	_shader = OpenGL::Program::New();

	_shader->attachVertexShader  ( R::string("Shaders/BlinnPhong.vs") );
	_shader->attachFragmentShader( R::string("Shaders/BlinnPhong.fs") );
	_shader->compile();

	if( !_shader->isLinked() )
	{
		std::cout << _shader->name() << ": " <<_shader->info() << std::endl;
		return;
	}
	
	
	_shader->uniform<Math::vec4>( "u_LightPosition"  )->setValue( Math::vec4(0,0,10,1) );
	_shader->uniform<Math::vec4>( "u_LightAmbient"   )->setValue( Math::vec4(0,0,0,1) );
	_shader->uniform<Math::vec4>( "u_LightDiffuse"   )->setValue( Math::vec4(1,1,1,1) );
	_shader->uniform<Math::vec4>( "u_LightSpecular"  )->setValue( Math::vec4(1,1,0,1) );
	_shader->uniform<float>     ( "u_LightShininess" )->setValue( 127.f         );
}

// ------------------------------------------------------------------------------------------------
void TestScene::initFonts()
{
	//s2::FontsLibrary::init(L"./res/fonts");
}

// ------------------------------------------------------------------------------------------------
void TestScene::drawBackground(QPainter *painter, const QRectF &rect)
{
	makeCurrent();
	
	painter->beginNativePainting();
	_renderer.clear();

	Math::dmat4 m = Math::scale( Math::dmat4(),Math::dvec3(1.0)) * 
		            Math::translate( Math::dmat4(), _camera.target() )*
					_trackball.matrix() *
					Math::translate( Math::dmat4(), -_camera.target() )
					;

	_viewState.view.set( _camera.matrix() * m  );

	OpenGL::DrawState ds( _shader );

	_renderer.draw( OpenGL::Triangles,    _cubeMesh, _viewState, ds );
	///_objectManager.drawObjects( &_renderer );

	_textRenderer.draw( &_renderer );
	painter->endNativePainting();
}

// ------------------------------------------------------------------------------------------------
void TestScene::toggleWireframe()
{
	_wireframe = !_wireframe;
}

// -----------------------------------------------------------------------------------------------
void TestScene::onMousePressed()
{
	const s2Qt::MouseStatus ms = _uim->mouseStatus();

	if( ms.buttonDown() & s2Qt::MouseStatus::ButtonRight )
		_trackball.update( s2::TrackBall::Start_Drag, Math::ivec2( ms.currPos().x(),ms.currPos().y() ) );
	invalidate();
}

// -----------------------------------------------------------------------------------------------
void TestScene::onMouseReleased()
{
	const s2Qt::MouseStatus ms = _uim->mouseStatus();
	
	if( ms.buttonUp() & s2Qt::MouseStatus::ButtonRight )
		_trackball.update( s2::TrackBall::End_Drag, Math::ivec2( ms.currPos().x(),ms.currPos().y() ) );
	invalidate();
}

// -----------------------------------------------------------------------------------------------
void TestScene::onMouseMoved()
{
	const s2Qt::MouseStatus ms = _uim->mouseStatus();

	if( ms.buttonDown() & s2Qt::MouseStatus::ButtonLeft )
	{
		Math::dvec3 p0 = _viewState.worldPoint( ms.prevPos().x(), ms.prevPos().y() );
		Math::dvec3 p1 = _viewState.worldPoint( ms.currPos().x(), ms.currPos().y() );
		Math::dvec3 d  = (p1-p0)*100.0;		

		_camera.set( 
			_camera.position()-d,
			_camera.target()-d,
			_camera.upVector()
			);
	}

	if( ms.buttonDown() & s2Qt::MouseStatus::ButtonRight )
		_trackball.update( s2::TrackBall::Drag, Math::ivec2( ms.currPos().x(),ms.currPos().y() ) );
	
	invalidate();
}

// -----------------------------------------------------------------------------------------------
void TestScene::onMouseWheel()
{
	const s2Qt::MouseStatus ms = _uim->mouseStatus();
	const double z              = ms.wheel();

	_viewState.model = Math::scale( _viewState.model, Math::dvec3(z) );

	//_viewState.camera.zoomToTarget(1.5);
	//const Math::dvec3 pos( currentWorldPoint.x, currentWorldPoint.y, 0.0 );

	//renderer->zoomInOut( z );

	//// get the center of the scene
	//Math::dvec3 target( _camera->center() );
	//Math::dvec3 center( _camera->center() );

	//target = pos;
	//_camera->lookAt( (center - target)/Math::dvec3(z) + target ); //compute the new center position




	invalidate();
}

// -----------------------------------------------------------------------------------------------
void TestScene::onMouseDoubleClick()
{
	const s2Qt::MouseStatus ms = _uim->mouseStatus();
	_camera.set( Math::dvec3(0,0,4), Math::dvec3(0,0,0), Math::dvec3(0,1,0));
	_viewState.model = Math::dmat4();
	_trackball.reset();
	invalidate();
}

// -----------------------------------------------------------------------------------------------
void TestScene::resizeScene( int w, int h )
{
	_viewState.viewport = Math::Rectangle(0,0, w,h);
	
	_viewState.view.setAspectRatio( w/(double)h );
	
	_trackball.resize( w,h );
}