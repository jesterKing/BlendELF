
gfx_shader_program* gfx_create_shader_program(const char* vertex, const char* fragment)
{
	const GLchar* my_string_ptrs[1];
	int shader_length;
	int success;
	char *info_log;
	int info_log_length;
	GLuint my_vertex_shader;
	GLuint my_fragment_shader;
	gfx_shader_program *shader_program;

	my_vertex_shader = glCreateShader(GL_VERTEX_SHADER);

	if(vertex)
	{
		my_string_ptrs[0] = vertex;
		shader_length = strlen(vertex);

		glShaderSource(my_vertex_shader, 1, (const GLchar**)my_string_ptrs, &shader_length);
		glCompileShader(my_vertex_shader);

		glGetShaderiv(my_vertex_shader, GL_COMPILE_STATUS, &success);

		if(!success)
		{
			glGetShaderiv(my_vertex_shader, GL_INFO_LOG_LENGTH, &info_log_length);

			info_log = (char*)malloc(sizeof(char)*(info_log_length+1));
			memset(info_log, '\0', sizeof(char)*(info_log_length+1));

			glGetShaderInfoLog(my_vertex_shader, info_log_length, 0, info_log);
			gfx_write_to_log("error: could not compile vertex shader, log message:\n%s", info_log);
			glDeleteShader(my_vertex_shader);

			free(info_log);

			return NULL;
		}
	}

	my_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	if(fragment)
	{
		my_string_ptrs[0] = fragment;
		shader_length = strlen(fragment);

		glShaderSource(my_fragment_shader, 1, (const GLchar**)my_string_ptrs, &shader_length);
		glCompileShader(my_fragment_shader);

		glGetShaderiv(my_fragment_shader, GL_COMPILE_STATUS, &success);

		if(!success)
		{
			glGetShaderiv(my_fragment_shader, GL_INFO_LOG_LENGTH, &info_log_length);

			info_log = (char*)malloc(sizeof(char)*(info_log_length+1));
			memset(info_log, '\0', sizeof(char)*(info_log_length+1));

			glGetShaderInfoLog(my_fragment_shader, info_log_length, 0, info_log);
			gfx_write_to_log("error: could not compile fragment shader, log message:\n%s", info_log);

			if(vertex) glDeleteShader(my_vertex_shader);
			glDeleteShader(my_fragment_shader);

			free(info_log);

			return NULL;
		}
	}

	shader_program = (gfx_shader_program*)malloc(sizeof(gfx_shader_program));
	memset(shader_program, 0x0, sizeof(gfx_shader_program));

	shader_program->id = glCreateProgram();

	if(vertex) glAttachShader(shader_program->id, my_vertex_shader);
	if(fragment) glAttachShader(shader_program->id, my_fragment_shader);

	glBindAttribLocation(shader_program->id, GFX_VERTEX, "elf_VertexAttr");
	glBindAttribLocation(shader_program->id, GFX_NORMAL, "elf_NormalAttr");
	glBindAttribLocation(shader_program->id, GFX_TEX_COORD, "elf_TexCoordAttr");
	glBindAttribLocation(shader_program->id, GFX_COLOR, "elf_ColorAttr");
	glBindAttribLocation(shader_program->id, GFX_TANGENT, "elf_TangentAttr");

	glLinkProgram(shader_program->id);

	if(vertex) glDeleteShader(my_vertex_shader);
	if(fragment) glDeleteShader(my_fragment_shader);

	glGetProgramiv(shader_program->id, GL_VALIDATE_STATUS, &success);

	if(!success)
	{
		glGetProgramiv(shader_program->id, GL_INFO_LOG_LENGTH, &info_log_length);

		info_log = (char*)malloc(sizeof(char)*(info_log_length+1));
		memset(info_log, '\0', sizeof(char)*(info_log_length+1));

		glGetProgramInfoLog(shader_program->id, info_log_length, 0, info_log);
		gfx_write_to_log("error: validating shader program failed, log message:\n%s", info_log);

		gfx_destroy_shader_program(shader_program);

		return NULL;
	}

	shader_program->projection_matrix_loc = glGetUniformLocation(shader_program->id, "elf_ProjectionMatrix");
	shader_program->modelview_matrix_loc = glGetUniformLocation(shader_program->id, "elf_ModelviewMatrix");
	shader_program->texture0_loc = glGetUniformLocation(shader_program->id, "elf_Texture0");
	shader_program->texture1_loc = glGetUniformLocation(shader_program->id, "elf_Texture1");
	shader_program->texture2_loc = glGetUniformLocation(shader_program->id, "elf_Texture2");
	shader_program->texture3_loc = glGetUniformLocation(shader_program->id, "elf_Texture3");
	shader_program->color_map_loc = glGetUniformLocation(shader_program->id, "elf_ColorMap");
	shader_program->normal_map_loc = glGetUniformLocation(shader_program->id, "elf_NormalMap");
	shader_program->height_map_loc = glGetUniformLocation(shader_program->id, "elf_HeightMap");
	shader_program->specular_map_loc = glGetUniformLocation(shader_program->id, "elf_SpecularMap");
	shader_program->color_ramp_map_loc = glGetUniformLocation(shader_program->id, "elf_ColorRampMap");
	shader_program->light_map_loc = glGetUniformLocation(shader_program->id, "elf_LightMap");
	shader_program->shadow_projection_matrix_loc = glGetUniformLocation(shader_program->id, "elf_ShadowProjectionMatrix");
	shader_program->shadow_map_loc = glGetUniformLocation(shader_program->id, "elf_ShadowMap");
	shader_program->color_loc = glGetUniformLocation(shader_program->id, "elf_Color");
	shader_program->specular_color_loc = glGetUniformLocation(shader_program->id, "elf_SpecularColor");
	shader_program->spec_power_loc = glGetUniformLocation(shader_program->id, "elf_SpecPower");
	shader_program->light_position_loc = glGetUniformLocation(shader_program->id, "elf_LightPosition");
	shader_program->light_color_loc = glGetUniformLocation(shader_program->id, "elf_LightColor");
	shader_program->light_spot_direction_loc = glGetUniformLocation(shader_program->id, "elf_LightSpotDirection");
	shader_program->light_distance_loc = glGetUniformLocation(shader_program->id, "elf_LightDistance");
	shader_program->light_fade_speed_loc = glGetUniformLocation(shader_program->id, "elf_LightFadeSpeed");
	shader_program->light_inner_cone_cos_loc = glGetUniformLocation(shader_program->id, "elf_LightInnerConeCos");
	shader_program->light_outer_cone_cos_loc = glGetUniformLocation(shader_program->id, "elf_LightOuterConeCos");
	shader_program->clip_start_loc = glGetUniformLocation(shader_program->id, "elf_ClipStart");
	shader_program->clip_end_loc = glGetUniformLocation(shader_program->id, "elf_ClipEnd");
	shader_program->viewport_width_loc = glGetUniformLocation(shader_program->id, "elf_ViewportWidth");
	shader_program->viewport_height_loc = glGetUniformLocation(shader_program->id, "elf_ViewportHeight");
	shader_program->parallax_scale_loc = glGetUniformLocation(shader_program->id, "elf_ParallaxScale");
	shader_program->alpha_threshold_loc = glGetUniformLocation(shader_program->id, "elf_AlphaThreshold");

	glUseProgram(shader_program->id);

	return shader_program;
}

void gfx_destroy_shader_program(gfx_shader_program *shader_program)
{
	if(shader_program->id) glDeleteShader(shader_program->id);

	free(shader_program);
}

void gfx_destroy_shader_programs(gfx_shader_program *shader_program)
{
	if(shader_program->next) gfx_destroy_shader_programs(shader_program->next);

	gfx_destroy_shader_program(shader_program);
}

void gfx_set_shader_program_uniform_1i(const char *name, int i)
{
	if(!driver->shader_params.shader_program) return;
	glUniform1i(glGetUniformLocation(driver->shader_params.shader_program->id, name), i);
}

void gfx_set_shader_program_uniform_1f(const char *name, float f)
{
	if(!driver->shader_params.shader_program) return;
	glUniform1f(glGetUniformLocation(driver->shader_params.shader_program->id, name), f);
}

void gfx_set_shader_program_uniform_vec2(const char *name, float x, float y)
{
	if(!driver->shader_params.shader_program) return;
	glUniform2f(glGetUniformLocation(driver->shader_params.shader_program->id, name), x, y);
}

void gfx_set_shader_program_uniform_vec3(const char *name, float x, float y, float z)
{
	if(!driver->shader_params.shader_program) return;
	glUniform3f(glGetUniformLocation(driver->shader_params.shader_program->id, name), x, y, z);
}

void gfx_set_shader_program_uniform_vec4(const char *name, float x, float y, float z, float w)
{
	if(!driver->shader_params.shader_program) return;
	glUniform4f(glGetUniformLocation(driver->shader_params.shader_program->id, name), x, y, z, w);
}

void gfx_set_shader_program_uniform_mat4(const char *name, float *matrix)
{
	if(!driver->shader_params.shader_program) return;
	glUniformMatrix4fv(glGetUniformLocation(driver->shader_params.shader_program->id, name), 1, GL_FALSE, matrix);
}

