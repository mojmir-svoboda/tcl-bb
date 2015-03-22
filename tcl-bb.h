/*
Copyright (c) 2007, Brian Hartvigsen
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * The names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
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

#include <blackbox/BBApi.h>
#define TCL_CFGVAL_ENCODING "cp1252"
#define TCL_THREADS 1 
#define STATIC_BUILD
#define TCL_CFG_OPTIMIZED
#include <tcl.h>
#undef TCLCFGVAL_ENCODING
#undef TCL_THREADS
#undef STATIC_BUILD
#undef TCL_CFG_OPTIMIZED

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
