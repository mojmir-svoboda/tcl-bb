#include "tcl-bb.h"

struct {
	char *cmd;
	UINT msg;
	WPARAM wParam;
	LPARAM lParam;
} Tcl_PostInfo[] = {
	{"editstyle",BB_EDITFILE,0,NULL},
	{"editmenu",BB_EDITFILE,1,NULL},
	{"editplugins",BB_EDITFILE,2,NULL},
	{"editextension",BB_EDITFILE,3,NULL},
	{"editblackbox",BB_EDITFILE,4,NULL},
	{"mainmenu",BB_MENU,0,NULL},
	{"workspacemenu",BB_MENU,1,NULL},
	{"toolbarmenu",BB_MENU,2,NULL},
	{"systembarmenu",BB_MENU,3,NULL},
	{"slitmenu",BB_MENU,4,NULL},
	{"hidemenu",BB_HIDEMENU,NULL,NULL},
	{"toggletray",BB_TOGGLESYSTEMBAR,NULL,NULL},
	{"togglesystembar",BB_TOGGLESYSTEMBAR,NULL,NULL},
	{"toggleslit",BB_TOGGLESLIT,NULL,NULL},
	{"toggletoolbar",BB_TOGGLETOOLBAR,NULL,NULL},
	{"deskleft",BB_WORKSPACE,0,NULL},
	{"deskright",BB_WORKSPACE,1,NULL},
	{"adddesk",BB_WORKSPACE,2,NULL},
	{"deletedesk",BB_WORKSPACE,3,NULL},
	{"deldesk",BB_WORKSPACE,3,NULL},
	{"shutdown",BB_SHUTDOWN,0,NULL},
	{"reboot",BB_SHUTDOWN,1,NULL},
	{"logoff",BB_SHUTDOWN,2,NULL},
	{"hibernate",BB_SHUTDOWN,3,NULL},
	{"suspend",BB_SHUTDOWN,4,NULL},
	{"lockworkstation",BB_SHUTDOWN,5,NULL},
	{"quit",BB_QUIT,NULL,NULL},
	{"restart",BB_RESTART,NULL,NULL},
	{"reconfig",BB_RECONFIGURE,NULL,NULL},
	{"run",BB_RUN,NULL,NULL},
	{"windowgather",BB_WORKSPACE,5,NULL},
	{"gatherwindows",BB_WORKSPACE,5,NULL},
	{"toggleplugins",BB_TOGGLEPLUGINS,NULL,NULL},
	{"aboutstyle",BB_ABOUTSTYLE,NULL,NULL},
	{"aboutplugins",BB_ABOUTPLUGINS,NULL,NULL},
	{NULL,NULL,NULL,NULL}
};

int Tcl_PostMessage(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
	int i;
	char *cmd = trimnamespace(Tcl_GetString(objv[0]));

	for(i=0;!_stricmp(Tcl_PostInfo[i].cmd,cmd) && Tcl_PostInfo[i].cmd != NULL;i++);

	if(Tcl_PostInfo[i].cmd == NULL)
		return TCL_OK;

	PostMessage(GetBBWnd(),Tcl_PostInfo[i].msg,Tcl_PostInfo[i].wParam,Tcl_PostInfo[i].lParam);

	return TCL_OK;
}