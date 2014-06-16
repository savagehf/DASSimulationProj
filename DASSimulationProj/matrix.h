#ifndef _matrix_H_
#define _matrix_H_

#ifdef __cplusplus
extern "C" {
#endif

//+============================================================================
//| �������ϵ�к���
//| ��4*4�ľ�����д���
//| Ҳ��һ�����Ƕ�4*1���������д���
//+============================================================================

typedef float vec4[4];

// ����xyz���괴��һ������
void vec4_fromXYZ(vec4 v, float x, float y, float z);
void vec4_fromXYZW(vec4 v, float x, float y, float z, float w);
// ��������
void vec4_copy(vec4 to, vec4 from);



typedef float mat4[16];

// ����Ϊ��λ����
void mat4_identity(mat4 m);
// ���ƾ���
void mat4_copy(mat4 to, mat4 from);
// ����˷�
void mat4_multiply(mat4 m1, mat4 m2, mat4 mResult);
// ����任��ʵ������4*4�ľ������4*1�ľ���Ҳ���������������Ϊ����
void mat4_transform3(mat4 m, vec4 v, vec4 result);

void mat4_transform2(mat4 m, vec4 v);

// �������һ����ʾ�ƶ��ľ���
void mat4_translate(mat4 m, float x, float y, float z);
// �������һ����ʾ��x����ת�ľ���
void mat4_rotateX(mat4 m, float theta);
// �������һ����ʾ��y����ת�ľ���
void mat4_rotateY(mat4 m, float theta);
// �������һ����ʾ��z����ת�ľ���
void mat4_rotateZ(mat4 m, float theta);
// ��ת�þ���
void mat4_transpose(mat4 m, mat4 result);
// �������
void mat4_inverse(mat4 m, mat4 result);

#ifdef __cplusplus
}
#endif

#endif
