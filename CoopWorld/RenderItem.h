#pragma once

#include "D3DUtilities.h"
#include "Mesh.h"

class RenderItem {
    public:
        RenderItem();
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
        Mesh* GetMesh() const;

        XMMATRIX BuildWorldMatrix();

    private:
        XMFLOAT3 mPosition;
        XMFLOAT3 mRotation;
        float mScale;
        XMMATRIX mWorldMatrix = XMMatrixIdentity();

        Mesh* mMesh;

        bool mDirty = false;
};

