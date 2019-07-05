#pragma once

#ifndef _D3D_UTILITIES_H_
#define _D3D_UTILITIES_H_

#include <dxgi.h>
#include <d3d11_4.h>
#include <d2d1.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <string>
#include <sstream>
#include <wrl.h>
#include <cassert>
#include <minwindef.h>

#include "DXException.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

#ifdef min
    #undef min
#endif

constexpr XMFLOAT4X4 Identity4x4 =
    XMFLOAT4X4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );

extern bool operator==( const D3D11_INPUT_ELEMENT_DESC& l, const D3D11_INPUT_ELEMENT_DESC& r );

class D3DUtilities {
    public:
        static bool IsKeyDown( int vKeyCode );
        static UINT AlignConstantBufferByte( UINT byteSize )
        {
            return ( byteSize + 15 ) & ~15;
        }
        static Microsoft::WRL::ComPtr<ID3DBlob> LoadBinary( const std::wstring& file );
        static Microsoft::WRL::ComPtr<ID3DBlob> CompileShader( const std::wstring& fileName, const D3D_SHADER_MACRO* defines, const std::string& entry, const std::string& target );
        static XMMATRIX InvTranspose( FXMMATRIX M );
        static std::wstring str_to_wstr( const std::string& str );
};

#ifndef ThrowIfFailed
#define ThrowIfFailed(statement) \
    { \
        HRESULT hr__ = (statement); \
        if(FAILED(hr__)) { throw DXException(hr__); } \
    }
#endif

#endif
