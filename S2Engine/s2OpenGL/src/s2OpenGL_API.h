// s2OpenGL_API.h
//
#ifndef S2OPENGL_API_ONCE
#define S2OPENGL_API_ONCE

#ifdef _WIN32
	#define S2OPENGL_EXPORT __declspec(dllexport)
	#define S2OPENGL_IMPORT __declspec(dllimport)

	#ifdef S2OPENGL_EXPORTS
		#define S2OPENGL_API S2OPENGL_EXPORT
	#else
		#define S2OPENGL_API S2OPENGL_IMPORT
	#endif

#else
	#define S2OPENGL_API
	#define S2OPENGL_EXPORT
	#define S2OPENGL_IMPORT
#endif

#endif

#pragma warning (disable: 4251)
#pragma warning (disable: 4275)

#define OBJECT_DISABLE_COPY(Class) \
    Class(const Class &) = delete;\
    Class &operator=(const Class &) = delete;

#define OBJECT_DECLARE_MOVEABLE(Class) \
    Class(Class &&);\
    Class &operator=(Class &&);

