
void gfx_set_color(gfx_color *color, float r, float g, float b, float a)
{
	color->r = r;
	color->g = g;
	color->b = b;
	color->a = a;
}

void gfx_set_shader_params_default(gfx_shader_params *shader_params)
{
	int i;

	memset(shader_params, 0x0, sizeof(gfx_shader_params));

	shader_params->render_params.depth_test = GFX_TRUE;
	shader_params->render_params.depth_write = GFX_TRUE;
	shader_params->render_params.depth_func = GFX_LESS;
	shader_params->render_params.color_write = GFX_TRUE;
	shader_params->render_params.alpha_write = GFX_TRUE;
	shader_params->render_params.alpha_test = GFX_FALSE;
	shader_params->render_params.alpha_threshold = 0.99;
	shader_params->render_params.alpha_test_in_shader = GFX_FALSE;
	shader_params->render_params.cull_face = GFX_TRUE;
	shader_params->render_params.blend_mode = GFX_NONE;
	shader_params->render_params.offset_bias = 0.0;
	shader_params->render_params.offset_scale = 0.0;
	shader_params->render_params.line_width = 1.0;
	shader_params->render_params.line_smooth = GFX_FALSE;
	shader_params->render_params.cull_face_mode = GFX_BACK;
	shader_params->render_params.front_face = GFX_COUNTER_CLOCK_WISE;
	shader_params->render_params.wireframe = GFX_FALSE;
	shader_params->render_params.vertex_color = GFX_FALSE;

	gfx_set_color(&shader_params->material_params.color, 1.0, 1.0, 1.0, 1.0);
	gfx_set_color(&shader_params->material_params.specular_color, 1.0, 1.0, 1.0, 1.0);
	shader_params->material_params.spec_power = 0.0;

	for(i = 0; i < GFX_MAX_TEXTURES; i++)
	{
		shader_params->texture_params[i].type = GFX_COLOR_MAP;
		shader_params->texture_params[i].texture = NULL;
		shader_params->texture_params[i].projection_mode = GFX_NONE;
		gfx_matrix4_set_identity(shader_params->texture_params[i].matrix);
	}

	gfx_matrix4_set_identity(shader_params->projection_matrix);
	gfx_matrix4_set_identity(shader_params->modelview_matrix);

	shader_params->shader_program = NULL;
}

void gfx_set_material_params_default(gfx_shader_params *shader_params)
{
	gfx_set_color(&shader_params->material_params.color, 1.0, 1.0, 1.0, 1.0);
	gfx_set_color(&shader_params->material_params.specular_color, 1.0, 1.0, 1.0, 1.0);
	shader_params->material_params.spec_power = 0.0;
}

void gfx_set_texture_params_default(gfx_shader_params *shader_params)
{
	int i;

	for(i = 0; i < GFX_MAX_TEXTURES; i++)
	{
		shader_params->texture_params[i].type = GFX_COLOR_MAP;
		shader_params->texture_params[i].texture = NULL;
		shader_params->texture_params[i].projection_mode = GFX_NONE;
		shader_params->texture_params[i].parallax_scale = 0.25;
		gfx_matrix4_set_identity(shader_params->texture_params[i].matrix);
	}
}

void gfx_set_shader_params(gfx_shader_params *shader_params)
{
	int i;
	gfx_shader_config shader_config;
	gfx_shader_program *shader_program;

	if(memcmp(&driver->shader_params.render_params, &shader_params->render_params, sizeof(gfx_render_params)))
	{
		if(shader_params->render_params.depth_test) glEnable(GL_DEPTH_TEST);
		else glDisable(GL_DEPTH_TEST);

		if(shader_params->render_params.depth_write) glDepthMask(1);
		else glDepthMask(0);

		switch(shader_params->render_params.depth_func)
		{
			case GFX_NEVER: glDepthFunc(GL_NEVER); break;
			case GFX_LESS: glDepthFunc(GL_LESS); break;
			case GFX_EQUAL: glDepthFunc(GL_EQUAL); break;
			case GFX_LEQUAL: glDepthFunc(GL_LEQUAL); break;
			case GFX_GREATER: glDepthFunc(GL_GREATER); break;
			case GFX_NOTEQUAL: glDepthFunc(GL_NOTEQUAL); break;
			case GFX_GEQUAL: glDepthFunc(GL_GEQUAL); break;
			case GFX_ALWAYS: glDepthFunc(GL_ALWAYS); break;
		}

		glColorMask(shader_params->render_params.color_write,
			shader_params->render_params.color_write,
			shader_params->render_params.color_write,
			shader_params->render_params.alpha_write);

		if(shader_params->render_params.cull_face) glEnable(GL_CULL_FACE);
		else glDisable(GL_CULL_FACE);

		if(shader_params->render_params.alpha_test && !shader_params->render_params.alpha_test_in_shader)
		{
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, shader_params->render_params.alpha_threshold);
		}
		else glDisable(GL_ALPHA_TEST);

		switch(shader_params->render_params.blend_mode)
		{
			case GFX_NONE:
				glDisable(GL_BLEND);
				break;
			case GFX_TRANSPARENT:
				glEnable(GL_BLEND);
				glBlendEquation(GL_FUNC_ADD);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				break;
			case GFX_ADD:
				glEnable(GL_BLEND);
				glBlendEquation(GL_FUNC_ADD);
				glBlendFunc(GL_ONE, GL_ONE);
				break;
			case GFX_MULTIPLY:
				glEnable(GL_BLEND);
				glBlendEquation(GL_FUNC_ADD);
				glBlendFunc(GL_DST_COLOR, GL_ZERO);
				break;
			case GFX_SUBTRACT:
				glEnable(GL_BLEND);
				glBlendEquation(GL_FUNC_SUBTRACT);
				glBlendFunc(GL_ONE, GL_ONE);
				break;
		}

		if(!shader_params->render_params.offset_scale ||
			!shader_params->render_params.offset_bias)
			glDisable(GL_POLYGON_OFFSET_FILL);
		else
		{
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(shader_params->render_params.offset_scale,
				shader_params->render_params.offset_bias);
		}

		glLineWidth(shader_params->render_params.line_width);
		if(shader_params->render_params.line_smooth)
			glEnable(GL_LINE_SMOOTH);
		else glDisable(GL_LINE_SMOOTH);

		if(shader_params->render_params.cull_face_mode == GFX_BACK)
			glCullFace(GL_BACK);
		else glCullFace(GL_FRONT);

		if(shader_params->render_params.front_face == GFX_COUNTER_CLOCK_WISE)
			glFrontFace(GL_CCW);
		else glFrontFace(GL_CW);

		if(shader_params->render_params.wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if(memcmp(&driver->shader_params.texture_params, &shader_params->texture_params, sizeof(gfx_texture_params)*GFX_MAX_TEXTURES))
	{
		for(i = 0; i < GFX_MAX_TEXTURES; i++)
		{
			glActiveTexture(GL_TEXTURE0+i);
			glClientActiveTexture(GL_TEXTURE0+i);

			if(shader_params->texture_params[i].texture)
			{
				glMatrixMode(GL_TEXTURE);
				glLoadMatrixf(shader_params->texture_params[i].matrix);

				glBindTexture(GL_TEXTURE_2D, shader_params->texture_params[i].texture->id);

				switch(shader_params->texture_params[i].projection_mode)
				{
					case GFX_NONE:
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_NONE);
						break;
					case GFX_SHADOW_PROJECTION:
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
						break;
				}
			}
			else
			{
				glMatrixMode(GL_TEXTURE);
				glLoadIdentity();

				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
	}

	shader_program = shader_params->shader_program;

	if(shader_program)
	{
		if(shader_program != driver->shader_params.shader_program)
			glUseProgram(shader_program->id);

		// just inputting with values that do not make sense
		driver->shader_config.textures = 255;
		driver->shader_config.light = 255;
	}
	else
	{
		gfx_get_shader_program_config(shader_params, &shader_config);
		if(memcmp(&driver->shader_config, &shader_config, sizeof(gfx_shader_config)))
		{
			memcpy(&driver->shader_config, &shader_config, sizeof(gfx_shader_config));
			shader_program = gfx_get_shader_program(&shader_config);
			if(shader_program) glUseProgram(shader_program->id);
			else return;
		}
		else
		{
			shader_program = driver->shader_params.shader_program;
		}
	}

	if(shader_program->projection_matrix_loc != -1)
		glUniformMatrix4fv(shader_program->projection_matrix_loc,
			1, GL_FALSE, shader_params->projection_matrix);
	if(shader_program->modelview_matrix_loc != -1)
		glUniformMatrix4fv(shader_program->modelview_matrix_loc,
			1, GL_FALSE, shader_params->modelview_matrix);

	if(shader_program->color_loc != -1)
		glUniform4fv(shader_program->color_loc, 1, &shader_params->material_params.color.r);
	if(shader_program->specular_color_loc != -1)
		glUniform3fv(shader_program->specular_color_loc, 1, &shader_params->material_params.specular_color.r);
	if(shader_program->spec_power_loc != -1)
		glUniform1f(shader_program->spec_power_loc, shader_params->material_params.spec_power);

	if(shader_program->texture0_loc != -1)
		glUniform1i(shader_program->texture0_loc, 0);
	if(shader_program->texture1_loc != -1)
		glUniform1i(shader_program->texture1_loc, 1);
	if(shader_program->texture2_loc != -1)
		glUniform1i(shader_program->texture2_loc, 2);
	if(shader_program->texture3_loc != -1)
		glUniform1i(shader_program->texture3_loc, 3);
	if(shader_program->alpha_threshold_loc != -1)
		glUniform1f(shader_program->alpha_threshold_loc, shader_params->render_params.alpha_threshold);

	for(i = 0; i < GFX_MAX_TEXTURES; i++)
	{
		if(shader_params->texture_params[i].texture)
		{
			switch(shader_params->texture_params[i].type)
			{
				case GFX_COLOR_MAP:
					if(shader_program->color_map_loc != -1)
						glUniform1i(shader_program->color_map_loc, i);
					break;
				case GFX_NORMAL_MAP:
					if(shader_program->normal_map_loc != -1)
						glUniform1i(shader_program->normal_map_loc, i);
					break;
				case GFX_HEIGHT_MAP:
					if(shader_program->parallax_scale_loc != -1)
						glUniform1f(shader_program->parallax_scale_loc, shader_params->texture_params[i].parallax_scale);
					if(shader_program->height_map_loc != -1)
						glUniform1i(shader_program->height_map_loc, i);
					break;
				case GFX_SPECULAR_MAP:
					if(shader_program->specular_map_loc != -1)
						glUniform1i(shader_program->specular_map_loc, i);
					break;
				case GFX_COLOR_RAMP_MAP:
					if(shader_program->color_ramp_map_loc != -1)
						glUniform1i(shader_program->color_ramp_map_loc, i);
					break;
				case GFX_LIGHT_MAP:
					if(shader_program->light_map_loc != -1)
						glUniform1i(shader_program->light_map_loc, i);
					break;
				case GFX_SHADOW_MAP:
					if(shader_program->shadow_map_loc != -1)
						glUniform1i(shader_program->shadow_map_loc, i);
					if(shader_program->shadow_projection_matrix_loc != -1)
						glUniformMatrix4fv(shader_program->shadow_projection_matrix_loc,
							1, GL_FALSE, shader_params->texture_params[i].matrix);
					break;
			}
		}
	}

	if(shader_params->light_params.type)
	{
		if(shader_program->light_position_loc != -1)
			glUniform3fv(shader_program->light_position_loc, 1, &shader_params->light_params.position.x);
		if(shader_program->light_color_loc != -1)
			glUniform3fv(shader_program->light_color_loc, 1, &shader_params->light_params.color.r);
		if(shader_program->light_distance_loc != -1)
			glUniform1f(shader_program->light_distance_loc, shader_params->light_params.distance);
		if(shader_program->light_fade_speed_loc != -1)
			glUniform1f(shader_program->light_fade_speed_loc, shader_params->light_params.fade_speed);
		if(shader_program->light_spot_direction_loc != -1)
			glUniform3fv(shader_program->light_spot_direction_loc, 1, &shader_params->light_params.direction.x);
		if(shader_program->light_inner_cone_cos_loc != -1)
			glUniform1f(shader_program->light_inner_cone_cos_loc, (float)cos(shader_params->light_params.inner_cone*GFX_PI_DIV_180));
		if(shader_program->light_outer_cone_cos_loc != -1)
			glUniform1f(shader_program->light_outer_cone_cos_loc, (float)cos((shader_params->light_params.inner_cone+shader_params->light_params.outer_cone)*GFX_PI_DIV_180));
	}
	
	if(shader_program->clip_start_loc != -1)
		glUniform1f(shader_program->clip_start_loc, shader_params->clip_start);
	if(shader_program->clip_end_loc != -1)
		glUniform1f(shader_program->clip_end_loc, shader_params->clip_end);
	if(shader_program->viewport_width_loc != -1)
		glUniform1i(shader_program->viewport_width_loc, shader_params->viewport_width);
	if(shader_program->viewport_height_loc != -1)
		glUniform1i(shader_program->viewport_height_loc, shader_params->viewport_height);

	memcpy(&driver->shader_params, shader_params, sizeof(gfx_shader_params));
	driver->shader_params.shader_program = shader_program;
}

