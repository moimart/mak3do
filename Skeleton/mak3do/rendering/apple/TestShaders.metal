#include <metal_stdlib>
using namespace metal;
#import "ShaderTypes.h"

typedef struct
{
    float4 position [[position]];
    float4 color;

} RasterizerData;

struct NodeBuffer {
  float4x4 modelTransform;
  float4x4 modelViewProjectionTransform;
  float4x4 modelViewTransform;
  float4x4 normalTransform;
  float2x3 boundingBox;
};

struct VertexInput {
  float3 position  [[attribute(SCNVertexSemanticPosition)]];
  float2 uv [[attribute(SCNVertexSemanticTexcoord0)]];
};

struct VertexOut {
  float4 position [[position]];
  float2 uv;
};

vertex VertexOut textureSamplerVertex(VertexInput in [[ stage_in ]],
                                      constant NodeBuffer& scn_node [[buffer(1)]]) {
  VertexOut out;
  out.position = scn_node.modelViewProjectionTransform * float4(in.position, 1.0);
  out.uv = in.uv;
  return out;
}

fragment float4 fragmentShader(VertexOut in [[stage_in]])
{
    return float4(1.0,0.5,0.0,1.0);
}

