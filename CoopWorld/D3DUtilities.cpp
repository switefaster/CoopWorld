#include "D3DUtilities.h"

#include <Windows.h>
#include <fstream>
#include <d3dcompiler.h>

using Microsoft::WRL::ComPtr;

bool D3DUtilities::IsKeyDown( int vKeyCode )
{
    return ( GetAsyncKeyState( vKeyCode ) & 0x8000 ) != 0;
}

ComPtr<ID3DBlob> D3DUtilities::LoadBinary( const std::wstring& file )
{
    std::ifstream fin( file, std::ios::binary );
    fin.seekg( 0, std::ios_base::end );
    std::fstream::pos_type size = fin.tellg();
    fin.seekg( 0, std::ios_base::beg );
    ComPtr<ID3DBlob> blob;
    ThrowIfFailed( D3DCreateBlob( size, blob.GetAddressOf() ) );
    fin.read( ( char* )blob->GetBufferPointer(), size );
    fin.close();
    return blob;
}

ComPtr<ID3DBlob> D3DUtilities::CompileShader( const std::wstring& fileName, const D3D_SHADER_MACRO* defines, const std::string& entry, const std::string& target )
{
    UINT compileFlags = 0;
    #if defined(DEBUG) || defined(_DEBUG)
    compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
    #endif
    HRESULT hr = S_OK;
    ComPtr<ID3DBlob> byteCode = nullptr;
    ComPtr<ID3DBlob> errors;
    hr = D3DCompileFromFile( fileName.c_str(), defines, nullptr, entry.c_str(), target.c_str(), compileFlags, 0, &byteCode, &errors );

    if ( errors != nullptr )
    {
        OutputDebugStringA( ( char* )errors->GetBufferPointer() );
    }

    ThrowIfFailed( hr );
    return byteCode;
}

XMMATRIX D3DUtilities::InvTranspose( FXMMATRIX M )
{
    XMMATRIX A = M;
    XMVECTOR det = XMMatrixDeterminant( A );
    A = XMMatrixInverse( &det, A );
    return XMMatrixTranspose( A );
}

std::wstring D3DUtilities::str_to_wstr(const std::string & str)
{
	size_t i;
	std::string curLocale = setlocale(LC_ALL, NULL);
	setlocale(LC_ALL, "chs");
	const char* _source = str.c_str();
	size_t _dsize = str.size() + 1;
	wchar_t* _dest = new wchar_t[_dsize];
	wmemset(_dest, 0x0, _dsize);
	mbstowcs_s(&i, _dest, _dsize, _source, _dsize);
	std::wstring result = _dest;
	delete[] _dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}
