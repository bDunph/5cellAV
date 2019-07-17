#include "CsoundSession.hpp"

#include <string>

int main(int argc, char **argv){
	std::string csdName = "";
	if(argc > 1) csdName = argv[1];
	CsoundSession *session = new CsoundSession(csdName);
	session->mainLoop();
}
