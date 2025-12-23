// Per-drawcall values
struct Push {
    // Not needed in pixel shader
    int cameraIndex;

    // Index of the texture to draw
    uint textureIndex;

    // Where in texture to draw (x, y, w, h)
    float4 texturePos;

    // Current colour modifier
    float4 colour;

    // Not needed in pixel shader
    float4x4 model;
};
[[vk::push_constant]] Push push;

// Texture and sampler
[[vk::binding(1,1)]] SamplerState sampler;
[[vk::binding(2,2)]] Texture2D textures[];

// Optional user data
struct UserData {
    float time; // whatever you want
};
[[vk::binding(3,3)]] ConstantBuffer<UserData> userData;

[shader("pixel")]
float4 PixelShader(float2 uv) {
    float4 uv_colour = textures[NonUniformResourceIndex(push.textureIndex)].Sample(sampler, uv);
    return push.colour * uv_colour;
}
