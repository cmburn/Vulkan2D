struct VSInput
{
    float3 position;
    float2 uv;
};

struct VSOutput
{
    float4 position;
    float2 uv;
};

[shader("vertex")]
VSOutput VertexShader(VSInput input)
{
    VSOutput out;
    out.position = float4(0.0, 0.0, 0.0, 0.0);
    out.uv = float2(0.0, 0.0);
    return out;
}

[shader("pixel")]
float4 PixelShader(VSOutput input)
{
    return float4(0.0, 0.0, 0.0, 0.0);
}