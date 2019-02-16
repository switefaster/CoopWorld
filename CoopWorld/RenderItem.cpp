#include "RenderItem.h"



RenderItem::RenderItem( Mesh* mesh ) :
    mMesh( mesh )
{
}

RenderItem::RenderItem( XMFLOAT3 pos, XMFLOAT3 rot, float scale, Mesh* mesh ) :
    mPosition( pos ),
    mRotation( rot ),
    mScale( scale ),
    mMesh( mesh )
{
}

void RenderItem::SetPosition( float x, float y, float z )
{
    if ( mPosition.x != x )
    {
        mPosition.x = x;
        mDirty = true;
    }

    if ( mPosition.y != y )
    {
        mPosition.y = y;
        mDirty = true;
    }

    if ( mPosition.z != z )
    {
        mPosition.z = z;
        mDirty = true;
    }
}

void RenderItem::Offset( float x, float y, float z )
{
    SetPosition( mPosition.x + x, mPosition.y + y, mPosition.z + z );
}

void RenderItem::SetRotation( float x, float y, float z )
{
    if ( mRotation.x != x )
    {
        mRotation.x = x;
        mDirty = true;
    }

    if ( mRotation.y != y )
    {
        mRotation.y = y;
        mDirty = true;
    }

    if ( mRotation.z != z )
    {
        mRotation.z = z;
        mDirty = true;
    }
}

void RenderItem::Rotate( float x, float y, float z )
{
    SetRotation( mRotation.x + x, mRotation.y + y, mRotation.z + z );
}

void RenderItem::SetScale( float x )
{
    if ( mScale != x )
    {
        mScale = x;
        mDirty = true;
    }
}

void RenderItem::SetMesh( Mesh* mesh )
{
    mMesh = mesh;
}

XMFLOAT3 RenderItem::GetPosition() const
{
    return mPosition;
}

XMFLOAT3 RenderItem::GetRotation() const
{
    return mRotation;
}

float RenderItem::GetScale() const
{
    return mScale;
}

void RenderItem::SetTexTransform( FXMMATRIX M )
{
    XMStoreFloat4x4( &mTexTransform, M );
}

Mesh* RenderItem::GetMesh() const
{
    return mMesh;
}

XMMATRIX RenderItem::BuildWorldMatrix()
{
    if ( mDirty )
    {
        XMMATRIX P = XMMatrixTranslationFromVector( DirectX::XMLoadFloat3( &mPosition ) );
        XMMATRIX R = XMMatrixRotationX( mRotation.x ) * XMMatrixRotationY( mRotation.y ) * XMMatrixRotationZ( mRotation.z );
        XMMATRIX S = XMMatrixScalingFromVector( XMVectorReplicate( mScale ) );
        XMStoreFloat4x4( &mWorldMatrix, S * R * P );
        mDirty = false;
    }

    return XMLoadFloat4x4( &mWorldMatrix );
}

XMMATRIX RenderItem::GetTexTransform()
{
    return XMLoadFloat4x4( &mTexTransform );
}
