
elf_texture *elf_create_texture()
{
	elf_texture *texture;

	texture = (elf_texture*)malloc(sizeof(elf_texture));
	memset(texture, 0x0, sizeof(elf_texture));
	texture->type = ELF_TEXTURE;

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

