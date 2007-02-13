#include "tcl-bb.h"
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