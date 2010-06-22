
elf_sprite* elf_create_sprite(const char *name)
{
	elf_sprite *sprite;

	sprite = (elf_sprite*)malloc(sizeof(elf_sprite));
	memset(sprite, 0x0, sizeof(elf_sprite));
	sprite->type = ELF_SPRITE;

	elf_init_actor((elf_actor*)sprite, ELF_FALSE);

	sprite->scale.x = sprite->scale.y = 1.0;
	sprite->tex_size.x = sprite->tex_size.y = 0.6;
	sprite->real_scale.x = sprite->real_scale.y = 0.6; sprite->real_scale.z = 1.0;
	sprite->query = gfx_create_query();
	sprite->visible = ELF_TRUE;
	sprite->culled = ELF_TRUE;

	sprite->dobject = elf_create_physics_object_box(0.3, 0.3, 0.01, 0.0, 0.0, 0.0, 0.0);
	elf_set_physics_object_actor(sprite->dobject, (elf_actor*)sprite);
	elf_inc_ref((elf_object*)sprite->dobject);

	sprite->pbb_lengths.x = sprite->pbb_lengths.y = 0.6; sprite->pbb_lengths.z = 0.01;

	sprite->frame_player = elf_create_frame_player();
	elf_inc_ref((elf_object*)sprite->frame_player);

	if(name) sprite->name = elf_create_string(name);

	sprite->id = ++gen->sprite_id_counter;

	elf_inc_obj_count();

	return sprite;
}

void elf_update_sprite(elf_sprite *sprite)
{
	elf_update_actor((elf_actor*)sprite);
}

void elf_sprite_pre_draw(elf_sprite *sprite, elf_camera *camera)
{
	elf_vec4f orient;

	elf_actor_pre_draw((elf_actor*)sprite);

	elf_get_actor_position_((elf_actor*)sprite, &sprite->position.x);

	if(sprite->face_camera && camera)
	{
		elf_get_actor_orientation_((elf_actor*)camera, &orient.x);
		elf_set_actor_orientation((elf_actor*)sprite, orient.x, orient.y, orient.z, orient.w);
	}
}

void elf_sprite_post_draw(elf_sprite *sprite)
{
	elf_actor_post_draw((elf_actor*)sprite);
}

void elf_destroy_sprite(elf_sprite *sprite)
{
	elf_clean_actor((elf_actor*)sprite);

	if(sprite->query) gfx_destroy_query(sprite->query);

	if(sprite->material) elf_dec_ref((elf_object*)sprite->material);
	elf_dec_ref((elf_object*)sprite->frame_player);

	free(sprite);

	elf_dec_obj_count();
}

void elf_calc_sprite_bounds(elf_sprite *sprite)
{
	int i;

	if(sprite->material)
	{
		for(i = 0; i < GFX_MAX_TEXTURES; i++)
		{
			if(sprite->material->textures[i] && sprite->material->textures[i]->texture)
			{
				sprite->tex_size.x = (float)gfx_get_texture_width(sprite->material->textures[i]->texture)/100.0;
				sprite->tex_size.y = (float)gfx_get_texture_height(sprite->material->textures[i]->texture)/100.0;
				break;
			}
		}
	}
	else
	{
		sprite->scale.x = sprite->scale.y = 1.0;
		sprite->tex_size.x = sprite->tex_size.y = 0.6;
		sprite->real_scale.x = sprite->real_scale.y = 0.6; sprite->real_scale.z = 1.0;
		sprite->pbb_lengths.x = sprite->pbb_lengths.y = 0.6; sprite->pbb_lengths.z = 0.01;
	}

	sprite->pbb_lengths.x = sprite->tex_size.x;
	sprite->pbb_lengths.y = sprite->tex_size.y;
	sprite->pbb_lengths.z = 0.01;
	sprite->real_scale.x = sprite->scale.x*sprite->tex_size.x;
	sprite->real_scale.y = sprite->scale.y*sprite->tex_size.y;
	sprite->real_scale.z = 0.0;

	sprite->cull_radius = elf_get_vec3f_length(sprite->real_scale)/2.0;

	sprite->real_scale.z = 1.0;
}

void elf_reset_sprite_debug_physics_object(elf_sprite *sprite)
{
	float position[3];
	float orient[4];
	float scale[3];

	if(sprite->dobject)
	{
		elf_set_physics_object_actor(sprite->dobject, NULL);
		elf_set_physics_object_world(sprite->dobject, NULL);
		elf_dec_ref((elf_object*)sprite->dobject);
	}

	sprite->dobject = elf_create_physics_object_box(sprite->tex_size.x/2.0,
		sprite->tex_size.y/2.0, 0.01/2.0, 0.0, 0.0, 0.0, 0.0);

	elf_set_physics_object_actor(sprite->dobject, (elf_actor*)sprite);
	elf_inc_ref((elf_object*)sprite->dobject);

	gfx_get_transform_position(sprite->transform, position);
	gfx_get_transform_orientation(sprite->transform, orient);
	gfx_get_transform_scale(sprite->transform, scale);

	elf_set_physics_object_position(sprite->dobject, position[0], position[1], position[2]);
	elf_set_physics_object_orientation(sprite->dobject, orient[0], orient[1], orient[2], orient[3]);
	elf_set_physics_object_scale(sprite->dobject, scale[0], scale[1], scale[2]);

	if(sprite->scene) elf_set_physics_object_world(sprite->dobject, sprite->scene->dworld);
}

void elf_set_sprite_material(elf_sprite *sprite, elf_material *material)
{
	if(sprite->material) elf_dec_ref((elf_object*)sprite->material);
	sprite->material = material;
	if(sprite->material) elf_inc_ref((elf_object*)sprite->material);

	elf_calc_sprite_bounds(sprite);

	gfx_set_transform_scale(sprite->transform, sprite->real_scale.x, sprite->real_scale.y, sprite->real_scale.z);

	if(sprite->object)
	{
		elf_set_actor_physics((elf_actor*)sprite, elf_get_actor_shape((elf_actor*)sprite),
			elf_get_actor_mass((elf_actor*)sprite));
	}

	elf_reset_sprite_debug_physics_object(sprite);
}

void elf_set_sprite_scale(elf_sprite *sprite, float x, float y)
{
	sprite->scale.x = x; sprite->scale.y = y;

	elf_calc_sprite_bounds(sprite);

	gfx_set_transform_scale(sprite->transform, sprite->real_scale.x, sprite->real_scale.y, sprite->real_scale.z);

	if(sprite->object) elf_set_physics_object_scale(sprite->object, sprite->scale.x, sprite->scale.y, 1.0);
	if(sprite->dobject) elf_set_physics_object_scale(sprite->dobject, sprite->scale.x, sprite->scale.y, 1.0);
}

void elf_set_sprite_face_camera(elf_sprite *sprite, unsigned char face_camera)
{
	sprite->face_camera = !face_camera == ELF_FALSE;
}

elf_material* elf_get_sprite_material(elf_sprite *sprite)
{
	return sprite->material;
}

elf_vec2f elf_get_sprite_scale(elf_sprite *sprite)
{
	return sprite->scale;
}

unsigned char elf_get_sprite_face_camera(elf_sprite *sprite)
{
	return sprite->face_camera;
}

unsigned char elf_cull_sprite(elf_sprite *sprite, elf_camera *camera)
{
	if(!sprite->material || !sprite->visible) return ELF_TRUE;

	return !elf_sphere_inside_frustum(camera, &sprite->position.x, sprite->cull_radius);
}

void elf_draw_sprite(elf_sprite *sprite, gfx_shader_params *shader_params)
{
	unsigned char light_type;

	if(!sprite->material || !sprite->visible) return;

	gfx_mul_matrix4_matrix4(gfx_get_transform_matrix(sprite->transform),
			shader_params->camera_matrix, shader_params->modelview_matrix);

	elf_set_material(sprite->material, shader_params);

	light_type = shader_params->light_params.type;

	if(!sprite->material->lighting)
	{
		if(sprite->material->non_lit_flag == eng->non_lit_flag && sprite->non_lit_flag == eng->non_lit_flag) return;
		sprite->material->non_lit_flag = sprite->non_lit_flag = eng->non_lit_flag;

		shader_params->light_params.type = GFX_NONE;
	}

	gfx_set_shader_params(shader_params);

	shader_params->light_params.type = light_type;

	gfx_draw_vertex_array(eng->sprite_vertex_array, 12, GFX_TRIANGLES);
}

void elf_draw_sprite_ambient(elf_sprite *sprite, gfx_shader_params *shader_params)
{
	if(!sprite->material || !sprite->visible) return;

	gfx_mul_matrix4_matrix4(gfx_get_transform_matrix(sprite->transform),
			shader_params->camera_matrix, shader_params->modelview_matrix);

	elf_set_material(sprite->material, shader_params);
	shader_params->material_params.color.r = eng->ambient_color.r*sprite->material->ambient_color.r;
	shader_params->material_params.color.g = eng->ambient_color.g*sprite->material->ambient_color.g;
	shader_params->material_params.color.b = eng->ambient_color.b*sprite->material->ambient_color.b;
	gfx_set_shader_params(shader_params);

	gfx_draw_vertex_array(eng->sprite_vertex_array, 12, GFX_TRIANGLES);
}

void elf_draw_sprite_without_materials(elf_sprite *sprite, gfx_shader_params *shader_params)
{
	if(!sprite->material || !sprite->visible) return;

	gfx_mul_matrix4_matrix4(gfx_get_transform_matrix(sprite->transform),
			shader_params->camera_matrix, shader_params->modelview_matrix);

	elf_set_material_alpha_textures(sprite->material, shader_params);
	gfx_set_shader_params(shader_params);
	gfx_set_texture_params_default(shader_params);

	gfx_draw_vertex_array(eng->sprite_vertex_array, 12, GFX_TRIANGLES);
}

void elf_draw_sprite_debug(elf_sprite *sprite, gfx_shader_params *shader_params)
{
	float *vertex_buffer;

	gfx_mul_matrix4_matrix4(gfx_get_transform_matrix(sprite->transform),
		shader_params->camera_matrix, shader_params->modelview_matrix);

	vertex_buffer = (float*)gfx_get_vertex_data_buffer(eng->lines);

	vertex_buffer[0] = -0.5;
	vertex_buffer[1] = 0.5;
	vertex_buffer[2] = 0.0;

	vertex_buffer[3] = -0.5;
	vertex_buffer[4] = -0.5;
	vertex_buffer[5] = 0.0;

	vertex_buffer[6] = -0.5;
	vertex_buffer[7] = -0.5;
	vertex_buffer[8] = 0.0;

	vertex_buffer[9] = 0.5;
	vertex_buffer[10] = -0.5;
	vertex_buffer[11] = 0.0;

	vertex_buffer[12] = 0.5;
	vertex_buffer[13] = -0.5;
	vertex_buffer[14] = 0.0;

	vertex_buffer[15] = 0.5;
	vertex_buffer[16] = 0.5;
	vertex_buffer[17] = 0.0;

	vertex_buffer[18] = 0.5;
	vertex_buffer[19] = 0.5;
	vertex_buffer[20] = 0.0;

	vertex_buffer[21] = -0.5;
	vertex_buffer[22] = 0.5;
	vertex_buffer[23] = 0.0;

	if(!sprite->selected) gfx_set_color(&shader_params->material_params.color, 0.6, 0.2, 0.6, 1.0);
	else gfx_set_color(&shader_params->material_params.color, 1.0, 0.0, 0.0, 1.0);
	gfx_set_shader_params(shader_params);
	gfx_draw_lines(8, eng->lines);
}

