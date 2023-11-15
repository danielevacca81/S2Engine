# Microsoft Developer Studio Project File - Name="RenderSystem" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=RenderSystem - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RenderSystem.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RenderSystem.mak" CFG="RenderSystem - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RenderSystem - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "RenderSystem - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RenderSystem - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "COMPILE_OPENGL_IMPLEMENTATION" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 OPENGL32.LIB Winmm.lib glu32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "RenderSystem - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /Gr /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "COMPILE_OPENGL_IMPLEMENTATION" /FD /I /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 OPENGL32.LIB Winmm.lib glu32.lib /nologo /subsystem:windows /profile /debug /machine:I386

!ENDIF 

# Begin Target

# Name "RenderSystem - Win32 Release"
# Name "RenderSystem - Win32 Debug"
# Begin Group "MFC"

# PROP Default_Filter ""
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Sources\App\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Sources\App\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Sources\App\RenderSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\Sources\App\RenderSystemDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\Sources\App\RenderSystemView.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Sources\App\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\Sources\App\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Sources\App\RenderSystem.h
# End Source File
# Begin Source File

SOURCE=.\Sources\App\RenderSystemDoc.h
# End Source File
# Begin Source File

SOURCE=.\Sources\App\RenderSystemView.h
# End Source File
# Begin Source File

SOURCE=.\Sources\App\Resource.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\RenderSystem.ico
# End Source File
# Begin Source File

SOURCE=.\res\RenderSystem.rc2
# End Source File
# Begin Source File

SOURCE=.\Sources\App\RenderSystemApp.rc
# End Source File
# Begin Source File

SOURCE=.\res\RenderSystemDoc.ico
# End Source File
# End Group
# End Group
# Begin Group "Render system"

# PROP Default_Filter ""
# Begin Group "User interfaces"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Sources\RenderSystem\RenderDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\Sources\RenderSystem\RenderDevice.h
# End Source File
# Begin Source File

SOURCE=.\Sources\RenderSystem\RenderTypeDefs.h
# End Source File
# Begin Source File

SOURCE=.\Sources\RenderSystem\Texture.h
# End Source File
# Begin Source File

SOURCE=.\Sources\RenderSystem\VertexBuffer.h
# End Source File
# End Group
# Begin Group "Others"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Sources\RenderSystem\Matrix4x4.cpp
# End Source File
# Begin Source File

SOURCE=.\Sources\RenderSystem\Matrix4x4.h
# End Source File
# Begin Source File

SOURCE=.\Sources\RenderSystem\MatrixStack.h
# End Source File
# Begin Source File

SOURCE=.\Sources\RenderSystem\Sphere.cpp
# End Source File
# Begin Source File

SOURCE=.\Sources\RenderSystem\Sphere.h
# End Source File
# End Group
# Begin Group "Implementors"

# PROP Default_Filter ""
# Begin Group "D3D implementation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Sources\RenderSystem\D3DRenderDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\Sources\RenderSystem\D3DRenderDevice.h
# End Source File
# Begin Source File

SOURCE=.\Sources\RenderSystem\D3DRenderObjectFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\Sources\RenderSystem\D3DRenderObjectFactory.h
# End Source File
# Begin Source File

SOURCE=.\Sources\RenderSystem\D3DTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\Sources\RenderSystem\D3DTexture.h
# End Source File
# Begin Source File

SOURCE=.\Sources\RenderSystem\D3DVertexBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\Sources\RenderSystem\D3DVertexBuffer.h
# End Source File
# End Group
# Begin Group "OpenGL implementation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Sources\RenderSystem\OpenGLRenderDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\Sources\RenderSystem\OpenGLRenderDevice.h
# End Source File
# Begin Source File

SOURCE=.\Sources\RenderSystem\OpenGLRenderObjectFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\Sources\RenderSystem\OpenGLRenderObjectFactory.h
# End Source File
# Begin Source File

SOURCE=.\Sources\RenderSystem\OpenGLTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\Sources\RenderSystem\OpenGLTexture.h
# End Source File
# Begin Source File

SOURCE=.\Sources\RenderSystem\OpenGLVertexBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\Sources\RenderSystem\OpenGLVertexBuffer.h
# End Source File
# End Group
# Begin Group "Interfaces"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Sources\RenderSystem\RenderDeviceImplementor.h
# End Source File
# Begin Source File

SOURCE=.\Sources\RenderSystem\RenderObjectFactory.h
# End Source File
# End Group
# End Group
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
