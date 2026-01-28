// RenderCore_API.h
//
#ifndef RENDERCORE_API_H
#define RENDERCORE_API_H

#ifdef _WIN32
	#define RENDERCORE_EXPORT __declspec(dllexport)
	#define RENDERCORE_IMPORT __declspec(dllimport)

	#ifdef RENDERCORE_EXPORTS
		#define RENDERCORE_API RENDERCORE_EXPORT
	#else
		#define RENDERCORE_API RENDERCORE_IMPORT
	#endif

#else
	#define RENDERCORE_API
	#define RENDERCORE_EXPORT
	#define RENDERCORE_IMPORT
#endif

#endif

#pragma warning (disable: 4251)
#pragma warning (disable: 4275)
