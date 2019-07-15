#include "DXException.h"

#include <comdef.h>
#include <sstream>

DXException::DXException( HRESULT hr ) :
    ErrCode( hr )
#if defined(DEBUG) || defined(_DEBUG)
	,StackTrace(boost::stacktrace::stacktrace())
#endif
{
}

std::wstring DXException::ToString()
{
    _com_error err( ErrCode );
    std::wstring msg = err.ErrorMessage();
	std::wstringstream wss;
#if defined(DEBUG) || defined(_DEBUG)
	wss << "Something went wrong for:" << std::endl << msg << std::endl << "Stack:" << std::endl << boost::stacktrace::to_string(StackTrace).c_str();
#else
	wss << "Something went wrong for:" << std::endl << msg << std::endl;
#endif
	return wss.str();
}
