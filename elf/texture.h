
elf_texture *elf_create_texture()
{
	elf_texture *texture;

	texture = (elf_texture*)malloc(sizeof(elf_texture));
	memset(texture, 0x0, sizeof(elf_texture));
	texture->type = ELF_TEXTURE;

	texture->id = ++texture_id_counter;

	global_obj_count++;

	return texture;
}

elf_texture* elf_create_texture_from_file(const char *file_path)
{
	elf_image *image;
	elf_texture *texture;
	int format;
	int internal_format;
	int width, height;
	unsigned char *data;

	image = elf_create_image_from_file(file_path);
	if(!image) return NULL;

	switch(elf_get_image_bits_per_pixel(image))
	{
		case 8: format = GFX_LUMINANCE; internal_format = GFX_LUMINANCE; break;
		case 16: format = GFX_LUMINANCE_ALPHA; internal_format = GFX_LUMINANCE_ALPHA; break;
		case 24:
			format = GFX_BGR;
			internal_format = GFX_COMPRESSED_RGB;
			width = elf_get_image_width(image);
			height = elf_get_image_height(image);
			data = (unsigned char*)elf_get_image_data(image);
			break;
		case 32:
			format = GFX_BGRA;
			internal_format = GFX_COMPRESSED_RGBA;
			break;
		default:
			elf_set_error(ELF_INVALID_FILE, "error: unsupported bits per pixel value [%d] in image \"%s\"\n", elf_get_image_bits_per_pixel(image), file_path);
			elf_destroy_image(image);
			return NULL;
	}

	texture = elf_create_texture();

	texture->file_path = elf_create_string(file_path);

	texture->texture = gfx_create_2d_texture(elf_get_image_width(image), elf_get_image_height(image),
		eng->texture_anisotropy, GFX_REPEAT, GFX_LINEAR, format, internal_format, GFX_UBYTE, elf_get_image_data(image));

	if(!texture->texture)
	{
		elf_set_error(ELF_CANT_CREATE, "error: failed to create texture \"%s\"\n", file_path);
		elf_destroy_texture(texture);
		elf_destroy_image(image);
		return NULL;
	}

	elf_destroy_image(image);

	return texture;
}

void elf_destroy_texture(elf_texture *texture)
{
	if(texture->name) elf_destroy_string(texture->name);
	if(texture->file_path) elf_destroy_string(texture->file_path);

	if(texture->texture) gfx_destroy_texture(texture->texture);
	if(texture->data) free(texture->data);

	global_obj_count--;

	free(texture);
}

const char* elf_get_texture_name(elf_texture *texture)
{
	return texture->name;
}

const char* elf_get_texture_file_path(elf_texture *texture)
{
	return texture->file_path;
}

int elf_get_texture_width(elf_texture *texture)
{
	return gfx_get_texture_width(texture->texture);
}

int elf_get_texture_height(elf_texture *texture)
{
	return gfx_get_texture_height(texture->texture);
}

int elf_get_texture_format(elf_texture *texture)
{
	return gfx_get_texture_format(texture->texture);
}

int elf_get_texture_data_format(elf_texture *texture)
{
	return gfx_get_texture_data_format(texture->texture);
}

gfx_texture* elf_get_gfx_texture(elf_texture* texture)
{
	return texture->texture;
}

void elf_set_texture(int slot, elf_texture *texture, gfx_shader_params *shader_params)
{
	if(!texture->texture || slot < 0 || slot > GFX_MAX_TEXTURES-1) return;

	shader_params->texture_params[slot].texture = texture->texture;
}

unsigned char elf_load_texture_data(elf_texture *texture)
{
	const char *file_type;
	FILE *file;
	elf_pak *pak;
	elf_pak_index *index;
	int magic;
	char name[64];
	unsigned char type;

	if(texture->data) return ELF_TRUE;

	file_type = strrchr(texture->file_path, '.');

	if(!strcmp(file_type, ".pak"))
	{
		pak = elf_create_pak_from_file(texture->file_path);
		if(!pak) return ELF_FALSE;

		index = elf_get_pak_index_by_name(pak, texture->name, ELF_TEXTURE);
		if(!index)
		{
			elf_set_error(ELF_INVALID_FILE, "error: couldn't fine index for \"%s//%s\"\n", texture->file_path, texture->name);
			elf_destroy_pak(pak);
			return ELF_FALSE;
		}

		file = fopen(pak->file_path, "rb");
		fseek(file, elf_get_pak_index_offset(index), SEEK_SET);
		if(feof(file)) return ELF_FALSE;

		fread((char*)&magic, sizeof(int), 1, file);

		if(magic != 179532108)
		{
			elf_set_error(ELF_INVALID_FILE, "error: invalid texture \"%s//%s\", wrong magic number\n", texture->file_path, texture->name);
			elf_destroy_pak(pak);
			fclose(file);
			return ELF_FALSE;
		}

		fread(name, sizeof(char), 64, file);
		fread((char*)&type, sizeof(unsigned char), 1, file);

		if(type == 1)
		{
			fread((char*)&texture->data_size, sizeof(unsigned int), 1, file);
	 
			texture->data = (char*)malloc(texture->data_size);
			fread((char*)texture->data, 1, texture->data_size, file);
		}
		else
		{
			elf_set_error(ELF_UNKNOWN_FORMAT, "error: can't load texture \"%s//%s\", unknown format\n", texture->file_path, texture->name);
			elf_destroy_pak(pak);
			fclose(file);
			return ELF_FALSE;
		}

		elf_destroy_pak(pak);
		fclose(file);
	}
	else
	{
		file = fopen(texture->file_path, "rb");
		if(!file)
		{
			elf_set_error(ELF_CANT_OPEN_FILE, "error: can't open file \"%s\"", texture->file_path);
			return ELF_FALSE;
		}

		fseek(file, 0, SEEK_END);
		texture->data_size = ftell(file);
		fseek(file, 0, SEEK_SET);

		texture->data = malloc(texture->data_size);
		fread((char*)texture->data, 1, texture->data_size, file);

		fclose(file);
	}

	return ELF_TRUE;
}

void elf_unload_texture_data(elf_texture *texture)
{
	if(texture->data) free(texture->data);
	texture->data = NULL;
	texture->data_size = 0;
}

