
elf_material* elf_create_material(const char *name)
{
	elf_material *material;
	int i = 0;

	material = (elf_material*)malloc(sizeof(elf_material));
	memset(material, 0x0, sizeof(elf_material));
	material->type = ELF_MATERIAL;

	for(i = 0; i < GFX_MAX_TEXTURES; i++)
	{
		material->textures[i] = NULL;
		material->texture_types[i] = GFX_COLOR_MAP;
		material->texture_parallax_scales[i] = 0.25;
		material->texture_alpha_tests[i] = 0.99;
	}

	material->diffuse_color.r = material->diffuse_color.g = material->diffuse_color.b = material->diffuse_color.a = 1.0;
	material->ambient_color.r = material->ambient_color.g = material->ambient_color.b = material->ambient_color.a = 1.0;
	material->specular_color.r = material->specular_color.g = material->specular_color.b = 0.25; material->specular_color.a = 1.0;
	material->spec_power = 32.0;
	material->lighting = ELF_TRUE;

	if(name) material->name = elf_create_string(name);

	global_obj_count++;

	material->id = ++material_id_counter;

	return material;
}

elf_material* elf_create_material_from_pak(FILE *file, const char *name, elf_scene *scene)
{
	elf_material *material;
	elf_texture *rtexture;
	int magic;
	char rname[64];
	unsigned char texture_count = 0;
	char texture[64];
	unsigned char texture_type;
	float parallax_scale;
	int i;

	fread((char*)&magic, sizeof(int), 1, file);

	if(magic != 179532109)
	{
		elf_set_error(ELF_INVALID_FILE, "error: invalid material \"%s//%s\", wrong magic number\n", elf_get_scene_file_path(scene), name);
		return NULL;
	}

	memset(rname, 0x0, sizeof(char)*64);
	fread(rname, sizeof(char), 64, file);

	material = elf_create_material(NULL);

	material->name = elf_create_string(rname);
	material->file_path = elf_create_string(elf_get_scene_file_path(scene));

	fread((char*)&material->diffuse_color.r, sizeof(float), 4, file);
	fread((char*)&material->ambient_color.r, sizeof(float), 4, file);
	fread((char*)&material->specular_color.r, sizeof(float), 4, file);
	fread((char*)&material->spec_power, sizeof(float), 1, file);

	elf_set_material_diffuse_color(material, material->diffuse_color.r, material->diffuse_color.g, material->diffuse_color.b, material->diffuse_color.a);
	elf_set_material_specular_color(material, material->specular_color.r, material->specular_color.g, material->specular_color.b, material->specular_color.a);
	elf_set_material_ambient_color(material, material->ambient_color.r, material->ambient_color.g, material->ambient_color.b, material->ambient_color.a);
	elf_set_material_specular_power(material, material->spec_power);

	fread((char*)&texture_count, sizeof(unsigned char), 1, file);

	for(i = 0; i < texture_count; i++)
	{
		if(i > GFX_MAX_TEXTURES-1) break;

		memset(texture, 0x0, sizeof(char)*64);
		fread(texture, sizeof(char), 64, file);
		fread((char*)&texture_type, sizeof(unsigned char), 1, file);
		fread((char*)&parallax_scale, sizeof(float), 1, file);

		rtexture = elf_get_or_load_texture_by_name(scene, texture);
		if(rtexture)
		{
			elf_set_material_texture(material, i, rtexture);
			elf_set_material_texture_type(material, i, texture_type);
			elf_set_material_texture_parallax_scale(material, i, parallax_scale);
		}
	}

	return material;
}

void elf_destroy_material(elf_material *material)
{
	int i;

	if(material->name) elf_destroy_string(material->name);
	if(material->file_path) elf_destroy_string(material->file_path);

	for(i = 0; i < GFX_MAX_TEXTURES; i++)
	{
		if(material->textures[i]) elf_dec_ref((elf_object*)material->textures[i]);
	}

	free(material);

	global_obj_count--;
}

void elf_set_material_texture(elf_material *material, int slot, elf_texture *texture)
{
	if(slot < 0 || slot > GFX_MAX_TEXTURES-1) return;

	if(material->textures[slot]) elf_dec_ref((elf_object*)material->textures[slot]);

	material->textures[slot] = texture;
	material->texture_types[slot] = ELF_COLOR_MAP;
	material->texture_parallax_scales[slot] = 0.25;

	if(material->textures[slot]) elf_inc_ref((elf_object*)material->textures[slot]);
}

void elf_set_material_texture_type(elf_material *material, int slot, int type)
{
	if(slot < 0 || slot > GFX_MAX_TEXTURES-1) return;

	if(type != ELF_COLOR_MAP && type != ELF_NORMAL_MAP && type != ELF_HEIGHT_MAP &&
		type != ELF_SPECULAR_MAP && type != ELF_COLOR_RAMP_MAP && type != ELF_LIGHT_MAP)
	{
		material->texture_types[slot] = ELF_COLOR_MAP;
	}
	else
	{
		material->texture_types[slot] = type;
	}
}

void elf_set_material_texture_parallax_scale(elf_material *material, int slot, float scale)
{
	if(slot < 0 || slot > GFX_MAX_TEXTURES-1) return;

	material->texture_parallax_scales[slot] = scale;
}

void elf_set_material_texture_alpha_test(elf_material *material, int slot, float test)
{
	if(slot < 0 || slot > GFX_MAX_TEXTURES-1) return;

	material->texture_alpha_tests[slot] = test;
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

elf_texture* elf_get_material_texture(elf_material *material, int slot)
{
	if(slot < 0 || slot > GFX_MAX_TEXTURES-1) return NULL;

	return material->textures[slot];
}

int elf_get_material_texture_type(elf_material *material, int slot)
{
	if(slot < 0 || slot > GFX_MAX_TEXTURES-1) return 0;

	return material->texture_types[slot];
}

float elf_get_material_texture_parallax_scale(elf_material *material, int slot)
{
	if(slot < 0 || slot > GFX_MAX_TEXTURES-1) return 0.0;

	return material->texture_parallax_scales[slot];
}

float elf_get_material_texture_alpha_texture(elf_material *material, int slot)
{
	if(slot < 0 || slot > GFX_MAX_TEXTURES-1) return 0.0;

	return material->texture_alpha_tests[slot];
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

unsigned char elf_material_has_alpha_texture(elf_material *material)
{
	int i;
	int format;

	for(i = 0; i < GFX_MAX_TEXTURES; i++)
	{
		if(material->textures[i] && material->texture_types[i] == GFX_COLOR_MAP)
		{
			format = elf_get_texture_format(material->textures[i]);
			if(format == GFX_RGBA || format == GFX_COMPRESSED_RGBA || format == GFX_BGRA || format == GFX_LUMINANCE_ALPHA) return ELF_TRUE;
		}
	}

	return ELF_FALSE;
}

void elf_set_material_alpha_textures(elf_material *material, gfx_shader_params *shader_params)
{
	int i;
	int format;

	shader_params->render_params.alpha_test = ELF_FALSE;
	shader_params->render_params.alpha_threshold = 0.0;

	for(i = 0; i < GFX_MAX_TEXTURES; i++)
	{
		if(shader_params->texture_params[i].type != GFX_SHADOW_MAP)
		{
			if(material->textures[i] && material->texture_types[i] == GFX_COLOR_MAP)
			{
				format = elf_get_texture_format(material->textures[i]);
				if(format == GFX_RGBA || format == GFX_COMPRESSED_RGBA || format == GFX_BGRA || format == GFX_LUMINANCE_ALPHA)
				{
					shader_params->render_params.alpha_test = ELF_TRUE;
					shader_params->render_params.alpha_threshold = material->texture_alpha_tests[i];
					shader_params->texture_params[i].type = material->texture_types[i];
					shader_params->texture_params[i].texture = material->textures[i]->texture;
					shader_params->texture_params[i].parallax_scale = material->texture_parallax_scales[i]*0.05;
					shader_params->texture_params[i].projection_mode = GFX_NONE;
					gfx_matrix4_set_identity(shader_params->texture_params[i].matrix);
				}
			}
			else
			{
				shader_params->texture_params[i].type = GFX_COLOR_MAP;
				shader_params->texture_params[i].texture = NULL;
				shader_params->texture_params[i].projection_mode = GFX_NONE;
				gfx_matrix4_set_identity(shader_params->texture_params[i].matrix);
			}
		}
	}
}

void elf_set_material(elf_material *material, gfx_shader_params *shader_params)
{
	int i;
	int format;

	memcpy(&shader_params->material_params.color.r, &material->diffuse_color.r, sizeof(float)*4);
	memcpy(&shader_params->material_params.specular_color.r, &material->specular_color.r, sizeof(float)*4);
	shader_params->material_params.spec_power = material->spec_power;

	shader_params->render_params.alpha_test = ELF_FALSE;
	shader_params->render_params.alpha_threshold = 0.0;

	for(i = 0; i < GFX_MAX_TEXTURES; i++)
	{
		if(shader_params->texture_params[i].type != GFX_SHADOW_MAP)
		{
			if(material->textures[i])
			{
				shader_params->texture_params[i].type = material->texture_types[i];
				shader_params->texture_params[i].texture = material->textures[i]->texture;
				shader_params->texture_params[i].parallax_scale = material->texture_parallax_scales[i]*0.05;
				shader_params->texture_params[i].projection_mode = GFX_NONE;
				gfx_matrix4_set_identity(shader_params->texture_params[i].matrix);
				format = elf_get_texture_format(material->textures[i]);
				if(format == GFX_RGBA || format == GFX_COMPRESSED_RGBA || format == GFX_BGRA || format == GFX_LUMINANCE_ALPHA)
				{
					shader_params->render_params.alpha_test = ELF_TRUE;
					shader_params->render_params.alpha_threshold = material->texture_alpha_tests[i];
				}
			}
			else
			{
				shader_params->texture_params[i].type = GFX_COLOR_MAP;
				shader_params->texture_params[i].texture = NULL;
				shader_params->texture_params[i].projection_mode = GFX_NONE;
				gfx_matrix4_set_identity(shader_params->texture_params[i].matrix);
			}
		}
	}
}

