typedef struct _dynabroam {
	char* broam;
	char* tcl;
	_dynabroam* next;
	_dynabroam* prev;
} DBROAM, *LPDBROAM;

extern LPDBROAM dbroams_tree;
void DestroyDynamicBroam(LPDBROAM);
void FindAndExecuteBroam(Tcl_Interp *interp, const char* broam, const char* args, bool suppressErrors);