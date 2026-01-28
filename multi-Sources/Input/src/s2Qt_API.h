// s2Qt.h
//
#ifndef S2QT_API_ONCE
#define S2QT_API_ONCE

#ifdef _WIN32
#define S2QT_EXPORT __declspec(dllexport)
#define S2QT_IMPORT __declspec(dllimport)

#ifdef S2QT_EXPORTS
#define S2QT_API S2QT_EXPORT
#else
#define S2QT_API S2QT_IMPORT
#endif

#else
#define S2QT_API
#define S2QT_EXPORT
#define S2QT_IMPORT
#endif

#endif

#pragma warning (disable: 4251)
#pragma warning (disable: 4275)