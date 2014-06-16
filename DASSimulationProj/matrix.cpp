#include "stdafx.h"
#include "matrix.h"

#include <math.h>

void vec4_fromXYZ(vec4 v, float x, float y, float z) {
    v[0] = x;
    v[1] = y;
    v[2] = z;
    v[3] = 1.0f;
}

void vec4_fromXYZW(vec4 v, float x, float y, float z, float w) {
    v[0] = x;
    v[1] = y;
    v[2] = z;
    v[3] = w;
}

void vec4_copy(vec4 to, vec4 from) {
    to[0] = from[0];
    to[1] = from[1];
    to[2] = from[2];
    to[3] = from[3];
}

void mat4_identity(mat4 m) {
    int i;
    for(i=0; i<16; ++i)
        m[i] = 0.0f;
    m[0] = m[5] = m[10] = m[15] = 1.0f;
}

void mat4_copy(mat4 to, mat4 from) {
    int i;
    for(i=0; i<16; ++i)
        to[i] = from[i];
}

void mat4_multiply(mat4 m1, mat4 m2, mat4 mResult) {
    int i, j, k;
    for(i=0; i<4; ++i) {
        for(j=0; j<4; ++j) {
            float tmp = 0.0f;
            for(k=0; k<4; ++k)
                tmp += m1[i*4+k] * m2[k*4+j];
            mResult[i*4+j] = tmp;
        }
    }
}

void mat4_transform3(mat4 m, vec4 v, vec4 result) {
    int i, k;
    for(i=0; i<4; ++i) {
        float tmp = 0.0f;
        for(k=0; k<4; ++k)
            tmp += m[i*4+k] * v[k];
        result[i] = tmp;
    }
}

void mat4_transform2(mat4 m, vec4 v) {
	  vec4 result;
    int i, k;
    for(i=0; i<4; ++i) {
        float tmp = 0.0f;
        for(k=0; k<4; ++k)
            tmp += m[i*4+k] * v[k];
        result[i] = tmp;
    }
    for(i=0;i<4;i++){
    	v[i] = result[i];
    }    
}

void mat4_translate(mat4 m, float x, float y, float z) {
    mat4 transform = {
        1, 0, 0, x,
        0, 1, 0, y,
        0, 0, 1, z,
        0, 0, 0, 1
    };
    mat4 tmp;
    mat4_multiply(m, transform, tmp);
    mat4_copy(m, tmp);
}

void mat4_rotateX(mat4 m, float theta) {
    mat4 transform = {
        1,            0,            0, 0,
        0,  cosf(theta), -sinf(theta), 0,
        0,  sinf(theta),  cosf(theta), 0,
        0,            0,            0, 1
    };
    mat4 tmp;
    mat4_multiply(m, transform, tmp);
    mat4_copy(m, tmp);
}

void mat4_rotateY(mat4 m, float theta) {
    mat4 transform = {
         cosf(theta), 0,  sinf(theta), 0,
                  0,  1,            0, 0,
        -sinf(theta), 0,  cosf(theta), 0,
                  0,  0,            0, 1
    };
    mat4 tmp;
    mat4_multiply(m, transform, tmp);
    mat4_copy(m, tmp);
}

void mat4_rotateZ(mat4 m, float theta) {
    mat4 transform = {
         cosf(theta), -sinf(theta), 0, 0,
         sinf(theta),  cosf(theta), 0, 0,
                   0,            0, 1, 0,
                   0,            0, 0, 1
    };
    mat4 tmp;
    mat4_multiply(m, transform, tmp);
    mat4_copy(m, tmp);
}

void mat4_transpose(mat4 m, mat4 result) {
		//mat4 tmp;
		//mat4_copy(m,tmp);
		int i, j;
    for(i=0; i<4; ++i)
        for(j=0; j<4; ++j)
            result[i*4+j] = m[j*4+i];
		/*
    int i, j;
    for(i=0; i<4; ++i)
        for(j=0; j<4; ++j)
            result[i*4+j] = m[j*4+i];
    */
}

static float det3(float* m,
                         int a1, int a2, int a3,
                         int a4, int a5, int a6,
                         int a7, int a8, int a9) {
    return m[a1] * m[a5] * m[a9]
         + m[a2] * m[a6] * m[a7]
         + m[a3] * m[a4] * m[a8]
         - m[a3] * m[a5] * m[a7]
         - m[a2] * m[a4] * m[a9]
         - m[a1] * m[a6] * m[a8];
}

void mat4_inverse(mat4 m, mat4 result) {
    float d =
        +m[ 0] * det3(m,  5,  9, 13,  6, 10, 14,  7, 11, 15)
        -m[ 4] * det3(m,  1,  9, 13,  2, 10, 14,  3, 11, 15)
        +m[ 8] * det3(m,  1,  5, 13,  2,  6, 14,  3,  7, 15)
        -m[12] * det3(m,  1,  5,  9,  2,  6, 10,  3,  7, 11);

    d = 1.0f / d;

    result[ 0] =  d * det3(m,  5,  9, 13,  6, 10, 14,  7, 11, 15);
    result[ 1] = -d * det3(m,  1,  9, 13,  2, 10, 14,  3, 11, 15);
    result[ 2] =  d * det3(m,  1,  5, 13,  2,  6, 14,  3,  7, 15);
    result[ 3] = -d * det3(m,  1,  5,  9,  2,  6, 10,  3,  7, 11);
    result[ 4] = -d * det3(m,  4,  8, 12,  6, 10, 14,  7, 11, 15);
    result[ 5] =  d * det3(m,  0,  8, 12,  2, 10, 14,  3, 11, 15);
    result[ 6] = -d * det3(m,  0,  4, 12,  2,  6, 14,  3,  7, 15);
    result[ 7] =  d * det3(m,  0,  4,  8,  2,  6, 10,  3,  7, 11);
    result[ 8] =  d * det3(m,  4,  8, 12,  5,  9, 13,  7, 11, 15);
    result[ 9] = -d * det3(m,  0,  8, 12,  1,  9, 13,  3, 11, 15);
    result[10] =  d * det3(m,  0,  4, 12,  1,  5, 13,  3,  7, 15);
    result[11] = -d * det3(m,  0,  4,  8,  1,  5,  9,  3,  7, 11);
    result[12] = -d * det3(m,  4,  8, 12,  5,  9, 13,  6, 10, 14);
    result[13] =  d * det3(m,  0,  8, 12,  1,  9, 13,  2, 10, 14);
    result[14] = -d * det3(m,  0,  4, 12,  1,  5, 13,  2,  6, 14);
    result[15] =  d * det3(m,  0,  4,  8,  1,  5,  9,  2,  6, 10);
}