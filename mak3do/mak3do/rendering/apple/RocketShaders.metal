#include <metal_stdlib>
using namespace metal;
#import "ShaderTypes.h"

struct VertexOut {
  float4 position [[position]];
  float2 uv;
  short4 color;
};

typedef struct  {
    float2 position;
    short4 color;
    float2 uv;
} _RocketVertextInput;

vertex VertexOut
rocket_vertex(uint vertexID [[ vertex_id ]],
              constant _RocketVertextInput *vertexArray [[ buffer(0) ]]) {
    VertexOut out;
    out.position.xy = vertexArray[vertexID].position;
    out.uv = vertexArray[vertexID].uv;
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

    // We return the color of the texture
    return float4(float3(colorSample.xyz),float(colorSample.w));
}

