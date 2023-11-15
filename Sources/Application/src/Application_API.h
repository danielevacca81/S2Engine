// Application_API.h
//
#ifndef APPLICATION_API_H
#define APPLICATION_API_H

#ifdef _WIN32
#define APPLICATION_EXPORT __declspec(dllexport)
#define APPLICATION_IMPORT __declspec(dllimport)

#ifdef APPLICATION_EXPORTS
#define APPLICATION_API APPLICATION_EXPORT
#else
#define APPLICATION_API APPLICATION_IMPORT
#endif

#else
#define APPLICATION_API
#define APPLICATION_EXPORT
#define APPLICATION_IMPORT
#endif

#endif

#pragma warning (disable: 4251)
#pragma warning (disable: 4275)
