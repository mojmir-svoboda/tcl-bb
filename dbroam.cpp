#include <tcl.h>
#include <unordered_map>

#define _CRT_SECURE_NO_DEPRECATE

#ifndef LPCSTR
#define LPCSTR const char *
#endif
class dbroam_factory {
private:
	std::unordered_map<char *, char *> workers;
	
public:
	void NewDBroam(char *name, char *tcl) {
		workers.insert(std::make_pair(name, tcl));
		//workers.push_back(new dbroam(name, tcl));
	}

	void DeleteDBroam(char *name) {
		//workers.
	}
};