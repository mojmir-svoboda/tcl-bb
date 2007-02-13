#include "tcl-bb.h"
extern Tcl_Interp *interp;

int Tcl_CurrentWorkspace(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
	if(objc != 2)
	{
		Tcl_WrongNumArgs(interp,1,objv,"callback");
		return TCL_ERROR;
	}

	const char *callback = Tcl_GetString(objv[1]);
	const char *arrayset = "if {[namespace exists GIZMONIC] == 0} { namespace eval GIZMONIC {} }\n" \
		"set GIZMONIC::MYSTERY_SCIENCE_THEATER_3000([array size GIZMONIC::MYSTERY_SCIENCE_THEATER_3000]) %s";
	char *temp = new char[ strlen(callback) + strlen(arrayset) + 1 ];

	sprintf(temp, arrayset, callback);
	Tcl_Eval(interp, temp);
	delete[] temp;
#if defined(_DEBUG)
	Process_Callbacks(1);
#else
	if (TryEnterCriticalSection(&crit) == TRUE)
		SendMessage(GetBBWnd(),BB_LISTDESKTOPS, NULL, NULL);
#endif

	return TCL_OK;
}

void Process_Callbacks(int thisWorkspace)
{
	char *tcl = "set i 0\n" \
		"while {$i < [array size GIZMONIC::MYSTERY_SCIENCE_THEATER_3000]} {\n" \
		"	$GIZMONIC::MYSTERY_SCIENCE_THEATER_3000($i) %d\n" \
		"	incr i\n" \
		"}\n" \
		"array unset GIZMONIC::MYSTERY_SCIENCE_THEATER_3000\n";

	char *temp = new char[ strlen(tcl) + 33 + 1 ];
	sprintf(temp, tcl, thisWorkspace);
	Tcl_Eval(interp, temp);
	delete[] temp;

	LeaveCriticalSection(&crit);
}