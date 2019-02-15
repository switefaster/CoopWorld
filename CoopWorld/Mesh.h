#pragma once

#include "D3DUtilities.h"
#include "Texture.h"

struct Material
{
    Material()
    {
        ZeroMemory( this, sizeof( this ) );
    }
    XMFLOAT4 Ambient;
    XMFLOAT4 Diffuse;
    XMFLOAT4 Specular;
    XMFLOAT4 Reflect;
	UINT HasTexture;
	XMFLOAT3 pad;
};

class Mesh {
    public:
        template<typename T>
        Mesh( ID3D11Device* device, T vertices[], std::uint16_t indices[], UINT vertexCount, UINT indexCount, D3D11_PRIMITIVE_TOPOLOGY primitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) :
            mVertexCount( vertexCount ),
            mIndexCount( indexCount ),
            mPrimitiveTopology( primitive ),
            mStride( sizeof( T ) )
        {
            assert( device != nullptr );
            D3D11_BUFFER_DESC vbDesc;
            vbDesc.Usage = D3D11_USAGE_DEFAULT;
            vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            vbDesc.ByteWidth = mStride * vertexCount;
            vbDesc.CPUAccessFlags = 0;
            vbDesc.MiscFlags = 0;
            vbDesc.StructureByteStride = 0;
            D3D11_SUBRESOURCE_DATA vbInit;
            vbInit.pSysMem = vertices;
            vbInit.SysMemPitch = 0;
            vbInit.SysMemSlicePitch = 0;
            ThrowIfFailed( device->CreateBuffer( &vbDesc, &vbInit, &mVertexBuffer ) );
            D3D11_BUFFER_DESC ibDesc;
            ibDesc.Usage = D3D11_USAGE_DEFAULT;
            ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
            ibDesc.ByteWidth = sizeof( std::uint16_t ) * indexCount;
            ibDesc.CPUAccessFlags = 0;
            ibDesc.MiscFlags = 0;
            ibDesc.StructureByteStride = 0;
            D3D11_SUBRESOURCE_DATA ibInit;
            ibInit.pSysMem = indices;
            ibInit.SysMemPitch = 0;
            ibInit.SysMemSlicePitch = 0;
            ThrowIfFailed( device->CreateBuffer( &ibDesc, &ibInit, &mIndexBuffer ) );
        }

        void Apply( ID3D11DeviceContext* context )
        {
            UINT stride = mStride;
            UINT offset = 0;
            context->IASetVertexBuffers( 0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset );
            context->IASetIndexBuffer( mIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0 );
            context->IASetPrimitiveTopology( mPrimitiveTopology );
            context->DrawIndexed( mIndexCount, 0, 0 );
        }

		Texture* GetTexture() const
		{
			return mTexture;
		}

    protected:
        ComPtr<ID3D11Buffer> mVertexBuffer;
        ComPtr<ID3D11Buffer> mIndexBuffer;

        UINT mVertexCount;
        UINT mIndexCount;

        UINT mStride;

        D3D11_PRIMITIVE_TOPOLOGY mPrimitiveTopology;

		Texture* mTexture;
};

