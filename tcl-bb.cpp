#include "tcl-bb.h"
#include "dynabroam.h"
#include "tclInt.h"

#define PLUGIN_LOADED	0
#define PLUGIN_FAILED	1
#define MY_PLUGIN_NAME "Tcl-bb"
#define MY_PLUGIN_VERSION "2.4.8.2"
#define MY_PLUGIN_AUTHOR "Tres`ni"
#define MY_PLUGIN_EMAIL "tresni@crackmonkey.us"
#define MY_PLUGIN_LINK "http://www.xoblite.net/plugins.html"
#define MY_PLUGIN_DATE "30 JUNE 2006"
#define MY_PLUGIN_BROAMS "\003@tcl-bb eval" "@tcl-bb evalfile" "@tcl\0" "@tcl-bb eval\0" "@tcl-bb evalfile\0" "@tcl\0\0"
#define MY_PLUGIN_DEFAULT MY_PLUGIN_NAME " " MY_PLUGIN_VERSION " (" MY_PLUGIN_DATE ")"

#ifdef _DEBUG
#undef MBoxErrorValue
#define MBoxErrorValue(a) MessageBox(NULL,a,"tcl-bb", MB_OK)
#endif

#define TCL_INIT 1

extern "C" {
	DLL_EXPORT int beginPluginEx(HINSTANCE, HWND);
	DLL_EXPORT int beginPlugin(HINSTANCE);
	DLL_EXPORT void endPlugin(HINSTANCE);
	DLL_EXPORT LPCSTR pluginInfo(int);
};

int msgs[] = { BB_BROADCAST, BB_DESKTOPINFO, 0 };
HWND hWnd;
char path[MAX_PATH+1];
bool suppressErrors = false;
Tcl_Interp *interp;

CRITICAL_SECTION crit;

LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case BB_BROADCAST:
		{
			char temp[MAX_LINE_LENGTH];
			strcpy(temp, (LPCSTR)lParam);

			if (!_strnicmp(temp,"@tcl-bb",7))
			{
				char broam[MAX_LINE_LENGTH], cmd[MAX_LINE_LENGTH], extra[MAX_LINE_LENGTH];
				char *token[2];

				token[0] = broam;
				token[1] = cmd;
				broam[0] = cmd[0] = extra[0] = '\0';
				BBTokenize(temp,token,2,extra);

				if(!_stricmp(cmd,"eval"))
				{
					if(Tcl_Eval(interp,extra) == TCL_ERROR && !suppressErrors)
						MBoxErrorValue(Tcl_GetString(Tcl_GetObjResult(interp)));
				}
				else if(!_stricmp(cmd,"evalfile"))
				{
					if(Tcl_EvalFile(interp,extra) == TCL_ERROR && !suppressErrors)
						MBoxErrorValue(Tcl_GetString(Tcl_GetObjResult(interp)));
				}
				else if (!suppressErrors)
				{
					MBoxErrorValue("Proper bro@m calling is:\n"
						"@tcl-bb eval <tcl statement>\n"
						"@tcl-bb evalfile <path to tcl script>");
				}
			}
			else if (!_strnicmp(temp, "@tcl ", 4)) {
				char broam[MAX_LINE_LENGTH], tcl[MAX_LINE_LENGTH];
				char *token[1];

				token[0] = broam;
				broam[0] = tcl[0] = '\0';
				BBTokenize(temp,token,1,tcl);

				if (Tcl_Eval(interp, tcl) == TCL_ERROR && !suppressErrors)
					MBoxErrorValue(Tcl_GetString(Tcl_GetObjResult(interp)));
				
			}
			else
			{
				char broam[MAX_LINE_LENGTH], extra[MAX_LINE_LENGTH];
				char *token[1];

				token[0] = broam;
				broam[0] = extra[0] = '\0';

				BBTokenize(temp,token,1,extra);
				FindAndExecuteBroam(interp, broam, extra, suppressErrors);
			}
		}
			return 0;
		case BB_DESKTOPINFO:
		{
			DesktopInfo* info = (DesktopInfo*)lParam;

			if (info == NULL) return 0;
			if (info->isCurrent)
					Process_Callbacks(info->number);
		}
			return 0;
		case WM_CLOSE:
			return 1;
		default:
			return DefWindowProc(hWnd,Msg,wParam,lParam);
	}
}
#ifdef TCL_INIT
char* CleanTclPath(const char* path)
{
	char *p = (char *)malloc(sizeof(char) * (strlen(path) + 1));
	strcpy(p, path);

	for (unsigned int i = 0; i < strlen(p); i++) {
		if (p[i] == '\\') p[i] = '/';
	}
	return p;
}
void SetScriptDir(const char* path) {
	char *s = "set tcl_library %s%s/";
	char *t = NULL;
#if defined(_DEBUG)
	char *p = "E:/shell/xoblite/plugins/Tcl4BB/";
#else
	char *p = CleanTclPath(path);
#endif

	t = (char *)malloc(sizeof(char) * (strlen(p) + strlen(s) + 7 + 1));
	sprintf(t, s, p, "library");
	Tcl_Eval(interp, t);
	free(t);
	
#if !defined(_DEBUG)
	free(p);
#endif
}

void SetLibraryDir(const char* path) {
	char *p = CleanTclPath(path);

	char *t = (char *)malloc(sizeof(char) * (strlen(p) + 7 + 2));
	sprintf(t, "%s%s/", p, "library");
	TclSetLibraryPath(Tcl_NewStringObj(t, lstrlen(t)));
	free(t);
	free(p);
}
#endif

int beginPlugin(HINSTANCE h)
{
	return beginPluginEx(h,NULL);
}

int beginPluginEx(HINSTANCE h, HWND w)
{
	GetModuleFileName(h,path,MAX_PATH);

	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.lpfnWndProc = WndProc;
	wc.hInstance = h;
	wc.lpszClassName = MY_PLUGIN_NAME;
	if(!RegisterClass(&wc))
	{
		MBoxErrorValue("Could not register window class...");
		return PLUGIN_FAILED;
	}

	hWnd = CreateWindowEx(
			WS_EX_TOOLWINDOW,
			MY_PLUGIN_NAME,
			NULL,
			WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
			0,0,0,0,
			NULL,
			NULL,
			h,
			NULL);
	if(hWnd == NULL)
	{
		MBoxErrorValue("Could not create window...");
		return PLUGIN_FAILED;
	}

	SendMessage(GetBBWnd(),BB_REGISTERMESSAGE,(WPARAM)hWnd,(LPARAM)msgs);
	InitializeCriticalSection(&crit);

	Tcl_FindExecutable(path);
	interp = Tcl_CreateInterp();
	Dde_Init(interp);
	Registry_Init(interp);

	Tcl_PkgProvide(interp, "tcl-bb", MY_PLUGIN_VERSION);

	// Bro@m users
	Tcl_CreateObjCommand(interp,"tcl-bb::editstyle",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::editmenu",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::editplugins",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::editextension",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::editblackbox",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::mainmenu",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::workspacemenu",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::toolbarmenu",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::systembarmenu",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::slitmenu",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::hidemenu",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::toggletray",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::togglesystembar",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::toggleslit",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::toggletoolbar",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::deskleft",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::deskright",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::adddesk",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::deletedesk",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::deldesk",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::shutdown",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::reboot",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::logoff",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::hibernate",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::suspend",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::lockworkstation",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::quit",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::restart",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::reconfig",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::run",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::windowgather",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::gatherwindows",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::toggleplugins",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::aboutstyle",Tcl_PostMessage,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::aboutplugins",Tcl_PostMessage,NULL,NULL);

	// Custom Functions
	Tcl_CreateObjCommand(interp,"tcl-bb::currentdesk",Tcl_CurrentWorkspace,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::pause",Tcl_Pause,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::softpause",Tcl_Pause,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::hardpause",Tcl_Pause,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::bro@m",Tcl_Broam,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::broam",Tcl_Broam,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::label",Tcl_Label,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::style",Tcl_Style,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::exec",Tcl_Shell,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::open",Tcl_Shell,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::shellexecute",Tcl_Shell,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::sound",Tcl_Sound,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::syncsound",Tcl_Sound,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::asyncsound",Tcl_Sound,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::readvar",Tcl_ReadVar,NULL,NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::writevar",Tcl_WriteVar,NULL,NULL);

	// Extra Extra..
	Tcl_CreateObjCommand(interp,"tcl-bb::log", Tcl_Log, NULL, NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::mbox", Tcl_MBox, NULL, NULL);

	// dyna broam, like wonder dog, but better!
	Tcl_CreateObjCommand(interp,"tcl-bb::create_broam", Tcl_RegisterBroam, NULL, NULL);
	Tcl_CreateObjCommand(interp,"tcl-bb::destroy_broam", Tcl_UnregisterBroam, NULL, NULL);

	Tcl_Eval(interp, "namespace eval tcl-bb namespace export *");
/*
	{
		char bbpath[MAX_PATH + 1];
		GetBlackboxPath(bbpath, sizeof(bbpath));
		strncat(bbpath, "library", MAX_PATH - strlen(bbpath));
		for(int i = 0; i < strlen(bbpath); i++)
			if(bbpath[i] == '\\') bbpath[i] = '/';

		char *temp = (char *)malloc(sizeof(char)* (strlen(bbpath) + 19));
		sprintf(temp, "[lappend auto_path %s]", bbpath);
		Tcl_Eval(interp, temp);
		free(temp);
	}

	if (Tcl_Init(interp) == TCL_ERROR)
		MBoxErrorValue(Tcl_GetString(Tcl_GetObjResult(interp)));
*/
#ifdef TCL_MESSAGE_PUMP
	Tcl_SetServiceMode(TCL_SERVICE_ALL);
#endif

	int i = lstrlen(path)-1;
	while(path[i]!='\\' && path[i]!='/')i--;
	path[++i] = 0;

#ifdef TCL_INIT
	SetScriptDir(path);
	SetLibraryDir(path);
	if (Tcl_Init(interp) == TCL_ERROR)
	{
		MBoxErrorValue(Tcl_GetString(Tcl_GetObjResult(interp)));
		return PLUGIN_FAILED;
	}
#endif

	strcat(path,"tcl-bb.rc");
/*
	if(ReadBool(path,"tcl-bb.import.edit:",false))
		Tcl_Eval(interp,"namespace import tcl-bb::edit*");
	if(ReadBool(path,"tcl-bb.import.menu:",false))
		Tcl_Eval(interp,"namespace import tcl-bb::*menu");
	if(ReadBool(path,"tcl-bb.import.toggle:",false))
		Tcl_Eval(interp,"namespace import tcl-bb::toggle*");
	if(ReadBool(path,"tcl-bb.import.workspace:",false))
		Tcl_Eval(interp,"namespace import tcl-bb::*desk* tcl-bb::*window*");
	if(ReadBool(path,"tcl-bb.import.shutdown:",false))
		Tcl_Eval(interp,"namespace import tcl-bb::shutdown tcl-bb::reboot tcl-bb::logoff " \
			"tcl-bb::hibernate tcl-bb::suspend tcl-bb::lockworkstation");
	if(ReadBool(path,"tcl-bb.import.bb4win:",false))
		Tcl_Eval(interp,"namespace import tcl-bb::quit tcl-bb::restart tcl-bb::reconfig " \
			"tcl-bb::label tcl-bb::style");
	if(ReadBool(path,"tcl-bb.import.about:",false))
		Tcl_Eval(interp,"namespace import tcl-bb::about*");
	if(ReadBool(path,"tcl-bb.import.pause:",false))
		Tcl_Eval(interp,"namespace import tcl-bb::*pause");
	if(ReadBool(path,"tcl-bb.import.broam:",false))
		Tcl_Eval(interp,"namespace import tcl-bb::bro?m");
	if(ReadBool(path,"tcl-bb.import.shell:",false))
		Tcl_Eval(interp,"namespace import tcl-bb::*exec* tcl-bb::open tcl-bb::run");
	if(ReadBool(path,"tcl-bb.import.sound:",false))
		Tcl_Eval(interp,"namespace import tcl-bb::*sound");
	if(ReadBool(path,"tcl-bb.import.var:",false))
		Tcl_Eval(interp,"namespace import tcl-bb::*var");

	if(ReadBool(path,"tcl-bb.import.all:",false))
		Tcl_Eval(interp,"namespace import tcl-bb::*");
*/
	//ZeroMemory(temp,1024);
	char temp[MAX_LINE_LENGTH];
	suppressErrors = ReadBool(path, "tcl-bb.suppressErrors:", false);
	strcpy(temp,ReadString(path,"tcl-bb.start.eval:","\0"));
	if(temp[0]!=0)
	{
		if (Tcl_EvalFile(interp,temp) == TCL_ERROR && !suppressErrors)
			MBoxErrorValue(Tcl_GetString(Tcl_GetObjResult(interp)));
	}
	

	return PLUGIN_LOADED;
}

void endPlugin(HINSTANCE h)
{
	DestroyDynamicBroam(dbroams_tree);
	SendMessage(GetBBWnd(),BB_UNREGISTERMESSAGE,(WPARAM)hWnd,(LPARAM)msgs);
	DestroyWindow(hWnd);
	UnregisterClass(MY_PLUGIN_NAME,h);	
	Tcl_DeleteInterp(interp);
	Tcl_Finalize();
	DeleteCriticalSection(&crit);
}

LPCSTR pluginInfo(int field)
{
	switch(field)
	{
		case PLUGIN_NAME: return MY_PLUGIN_NAME;
		case PLUGIN_AUTHOR: return MY_PLUGIN_AUTHOR;
		case PLUGIN_EMAIL: return MY_PLUGIN_EMAIL;
		case PLUGIN_LINK: return MY_PLUGIN_LINK;
		case PLUGIN_RELEASEDATE: return MY_PLUGIN_DATE;
		case PLUGIN_VERSION: return MY_PLUGIN_VERSION;
		case PLUGIN_BROAMS: return MY_PLUGIN_BROAMS;
		default: return MY_PLUGIN_DEFAULT;
	}
}

/*
int Tcl_CustomFunc(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
	char *cmd;
	cmd = trimnamespace(Tcl_GetString(objv[0]));
	Log(cmd,"Tcl_CustomFunc");
	//MessageBox(NULL, p, "Tcl_CustomFunc", MB_OK);

	for(int i=0;_stricmp(Tcl_CommandChecks[i].cmd,cmd) && Tcl_CommandChecks[i].cmd != NULL;i++);

	if(Tcl_CommandChecks[i].cmd == NULL)
		return TCL_OK;

	if(objc-1 < Tcl_CommandChecks[i].req_args || objc-1 > Tcl_CommandChecks[i].max_args)
	{
		Tcl_WrongNumArgs(interp,1,objv,Tcl_CommandChecks[i].arg_exp);
		return TCL_ERROR;
	}

	return Tcl_CommandChecks[i].function(interp,objc,objv);
}
*/
char *trimnamespace(char *func)
{
	char *p = strstr(func,"tcl-bb::");
	if(p == NULL)
		return func;
	else
		return p+8;
}