#ifndef CSOUNDSESSION_H
#define CSOUNDSESSION_H

#include <string>
#include <csound.hpp>
#include <csPerfThread.hpp>

class CsoundSession : public Csound{

	std::string m_csd;
	CsoundPerformanceThread *m_pt;

public:

	CsoundSession(std::string const &csdFileName) : Csound() {
	m_pt = NULL;
	m_csd = "";
	if(!csdFileName.empty()){
		m_csd = csdFileName;
		startThread();
	}
};
	void startThread();
	void resetSession(std::string const &csdFileName);
	void stopPerformance();
	void mainLoop();

};

#endif
