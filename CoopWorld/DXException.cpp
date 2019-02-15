#include "DXException.h"

#include <comdef.h>
#include <sstream>

DXException::DXException( HRESULT hr ) :
    ErrCode( hr ),
	StackTrace(boost::stacktrace::stacktrace())
{
}

std::wstring DXException::ToString()
{
    _com_error err( ErrCode );
    std::wstring msg = err.ErrorMessage();
	std::wstringstream wss;
	wss << "Something went wrong for:" << std::endl << msg << std::endl << "Stack:" << std::endl << boost::stacktrace::to_string(StackTrace).c_str();
	return wss.str();
}
