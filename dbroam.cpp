#include <tcl.h>
#include <hash_map>

#define _CRT_SECURE_NO_DEPRECATE

#ifndef LPCSTR
#define LPCSTR const char *
#endif
class dbroam_factory {
private:
	stdext::hash_map<char *, char *> workers;
	
public:
	void NewDBroam(char *name, char *tcl) {
		workers.insert(name, tcl);
		//workers.push_back(new dbroam(name, tcl));
	}

	void DeleteDBroam(char *name) {
		//workers.
	}
};