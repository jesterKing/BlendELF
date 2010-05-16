
void gfx_set_viewport(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
}

void gfx_get_perspective_projection_matrix(float fov, float aspect, float near, float far, float *mat)
{
	float top, bottom, left, right;

	top = near * (float)tan(fov * 3.14159265f / 360.0f);
	bottom = -top;
	left = bottom * aspect;
	right = top * aspect;

	memset(mat, 0x0, sizeof(float)*16);
	mat[0] = near/right;
	mat[5] = near/top;
	mat[10] = (-(far+near))/(far-near);
	mat[11] = -1;
	mat[14] = (-2*far*near)/(far-near);
}

void gfx_get_orthographic_projection_matrix(float left, float right, float bottom, float top, float near, float far, float *matrix)
{
	memset(matrix, 0x0, sizeof(float)*16);
	matrix[0] = 2/(right-left);
	matrix[5] = 2/(top-bottom);
	matrix[10] = -2/(far-near);
	matrix[12] = -((right+left)/(right-left));
	matrix[13] = -((top+bottom)/(top-bottom));
	matrix[14] = -((far+near)/(far-near));
	matrix[15] = 1;
}

void gfx_project(float x, float y, float z, float modl[16], float proj[16], int viewp[4], float win_coord[3])
{
	float ftempo[8];

	ftempo[0] = modl[0] * x + modl[4] * y + modl[8] * z + modl[12];
	ftempo[1] = modl[1] * x + modl[5] * y + modl[9] * z + modl[13];
	ftempo[2] = modl[2] * x + modl[6] * y + modl[10] * z + modl[14];
	ftempo[3] = modl[3] * x + modl[7] * y + modl[11] * z + modl[15];
	ftempo[4] = proj[0] * ftempo[0] + proj[4] * ftempo[1] + proj[8] * ftempo[2] + proj[12] * ftempo[3];
	ftempo[5] = proj[1] * ftempo[0] + proj[5] * ftempo[1] + proj[9] * ftempo[2] + proj[13] * ftempo[3];
	ftempo[6] = proj[2] * ftempo[0] + proj[6] * ftempo[1] + proj[10] * ftempo[2] + proj[14] * ftempo[3];
	ftempo[7] = -ftempo[2];

	if(ftempo[7] == 0.0f) return;
	ftempo[7] = 1.0f / ftempo[7];
	ftempo[4] *= ftempo[7];
	ftempo[5] *= ftempo[7];
	ftempo[6] *= ftempo[7];

	win_coord[0] = (ftempo[4] * 0.5f + 0.5f) * viewp[2] + viewp[0];
	win_coord[1] = (ftempo[5] * 0.5f + 0.5f) * viewp[3] + viewp[1];
	win_coord[2] = (1.0f + ftempo[6]) * 0.5f;
}

void gfx_un_project(float x, float y, float z, float modl[16], float proj[16], int viewp[4], float obj_coord[3])
{
	float m[16], m2[16], in[4], out[4];

	gfx_mul_matrix4_matrix4(proj, modl, m2);
	gfx_matrix4_get_inverse(m2, m);

	in[0] = (x - (float)viewp[0]) / (float)viewp[2] * 2.0 - 1.0;
	in[1] = (y - (float)viewp[1]) / (float)viewp[3] * 2.0 - 1.0;
	in[2] = 2.0 * z - 1.0;
	in[3] = 1.0;

	gfx_mul_matrix4_vec4(m, in, out);
	if(out[3] == 0.0) return;

	out[3] = 1.0 / out[3];
	obj_coord[0] = out[0] * out[3];
	obj_coord[1] = out[1] * out[3];
	obj_coord[2] = out[2] * out[3];
}

void gfx_get_frustum(float proj[16], float modl[16], float frustum[6][4])
{
	float clip[16];
	int i;
	float length;

	gfx_mul_matrix4_matrix4(modl, proj, clip);

	// right plane
	frustum[0][0] = clip[3]-clip[0];
	frustum[0][1] = clip[7]-clip[4];
	frustum[0][2] = clip[11]-clip[8];
	frustum[0][3] = clip[15]-clip[12];
	// left plane
	frustum[1][0] = clip[3]+clip[0];
	frustum[1][1] = clip[7]+clip[4];
	frustum[1][2] = clip[11]+clip[8];
	frustum[1][3] = clip[15]+clip[12];
	// bottom plane
	frustum[2][0] = clip[3]+clip[1];
	frustum[2][1] = clip[7]+clip[5];
	frustum[2][2] = clip[11]+clip[9];
	frustum[2][3] = clip[15]+clip[13];
	// top plane
	frustum[3][0] = clip[3]-clip[1];
	frustum[3][1] = clip[7]-clip[5];
	frustum[3][2] = clip[11]-clip[9];
	frustum[3][3] = clip[15]-clip[13];
	// back plane
	frustum[4][0] = clip[3]-clip[2];
	frustum[4][1] = clip[7]-clip[6];
	frustum[4][2] = clip[11]-clip[10];
	frustum[4][3] = clip[15]-clip[14];
	// front plane
	frustum[5][0] = clip[3]+clip[2];
	frustum[5][1] = clip[7]+clip[6];
	frustum[5][2] = clip[11]+clip[10];
	frustum[5][3] = clip[15]+clip[14];
	// normalize planes
	for(i = 0; i < 6; i++)
	{
		length = (float)sqrt(frustum[i][0]*frustum[i][0]+
				frustum[i][1]*frustum[i][1]+
				frustum[i][2]*frustum[i][2]);
		frustum[i][0] /= length;
		frustum[i][1] /= length;
		frustum[i][2] /= length;
		frustum[i][3] /= length;
	}
}

void gfx_set_perspective(float fov, float aspect, float near, float far)
{
	float matrix[16];

	gfx_get_perspective_projection_matrix(fov, aspect, near, far, matrix);
}

void gfx_set_orthographic(float left, float right, float bottom, float top, float near, float far)
{
	float matrix[16];

	gfx_get_orthographic_projection_matrix(left, right, bottom, top, near, far, matrix);
}

void gfx_recalc_transform_matrix(gfx_transform *transform)
{
	float temp_matrix1[16];
	float temp_matrix2[16];
	float inv_qua[4];

	if(transform->camera_mode == GFX_FALSE)
	{
		gfx_matrix4_set_identity(temp_matrix1);

		temp_matrix1[12] = transform->position[0];
		temp_matrix1[13] = transform->position[1];
		temp_matrix1[14] = transform->position[2];

		temp_matrix1[0] = transform->scale[0];
		temp_matrix1[5] = transform->scale[1];
		temp_matrix1[10] = transform->scale[2];

		gfx_qua_get_inverse(transform->orient, inv_qua);

		gfx_qua_to_matrix4(inv_qua, temp_matrix2);

		gfx_mul_matrix4_matrix4(temp_matrix2, temp_matrix1, transform->matrix);
	}
	else
	{
		gfx_matrix4_set_identity(temp_matrix1);

		temp_matrix1[12] = -transform->position[0];
		temp_matrix1[13] = -transform->position[1];
		temp_matrix1[14] = -transform->position[2];

		temp_matrix1[0] = transform->scale[0];
		temp_matrix1[5] = transform->scale[1];
		temp_matrix1[10] = transform->scale[2];

		gfx_qua_to_matrix4(transform->orient, temp_matrix2);

		gfx_mul_matrix4_matrix4(temp_matrix1, temp_matrix2, transform->matrix);
	}
}

float* gfx_get_transform_matrix(gfx_transform *transform)
{
	if(transform->recalc_matrix == GFX_TRUE)
	{
		gfx_recalc_transform_matrix(transform);
		transform->recalc_matrix = GFX_FALSE;
	}

	return transform->matrix;
}

gfx_transform* gfx_create_camera_transform()
{
	gfx_transform *transform;

	transform = (gfx_transform*)malloc(sizeof(gfx_transform));
	memset(transform, 0x0, sizeof(gfx_transform));

	gfx_qua_set_identity(transform->orient);
	gfx_matrix4_set_identity(transform->matrix);

	transform->scale[0] = 1.0;
	transform->scale[1] = 1.0;
	transform->scale[2] = 1.0;

	transform->camera_mode = GFX_TRUE;

	return transform;
}

gfx_transform* gfx_create_object_transform()
{
	gfx_transform *transform;

	transform = (gfx_transform*)malloc(sizeof(gfx_transform));
	memset(transform, 0x0, sizeof(gfx_transform));

	gfx_qua_set_identity(transform->orient);
	gfx_matrix4_set_identity(transform->matrix);

	transform->scale[0] = 1.0;
	transform->scale[1] = 1.0;
	transform->scale[2] = 1.0;

	return transform;
}

void gfx_destroy_transform(gfx_transform *transform)
{
	free(transform);
}

void gfx_set_transform_position(gfx_transform *transform, float x, float y, float z)
{
	transform->position[0] = x;
	transform->position[1] = y;
	transform->position[2] = z;

	transform->recalc_matrix = GFX_TRUE;
}

void gfx_set_transform_rotation(gfx_transform *transform, float x, float y, float z)
{
	transform->rotation[0] = x;
	transform->rotation[1] = y;
	transform->rotation[2] = z;

	gfx_set_qua_rotation(x, y, z, transform->orient);

	transform->recalc_matrix = GFX_TRUE;
}

void gfx_set_transform_scale(gfx_transform *transform, float x, float y, float z)
{
	transform->scale[0] = x;
	transform->scale[1] = y;
	transform->scale[2] = z;

	transform->recalc_matrix = GFX_TRUE;
}

void gfx_set_transform_orientation(gfx_transform *transform, float x, float y, float z, float w)
{
	transform->orient[0] = x;
	transform->orient[1] = y;
	transform->orient[2] = z;
	transform->orient[3] = w;

	gfx_qua_to_euler(transform->orient, transform->rotation);

	transform->recalc_matrix = GFX_TRUE;
}

void gfx_rotate_transform(gfx_transform *transform, float x, float y, float z)
{
	transform->rotation[0] += x;
	transform->rotation[1] += y;
	transform->rotation[2] += z;

	gfx_rotate_qua(x, y, z, transform->orient);

	transform->recalc_matrix = GFX_TRUE;
}

void gfx_rotate_transform_local(gfx_transform *transform, float x, float y, float z)
{
	transform->rotation[0] += x;
	transform->rotation[1] += y;
	transform->rotation[2] += z;

	gfx_rotate_qua_local(x, y, z, transform->orient);

	transform->recalc_matrix = GFX_TRUE;
}

void gfx_move_transform(gfx_transform *transform, float x, float y, float z)
{
	transform->position[0] += x;
	transform->position[1] += y;
	transform->position[2] += z;

	transform->recalc_matrix = GFX_TRUE;
}

void gfx_move_transform_local(gfx_transform *transform, float x, float y, float z)
{
	float temp_vec[3];
	float vec[3];

	temp_vec[0] = x;
	temp_vec[1] = y;
	temp_vec[2] = z;

	gfx_mul_qua_vec(transform->orient, temp_vec, vec);

	transform->position[0] += vec[0];
	transform->position[1] += vec[1];
	transform->position[2] += vec[2];

	transform->recalc_matrix = GFX_TRUE;
}

unsigned char gfx_get_transform_camera_mode(gfx_transform *transform)
{
	return transform->camera_mode;
}

void gfx_get_transform_position(gfx_transform *transform, float *params)
{
	memcpy(params, transform->position, sizeof(float)*3);
}

void gfx_get_transform_rotation(gfx_transform *transform, float *params)
{
	memcpy(params, transform->rotation, sizeof(float)*3);
}

void gfx_get_transform_scale(gfx_transform *transform, float *params)
{
	memcpy(params, transform->scale, sizeof(float)*3);
}

void gfx_get_transform_orientation(gfx_transform *transform, float *params)
{
	memcpy(params, transform->orient, sizeof(float)*4);
}

