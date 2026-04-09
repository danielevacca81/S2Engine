// MainWindow.cpp
//
#include "MainWindow.h"

#include "MyApplication.h"

#include "Application/Application.h"

#include "RenderCore/RenderTarget.h"
#include "RenderCore/Context.h"
#include "RenderCore/Device.h"
#include "RenderCore/RenderCommands.h"
#include "RenderCore/ShaderCompiler.h"

#include "Renderer/RenderMaterial.h"
#include "Renderer/PickPass.h"

#include "Geometry/GeometryFactory3D.h"

#include <iostream>

////// TODO:
// - add keyboard handling
// - computeshaders and opengl 4.6


/* memo:
* 
* resourceManager::registerMesh accept meshdata instead of vertexdata, and create vertexdata internally.
* dynamic mesh
* static mesh
* 
* set uniform value bugged?
*
*
*/

// ------------------------------------------------------------------------------------------------
void MainWindow::loadResources()
{
	struct texturetag
	{
		std::string           name;
		std::filesystem::path path;
	};

	std::array<texturetag, 5> texturesToLoad = 
	{
		texturetag{ "pbr_albedo",   R"(F:\Sviluppo\Materials\group14\g5\g5_basecolor.png)" },
		texturetag{ "pbr_normal",   R"(F:\Sviluppo\Materials\group14\g5\g5_normal.png)" },
		texturetag{ "pbr_metallic", R"(F:\Sviluppo\Materials\group14\g5\g5_metallic.png)" },
		texturetag{ "pbr_roughness",R"(F:\Sviluppo\Materials\group14\g5\g5_roughness.png)" },
		texturetag{ "pbr_ao",       R"(F:\Sviluppo\Materials\group14\g5\g5_ao.png)" }
	};



	auto& resourceManager = _renderer->resources();

	for( const auto& tex : texturesToLoad )
	{
		if( auto handle = resourceManager.registerTexture( tex.name, tex.path ) )
			std::cout << "Loaded texture: " << tex.name << std::endl;
		else
		{
			std::cout << "Failed to load texture: " << tex.name << std::endl;
			return;
		}
	}

	auto vtx = s2::RenderCore::ShaderCompiler::compile( s2::RenderCore::ShaderStageType::Vertex,
		R"(
			#version 450 core
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			layout(location = 2) in vec3 a_Normal;
			layout(location = 3) in vec2 a_TexCoord;
			
			uniform mat4 u_ModelViewProjectionMatrix;
			uniform mat4 u_ModelViewMatrix;
			uniform mat3 u_NormalMatrix;
			
			out VS_OUT {
				vec3 FragPos;
				vec3 Normal;
				vec2 TexCoord;
				vec4 Color;
			} vs_out;
			
			void main()
			{
				vec4 viewPos = u_ModelViewMatrix * vec4(a_Position, 1.0);
				vs_out.FragPos = viewPos.xyz;
				vs_out.Normal = normalize(u_NormalMatrix * a_Normal);
				vs_out.TexCoord = a_TexCoord;
				vs_out.Color = a_Color;
				gl_Position = u_ModelViewProjectionMatrix * vec4(a_Position, 1.0);
			}
		)"
	);

	// PBR fragment shader
	auto fragPBR = s2::RenderCore::ShaderCompiler::compile( s2::RenderCore::ShaderStageType::Fragment,
	R"(
		#version 450 core
		
		const float PI = 3.14159265359;
		
		in VS_OUT {
			vec3 FragPos;
			vec3 Normal;
			vec2 TexCoord;
			vec4 Color;
		} fs_in;
		
		out vec4 FragColor;
		
		// Material properties
		uniform vec3 u_Albedo;
		uniform float u_Metallic;
		uniform float u_Roughness;
		uniform float u_AO;
		
		// Texture maps
		layout(binding = 0) uniform sampler2D u_AlbedoMap;
		layout(binding = 1) uniform sampler2D u_NormalMap;
		layout(binding = 2) uniform sampler2D u_MetallicMap;
		layout(binding = 3) uniform sampler2D u_RoughnessMap;
		layout(binding = 4) uniform sampler2D u_AOMap;
		
		// Texture usage flags
		uniform bool u_UseAlbedoMap;
		uniform bool u_UseNormalMap;
		uniform bool u_UseMetallicMap;
		uniform bool u_UseRoughnessMap;
		uniform bool u_UseAOMap;
		
		// Single light source (no arrays)
		uniform vec3 u_LightPosition;
		uniform vec3 u_LightColor;
		uniform float u_LightIntensity;
		
		uniform vec3 u_CamPos;
		
		// Normal Distribution Function (GGX/Trowbridge-Reitz)
		float DistributionGGX(vec3 N, vec3 H, float roughness)
		{
			float a = roughness * roughness;
			float a2 = a * a;
			float NdotH = max(dot(N, H), 0.0);
			float NdotH2 = NdotH * NdotH;
			
			float num = a2;
			float denom = (NdotH2 * (a2 - 1.0) + 1.0);
			denom = PI * denom * denom;
			
			return num / denom;
		}
		
		// Geometry Function (Schlick-GGX)
		float GeometrySchlickGGX(float NdotV, float roughness)
		{
			float r = (roughness + 1.0);
			float k = (r * r) / 8.0;
			
			float num = NdotV;
			float denom = NdotV * (1.0 - k) + k;
			
			return num / denom;
		}
		
		float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
		{
			float NdotV = max(dot(N, V), 0.0);
			float NdotL = max(dot(N, L), 0.0);
			float ggx2 = GeometrySchlickGGX(NdotV, roughness);
			float ggx1 = GeometrySchlickGGX(NdotL, roughness);
			
			return ggx1 * ggx2;
		}
		
		// Fresnel-Schlick approximation
		vec3 fresnelSchlick(float cosTheta, vec3 F0)
		{
			return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
		}
		
		// Get normal from normal map
		vec3 getNormalFromMap()
		{
			vec3 tangentNormal = texture(u_NormalMap, fs_in.TexCoord).xyz * 2.0 - 1.0;
			
			vec3 Q1 = dFdx(fs_in.FragPos);
			vec3 Q2 = dFdy(fs_in.FragPos);
			vec2 st1 = dFdx(fs_in.TexCoord);
			vec2 st2 = dFdy(fs_in.TexCoord);
			
			vec3 N = normalize(fs_in.Normal);
			vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
			vec3 B = -normalize(cross(N, T));
			mat3 TBN = mat3(T, B, N);
			
			return normalize(TBN * tangentNormal);
		}
		
		void main()
		{
			// Sample material properties
			vec3 albedo = u_UseAlbedoMap 
				? pow(texture(u_AlbedoMap, fs_in.TexCoord).rgb, vec3(2.2)) 
				: u_Albedo;
			
			// Mix with vertex color if available
			//albedo *= fs_in.Color.rgb;
			
			float metallic = u_UseMetallicMap 
				? texture(u_MetallicMap, fs_in.TexCoord).r 
				: u_Metallic;
			
			float roughness = u_UseRoughnessMap 
				? texture(u_RoughnessMap, fs_in.TexCoord).r 
				: u_Roughness;
			
			float ao = u_UseAOMap 
				? texture(u_AOMap, fs_in.TexCoord).r 
				: u_AO;
			
			// Get normal
			vec3 N = u_UseNormalMap ? getNormalFromMap() : normalize(fs_in.Normal);
			vec3 V = normalize(u_CamPos - fs_in.FragPos);
			
			// Calculate reflectance at normal incidence
			vec3 F0 = vec3(0.04);
			F0 = mix(F0, albedo, metallic);
			
			// Calculate per-light radiance
			vec3 L = normalize(u_LightPosition - fs_in.FragPos);
			vec3 H = normalize(V + L);
			float distance = length(u_LightPosition - fs_in.FragPos);
			float attenuation = 1.0 / (distance * distance);
			vec3 radiance = u_LightColor * u_LightIntensity * attenuation;
			
			// Cook-Torrance BRDF
			float NDF = DistributionGGX(N, H, roughness);
			float G = GeometrySmith(N, V, L, roughness);
			vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
			
			vec3 kS = F;
			vec3 kD = vec3(1.0) - kS;
			kD *= 1.0 - metallic;
			
			vec3 numerator = NDF * G * F;
			float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
			vec3 specular = numerator / denominator;
			
			// Calculate outgoing radiance Lo
			float NdotL = max(dot(N, L), 0.0);
			vec3 Lo = (kD * albedo / PI + specular) * radiance * NdotL;
			
			// Ambient lighting (simple approximation)
			vec3 ambient = vec3(0.03) * albedo * ao;
			vec3 color = ambient + Lo;
			
			// HDR tonemapping (Reinhard)
			color = color / (color + vec3(1.0));
			
			// Gamma correction
			color = pow(color, vec3(1.0/2.2));
			
			FragColor = vec4(color, 1.0);
		}
	)"
	);

	if( vtx && fragPBR )
	{
		auto pbrShader = s2::RenderCore::Shader::New();
		pbrShader->attachVertexShaderStage( vtx.stage );
		pbrShader->attachFragmentShaderStage( fragPBR.stage );
		
		auto linkResult = s2::RenderCore::ShaderCompiler::linkShader( pbrShader, "PBR_Shader" );
		if( linkResult.success )
		{
			auto& resources = _renderer->resources();
			auto shaderHandle = resources.registerShader( "pbr", pbrShader );
			
			// Setup PBR material
			_materialPBR.shader = shaderHandle;
			
			// Texture usage flags
			_materialPBR.set( "u_UseAlbedoMap", true );
			_materialPBR.set( "u_UseNormalMap", true );
			_materialPBR.set( "u_UseMetallicMap", true );
			_materialPBR.set( "u_UseRoughnessMap", true );
			_materialPBR.set( "u_UseAOMap", true );
			
			// set textures to the material
			_materialPBR.setTexture( "u_AlbedoMap",    resourceManager.texture( "pbr_albedo" ) );
			_materialPBR.setTexture( "u_NormalMap",    resourceManager.texture( "pbr_normal" ) );
			_materialPBR.setTexture( "u_MetallicMap",  resourceManager.texture( "pbr_metallic" ) );
			_materialPBR.setTexture( "u_RoughnessMap", resourceManager.texture( "pbr_roughness" ) );
			_materialPBR.setTexture( "u_AOMap",        resourceManager.texture( "pbr_ao" ) );
			
			// Setup single light
			_materialPBR.set( "u_LightIntensity", 100.0f );
			
			std::cout << "PBR Shader compiled and linked successfully" << std::endl;
		}
		else
		{
			std::cout << "Failed to link PBR shader: " << linkResult.errorLog << std::endl;
		}
	}
	else
	{
		if( !vtx )
			std::cout << "Failed to compile PBR vertex shader" << std::endl;
		if( !fragPBR )
			std::cout << "Failed to compile PBR fragment shader" << std::endl;
	}
}

// ------------------------------------------------------------------------------------------------
void MainWindow::onInitializeEvent()
{
	auto pipeline = s2::Renderer::RenderPipeline::createForwardPipeline().addPass( std::make_shared<s2::Renderer::PickPass>() );

	// Initialize renderer with the current rendering context and default render pipeline
	_renderer = std::make_unique<s2::Renderer::Renderer>( _renderingContext.get(), pipeline );
	
	// Initialize the picker and connect to pick results
	_picker = std::make_unique<s2::Renderer::Picker>( *_renderer.get() );
	_picker->onObjectHit( []( const s2::Renderer::PickResult& result )
	{
		if( result.isHit() )
		{
			std::cout << std::dec
				<< "Pick Result - Object ID: " << result.objectID
				<< ", Primitive ID: " << result.primitiveID
				<< ", Screen Pos: (" << result.screenPos.x << ", " << result.screenPos.y << ")"
				<< std::endl;
		}
		else
		{
			std::cout << "No valid pick result for position (" << result.screenPos.x << ", " << result.screenPos.y << ")" << std::endl;
		}
	} );


	loadResources();


	auto& resources = _renderer->resources();
	
	// register cube mesh
	{
		resources.registerMesh( "cube", s2::GeometryFactory3D::createCube( { 5.0, 0.0, 0.0 }, 2.0 ) );
	}

	// register torus mesh
	{
		const auto torus = resources.registerMesh( "torus", s2::GeometryFactory3D::createTorus( 1.0, 0.5, 64, 16 ) );
		resources.mesh( torus )->setColor( Color::red() );
	}


	// register cone mesh
	{
		const auto cone = resources.registerMesh( "cone", s2::GeometryFactory3D::createCone( Math::dvec3( 2.5, 0.0, 0.0 ), Math::dvec3( 2.5, 0.0, 3.0 ), 1, true, 32 ) );
		resources.mesh( cone )->setColor( Color::yellow() );
	}

	// register sphere mesh
	{
		const auto sphere = resources.registerMesh( "sphere", s2::GeometryFactory3D::createSphere( Math::dvec3( -2.5, 0.0, 0.0 ), 1.0, 32 ) );
		resources.mesh( sphere )->setColor( Color::blue().lighter() );
	}

	// register cylinder mesh
	{
		const auto cyl = resources.registerMesh( "cylinder", s2::GeometryFactory3D::createCylinder( Math::dvec3( -5.0, 0.0, 0.0 ), Math::dvec3( -5.0, 0.0, 2.0 ), 1.0, true, true, 32 ) );
		resources.mesh( cyl )->setColor( Color::cyan() );
	}

	// register capsule mesh
	{
		const auto capsule = resources.registerMesh( "capsule", s2::GeometryFactory3D::createCapsule( Math::dvec3( -2.0, -3.0, 0.0 ), Math::dvec3( 2.0, -3.0, 3.0 ), 1.0, 32, 32 ) );
		resources.mesh( capsule )->setColor( Color::magenta() );
	}

	_material.shader = resources.registerShader( "blinnPhong", s2::RenderCore::DefaultShaders.BlinnPhong );
	// _material.setTexture( "u_DiffuseMap", (int) resources.registerTexture( "texture",
	// 					  s2::RenderCore::Texture2D::New(
	// 					  s2::RenderCore::TextureDescription(
	// 					  _texture.pixmap.width(),
	// 					  _texture.pixmap.height(),
	// 					  s2::RenderCore::TextureFormat::RedGreenBlue8 ),
	// 					  (void*) _texture.pixmap.pixels() ) ) );
	
	_material.set( "u_UseDiffuseMap", false );

	_camera.set( Math::dvec3( 0.0, 0.0, 8.0 ),
				 Math::dvec3( 0.0, 0.0, 0.0 ),
				 Math::dvec3( 0.0, 1.0, 0.0 )
	);

	//_trackball.setRadius( 1.0 );
	_trackball.setCenter( Math::ivec2( width() / 2, height() / 2 ) );

	//_trackballLight.setRadius( 1.0 );
	_trackballLight.setCenter( Math::ivec2( width() / 2, height() / 2 ) );
}

// ------------------------------------------------------------------------------------------------
void MainWindow::onShutdownEvent()
{
	_renderer.reset();
	_picker.reset();
}

// ------------------------------------------------------------------------------------------------
void MainWindow::onResizeEvent( uint32_t width, uint32_t height )
{
	const auto vp = Math::irect( 0, 0, width, height );
	if( vp.isEmpty() )
		return;

	_camera.setViewport( vp );
	_camera.setProjectionTransform( Math::ProjectionTransform::createPerspective( width / (double) height, 45.0, 0.1, 100.0 ) );

	_trackball.resize( width, height );
	_trackballLight.resize( width, height );
}


// ------------------------------------------------------------------------------------------------
void MainWindow::onCloseEvent()
{
}

// ------------------------------------------------------------------------------------------------
void MainWindow::onPaintEvent()
{
	// Scene.draw
	auto app = static_cast<MyApplication*>( s2::Application::instance() );

	const auto scale = app->scaleFactor;
	const auto lightPosition = app->lightPosition;

	auto& resources = _renderer->resources();

	using namespace s2::Renderer;

	// Setup PBR material lighting
	_materialPBR.set( "u_LightPosition", Math::vec3( _trackballLight.matrix() * lightPosition ) );
	_materialPBR.set( "u_LightColor", Math::vec3( 1.0f, 1.0f, 1.0f ) );
	_materialPBR.set( "u_CamPos", Math::vec3( _camera.position() ) );


	// setup material properties and shader
	// note: no need to do this every frame if the material properties are static.
	// we can create a material instance once and reuse it for multiple draw calls and update it only when properties change.
	_material.set("u_LightPosition" , Math::vec4(_trackballLight.matrix() * lightPosition) );
	_material.set("u_LightAmbient"  , Math::vec4{ .01f,.01f,.01f,1.f });
	_material.set("u_LightDiffuse"  , Math::vec4{ 1.f,1.f,1.f,1.f });
	_material.set("u_LightSpecular" , Math::vec4{ 1.f,1.f,1.f,1.f });
	_material.set("u_LightShininess", 60.f);

	auto modelMatrix = Math::scale( Math::dvec3( scale ) ) * _trackball.matrix();

	_renderer->beginFrame( {
		.mainTarget             = _renderTarget.get(),
		.cameraViewMatrix       = _camera.worldToCameraMatrix(),
		.cameraProjectionMatrix = _camera.projectionMatrix(),
						   } );
	{
		_renderer->clear( { .color = Color{ 0.3f, 0.4f, 0.5f, 1.0f } } );

		s2::Renderer::RenderCommand cmd
		{
			.renderMode  = s2::Renderer::RenderMode::Triangles,
			.material    = _materialPBR,
			.mesh        = resources.mesh( "cube" ),
			.pickableID = 1, // assign a unique ID for picking
			.modelMatrix = modelMatrix,
		};
		_renderer->render( cmd );

		_renderer->render(
			{
			.renderMode  = s2::Renderer::RenderMode::Triangles,
			.material    = _materialPBR,
			.mesh        = resources.mesh( "sphere" ),
			.pickableID  = 2, // assign a unique ID for picking
			.modelMatrix = modelMatrix,
			} );

		_renderer->render(
			{
			.renderMode  = s2::Renderer::RenderMode::Triangles,
			.material    = _materialPBR,
			.mesh        = resources.mesh( "torus" ),
			.pickableID  = 3, // assign a unique ID for picking
			.modelMatrix = modelMatrix,
			} );

		_renderer->render(
			{
			.renderMode  = s2::Renderer::RenderMode::Triangles,
			.material    = _materialPBR,
			.mesh        = resources.mesh( "cone" ),
			.pickableID  = 4, // assign a unique ID for picking
			.modelMatrix = modelMatrix,
			} );

		_renderer->render(
			{
			.renderMode  = s2::Renderer::RenderMode::Triangles,
			.material    = _materialPBR,
			.mesh        = resources.mesh( "cylinder" ),
			.pickableID  = 5, // assign a unique ID for picking
			.modelMatrix = modelMatrix,
			} );

		_renderer->render(
			{
			.renderMode  = s2::Renderer::RenderMode::Triangles,
			.material    = _materialPBR,
			.mesh        = resources.mesh( "capsule" ),
			.pickableID  = 6, // assign a unique ID for picking
			.modelMatrix = modelMatrix,
			} );
	}
	_renderer->endFrame();
}

// ------------------------------------------------------------------------------------------------
void MainWindow::onMouseMoveEvent( const s2::Input::MouseState& ms )
{
	// handle dragging of light trackball
	//if( ms.isDragging() && ms.isButtonDown( s2::Input::MouseState::ButtonRight ) )
	//	_trackballLight.update( Scene::TrackBall::DragEvent::Update, ms.position() );


	// handle dragging of object trackball
	if( ms.isDragging() && ms.isButtonDown( s2::Input::MouseState::ButtonRight ) )
		_trackball.update( Scene::TrackBall::DragEvent::Update, ms.position() );

	//	ms.dumpStatus( "onMouseMoveEvent" );
}

// ------------------------------------------------------------------------------------------------
void MainWindow::onMouseDoubleClickEvent( const s2::Input::MouseState& ms )
{
	//	ms.dumpStatus( "onMouseDoubleClickEvent" );
}

// ------------------------------------------------------------------------------------------------
void MainWindow::onMouseButtonEvent( const s2::Input::MouseState& ms )
{
	//	ms.dumpStatus( "onMouseButtonEvent" );
	//if( ms.isButtonDown( s2::Input::MouseState::ButtonLeft) )		_trackball.update( Scene::TrackBall::DragEvent::Begin, ms.position() );
	//else if( ms.isButtonUp( s2::Input::MouseState::ButtonLeft ) )   _trackball.update( Scene::TrackBall::DragEvent::End, ms.position() );
	
	if( ms.isButtonDown( s2::Input::MouseState::ButtonLeft ) )
		_picker->pickObjectAt( ms.position() );

	if( ms.isButtonDown( s2::Input::MouseState::ButtonRight ) )    _trackball.update( Scene::TrackBall::DragEvent::Begin, ms.position() );
	else if( ms.isButtonUp( s2::Input::MouseState::ButtonRight ) ) _trackball.update( Scene::TrackBall::DragEvent::End, ms.position() );
}

// ------------------------------------------------------------------------------------------------
void MainWindow::onMouseScrollEvent( const s2::Input::MouseState& ms )
{
	//ms.dumpStatus( "onMouseScrollEvent" );
	auto app = static_cast<MyApplication*>( s2::Application::instance() );
	app->scaleFactor *= std::pow( 1.2, ms.wheel() );

	// Optionally add limits to prevent extreme scaling
	app->scaleFactor = std::max( 0.1, std::min( 10.0, app->scaleFactor ) );
}