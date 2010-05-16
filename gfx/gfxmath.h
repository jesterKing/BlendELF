
void gfx_vec_normalize(float *vec)
{
	float length = 1.0/(float)sqrt(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]);
	vec[0] *= length;
	vec[1] *= length;
	vec[2] *= length;
}

void gfx_vec_dot_vec(float *vec1, float *vec2, float *dot)
{
	*dot = vec1[0]*vec2[0]+vec1[1]*vec2[1]+vec1[2]*vec2[2];
}

void gfx_vec_cross_product(float *vec1, float *vec2, float *vec3)
{
	vec3[0] = vec1[1]*vec2[2]-vec1[2]*vec2[1];
	vec3[1] = vec1[2]*vec2[0]-vec1[0]*vec2[2];
	vec3[2] = vec1[0]*vec2[1]-vec1[1]*vec2[0];
}

void gfx_qua_normalize(float *qua, float *result)
{
	float length = 1.0/sqrt(qua[0]*qua[0]+qua[1]*qua[1]+qua[2]*qua[2]+qua[3]*qua[3]);
	result[0] = qua[0]*length;
	result[1] = qua[1]*length;
	result[2] = qua[2]*length;
	result[3] = qua[3]*length;
}

void gfx_mul_qua_vec(float *qua, float *vec1, float *vec2)
{
	float uv[3];
	float uuv[3];
	float qvec[3];
	float qmul;

	qvec[0] = qua[0]; qvec[1] = qua[1]; qvec[2] = qua[2];

	gfx_vec_cross_product(qvec, vec1, uv);
	gfx_vec_cross_product(qvec, uv, uuv);

	qmul = 2.0*qua[3];
	uv[0] *= qmul;
	uv[1] *= qmul;
	uv[2] *= qmul;

	uuv[0] *= 2.0f;
	uuv[1] *= 2.0f;
	uuv[2] *= 2.0f;

	vec2[0] = vec1[0]+uv[0]+uuv[0];
	vec2[1] = vec1[1]+uv[1]+uuv[1];
	vec2[2] = vec1[2]+uv[2]+uuv[2];
}

void gfx_mul_qua_qua(float *qua1, float *qua2, float *qua3)
{
	qua3[0] =  qua1[0]*qua2[3]+qua1[1]*qua2[2]-qua1[2]*qua2[1]+qua1[3]*qua2[0];
	qua3[1] = -qua1[0]*qua2[2]+qua1[1]*qua2[3]+qua1[2]*qua2[0]+qua1[3]*qua2[1];
	qua3[2] =  qua1[0]*qua2[1]-qua1[1]*qua2[0]+qua1[2]*qua2[3]+qua1[3]*qua2[2];
	qua3[3] = -qua1[0]*qua2[0]-qua1[1]*qua2[1]-qua1[2]*qua2[2]+qua1[3]*qua2[3];
}

void gfx_qua_slerp(float *qa, float* qc, double t, float *result)
{
	float cos_half_theta;
	float half_theta;
	float sin_half_theta;
	float ratio_a;
	float ratio_b;
	float qb[4];

	memcpy(qb, qc, sizeof(float)*4);

	cos_half_theta = qa[3]*qb[3]+qa[0]*qb[0]+qa[1]*qb[1]+qa[2]*qb[2];
	if(abs(cos_half_theta) >= 1.0)
	{
		result[0] = qa[0];
		result[1] = qa[1];
		result[2] = qa[2];
		result[3] = qa[3];
		return;
	}

	if (cos_half_theta < 0.0)
	{
		qb[0] = -qb[0];
		qb[1] = -qb[1];
		qb[2] = -qb[2];
		qb[3] = -qb[3];
		cos_half_theta = -cos_half_theta;
	}

	half_theta = acos(cos_half_theta);
	sin_half_theta = sqrt(1.0-cos_half_theta*cos_half_theta);

	if(fabs(sin_half_theta) < 0.001)
	{
		result[0] = (qa[0] * 0.5 + qb[0] * 0.5);
		result[1] = (qa[1] * 0.5 + qb[1] * 0.5);
		result[2] = (qa[2] * 0.5 + qb[2] * 0.5);
		result[3] = (qa[3] * 0.5 + qb[3] * 0.5);
		return;
	}

	ratio_a = sin((1 - t) * half_theta) / sin_half_theta;
	ratio_b = sin(t * half_theta) / sin_half_theta; 

	result[0] = (qa[0] * ratio_a + qb[0] * ratio_b);
	result[1] = (qa[1] * ratio_a + qb[1] * ratio_b);
	result[2] = (qa[2] * ratio_a + qb[2] * ratio_b);
	result[3] = (qa[3] * ratio_a + qb[3] * ratio_b);
}

void gfx_mul_matrix4_vec3(float *m1, float *vec1, float *vec2)
{
	vec2[0] = m1[0]*vec1[0]+m1[1]*vec1[1]+m1[2]*vec1[2];
	vec2[1] = m1[4]*vec1[0]+m1[5]*vec1[1]+m1[6]*vec1[2];
	vec2[2] = m1[8]*vec1[0]+m1[9]*vec1[1]+m1[10]*vec1[2];
}

void gfx_mul_matrix4_vec4(float *m1, float *vec1, float *vec2)
{
	vec2[0] = m1[0]*vec1[0]+m1[1]*vec1[1]+m1[2]*vec1[2]+m1[3]*vec1[3];
	vec2[1] = m1[4]*vec1[0]+m1[5]*vec1[1]+m1[6]*vec1[2]+m1[7]*vec1[3];
	vec2[2] = m1[8]*vec1[0]+m1[9]*vec1[1]+m1[10]*vec1[2]+m1[11]*vec1[3];
	vec2[3] = m1[12]*vec1[0]+m1[13]*vec1[1]+m1[14]*vec1[2]+m1[15]*vec1[3];
}

void gfx_mul_matrix4_matrix4(float *m1, float *m2, float *m3)
{
	m3[0] = m1[0]*m2[0]+m1[1]*m2[4]+m1[2]*m2[8]+m1[3]*m2[12];
	m3[1] = m1[0]*m2[1]+m1[1]*m2[5]+m1[2]*m2[9]+m1[3]*m2[13];
	m3[2] = m1[0]*m2[2]+m1[1]*m2[6]+m1[2]*m2[10]+m1[3]*m2[14];
	m3[3] = m1[0]*m2[3]+m1[1]*m2[7]+m1[2]*m2[11]+m1[3]*m2[15];
	m3[4] = m1[4]*m2[0]+m1[5]*m2[4]+m1[6]*m2[8]+m1[7]*m2[12];
	m3[5] = m1[4]*m2[1]+m1[5]*m2[5]+m1[6]*m2[9]+m1[7]*m2[13];
	m3[6] = m1[4]*m2[2]+m1[5]*m2[6]+m1[6]*m2[10]+m1[7]*m2[14];
	m3[7] = m1[4]*m2[3]+m1[5]*m2[7]+m1[6]*m2[11]+m1[7]*m2[15];
	m3[8] = m1[8]*m2[0]+m1[9]*m2[4]+m1[10]*m2[8]+m1[11]*m2[12];
	m3[9] = m1[8]*m2[1]+m1[9]*m2[5]+m1[10]*m2[9]+m1[11]*m2[13];
	m3[10] = m1[8]*m2[2]+m1[9]*m2[6]+m1[10]*m2[10]+m1[11]*m2[14];
	m3[11] = m1[8]*m2[3]+m1[9]*m2[7]+m1[10]*m2[11]+m1[11]*m2[15];
	m3[12] = m1[12]*m2[0]+m1[13]*m2[4]+m1[14]*m2[8]+m1[15]*m2[12];
	m3[13] = m1[12]*m2[1]+m1[13]*m2[5]+m1[14]*m2[9]+m1[15]*m2[13];
	m3[14] = m1[12]*m2[2]+m1[13]*m2[6]+m1[14]*m2[10]+m1[15]*m2[14];
	m3[15] = m1[12]*m2[3]+m1[13]*m2[7]+m1[14]*m2[11]+m1[15]*m2[15];
}

void gfx_qua_from_angle_axis(float angle, float *axis, float *qua)
{
	float sin_a;
	float cos_a;

	sin_a = (float)sin(angle/2.0f*GFX_PI_DIV_180);
	cos_a = (float)cos(angle/2.0f*GFX_PI_DIV_180);

	qua[0] = axis[0]*sin_a;
	qua[1] = axis[1]*sin_a;
	qua[2] = axis[2]*sin_a;
	qua[3] = cos_a;
}

void gfx_qua_to_matrix4(float *qua, float *mat)
{
	float xx = 2*qua[0]*qua[0];
	float xy = 2*qua[0]*qua[1];
	float xz = 2*qua[0]*qua[2];
	float xw = 2*qua[0]*qua[3];
	float yy = 2*qua[1]*qua[1];
	float yz = 2*qua[1]*qua[2];
	float yw = 2*qua[1]*qua[3];
	float zz = 2*qua[2]*qua[2];
	float zw = 2*qua[2]*qua[3];
	mat[0] = 1-yy-zz; mat[1] = xy-zw; mat[2] = xz+yw;
	mat[4] = xy+zw; mat[5] = 1-xx-zz; mat[6] = yz-xw;
	mat[8] = xz-yw; mat[9] = yz+xw; mat[10] = 1-xx-yy;
	mat[12] = mat[13] = mat[14] = mat[3] = mat[7] = mat[11] = 0.0f;
	mat[15] = 1.0f;
}

void gfx_qua_to_euler(float *qua, float *euler)
{
	float sqx = qua[0]*qua[0];
	float sqy = qua[1]*qua[1];
	float sqz = qua[2]*qua[2];
	float sqw = qua[3]*qua[3];

	euler[0] = (float)atan2(2.0f*(qua[1]*qua[2]+qua[0]*qua[3]), -sqx-sqy+sqz+sqw);
	euler[1] = (float)asin(-2.0f*(qua[0]*qua[2]-qua[1]*qua[3]));
	euler[2] = (float)atan2(2.0f*(qua[0]*qua[1]+qua[2]*qua[3]), sqx-sqy-sqz+sqw);

	euler[0] *= GFX_180_DIV_PI;
	euler[1] *= GFX_180_DIV_PI;
	euler[2] *= GFX_180_DIV_PI;
}

void gfx_qua_get_inverse(float *qua, float *invqua)
{
	float length = 1.0f/(qua[0]*qua[0]+qua[1]*qua[1]+qua[2]*qua[2]+qua[3]*qua[3]);
	invqua[0] = qua[0]*(-length);
	invqua[1] = qua[1]*(-length);
	invqua[2] = qua[2]*(-length);
	invqua[3] = qua[3]*length;
}

void gfx_qua_set_identity(float *qua)
{
	qua[0] = qua[1] = qua[2] = 0.0;
	qua[3] = 1.0;
}

void gfx_matrix4_set_identity(float *mat)
{
	memset(mat, 0x0, sizeof(float)*16);
	mat[0] = mat[5] = mat[10] = mat[15] = 1.0;
}

void gfx_matrix4_transpose(float *mat1, float *mat2)
{
	mat2[0] = mat1[0];
	mat2[4] = mat1[1];
	mat2[8] = mat1[2];
	mat2[12] = mat1[3];
	mat2[1] = mat1[4];
	mat2[5] = mat1[5];
	mat2[9] = mat1[6];
	mat2[13] = mat1[7];
	mat2[2] = mat1[8];
	mat2[6] = mat1[9];
	mat2[10] = mat1[10];
	mat2[14] = mat1[11];
	mat2[3] = mat1[12];
	mat2[7] = mat1[13];
	mat2[11] = mat1[14];
	mat2[15] = mat1[15];
}

void gfx_matrix4_lol_invert(float *mat1, float *mat2)
{
	mat2[3] = mat2[7] = mat2[11] = 0.0; mat2[15] = 1.0;
	mat2[0] = mat1[0];
	mat2[1] = mat1[4];
	mat2[2] = mat1[8];
	mat2[4] = mat1[1];
	mat2[5] = mat1[5];
	mat2[6] = mat1[9];
	mat2[8] = mat1[2];
	mat2[9] = mat1[6];
	mat2[10] = mat1[10];
	mat2[12] = -(mat1[12] * mat2[0] + mat1[13] * mat2[4] + mat1[14] * mat2[8]);
	mat2[13] = -(mat1[12] * mat2[1] + mat1[13] * mat2[5] + mat1[14] * mat2[9]);
	mat2[14] = -(mat1[12] * mat2[2] + mat1[13] * mat2[6] + mat1[14] * mat2[10]);
}

#define _m11 0
#define _m12 1
#define _m13 2
#define _m14 3
#define _m21 4
#define _m22 5
#define _m23 6
#define _m24 7
#define _m31 8
#define _m32 9
#define _m33 10
#define _m34 11
#define _m41 12
#define _m42 13
#define _m43 14
#define _m44 15

float gfx_matrix4_get_determinant(float *mat1)
{
	return mat1[_m11] * mat1[_m22] * mat1[_m33] * mat1[_m44] +
		mat1[_m11] * mat1[_m23] * mat1[_m34] * mat1[_m42] +
		mat1[_m11] * mat1[_m24] * mat1[_m32] * mat1[_m43] +

		mat1[_m12] * mat1[_m21] * mat1[_m34] * mat1[_m43] +
		mat1[_m12] * mat1[_m23] * mat1[_m31] * mat1[_m44] +
		mat1[_m12] * mat1[_m24] * mat1[_m33] * mat1[_m41] +

		mat1[_m13] * mat1[_m21] * mat1[_m32] * mat1[_m44] +
		mat1[_m13] * mat1[_m22] * mat1[_m34] * mat1[_m41] +
		mat1[_m13] * mat1[_m24] * mat1[_m31] * mat1[_m42] +

		mat1[_m14] * mat1[_m21] * mat1[_m33] * mat1[_m42] +
		mat1[_m14] * mat1[_m22] * mat1[_m31] * mat1[_m43] +
		mat1[_m14] * mat1[_m23] * mat1[_m32] * mat1[_m41] -

		mat1[_m11] * mat1[_m22] * mat1[_m34] * mat1[_m43] -
		mat1[_m11] * mat1[_m23] * mat1[_m32] * mat1[_m44] -
		mat1[_m11] * mat1[_m24] * mat1[_m33] * mat1[_m42] -

		mat1[_m12] * mat1[_m21] * mat1[_m33] * mat1[_m44] -
		mat1[_m12] * mat1[_m23] * mat1[_m34] * mat1[_m41] -
		mat1[_m12] * mat1[_m24] * mat1[_m31] * mat1[_m43] -

		mat1[_m13] * mat1[_m21] * mat1[_m34] * mat1[_m42] -
		mat1[_m13] * mat1[_m22] * mat1[_m31] * mat1[_m44] -
		mat1[_m13] * mat1[_m24] * mat1[_m32] * mat1[_m41] -

		mat1[_m14] * mat1[_m21] * mat1[_m32] * mat1[_m43] -
		mat1[_m14] * mat1[_m22] * mat1[_m33] * mat1[_m41] -
		mat1[_m14] * mat1[_m23] * mat1[_m31] * mat1[_m42];
}

void gfx_matrix4_get_inverse(float *mat1, float *mat2)
{
	float det;

	det = gfx_matrix4_get_determinant(mat1);

	if(det == 0.0f) return;

	det = 1.0f/det;

	mat2[_m11] = (mat1[_m22] * mat1[_m33] * mat1[_m44] +
		mat1[_m23] * mat1[_m34] * mat1[_m42] +
		mat1[_m24] * mat1[_m32] * mat1[_m43] -
		mat1[_m22] * mat1[_m34] * mat1[_m43] -
		mat1[_m23] * mat1[_m32] * mat1[_m44] -
		mat1[_m24] * mat1[_m33] * mat1[_m42])*det;

	mat2[_m12] = (mat1[_m12] * mat1[_m34] * mat1[_m43] +
		mat1[_m13] * mat1[_m32] * mat1[_m44] +
		mat1[_m14] * mat1[_m33] * mat1[_m42] -
		mat1[_m12] * mat1[_m33] * mat1[_m44] -
		mat1[_m13] * mat1[_m34] * mat1[_m42] -
		mat1[_m14] * mat1[_m32] * mat1[_m43])*det;

	mat2[_m13] = (mat1[_m12] * mat1[_m23] * mat1[_m44] +
		mat1[_m13] * mat1[_m24] * mat1[_m42] +
		mat1[_m14] * mat1[_m22] * mat1[_m43] -
		mat1[_m12] * mat1[_m24] * mat1[_m43] -
		mat1[_m13] * mat1[_m22] * mat1[_m44] -
		mat1[_m14] * mat1[_m23] * mat1[_m42])*det;

	mat2[_m14] = (mat1[_m12] * mat1[_m24] * mat1[_m33] +
		mat1[_m13] * mat1[_m22] * mat1[_m34] +
		mat1[_m14] * mat1[_m23] * mat1[_m32] -
		mat1[_m12] * mat1[_m23] * mat1[_m34] -
		mat1[_m13] * mat1[_m24] * mat1[_m32] -
		mat1[_m14] * mat1[_m22] * mat1[_m33])*det;

	mat2[_m21] = (mat1[_m21] * mat1[_m34] * mat1[_m43] +
		mat1[_m23] * mat1[_m31] * mat1[_m44] +
		mat1[_m24] * mat1[_m33] * mat1[_m41] -
		mat1[_m21] * mat1[_m33] * mat1[_m44] -
		mat1[_m23] * mat1[_m34] * mat1[_m41] -
		mat1[_m24] * mat1[_m31] * mat1[_m43])*det;

	mat2[_m22] = (mat1[_m11] * mat1[_m33] * mat1[_m44] +
		mat1[_m13] * mat1[_m34] * mat1[_m41] +
		mat1[_m14] * mat1[_m31] * mat1[_m43] -
		mat1[_m11] * mat1[_m34] * mat1[_m43] -
		mat1[_m13] * mat1[_m31] * mat1[_m44] -
		mat1[_m14] * mat1[_m33] * mat1[_m41])*det;

	mat2[_m23] = (mat1[_m11] * mat1[_m24] * mat1[_m43] +
		mat1[_m13] * mat1[_m21] * mat1[_m44] +
		mat1[_m14] * mat1[_m23] * mat1[_m41] -
		mat1[_m11] * mat1[_m23] * mat1[_m44] -
		mat1[_m13] * mat1[_m24] * mat1[_m41] -
		mat1[_m14] * mat1[_m21] * mat1[_m43])*det;

	mat2[_m24] = (mat1[_m11] * mat1[_m23] * mat1[_m34] +
		mat1[_m13] * mat1[_m24] * mat1[_m31] +
		mat1[_m14] * mat1[_m21] * mat1[_m33] -
		mat1[_m11] * mat1[_m24] * mat1[_m33] -
		mat1[_m13] * mat1[_m21] * mat1[_m34] -
		mat1[_m14] * mat1[_m23] * mat1[_m31])*det;

	mat2[_m31] = (mat1[_m21] * mat1[_m32] * mat1[_m44] +
		mat1[_m22] * mat1[_m34] * mat1[_m41] +
		mat1[_m24] * mat1[_m31] * mat1[_m42] -
		mat1[_m21] * mat1[_m34] * mat1[_m42] -
		mat1[_m22] * mat1[_m31] * mat1[_m44] -
		mat1[_m24] * mat1[_m32] * mat1[_m41])*det;

	mat2[_m32] = (mat1[_m11] * mat1[_m34] * mat1[_m42] +
		mat1[_m12] * mat1[_m31] * mat1[_m44] +
		mat1[_m14] * mat1[_m32] * mat1[_m41] -
		mat1[_m11] * mat1[_m32] * mat1[_m44] -
		mat1[_m12] * mat1[_m34] * mat1[_m41] -
		mat1[_m14] * mat1[_m31] * mat1[_m42])*det;

	mat2[_m33] = (mat1[_m11] * mat1[_m22] * mat1[_m44] +
		mat1[_m12] * mat1[_m24] * mat1[_m41] +
		mat1[_m14] * mat1[_m21] * mat1[_m42] -
		mat1[_m11] * mat1[_m24] * mat1[_m42] -
		mat1[_m12] * mat1[_m21] * mat1[_m44] -
		mat1[_m14] * mat1[_m22] * mat1[_m41])*det;

	mat2[_m34] = (mat1[_m11] * mat1[_m24] * mat1[_m32] +
		mat1[_m12] * mat1[_m21] * mat1[_m34] +
		mat1[_m14] * mat1[_m22] * mat1[_m31] -
		mat1[_m11] * mat1[_m22] * mat1[_m34] -
		mat1[_m12] * mat1[_m24] * mat1[_m31] -
		mat1[_m14] * mat1[_m21] * mat1[_m32])*det;

	mat2[_m41] = (mat1[_m21] * mat1[_m33] * mat1[_m42] +
		mat1[_m22] * mat1[_m31] * mat1[_m43] +
		mat1[_m23] * mat1[_m32] * mat1[_m41] -
		mat1[_m21] * mat1[_m32] * mat1[_m43] -
		mat1[_m22] * mat1[_m33] * mat1[_m41] -
		mat1[_m24] * mat1[_m31] * mat1[_m42])*det;

	mat2[_m42] = (mat1[_m11] * mat1[_m32] * mat1[_m43] +
		mat1[_m12] * mat1[_m33] * mat1[_m41] +
		mat1[_m13] * mat1[_m31] * mat1[_m42] -
		mat1[_m11] * mat1[_m33] * mat1[_m42] -
		mat1[_m12] * mat1[_m31] * mat1[_m43] -
		mat1[_m13] * mat1[_m32] * mat1[_m41])*det;

	mat2[_m43] = (mat1[_m11] * mat1[_m23] * mat1[_m42] +
		mat1[_m12] * mat1[_m21] * mat1[_m43] +
		mat1[_m13] * mat1[_m22] * mat1[_m41] -
		mat1[_m11] * mat1[_m22] * mat1[_m43] -
		mat1[_m12] * mat1[_m23] * mat1[_m41] -
		mat1[_m13] * mat1[_m21] * mat1[_m42])*det;

	mat2[_m44] = (mat1[_m11] * mat1[_m22] * mat1[_m33] +
		mat1[_m12] * mat1[_m23] * mat1[_m31] +
		mat1[_m13] * mat1[_m21] * mat1[_m32] -
		mat1[_m11] * mat1[_m23] * mat1[_m32] -
		mat1[_m12] * mat1[_m21] * mat1[_m33] -
		mat1[_m13] * mat1[_m22] * mat1[_m31])*det;
}

void gfx_matrix3_to_qua(float *mat, float *qua)
{
	float S;
	float tr = mat[0] + mat[4] + mat[8];

	if (tr > 0)
	{
		S = (float)sqrt(tr+1.0) * 2.0f;
		qua[3] = 0.25f * S;
		qua[0] = (mat[7] - mat[5]) / S;
		qua[1] = (mat[2] - mat[6]) / S;
		qua[2] = (mat[3] - mat[1]) / S;
	}
	else if(mat[0] > mat[4] && mat[0] > mat[8])
	{
		S = (float)sqrt(1.0 + mat[0] - mat[4] - mat[8]) * 2.0f;
		qua[3] = (mat[7] - mat[5]) / S;
		qua[0] = 0.25f * S;
		qua[1] = (mat[1] + mat[3]) / S;
		qua[2] = (mat[2] + mat[6]) / S;
	}
	else if(mat[4] > mat[8])
	{
		S = (float)sqrt(1.0 + mat[4] - mat[0] - mat[8]) * 2.0f;
		qua[3] = (mat[2] - mat[6]) / S;
		qua[0] = (mat[1] + mat[3]) / S;
		qua[1] = 0.25f * S;
		qua[2] = (mat[5] + mat[7]) / S;
	}
	else
	{ 
		S = (float)sqrt(1.0 + mat[8] - mat[0] - mat[4]) * 2.0f;
		qua[3] = (mat[3] - mat[1]) / S;
		qua[0] = (mat[2] + mat[6]) / S;
		qua[1] = (mat[5] + mat[7]) / S;
		qua[2] = 0.25f * S;
	}
}

void gfx_matrix4_to_euler(float *mat, float *eul)
{
	if(mat[4] > 0.998)
	{
		eul[0] = atan2(mat[2],mat[10])*GFX_180_DIV_PI;
		eul[1] = GFX_PI/2*GFX_180_DIV_PI;
		eul[2] = 0;
		return;
	}
	if(mat[4] < -0.998)
	{
		eul[0] = atan2(mat[2],mat[10])*GFX_180_DIV_PI;
		eul[1] = -GFX_PI/2*GFX_180_DIV_PI;
		eul[2] = 0;
		return;
	}
	eul[0] = atan2(-mat[8],mat[0])*GFX_180_DIV_PI;
	eul[1] = atan2(-mat[6],mat[5])*GFX_180_DIV_PI;
	eul[2] = asin(mat[4])*GFX_180_DIV_PI;
}

void gfx_set_qua_rotation(float x, float y, float z, float *qua)
{
	float axis_angle_qua[4];
	float temp_qua[4];
	float axis[3];

	gfx_qua_set_identity(qua);
	memset(axis, 0x0, sizeof(float)*3);

	if(x != 0.0)
	{
		axis[0] = 1.0; axis[1] = axis[2] = 0.0;
		gfx_qua_from_angle_axis(x, axis, axis_angle_qua);
		gfx_mul_qua_qua(axis_angle_qua, qua, temp_qua);
		memcpy(qua, temp_qua, sizeof(float)*4);
	}
	if(y != 0.0)
	{
		axis[1] = 1.0; axis[0] = axis[2] = 0.0;
		gfx_qua_from_angle_axis(y, axis, axis_angle_qua);
		gfx_mul_qua_qua(axis_angle_qua, qua, temp_qua);
		memcpy(qua, temp_qua, sizeof(float)*4);
	}
	if(z != 0.0)
	{
		axis[2] = 1.0; axis[0] = axis[1] = 0.0;
		gfx_qua_from_angle_axis(z, axis, axis_angle_qua);
		gfx_mul_qua_qua(axis_angle_qua, qua, temp_qua);
		memcpy(qua, temp_qua, sizeof(float)*4);
	}
}

void gfx_rotate_qua(float x, float y, float z, float *qua)
{
	float axis_angle_qua[4];
	float temp_qua0[4];
	float temp_qua1[4];
	float axis[3];

	gfx_qua_set_identity(temp_qua0);
	memset(axis, 0x0, sizeof(float)*3);

	if(x != 0.0)
	{
		axis[0] = 1.0; axis[1] = axis[2] = 0.0;
		gfx_qua_from_angle_axis(x, axis, axis_angle_qua);
		gfx_mul_qua_qua(axis_angle_qua, temp_qua0, temp_qua1);
		memcpy(temp_qua0, temp_qua1, sizeof(float)*4);
	}
	if(y != 0.0)
	{
		axis[1] = 1.0; axis[0] = axis[2] = 0.0;
		gfx_qua_from_angle_axis(y, axis, axis_angle_qua);
		gfx_mul_qua_qua(axis_angle_qua, temp_qua0, temp_qua1);
		memcpy(temp_qua0, temp_qua1, sizeof(float)*4);
	}
	if(z != 0.0)
	{
		axis[2] = 1.0; axis[0] = axis[1] = 0.0;
		gfx_qua_from_angle_axis(z, axis, axis_angle_qua);
		gfx_mul_qua_qua(axis_angle_qua, temp_qua0, temp_qua1);
		memcpy(temp_qua0, temp_qua1, sizeof(float)*4);
	}

	memcpy(temp_qua1, qua, sizeof(float)*4);
	gfx_mul_qua_qua(temp_qua0, temp_qua1, qua);
}

void gfx_rotate_qua_local(float x, float y, float z, float *qua)
{
	float axis_angle_qua[4];
	float temp_qua0[4];
	float temp_qua1[4];
	float temp_axis[3];
	float axis[3];

	gfx_qua_set_identity(temp_qua0);
	memset(axis, 0x0, sizeof(float)*3);

	if(x != 0.0)
	{
		temp_axis[0] = 1.0; temp_axis[1] = temp_axis[2] = 0.0;
		gfx_mul_qua_vec(qua, temp_axis, axis);
		gfx_qua_from_angle_axis(x, axis, axis_angle_qua);
		gfx_mul_qua_qua(axis_angle_qua, temp_qua0, temp_qua1);
		memcpy(temp_qua0, temp_qua1, sizeof(float)*4);
	}
	if(y != 0.0)
	{
		temp_axis[1] = 1.0; temp_axis[0] = temp_axis[2] = 0.0;
		gfx_mul_qua_vec(qua, temp_axis, axis);
		gfx_qua_from_angle_axis(y, axis, axis_angle_qua);
		gfx_mul_qua_qua(axis_angle_qua, temp_qua0, temp_qua1);
		memcpy(temp_qua0, temp_qua1, sizeof(float)*4);
	}
	if(z != 0.0)
	{
		temp_axis[2] = 1.0; temp_axis[0] = temp_axis[1] = 0.0;
		gfx_mul_qua_vec(qua, temp_axis, axis);
		gfx_qua_from_angle_axis(z, axis, axis_angle_qua);
		gfx_mul_qua_qua(axis_angle_qua, temp_qua0, temp_qua1);
		memcpy(temp_qua0, temp_qua1, sizeof(float)*4);
	}

	memcpy(temp_qua1, qua, sizeof(float)*4);
	gfx_mul_qua_qua(temp_qua0, temp_qua1, qua);
}

float gfx_vec_length(float *vec)
{
	return (float)sqrt(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]);
}

