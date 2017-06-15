; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDigitRecView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "DigitRec.h"
LastPage=0

ClassCount=7
Class1=CDigitRecApp
Class2=CDigitRecDoc
Class3=CDigitRecView
Class4=CMainFrame

ResourceCount=5
Resource1=IDR_MAINFRAME
Class5=CAboutDlg
Resource2=IDD_DIALOG_TRAIN
Resource3=IDD_DIALOG_REC
Class6=CTestDialog
Resource4=IDD_ABOUTBOX
Class7=CPreprocessDlg
Resource5=IDD_DIALOG_PREPROCESS

[CLS:CDigitRecApp]
Type=0
HeaderFile=DigitRec.h
ImplementationFile=DigitRec.cpp
Filter=N

[CLS:CDigitRecDoc]
Type=0
HeaderFile=DigitRecDoc.h
ImplementationFile=DigitRecDoc.cpp
Filter=N

[CLS:CDigitRecView]
Type=0
HeaderFile=DigitRecView.h
ImplementationFile=DigitRecView.cpp
Filter=C
BaseClass=CScrollView
VirtualFilter=VWC
LastObject=CDigitRecView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=ID_FILE_SAVE_AS




[CLS:CAboutDlg]
Type=0
HeaderFile=DigitRec.cpp
ImplementationFile=DigitRec.cpp
Filter=D
LastObject=CAboutDlg

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
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE_AS
Command4=ID_FILE_PRINT
Command5=ID_FILE_PRINT_PREVIEW
Command6=ID_FILE_PRINT_SETUP
Command7=ID_FILE_MRU_FILE1
Command8=ID_FILE_ROTATE
Command9=ID_APP_EXIT
Command10=ID_VIEW_TOOLBAR
Command11=ID_VIEW_STATUS_BAR
Command12=IDM_TRAIN
Command13=IDM_REC
Command14=IDM_PREPROCESS
Command15=ID_APP_ABOUT
CommandCount=15

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE_AS
Command4=ID_FILE_ROTATE
Command5=ID_APP_ABOUT
CommandCount=5

[DLG:IDD_DIALOG_TRAIN]
Type=1
Class=?
ControlCount=22
Control1=ID_BUTTON_TRAIN,button,1342242817
Control2=IDC_EDIT_MAXEPOCH,edit,1350631552
Control3=IDC_EDIT_ERROR,edit,1350631552
Control4=IDC_EDIT_LEARNRATE,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT_HIDLYR,edit,1350631552
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC_ERROR,static,1342177287
Control11=IDC_EDIT_FILE,edit,1350633600
Control12=IDC_BUTTON_BROWSE,button,1342242816
Control13=ID_BUTTON_STOP,button,1342242816
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC_SHOW,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_EDIT_TIMESTEP,edit,1350631552
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_EDIT_TRAIN_FILE,edit,1350633600
Control21=IDC_STATIC,static,1342308352
Control22=IDC_BUTTON_SAVEAS,button,1342242816

[DLG:IDD_DIALOG_REC]
Type=1
Class=CTestDialog
ControlCount=10
Control1=IDC_CHECK_DIR,button,1342242819
Control2=IDC_EDIT_FILE,edit,1350633600
Control3=IDC_STATIC,static,1342308352
Control4=IDC_BUTTON_TEST_BROWSE,button,1342242816
Control5=IDC_BUTTON_REC,button,1342242816
Control6=IDC_EDIT_LOAD_TRAIN_FILE,edit,1350633600
Control7=IDC_STATIC,static,1342308352
Control8=IDC_BUTTON_LOAD,button,1342242816
Control9=IDC_STATICIMG,static,1342177287
Control10=IDC_BUTTON_STOP,button,1342242816

[CLS:CTestDialog]
Type=0
HeaderFile=TestDialog.h
ImplementationFile=TestDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_BUTTON_STOP
VirtualFilter=dWC

[DLG:IDD_DIALOG_PREPROCESS]
Type=1
Class=CPreprocessDlg
ControlCount=21
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT_THRES,edit,1350631552
Control4=IDC_EDIT_FILE,edit,1350633600
Control5=IDC_BUTTON_LOAD,button,1342242816
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EDIT_LOWER,edit,1350631552
Control11=IDC_EDIT_UPPER,edit,1350631552
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_EDIT_DIR,edit,1350633600
Control18=IDC_BUTTON_DIR,button,1342242816
Control19=IDC_STATIC,static,1342308352
Control20=IDC_RADIO_TEST,button,1342177289
Control21=IDC_RADIO_TRAIN,button,1342177289

[CLS:CPreprocessDlg]
Type=0
HeaderFile=PreprocessDlg.h
ImplementationFile=PreprocessDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CPreprocessDlg

