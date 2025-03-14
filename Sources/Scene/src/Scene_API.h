// RenderPipeline_API.h
//
#ifndef RENDERPIPELINE_API_H
#define RENDERPIPELINE_API_H

#ifdef _WIN32
	#define RENDERPIPELINE_EXPORT __declspec(dllexport)
	#define RENDERPIPELINE_IMPORT __declspec(dllimport)

	#ifdef RENDERPIPELINE_EXPORTS
		#define RENDERPIPELINE_API RENDERPIPELINE_EXPORT
	#else
		#define RENDERPIPELINE_API RENDERPIPELINE_IMPORT
	#endif

#else
	#define RENDERPIPELINE_API
	#define RENDERPIPELINE_EXPORT
	#define RENDERPIPELINE_IMPORT
#endif

#endif

#pragma warning (disable: 4251)
#pragma warning (disable: 4275)
