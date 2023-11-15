// Context.h
//
#ifndef CONTEXT_ONCE
#define CONTEXT_ONCE

#include "RenderSystem_API.h"

#include "ContextInfo.h"
#include "StateManager.h"
#include "Surface.h"

namespace RenderSystem {

class RENDERSYSTEM_API Context
{
public:
	static Context *current();
	//static Context *defaultContext();

public:
	Surface* createSurface( const std::initializer_list<Surface::AttachmentDescription> &init = 
							{ 
								{FrameBuffer::ColorAttachment0,TextureFormat::RedGreenBlueAlpha8},
								{FrameBuffer::DepthAttachment, TextureFormat::Depth24} 
							} );


	// OpenGLObjectFactory??
	
	const ContextInfo& info() const { return _info; }

	bool operator==( const Context& o ) const { return _nativeHandle == o._nativeHandle; }
	bool operator!=( const Context& o ) const { return _nativeHandle != o._nativeHandle; }

private:
	Context();
	~Context();

	void makeCurrent() {}
	void doneCurrent() {}

protected:
	//std::string  _name;
	uint64_t  _nativeHandle { 0 };
	int32_t   _defaultFBO { -1 };

	ContextInfo  _info;
	StateManager _stateManager;
	
	friend class RenderEngine;
};

#if 0

class Surface;
class ContextRegistry;

class Context;
typedef std::shared_ptr<Context> ContextPtr;
typedef std::weak_ptr<Context>   ContextWeakPtr;


#if 0
class RENDERSYSTEM_API Context
{
public:
	static ContextPtr Current();
	static ContextPtr New( int majorVersion = 3, int minorVersion = 3);

public:
	Context();
	~Context();

	//void addResource( const ResourcePtr &resource );

	void makeCurrent();
	void swapBuffers();
	void enableVSync( bool enable ); // @todo: set attributes
	void release();

	int64_t                  id()         const;
	std::vector<std::string> extensions() const;
	std::string              info()       const;


protected:
	bool initExtensions() const;
	//void releaseResources();

private:
	static std::map<void*, std::weak_ptr<Context> > _contextList;

private:
	void *_hDC;
	void *_hRC;
	bool _external; // handled by third party framework (i.e. Qt)
	//std::set<ResourcePtr> _resources;
};
#endif

/**
	\brief Wraps an OpenGL context adding OpenGL state shadowing

	This class its not intended to be used directly, it is intended to be a base class.
	Inherit from this class and override virtual methods (onMakeCurrent in particular)
*/
class RENDERSYSTEM_API Context : public std::enable_shared_from_this<Context>
{
public:
	class RENDERSYSTEM_API NativeHandle
	{
	public:
		static NativeHandle current();
		static NativeHandle null();


	#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
		void *_glRenderingContex = 0;
		void *_deviceContext     = 0; //only for debugging purpose

		bool operator==( const NativeHandle &o ) const { return _glRenderingContex == o._glRenderingContex; }
		bool operator!=( const NativeHandle &o ) const { return _glRenderingContex != o._glRenderingContex; }
	#else
		void *_glRenderingContex = 0;

		bool operator==( const NativeHandle &o ) const { return _glRenderingContex == o._glRenderingContex; }
		bool operator!=( const NativeHandle &o ) const { return _glRenderingContex != o._glRenderingContex; }
	#endif
	};

protected:
	/*struct RawGLObject
	{
		enum class Type { None };

		unsigned int objectID = 0;
		Type         type     = Type::None;

		~RawGLObject();
	};*/

public:
	Context( const Context & )             = delete;
	Context( Context && )                  = delete;
	Context &operator==( const Context & ) = delete;
	Context &operator==( Context && )      = delete;

	virtual ~Context();

	const NativeHandle &id()        const { return _nativeHandle; }
	const std::string  &name()      const { return _name; }
	bool                isCurrent() const;

	void                setName( const std::string &name ) { _name = name; }
	void                makeCurrent();
	void                disableShadowingOneShot() { _stateManager.disableShadowingOneShot(); }
	//virtual void      resetState();

				    
protected:
	Context( const NativeHandle &nativeHandle );  

	virtual void        onMakeCurrent ()       {} // do call low level make current for example wglMakeCurrent or QOpenGLContext::makeCurrent
	virtual void        onDoneCurrent ()       {}
	virtual void        checkInvariant() const { /*empty for now*/; } 
	friend ContextRegistry;

	void                clear( const ClearState &cs, const FrameBufferPtr &fb );
	void                draw ( const PrimitiveType &primitiveType, const VertexArrayPtr     &va, const DrawState &ds, const FrameBufferPtr &fb );
	void                draw ( const PrimitiveType &primitiveType, const PrimitiveBufferPtr &p,  const DrawState &ds, const FrameBufferPtr &fb );
	void                draw ( const PrimitiveType &primitiveType, const PrimitiveBufferPtr &p,  const DrawState &ds, const unsigned int targetFBO );
	void                draw ( const PrimitiveType &primitiveType, const VertexArrayPtr &va );

	void                draw ( const PrimitiveBatch &batch, const DrawState& ds, const FrameBufferPtr& fb );
	friend class Surface;

protected:
	//static std::vector< RawGLObject > _sharedGarbageCollector;   // todo: all context are sharing implicitly. single thread for now. add a mutex for concurrent access

	NativeHandle                   _nativeHandle;
	std::string                    _name;
	StateManager                   _stateManager;

	//std::vector< RawGLObject >   _garbageCollector;
};

#endif
}
#endif