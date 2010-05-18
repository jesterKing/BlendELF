
void elf_start_log(const char *text)
{
	FILE *file;

	printf("%s", text);

	file = fopen("elf.log", "w");
	if(file)
	{
		fwrite(text, sizeof(char), strlen(text), file);
		fclose(file);
	}
}

void elf_write_to_log(const char *fmt, ...)
{
	va_list list;
	const char *p, *s;
	int d;
	double f;
	FILE *file;

	va_start(list, fmt);

	file = fopen("elf.log", "a");
	if(!file) fopen("elf.log", "w");

	for(p = fmt; *p; ++p)
	{
		if(*p != '%')
		{
			putc(*p, stdout);
			if(file) putc(*p, file);
		}
		else
		{
			switch(*++p)
			{
				case 's':
					s = va_arg(list, char*);
					printf("%s", s);
					if(file) fprintf(file, "%s", s);
					continue;
				case 'd':
					d = va_arg(list, int);
					printf("%d", d);
					if(file) fprintf(file, "%d", d);
					continue;
				case 'f':
					f = va_arg(list, double);
					printf("%f", f);
					if(file) fprintf(file, "%f", f);
					continue;
			}
		}
	}

	if(file) fclose(file);
}

void elf_set_error(int code, const char *fmt, ...)
{
	va_list list;
	const char *p, *s;
	int d;
	double f;
	char* err_str;
	char* tmp_str;
	char num[32];
	FILE *file;

	va_start(list, fmt);

	file = fopen("elf.log", "a");
	if(!file) fopen("elf.log", "w");

	err_str = elf_create_string("");

	for(p = fmt; *p; ++p)
	{
		if(*p != '%')
		{
			putc(*p, stdout);
			if(file) putc(*p, file);
			tmp_str = elf_append_char_to_string(err_str, *p);
			elf_destroy_string(err_str);
			err_str = tmp_str;
		}
		else
		{
			switch(*++p)
			{
				case 's':
					s = va_arg(list, char*);
					printf("%s", s);
					if(file) fprintf(file, "%s", s);

					tmp_str = elf_merge_strings(err_str, s);
					elf_destroy_string(err_str);
					err_str = tmp_str;
					continue;
				case 'd':
					d = va_arg(list, int);
					printf("%d", d);
					if(file) fprintf(file, "%d", d);

					memset(num, 0x0, sizeof(char)*32);
					sprintf(num, "%d", d);

					tmp_str = elf_merge_strings(err_str, num);
					elf_destroy_string(err_str);
					err_str = tmp_str;
					continue;
				case 'f':
					f = va_arg(list, double);
					printf("%f", f);
					if(file) fprintf(file, "%f", f);

					memset(num, 0x0, sizeof(char)*32);
					sprintf(num, "%f", f);

					tmp_str = elf_merge_strings(err_str, num);
					elf_destroy_string(err_str);
					err_str = tmp_str;
					continue;
			}
		}
	}

	elf_err_code = code;

	if(elf_err_str) elf_destroy_string(elf_err_str);
	elf_err_str = elf_create_string(err_str);
	elf_destroy_string(err_str);

	if(file) fclose(file);
}

elf_game_config* elf_create_game_config()
{
	elf_game_config *config;

	config = (elf_game_config*)malloc(sizeof(elf_game_config));
	memset(config, 0x0, sizeof(elf_game_config));
	config->type = ELF_GAME_CONFIG;

	global_obj_count--;

	return config;
}

void elf_destroy_game_config(elf_game_config *config)
{
	if(config->start) elf_destroy_string(config->start);

	free(config);

	global_obj_count++;
}

elf_game_config* elf_read_game_config(const char *file_path)
{
	elf_game_config *config;
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

	config = elf_create_game_config();

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
				elf_write_to_log("warning: unknown element \"%s\" in config.txt\n", str);
			}
		}
		elf_destroy_string(str);
		str = NULL;
	}

	free(text);

	return config;
}

int elf_get_game_config_window_width(elf_game_config *config)
{
	return config->window_size[0];
}

int elf_get_game_config_window_height(elf_game_config *config)
{
	return config->window_size[1];
}

unsigned char elf_get_game_config_fullscreen(elf_game_config *config)
{
	return !config->fullscreen == ELF_FALSE;
}

float elf_get_game_config_texture_anisotropy(elf_game_config *config)
{
	return config->texture_anisotropy;
}

int elf_get_game_config_shadow_map_size(elf_game_config *config)
{
	return config->shadow_map_size;
}

const char* elf_get_game_config_start(elf_game_config *config)
{
	return config->start;
}

elf_engine* elf_create_engine()
{
	elf_engine *engine;

	if(!gfx_init()) return NULL;

	engine = (elf_engine*)malloc(sizeof(elf_engine));
	memset(engine, 0x0, sizeof(elf_engine));
	engine->type = ELF_ENGINE;

	engine->fps_timer = elf_create_timer();
	engine->fps_limit_timer = elf_create_timer();
	engine->time_sync_timer = elf_create_timer();

	elf_inc_ref((elf_object*)engine->fps_timer);
	elf_inc_ref((elf_object*)engine->fps_limit_timer);
	elf_inc_ref((elf_object*)engine->time_sync_timer);

	engine->free_run = ELF_TRUE;
	engine->fps_limit = 0;
	engine->speed = 1.0;
	engine->f10_exit = ELF_TRUE;

	engine->shadow_map_size = 1024;
	engine->shadow_map = gfx_create_2d_texture(1024, 1024, 0.0, GFX_CLAMP, GFX_LINEAR, GFX_DEPTH_COMPONENT, GFX_DEPTH_COMPONENT, GFX_UBYTE, NULL);
	engine->shadow_target = gfx_create_render_target(1024, 1024);
	gfx_set_render_target_depth_texture(engine->shadow_target, engine->shadow_map);
	engine->texture_anisotropy = 1.0;
	engine->occlusion_culling = ELF_TRUE;

	engine->lines = gfx_create_vertex_data(256, GFX_FLOAT, GFX_VERTEX_DATA_DYNAMIC);
	gfx_inc_ref((gfx_object*)engine->lines);

	return engine;
}

void elf_destroy_engine(elf_engine *engine)
{
	gfx_dec_ref((gfx_object*)engine->lines);

	if(engine->scene) elf_dec_ref((elf_object*)engine->scene);
	if(engine->gui) elf_dec_ref((elf_object*)engine->gui);

	if(engine->post_process) elf_destroy_post_process(engine->post_process);
	gfx_destroy_render_target(engine->shadow_target);
	gfx_destroy_texture(engine->shadow_map);

	elf_dec_ref((elf_object*)engine->fps_timer);
	elf_dec_ref((elf_object*)engine->fps_limit_timer);
	elf_dec_ref((elf_object*)engine->time_sync_timer);

	free(engine);

	gfx_deinit();
}

unsigned char elf_init_engine()
{
	if(eng)
	{
		elf_set_error(ELF_CANT_INITIALIZE, "error: can't initialize the engine twice!\n");
		return ELF_FALSE;
	}

	eng = elf_create_engine();
	if(!eng) return ELF_FALSE;
	elf_inc_ref((elf_object*)eng);

	return ELF_TRUE;
}

void elf_deinit_engine()
{
	if(!eng) return;

	elf_dec_ref((elf_object*)eng);
	eng = NULL;
}

unsigned char elf_init(int width, int height,
	const char *title, unsigned char fullscreen)
{
	elf_start_log("BlendELF 0.9 Beta\n");

	elf_init_objects();
	if(!elf_init_context(width, height, title, fullscreen)) return ELF_FALSE;
	if(!elf_init_audio());
	if(!elf_init_engine()) return ELF_FALSE;
	if(!elf_init_scripting()) return ELF_FALSE;
	if(!elf_init_networking()) return ELF_FALSE;

	return ELF_TRUE;
}

#if defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__)
	#ifndef ELF_PLAYER
unsigned char elf_init_with_hwnd(int width, int height,
	const char *title, unsigned char fullscreen, HWND hwnd)
{
	elf_init_objects();
	if(!elf_init_context_with_hwnd(width, height, title, fullscreen, hwnd)) return ELF_FALSE;
	if(!elf_init_audio());
	if(!elf_init_engine()) return ELF_FALSE;
	if(!elf_init_scripting()) return ELF_FALSE;
	if(!elf_init_networking()) return ELF_FALSE;

	return ELF_TRUE;
}
	#endif
#endif

unsigned char elf_init_with_config(const char *file_path)
{
	elf_game_config *config;

	if(!(config = elf_read_game_config("config.txt")))
	{
		config = elf_create_game_config();
		config->window_size[0] = 1024;
		config->window_size[1] = 768;
		config->fullscreen = ELF_FALSE;
		config->texture_anisotropy = 1.0f;
		config->shadow_map_size = 1024;
		config->start = elf_create_string("game.pak");
	}

	if(!elf_init(config->window_size[0], config->window_size[1], "BlendELF", !config->fullscreen == ELF_FALSE))
	{
		elf_set_error(ELF_CANT_INITIALIZE, "error: could not initialize engine\n");
		elf_destroy_game_config(config);
		return ELF_FALSE;
	}

	elf_set_texture_anisotropy(config->texture_anisotropy);
	elf_set_shadow_map_size(config->shadow_map_size);

	elf_destroy_game_config(config);

	return ELF_TRUE;
}

void elf_limit_engine_fps()
{
	if(eng->fps_limit > 0)
	{
		if(elf_get_elapsed_time(eng->fps_limit_timer) > 0.0)
		{
			while(elf_get_elapsed_time(eng->fps_limit_timer) < 1.0/(float)eng->fps_limit);
			elf_start_timer(eng->fps_limit_timer);
		}
		else
		{
			elf_start_timer(eng->fps_limit_timer);
		}
	}
}

void elf_update_engine()
{
	elf_update_audio();

	if(elf_get_elapsed_time(eng->time_sync_timer) > 0.0)
	{
		if(elf_about_zero(eng->tick_rate))
			eng->sync = (eng->sync*4.0+((float)elf_get_elapsed_time(eng->time_sync_timer)*eng->speed))/5.0;
		else eng->sync = eng->tick_rate;

		elf_start_timer(eng->time_sync_timer);

		if(eng->sync > 0.0)
		{
			if(eng->gui) elf_update_gui(eng->gui, eng->sync);

			if(eng->scene)
			{
				elf_update_scene(eng->scene, eng->sync);
			}
		}
	}
	else
	{
		elf_start_timer(eng->time_sync_timer);
	}
}

void elf_count_engine_fps()
{
	eng->frames++;

	if(elf_get_elapsed_time(eng->fps_timer) > 0.0)
	{
		if(elf_get_elapsed_time(eng->fps_timer) >= 1.0)
		{
			eng->fps = eng->frames;
			eng->frames = 0;
			elf_start_timer(eng->fps_timer);
		}
	}
	else
	{
		elf_start_timer(eng->fps_timer);
	}
}

unsigned char elf_run()
{
	if(!eng || !eng->free_run) return ELF_FALSE;

	eng->free_run = ELF_FALSE;

	if((eng->f10_exit && elf_get_key_state(ELF_KEY_F10)) || !elf_is_window_opened() || eng->quit)
	{
		eng->free_run = ELF_TRUE;
		return ELF_FALSE;
	}

	gfx_reset_vertices_drawn();

	if(eng->scene && eng->post_process) elf_begin_post_process(eng->post_process, eng->scene);
	else gfx_clear_buffers(0.0, 0.0, 0.0, 0.0, 1.0);

	if(eng->scene) elf_draw_scene(eng->scene);
	if(eng->scene && eng->post_process) elf_end_post_process(eng->post_process, eng->scene);
	if(eng->scene && eng->debug_draw) elf_draw_scene_debug(eng->scene);
	if(eng->gui) elf_draw_gui(eng->gui);

	elf_swap_buffers();

	elf_limit_engine_fps();
	elf_update_engine();
	elf_count_engine_fps();

	eng->free_run = ELF_TRUE;

	return ELF_TRUE;
}

void elf_deinit()
{
	elf_deinit_networking();
	elf_deinit_scripting();
	elf_deinit_engine();
	elf_deinit_audio();
	elf_deinit_context();
	if(elf_err_str) elf_destroy_string(elf_err_str);
	if(elf_err_str_store) elf_destroy_string(elf_err_str_store);
	elf_deinit_objects();
	elf_err_str = NULL;
	elf_err_str_store = NULL;
}

int elf_get_version_major()
{
	return 0;
}

int elf_get_version_minor()
{
	return 91;
}

const char* elf_get_version_release()
{
	return "Beta";
}

const char* elf_get_version()
{
	return "BlendELF 0.91 Beta";
}

const char* elf_get_error_string()
{
	if(elf_err_str_store) elf_destroy_string(elf_err_str_store);
	elf_err_str_store = elf_err_str;
	elf_err_str = NULL;
	return elf_err_str_store;
}

int elf_get_error()
{
	int err;
	err = elf_err_code;
	return elf_err_code;
}

void elf_quit()
{
	eng->quit = ELF_TRUE;
}

void elf_set_f10_exit(unsigned char exit)
{
	eng->f10_exit = !(exit == ELF_FALSE);
}

unsigned char elf_get_f10_exit()
{
	return eng->f10_exit;
}

elf_scene* elf_load_scene(const char *file_path)
{
	elf_scene *scene;

	scene = elf_create_scene_from_file(file_path);
	if(scene)
	{
		if(eng->scene) elf_dec_ref((elf_object*)eng->scene);
		eng->scene = scene;
		elf_inc_ref((elf_object*)eng->scene);
	}

	return scene;
}

void elf_set_scene(elf_scene *scene)
{
	if(eng->scene) elf_dec_ref((elf_object*)eng->scene);
	eng->scene = scene;
	if(eng->scene) elf_inc_ref((elf_object*)eng->scene);
}

elf_scene* elf_get_scene()
{
	return eng->scene;
}

void elf_set_gui(elf_gui *gui)
{
	if(eng->gui) elf_dec_ref((elf_object*)eng->gui);
	eng->gui = gui;
	if(eng->gui) elf_inc_ref((elf_object*)eng->gui);
}

elf_gui* elf_get_gui()
{
	return eng->gui;
}

float elf_get_sync()
{
	return eng->sync;
}

int elf_get_fps()
{
	return eng->fps;
}

void elf_set_fps_limit(int fps_limit)
{
	eng->fps_limit = fps_limit;
	if(eng->fps_limit < 0) eng->fps_limit = 0;
}

int elf_get_fps_limit()
{
	return eng->fps_limit;
}

void elf_set_tick_rate(float tick_rate)
{
	eng->tick_rate = tick_rate;
	if(eng->tick_rate < 0.0) eng->tick_rate = 0.0;
}

float elf_get_tick_rate()
{
	return eng->tick_rate;
}

void elf_set_speed(float speed)
{
	eng->speed = speed;
	if(eng->speed < 0.0001) eng->speed = 0.0001;
}

float elf_get_speed()
{
	return eng->speed;
}

unsigned char elf_save_screen_shot(const char *file_path)
{
	unsigned char *data;

	data = (unsigned char*)malloc(sizeof(unsigned char)*elf_get_window_width()*elf_get_window_height()*3);

	gfx_read_pixels(0, 0, elf_get_window_width(), elf_get_window_height(), GFX_BGR, GFX_UBYTE, data);

	if(!elf_save_image_data(file_path, elf_get_window_width(), elf_get_window_height(), 24, data))
	{
		free(data);
		return ELF_FALSE;
	}

	free(data);

	return ELF_TRUE;
}

void elf_set_texture_anisotropy(float anisotropy)
{
	eng->texture_anisotropy = anisotropy;
}

float elf_get_texture_anisotropy()
{
	return eng->texture_anisotropy;
}

void elf_set_shadow_map_size(int size)
{
	// why would someone want a shadow map of 1 pixel?...
	if(size < 1) return;

	gfx_destroy_render_target(eng->shadow_target);
	gfx_destroy_texture(eng->shadow_map);

	eng->shadow_map_size = size;
	eng->shadow_map = gfx_create_2d_texture(size, size, 0.0f, GFX_CLAMP, GFX_LINEAR, GFX_DEPTH_COMPONENT, GFX_DEPTH_COMPONENT, GFX_UBYTE, NULL);
	eng->shadow_target = gfx_create_render_target(size, size);
	gfx_set_render_target_depth_texture(eng->shadow_target, eng->shadow_map);
}

int elf_get_shadow_map_size()
{
	return eng->shadow_map_size;
}

int elf_get_polygons_rendered()
{
	return gfx_get_vertices_drawn(GFX_TRIANGLES)/3+gfx_get_vertices_drawn(GFX_TRIANGLE_STRIP)/3;
}

void elf_set_bloom(float threshold)
{
	if(!eng->post_process) eng->post_process = elf_create_post_process();
	elf_set_post_process_bloom(eng->post_process, threshold);
}

void elf_disable_bloom()
{
	if(eng->post_process)
	{
		elf_disable_post_process_bloom(eng->post_process);
		if(!elf_is_post_process_bloom(eng->post_process) &&
			!elf_is_post_process_ssao(eng->post_process) &&
			!elf_is_post_process_dof(eng->post_process) &&
			!elf_is_post_process_light_shafts(eng->post_process))
		{
			elf_destroy_post_process(eng->post_process);
			eng->post_process = NULL;
		}
	}
}

float elf_get_bloom_threshold()
{
	if(!eng->post_process) return ELF_FALSE;
	return elf_get_post_process_bloom_threshold(eng->post_process);
}

void elf_set_dof(float focal_range, float focal_distance)
{
	if(!eng->post_process) eng->post_process = elf_create_post_process();
	elf_set_post_process_dof(eng->post_process, focal_range, focal_distance);
}

void elf_disable_dof()
{
	if(eng->post_process)
	{
		elf_disable_post_process_dof(eng->post_process);
		if(!elf_is_post_process_bloom(eng->post_process) &&
			!elf_is_post_process_ssao(eng->post_process) &&
			!elf_is_post_process_dof(eng->post_process) &&
			!elf_is_post_process_light_shafts(eng->post_process))
		{
			elf_destroy_post_process(eng->post_process);
			eng->post_process = NULL;
		}
	}
}

float elf_get_dof_focal_range()
{
	if(!eng->post_process) return ELF_FALSE;
	return elf_get_post_process_dof_focal_range(eng->post_process);
}

float elf_get_dof_focal_distance()
{
	if(!eng->post_process) return ELF_FALSE;
	return elf_get_post_process_dof_focal_distance(eng->post_process);
}

void elf_set_ssao(float amount)
{
	if(!eng->post_process) eng->post_process = elf_create_post_process();
	elf_set_post_process_ssao(eng->post_process, amount);
}

void elf_disable_ssao()
{
	if(eng->post_process)
	{
		elf_disable_post_process_ssao(eng->post_process);
		if(!elf_is_post_process_bloom(eng->post_process) &&
			!elf_is_post_process_ssao(eng->post_process) &&
			!elf_is_post_process_dof(eng->post_process) &&
			!elf_is_post_process_light_shafts(eng->post_process))
		{
			elf_destroy_post_process(eng->post_process);
			eng->post_process = NULL;
		}
	}
}

float elf_get_ssao_amount()
{
	if(eng->post_process) return elf_get_post_process_ssao_amount(eng->post_process);
	return 0.0;
}

void elf_set_light_shafts(float intensity)
{
	if(!eng->post_process) eng->post_process = elf_create_post_process();
	elf_set_post_process_light_shafts(eng->post_process, intensity);
}

void elf_disable_light_shafts()
{
	if(eng->post_process)
	{
		elf_disable_post_process_light_shafts(eng->post_process);
		if(!elf_is_post_process_bloom(eng->post_process) &&
			!elf_is_post_process_ssao(eng->post_process) &&
			!elf_is_post_process_dof(eng->post_process) &&
			!elf_is_post_process_light_shafts(eng->post_process))
		{
			elf_destroy_post_process(eng->post_process);
			eng->post_process = NULL;
		}
	}
}

float elf_get_light_shafts_inteisity()
{
	if(eng->post_process) return elf_get_post_process_light_shafts_intensity(eng->post_process);
	return 0.0;
}

unsigned char elf_is_bloom()
{
	if(eng->post_process) return elf_is_post_process_bloom(eng->post_process);
	return ELF_FALSE;
}

unsigned char elf_is_ssao()
{
	if(eng->post_process) return elf_is_post_process_ssao(eng->post_process);
	return ELF_FALSE;
}

unsigned char elf_is_dof()
{
	if(eng->post_process) return elf_is_post_process_dof(eng->post_process);
	return ELF_FALSE;
}

unsigned char elf_is_light_shafts()
{
	if(eng->post_process) return elf_is_post_process_light_shafts(eng->post_process);
	return ELF_FALSE;
}

void elf_set_occlusion_culling(unsigned char cull)
{
	eng->occlusion_culling = !cull == ELF_FALSE;
}

unsigned char elf_is_occlusion_culling()
{
	return eng->occlusion_culling;
}

void elf_set_debug_draw(unsigned char debug_draw)
{
	eng->debug_draw = !debug_draw == ELF_FALSE;
}

unsigned char elf_is_debug_draw()
{
	return eng->debug_draw;
}

elf_object* elf_get_actor()
{
	return eng->actor;
}

elf_directory* elf_create_directory()
{
	elf_directory *directory;

	directory = (elf_directory*)malloc(sizeof(elf_directory));
	memset(directory, 0x0, sizeof(elf_directory));
	directory->type = ELF_DIRECTORY;

	directory->items = elf_create_list();

	global_obj_count++;

	return directory;
}

elf_directory_item* elf_create_directory_item()
{
	elf_directory_item *dir_item;

	dir_item = (elf_directory_item*)malloc(sizeof(elf_directory_item));
	memset(dir_item, 0x0, sizeof(elf_directory_item));
	dir_item->type = ELF_DIRECTORY_ITEM;

	global_obj_count++;

	return dir_item;
}

void elf_destroy_directory(elf_directory *directory)
{
	if(directory->path) elf_destroy_string(directory->path);
	elf_destroy_list(directory->items);

	free(directory);

	global_obj_count--;
}

void elf_destroy_directory_item(elf_directory_item *directory_item)
{
	if(directory_item->name) elf_destroy_string(directory_item->name);

	free(directory_item);

	global_obj_count--;
}

void elf_append_folder_to_directory_item_list(elf_list *items, elf_directory_item *nitem)
{
	elf_directory_item *dir_item;
	int i;

	for(i = 0, dir_item = (elf_directory_item*)elf_begin_list(items); dir_item;
		dir_item = (elf_directory_item*)elf_next_in_list(items), i++)
	{
		if(dir_item->item_type == ELF_FILE)
		{
			elf_insert_to_list(items, i, (elf_object*)nitem);
			return;
		}
		else continue;
	}

	elf_append_to_list(items, (elf_object*)nitem);
}

elf_directory* elf_read_directory(const char *path)
{
#if defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__)
	/*elf_directory *directory;
	elf_directory_item *dir_item;
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	size_t length_of_arg;
	TCHAR szDir[MAX_PATH];
	int i;

	StringCbLength(path, MAX_PATH, &length_of_arg);
	if (length_of_arg > (MAX_PATH - 3))
	{
		elf_set_error(ELF_CANT_OPEN_DIRECTORY, error: can't open directory, \"%s\". directory path is too long\n", path);
		return NULL;
	}

	StringCbCopyN(szDir, MAX_PATH, path, length_of_arg+1);
	StringCbCatN(szDir, MAX_PATH, "\\*", 3);

	hFind = FindFirstFile(szDir, &ffd);

	if(hFind == INVALID_HANDLE_VALUE) 
	{
		elf_set_error(ELF_CANT_OPEN_DIRECTORY, "error: can't open directory \"%s\"\n", path);
		return NULL;
	}

	directory = elf_create_directory();

	do
	{
		dir_item = elf_create_directory_item();
		StringCbLength(ffd.cFileName, MAX_PATH, &length_of_arg);
		dir_item->name = malloc(length_of_arg+1);
		memset(dir_item->name, 0x0, sizeof(char)*(length_of_arg+1));
		for(i = 0; i < length_of_arg; i++) dir_item->name[i] = (char)ffd.cFileName[i];
		global_obj_count++;
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			dir_item->item_type = ELF_DIR;
			elf_append_folder_to_directory_item_list(directory->items, dir_item);
		}
		else
		{
			dir_item->item_type = ELF_FILE;
			elf_append_to_list(directory->items, (elf_object*)dir_item);
		}
	}
	while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES) 
	{
		elf_set_error(ELF_LOL, "error: something happened during listing the directory \"%s\"... dunno what though :D\n", path);
	}

	FindClose(hFind);

	return directory;*/
	elf_set_error(ELF_MISSING_FEATURE, "error: file listing not supported on windows yet\n");
#else
	elf_directory *directory;
	elf_directory_item *dir_item;
	int num_entries, i;
	struct dirent **namelist, **list;

	if((num_entries = scandir(path, &namelist, NULL, alphasort)) < 0)
	{
		elf_set_error(ELF_CANT_OPEN_DIRECTORY, "error: could not open directory \"%s\"\n", path);
		return NULL;
	}
	else
	{
		directory = elf_create_directory();
		directory->path = elf_create_string(path);
		for(i = 0, list = namelist; i < num_entries; i++, list++)
		{
			dir_item = elf_create_directory_item();
			dir_item->name = elf_create_string((*list)->d_name);
			if((*list)->d_type == 4)
			{
				dir_item->item_type = ELF_DIR;
				elf_append_folder_to_directory_item_list(directory->items, dir_item);
			}
			else
			{
				dir_item->item_type = ELF_FILE;
				elf_append_to_list(directory->items, (elf_object*)dir_item);
			}
			free(*list);
		}
		free(namelist);
	}

	return directory;
#endif
}

const char* elf_get_directory_path(elf_directory *directory)
{
	return directory->path;
}

int elf_get_directory_item_count(elf_directory *directory)
{
	return elf_get_list_length(directory->items);
}

elf_directory_item* elf_get_directory_item(elf_directory *directory, int idx)
{
	int i;
	elf_directory_item *dir_item;

	if(idx < 0 || idx > elf_get_list_length(directory->items)-1) return NULL;

	for(i = 0, dir_item = (elf_directory_item*)elf_begin_list(directory->items); dir_item;
		dir_item = (elf_directory_item*)elf_next_in_list(directory->items), i++)
	{
		if(i == idx) return dir_item;
	}

	return NULL;
}

const char* elf_get_directory_item_name(elf_directory_item *dir_item)
{
	return dir_item->name;
}

int elf_get_directory_item_type(elf_directory_item *dir_item)
{
	return dir_item->item_type;
}

elf_vec3f elf_create_vec3f()
{
	elf_vec3f result;
	memset(&result, 0x0, sizeof(elf_vec3f));
	return result;
}

elf_vec3f elf_create_vec3f_from_values(float x, float y, float z)
{
	elf_vec3f result;
	memset(&result, 0x0, sizeof(elf_vec3f));

	result.x = x;
	result.y = y;
	result.z = z;

	return result;
}

elf_vec4f elf_create_qua()
{
	elf_vec4f result;
	memset(&result, 0x0, sizeof(elf_vec4f));
	return result;
}

elf_vec4f elf_create_qua_from_euler(float x, float y, float z)
{
	elf_vec4f result;
	memset(&result, 0x0, sizeof(elf_vec4f));

	gfx_set_qua_rotation(x, y, z, &result.x);

	return result;
}

elf_vec3f elf_mul_qua_vec3f(elf_vec4f qua, elf_vec3f vec)
{
	elf_vec3f result;

	gfx_mul_qua_vec(&qua.x, &vec.x, &result.x);

	return result;
}

elf_vec4f elf_mul_qua_qua(elf_vec4f qua1, elf_vec4f qua2)
{
	elf_vec4f result;

	gfx_mul_qua_qua(&qua1.x, &qua2.x, &result.x);

	return result;
}

elf_vec4f elf_get_qua_inverted(elf_vec4f qua)
{
	elf_vec4f result;

	gfx_qua_get_inverse(&qua.x, &result.x);

	return result;
}

elf_vec3f elf_sub_vec3f_vec3f(elf_vec3f vec1, elf_vec3f vec2)
{
	elf_vec3f result;
	result.x = vec1.x-vec2.x;
	result.y = vec1.y-vec2.y;
	result.z = vec1.z-vec2.z;
	return result;
}

elf_vec3f elf_add_vec3f_vec3f(elf_vec3f vec1, elf_vec3f vec2)
{
	elf_vec3f result;
	result.x = vec1.x+vec2.x;
	result.y = vec1.y+vec2.y;
	result.z = vec1.z+vec2.z;
	return result;
}

float elf_get_vec3f_length(elf_vec3f vec)
{
	return gfx_vec_length(&vec.x);
}

unsigned char elf_about_zero(float val)
{
	if(val < 0.0001 && val > -0.0001) return ELF_TRUE;
	return ELF_FALSE;
}

float elf_float_abs(float val)
{
	if(val < 0.0) return -val;
	return val;
}

float elf_float_max(float a, float b)
{
	if(a > b) return a;
	return b;
}

float elf_random_float()
{
	return (float)rand()/(float)RAND_MAX;
}

float elf_random_float_range(float min, float max)
{
	return min+(max-min)*elf_random_float();
}

int elf_random_int()
{
	return rand();
}

int elf_random_int_range(int min, int max)
{
	return (int)(((float)(min+(max-min)))*elf_random_float());
}

