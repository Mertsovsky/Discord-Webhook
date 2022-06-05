#ifndef _CVARS_HEADER_
#define _CVARS_HEADER_

namespace wavvve::misc {
	class Cvars {
	public:
		HWND     m_phWnd;
		bool     m_bClosingApp;
	};
}

extern std::unique_ptr<wavvve::misc::Cvars> g_pCvars;

#endif 
