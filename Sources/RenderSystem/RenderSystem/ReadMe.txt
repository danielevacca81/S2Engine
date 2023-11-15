//////////////////////////////////////////////////////////////////
// An API independant render system
//
// Author  : Vincent Prat
// Released: 21-03-2003
/////////////////////


-- About RenderSystem --

  This material is intented to provide a sample framework for 
  beginners that would start to develop a render system capable
  of supporting various 3DApi (Direct X, OpenGL are included).
  This sample render system should give a starting point to those
  who are still wondering how to design this, and also provide 
  the base to add other features that are not yet implemented 
  here (as you can see, the goal was not to provide a complete 
  render system but to present how it should be layed out 
  according to me).

  I would be glad to ear from experienced people about this 
  approach as this is the first time I give a try to designing such 
  a renderer. Of course, as "educational" material, I am sure that
  a lot of improvements/optimisations can be implemented, starting
  with adding decent math library, using vertex array range in the
  OpenGL implementation of static vertex buffers, ... Feel free to
  suggest me other ideas, they will be appreciated and studied.
  
  I would also be happy to get your ideas and comments. See contact 
  info at the end of this file. Thanks.


-- Overview of the structure --

  Basic idea was to use the bridge design pattern in coordination
  with the factory design pattern in order to provide maximum
  flexibility when adding new implementations (with a small 
  performance hit however I think).

  The factory is used to provide an abstract interface that specifies
  what an implementation should be able to created. The objects that
  we want to create with this factory are all the objects that have
  an Api dependant implementation (textures, vertex buffers, etc.).

  On the side of the factory, we also have an interface used to specify
  what an implementation of a renderer should be able to do (set render 
  states, set transformation matrices, draw primitives, ...).

  Finally, we also added some interfaces describing the responsibilities
  of objects to be created by the factory (VB, textures, ...).

  The bridge pattern is then used to provide the user a global interface
  to our render system, allowing it to create objects (VB, textures, 
  etc.) and to issue rendering commands without worrying of the Api
  the render system is using (GL or D3D for now).


-- Additional notes on the sample app --

  You might want to define or undefine COMPILE_OPENGL_IMPLEMENTATION
  and/or COMPILE_DIRECT3D_IMPLEMENTATION to enable/disable the compilation 
  of one or the other of the implementations. For this, go to:
  Project/Settings/C++ -> Preprocessor definitions.

  I know the UI of the sample program is a piece of s**t. Keys to change
  render states are: 
    - 1  : use Direct3D
	- 2  : use OpenGL
    - w/W: wireframe
    - l/L: lighting
	- c/C: backface culling
	- esc: guess this one!


-- Content of the zip file --

  * VC 6 project file
  * Sources of the render system (.\sources\RenderSystem\)
  * Sources of the sample MFC program (.\sources\App\)
  * Documentation of the render system in .chm format
  * This dumb readme.txt file


-- Credits --

  * Doxygen for the documentation (great tool) 
    http://www.doxygen.org

  * Tobin Schwaiger and his code of the day for a vertex buffer
    interface. Look for "Vertex Buffer Interface" in the COTD 
	gallery of http://www.flipcode.com

  * People in gdalgorithm and DirectXDev lists for help about how
    to get DirectX without linking to d3d static libs.
  
  * Many thanks to all the people that release source code and/or
    tutorials on the web.


-- Contact info --

  Vincent PRAT
  vprat@ifrance.com
  http://vprat.alrj.org


-- Disclaimer, usual legal blabla --

  As usual, use this at your own risks. You may use this code for
  your projects as far as you give credits, be fair. You may also
  redistribute this zip file as far as you do not alter its 
  content (i.e. add/remove/modify files). etc. etc. etc. you get 
  the idea, use it, give credits, do not bother me with anything
  else than questions about it.

  All the files are (c) Vincent Prat 2003


