
elf_particle* elf_create_particle()
{
	elf_particle *particle;

	particle = (elf_particle*)malloc(sizeof(elf_particle));
	memset(particle, 0x0, sizeof(elf_particle));
	particle->type = ELF_PARTICLE;

	global_obj_count++;

	return particle;
}

void elf_destroy_particle(elf_particle *particle)
{
	free(particle);

	global_obj_count--;
}

elf_particles* elf_create_particles(const char *name, int max_count)
{
	elf_particles *particles;
	float *tex_coord_buffer;
	float *color_buffer;
	int i, j, k;

	if(max_count < 1) return NULL;

	particles = (elf_particles*)malloc(sizeof(elf_particles));
	memset(particles, 0x0, sizeof(elf_particles));
	particles->type = ELF_PARTICLES;

	elf_init_actor((elf_actor*)particles, ELF_FALSE);

	particles->max_count = max_count;
	particles->particles = elf_create_list();
	elf_inc_ref((elf_object*)particles->particles);

	particles->draw_mode = ELF_ADD;
	particles->spawn_delay = 0.02;
	particles->spawn = ELF_TRUE;
	particles->size_min = 1.0;
	particles->size_max = 1.0;
	particles->size_growth_min = 0.0;
	particles->size_growth_max = 0.0;
	particles->rotation_min = 0.0;
	particles->rotation_max = 0.0;
	particles->rotation_growth_min = 0.0;
	particles->rotation_growth_max = 0.0;
	particles->life_span_min = 3.0;
	particles->life_span_max = 3.0;
	particles->fade_speed_min = 0.33;
	particles->fade_speed_max = 0.33;
	particles->velocity_min.x = -10.0; particles->velocity_min.y = -10.0; particles->velocity_min.z = -10.0;
	particles->velocity_max.x = 10.0; particles->velocity_max.y = 10.0; particles->velocity_max.z = 10.0;
	particles->color_min.r = 1.0; particles->color_min.g = 1.0; particles->color_min.b = 1.0; particles->color_min.a = 1.0;
	particles->color_max.r = 1.0; particles->color_max.g = 1.0; particles->color_max.b = 1.0; particles->color_max.a = 1.0;

	particles->vertices = gfx_create_vertex_data(3*6*max_count, GFX_FLOAT, GFX_VERTEX_DATA_DYNAMIC);
	particles->tex_coords = gfx_create_vertex_data(2*6*max_count, GFX_FLOAT, GFX_VERTEX_DATA_DYNAMIC);
	particles->colors = gfx_create_vertex_data(4*6*max_count,  GFX_FLOAT, GFX_VERTEX_DATA_DYNAMIC);
	particles->vertex_array = gfx_create_vertex_array(GFX_FALSE);
	gfx_set_vertex_array_data(particles->vertex_array, GFX_VERTEX, particles->vertices);
	gfx_set_vertex_array_data(particles->vertex_array, GFX_TEX_COORD, particles->tex_coords);
	gfx_set_vertex_array_data(particles->vertex_array, GFX_COLOR, particles->colors);

	gfx_inc_ref((gfx_object*)particles->vertices);
	gfx_inc_ref((gfx_object*)particles->tex_coords);
	gfx_inc_ref((gfx_object*)particles->colors);
	gfx_inc_ref((gfx_object*)particles->vertex_array);

	tex_coord_buffer = (float*)gfx_get_vertex_data_buffer(particles->tex_coords);
	color_buffer = (float*)gfx_get_vertex_data_buffer(particles->colors);

	for(i = 0; i < particles->max_count; i++)
	{
		j = i*12;
		k = i*24;

		tex_coord_buffer[j] = 0.0;
		tex_coord_buffer[j+1] = 1.0;
		tex_coord_buffer[j+2] = 0.0;
		tex_coord_buffer[j+3] = 0.0;
		tex_coord_buffer[j+4] = 1.0;
		tex_coord_buffer[j+5] = 0.0;

		tex_coord_buffer[j+6] = 0.0;
		tex_coord_buffer[j+7] = 1.0;
		tex_coord_buffer[j+8] = 1.0;
		tex_coord_buffer[j+9] = 0.0;
		tex_coord_buffer[j+10] = 1.0;
		tex_coord_buffer[j+11] = 1.0;

		color_buffer[k] = 1.0;
		color_buffer[k+1] = 1.0;
		color_buffer[k+2] = 1.0;
		color_buffer[k+3] = 1.0;
		color_buffer[k+4] = 1.0;
		color_buffer[k+5] = 1.0;
		color_buffer[k+6] = 1.0;
		color_buffer[k+7] = 1.0;
		color_buffer[k+8] = 1.0;
		color_buffer[k+9] = 1.0;
		color_buffer[k+10] = 1.0;
		color_buffer[k+11] = 1.0;
		color_buffer[k+12] = 1.0;
		color_buffer[k+13] = 1.0;
		color_buffer[k+14] = 1.0;
		color_buffer[k+15] = 1.0;
		color_buffer[k+16] = 1.0;
		color_buffer[k+17] = 1.0;
		color_buffer[k+18] = 1.0;
		color_buffer[k+19] = 1.0;
		color_buffer[k+20] = 1.0;
		color_buffer[k+21] = 1.0;
		color_buffer[k+22] = 1.0;
		color_buffer[k+23] = 1.0;
	}

	if(name) particles->name = elf_create_string(name);

	particles->id = ++particles_id_counter;

	global_obj_count++;

	return particles;
}

void elf_init_new_particle(elf_particles *particles, elf_particle *particle)
{
	static int num;
	static float *vertices;
	static elf_vec4f orient;
	static elf_vec3f local_pos;
	static elf_vec3f result;

	particle->life_span = elf_random_float_range(particles->life_span_min, particles->life_span_max);
	particle->fade_speed = elf_random_float_range(particles->fade_speed_min, particles->fade_speed_max);
	particle->size = elf_random_float_range(particles->size_min, particles->size_max);
	particle->size_growth = elf_random_float_range(particles->size_growth_min, particles->size_growth_max);
	particle->rotation = elf_random_float_range(particles->rotation_min, particles->rotation_max);
	particle->rotation_growth = elf_random_float_range(particles->rotation_growth_min, particles->rotation_growth_max);
	if(particles->model && elf_get_model_vertice_count(particles->model) > 0)
	{
		elf_get_actor_position_((elf_actor*)particles, &particle->position.x);
		num = elf_random_int_range(0, elf_get_model_vertice_count(particles->model));
		vertices = elf_get_model_vertices(particles->model);
		particle->position.x += vertices[3*num];
		particle->position.y += vertices[3*num+1];
		particle->position.z += vertices[3*num+2];
	}
	else if(particles->entity && particles->entity->model && particles->entity->vertices &&
		elf_get_model_vertice_count(particles->entity->model) > 0)
	{
		elf_get_actor_position_((elf_actor*)particles->entity, &particle->position.x);
		num = elf_random_int_range(0, elf_get_model_vertice_count(particles->entity->model));
		vertices = gfx_get_vertex_data_buffer(particles->entity->vertices);
		local_pos.x = vertices[3*num];
		local_pos.y = vertices[3*num+1];
		local_pos.z = vertices[3*num+2];
		elf_get_actor_orientation_((elf_actor*)particles->entity, &orient.x);
		gfx_mul_qua_vec(&orient.x, &local_pos.x, &result.x);
		particle->position.x += result.x;
		particle->position.y += result.y;
		particle->position.z += result.z;
	}
	else
	{
		elf_get_actor_position_((elf_actor*)particles, &particle->position.x);
		particle->position.x += elf_random_float_range(particles->position_min.x, particles->position_max.x);
		particle->position.y += elf_random_float_range(particles->position_min.y, particles->position_max.y);
		particle->position.z += elf_random_float_range(particles->position_min.z, particles->position_max.z);
	}
	particle->velocity.x = elf_random_float_range(particles->velocity_min.x, particles->velocity_max.x);
	particle->velocity.y = elf_random_float_range(particles->velocity_min.y, particles->velocity_max.y);
	particle->velocity.z = elf_random_float_range(particles->velocity_min.z, particles->velocity_max.z);
	particle->color.r = elf_random_float_range(particles->color_min.r, particles->color_max.r);
	particle->color.g = elf_random_float_range(particles->color_min.g, particles->color_max.g);
	particle->color.b = elf_random_float_range(particles->color_min.b, particles->color_max.b);
	particle->color.a = elf_random_float_range(particles->color_min.a, particles->color_max.a);
}

void elf_update_particles(elf_particles *particles, float sync)
{
	elf_particle *particle;
	int spawn_count;
	int i;

	elf_update_actor((elf_actor*)particles);

	// update, remove and spawn particles
	particles->cur_time += sync;
	spawn_count = (int)(particles->cur_time/particles->spawn_delay);
	if(elf_get_list_length(particles->particles)+spawn_count > particles->max_count)
		spawn_count -= (elf_get_list_length(particles->particles)+spawn_count)-particles->max_count;
	if(spawn_count > 0) particles->cur_time -= particles->spawn_delay*spawn_count;

	for(particle = (elf_particle*)elf_begin_list(particles->particles); particle;
		particle = (elf_particle*)elf_next_in_list(particles->particles))
	{
		if(particle->life_span < 0.0 || particle->color.a < 0.0)
		{
			if(spawn_count > 0 && particles->spawn)
			{
				elf_init_new_particle(particles, particle);
				spawn_count--;
			}
			else
			{
				elf_remove_from_list(particles->particles, (elf_object*)particle);
			}
			continue;
		}
		particle->size += particle->size_growth*sync;
		particle->rotation += particle->rotation_growth*sync;
		particle->position.x += particle->velocity.x*sync;
		particle->position.y += particle->velocity.y*sync;
		particle->position.z += particle->velocity.z*sync;
		particle->life_span -= sync;
		particle->velocity.x += particles->gravity.x*sync;
		particle->velocity.y += particles->gravity.y*sync;
		particle->velocity.z += particles->gravity.z*sync;
		particle->color.a -= particle->fade_speed*sync;
	}

	// spawn particles
	if(particles->spawn)
	{
		for(i = 0; i < spawn_count; i++)
		{
			particle = elf_create_particle();

			elf_init_new_particle(particles, particle);

			elf_append_to_list(particles->particles, (elf_object*)particle);
		}
	}
}

void elf_draw_particles(elf_particles *particles, elf_camera *camera, gfx_shader_params *shader_params)
{
	elf_particle *particle;
	int i, j;
	float offset;
	float pos[3];
	float camera_pos[3];
	float camera_orient[4];
	float inv_camera_pos[3];
	float inv_camera_orient[4];
	float particle_offset[3];
	elf_color real_color;
	float sin_x1;
	float cos_y1;
	float sin_x2;
	float cos_y2;
	float sin_x3;
	float cos_y3;
	float sin_x4;
	float cos_y4;
	float radius;
	float *vertex_buffer;
	float *color_buffer;

	vertex_buffer = gfx_get_vertex_data_buffer(particles->vertices);
	color_buffer = gfx_get_vertex_data_buffer(particles->colors);

	elf_get_actor_position_((elf_actor*)camera, camera_pos);
	elf_get_actor_orientation_((elf_actor*)camera, camera_orient);

	inv_camera_pos[0] = -camera_pos[0]; inv_camera_pos[1] = -camera_pos[1]; inv_camera_pos[2] = -camera_pos[2];
	gfx_qua_get_inverse(camera_orient, inv_camera_orient);

	// rotating the particles takes up a lot of processing power, so see if it is really
	// necessary before going ahead and doing it
	if(elf_about_zero(particles->rotation_min) && elf_about_zero(particles->rotation_max) &&
		elf_about_zero(particles->rotation_growth_min) && elf_about_zero(particles->rotation_growth_max))
	{
		for(i = 0, j = 0, particle = (elf_particle*)elf_begin_list(particles->particles); particle;
			particle = (elf_particle*)elf_next_in_list(particles->particles), i++)
		{
			particle_offset[0] = inv_camera_pos[0]+particle->position.x;
			particle_offset[1] = inv_camera_pos[1]+particle->position.y;
			particle_offset[2] = inv_camera_pos[2]+particle->position.z;

			gfx_mul_qua_vec(inv_camera_orient, particle_offset, pos);

			j = i*18;
			offset = particle->size*0.5;

			vertex_buffer[j] = pos[0]-offset;
			vertex_buffer[j+1] = pos[1]+offset;
			vertex_buffer[j+2] = pos[2];
			vertex_buffer[j+3] = pos[0]-offset;
			vertex_buffer[j+4] = pos[1]-offset;
			vertex_buffer[j+5] = pos[2];
			vertex_buffer[j+6] = pos[0]+offset;
			vertex_buffer[j+7] = pos[1]-offset;
			vertex_buffer[j+8] = pos[2];

			vertex_buffer[j+9] = pos[0]-offset;
			vertex_buffer[j+10] = pos[1]+offset;
			vertex_buffer[j+11] = pos[2];
			vertex_buffer[j+12] = pos[0]+offset;
			vertex_buffer[j+13] = pos[1]-offset;
			vertex_buffer[j+14] = pos[2];
			vertex_buffer[j+15] = pos[0]+offset;
			vertex_buffer[j+16] = pos[1]+offset;
			vertex_buffer[j+17] = pos[2];

			j = i*24;
			real_color = particle->color;
			if(particles->draw_mode == ELF_ADD)
			{
				real_color.r *= real_color.a;
				real_color.g *= real_color.a;
				real_color.b *= real_color.a;
				real_color.a = 1.0;
			}

			color_buffer[j] = real_color.r;
			color_buffer[j+1] = real_color.g;
			color_buffer[j+2] = real_color.b;
			color_buffer[j+3] = real_color.a;
			color_buffer[j+4] = real_color.r;
			color_buffer[j+5] = real_color.g;
			color_buffer[j+6] = real_color.b;
			color_buffer[j+7] = real_color.a;
			color_buffer[j+8] = real_color.r;
			color_buffer[j+9] = real_color.g;
			color_buffer[j+10] = real_color.b;
			color_buffer[j+11] = real_color.a;
			color_buffer[j+12] = real_color.r;
			color_buffer[j+13] = real_color.g;
			color_buffer[j+14] = real_color.b;
			color_buffer[j+15] = real_color.a;
			color_buffer[j+16] = real_color.r;
			color_buffer[j+17] = real_color.g;
			color_buffer[j+18] = real_color.b;
			color_buffer[j+19] = particle->color.a;
			color_buffer[j+20] = real_color.r;
			color_buffer[j+21] = real_color.g;
			color_buffer[j+22] = real_color.b;
			color_buffer[j+23] = particle->color.a;
		}
	}
	else
	{
		for(i = 0, j = 0, particle = (elf_particle*)elf_begin_list(particles->particles); particle;
			particle = (elf_particle*)elf_next_in_list(particles->particles), i++)
		{

			particle_offset[0] = inv_camera_pos[0]+particle->position.x;
			particle_offset[1] = inv_camera_pos[1]+particle->position.y;
			particle_offset[2] = inv_camera_pos[2]+particle->position.z;

			gfx_mul_qua_vec(inv_camera_orient, particle_offset, pos);

			j = i*18;
			offset = particle->size*0.5;
			radius = offset/0.707107;
			sin_x1 = sin(GFX_PI_DIV_180*(45.0+particle->rotation));
			cos_y1 = cos(GFX_PI_DIV_180*(45.0+particle->rotation));
			sin_x2 = sin(GFX_PI_DIV_180*(135.0+particle->rotation));
			cos_y2 = cos(GFX_PI_DIV_180*(135.0+particle->rotation));
			sin_x3 = sin(GFX_PI_DIV_180*(225.0+particle->rotation));
			cos_y3 = cos(GFX_PI_DIV_180*(225.0+particle->rotation));
			sin_x4 = sin(GFX_PI_DIV_180*(315.0+particle->rotation));
			cos_y4 = cos(GFX_PI_DIV_180*(315.0+particle->rotation));

			vertex_buffer[j] = pos[0]+radius*sin_x4;
			vertex_buffer[j+1] = pos[1]+radius*cos_y4;
			vertex_buffer[j+2] = pos[2];
			vertex_buffer[j+3] = pos[0]+radius*sin_x3;
			vertex_buffer[j+4] = pos[1]+radius*cos_y3;
			vertex_buffer[j+5] = pos[2];
			vertex_buffer[j+6] = pos[0]+radius*sin_x2;
			vertex_buffer[j+7] = pos[1]+radius*cos_y2;
			vertex_buffer[j+8] = pos[2];

			vertex_buffer[j+9] = pos[0]+radius*sin_x4;
			vertex_buffer[j+10] = pos[1]+radius*cos_y4;
			vertex_buffer[j+11] = pos[2];
			vertex_buffer[j+12] = pos[0]+radius*sin_x2;
			vertex_buffer[j+13] = pos[1]+radius*cos_y2;
			vertex_buffer[j+14] = pos[2];
			vertex_buffer[j+15] = pos[0]+radius*sin_x1;
			vertex_buffer[j+16] = pos[1]+radius*cos_y1;
			vertex_buffer[j+17] = pos[2];

			j = i*24;
			real_color = particle->color;
			if(particles->draw_mode == ELF_ADD)
			{
				real_color.r *= real_color.a;
				real_color.g *= real_color.a;
				real_color.b *= real_color.a;
				real_color.a = 1.0;
			}

			color_buffer[j] = real_color.r;
			color_buffer[j+1] = real_color.g;
			color_buffer[j+2] = real_color.b;
			color_buffer[j+3] = real_color.a;
			color_buffer[j+4] = real_color.r;
			color_buffer[j+5] = real_color.g;
			color_buffer[j+6] = real_color.b;
			color_buffer[j+7] = real_color.a;
			color_buffer[j+8] = real_color.r;
			color_buffer[j+9] = real_color.g;
			color_buffer[j+10] = real_color.b;
			color_buffer[j+11] = real_color.a;
			color_buffer[j+12] = real_color.r;
			color_buffer[j+13] = real_color.g;
			color_buffer[j+14] = real_color.b;
			color_buffer[j+15] = real_color.a;
			color_buffer[j+16] = real_color.r;
			color_buffer[j+17] = real_color.g;
			color_buffer[j+18] = real_color.b;
			color_buffer[j+19] = particle->color.a;
			color_buffer[j+20] = real_color.r;
			color_buffer[j+21] = real_color.g;
			color_buffer[j+22] = real_color.b;
			color_buffer[j+23] = particle->color.a;
		}
	}

	if(elf_get_list_length(particles->particles) > 0)
	{
		shader_params->render_params.blend_mode = particles->draw_mode;
		shader_params->render_params.vertex_color = GFX_TRUE;
		gfx_matrix4_set_identity(shader_params->modelview_matrix);
		if(particles->texture) shader_params->texture_params->texture = particles->texture->texture;
		else shader_params->texture_params->texture = NULL;
		shader_params->texture_params->type = GFX_COLOR_MAP;
		gfx_set_shader_params(shader_params);

		gfx_draw_vertex_array(particles->vertex_array, 6*elf_get_list_length(particles->particles), GFX_TRIANGLES);
	}
}

void elf_destroy_particles(elf_particles *particles)
{
	elf_clean_actor((elf_actor*)particles);

	elf_dec_ref((elf_object*)particles->particles);
	if(particles->texture) elf_dec_ref((elf_object*)particles->texture);
	if(particles->model) elf_dec_ref((elf_object*)particles->model);
	if(particles->entity) elf_dec_ref((elf_object*)particles->entity);

	gfx_dec_ref((gfx_object*)particles->vertex_array);
	gfx_dec_ref((gfx_object*)particles->vertices);
	gfx_dec_ref((gfx_object*)particles->tex_coords);
	gfx_dec_ref((gfx_object*)particles->colors);

	free(particles);

	global_obj_count--;
}

const char* elf_get_particles_name(elf_particles *particles)
{
	return particles->name;
}

const char* elf_get_particles_file_path(elf_particles *particles)
{
	return particles->file_path;
}

void elf_set_particles_max_count(elf_particles *particles, int max_count)
{
	int i, j, k;
	float *tex_coord_buffer;
	float *color_buffer;

	if(max_count < 1) return;

	elf_dec_ref((elf_object*)particles->particles);
	gfx_destroy_vertex_array(particles->vertex_array);
	free(particles->vertices);
	free(particles->tex_coords);
	free(particles->colors);

	particles->max_count = max_count;
	particles->particles = elf_create_list();
	elf_inc_ref((elf_object*)particles->particles);

	gfx_dec_ref((gfx_object*)particles->vertices);
	gfx_dec_ref((gfx_object*)particles->tex_coords);
	gfx_dec_ref((gfx_object*)particles->colors);
	gfx_dec_ref((gfx_object*)particles->vertex_array);

	particles->vertices = gfx_create_vertex_data(3*6*max_count, GFX_FLOAT, GFX_VERTEX_DATA_DYNAMIC);
	particles->tex_coords = gfx_create_vertex_data(2*6*max_count, GFX_FLOAT, GFX_VERTEX_DATA_DYNAMIC);
	particles->colors = gfx_create_vertex_data(4*6*max_count,  GFX_FLOAT, GFX_VERTEX_DATA_DYNAMIC);
	particles->vertex_array = gfx_create_vertex_array(GFX_FALSE);
	gfx_set_vertex_array_data(particles->vertex_array, GFX_VERTEX, particles->vertices);
	gfx_set_vertex_array_data(particles->vertex_array, GFX_TEX_COORD, particles->tex_coords);
	gfx_set_vertex_array_data(particles->vertex_array, GFX_COLOR, particles->colors);

	gfx_inc_ref((gfx_object*)particles->vertices);
	gfx_inc_ref((gfx_object*)particles->tex_coords);
	gfx_inc_ref((gfx_object*)particles->colors);
	gfx_inc_ref((gfx_object*)particles->vertex_array);

	tex_coord_buffer = (float*)gfx_get_vertex_data_buffer(particles->tex_coords);
	color_buffer = (float*)gfx_get_vertex_data_buffer(particles->colors);

	for(i = 0; i < particles->max_count; i++)
	{
		j = i*12;
		k = i*24;

		tex_coord_buffer[j] = 0.0;
		tex_coord_buffer[j+1] = 1.0;
		tex_coord_buffer[j+2] = 0.0;
		tex_coord_buffer[j+3] = 0.0;
		tex_coord_buffer[j+4] = 1.0;
		tex_coord_buffer[j+5] = 0.0;

		tex_coord_buffer[j+6] = 0.0;
		tex_coord_buffer[j+7] = 1.0;
		tex_coord_buffer[j+8] = 1.0;
		tex_coord_buffer[j+9] = 0.0;
		tex_coord_buffer[j+10] = 1.0;
		tex_coord_buffer[j+11] = 1.0;

		color_buffer[k] = 1.0;
		color_buffer[k+1] = 1.0;
		color_buffer[k+2] = 1.0;
		color_buffer[k+3] = 1.0;
		color_buffer[k+4] = 1.0;
		color_buffer[k+5] = 1.0;
		color_buffer[k+6] = 1.0;
		color_buffer[k+7] = 1.0;
		color_buffer[k+8] = 1.0;
		color_buffer[k+9] = 1.0;
		color_buffer[k+10] = 1.0;
		color_buffer[k+11] = 1.0;
		color_buffer[k+12] = 1.0;
		color_buffer[k+13] = 1.0;
		color_buffer[k+14] = 1.0;
		color_buffer[k+15] = 1.0;
		color_buffer[k+16] = 1.0;
		color_buffer[k+17] = 1.0;
		color_buffer[k+18] = 1.0;
		color_buffer[k+19] = 1.0;
		color_buffer[k+20] = 1.0;
		color_buffer[k+21] = 1.0;
		color_buffer[k+22] = 1.0;
		color_buffer[k+23] = 1.0;
	}
}

void elf_set_particles_draw_mode(elf_particles *particles, int mode)
{
	if(particles->draw_mode < 1 || particles->draw_mode > 4) return;
	particles->draw_mode = mode;
}

void elf_set_particles_texture(elf_particles *particles, elf_texture *texture)
{
	if(particles->texture) elf_dec_ref((elf_object*)particles->texture);
	particles->texture = texture;
	if(particles->texture) elf_inc_ref((elf_object*)particles->texture);
}

void elf_set_particles_model(elf_particles *particles, elf_model *model)
{
	if(particles->entity) elf_dec_ref((elf_object*)particles->entity);
	particles->entity = NULL;

	if(particles->model) elf_dec_ref((elf_object*)particles->model);
	particles->model = model;
	if(particles->model) elf_inc_ref((elf_object*)particles->model);
}

void elf_set_particles_entity(elf_particles *particles, elf_entity *entity)
{
	if(particles->model) elf_dec_ref((elf_object*)particles->model);
	particles->model = NULL;

	if(particles->entity) elf_dec_ref((elf_object*)particles->entity);
	particles->entity = entity;
	if(particles->entity) elf_inc_ref((elf_object*)particles->entity);
}

void elf_set_particles_gravity(elf_particles *particles, float x, float y, float z)
{
	particles->gravity.x = x;
	particles->gravity.y = y;
	particles->gravity.z = z;
}

void elf_set_particles_spawn_delay(elf_particles *particles, float delay)
{
	particles->spawn_delay = delay;
}

void elf_set_particles_spawn(elf_particles *particles, unsigned char spawn)
{
	particles->spawn = !spawn == ELF_FALSE;
}

void elf_set_particles_size(elf_particles *particles, float min, float max)
{
	particles->size_min = min;
	particles->size_max = max;
}

void elf_set_particles_size_growth(elf_particles *particles, float min, float max)
{
	particles->size_growth_min = min;
	particles->size_growth_max = max;
}

void elf_set_particles_rotation(elf_particles *particles, float min, float max)
{
	particles->rotation_min = min;
	particles->rotation_max = max;
}

void elf_set_particles_rotation_growth(elf_particles *particles, float min, float max)
{
	particles->rotation_growth_min = min;
	particles->rotation_growth_max = max;
}

void elf_set_particles_life_span(elf_particles *particles, float min, float max)
{
	particles->life_span_min = min;
	particles->life_span_max = max;
}

void elf_set_particles_fade_speed(elf_particles *particles, float min, float max)
{
	particles->fade_speed_min = min;
	particles->fade_speed_max = max;
}

void elf_set_particles_velocity_min(elf_particles *particles, float x, float y, float z)
{
	particles->velocity_min.x = x;
	particles->velocity_min.y = y;
	particles->velocity_min.z = z;
}

void elf_set_particles_position_min(elf_particles *particles, float x, float y, float z)
{
	particles->position_min.x = x;
	particles->position_min.y = y;
	particles->position_min.z = z;
}

void elf_set_particles_position_max(elf_particles *particles, float x, float y, float z)
{
	particles->position_max.x = x;
	particles->position_max.y = y;
	particles->position_max.z = z;
}

void elf_set_particles_velocity_max(elf_particles *particles, float x, float y, float z)
{
	particles->velocity_max.x = x;
	particles->velocity_max.y = y;
	particles->velocity_max.z = z;
}

void elf_set_particles_color_min(elf_particles *particles, float r, float g, float b, float a)
{
	particles->color_min.r = r;
	particles->color_min.g = g;
	particles->color_min.b = b;
	particles->color_min.a = a;
}

void elf_set_particles_color_max(elf_particles *particles, float r, float g, float b, float a)
{
	particles->color_max.r = r;
	particles->color_max.g = g;
	particles->color_max.b = b;
	particles->color_max.a = a;
}

int elf_get_particles_max_count(elf_particles *particles)
{
	return particles->max_count;
}

int elf_get_particles_count(elf_particles *particles)
{
	return elf_get_list_length(particles->particles);
}

int elf_get_particles_draw_mode(elf_particles *particles)
{
	return particles->draw_mode;
}

elf_texture* elf_get_particles_texture(elf_particles *particles)
{
	return particles->texture;
}

elf_model* elf_get_particles_model(elf_particles *particles)
{
	return particles->model;
}

elf_entity* elf_get_particles_entity(elf_particles *particles)
{
	return particles->entity;
}

elf_vec3f elf_get_particles_gravity(elf_particles *particles)
{
	return particles->gravity;
}

float elf_get_particles_spawn_delay(elf_particles *particles)
{
	return particles->spawn_delay;
}

unsigned char elf_get_particles_spawn(elf_particles *particles)
{
	return particles->spawn;
}

float elf_get_particles_size_min(elf_particles *particles)
{
	return particles->size_min;
}

float elf_get_particles_size_max(elf_particles *particles)
{
	return particles->size_max;
}

float elf_get_particles_rotation_min(elf_particles *particles)
{
	return particles->rotation_min;
}

float elf_get_particles_rotation_max(elf_particles *particles)
{
	return particles->rotation_max;
}

float elf_get_particles_rotation_growth_min(elf_particles *particles)
{
	return particles->rotation_growth_min;
}

float elf_get_particles_rotation_growth_max(elf_particles *particles)
{
	return particles->rotation_growth_max;
}

float elf_get_particles_size_growth_min(elf_particles *particles)
{
	return particles->size_growth_min;
}

float elf_get_particles_size_growth_max(elf_particles *particles)
{
	return particles->size_growth_max;
}

float elf_get_particles_life_span_min(elf_particles *particles)
{
	return particles->life_span_min;
}

float elf_get_particles_life_span_max(elf_particles *particles)
{
	return particles->life_span_max;
}

float elf_get_particles_fade_speed_min(elf_particles *particles)
{
	return particles->fade_speed_min;
}

float elf_get_particles_fade_speed_max(elf_particles *particles)
{
	return particles->fade_speed_max;
}

elf_vec3f elf_get_particles_position_min(elf_particles *particles)
{
	return particles->position_min;
}

elf_vec3f elf_get_particles_position_max(elf_particles *particles)
{
	return particles->position_max;
}

elf_vec3f elf_get_particles_velocity_min(elf_particles *particles)
{
	return particles->velocity_min;
}

elf_vec3f elf_get_particles_velocity_max(elf_particles *particles)
{
	return particles->velocity_max;
}

elf_color elf_get_particles_color_min(elf_particles *particles)
{
	return particles->color_min;
}

elf_color elf_get_particles_color_max(elf_particles *particles)
{
	return particles->color_max;
}
