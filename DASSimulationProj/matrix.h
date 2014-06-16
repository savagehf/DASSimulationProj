#ifndef _matrix_H_
#define _matrix_H_

#ifdef __cplusplus
extern "C" {
#endif

//+============================================================================
//| 矩阵计算系列函数
//| 对4*4的矩阵进行处理
//| 也有一部分是对4*1的向量进行处理
//+============================================================================

typedef float vec4[4];

// 根据xyz坐标创建一个向量
void vec4_fromXYZ(vec4 v, float x, float y, float z);
void vec4_fromXYZW(vec4 v, float x, float y, float z, float w);
// 复制向量
void vec4_copy(vec4 to, vec4 from);



typedef float mat4[16];

// 设置为单位矩阵
void mat4_identity(mat4 m);
// 复制矩阵
void mat4_copy(mat4 to, mat4 from);
// 矩阵乘法
void mat4_multiply(mat4 m1, mat4 m2, mat4 mResult);
// 矩阵变换，实际上是4*4的矩阵乘以4*1的矩阵（也就是向量），结果为向量
void mat4_transform3(mat4 m, vec4 v, vec4 result);

void mat4_transform2(mat4 m, vec4 v);

// 矩阵乘上一个表示移动的矩阵
void mat4_translate(mat4 m, float x, float y, float z);
// 矩阵乘上一个表示绕x轴旋转的矩阵
void mat4_rotateX(mat4 m, float theta);
// 矩阵乘上一个表示绕y轴旋转的矩阵
void mat4_rotateY(mat4 m, float theta);
// 矩阵乘上一个表示绕z轴旋转的矩阵
void mat4_rotateZ(mat4 m, float theta);
// 求转置矩阵
void mat4_transpose(mat4 m, mat4 result);
// 求逆矩阵
void mat4_inverse(mat4 m, mat4 result);

#ifdef __cplusplus
}
#endif

#endif
