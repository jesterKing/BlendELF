
elf_config* elf_create_config()
{
	elf_config *config;

	config = (elf_config*)malloc(sizeof(elf_config));
	memset(config, 0x0, sizeof(elf_config));
	config->type = ELF_CONFIG;

	config->window_size[0] = 1024;
	config->window_size[1] = 768;
	config->fullscreen = ELF_FALSE;
	config->texture_anisotropy = 1.0;
	config->shadow_map_size = 1024;
	config->start = elf_create_string("");
	config->log = elf_create_string("elf.log");

	elf_inc_obj_count();

	return config;
}

void elf_destroy_config(elf_config *config)
{
	if(config->start) elf_destroy_string(config->start);
	if(config->log) elf_destroy_string(config->log);

	free(config);

	elf_dec_obj_count();
}

elf_config* elf_read_config(const char *file_path)
{
	elf_config *config;
	FILE *file;
	int length;
	char *text;
	int pos;
	char *str;
	int scope;

	file = fopen(file_path, "r");
	if(!file) return NULL;

	fseek(file, 0, SEEK_END);
	length = ftell(file);
	fseek(file, 0, SEEK_SET);
	if(length > 0)
	{
		text = (char*)malloc(sizeof(char)*length+1);
		memset(text, 0x0, sizeof(char)*length+1);
		fread(text, sizeof(char), length, file);
		fclose(file);
	}
	else
	{
		fclose(file);
		return NULL;
	}

	config = elf_create_config();

	pos = 0;
	scope = 0;
	while((str = elf_read_next(text, &pos)))
	{
		if(scope > 0)
		{
			if(!strcmp(str, "{"))
			{
				scope++;
			}
			else if(!strcmp(str, "}"))
			{
				scope--;
			}
		}
		else
		{
			if(!strcmp(str, "window_size"))
			{
				elf_read_sst_ints(text, &pos, 2, config->window_size);
			}
			else if(!strcmp(str, "fullscreen"))
			{
				config->fullscreen = elf_read_sst_bool(text, &pos);
			}
			else if(!strcmp(str, "texture_anisotropy"))
			{
				config->texture_anisotropy = elf_read_sst_float(text, &pos);
			}
			else if(!strcmp(str, "shadow_map_size"))
			{
				config->shadow_map_size = elf_read_sst_int(text, &pos);
			}
			else if(!strcmp(str, "start"))
			{
				if(config->start) elf_destroy_string(config->start);
				config->start = elf_read_sst_string(text, &pos);
			}
			else if(!strcmp(str, "log"))
			{
				if(config->log) elf_destroy_string(config->log);
				config->log = elf_read_sst_string(text, &pos);
			}
			else if(!strcmp(str, "{"))
			{
				scope++;
			}
			else if(!strcmp(str, "}"))
			{
				scope--;
				if(scope < 0)
				{
					elf_destroy_string(str);
					break;
				}
			}
			else
			{
				printf("warning: unknown element \"%s\" in config.txt\n", str);
			}
		}
		elf_destroy_string(str);
		str = NULL;
	}

	free(text);

	return config;
}

int elf_get_config_window_width(elf_config *config)
{
	return config->window_size[0];
}

int elf_get_config_window_height(elf_config *config)
{
	return config->window_size[1];
}

unsigned char elf_get_config_fullscreen(elf_config *config)
{
	return !config->fullscreen == ELF_FALSE;
}

float elf_get_config_texture_anisotropy(elf_config *config)
{
	return config->texture_anisotropy;
}

int elf_get_config_shadow_map_size(elf_config *config)
{
	return config->shadow_map_size;
}

const char* elf_get_config_start(elf_config *config)
{
	return config->start;
}

const char* elf_get_config_log(elf_config *config)
{
	return config->start;
}

