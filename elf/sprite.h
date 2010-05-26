
elf_sprite* elf_create_sprite(const char *name)
{
	elf_sprite *sprite;

	sprite = (elf_sprite*)malloc(sizeof(elf_sprite));
	memset(sprite, 0x0, sizeof(elf_sprite));
	sprite->type = ELF_SPRITE;

	elf_init_actor((elf_actor*)sprite, ELF_FALSE);

	sprite->scale.x = sprite->scale.y = sprite->scale.z = 1.0;
	sprite->texture_scale.x = sprite->texture_scale.y = 1.0;
	sprite->query = gfx_create_query();
	sprite->visible = ELF_TRUE;
	sprite->culled = ELF_TRUE;

	sprite->dobject = elf_create_physics_object_box(0.2, 0.2, 0.01, 0.0, 0.0, 0.0, 0.0);
	elf_set_physics_object_actor(sprite->dobject, (elf_actor*)sprite);
	elf_inc_ref((elf_object*)sprite->dobject);

	sprite->pbb_lengths.x = sprite->pbb_lengths.y = 0.4; sprite->pbb_lengths.z = 0.01;

	sprite->frame_player = elf_create_frame_player();
	elf_inc_ref((elf_object*)sprite->frame_player);

	if(name) sprite->name = elf_create_string(name);

	global_obj_count++;

	return sprite;
}

void elf_update_sprite(elf_sprite *sprite)
{
	elf_update_actor((elf_actor*)sprite);
}

void elf_destroy_sprite(elf_sprite *sprite)
{
	elf_clean_actor((elf_actor*)sprite);

	if(sprite->query) gfx_destroy_query(sprite->query);

	if(sprite->material) elf_dec_ref((elf_object*)sprite->material);
	elf_dec_ref((elf_object*)sprite->frame_player);

	free(sprite);

	global_obj_count--;
}

void elf_set_sprite_material(elf_sprite *sprite, elf_material *material)
{
	int i;
	if(sprite->material) elf_dec_ref((elf_object*)sprite->material);
	sprite->material = material;
	if(sprite->material) elf_inc_ref((elf_object*)sprite->material);

	for(i = 0; i < GFX_MAX_TEXTURES; i++)
	{
		if(sprite->material->textures[i] && sprite->material->textures[i]->texture)
		{
			sprite->texture_scale.x = (float)gfx_get_texture_width(sprite->material->textures[i]->texture)/100.0;
			sprite->texture_scale.y = (float)gfx_get_texture_height(sprite->material->textures[i]->texture)/100.0;
			gfx_set_transform_scale(sprite->transform, sprite->scale.x*sprite->texture_scale.x,
				sprite->scale.y*sprite->texture_scale.y, sprite->scale.z);
		}
	}
}

elf_material* elf_get_sprite_material(elf_sprite *sprite)
{
	return sprite->material;
}

void elf_set_sprite_scale(elf_sprite *sprite, float x, float y, float z)
{
	sprite->scale.x = x; sprite->scale.y = y; sprite->scale.z = z;
	gfx_set_transform_scale(sprite->transform, sprite->scale.x*sprite->texture_scale.x,
		sprite->scale.y*sprite->texture_scale.y, sprite->scale.z);

	if(sprite->object) elf_set_physics_object_scale(sprite->object, x, y, z);
	if(sprite->dobject) elf_set_physics_object_scale(sprite->dobject, x, y, z);
}

elf_vec3f elf_get_sprite_scale(elf_sprite *sprite)
{
	return sprite->scale;
}

unsigned char elf_cull_sprite(elf_sprite *sprite, elf_camera *camera)
{
	return ELF_FALSE;
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

