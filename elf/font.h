
elf_font* elf_create_font()
{
	elf_font *font;

	font = (elf_font*)malloc(sizeof(elf_font));
	memset(font, 0x0, sizeof(elf_font));
	font->type = ELF_FONT;

	global_obj_count++;

	return font;
}

elf_font* elf_create_font_from_file(const char *file_path, int size)
{
	FT_Library library;
	FT_Face face;
	FT_GlyphSlot slot;
	elf_font *font;
	int width;
	int height;
	unsigned char *data;
	int error;
	int i, j, k;

	if(size < 1)
	{
		elf_set_error(ELF_INVALID_SIZE, "error: can not load a font size smaller than 1\n");
		return NULL;
	}

	error = FT_Init_FreeType(&library);
	if(error)
	{
		elf_set_error(ELF_CANT_INITIALIZE, "error: could not initialize freetype, could not open \"%s\"\n", file_path);
		return NULL;
	}

	error = FT_New_Face(library, file_path, 0, &face);
	if(error)
	{
		elf_set_error(ELF_CANT_OPEN_FILE, "error: can't open file \"%s\"\n", file_path);
		error = FT_Done_FreeType(library);
		if(error)
		{
			elf_set_error(ELF_CANT_OPEN_FILE, "error: can't deinitialize freetype\n");
			return NULL;
		}
		return NULL;
	}

	error = FT_Set_Pixel_Sizes(face, 0, size);
	if(error)
	{
		elf_set_error(ELF_INVALID_SIZE, "error: can't set the size of font \"%s\"\n", file_path);

		error = FT_Done_Face(face);
		error = FT_Done_FreeType(library);

		return NULL;
	}

	font = elf_create_font();

	font->file_path = elf_create_string(file_path);
	font->size = size;

	slot = face->glyph;

	for(i = 33; i < 127; i++)
	{
		error = FT_Load_Char(face, (char)i, FT_LOAD_RENDER|FT_LOAD_FORCE_AUTOHINT);
		if(error)
		{
			elf_write_to_log("warning: could not load character \"%c\" in font \"%s\"\n", (char)i, file_path);
			continue;
		}

		width = slot->bitmap.width;
		height = slot->bitmap.rows;
		if(width < 1 || height < 1) continue;

		data = (unsigned char*)malloc(sizeof(unsigned char)*width*height*2);
		for(j = 0; j < height; j++)
		{
			for(k = 0; k < width; k++)
			{
				data[j*width*2+k*2] = 255;
				data[j*width*2+k*2+1] = slot->bitmap.buffer[((height-1)-j)*width+k];
			}
		}

		font->chars[i].code = (char)i;
		font->chars[i].offset_x = width+size/7;
		font->chars[i].offset_y = -(face->glyph->bitmap.rows-face->glyph->bitmap_top);
		font->chars[i].texture = gfx_create_2d_texture(width, height, 0.0f, GFX_CLAMP, GFX_NEAREST, GFX_LUMINANCE_ALPHA, GFX_LUMINANCE_ALPHA, GFX_UBYTE, data);

		if(-font->chars[i].offset_y > font->offset_y) font->offset_y = -font->chars[i].offset_y;

		free(data);
		data = NULL;
	}

	error = FT_Done_Face(face);
	error = FT_Done_FreeType(library);

	return font;
}

void elf_destroy_font(elf_font *font)
{
	int i;

	if(font->name) elf_destroy_string(font->name);
	if(font->file_path) elf_destroy_string(font->file_path);

	for(i = 0; i < 128; i++)
		if(font->chars[i].texture) gfx_destroy_texture(font->chars[i].texture);

	free(font);

	global_obj_count--;
}

const char* elf_get_font_name(elf_font *font)
{
	return font->name;
}

const char* elf_get_font_file_path(elf_font *font)
{
	return font->file_path;
}

int elf_get_font_size(elf_font *font)
{
	return font->size;
}

int elf_get_string_width(elf_font *font, const char *str)
{
	int x = 0;
	int ox = 0;
	int y = font->size;
	elf_character *chr = 0;
	unsigned int i;

	for(i = 0; i < strlen(str); i++)
	{
		chr = &font->chars[(unsigned int)str[i]];
		if(str[i] == ' ') ox += font->size/3;
		else if(str[i] == '\t') ox += font->size/3*5;
		else if(str[i] == '\n')
		{
			y += font->size+1;
			ox = 0;
		}
		else
		{
			if(i != strlen(str)-1 || !chr->texture) ox += chr->offset_x;
			else ox += gfx_get_texture_width(chr->texture);
		}
		if(ox > x) x = ox;
	}

	return x;
}

int elf_get_string_height(elf_font *font, const char *str)
{
	int ox = 0;
	int y = font->size;
	elf_character *chr = 0;
	unsigned int i;

	for(i = 0; i < strlen(str); i++)
	{
		chr = &font->chars[(unsigned int)str[i]];
		if(str[i] == ' ') ox += font->size/3;
		else if(str[i] == '\t') ox += font->size/3*5;
		else if(str[i] == '\n')
		{
			y += font->size+1;
			ox = 0;
		}
		else
		{
			if(i != strlen(str)-1 || !chr->texture) ox += chr->offset_x;
			else ox += gfx_get_texture_width(chr->texture);
		}
	}

	return y;
}

void elf_draw_string(elf_font *font, const char *str, int x, int y, gfx_shader_params *shader_params)
{
	int ox;
	int oy;
	elf_character *chr;
	unsigned int i;

	ox = x;
	oy = y;

	for(i = 0; i < strlen(str); i++)
	{
		chr = &font->chars[(unsigned int)str[i]];
		if(str[i] == ' ') ox += font->size/3;
		else if(str[i] == '\t') ox += font->size/3*5;
		else if(str[i] == '\n')
		{
			oy -= font->size+1;
			ox = x;
		}
		else
		{
			if(!chr->texture) continue;
			shader_params->texture_params[0].texture = chr->texture;
			shader_params->texture_params[0].type = GFX_COLOR_MAP;
			gfx_set_shader_params(shader_params);
			gfx_draw_textured_2d_quad((float)ox, (float)(oy+chr->offset_y+font->offset_y),
				(float)gfx_get_texture_width(chr->texture), (float)gfx_get_texture_height(chr->texture));
			ox += chr->offset_x;
		}
	}
}

