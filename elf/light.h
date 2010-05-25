
elf_light* elf_create_light(const char *name)
{
	elf_light *light;

	light = (elf_light*)malloc(sizeof(elf_light));
	memset(light, 0x0, sizeof(elf_light));
	light->type = ELF_LIGHT;

	elf_init_actor((elf_actor*)light, ELF_FALSE);

	light->distance = 30.0;
	light->fade_speed = 1.0/30.0;
	light->inner_cone = 45.0;
	light->outer_cone = 0.0;
	light->light_type = ELF_POINT_LIGHT;
	light->visible = ELF_TRUE;
	light->shaft = ELF_FALSE;
	light->shaft_size = 1.0;
	light->shaft_intensity = 1.0;
	light->shaft_fade_off = 0.0;

	elf_set_light_color(light, 1.0, 1.0, 1.0, 1.0);

	light->shadow_camera = elf_create_camera("elf_shadow_camera");
	elf_set_camera_viewport(light->shadow_camera, 0, 0, 512, 512);
	elf_set_camera_perspective(light->shadow_camera, (light->inner_cone+light->outer_cone)*2, 1.0, 1.0, light->distance+(1.0/light->fade_speed));

	gfx_matrix4_set_identity(light->projection_matrix);

	light->dobject = elf_create_physics_object_box(0.25, 0.25, 0.25, 0.0, 0.0, 0.0, 0.0);
	elf_set_physics_object_actor(light->dobject, (elf_actor*)light);
	elf_inc_ref((elf_object*)light->dobject);

	light->pbb_lengths.x = light->pbb_lengths.y = light->pbb_lengths.z = 0.5;

	if(name) light->name = elf_create_string(name);

	global_obj_count++;

	return light;
}

elf_light* elf_create_light_from_pak(FILE *file, const char *name, elf_scene *scene)
{
	elf_light *light;
	int magic = 0;
	unsigned int junk;

	fread((char*)&magic, sizeof(int), 1, file);

	if(magic != 179532113)
	{
		elf_set_error(ELF_INVALID_FILE, "error: invalid light \"%s//%s\", wrong magic number\n", elf_get_scene_file_path(scene), name);
		return NULL;
	}

	light = elf_create_light(NULL);
	elf_read_actor_header((elf_actor*)light, file, scene);

	fread((char*)&light->light_type, sizeof(unsigned char), 1, file);
	if(light->light_type == ELF_SPOT_LIGHT) elf_set_light_shadow_caster(light, ELF_TRUE);
	fread((char*)&light->color.r, sizeof(float), 4, file);
	fread((char*)&light->distance, sizeof(float), 1, file);
	fread((char*)&light->fade_speed, sizeof(float), 1, file);
	fread((char*)&light->inner_cone, sizeof(float), 1, file);
	fread((char*)&light->outer_cone, sizeof(float), 1, file);
	fread((char*)&junk, sizeof(unsigned int), 1, file);
	fread((char*)&light->shadow_caster, sizeof(unsigned char), 1, file);

	elf_set_light_type(light, light->light_type);
	elf_set_light_color(light, light->color.r, light->color.g, light->color.b, light->color.a);
	elf_set_light_distance(light, light->distance);
	elf_set_light_fade_speed(light, light->fade_speed);
	elf_set_light_shadow_caster(light, light->shadow_caster);
	elf_set_light_cone(light, light->inner_cone, light->outer_cone);

	return light;
}

void elf_update_light(elf_light *light)
{
	elf_update_actor((elf_actor*)light);
}

void elf_light_pre_draw(elf_light *light)
{
	elf_camera_pre_draw(light->shadow_camera);
}

void elf_destroy_light(elf_light *light)
{
	elf_clean_actor((elf_actor*)light);

	elf_destroy_camera(light->shadow_camera);

	free(light);

	global_obj_count--;
}

void elf_set_light_type(elf_light *light, int type)
{
	if(type != ELF_POINT_LIGHT &&
		type != ELF_SPOT_LIGHT &&
		type != ELF_SUN_LIGHT) return;
	light->light_type = type;
}

void elf_set_light_color(elf_light *light, float r, float g, float b, float a)
{
	light->color.r = r;
	light->color.g = g;
	light->color.b = b;
	light->color.a = a;
}

void elf_set_light_distance(elf_light *light, float distance)
{
	light->distance = distance;
	if(light->distance < 0.0) light->distance = 0.0;
	elf_set_camera_perspective(light->shadow_camera, (light->inner_cone+light->outer_cone)*2, 1.0, 1.0, light->distance+(1.0/(light->fade_speed)));
}

void elf_set_light_fade_speed(elf_light *light, float fade_speed)
{
	light->fade_speed = fade_speed;
	if(light->fade_speed < 0.0001) light->fade_speed = 0.0001;
	if(light->fade_speed > 1.0) light->fade_speed = 1.0;
	elf_set_camera_perspective(light->shadow_camera, (light->inner_cone+light->outer_cone)*2, 1.0, 1.0, light->distance+(1.0/(light->fade_speed)));
}

void elf_set_light_shadow_caster(elf_light *light, unsigned char shadow_caster)
{
	if(light->shadow_caster == shadow_caster) return;

	light->shadow_caster = shadow_caster;

	light->moved = ELF_TRUE;
}

int elf_get_light_type(elf_light *light)
{
	return light->light_type;
}

elf_color elf_get_light_color(elf_light *light)
{
	return light->color;
}

float elf_get_light_distance(elf_light *light)
{
	return light->distance;
}

float elf_get_light_fade_speed(elf_light *light)
{
	return light->fade_speed;
}

unsigned char elf_get_light_shadow_caster(elf_light *light)
{
	return light->shadow_caster;
}

unsigned char elf_get_light_visible(elf_light *light)
{
	return light->visible;
}

unsigned char elf_is_light_shaft(elf_light *light)
{
	return light->shaft;
}

float elf_get_light_shaft_size(elf_light *light)
{
	return light->shaft_size;
}

float elf_get_light_shaft_intensity(elf_light *light)
{
	return light->shaft_intensity;
}

float elf_get_light_shaft_fade_off(elf_light *light)
{
	return light->shaft_fade_off;
}

elf_vec2f elf_get_light_cone(elf_light *light)
{
	elf_vec2f cone;

	cone.x = light->inner_cone;
	cone.y = light->outer_cone;

	return cone;
}

void elf_set_light_visible(elf_light *light, unsigned char visible)
{
	light->visible = !(visible == ELF_FALSE);
}

void elf_set_light_cone(elf_light *light, float inner_cone, float outer_cone)
{
	light->inner_cone = inner_cone;
	light->outer_cone = outer_cone;
	if(light->inner_cone < 0.0) light->inner_cone = 0.0;
	if(light->outer_cone < 0.0) light->outer_cone = 0.0;
	elf_set_camera_perspective(light->shadow_camera, (light->inner_cone+light->outer_cone)*2, 1.0, 1.0, light->distance+(1.0/(light->fade_speed)));
}

void elf_set_light_shaft(elf_light *light, float size, float intensity, float fade_off)
{
	light->shaft = ELF_TRUE;
	light->shaft_size = size;
	light->shaft_intensity = intensity;
	if(light->shaft_intensity < 0.0) light->shaft_intensity = 0.0;
	light->shaft_fade_off = fade_off;
	if(light->shaft_fade_off < 0.0) light->shaft_fade_off = 0.0;
	if(light->shaft_fade_off > 1.0) light->shaft_fade_off = 1.0;
}

void elf_disable_light_shaft(elf_light *light)
{
	light->shaft = ELF_FALSE;
}

void elf_set_light(elf_light *light, elf_camera *camera, gfx_shader_params *shader_params)
{
	float axis[3];
	float final_axis[3] = {0.0, 0.0, -1.0};
	float matrix[16];
	float matrix2[16];
	float orient[4];
	float final_pos[4];

	gfx_mul_matrix4_matrix4(gfx_get_transform_matrix(light->transform), elf_get_camera_modelview_matrix(camera), matrix);
	final_pos[0] = matrix[12];
	final_pos[1] = matrix[13];
	final_pos[2] = matrix[14];

	gfx_get_transform_orientation(light->transform, orient);
	gfx_mul_qua_vec(orient, final_axis, axis);
	gfx_matrix4_get_inverse(elf_get_camera_modelview_matrix(camera), matrix2);
	gfx_mul_matrix4_vec3(matrix2, axis, final_axis);

	shader_params->light_params.type = light->light_type;
	memcpy(&shader_params->light_params.position.x, final_pos, sizeof(float)*3);
	memcpy(&shader_params->light_params.direction.x, final_axis, sizeof(float)*3);
	memcpy(&shader_params->light_params.color.r, &light->color.r, sizeof(float)*4);
	shader_params->light_params.distance = light->distance;
	shader_params->light_params.fade_speed = light->fade_speed;
	shader_params->light_params.inner_cone = light->inner_cone;
	shader_params->light_params.outer_cone = light->outer_cone;

	/*if(light->shadow_caster)
	{
		shader_params->texture_params[GFX_MAX_TEXTURES-1].type = GFX_SHADOW_MAP;
		shader_params->texture_params[GFX_MAX_TEXTURES-1].texture = light->shadow_map;
		shader_params->texture_params[GFX_MAX_TEXTURES-1].projection_mode = GFX_SHADOW_PROJECTION;
		memcpy(shader_params->texture_params[GFX_MAX_TEXTURES-1].matrix,
			light->projection_matrix, sizeof(float)*16);
	}*/
}

unsigned char elf_get_light_changed(elf_light *light)
{
	return light->moved;
}

void elf_draw_light_debug(elf_light *light, gfx_shader_params *shader_params)
{
	float min[3];
	float max[3];
	float *vertex_buffer;

	gfx_mul_matrix4_matrix4(gfx_get_transform_matrix(light->transform),
		shader_params->camera_matrix, shader_params->modelview_matrix);

	min[0] = min[1] = min[2] = -0.25;
	max[0] = max[1] = max[2] = 0.25;

	gfx_set_color(&shader_params->material_params.color, 0.5, 0.5, 0.2, 1.0);
	gfx_set_shader_params(shader_params);
	gfx_draw_bounding_box(min, max);

	vertex_buffer = (float*)gfx_get_vertex_data_buffer(eng->lines);

	vertex_buffer[0] = 0.0;
	vertex_buffer[1] = 0.0;
	vertex_buffer[2] = 0.0;
	vertex_buffer[3] = 0.0;
	vertex_buffer[4] = 0.0;
	vertex_buffer[5] = -light->distance;
	vertex_buffer[6] = -1.5;
	vertex_buffer[7] = 0.0;
	vertex_buffer[8] = 0.0;
	vertex_buffer[9] = 1.5;
	vertex_buffer[10] = 0.0;
	vertex_buffer[11] = 0.0;
	vertex_buffer[12] = 0.0;
	vertex_buffer[13] = -1.5;
	vertex_buffer[14] = 0.0;
	vertex_buffer[15] = 0.0;
	vertex_buffer[16] = 1.5;
	vertex_buffer[17] = 0.0;

	if(!light->selected) gfx_set_color(&shader_params->material_params.color, 0.5, 0.5, 0.2, 1.0);
	else gfx_set_color(&shader_params->material_params.color, 1.0, 0.0, 0.0, 1.0);
	gfx_set_shader_params(shader_params);
	gfx_draw_lines(6, eng->lines);
}

