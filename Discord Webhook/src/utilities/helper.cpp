#include "main.h"

#define INTERNET_FLAG_SECURE_WITHOUT_CACHE INTERNET_FLAG_SECURE | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_NO_CACHE_WRITE

namespace wavvve {
	std::string Helper::WstringToString( const std::wstring &text ) {
		if ( text.empty() )
			return std::string();

		int outSize = WideCharToMultiByte( CP_UTF8, 0, &text[0], ( int )text.size(), NULL, 0, NULL, NULL );
		std::string ret( outSize, 0 );
		WideCharToMultiByte( CP_UTF8, 0, &text[0], ( int )text.size(), &ret[0], outSize, NULL, NULL );

		return ret;
	}

	std::wstring Helper::StringToWstring( const std::string &text ) {
		if ( text.empty() )
			return std::wstring();

		int outSize = MultiByteToWideChar( CP_UTF8, 0, &text[0], ( int )text.size(), NULL, 0 );
		std::wstring ret( outSize, 0 );
		MultiByteToWideChar( CP_UTF8, 0, &text[0], ( int )text.size(), &ret[0], outSize );
		return ret;
	}

	void Helper::StringReplace( std::string &text, const std::string &from, const std::string &to ) {
		size_t startPos = 0;
		while ( ( startPos = text.find( from, startPos ) ) != std::string::npos ) {
			text.replace( startPos, from.length(), to );
			startPos += to.length();
		}
	}

	std::string Helper::InternetPOSTRequest( const std::string site, const std::string path, const std::string parameters, const std::string header ) {
		HINTERNET hInternet = InternetOpenW(L"DiscordWebHook", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0 );

		if ( !hInternet ) {
			MessageBoxA( NULL, "Cannot connect to server.", __FUNCTION__, MB_SYSTEMMODAL | MB_ICONERROR );
			return NULL;
		}

		std::wstring wSite = StringToWstring( site ), wPath = StringToWstring( path );

		HINTERNET hConnect = InternetConnectW( hInternet, wSite.c_str(), INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL );

		if ( !hConnect ) {
			MessageBoxA( NULL, "Error sending message to server.", __FUNCTION__, MB_SYSTEMMODAL | MB_ICONERROR );
			return NULL;
		}

		const wchar_t *pszAcceptTypes[] = {  L"text/*" , NULL };

		HINTERNET hRequest = HttpOpenRequestW( hConnect, L"POST" , wPath.c_str(), L"HTTP/1.1" , NULL, pszAcceptTypes, INTERNET_FLAG_SECURE_WITHOUT_CACHE, 0 );

		if ( !hRequest ) {
			MessageBoxA( NULL, "Error sending message to server 2.", __FUNCTION__, MB_SYSTEMMODAL | MB_ICONERROR );
			return NULL;
		}

		std::wstring wHeader = StringToWstring( header );
		if ( !wHeader.empty() ) {
			BOOL bAddRequestHeader = HttpAddRequestHeadersW( hRequest, wHeader.c_str(), ( DWORD )wHeader.length(), HTTP_ADDREQ_FLAG_ADD );
			if ( !bAddRequestHeader ) {
				MessageBoxA( NULL, "Error add request header.", __FUNCTION__, MB_SYSTEMMODAL | MB_ICONERROR );
				return NULL;
			}
		}

		BOOL bRequestSend = HttpSendRequestW( hRequest, NULL, 0, (LPVOID)parameters.data(), ( DWORD )parameters.length() );

		if ( !bRequestSend ) {
			MessageBoxA( NULL, "Error sending message to server 3.", __FUNCTION__, MB_SYSTEMMODAL | MB_ICONERROR );
			return NULL;
		}

		std::string response;
		const int iBufferSize = 1024;
		char szBuffer[iBufferSize];

		BOOL bKeepReading = true;
		DWORD dwBytesRead = -1;

		while ( bKeepReading && dwBytesRead != 0 ) {
			bKeepReading = InternetReadFile( hRequest, szBuffer, iBufferSize, &dwBytesRead );
			response.append( szBuffer, dwBytesRead );
		}

		InternetCloseHandle( hRequest );
		InternetCloseHandle( hConnect );
		InternetCloseHandle( hInternet );

		return response;
	}
}