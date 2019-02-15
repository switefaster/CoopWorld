#pragma once

#include <Windows.h>
#include <string>
#include <boost/stacktrace.hpp>

class DXException {
    public:
        DXException() = default;
        DXException( HRESULT hr );

        std::wstring ToString();

        HRESULT ErrCode = S_OK;
		boost::stacktrace::stacktrace StackTrace;
};

