struct FragmentShaderArguments {
    float2 fragPosition;
    Texture2D<float4> textures[];
    uint textureIndex;
    SamplerState sampler;
    float4 colourModifier;
    StructuredBuffer<UserData> userBuffer
};

struct VertexShaderArguments {
    StructuredBuffer<UserData> userBuffer
    float4x4 camera;
    float2 inPosition;
};
