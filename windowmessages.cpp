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