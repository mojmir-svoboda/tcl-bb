#if (_MSC_VER >= 1400)
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "advapi32.lib")
#define _CRT_SECURE_NO_DEPRECATE
#endif

#define MMNODRV
#define MMNOMMSYSTEM
#define MMNOWAVE
#define MMNOMIDI
#define MMNOAUX
#define MMNOMIXER
#define MMNOTIMER
#define MMNOJOY
#define MMNOMMIO
#define MMNOMCI

#include "../sdk/bbapi.h"
#pragma comment(lib, "../sdk/blackbox.lib")

#include <mmsystem.h>
#pragma comment( lib, "Winmm.lib" )

#define TCL_CFGVAL_ENCODING "cp1252"
#define TCL_THREADS 1 
#define STATIC_BUILD
#define TCL_CFG_OPTIMIZED
#include <tcl.h>
#undef TCLCFGVAL_ENCODING
#undef TCL_THREADS
#undef STATIC_BUILD
#undef TCL_CFG_OPTIMIZED

#if defined(_DEBUG)
#define TCL_LIB_DEBUG "g"
#else
#define TCL_LIB_DEBUG ""
#endif
#define TCL_LIB_THREAD "t"
#define TCL_LIB_STATIC "s"
#define TCL_LIB_MSVCRT "x"

#pragma comment(lib, "tcl" STRINGIFY(TCL_MAJOR_VERSION) STRINGIFY(TCL_MINOR_VERSION) TCL_LIB_THREAD TCL_LIB_STATIC TCL_LIB_DEBUG TCL_LIB_MSVCRT".lib")
#pragma comment(lib, "tcldde12" TCL_LIB_STATIC TCL_LIB_DEBUG TCL_LIB_MSVCRT ".lib")
#pragma comment(lib, "tclreg11" TCL_LIB_STATIC TCL_LIB_DEBUG TCL_LIB_MSVCRT ".lib")

#define BB_ABOUTSTYLE		10203	// (definition reserved by bb4win)
#define BB_ABOUTPLUGINS		10204	// (definition reserved by bb4win)

int Tcl_PostMessage(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
int Tcl_Pause(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
int Tcl_Broam(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
int Tcl_Label(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
int Tcl_Shell(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
int Tcl_Sound(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
int Tcl_Style(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
int Tcl_ReadVar(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
int Tcl_WriteVar(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
int Tcl_Log(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
int Tcl_MBox(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
int Tcl_RegisterBroam(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
int Tcl_UnregisterBroam(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
int Tcl_CurrentWorkspace(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);

// Utility functions
char *trimnamespace(char *func);
void Process_Callbacks(int thisWorkspace);

extern char path[MAX_PATH + 1];
extern CRITICAL_SECTION crit;

EXTERN int Registry_Init(Tcl_Interp *interp);
EXTERN int Dde_Init(Tcl_Interp *interp);