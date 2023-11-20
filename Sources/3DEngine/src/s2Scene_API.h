// s2Scene_API.h
//
#ifndef S2SCENE_API_ONCE
#define S2SCENE_API_ONCE

#ifdef _WIN32
#define S2SCENE_EXPORT __declspec(dllexport)
#define S2SCENE_IMPORT __declspec(dllimport)

#ifdef S2SCENE_EXPORTS
#define S2SCENE_API S2SCENE_EXPORT
#else
#define S2SCENE_API S2SCENE_IMPORT
#endif

#else
#define S2SCENE_API
#define S2SCENE_EXPORT
#define S2SCENE_IMPORT
#endif

#endif

#pragma warning (disable: 4251)
#pragma warning (disable: 4275)