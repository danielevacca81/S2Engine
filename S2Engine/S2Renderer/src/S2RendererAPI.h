// S2RendererAPI.h
//
#ifndef S2RENDERERAPI_ONCE
#define S2RENDERERAPI_ONCE

#ifdef _WIN32
	#define S2RENDERER_EXPORT __declspec(dllexport)
	#define S2RENDERER_IMPORT __declspec(dllimport)

	#ifdef S2RENDERER_EXPORTS
		#define S2RENDERER_API S2RENDERER_EXPORT
	#else
		#define S2RENDERER_API S2RENDERER_IMPORT
	#endif

#else
	#define S2RENDERER_API
	#define S2RENDERER_EXPORT
	#define S2RENDERER_IMPORT
#endif

#endif

#pragma warning (disable: 4251)
#pragma warning (disable: 4275)
