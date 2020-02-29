#ifndef RocketTypes_h
#define RocketTypes_h

#include <simd/simd.h>

typedef enum RocketVertextInput
{
    RocketVertexInputVertices     = 0,
} RocketVertexInputIndex;

typedef struct
{
    vector_float2 position;
    vector_short4 color;
    vector_float2 texcoord;
} RocketVertex;

#include <SceneKit/scn_metal>

#endif /* RocketTypes_h */
