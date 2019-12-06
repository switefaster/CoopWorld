struct VertexIn
{
    float3 PosL : POSITION;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
};

cbuffer cbMatrices : register(b0)
{
    float4x4 gWorldViewProj;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout = (VertexOut)0;
    vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
    return vout;
}