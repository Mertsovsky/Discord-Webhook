#ifndef _HELPER_HEADER_
#define _HELPER_HEADER_

namespace wavvve {

	class Helper {
	public:
		static std::string                       WstringToString( const std::wstring &text );
		static std::wstring                      StringToWstring( const std::string &text );

		static void                              StringReplace( std::string &text, const std::string &from, const std::string &to );

		static std::string                       InternetPOSTRequest( const std::string site, const std::string path, const std::string parameters, const std::string header = "");
	};
}

#endif