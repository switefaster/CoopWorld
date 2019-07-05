#pragma once

#ifndef _DX_EXCEPTION_H_
#define _DX_EXCEPTION_H_

#include <Windows.h>
#include <string>
#include <boost/stacktrace.hpp>

class DXException {
    public:
        DXException() = default;
        DXException( HRESULT hr );

        std::wstring ToString();

        HRESULT ErrCode = S_OK;
#if defined(DEBUG) || defined(_DEBUG)
		boost::stacktrace::stacktrace StackTrace;
#endif
};

#endif
