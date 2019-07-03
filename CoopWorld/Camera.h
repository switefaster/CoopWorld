#pragma once

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "D3DUtilities.h"

class Camera {
    public:
        Camera();
        ~Camera() = default;

        XMVECTOR GetPositionXM() const;
        XMFLOAT3 GetPosition() const;
        void SetPosition( float x, float y, float z );
        void SetPosition( const XMFLOAT3& pos );

        XMVECTOR GetRightXM() const;
        XMFLOAT3 GetRight() const;
        XMVECTOR GetUpXM() const;
        XMFLOAT3 GetUp() const;
        XMVECTOR GetLookXM() const;
        XMFLOAT3 GetLook() const;

        float GetNearZ() const;
        float GetFarZ() const;
        float GetAspect() const;
        float GetFovY() const;
        float GetFovX() const;

        float GetNearWindowWidth() const;
        float GetFarWindowWidth() const;
        float GetNearWindowHeight() const;
        float GetFarWindowHeight() const;

        void SetLens( float fovY, float aspect, float nz, float fz );

        void LookAt( FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp );
        void LookAt( const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up );

        XMMATRIX View() const;
        XMMATRIX Proj() const;
        XMMATRIX ViewProj() const;

        void Strafe( float d );
        void Walk( float d );

        void Pitch( float angle );
        void RotateY( float angle );

        void UpdateViewMatrix();

    private:
        XMFLOAT3 mPosition;
        XMFLOAT3 mRight;
        XMFLOAT3 mUp;
        XMFLOAT3 mLook;

        float mNearZ;
        float mFarZ;
        float mAspect;
        float mFovY;
        float mNearWindowHeight;
        float mFarWindowHeight;

        XMFLOAT4X4 mView;
        XMFLOAT4X4 mProj;
};

#endif
