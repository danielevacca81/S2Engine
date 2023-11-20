// s2SceneGraph_API.h
//
#ifndef S2SCENEGRAPH_API_ONCE
#define S2SCENEGRAPH_API_ONCE

#ifdef _WIN32
#define S2SCENEGRAPH_EXPORT __declspec(dllexport)
#define S2SCENEGRAPH_IMPORT __declspec(dllimport)

#ifdef S2SCENEGRAPH_EXPORTS
#define S2SCENEGRAPH_API S2SCENEGRAPH_EXPORT
#else
#define S2SCENEGRAPH_API S2SCENEGRAPH_IMPORT
#endif

#else
#define S2SCENEGRAPH_API
#define S2SCENEGRAPH_EXPORT
#define S2SCENEGRAPH_IMPORT
#endif

#endif

#pragma warning (disable: 4251)
#pragma warning (disable: 4275)