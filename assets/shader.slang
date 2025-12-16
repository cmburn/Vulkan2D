struct VSInput
{
    float3 position : POSITION;
    float2 uv       : TEXCOORD0;
};

struct VSOutput
{
    float4 position : SV_Position;
    float2 uv       : TEXCOORD0;
};

cbuffer Camera : register(b0)
{
    float4x4 viewProj;
};

VSOutput VertexMain(VSInput input)
{
    VSOutput out;
    out.position = mul(viewProj, float4(input.position, 1.0));
    out.uv = input.uv;
    return out;
}

Texture2D    gTexture : register(t0);
SamplerState gSampler : register(s0);

float4 FragmentMain(VSOutput input) : SV_Target
{
    return gTexture.Sample(gSampler, input.uv);
}
