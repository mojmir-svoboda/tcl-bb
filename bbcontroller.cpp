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

#include "tcl-bb.h"
#include <shellapi.h>
#include <Mmsystem.h>
int Tcl_Pause(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
	double sec;
	if(objc == 1)
	{
		MessageBox(NULL,"Press OK to Continue!", "tcl-bb Paused", MB_OK|MB_TOPMOST|MB_SETFOREGROUND);
		return TCL_OK;
	}

	if(TCL_ERROR == Tcl_GetDoubleFromObj(interp, objv[1], &sec))
		return TCL_ERROR;

	if(sec <= 0)
		MessageBox(NULL,"Press OK to Continue!", "tcl-bb Paused", MB_OK|MB_TOPMOST|MB_SETFOREGROUND);
	else
		Sleep((DWORD)(sec*1000.0));

	return TCL_OK;
}

int Tcl_Broam(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
	if(objc == 1)
	{
		Tcl_WrongNumArgs(interp,1,objv,"@broam[ @broam[ ...]]");
		return TCL_ERROR;
	}
	for(int i=1; i<objc; i++)
		SendMessage(GetBBWnd(),BB_BROADCAST,NULL,(LPARAM)Tcl_GetString(objv[i]));

	return TCL_OK;
}

int Tcl_Label(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
	if(objc != 2)
	{
		Tcl_WrongNumArgs(interp,1,objv,"label");
		return TCL_ERROR;
	}
	SendMessage(GetBBWnd(),BB_SETTOOLBARLABEL, NULL,(LPARAM)Tcl_GetString(objv[1]));
	return TCL_OK;
}

int Tcl_Shell(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
	int i = 0;
	if( !_stricmp(trimnamespace(Tcl_GetString(objv[0])), "shellexecute"))
		i = 1;

	if ((objc<3 || objc>5) && i)
	{
		Tcl_WrongNumArgs(interp,1,objv,"operation file[ params[ \"working directory\"]]");
		return TCL_ERROR;
	}
	else if ((objc < 2 || objc > 4) && !i)
	{
		Tcl_WrongNumArgs(interp,1,objv,"file[ params[ \"working directory\"]]");
		return TCL_ERROR;
	}

	char *op, *file, *param, *wdir;
	op = file = param = wdir = NULL;
	
	op = trimnamespace(Tcl_GetString(objv[i++]));
	if (!_stricmp(op, "exec"))
		op = NULL;
	file = Tcl_GetString(objv[i++]);
	if (objc >= i + 1)
		param = Tcl_GetString(objv[i++]);
	if (objc == i + 1)
		wdir = Tcl_GetString(objv[i]);

	ShellExecute(NULL,op,file,param,wdir,SW_SHOWNORMAL);
	return TCL_OK;
}

int Tcl_Style(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
	if(objc!=2)
	{
		Tcl_WrongNumArgs(interp,1,objv,"style");
		return TCL_ERROR;
	}

	SendMessage(GetBBWnd(),BB_SETSTYLE,NULL,(LPARAM)Tcl_GetString(objv[1]));
	return TCL_OK;
}

int Tcl_Sound(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
		if(objc==1 || objc>3)
		{
			if(!_stricmp(Tcl_GetString(objv[0]),"sound"))
				Tcl_WrongNumArgs(interp,1,objv,"file[ sync|async]");
			else
				Tcl_WrongNumArgs(interp,1,objv,"file");
			return TCL_ERROR;
		}

		int flag = SND_FILENAME;
		char *p;
		if(objc==2)
			p = Tcl_GetString(objv[0]);
		else
			p = Tcl_GetString(objv[2]);

		if(!_strnicmp(p,"async",5))
			flag |= SND_ASYNC;
		else
			flag |= SND_SYNC;

		PlaySound(Tcl_GetString(objv[1]),NULL,flag);
		return TCL_OK;
}

int Tcl_Log(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
	if(objc!=2)
	{
		Tcl_WrongNumArgs(interp,1,objv,"logdata");
		return TCL_ERROR;
	}
	Log("tcl-bb", Tcl_GetString(objv[1]));
	return TCL_OK;
}


int Tcl_MBox(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
	if(objc!=2)
	{
		Tcl_WrongNumArgs(interp,1,objv,"messagebox_text");
		return TCL_ERROR;
	}
	//MBoxErrorValue(Tcl_GetString(objv[1]));
	MessageBox(NULL, Tcl_GetString(objv[1]), "MBOX", MB_OK);
	return TCL_OK;
}