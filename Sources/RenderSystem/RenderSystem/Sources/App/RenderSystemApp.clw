; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CD3DRendererTestAppView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "rendersystemapp.h"
LastPage=0

ClassCount=6
Class1=CChildFrame
Class2=CMainFrame
Class3=CD3DRendererTestAppApp
Class4=CAboutDlg
Class5=CD3DRendererTestAppDoc
Class6=CD3DRendererTestAppView

ResourceCount=3
Resource1=IDR_MAINFRAME
Resource2=IDD_ABOUTBOX
Resource3=IDR_D3DRENTYPE

[CLS:CChildFrame]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[CLS:CD3DRendererTestAppApp]
Type=0
BaseClass=CWinApp
HeaderFile=RenderSystem.h
ImplementationFile=RenderSystem.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=RenderSystem.cpp
ImplementationFile=RenderSystem.cpp
LastObject=ID_TEXTURE_CHECKER

[CLS:CD3DRendererTestAppDoc]
Type=0
BaseClass=CDocument
HeaderFile=RenderSystemDoc.h
ImplementationFile=RenderSystemDoc.cpp

[CLS:CD3DRendererTestAppView]
Type=0
BaseClass=CView
HeaderFile=RenderSystemView.h
ImplementationFile=RenderSystemView.cpp
Filter=C
VirtualFilter=VWC
LastObject=ID_TEXTURE_CHECKER

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_APP_EXIT
Command4=ID_VIEW_STATUS_BAR
Command5=ID_APP_ABOUT
CommandCount=5

[MNU:IDR_D3DRENTYPE]
Type=1
Class=?
Command1=ID_APP_EXIT
Command2=ID_VIEW_STATUS_BAR
Command3=ID_VIEW_DIRECTX
Command4=ID_VIEW_OPENGL
Command5=ID_TEXTURE_CHECKER
Command6=ID_TEXTURE_PLASMA
Command7=ID_TEXTURE_ANIMATED
Command8=ID_APP_ABOUT
CommandCount=8

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_SWITCH_TO_D3D
Command2=ID_SWITCH_TO_OGL
CommandCount=2

