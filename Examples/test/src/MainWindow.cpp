// MainWindow.cpp
//
#include "MainWindow.h"

#include "MyApplication.h"

#include "Application/Application.h"
#include "Application/UILayer.h"

#include "RenderCore/RenderTarget.h"
#include "RenderCore/Context.h"
#include "RenderCore/ShaderCompiler.h"

#include "Renderer/RenderMaterial.h"
#include "Renderer/PickPass.h"
#include "Renderer/ForwardPass.h"
#include "Renderer/UIPass.h"

#include "Core/Log.h"


#include "Geometry/GeometryFactory3D.h"

#include "imgui.h"

#include <iostream>

// TODO:
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
bool MainWindow::loadResources()
{
	//static std::filesystem::path assetBasePath( R"(E:\@Devel\Assets\Meterials)" );  
	//static std::filesystem::path assetBasePath (R"(F:\Sviluppo\Materials\group14)" );  
	static std::filesystem::path assetBasePath (R"(./materials/)" );  
	
	LOG(  Info,"Loading resources from '{}'", assetBasePath.generic_string() );
	
	struct texturetag  
	{  
		std::string           name;
		std::filesystem::path path;
	};  
  
	std::array<texturetag, 5> texturesToLoad =   
	{  
		texturetag{ "pbr_albedo",    assetBasePath / R"(g5/g5_basecolor.png)" },  
		texturetag{ "pbr_normal",    assetBasePath / R"(g5/g5_normal.png)" },  
		texturetag{ "pbr_metallic",  assetBasePath / R"(g5/g5_metallic.png)" },  
		texturetag{ "pbr_roughness", assetBasePath / R"(g5/g5_roughness.png)" },  
		texturetag{ "pbr_ao",        assetBasePath / R"(g5/g5_ao.png)" }  
	};  
  
  
  
	auto& resourceManager = _renderer->resources();  
  
	for( const auto& tex : texturesToLoad )  
	{  
		if( auto handle = resourceManager.registerTexture( tex.name,  tex.path ); handle.isValid() )
			LOG(  Info,"Loaded texture: {} - {:x}", tex.name, handle.handle );
		else
		{
			LOG(  Info,"Failed to load texture: {}", tex.name);
			return false;
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
		#extension GL_ARB_bindless_texture : require

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
		layout(location = 0) uniform sampler2D u_AlbedoMap;
		layout(location = 1) uniform sampler2D u_NormalMap;
		layout(location = 2) uniform sampler2D u_MetallicMap;
		layout(location = 3) uniform sampler2D u_RoughnessMap;
		layout(location = 4) uniform sampler2D u_AOMap;
		
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

	// Outline shader (simple pass to create silhouette)
	auto outVtx = s2::RenderCore::ShaderCompiler::compile( s2::RenderCore::ShaderStageType::Vertex,
	R"(
		#version 450 core
		layout(location = 0) in vec3 a_Position;
		layout(location = 2) in vec3 a_Normal;

		// Use model-view + projection explicitly so extrusion occurs in view space
		uniform mat4 u_ModelViewMatrix;
		uniform mat4 u_ProjectionMatrix;
		uniform float u_OutlineWidth;

		void main()
		{
			// Transform vertex and normal to view (camera) space
			vec4 posView4 = u_ModelViewMatrix * vec4(a_Position, 1.0);
			vec3 posView = posView4.xyz / posView4.w;
			vec3 nView = normalize( mat3(u_ModelViewMatrix) * a_Normal );

			// Extrude in view space so silhouette is stable when camera rotates
			vec3 extruded = posView + nView * u_OutlineWidth;

			// Project using projection matrix
			gl_Position = u_ProjectionMatrix * vec4(extruded, 1.0);
		}
	)"
	);

	auto outFrag = s2::RenderCore::ShaderCompiler::compile( s2::RenderCore::ShaderStageType::Fragment,
	R"(
		#version 450 core
		out vec4 FragColor;
		uniform vec4 u_OutlineColor;
		void main()
		{
			FragColor = u_OutlineColor;
		}
	)"
	);

	// Register PBR shader and outline shader if compilation succeeded
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
			_materialPBRInstance = _materialPBR.createMaterial();
			
			// Texture usage flags
			_materialPBRInstance.set( "u_UseAlbedoMap", true );
			_materialPBRInstance.set( "u_UseNormalMap", true );
			_materialPBRInstance.set( "u_UseMetallicMap", true );
			_materialPBRInstance.set( "u_UseRoughnessMap", true );
			_materialPBRInstance.set( "u_UseAOMap", true );
			
			// set textures to the material
			_materialPBRInstance.set( "u_AlbedoMap",    resourceManager.texture( "pbr_albedo" ) );
			_materialPBRInstance.set( "u_NormalMap",    resourceManager.texture( "pbr_normal" ) );
			_materialPBRInstance.set( "u_MetallicMap",  resourceManager.texture( "pbr_metallic" ) );
			_materialPBRInstance.set( "u_RoughnessMap", resourceManager.texture( "pbr_roughness" ) );
			_materialPBRInstance.set( "u_AOMap",        resourceManager.texture( "pbr_ao" ) );
			
			// Setup single light
			_materialPBRInstance.set( "u_LightIntensity", 100.0f );
			
			LOG(  Info,"PBR Shader compiled and linked successfully");
		}
		else
		{
			LOG(  Info,"Failed to link PBR shader: {}",linkResult.errorLog);
			return false;
		}
	}
	else
	{
		LOG_IF( !vtx, Fatal, "Failed to compile PBR vertex shader" );
		LOG_IF( !fragPBR, Fatal,"Failed to compile PBR fragment shader");

		return false;
	}

	if( outVtx && outFrag )
	{
		auto outShader = s2::RenderCore::Shader::New();
		outShader->attachVertexShaderStage( outVtx.stage );
		outShader->attachFragmentShaderStage( outFrag.stage );

		auto linkResult = s2::RenderCore::ShaderCompiler::linkShader( outShader, "Outline_Shader" );
		if( linkResult.success )
		{
			auto& resources = _renderer->resources();
			auto outlineHandle = resources.registerShader( "outline", outShader );

			_outlineMaterial.shader = outlineHandle;
			_outlineMaterial.state.cullMode = s2::Renderer::CullMode::Front; // render backfaces only
			_outlineMaterial.state.depthWrite = false; // don't overwrite depth

			_outlineMaterialInstance = _outlineMaterial.createMaterial();
			
			// Set defaults for outline material
			_outlineMaterialInstance.set( "u_OutlineColor", Math::fvec4{ 0.7f, 0.5f, 0.f, 0.6f } );
			_outlineMaterialInstance.set( "u_OutlineWidth", 0.02f );
			//_outlineMaterial.blendMode = s2::Renderer::BlendMode::AlphaBlend; // enable blending for transparency
			LOG(  Info,"Outline shader compiled and linked successfully");
		}
		else
		{
			LOG(  Info,"Failed to link Outline shader: {}", linkResult.errorLog);
			return false;
		}
	}
	else
	{
		LOG_IF( !outVtx, Fatal,"Failed to compile Outline vertex shader");
		LOG_IF( !outFrag, Fatal,"Failed to compile Outline fragment shader");

		return false;
	}	

	return true;
}

// ------------------------------------------------------------------------------------------------
void MainWindow::onInitializeEvent()
{
	LOG( Info, "Initialization..." );

	// Initialize renderer with the current rendering context
	_renderer = std::make_unique<s2::Renderer::Renderer>( _renderingContext.get() );

	_renderPasses.emplace( "forward", std::make_shared<s2::Renderer::ForwardPass>() );
	_renderPasses.emplace( "pick", std::make_shared<s2::Renderer::PickPass>() );
	_renderPasses.emplace( "ui", std::make_shared<s2::Renderer::UIPass>() );

	_ui->setEnabled( false );
	
	// Initialize the picker and connect to pick results
	_picker = std::make_unique<s2::Renderer::Picker>( *_renderer.get() );

	// capture 'this' so the lambda can update UI selection state
	_picker->onObjectHit( [this]( const s2::Renderer::PickResult& result )
	{
		if( result.isHit() )
		{
			// populate selection info
			_hasSelection = true;
			_selectedObjectID = result.objectID;
			_selectedPrimitiveID = result.primitiveID;
			_selectedScreenPos = result.screenPos;

			// resolve mesh handle and name (if mapped)
			auto it = _pickableToHandle.find( result.objectID );
			if( it != _pickableToHandle.end() )
			{
				auto handle = it->second;
				auto itname = _handleToName.find( handle );
				_selectedMeshName = ( itname != _handleToName.end() ) ? itname->second : std::string( "<unknown>" );

				// query vertex count from resource manager
				auto vtx = _renderer->resources().mesh( handle );
				_selectedVertexCount = vtx ? vtx->vertexCount() : 0;
			}
			else
			{
				_selectedMeshName = "<none>";
				_selectedVertexCount = 0;
			}

			// request thumbnail update
			_thumbnailNeedsUpdate = true;

			LOG(  Info,
				"Pick Result - Object ID: {}\n"\
				"  Primitive ID: {}\n"\
				"  Screen Pos:   {}, {}\n"\
				"  Mesh:         {}\n"
				"  VtXCout:      {}\n"
				, result.objectID
				, result.primitiveID
				, result.screenPos.x
				, result.screenPos.y
				, _selectedMeshName
				, _selectedVertexCount
				);
		}
		else
		{
			_hasSelection = false;
			_selectedObjectID = 0;
			_selectedPrimitiveID = 0;
			_selectedMeshName.clear();
			_selectedVertexCount = 0;
			_thumbnailNeedsUpdate = true;

			LOG(  Info,"No valid pick result for position ( {} ,{} )", result.screenPos.x, result.screenPos.y);
		}
	} );


	if (!loadResources())
	{
		S2_ASSERT( false, "Failed to load reasources");
		return;
	}

	auto& resources = _renderer->resources();

	// register cube mesh
	{
		auto mesh = s2::GeometryFactory3D::createCube( { 5.0, 0.0, 0.0 }, 2.0 );
		auto handle = resources.registerMesh( "cube", mesh );
		if( handle.isValid() )
			_meshDataCache[handle] = std::move(mesh);
	}

	// register torus mesh
	{
		auto mesh = s2::GeometryFactory3D::createTorus( 1.0, 0.5, 64, 16 );
		auto handle = resources.registerMesh( "torus", mesh );
		if( handle.isValid() )
		{
			resources.mesh( handle )->setColor( s2::Color::red() );
			_meshDataCache[handle] = std::move(mesh);
		}
	}


	// register cone mesh
	{
		auto mesh = s2::GeometryFactory3D::createCone( Math::dvec3( 2.5, 0.0, 0.0 ), Math::dvec3( 2.5, 0.0, 3.0 ), 1, true, 32 );
		auto handle = resources.registerMesh( "cone", mesh );
		if( handle.isValid() )
		{
			resources.mesh( handle )->setColor( s2::Color::yellow() );
			_meshDataCache[handle] = std::move(mesh);
		}
	}

	// register sphere mesh
	{
		auto mesh = s2::GeometryFactory3D::createSphere( Math::dvec3( -2.5, 0.0, 0.0 ), 1.0, 32 );
		auto handle = resources.registerMesh( "sphere", mesh );
		if( handle.isValid() )
		{
			resources.mesh( handle )->setColor( s2::Color::blue().lighter() );
			_meshDataCache[handle] = std::move(mesh);
		}
	}

	// register cylinder mesh
	{
		auto mesh = s2::GeometryFactory3D::createCylinder( Math::dvec3( -5.0, 0.0, 0.0 ), Math::dvec3( -5.0, 0.0, 2.0 ), 1.0, true, true, 32 );
		auto handle = resources.registerMesh( "cylinder", mesh );
		if( handle.isValid() )
		{
			resources.mesh( handle )->setColor( s2::Color::cyan() );
			_meshDataCache[handle] = std::move(mesh);
		}
	}

	// register capsule mesh
	{
		auto mesh = s2::GeometryFactory3D::createCapsule( Math::dvec3( -2.0, -3.0, 0.0 ), Math::dvec3( 2.0, -3.0, 3.0 ), 1.0, 32, 32 );
		auto handle = resources.registerMesh( "capsule", mesh );
		if( handle.isValid() )
		{
			resources.mesh( handle )->setColor( s2::Color::magenta() );
			_meshDataCache[handle] = std::move(mesh);
		}
	}

	// Build pickableID -> handle and handle -> name maps
	// Note: these pickableIDs match the ones used in onDraw()
	_pickableToHandle.clear();
	_handleToName.clear();
	auto h = resources.mesh( "cube" );       if( h.isValid() ) { _pickableToHandle[1] = h; _handleToName[h] = "cube"; }
	h = resources.mesh( "sphere" );          if( h.isValid() ) { _pickableToHandle[2] = h; _handleToName[h] = "sphere"; }
	h = resources.mesh( "torus" );           if( h.isValid() ) { _pickableToHandle[3] = h; _handleToName[h] = "torus"; }
	h = resources.mesh( "cone" );            if( h.isValid() ) { _pickableToHandle[4] = h; _handleToName[h] = "cone"; }
	h = resources.mesh( "cylinder" );        if( h.isValid() ) { _pickableToHandle[5] = h; _handleToName[h] = "cylinder"; }
	h = resources.mesh( "capsule" );         if( h.isValid() ) { _pickableToHandle[6] = h; _handleToName[h] = "capsule"; }

	_material.shader = resources.registerShader( "blinnPhong", s2::RenderCore::DefaultShaders.BlinnPhong );
	_materialInstance = _material.createMaterial();
	
	_materialInstance.set( "u_UseDiffuseMap", false );

	_camera.set( Math::dvec3( 0.0, 0.0, 8.0 ),
				 Math::dvec3( 0.0, 0.0, 0.0 ),
				 Math::dvec3( 0.0, 1.0, 0.0 )
	);

	// thumbnail target (offscreen)
	_thumbnailTarget = std::make_unique<s2::RenderCore::RenderTarget>();
	_thumbnailTarget->createAttachment( s2::RenderCore::FrameBuffer::AttachmentPoint::ColorAttachment0, s2::RenderCore::TextureFormat::RedGreenBlueAlpha8 );
	_thumbnailTarget->createAttachment( s2::RenderCore::FrameBuffer::AttachmentPoint::DepthAttachment, s2::RenderCore::TextureFormat::Depth24 );
	_thumbnailTarget->resize( 128, 128 );


	_trackball.setCenter( Math::ivec2( width() / 2, height() / 2 ) );
	_trackballLight.setCenter( Math::ivec2( width() / 2, height() / 2 ) );
}

// ------------------------------------------------------------------------------------------------
void MainWindow::renderThumbnailIfNeeded()
{
	if( !_thumbnailNeedsUpdate || !_thumbnailTarget )
		return;

	_thumbnailNeedsUpdate = false;

	// If nothing selected clear thumbnail (simple clear)
	if( !_hasSelection )
	{
		_renderer->begin( 
			{
				.renderPasses = { _renderPasses["forward"] },
				.renderTarget = _thumbnailTarget.get(),
				.cameraViewMatrix = _camera.worldToCameraMatrix(),
				.cameraProjectionMatrix = _camera.projectionMatrix() 
			} );
		{
			_renderer->submit( { .clearColor = s2::Color{ 0.1f, 0.1f, 0.1f, 1.0f } } );
		}
		_renderer->execute();
		return;
	}

	// find mesh handle for selected object
	auto it = _pickableToHandle.find( _selectedObjectID );
	if( it == _pickableToHandle.end() )
		return;

	auto meshHandle = it->second;

	// We need the original MeshData to compute bounding box.
	auto mdIt = _meshDataCache.find( meshHandle );
	if( mdIt == _meshDataCache.end() )
	{
		// fallback: render using main camera if we don't have mesh data
		_renderer->begin(
			{
				.renderPasses = { _renderPasses["forward"] },
				.renderTarget = _thumbnailTarget.get(),
				.cameraViewMatrix = _camera.worldToCameraMatrix(),
				.cameraProjectionMatrix = _camera.projectionMatrix()
			} );
		{
			_renderer->submit( { .clearColor = s2::Color{ 0.1f, 0.1f, 0.1f, 1.0f } } );

			_renderer->submit( {
				.renderMode  = s2::Renderer::RenderMode::Triangles,
				.material    = _materialPBRInstance,
				.mesh        = meshHandle,
				.pickableID  = 0,
				.modelMatrix = Math::scale( Math::dvec3( 1.0 ) )
			} );
		}
		_renderer->execute();
		return;
	}

	const auto& meshData = mdIt->second;
	if( meshData.vertices.empty() )
		return;

	// Compute axis-aligned bounding box in mesh local space
	Math::dvec3 bbMin = meshData.vertices[0];
	Math::dvec3 bbMax = meshData.vertices[0];

	for( const auto& v : meshData.vertices )
	{
		bbMin.x = std::min( bbMin.x, v.x );
		bbMin.y = std::min( bbMin.y, v.y );
		bbMin.z = std::min( bbMin.z, v.z );

		bbMax.x = std::max( bbMax.x, v.x );
		bbMax.y = std::max( bbMax.y, v.y );
		bbMax.z = std::max( bbMax.z, v.z );
	}

	Math::dvec3 center = ( bbMin + bbMax ) * 0.5;
	double radius = 0.0;
	for( const auto& v : meshData.vertices )
	{
		radius = std::max( radius, Math::length( v - center ) );
	}

	// Choose a view direction: use main camera direction so thumbnail orientation feels consistent
	Math::dvec3 mainCamDir = Math::normalize( _camera.position() - _camera.target() );
	if( Math::length( mainCamDir ) < 1e-6 )
		mainCamDir = Math::dvec3{ 0.0, 0.0, 1.0 };

	// Field of view: use same vertical fov as main (we set 45deg when resizing). Use 45 deg if unknown.
	const double fovYdeg = 45.0;
	const double fovY = fovYdeg * ( 3.14159265358979323846 / 180.0 );

	// compute distance such that the bounding sphere fits into camera frustum (with margin)
	const double margin = 1.15; // slight margin
	double distance = ( radius * margin ) / std::sin( fovY * 0.5 );
	if( distance <= 0.0 ) distance = radius * 2.0 + 0.1;

	// Position the thumbnail camera along mainCamDir at computed distance from center
	Math::dvec3 eye = center + mainCamDir * distance;
	Math::dvec3 up = _camera.up(); // keep same up vector

	// Construct a temporary camera for thumbnail
	s2::Scene::Camera thumbCam;
	thumbCam.set( eye, center, up );
	thumbCam.setProjectionTransform( Math::ProjectionTransform::createPerspective( 1.0, fovYdeg, std::max( 0.01, distance - radius*2.0 ), distance + radius*2.0 ) );
	thumbCam.setViewport( Math::irect( 0, 0, static_cast<int>( _thumbnailTarget->width() ), static_cast<int>( _thumbnailTarget->height() ) ) );

	// Render into thumbnail target with the thumb camera
	_renderer->begin(
		{
			.renderPasses = { _renderPasses["forward"] },
			.renderTarget = _thumbnailTarget.get(),
			.cameraViewMatrix = thumbCam.worldToCameraMatrix(),
			.cameraProjectionMatrix = thumbCam.projectionMatrix() 
		} );
	{
		_renderer->submit( { .clearColor = s2::Color{ 0.1f, 0.1f, 0.1f, 1.0f } } );

		// Render the mesh centered at its local vertex center (mesh vertices are already in world positions when factories generated them).
		// If your meshes are in local space you may need to transform them; here factories use world-space centers so we render with identity transform.
		_renderer->submit( {
			.renderMode  = s2::Renderer::RenderMode::Triangles,
			.material    = _materialPBRInstance,
			.mesh        = meshHandle,
			.pickableID  = 0,
			.modelMatrix = Math::translate( Math::dvec3( 0.0, 0.0, 0.0 ) ) // identity model; meshes already positioned by factory
		} );
	}
	_renderer->execute();
}

// ------------------------------------------------------------------------------------------------
void MainWindow::onShutdownEvent()
{
	// free render passes
	_renderPasses.clear();

	_renderer.reset();
	_picker.reset();
	_thumbnailTarget.reset();
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

#include "imgui_internal.h"

// ------------------------------------------------------------------------------------------------
void MainWindow::drawImGui()
{
	ImGui::SetCurrentContext( static_cast<ImGuiContext*> ( _ui->uiData( "ImGuiContext" ) ) );

	//ImGui::GetCurrentContext()->DebugLogFlags |= ImGuiDebugLogFlags_EventIO;

	ImGui::ShowDebugLogWindow();

	ImGui::SetNextWindowPos( ImVec2( 10, 10 ), ImGuiCond_Once );
	ImGui::SetNextWindowSize( ImVec2( 360, 0 ), ImGuiCond_Once );

	if( ImGui::Begin( "PBR Material" ) )
	{
		auto app = static_cast<MyApplication*>( s2::Application::instance() );

		// --- Material section ---
		if( ImGui::CollapsingHeader( "Material", ImGuiTreeNodeFlags_DefaultOpen ) )
		{
			ImGui::ColorEdit3( "Albedo", _uiAlbedo );
			ImGui::SliderFloat( "Metallic", &_uiMetallic, 0.0f, 1.0f );
			ImGui::SliderFloat( "Roughness", &_uiRoughness, 0.0f, 1.0f );
			ImGui::SliderFloat( "AO", &_uiAO, 0.0f, 1.0f );

			ImGui::Separator();
			ImGui::Checkbox( "Albedo Map", &_uiUseAlbedoMap );
			ImGui::Checkbox( "Normal Map", &_uiUseNormalMap );
			ImGui::Checkbox( "Metallic Map", &_uiUseMetallicMap );
			ImGui::Checkbox( "Roughness Map", &_uiUseRoughnessMap );
			ImGui::Checkbox( "AO Map", &_uiUseAOMap );
		}

		// --- Light section ---
		if( ImGui::CollapsingHeader( "Light", ImGuiTreeNodeFlags_DefaultOpen ) )
		{
			ImGui::DragFloat3( "Position", _uiLightPosition, 0.1f );
			ImGui::ColorEdit3( "Color", _uiLightColor );
			ImGui::SliderFloat( "Intensity", &_uiLightIntensity, 0.0f, 500.0f );
		}

		// --- Scene section ---
		if( ImGui::CollapsingHeader( "Scene" ) )
		{
			auto scale = static_cast<float>( app->scaleFactor );
			if( ImGui::SliderFloat( "Scale", &scale, 0.1f, 10.0f ) )
				app->scaleFactor = static_cast<double>( scale );
		}

		// --- Selection section (shows when an object is picked) ---
		if( ImGui::CollapsingHeader( "Selection", ImGuiTreeNodeFlags_DefaultOpen ) )
		{
			if( _hasSelection )
			{
				ImGui::Text( "Object ID: %u", _selectedObjectID );
				ImGui::Text( "Primitive ID: %u", _selectedPrimitiveID );
				ImGui::Text( "Screen Pos: (%d, %d)", _selectedScreenPos.x, _selectedScreenPos.y );
				ImGui::Separator();
				ImGui::Text( "Mesh: %s", _selectedMeshName.c_str() );
				ImGui::Text( "Vertex Count: %zu", _selectedVertexCount );

				// Material quick info for selected object (reads from the PBR material)
				ImGui::Separator();
				ImGui::TextColored( ImVec4( 0.8f, 0.8f, 0.2f, 1.0f ), "Material (preview)" );
				// show albedo / scalars
				ImGui::Text( "Albedo: %.2f, %.2f, %.2f", _uiAlbedo[0], _uiAlbedo[1], _uiAlbedo[2] );
				ImGui::Text( "Metallic: %.2f  Roughness: %.2f  AO: %.2f", _uiMetallic, _uiRoughness, _uiAO );
				ImGui::Text( "Use maps: A(%d) N(%d) M(%d) R(%d) AO(%d)",
							 _uiUseAlbedoMap ? 1 : 0, _uiUseNormalMap ? 1 : 0, _uiUseMetallicMap ? 1 : 0, _uiUseRoughnessMap ? 1 : 0, _uiUseAOMap ? 1 : 0 );

				// Thumbnail block
				ImGui::Separator();
				ImGui::Text( "Thumbnail" );
				if( ImGui::Button( "Render Thumbnail" ) )
					_thumbnailNeedsUpdate = true;

				if( _thumbnailTarget )
				{
					auto colorTex = _thumbnailTarget->attachment( s2::RenderCore::FrameBuffer::AttachmentPoint::ColorAttachment0 );
					if( colorTex && colorTex->isValid() )
					{
						colorTex->makeResident(); // ensure texture is resident before using bindless handle. no cost if already resident.
						ImGui::Image( colorTex->bindlessHandle(), ImVec2( 128, 128 ) );
					}
					else
					{
						ImGui::TextDisabled( "No thumbnail available" );
					}
				}

				// quick extras easy to implement
				if( ImGui::Button( "Clear Selection" ) )
				{
					_hasSelection = false;
					_selectedObjectID = 0;
					_selectedPrimitiveID = 0;
					_selectedMeshName.clear();
					_selectedVertexCount = 0;
					_thumbnailNeedsUpdate = true;
				}
			}
			else
			{
				ImGui::TextDisabled( "No object selected" );
			}
		}

		ImGui::Separator();
		ImGui::Text( "%.1f FPS", ImGui::GetIO().Framerate );
	}
	ImGui::End();

}

// ------------------------------------------------------------------------------------------------
void MainWindow::onDraw()
{
	// Scene.draw
	auto app = static_cast<MyApplication*>( s2::Application::instance() );

	const auto scale = app->scaleFactor;

	auto& resources = _renderer->resources();

	using namespace s2::Renderer;

	// Apply UI state to PBR material
	_materialPBRInstance.set( "u_Albedo",    Math::vec3( _uiAlbedo[0], _uiAlbedo[1], _uiAlbedo[2] ) );
	_materialPBRInstance.set( "u_Metallic",  _uiMetallic );
	_materialPBRInstance.set( "u_Roughness", _uiRoughness );
	_materialPBRInstance.set( "u_AO",        _uiAO );

	_materialPBRInstance.set( "u_UseAlbedoMap",    _uiUseAlbedoMap );
	_materialPBRInstance.set( "u_UseNormalMap",    _uiUseNormalMap );
	_materialPBRInstance.set( "u_UseMetallicMap",  _uiUseMetallicMap );
	_materialPBRInstance.set( "u_UseRoughnessMap", _uiUseRoughnessMap );
	_materialPBRInstance.set( "u_UseAOMap",        _uiUseAOMap );

	// Setup PBR material lighting from UI
	const Math::vec4 lightPos( _uiLightPosition[0], _uiLightPosition[1], _uiLightPosition[2], 1.0f );
	_materialPBRInstance.set( "u_LightPosition",  Math::vec3( _trackballLight.matrix() * lightPos ) );
	_materialPBRInstance.set( "u_LightColor",     Math::vec3( _uiLightColor[0], _uiLightColor[1], _uiLightColor[2] ) );
	_materialPBRInstance.set( "u_LightIntensity", _uiLightIntensity );
	_materialPBRInstance.set( "u_CamPos",         Math::vec3( _camera.position() ) );


	// setup material properties and shader
	// note: no need to do this every frame if the material properties are static.
	// we can create a material instance once and reuse it for multiple draw calls and update it only when properties change.
	_materialInstance.set("u_LightPosition" , Math::vec4(_trackballLight.matrix() * lightPos) );
	_materialInstance.set("u_LightAmbient"  , Math::vec4{ .01f,.01f,.01f,1.f });
	_materialInstance.set("u_LightDiffuse"  , Math::vec4{ 1.f,1.f,1.f,1.f });
	_materialInstance.set("u_LightSpecular" , Math::vec4{ 1.f,1.f,1.f,1.f });
	_materialInstance.set("u_LightShininess", 60.f);

	auto modelMatrix = Math::scale( Math::dvec3( scale ) ) * _trackball.matrix();

	_renderer->begin(
		{
			.renderPasses           = { _renderPasses["forward"], _renderPasses["pick"] },
			.renderTarget           = _mainRenderTarget.get(),
			.cameraViewMatrix       = _camera.worldToCameraMatrix(),
			.cameraProjectionMatrix = _camera.projectionMatrix(),
		} );
	{
		_renderer->submit( { .clearColor = s2::Color{ 0.3f, 0.4f, 0.5f, 1.0f } } );

		// For each object: if selected -> draw outline pass first, then regular pass.
		auto drawWithPossibleOutline = [&]( uint32_t pickableID, const s2::Renderer::Material& mat, s2::Renderer::MeshID meshHandle )
		{
			if( _hasSelection && pickableID == _selectedObjectID && _outlineMaterial.shader.isValid() )
			{
				// outline pass uses same mesh, outline material (we copy and set uniforms that may change)
				//auto outlineMat = _outlineMaterial; // copy to modify per-draw uniforms if needed
				// optionally change outline width depending on camera distance (not implemented here)
				_renderer->submit( {
					
					.renderMode  = s2::Renderer::RenderMode::Triangles,
					.material    = _outlineMaterialInstance,
					.mesh        = meshHandle,
					.pickableID  = 0,
					.modelMatrix = modelMatrix
				} );
			}

			// regular pass
			_renderer->submit( {
				.renderMode  = s2::Renderer::RenderMode::Triangles,
				.material    = mat,
				.mesh        = meshHandle,
				.pickableID  = pickableID,
				.modelMatrix = modelMatrix
			} );
		};

		// cube
		drawWithPossibleOutline( 1, _materialPBRInstance, resources.mesh( "cube" ) );
		// sphere
		drawWithPossibleOutline( 2, _materialPBRInstance, resources.mesh( "sphere" ) );
		// torus
		drawWithPossibleOutline( 3, _materialPBRInstance, resources.mesh( "torus" ) );
		// cone
		drawWithPossibleOutline( 4, _materialPBRInstance, resources.mesh( "cone" ) );
		// cylinder
		drawWithPossibleOutline( 5, _materialPBRInstance, resources.mesh( "cylinder" ) );
		// capsule
		drawWithPossibleOutline( 6, _materialPBRInstance, resources.mesh( "capsule" ) );

	}
	_renderer->execute();

	// todo: add logic to enable/disable UI rendering:
	// make used textures resident when enabled, make them non-resident when disabled to save GPU memory if needed.
	
	// if ui layer is enabled, draw it in a separate pass on top of the scene
	//if( _ui->isEnabled() )
	if( false )
	{
		renderThumbnailIfNeeded();
		
		_ui->draw( [&] () { drawImGui(); } );

		_renderer->begin(
			{
				.renderPasses = { _renderPasses["ui"] },
				.renderTarget = _mainRenderTarget.get(),
			} );

		_renderer->execute(); 
	}
}

// ------------------------------------------------------------------------------------------------
void MainWindow::onMouseMoveEvent( const s2::Input::MouseState& ms )
{
	if( uiWantCaptureMouse() )
		return;

	// handle dragging of light trackball
	//if( ms.isDragging() && ms.isButtonDown( s2::Input::MouseState::ButtonRight ) )
	//	_trackballLight.update( Scene::TrackBall::DragEvent::Update, ms.position() );


	// handle dragging of object trackball
	if( ms.isDragging() && ms.isButtonDown( s2::Input::MouseState::ButtonRight ) )
		_trackball.update( s2::Scene::TrackBall::DragEvent::Update, ms.position() );

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
	if( uiWantCaptureMouse() )
		return;

	//	ms.dumpStatus( "onMouseButtonEvent" );
	//if( ms.isButtonDown( s2::Input::MouseState::ButtonLeft) )		_trackball.update( Scene::TrackBall::DragEvent::Begin, ms.position() );
	//else if( ms.isButtonUp( s2::Input::MouseState::ButtonLeft ) )   _trackball.update( Scene::TrackBall::DragEvent::End, ms.position() );
	
	if( ms.isButtonDown( s2::Input::MouseState::ButtonLeft ) )
		_picker->pickObjectAt( ms.position() );
		//_ui->setEnabled( !_ui->isEnabled() ); // toggle UI on left click for testing

	if( ms.isButtonDown( s2::Input::MouseState::ButtonRight ) )    _trackball.update( s2::Scene::TrackBall::DragEvent::Begin, ms.position() );
	else if( ms.isButtonUp( s2::Input::MouseState::ButtonRight ) ) _trackball.update( s2::Scene::TrackBall::DragEvent::End, ms.position() );
}

// ------------------------------------------------------------------------------------------------
void MainWindow::onMouseScrollEvent( const s2::Input::MouseState& ms )
{
	if( uiWantCaptureMouse() )
		return;

	//ms.dumpStatus( "onMouseScrollEvent" );
	auto app = static_cast<MyApplication*>( s2::Application::instance() );
	app->scaleFactor *= std::pow( 1.2, ms.wheel() );

	// Optionally add limits to prevent extreme scaling
	app->scaleFactor = std::max( 0.1, std::min( 10.0, app->scaleFactor ) );
}