
elf_camera* elf_create_camera(const char *name)
{
	elf_camera *camera;

	camera = (elf_camera*)malloc(sizeof(elf_camera));
	memset(camera, 0x0, sizeof(elf_camera));
	camera->type = ELF_CAMERA;

	elf_init_actor((elf_actor*)camera, ELF_TRUE);

	camera->mode = ELF_PERSPECTIVE;
	camera->viewp_x = 0;
	camera->viewp_y = 0;
	camera->viewp_width = elf_get_window_width();
	camera->viewp_height = elf_get_window_height();
	camera->fov = 35.0;
	camera->aspect = ((float)elf_get_window_width()/(float)elf_get_window_height() >= 1.0) ?
		(float)elf_get_window_width()/(float)elf_get_window_height() :
		(float)elf_get_window_height()/(float)elf_get_window_width();
	camera->ortho_x = 0;
	camera->ortho_y = 0;
	camera->ortho_width = elf_get_window_width();
	camera->ortho_height = elf_get_window_height();
	camera->clip_far = 1.0;
	camera->clip_near = 100.0;

	gfx_matrix4_set_identity(camera->projection_matrix);
	gfx_matrix4_set_identity(camera->modelview_matrix);

	camera->dobject = elf_create_physics_object_box(0.35, 0.35, 0.35, 0.0, 0.0, 0.0, 0.0);
	elf_set_physics_object_actor(camera->dobject, (elf_actor*)camera);
	elf_inc_ref((elf_object*)camera->dobject);

	if(name) camera->name = elf_create_string(name);
	
	global_obj_count++;

	return camera;
}

elf_camera* elf_create_camera_from_pak(FILE *file, const char *name, elf_scene *scene)
{
	elf_camera *camera;
	int magic;
	float fov = 0.0;
	float clip_near = 0.0;
	float clip_far = 0.0;

	fread((char*)&magic, sizeof(int), 1, file);

	if(magic != 179532111)
	{
		elf_set_error(ELF_INVALID_FILE, "error: invalid camera \"%s//%s\", wrong magic number\n", elf_get_scene_file_path(scene), name);
		return NULL;
	}

	camera = elf_create_camera(NULL);
	elf_read_actor_header((elf_actor*)camera, file, scene);

	fread((char*)&fov, sizeof(float), 1, file);
	fread((char*)&clip_near, sizeof(float), 1, file);
	fread((char*)&clip_far, sizeof(float), 1, file);

	elf_set_camera_viewport(camera, 0, 0, elf_get_window_width(), elf_get_window_height());
	elf_set_camera_perspective(camera, fov,
		((float)elf_get_window_width()/(float)elf_get_window_height() >= 1.0) ?
		(float)elf_get_window_width()/(float)elf_get_window_height() :
		(float)elf_get_window_height()/(float)elf_get_window_width(),
		clip_near, clip_far);

	return camera;
}

void elf_update_camera(elf_camera *camera)
{
	elf_update_actor((elf_actor*)camera);
}

void elf_camera_pre_draw(elf_camera *camera)
{
	gfx_get_frustum(camera->projection_matrix, elf_get_camera_modelview_matrix(camera), camera->frustum);
	elf_get_actor_position_((elf_actor*)camera, &camera->position.x);
}

void elf_destroy_camera(elf_camera *camera)
{
	elf_clean_actor((elf_actor*)camera);

	free(camera);

	global_obj_count--;
}

void elf_set_camera_viewport(elf_camera *camera, int x, int y, int width, int height)
{
	if(width < 0) width = 0;
	if(height < 0) height = 0;
	camera->viewp_x = x;
	camera->viewp_y = y;
	camera->viewp_width = width;
	camera->viewp_height = height;
}

void elf_set_camera_perspective(elf_camera *camera, float fov, float aspect, float clip_near, float clip_far)
{
	camera->mode = ELF_PERSPECTIVE;

	camera->fov = fov;
	camera->aspect = aspect;
	camera->clip_near = clip_near;
	camera->clip_far = clip_far;

	camera->far_plane_height = 2 * (float)tan(camera->fov * GFX_PI_DIV_180 / 2) * camera->clip_far;
	camera->far_plane_width = camera->far_plane_height * camera->aspect;

	gfx_get_perspective_projection_matrix(camera->fov, camera->aspect,
		camera->clip_near, camera->clip_far, camera->projection_matrix);
}

void elf_set_camera_orthographic(elf_camera *camera, int x, int y, int width, int height, float clip_near, float clip_far)
{
	if(width < 0) width = 0;
	if(height < 0) height = 0;

	camera->mode = ELF_ORTHOGRAPHIC;

	camera->ortho_x = x;
	camera->ortho_y = y;
	camera->ortho_width = width;
	camera->ortho_height = height;
	camera->clip_near = clip_near;
	camera->clip_far = clip_far;

	camera->far_plane_height = height;
	camera->far_plane_width = width;

	gfx_get_orthographic_projection_matrix(
		(float)camera->ortho_x, (float)(camera->ortho_x+camera->ortho_width),
		(float)camera->ortho_y, (float)(camera->ortho_y+camera->ortho_height),
		(float)camera->clip_near, camera->clip_far, camera->projection_matrix);
}

float elf_get_camera_fov(elf_camera *camera)
{
	return camera->fov;
}

float elf_get_camera_aspect(elf_camera *camera)
{
	return camera->aspect;
}

elf_vec2f elf_get_camera_clip(elf_camera *camera)
{
	elf_vec2f clip;

	clip.x = camera->clip_near;
	clip.y = camera->clip_far;

	return clip;
}

elf_vec2f elf_get_camera_far_plane_size(elf_camera *camera)
{
	elf_vec2f size;

	size.x = camera->far_plane_width;
	size.y = camera->far_plane_height;

	return size;
}

float* elf_get_camera_projection_matrix(elf_camera *camera)
{
	return camera->projection_matrix;
}

float* elf_get_camera_modelview_matrix(elf_camera *camera)
{
	memcpy(camera->modelview_matrix, gfx_get_transform_matrix(camera->transform), sizeof(float)*16);
	return camera->modelview_matrix;
}

void elf_set_camera(elf_camera *camera, gfx_shader_params *shader_params)
{
	gfx_set_viewport(camera->viewp_x, camera->viewp_y, camera->viewp_width, camera->viewp_height);

	memcpy(shader_params->projection_matrix, camera->projection_matrix, sizeof(float)*16);
	memcpy(camera->modelview_matrix, gfx_get_transform_matrix(camera->transform), sizeof(float)*16);
	memcpy(shader_params->modelview_matrix, camera->modelview_matrix, sizeof(float)*16);
	memcpy(shader_params->camera_matrix, camera->modelview_matrix, sizeof(float)*16);

	shader_params->clip_start = camera->clip_near;
	shader_params->clip_end = camera->clip_far;
	shader_params->viewport_width = camera->viewp_width;
	shader_params->viewport_height = camera->viewp_height;
}

unsigned char elf_aabb_inside_frustum(elf_camera *camera, float *min, float *max)
{
	int i;

	for(i = 0; i < 6; i++)
	{
		if(camera->frustum[i][0]*min[0]+camera->frustum[i][1]*min[1]+camera->frustum[i][2]*max[2]+camera->frustum[i][3] > 0.0)
			continue;
		if(camera->frustum[i][0]*max[0]+camera->frustum[i][1]*min[1]+camera->frustum[i][2]*max[2]+camera->frustum[i][3] > 0.0)
			continue;
		if(camera->frustum[i][0]*max[0]+camera->frustum[i][1]*max[1]+camera->frustum[i][2]*max[2]+camera->frustum[i][3] > 0.0)
			continue;
		if(camera->frustum[i][0]*min[0]+camera->frustum[i][1]*max[1]+camera->frustum[i][2]*max[2]+camera->frustum[i][3] > 0.0)
			continue;
		if(camera->frustum[i][0]*min[0]+camera->frustum[i][1]*min[1]+camera->frustum[i][2]*min[2]+camera->frustum[i][3] > 0.0)
			continue;
		if(camera->frustum[i][0]*max[0]+camera->frustum[i][1]*min[1]+camera->frustum[i][2]*min[2]+camera->frustum[i][3] > 0.0)
			continue;
		if(camera->frustum[i][0]*max[0]+camera->frustum[i][1]*max[1]+camera->frustum[i][2]*min[2]+camera->frustum[i][3] > 0.0)
			continue;
		if(camera->frustum[i][0]*min[0]+camera->frustum[i][1]*max[1]+camera->frustum[i][2]*min[2]+camera->frustum[i][3] > 0.0)
			continue;
		return ELF_FALSE;
	}
	return ELF_TRUE;
}

unsigned char elf_sphere_inside_frustum(elf_camera *camera, float *pos, float radius)
{
	int i;
	for(i = 0; i < 6; i++)
	{
		if(camera->frustum[i][0]*pos[0]+camera->frustum[i][1]*pos[1]+
			camera->frustum[i][2]*pos[2]+camera->frustum[i][3] <= -radius)
			return ELF_FALSE;
	}
	return ELF_TRUE;
}

unsigned char elf_camera_inside_aabb(elf_camera *camera, float *min, float *max)
{
	return camera->position.x > min[0]-camera->clip_near && camera->position.y > min[1]-camera->clip_near && camera->position.z > min[2]-camera->clip_near &&
		camera->position.x < max[0]+camera->clip_near && camera->position.y < max[1]+camera->clip_near && camera->position.z < max[2]+camera->clip_near;
}

unsigned char elf_camera_inside_sphere(elf_camera *camera, float *pos, float radius)
{
	return camera->position.x > pos[0]-radius && camera->position.y > pos[1]-radius && camera->position.z > pos[2]-radius &&
		camera->position.x < pos[0]+radius && camera->position.y < pos[1]+radius && camera->position.z < pos[2]+radius;
}

void elf_draw_camera_debug(elf_camera *camera, gfx_shader_params *shader_params)
{
	float min[3];
	float max[3];
	float position[3];
	float rotation[3];
	gfx_transform *transform;
	float *vertex_buffer;

	transform = gfx_create_object_transform();

	gfx_get_transform_position(camera->transform, position);
	gfx_get_transform_rotation(camera->transform, rotation);
	gfx_set_transform_position(transform, position[0], position[1], position[2]);
	gfx_set_transform_rotation(transform, rotation[0], rotation[1], rotation[2]);

	gfx_mul_matrix4_matrix4(gfx_get_transform_matrix(transform),
		shader_params->camera_matrix, shader_params->modelview_matrix);

	gfx_destroy_transform(transform);

	min[0] = min[1] = min[2] = -0.35;
	max[0] = max[1] = max[2] = 0.35;

	gfx_set_color(&shader_params->material_params.color, 0.2, 0.6, 0.2, 1.0);
	shader_params->render_params.blend_mode = ELF_ADD;
	gfx_set_shader_params(shader_params);
	gfx_draw_bounding_box(min, max);

	vertex_buffer = (float*)gfx_get_vertex_data_buffer(eng->lines);

	vertex_buffer[0] = -1.5;
	vertex_buffer[1] = 0.0;
	vertex_buffer[2] = 0.0;
	vertex_buffer[3] = 1.5;
	vertex_buffer[4] = 0.0;
	vertex_buffer[5] = 0.0;
	vertex_buffer[6] = 0.0;
	vertex_buffer[7] = -1.5;
	vertex_buffer[8] = 0.0;
	vertex_buffer[9] = 0.0;
	vertex_buffer[10] = 1.5;
	vertex_buffer[11] = 0.0;
	vertex_buffer[12] = 0.0;
	vertex_buffer[13] = 0.0;
	vertex_buffer[14] = 0.5;
	vertex_buffer[15] = 0.0;
	vertex_buffer[16] = 0.0;
	vertex_buffer[17] = -3.0;

	if(!camera->selected) gfx_set_color(&shader_params->material_params.color, 0.2, 0.6, 0.2, 1.0);
	else gfx_set_color(&shader_params->material_params.color, 1.0, 0.0, 0.0, 1.0);
	gfx_set_shader_params(shader_params);
	gfx_draw_lines(6, eng->lines);
}

elf_vec3f elf_un_project_camera_point(elf_camera *camera, float x, float y, float z)
{
	elf_vec3f result;
	int viewp[4];

	viewp[0] = camera->viewp_x;
	viewp[1] = camera->viewp_y;
	viewp[2] = camera->viewp_width;
	viewp[3] = camera->viewp_height;

	gfx_un_project(x, camera->viewp_height-y, z, elf_get_camera_modelview_matrix(camera),
		elf_get_camera_projection_matrix(camera), viewp, &result.x);

	return result;
}

