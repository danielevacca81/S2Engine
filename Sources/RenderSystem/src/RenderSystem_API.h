// RenderSystem_API.h
//
#ifndef RENDERSYSTEM_API_H
#define RENDERSYSTEM_API_H

#ifdef _WIN32
	#define RENDERSYSTEM_EXPORT __declspec(dllexport)
	#define RENDERSYSTEM_IMPORT __declspec(dllimport)

	#ifdef RENDERSYSTEM_EXPORTS
		#define RENDERSYSTEM_API RENDERSYSTEM_EXPORT
	#else
		#define RENDERSYSTEM_API RENDERSYSTEM_IMPORT
	#endif

#else
	#define RENDERSYSTEM_API
	#define RENDERSYSTEM_EXPORT
	#define RENDERSYSTEM_IMPORT
#endif

#endif

#pragma warning (disable: 4251)
#pragma warning (disable: 4275)
