struct DirectionalLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Direction;
    float pad;
};

struct PointLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;

    float3 Position;
    float Range;

    float3 Att;
    float pad;
};

struct SpotLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;

    float3 Position;
    float Range;

    float3 Direction;
    float Spot;

    float3 Att;
    float pad;
};

struct Material
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Reflect;
};

void ComputeDirectionalLight(Material mat, DirectionalLight L,
                             float3 normal, float3 toEye,
					         out float4 ambient,
						     out float4 diffuse,
						     out float4 spec)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float3 lightVec = -L.Direction;
    
    ambient = mat.Ambient * L.Ambient;
	
    float diffuseFactor = dot(lightVec, normal);
    
	[flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
					
        diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
        spec = specFactor * mat.Specular * L.Specular;
    }
}

void ComputePointLight(Material mat, PointLight L, float3 pos, float3 normal, float3 toEye,
				   out float4 ambient, out float4 diffuse, out float4 spec)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float3 lightVec = L.Position - pos;
		
    float d = length(lightVec);
	
    if (d > L.Range)
        return;
		
    lightVec /= d;
	
    ambient = mat.Ambient * L.Ambient;

    float diffuseFactor = dot(lightVec, normal);
    
	[flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
					
        diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
        spec = specFactor * mat.Specular * L.Specular;
    }
    
    float att = 1.0f / dot(L.Att, float3(1.0f, d, d * d));

    diffuse *= att;
    spec *= att;
}

void ComputeSpotLight(Material mat, SpotLight L, float3 pos, float3 normal, float3 toEye,
				  out float4 ambient, out float4 diffuse, out float4 spec)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float3 lightVec = L.Position - pos;
		
    float d = length(lightVec);
	
    if (d > L.Range)
        return;
		
    lightVec /= d;
	
    ambient = mat.Ambient * L.Ambient;

    float diffuseFactor = dot(lightVec, normal);
    
	[flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
					
        diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
        spec = specFactor * mat.Specular * L.Specular;
    }
	
    float spot = pow(max(dot(-lightVec, L.Direction), 0.0f), L.Spot);
    
    float att = spot / dot(L.Att, float3(1.0f, d, d * d));

    ambient *= spot;
    diffuse *= att;
    spec *= att;
}

static const float SMAP_SIZE = 2048.0f;
static const float SMAP_DX = 1.0f / SMAP_SIZE;
float CalcShadowFactor(SamplerComparisonState comSampler, Texture2D smap, float4 shadH)
{
    shadH.xyz /= shadH.w;
    float depth = shadH.z;
    const float dx = SMAP_DX;
    float percentLit = 0.0f;
    const float2 offsets[9] =
    {
        float2(-dx, -dx), float2(0.0f, -dx), float2(dx, -dx),
        float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
        float2(-dx, dx), float2(0.0f, dx), float2(dx, dx)
    };
    [unroll]
    for (int i = 0; i < 9; i++)
    {
        percentLit += smap.SampleCmpLevelZero(comSampler, shadH.xy + offsets[i], depth).r;
    }
    return percentLit /= 9.0f;
}

cbuffer cbChangesPerObject : register(b0)
{
    float4x4 gWorld;
    float4x4 gWorldInvTranspose;
    float4x4 gWorldViewProj;
    float4x4 gTexTransform;
    Material gMaterial;
}

cbuffer cbChangesPerFrame : register(b1)
{
    DirectionalLight gDirLights[3];
    float4x4 gShadowTransform;
    float3 gEyePosW;
    
    float gFogStart;
    float gFogRange;
    float3 gFogColor;
    float3 gLightCount;
    float gPad0;
}

SamplerState samLinear : register(s0);

SamplerComparisonState samShadow : register(s1);

Texture2D gDiffuseMap : register(t0);
Texture2D gShadowMap : register(t1);

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
    float4 ShadowPosH : TEXCOORD1;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
    vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
    vout.NormalW = mul(vin.NormalL, (float3x3) gWorldInvTranspose);
    vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
    vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;
    vout.ShadowPosH = mul(float4(vout.PosW, 1.0f), gShadowTransform);
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    pin.NormalW = normalize(pin.NormalW);
    float3 toEye = gEyePosW - pin.PosW;
    float distToEye = length(toEye);
    toEye /= distToEye;
    
    float4 texColor = gMaterial.Ambient;
    texColor.a = 1.0f;
    if (gMaterial.Ambient.w != 0)
    {
        texColor = gDiffuseMap.Sample(samLinear, pin.Tex);
    }

    float4 litColor = texColor;
    if (gLightCount.x > 0)
    {
        float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
        float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
        float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

        float3 shadow = float3(1.0f, 1.0f, 1.0f);
        shadow[0] = CalcShadowFactor(samShadow, gShadowMap, pin.ShadowPosH);
        [unroll]
        for (int i = 0; i < gLightCount.x; i++)
        {
            float4 A, D, S;
            ComputeDirectionalLight(gMaterial, gDirLights[i], pin.NormalW, toEye, A, D, S);
            ambient += A;
            diffuse += D * shadow[i];
            specular += S * shadow[i];
        }
        litColor = texColor * (ambient + diffuse) + specular;
    }
    litColor.a = texColor.a * gMaterial.Diffuse.a;
    return litColor;
}