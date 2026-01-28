// SceneManager_API.h
//
#ifndef SCENEMANAGER_API_H
#define SCENEMANAGER_API_H

#ifdef _WIN32
	#define SCENEMANAGER_EXPORT __declspec(dllexport)
	#define SCENEMANAGER_IMPORT __declspec(dllimport)

	#ifdef SCENEMANAGER_EXPORTS
		#define SCENEMANAGER_API SCENEMANAGER_EXPORT
	#else
		#define SCENEMANAGER_API SCENEMANAGER_IMPORT
	#endif

#else
	#define SCENEMANAGER_API
	#define SCENEMANAGER_EXPORT
	#define SCENEMANAGER_IMPORT
#endif

#endif

#pragma warning (disable: 4251)
#pragma warning (disable: 4275)
