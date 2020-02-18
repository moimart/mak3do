/**
 Copyright 2013 BlackBerry Inc.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

 Original file from GamePlay3D: http://gameplay3d.org

 This file was modified to fit the cocos2d-x project
 */

NS_CC_MATH_BEGIN

#if defined (__arm64__)
#define NEON64
#endif


inline void MathUtil::addMatrix(const float* m, float scalar, float* dst)
{
    dst[0]  = m[0]  + scalar;
    dst[1]  = m[1]  + scalar;
    dst[2]  = m[2]  + scalar;
    dst[3]  = m[3]  + scalar;
    dst[4]  = m[4]  + scalar;
    dst[5]  = m[5]  + scalar;
    dst[6]  = m[6]  + scalar;
    dst[7]  = m[7]  + scalar;
    dst[8]  = m[8]  + scalar;
    dst[9]  = m[9]  + scalar;
    dst[10] = m[10] + scalar;
    dst[11] = m[11] + scalar;
    dst[12] = m[12] + scalar;
    dst[13] = m[13] + scalar;
    dst[14] = m[14] + scalar;
    dst[15] = m[15] + scalar;
}

inline void MathUtil::addMatrix(const float* m1, const float* m2, float* dst)
{
#if defined (NEON64)
    asm volatile(
     "ld4     {v0.4s, v1.4s, v2.4s, v3.4s},     [%1]     \n\t" // M1[m0-m7] M1[m8-m15]
     "ld4     {v8.4s, v9.4s, v10.4s, v11.4s},   [%2]     \n\t" // M2[m0-m7] M2[m8-m15]

     "fadd   v12.4s, v0.4s, v8.4s          \n\t" // DST->M[m0-m3] = M1[m0-m3] + M2[m0-m3]
     "fadd   v13.4s, v1.4s, v9.4s          \n\t" // DST->M[m4-m7] = M1[m4-m7] + M2[m4-m7]
     "fadd   v14.4s, v2.4s, v10.4s         \n\t" // DST->M[m8-m11] = M1[m8-m11] + M2[m8-m11]
     "fadd   v15.4s, v3.4s, v11.4s         \n\t" // DST->M[m12-m15] = M1[m12-m15] + M2[m12-m15]

     "st4    {v12.4s, v13.4s, v14.4s, v15.4s}, [%0]    \n\t" // DST->M[m0-m7] DST->M[m8-m15]
     :
     : "r"(dst), "r"(m1), "r"(m2)
     : "v0", "v1", "v2", "v3", "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15", "memory"
     );
#else
    dst[0]  = m1[0]  + m2[0];
    dst[1]  = m1[1]  + m2[1];
    dst[2]  = m1[2]  + m2[2];
    dst[3]  = m1[3]  + m2[3];
    dst[4]  = m1[4]  + m2[4];
    dst[5]  = m1[5]  + m2[5];
    dst[6]  = m1[6]  + m2[6];
    dst[7]  = m1[7]  + m2[7];
    dst[8]  = m1[8]  + m2[8];
    dst[9]  = m1[9]  + m2[9];
    dst[10] = m1[10] + m2[10];
    dst[11] = m1[11] + m2[11];
    dst[12] = m1[12] + m2[12];
    dst[13] = m1[13] + m2[13];
    dst[14] = m1[14] + m2[14];
    dst[15] = m1[15] + m2[15];
#endif
}

inline void MathUtil::subtractMatrix(const float* m1, const float* m2, float* dst)
{
    dst[0]  = m1[0]  - m2[0];
    dst[1]  = m1[1]  - m2[1];
    dst[2]  = m1[2]  - m2[2];
    dst[3]  = m1[3]  - m2[3];
    dst[4]  = m1[4]  - m2[4];
    dst[5]  = m1[5]  - m2[5];
    dst[6]  = m1[6]  - m2[6];
    dst[7]  = m1[7]  - m2[7];
    dst[8]  = m1[8]  - m2[8];
    dst[9]  = m1[9]  - m2[9];
    dst[10] = m1[10] - m2[10];
    dst[11] = m1[11] - m2[11];
    dst[12] = m1[12] - m2[12];
    dst[13] = m1[13] - m2[13];
    dst[14] = m1[14] - m2[14];
    dst[15] = m1[15] - m2[15];
}

inline void MathUtil::multiplyMatrix(const float* m, float scalar, float* dst)
{
#if defined (NEON64)
    asm volatile(
    "ld1     {v0.s}[0],         [%2]            \n\t" //s
    "ld4     {v4.4s, v5.4s, v6.4s, v7.4s}, [%1]       \n\t" //M[m0-m7] M[m8-m15]

    "fmul     v8.4s, v4.4s, v0.s[0]               \n\t" // DST->M[m0-m3] = M[m0-m3] * s
    "fmul     v9.4s, v5.4s, v0.s[0]               \n\t" // DST->M[m4-m7] = M[m4-m7] * s
    "fmul     v10.4s, v6.4s, v0.s[0]              \n\t" // DST->M[m8-m11] = M[m8-m11] * s
    "fmul     v11.4s, v7.4s, v0.s[0]              \n\t" // DST->M[m12-m15] = M[m12-m15] * s

    "st4     {v8.4s, v9.4s, v10.4s, v11.4s},           [%0]     \n\t" // DST->M[m0-m7] DST->M[m8-m15]
    :
    : "r"(dst), "r"(m), "r"(&scalar)
    : "v0", "v4", "v5", "v6", "v7", "v8", "v9", "v10", "v11", "memory"
    );
#else
    dst[0]  = m[0]  * scalar;
    dst[1]  = m[1]  * scalar;
    dst[2]  = m[2]  * scalar;
    dst[3]  = m[3]  * scalar;
    dst[4]  = m[4]  * scalar;
    dst[5]  = m[5]  * scalar;
    dst[6]  = m[6]  * scalar;
    dst[7]  = m[7]  * scalar;
    dst[8]  = m[8]  * scalar;
    dst[9]  = m[9]  * scalar;
    dst[10] = m[10] * scalar;
    dst[11] = m[11] * scalar;
    dst[12] = m[12] * scalar;
    dst[13] = m[13] * scalar;
    dst[14] = m[14] * scalar;
    dst[15] = m[15] * scalar;
#endif
}
#if defined (NEON64)

static inline void NEON_Matrix4Mul(const float* a, const float* b, float* output )
{
    asm volatile
    (
     "ld1     {v8.4s, v9.4s, v10.4s, v11.4s}, [%1] \n\t"       // M1[m0-m7] M1[m8-m15] M2[m0-m7]  M2[m8-m15]
     "ld4     {v0.4s, v1.4s, v2.4s, v3.4s},  [%2]   \n\t"       // M2[m0-m15]

     "fmul    v12.4s, v8.4s, v0.s[0]     \n\t"         // DST->M[m0-m3] = M1[m0-m3] * M2[m0]
     "fmul    v13.4s, v8.4s, v0.s[1]     \n\t"         // DST->M[m4-m7] = M1[m4-m7] * M2[m4]
     "fmul    v14.4s, v8.4s, v0.s[2]     \n\t"         // DST->M[m8-m11] = M1[m8-m11] * M2[m8]
     "fmul    v15.4s, v8.4s, v0.s[3]     \n\t"         // DST->M[m12-m15] = M1[m12-m15] * M2[m12]

     "fmla    v12.4s, v9.4s, v1.s[0]     \n\t"         // DST->M[m0-m3] += M1[m0-m3] * M2[m1]
     "fmla    v13.4s, v9.4s, v1.s[1]     \n\t"         // DST->M[m4-m7] += M1[m4-m7] * M2[m5]
     "fmla    v14.4s, v9.4s, v1.s[2]     \n\t"         // DST->M[m8-m11] += M1[m8-m11] * M2[m9]
     "fmla    v15.4s, v9.4s, v1.s[3]     \n\t"         // DST->M[m12-m15] += M1[m12-m15] * M2[m13]

     "fmla    v12.4s, v10.4s, v2.s[0]    \n\t"         // DST->M[m0-m3] += M1[m0-m3] * M2[m2]
     "fmla    v13.4s, v10.4s, v2.s[1]    \n\t"         // DST->M[m4-m7] += M1[m4-m7] * M2[m6]
     "fmla    v14.4s, v10.4s, v2.s[2]    \n\t"         // DST->M[m8-m11] += M1[m8-m11] * M2[m10]
     "fmla    v15.4s, v10.4s, v2.s[3]    \n\t"         // DST->M[m12-m15] += M1[m12-m15] * M2[m14]

     "fmla    v12.4s, v11.4s, v3.s[0]    \n\t"         // DST->M[m0-m3] += M1[m0-m3] * M2[m3]
     "fmla    v13.4s, v11.4s, v3.s[1]    \n\t"         // DST->M[m4-m7] += M1[m4-m7] * M2[m7]
     "fmla    v14.4s, v11.4s, v3.s[2]    \n\t"         // DST->M[m8-m11] += M1[m8-m11] * M2[m11]
     "fmla    v15.4s, v11.4s, v3.s[3]    \n\t"         // DST->M[m12-m15] += M1[m12-m15] * M2[m15]

     "st1    {v12.4s, v13.4s, v14.4s, v15.4s}, [%0]  \n\t"       // DST->M[m0-m7]// DST->M[m8-m15]

     : // output
     : "r"(output), "r"(b), "r"(a) // input - note *value* of pointer doesn't change.
     : "memory", "v0", "v1", "v2", "v3", "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15"
     );
}

#endif

inline void MathUtil::multiplyMatrix(const float* m1, const float* m2, float* dst)
{
#if defined (NEON64)

    float product[16];

    // Invert column-order with row-order
    NEON_Matrix4Mul( &m2[0], &m1[0], &product[0] );

#else
    // Support the case where m1 or m2 is the same array as dst.
    float product[16];

    product[0]  = m1[0] * m2[0]  + m1[4] * m2[1] + m1[8]   * m2[2]  + m1[12] * m2[3];
    product[1]  = m1[1] * m2[0]  + m1[5] * m2[1] + m1[9]   * m2[2]  + m1[13] * m2[3];
    product[2]  = m1[2] * m2[0]  + m1[6] * m2[1] + m1[10]  * m2[2]  + m1[14] * m2[3];
    product[3]  = m1[3] * m2[0]  + m1[7] * m2[1] + m1[11]  * m2[2]  + m1[15] * m2[3];

    product[4]  = m1[0] * m2[4]  + m1[4] * m2[5] + m1[8]   * m2[6]  + m1[12] * m2[7];
    product[5]  = m1[1] * m2[4]  + m1[5] * m2[5] + m1[9]   * m2[6]  + m1[13] * m2[7];
    product[6]  = m1[2] * m2[4]  + m1[6] * m2[5] + m1[10]  * m2[6]  + m1[14] * m2[7];
    product[7]  = m1[3] * m2[4]  + m1[7] * m2[5] + m1[11]  * m2[6]  + m1[15] * m2[7];

    product[8]  = m1[0] * m2[8]  + m1[4] * m2[9] + m1[8]   * m2[10] + m1[12] * m2[11];
    product[9]  = m1[1] * m2[8]  + m1[5] * m2[9] + m1[9]   * m2[10] + m1[13] * m2[11];
    product[10] = m1[2] * m2[8]  + m1[6] * m2[9] + m1[10]  * m2[10] + m1[14] * m2[11];
    product[11] = m1[3] * m2[8]  + m1[7] * m2[9] + m1[11]  * m2[10] + m1[15] * m2[11];

    product[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8]  * m2[14] + m1[12] * m2[15];
    product[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9]  * m2[14] + m1[13] * m2[15];
    product[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
    product[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];
#endif
    memcpy(dst, product, MATRIX_SIZE);
}

inline void MathUtil::negateMatrix(const float* m, float* dst)
{
    dst[0]  = -m[0];
    dst[1]  = -m[1];
    dst[2]  = -m[2];
    dst[3]  = -m[3];
    dst[4]  = -m[4];
    dst[5]  = -m[5];
    dst[6]  = -m[6];
    dst[7]  = -m[7];
    dst[8]  = -m[8];
    dst[9]  = -m[9];
    dst[10] = -m[10];
    dst[11] = -m[11];
    dst[12] = -m[12];
    dst[13] = -m[13];
    dst[14] = -m[14];
    dst[15] = -m[15];
}

inline void MathUtil::transposeMatrix(const float* m, float* dst)
{
    float t[16] = {
        m[0], m[4], m[8], m[12],
        m[1], m[5], m[9], m[13],
        m[2], m[6], m[10], m[14],
        m[3], m[7], m[11], m[15]
    };
    memcpy(dst, t, MATRIX_SIZE);
}

inline void MathUtil::transformVec4(const float* m, float x, float y, float z, float w, float* dst)
{
#if defined (NEON64)
    asm volatile(
    "ld1    {v0.s}[0],        [%1]    \n\t"    // V[x]
    "ld1    {v0.s}[1],        [%2]    \n\t"    // V[y]
    "ld1    {v0.s}[2],        [%3]    \n\t"    // V[z]
    "ld1    {v0.s}[3],        [%4]    \n\t"    // V[w]
    "ld1    {v9.4s, v10.4s, v11.4s, v12.4s}, [%5]   \n\t"    // M[m0-m7] M[m8-m15]


    "fmul v13.4s, v9.4s, v0.s[0]           \n\t"      // DST->V = M[m0-m3] * V[x]
    "fmla v13.4s, v10.4s, v0.s[1]           \n\t"    // DST->V += M[m4-m7] * V[y]
    "fmla v13.4s, v11.4s, v0.s[2]           \n\t"    // DST->V += M[m8-m11] * V[z]
    "fmla v13.4s, v12.4s, v0.s[3]           \n\t"    // DST->V += M[m12-m15] * V[w]

    //"st1 {v13.4s}, [%0]               \n\t"    // DST->V[x, y] // DST->V[z]
    "st1 {v13.2s}, [%0], 8               \n\t"
    "st1 {v13.s}[2], [%0]                \n\t"
    :
    : "r"(dst), "r"(&x), "r"(&y), "r"(&z), "r"(&w), "r"(m)
    : "v0", "v9", "v10","v11", "v12", "v13", "memory"
    );
#else
    dst[0] = x * m[0] + y * m[4] + z * m[8] + w * m[12];
    dst[1] = x * m[1] + y * m[5] + z * m[9] + w * m[13];
    dst[2] = x * m[2] + y * m[6] + z * m[10] + w * m[14];
#endif
}

inline void MathUtil::transformVec4(const float* m, const float* v, float* dst)
{
#if defined (NEON64)
    asm volatile
    (
     "ld1    {v0.4s}, [%1]     \n\t"   // V[x, y, z, w]
     "ld1    {v9.4s, v10.4s, v11.4s, v12.4s}, [%2] \n\t"   // M[m0-m7] M[m8-m15]

     "fmul   v13.4s, v9.4s, v0.s[0]     \n\t"   // DST->V = M[m0-m3] * V[x]
     "fmla   v13.4s, v10.4s, v0.s[1]    \n\t"   // DST->V = M[m4-m7] * V[y]
     "fmla   v13.4s, v11.4s, v0.s[2]    \n\t"   // DST->V = M[m8-m11] * V[z]
     "fmla   v13.4s, v12.4s, v0.s[3]    \n\t"   // DST->V = M[m12-m15] * V[w]

     "st1    {v13.4s}, [%0]       \n\t"   // DST->V
     :
     : "r"(dst), "r"(v), "r"(m)
     : "v0", "v9", "v10","v11", "v12", "v13", "memory"
     );
#else
    // Handle case where v == dst.
    float x = v[0] * m[0] + v[1] * m[4] + v[2] * m[8] + v[3] * m[12];
    float y = v[0] * m[1] + v[1] * m[5] + v[2] * m[9] + v[3] * m[13];
    float z = v[0] * m[2] + v[1] * m[6] + v[2] * m[10] + v[3] * m[14];
    float w = v[0] * m[3] + v[1] * m[7] + v[2] * m[11] + v[3] * m[15];

    dst[0] = x;
    dst[1] = y;
    dst[2] = z;
    dst[3] = w;
#endif
}

inline void MathUtil::crossVec3(const float* v1, const float* v2, float* dst)
{
    float x = (v1[1] * v2[2]) - (v1[2] * v2[1]);
    float y = (v1[2] * v2[0]) - (v1[0] * v2[2]);
    float z = (v1[0] * v2[1]) - (v1[1] * v2[0]);

    dst[0] = x;
    dst[1] = y;
    dst[2] = z;
}

NS_CC_MATH_END
