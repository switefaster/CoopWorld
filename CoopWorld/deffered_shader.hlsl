Texture2D gDiffuseMap : register(t0);
SamplerState gLinearSampler : register(s0);

struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Reflect;
};

cbuffer cbChangesPerObject : register(b0)
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
	float4x4 gWorldViewProj;
	float4x4 gTexTransform;
	Material gMaterial;
};

struct VertexIn
{
	float3 PosL : POSITION;
	float3 NormalL : NORMAL;
	float2 Tex : TEXCOORD;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float3 PosW : POSITION;
	float3 NormalW : NORMAL;
	float2 Tex : TEXCOORD0;
};

struct PixelOut
{
	float4 Color : SV_TARGET0;
	float4 Normal : SV_TARGET1;
	float4 Position : SV_TARGET2;
	float4 Ambient : SV_TARGET3;
	float4 Diffuse : SV_TARGET4;
	float4 Specular : SV_TARGET5;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout = (VertexOut)0;
	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;
	return vout;
}

PixelOut PS(VertexOut pin) : SV_TARGET
{
	PixelOut pout;
	pout.Color = gDiffuseMap.Sample(gLinearSampler, pin.Tex);
	pout.Normal = float4(pin.NormalW, 1.0f);
	pout.Position = float4(pin.PosW, 1.0f);
	pout.Ambient = gMaterial.Ambient;
	pout.Diffuse = gMaterial.Diffuse;
	pout.Specular = gMaterial.Specular;
	return pout;
}
