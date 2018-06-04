// Renderer_API.h
//
#ifndef RENDERER_API_ONCE
#define RENDERER_API_ONCE

#ifdef _WIN32
	#define RENDERER_EXPORT __declspec(dllexport)
	#define RENDERER_IMPORT __declspec(dllimport)

	#ifdef RENDERER_EXPORTS
		#define RENDERER_API RENDERER_EXPORT
	#else
		#define RENDERER_API RENDERER_IMPORT
	#endif

#else
	#define RENDERER_API
	#define RENDERER_EXPORT
	#define RENDERER_IMPORT
#endif

#endif

#pragma warning (disable: 4251)
#pragma warning (disable: 4275)
