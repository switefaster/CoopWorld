#pragma once

#ifndef _RENDER_ITEM_H_
#define _RENDER_ITEM_H_

#include "D3DUtilities.h"
#include "Mesh.h"

class RenderItem {
    public:
        RenderItem( Mesh* mesh );
        RenderItem( XMFLOAT3 pos, XMFLOAT3 rot, float scale, Mesh* mesh );
        ~RenderItem() = default;

        void SetPosition( float x, float y, float z );
        void Offset( float x, float y, float z );
        void SetRotation( float x, float y, float z );
        void Rotate( float x, float y, float z );
        void SetScale( float x );
        void SetMesh( Mesh* mesh );

        XMFLOAT3 GetPosition() const;
        XMFLOAT3 GetRotation() const;
        float GetScale() const;
        void SetTexTransform( FXMMATRIX M );
        Mesh* GetMesh() const;

        XMMATRIX BuildWorldMatrix();
        XMMATRIX GetTexTransform();

    private:
        XMFLOAT3 mPosition;
        XMFLOAT3 mRotation;
        float mScale = 1.0f;
        XMFLOAT4X4 mWorldMatrix = Identity4x4;
        XMFLOAT4X4 mTexTransform = Identity4x4;

        Mesh* mMesh;

        bool mDirty = false;
};

#endif
