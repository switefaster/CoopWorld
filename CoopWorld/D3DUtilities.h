#pragma once

#include <dxgi.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <string>
#include <sstream>
#include <wrl.h>
#include <cassert>

#include "DXException.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

constexpr XMFLOAT4X4 Identity4x4 = XMFLOAT4X4(
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
);

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
};

#ifndef ThrowIfFailed
#define ThrowIfFailed(statement) \
    { \
        HRESULT hr__ = (statement); \
        if(FAILED(hr__)) { throw DXException(hr__); } \
    }
#endif
