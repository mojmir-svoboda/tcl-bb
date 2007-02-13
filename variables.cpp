#include "tcl-bb.h"

int Tcl_ReadVar(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
	char val[1024];

	if(objc!=2)
	{
		Tcl_WrongNumArgs(interp,1,objv,"var_name");
		return TCL_ERROR;
	}

	strcpy(val,"tcl-bb.var.");
	strcat(val,Tcl_GetString(objv[1]));
	strcat(val,":");

	strcpy(val,ReadString(path,val,""));

	Tcl_SetResult(interp,val,TCL_VOLATILE);
	return TCL_OK;
}

int Tcl_WriteVar(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
	char val[1024];
	if(objc!=3)
	{
		Tcl_WrongNumArgs(interp,1,objv,"var_name variable");
		return TCL_ERROR;
	}
	strcpy(val,"tcl-bb.var.");
	strcat(val,Tcl_GetString(objv[1]));
	strcat(val,":");
	WriteString(path,val,Tcl_GetString(objv[2]));
	return TCL_OK;
}