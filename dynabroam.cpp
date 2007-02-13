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
#include "dynabroam.h"

LPDBROAM dbroams_tree = NULL;
LPDBROAM dbroams_branch = NULL;

void DestroyDynamicBroam(LPDBROAM b)
{
	if (b == NULL)
		return;

	if (b->next)
	{
		DestroyDynamicBroam(b->next);
		free(b->next);
	}

	free(b->broam);
	free(b->tcl);
}

LPDBROAM FindDynamicBroam(const char* broam)
{
	LPDBROAM b = dbroams_tree;
	while (b != NULL)
	{
		if (!_stricmp(b->broam, broam))
			break;
		else
			b = b->next;
	}
	return b;
}

void FindAndExecuteBroam(Tcl_Interp *interp, const char* broam, const char* args, bool suppressErrors)
{
	LPDBROAM itt = FindDynamicBroam(broam + 1);
	if (itt != NULL) {
		char *temp;
		if (*args != 0) {
			temp = (char *)malloc(sizeof(char) * (strlen(itt->tcl) + strlen(args) + 6));
			sprintf(temp, "%s { %s }", itt->tcl, args);
		}
		else
			temp = itt->tcl;
		
		if (Tcl_Eval(interp, temp) == TCL_ERROR && !suppressErrors)
			MBoxErrorValue(Tcl_GetString(Tcl_GetObjResult(interp)));

		if (temp != itt->tcl)
			free(temp);
	}
}

bool CheckBroam(char *b)
{
	if(strchr(b, ' ') != NULL &&
		strchr(b, '\t') != NULL &&
		strchr(b, '\r') != NULL &&
		strchr(b, '\n') != NULL)
			return false;

	return true;
}

int Tcl_RegisterBroam(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
	if(objc!=3)
	{
		Tcl_WrongNumArgs(interp,1,objv,"broam tcl_syntax");
		return TCL_ERROR;
	}

	char *t = Tcl_GetString(objv[1]);
	if(!CheckBroam(t))
	{
		Tcl_SetResult(interp, "New broam name must not contain spaces, tabs, or carriage returns!", TCL_STATIC);
		return TCL_ERROR;
	}


	if(dbroams_tree  == NULL)
		dbroams_branch = dbroams_tree = (LPDBROAM)malloc(sizeof(DBROAM));
	else
	{
		dbroams_branch->next = (LPDBROAM)malloc(sizeof(DBROAM));
		dbroams_branch->next->prev = dbroams_branch;
		dbroams_branch = dbroams_branch->next;
	}
	ZeroMemory(dbroams_branch, sizeof(DBROAM));

	//char *t = Tcl_GetString(objv[1]);
	dbroams_branch->broam = (char *)malloc(sizeof(char) * strlen(t) + 1);
	strcpy(dbroams_branch->broam, t);

	t = Tcl_GetString(objv[2]);
	dbroams_branch->tcl = (char *)malloc(sizeof(char) * strlen(t) + 1);
	strcpy(dbroams_branch->tcl, t);

	return TCL_OK;
}

int Tcl_UnregisterBroam(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
	if (objc != 2)
	{
		Tcl_WrongNumArgs(interp, 1, objv, "broam");
		return TCL_ERROR;
	}

	LPDBROAM b = FindDynamicBroam(Tcl_GetString(objv[1]));

	if (b == NULL)
		return TCL_OK;

	if (b->prev)
		b->prev->next = b->next;
	if (b->next)
		b->next->prev = b->prev;
	if (b == dbroams_tree) {
		if (b->next)
			dbroams_tree = b->next;
		else
			dbroams_tree = NULL;
	}

	b->next = NULL;
	b->prev = NULL;

	DestroyDynamicBroam(b);

	return TCL_OK;
}