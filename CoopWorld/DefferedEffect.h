#pragma once

#ifndef _DEFFERED_EFFECT_H_
#define _DEFFERED_EFFECT_H_

#include "BasicEffect.h"
class DefferedEffect :
	public Effect
{
public:
	DefferedEffect(ID3D11Device* device);

	void SetWorld(FXMMATRIX M);
	void SetWorldInvTranspose(FXMMATRIX M);
	void SetWorldViewProj(FXMMATRIX M);
	void SetTexTransform(FXMMATRIX M);

protected:
	void UpdateConstantBuffers(ID3D11DeviceContext* context) override;

private:
	BasicEffect::CBObject mCBObject;
};

#endif
