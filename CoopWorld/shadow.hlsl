cbuffer cbPerFrame : register(b0)
{
    float4x4 gLightWVP;
    float4x4 gTexTransform;
    uint gHasTexture;
    float3 gPad0;
};

Texture2D gDiffuseMap : register(t0);

SamplerState samLinear : register(s0);

struct VertexIn
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float2 Tex : TEXCOORD;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float2 Tex : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
    vout.PosH = mul(float4(vin.PosL, 1.0f), gLightWVP);
    vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;
    return vout;
}

void PS(VertexOut pin)
{
    if(gHasTexture)
    {
        float4 diffuse = gDiffuseMap.Sample(samLinear, pin.Tex);
        clip(diffuse.a - 0.15f);
    }
}