#include <metal_stdlib>
using namespace metal;
#import "ShaderTypes.h"

struct VertexOut {
    float4 position [[position]];
    float4 color;
    float2 uv;
};

vertex VertexOut
rocket_vertex(uint vertexID [[ vertex_id ]],
              constant RocketVertex *vertexArray [[ buffer(0) ]],
              constant matrix_float4x4 &transform [[ buffer(1) ]])
{    
    VertexOut out;
    out.position = transform * vertexArray[vertexID].position;
    out.uv = vertexArray[vertexID].texcoord;
    out.color = vertexArray[vertexID].color;
    return out;
}

fragment float4 rocket_color(VertexOut in [[stage_in]])
{
    return float4(1.0,0.5,0.0,1.0);
}

fragment float4 rocket_texture(VertexOut in [[stage_in]],
                               texture2d<half> colorTexture [[ texture(0) ]],
                               sampler textureSampler [[sampler(0)]])
{
    const half4 colorSample = colorTexture.sample(textureSampler, in.uv);

    return float4(colorSample);
}

