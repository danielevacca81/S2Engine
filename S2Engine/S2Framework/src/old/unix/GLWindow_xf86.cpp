// GLWindow_xf86.cpp
//
#include "GLWindow_xf86.h"

#include<X11/X.h>
#include<X11/Xlib.h>
#include<GL/gl.h>
#include<GL/glx.h>

using namespace s2Core;

// ------------------------------------------------------------------------------------------------
GLWindow::GLWindow( const std::string &title )
: win(0),
  windowTitle(title)
{
    destroy();
    create();
}

// ------------------------------------------------------------------------------------------------
GLWindow::~GLWindow()
{
    destroy();
}

// ------------------------------------------------------------------------------------------------
void  GLWindow::setSize(unsigned int w, unsigned int h)
{
	windowWidth  = w;
	windowHeight = h;
}

// ------------------------------------------------------------------------------------------------
void GLWindow::show( bool visible )
{

}

// ------------------------------------------------------------------------------------------------
void GLWindow::hide()
{

}

// ------------------------------------------------------------------------------------------------
void GLWindow::create()
{
	// Create the GL rendering context
	/*if(!gl.create())
		return false;

	Display *dpy = gl.getDisplay();
	const XVisualInfo *vi = gl.getXVisualInfo();
	int screen = gl.getScreen();

	int modeNum;
	XF86VidModeModeInfo **modes;
	XF86VidModeGetAllModeLines(dpy, screen, &modeNum, &modes);

	deskMode = *modes[0]; // save current Desktop settings

	// Select the best available mode
	int bestMode = 0;
	for (int i = 0; i < modeNum; i++)
	{
		if ((modes[i]->hdisplay == windowWidth) && (modes[i]->vdisplay == windowHeight))
		{
			bestMode = i;
		}
	}*/

    Display     *dpy        = XOpenDisplay(NULL);
    Window       rootWindow = DefaultRootWindow(dpy);

    //int attributes[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    //XVisualInfo *vi  = glXChooseVisual(dpy, 0, attributes);
    //
    //
	//Colormap cmap = XCreateColormap(dpy, rootWindow, vi->visual, AllocNone);
    //
    //XSetWindowAttributes swa;
    //swa.colormap     = cmap;
	//swa.event_mask   = ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask;
    //swa.border_pixel = 0;

    win = XCreateWindow(dpy,                                        // X display
                        rootWindow,                                 // parentWindow (desktop)
                        0, 0,                                       // position
                        300, 300,                                   // sizes
                        0,                                          // borders
                        CopyFromParent,                             // bitdepth
                        InputOutput,                                // window class
                        CopyFromParent,                             // visual
                        0,                                          // attribute mask
                        0);                                          // attributes
                        //CWBorderPixel | CWColormap | CWEventMask,   // attribute mask
                        //&swa);                                      // attributes

    XMapWindow(dpy, win);
    XStoreName(dpy, win, windowTitle.c_str());
	/*if(bFullscreen)
	{
		XF86VidModeSwitchToMode(dpy, screen, modes[bestMode]);
		XF86VidModeSetViewPort(dpy, screen, 0, 0);

		int dpyWidth = modes[bestMode]->hdisplay;
		int dpyHeight = modes[bestMode]->vdisplay;

		XFree(modes);

		attr.override_redirect = True;
		attr.event_mask = ExposureMask | KeyPressMask | ButtonPressMask |
								StructureNotifyMask;

		win = XCreateWindow(dpy, RootWindow(dpy, vi->screen),
								0, 0, dpyWidth, dpyHeight, 0, vi->depth, InputOutput, vi->visual,
								CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect,
								&attr);

		XWarpPointer(dpy, None, win, 0, 0, 0, 0, 0, 0);
		XMapRaised(dpy, win);
		XGrabKeyboard(dpy, win, True, GrabModeAsync,GrabModeAsync, CurrentTime);
		XGrabPointer(dpy, win, True, ButtonPressMask,
		GrabModeAsync, GrabModeAsync, win, None, CurrentTime);
	}
	else
	{

		attr.event_mask = ExposureMask | KeyPressMask | ButtonPressMask |
								StructureNotifyMask;

		win = XCreateWindow(dpy, RootWindow(dpy, vi->screen),
								  0, 0, windowWidth, windowHeight, 0, vi->depth, InputOutput, vi->visual,
								  CWBorderPixel | CWColormap | CWEventMask, &attr);

		Atom wmDelete = XInternAtom(dpy, "WM_DELETE_WINDOW", True);
		XSetWMProtocols(dpy, win, &wmDelete, 1);

		XSetStandardProperties(dpy, win, title,title, None, NULL, 0, NULL);
		XMapRaised(dpy, win);
	}

	if(!win)
	{
		destroy();
		return false;
	}

	gl.makeCurrent(win);

	Window winDummy;
	unsigned int borderDummy;
	int xPos,yPos;
	XGetGeometry(dpy, win, &winDummy, &xPos, &yPos,
	             &windowWidth, &windowHeight, &borderDummy, &windowBpp);



	bActive	= true;
	bMinimized = false;
	bValid = true;

	return true;*/
}

// ------------------------------------------------------------------------------------------------
void GLWindow::destroy()
{
	/*if(!bValid)
		return;

	if(gl.isValid())
	{
		if(bFullscreen)
		{
			Display *dpy = gl.getDisplay();
			int screen = gl.getScreen();

			XF86VidModeSwitchToMode(dpy, screen, &deskMode);
			XF86VidModeSetViewPort(dpy, screen, 0, 0);
		}

		gl.destroy();
	}

	bActive = false;
	bValid = false;*/
}

// ------------------------------------------------------------------------------------------------
void GLWindow::setWindowed( bool on )
{
	windowed = on;
}

// ------------------------------------------------------------------------------------------------
unsigned int GLWindow::getWidth()     {return windowWidth;}
unsigned int GLWindow::getHeight()    {return windowHeight;}

//Window GLWindow::getHandler()   {return win;}
unsigned int GLWindow::getHWND()   {return 0;}

// Aggiorna lo stato del processo
// ------------------------------------------------------------------------------------------------
/*bool GLWindow::update()
{
	Display *dpy = gl.getDisplay();

	if(XPending(dpy) > 0)
	{
		XEvent event;
		XNextEvent(dpy, &event);

		switch(event.type)
		{
			case Expose:
				if (event.xexpose.count != 0)	break;
					break;

			case ConfigureNotify:
				if ((event.xconfigure.width != windowWidth) ||
					(event.xconfigure.height != windowHeight))
				{
					windowWidth = event.xconfigure.width;
					windowHeight = event.xconfigure.height;
					printf("Resize Event\n");

					resizeGL();
				}
				break;

			case KeyPress:
//				switch(XLookupKeysym(&event.xkey,0))
//				{
//					case XK_Escape:									// Quit application
//						running = 0;
//						break;
//					case XK_F1:
//						// Switch between fullscreen and windowed mode
//						killGLWindow();
//						GLWin.fs = !GLWin.fs;
//						createGLWindow("Nehe's GLX Base Code", 640, 480, 24, GLWin.fs);
//						break;
//				}
				break;

			case KeyRelease:
				break;

			case ClientMessage:
				if (*XGetAtomName(dpy, event.xclient.message_type) == *"WM_PROTOCOLS")
				{
					bActive = false;
				}
				break;

			default:
				break;
		}
	}

	paintGL();
	updateGL();

	return bActive;
}*/

//void GLWindow::paintGL(){}
//void GLWindow::resizeGL(){}

//void GLWindow::updateGL()
//{
//	glXSwapBuffers(gl.getDisplay(), win);
//}
