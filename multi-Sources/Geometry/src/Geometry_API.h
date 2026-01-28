// Geometry_API.h
//
#ifndef GEOMETRY_API_H
#define GEOMETRY_API_H

#ifdef _WIN32
	#define GEOMETRY_EXPORT __declspec(dllexport)
	#define GEOMETRY_IMPORT __declspec(dllimport)

	#ifdef GEOMETRY_EXPORTS
		#define GEOMETRY_API GEOMETRY_EXPORT
	#else
		#define GEOMETRY_API GEOMETRY_IMPORT
	#endif

#else
	#define GEOMETRY_API
	#define GEOMETRY_EXPORT
	#define GEOMETRY_IMPORT
#endif

#endif

#pragma warning (disable: 4251)
#pragma warning (disable: 4275)
