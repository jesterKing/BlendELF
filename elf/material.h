
elf_material* elf_create_material(const char *name)
{
	elf_material *material;

	material = (elf_material*)malloc(sizeof(elf_material));
	memset(material, 0x0, sizeof(elf_material));
	material->type = ELF_MATERIAL;

	material->diffuse_color.r = material->diffuse_color.g = material->diffuse_color.b = material->diffuse_color.a = 1.0;
	material->ambient_color.r = material->ambient_color.g = material->ambient_color.b = material->ambient_color.a = 1.0;
	material->specular_color.r = material->specular_color.g = material->specular_color.b = 0.25; material->specular_color.a = 1.0;
	material->spec_power = 32.0;
	material->lighting = ELF_TRUE;

	material->alpha_test = ELF_FALSE;
	material->alpha_threshold = 0.99;
	material->parallax_scale = 0.25;

	if(name) material->name = elf_create_string(name);

	elf_inc_obj_count();

	material->id = ++gen->material_id_counter;

	return material;
}

void elf_destroy_material(elf_material *material)
{
	if(material->name) elf_destroy_string(material->name);
	if(material->file_path) elf_destroy_string(material->file_path);

	if(material->diffuse_map) elf_dec_ref((elf_object*)material->diffuse_map);
	if(material->normal_map) elf_dec_ref((elf_object*)material->normal_map);
	if(material->height_map) elf_dec_ref((elf_object*)material->height_map);
	if(material->specular_map) elf_dec_ref((elf_object*)material->specular_map);
	if(material->light_map) elf_dec_ref((elf_object*)material->light_map);

	free(material);

	elf_dec_obj_count();
}

void elf_set_material_diffuse_color(elf_material *material, float r, float g, float b, float a)
{
	material->diffuse_color.r = r;
	material->diffuse_color.g = g;
	material->diffuse_color.b = b;
	material->diffuse_color.a = a;
}

void elf_set_material_ambient_color(elf_material *material, float r, float g, float b, float a)
{
	material->ambient_color.r = r;
	material->ambient_color.g = g;
	material->ambient_color.b = b;
	material->ambient_color.a = a;
}

void elf_set_material_specular_color(elf_material *material, float r, float g, float b, float a)
{
	material->specular_color.r = r;
	material->specular_color.g = g;
	material->specular_color.b = b;
	material->specular_color.a = a;
}

void elf_set_material_specular_power(elf_material *material, float power)
{
	material->spec_power = power;
}

void elf_set_material_lighting(elf_material *material, unsigned char lighting)
{
	material->lighting = !lighting == ELF_FALSE;
}

const char* elf_get_material_name(elf_material *material)
{
	return material->name;
}

const char* elf_get_material_file_path(elf_material *material)
{
	return material->file_path;
}

elf_color elf_get_material_diffuse_color(elf_material *material)
{
	return material->diffuse_color;
}

elf_color elf_get_material_ambient_color(elf_material *material)
{
	return material->ambient_color;
}

elf_color elf_get_material_specular_color(elf_material *material)
{
	return material->specular_color;
}

float elf_get_material_specular_power(elf_material *material)
{
	return material->spec_power;
}

unsigned char elf_get_material_lighting(elf_material *material)
{
	return material->lighting;
}

void elf_set_material_diffuse_map(elf_material *material, elf_texture *texture)
{
	int format;

	if(material->diffuse_map) elf_dec_ref((elf_object*)material->diffuse_map);
	material->diffuse_map = texture;
	if(material->diffuse_map)
	{
		elf_inc_ref((elf_object*)material->diffuse_map);

		format = elf_get_texture_format(material->diffuse_map);
		if(format == GFX_RGBA || format == GFX_COMPRESSED_RGBA ||
			format == GFX_BGRA || format == GFX_LUMINANCE_ALPHA)
			material->alpha_test = ELF_TRUE;
	}
}

void elf_set_material_normal_map(elf_material *material, elf_texture *texture)
{
	if(material->normal_map) elf_dec_ref((elf_object*)material->normal_map);
	material->normal_map = texture;
	if(material->normal_map) elf_inc_ref((elf_object*)material->normal_map);
}

void elf_set_material_height_map(elf_material *material, elf_texture *texture)
{
	if(material->height_map) elf_dec_ref((elf_object*)material->height_map);
	material->height_map = texture;
	if(material->height_map) elf_inc_ref((elf_object*)material->height_map);
}

void elf_set_material_specular_map(elf_material *material, elf_texture *texture)
{
	if(material->specular_map) elf_dec_ref((elf_object*)material->specular_map);
	material->specular_map = texture;
	if(material->specular_map) elf_inc_ref((elf_object*)material->specular_map);
}

void elf_set_material_light_map(elf_material *material, elf_texture *texture)
{
	if(material->light_map) elf_dec_ref((elf_object*)material->light_map);
	material->light_map = texture;
	if(material->light_map) elf_inc_ref((elf_object*)material->light_map);
}

elf_texture* elf_get_material_diffuse_map(elf_material *material)
{
	return material->diffuse_map;
}

elf_texture* elf_get_material_normal_map(elf_material *material)
{
	return material->normal_map;
}

elf_texture* elf_get_material_height_map(elf_material *material)
{
	return material->height_map;
}

elf_texture* elf_get_material_specular_map(elf_material *material)
{
	return material->specular_map;
}

elf_texture* elf_get_material_light_map(elf_material *material)
{
	return material->light_map;
}

void elf_set_material_parallax_scale(elf_material *material, float scale)
{
	material->parallax_scale = scale;
	if(material->parallax_scale < 0.0) material->parallax_scale = 0.0;
}

void elf_set_material_alpha_test(elf_material *material, unsigned char alpha_test)
{
	material->alpha_test = !alpha_test == ELF_FALSE;
}

void elf_set_material_alpha_threshold(elf_material *material, float threshold)
{
	material->alpha_threshold = threshold;
	if(material->alpha_threshold < 0.0) material->alpha_threshold = 0.0;
	if(material->alpha_threshold > 1.0) material->alpha_threshold = 1.0;
}

float elf_get_material_parallax_scale(elf_material *material)
{
	return material->parallax_scale;
}

unsigned char elf_get_material_alpha_test(elf_material *material)
{
	return material->alpha_test;
}

float elf_get_material_alpha_threshold(elf_material *material)
{
	return material->alpha_threshold;
}

void elf_set_texture_params_default(gfx_texture_params *params)
{
	params->type = ELF_COLOR_MAP;
	params->texture = NULL;
	params->projection_mode = GFX_NONE;
}

void elf_set_material_alpha_texture(elf_material *material, gfx_shader_params *shader_params)
{
	if(!material->alpha_test) return;

	if(material->diffuse_map)
	{
		if(material->alpha_test)
		{
			shader_params->render_params.alpha_test = ELF_TRUE;
			shader_params->render_params.alpha_threshold = material->alpha_threshold;
		}
		shader_params->texture_params[0].type = ELF_COLOR_MAP;
		shader_params->texture_params[0].texture = material->diffuse_map->texture;
		shader_params->texture_params[0].projection_mode = GFX_NONE;
	}
	else
	{
		shader_params->render_params.alpha_test = ELF_FALSE;
		shader_params->render_params.alpha_threshold = 0.99;
		elf_set_texture_params_default(&shader_params->texture_params[0]);
	}
}

void elf_set_material(elf_material *material, gfx_shader_params *shader_params)
{
	memcpy(&shader_params->material_params.color.r, &material->diffuse_color.r, sizeof(float)*4);
	memcpy(&shader_params->material_params.specular_color.r, &material->specular_color.r, sizeof(float)*4);
	shader_params->material_params.spec_power = material->spec_power;

	if(material->diffuse_map)
	{
		if(material->alpha_test)
		{
			shader_params->render_params.alpha_test = ELF_TRUE;
			shader_params->render_params.alpha_threshold = material->alpha_threshold;
		}
		shader_params->texture_params[0].type = ELF_COLOR_MAP;
		shader_params->texture_params[0].texture = material->diffuse_map->texture;
		shader_params->texture_params[0].projection_mode = GFX_NONE;
	}
	else
	{
		shader_params->render_params.alpha_test = ELF_FALSE;
		shader_params->render_params.alpha_threshold = 0.99;
		elf_set_texture_params_default(&shader_params->texture_params[0]);
	}

	if(material->normal_map)
	{
		shader_params->texture_params[1].type = ELF_NORMAL_MAP;
		shader_params->texture_params[1].texture = material->normal_map->texture;
		shader_params->texture_params[1].projection_mode = GFX_NONE;
	}
	else
	{
		elf_set_texture_params_default(&shader_params->texture_params[1]);
	}

	if(material->height_map)
	{
		shader_params->texture_params[2].type = ELF_HEIGHT_MAP;
		shader_params->texture_params[2].texture = material->height_map->texture;
		shader_params->texture_params[2].projection_mode = GFX_NONE;
		shader_params->texture_params[2].parallax_scale = material->parallax_scale*0.05;
	}
	else
	{
		elf_set_texture_params_default(&shader_params->texture_params[2]);
	}

	if(material->specular_map)
	{
		shader_params->texture_params[3].type = ELF_SPECULAR_MAP;
		shader_params->texture_params[3].texture = material->specular_map->texture;
		shader_params->texture_params[3].projection_mode = GFX_NONE;
	}
	else
	{
		elf_set_texture_params_default(&shader_params->texture_params[3]);
	}

	if(material->light_map)
	{
		shader_params->texture_params[4].type = ELF_LIGHT_MAP;
		shader_params->texture_params[4].texture = material->light_map->texture;
		shader_params->texture_params[4].projection_mode = GFX_NONE;
	}
	else
	{
		elf_set_texture_params_default(&shader_params->texture_params[4]);
	}
}

