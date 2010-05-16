
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
			elf_write_to_log("error: unsupported bits per pixel value [%d] in image \"%s\"\n", elf_get_image_bits_per_pixel(image), file_path);
			elf_destroy_image(image);
			return NULL;
	}

	texture = elf_create_texture();

	texture->file_path = elf_create_string(file_path);

	texture->texture = gfx_create_2d_texture(elf_get_image_width(image), elf_get_image_height(image),
		eng->texture_anisotropy, GFX_REPEAT, GFX_LINEAR, format, internal_format, GFX_UBYTE, elf_get_image_data(image));

	if(!texture->texture)
	{
		elf_write_to_log("error: failed to create texture \"%s\"\n", file_path);
		elf_destroy_texture(texture);
		return NULL;
	}

	elf_destroy_image(image);

	return texture;
}

elf_texture *elf_create_texture_from_pak(FILE *file, const char *name, elf_scene *scene)
{
	elf_texture *texture;
	FIMEMORY *fi_mem;
	FIBITMAP *fi_bitmap;
	char *mem;
	FREE_IMAGE_FORMAT fi_format;
	int magic;
	char rname[64];
	unsigned char type;
	int width;
	int height;
	unsigned char bpp;
	int length;
	int format;
	int internal_format;
	int data_format;
	unsigned char *data;

	fread((char*)&magic, sizeof(int), 1, file);

	if(magic != 179532108)
	{
		elf_write_to_log("error: invalid texture file \"%s//%s\", wrong magic number\n", elf_get_scene_file_path(scene), name);
		return NULL;
	}

	fread(rname, sizeof(char), 64, file);
	fread((char*)&type, sizeof(unsigned char), 1, file);

	if(type == 0)
	{
		fread((char*)&width, sizeof(int), 1, file);
		fread((char*)&height, sizeof(int), 1, file);
		fread((char*)&bpp, sizeof(unsigned char), 1, file);
		data = (unsigned char*)malloc(sizeof(unsigned char)*width*height*bpp);
		fread(data, sizeof(unsigned char), width*height*bpp, file);
	}
	else if(type == 1)
	{
		// mbg: sizeof(int) fixes a bug on 64-bit platforms
		fread((char*)&length, sizeof(int), 1, file);

		mem = (char*)malloc(sizeof(char)*length);
		fread(mem, sizeof(char), length, file);

		fi_mem = FreeImage_OpenMemory((BYTE*)mem, length);
		fi_format = FreeImage_GetFileTypeFromMemory(fi_mem, 0);
		fi_bitmap = FreeImage_LoadFromMemory(fi_format, fi_mem, 0);

		width = FreeImage_GetWidth(fi_bitmap);
		height = FreeImage_GetHeight(fi_bitmap);
		bpp = FreeImage_GetBPP(fi_bitmap);

		data = (unsigned char*)malloc(sizeof(char)*width*height*(bpp/8));
		FreeImage_ConvertToRawBits((BYTE*)data, fi_bitmap, width*(bpp/8), bpp,
			FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, FALSE);

		FreeImage_Unload(fi_bitmap);
		FreeImage_CloseMemory(fi_mem);

		free(mem);
	}
	else
	{
		elf_write_to_log("error: invalid texture file \"%s//%s\", unknown format\n", elf_get_scene_file_path(scene), rname);
		return NULL;
	}

	switch(bpp)
	{
		case 8: format = GFX_LUMINANCE; internal_format = GFX_LUMINANCE; data_format = GFX_UBYTE; break;
		case 16: format = GFX_LUMINANCE_ALPHA; internal_format = GFX_LUMINANCE_ALPHA; data_format = GFX_UBYTE; break;
		case 24: format = GFX_BGR; internal_format = GFX_COMPRESSED_RGB; data_format = GFX_UBYTE; break;
		case 32: format = GFX_BGRA; internal_format = GFX_COMPRESSED_RGBA; data_format = GFX_UBYTE; break;
		case 48: format = GFX_BGR; internal_format = GFX_COMPRESSED_RGB; data_format = GFX_USHORT; break;
		default:
			elf_write_to_log("error: unsupported bits per pixel value [%d] in texture \"%s//%s\"\n", (int)bpp, elf_get_scene_file_path(scene), rname);
			free(data);
			return NULL;
	}

	texture = elf_create_texture();

	texture->name = elf_create_string(rname);
	texture->file_path = elf_create_string(elf_get_scene_file_path(scene));
	texture->texture = gfx_create_2d_texture(width, height, eng->texture_anisotropy, GFX_REPEAT, GFX_LINEAR, format, internal_format, data_format, data);

	free(data);

	if(!texture->texture)
	{
		elf_write_to_log("error: failed to create texture \"%s//%s\"\n", elf_get_scene_file_path(scene), rname);
		elf_destroy_texture(texture);
		return NULL;
	}

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
