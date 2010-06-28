
elf_model* elf_create_model(const char *name)
{
	elf_model *model;

	model = (elf_model*)malloc(sizeof(elf_model));
	memset(model, 0x0, sizeof(elf_model));
	model->type = ELF_MODEL;

	model->id = ++gen->model_id_counter;

	if(name) model->name = elf_create_string(name);

	elf_inc_obj_count();

	return model;
}

void elf_generate_model_tangents(elf_model *model)
{
	float *vertex_buffer;
	float *tex_coord_buffer;
	float *normal_buffer;
	float *tangent_buffer;
	float *vertices;
	float *tex_coords;
	float *tangents;
	float edge1[3];
	float edge2[3];
	float edge1uv[2];
	float edge2uv[2];
	float cp;
	float mul;
	float dot;
	int i, j;

	if(!model->vertices || !model->tex_coords || !model->index) return;

	if(model->tangents) gfx_dec_ref((gfx_object*)model->tangents);

	vertex_buffer = (float*)gfx_get_vertex_data_buffer(model->vertices);
	tex_coord_buffer = (float*)gfx_get_vertex_data_buffer(model->tex_coords);

	vertices = (float*)malloc(sizeof(float)*model->indice_count*3);
	tex_coords = (float*)malloc(sizeof(float)*model->indice_count*2);
	tangents = (float*)malloc(sizeof(float)*model->indice_count*3);

	// create corresponding vertice and tex coord arrays independent of index
	for(i = 0; i < (int)model->indice_count/3; i++)
	{
		vertices[i*9] = vertex_buffer[model->index[i*3]*3];
		vertices[i*9+1] = vertex_buffer[model->index[i*3]*3+1];
		vertices[i*9+2] = vertex_buffer[model->index[i*3]*3+2];
		vertices[i*9+3] = vertex_buffer[model->index[i*3+1]*3];
		vertices[i*9+4] = vertex_buffer[model->index[i*3+1]*3+1];
		vertices[i*9+5] = vertex_buffer[model->index[i*3+1]*3+2];
		vertices[i*9+6] = vertex_buffer[model->index[i*3+2]*3];
		vertices[i*9+7] = vertex_buffer[model->index[i*3+2]*3+1];
		vertices[i*9+8] = vertex_buffer[model->index[i*3+2]*3+2];

		tex_coords[i*6] = tex_coord_buffer[model->index[i*3]*2];
		tex_coords[i*6+1] = tex_coord_buffer[model->index[i*3]*2+1];
		tex_coords[i*6+2] = tex_coord_buffer[model->index[i*3+1]*2];
		tex_coords[i*6+3] = tex_coord_buffer[model->index[i*3+1]*2+1];
		tex_coords[i*6+4] = tex_coord_buffer[model->index[i*3+2]*2];
		tex_coords[i*6+5] = tex_coord_buffer[model->index[i*3+2]*2+1];
	}

	memset(tangents, 0x0, sizeof(float)*model->indice_count*3);

	// calculate tangents
	for(i = 0, j = 0; i < (int)model->indice_count*3; i+=9, j+=6)
	{
		edge1[0] = vertices[i+3]-vertices[i];
		edge1[1] = vertices[i+4]-vertices[i+1];
		edge1[2] = vertices[i+5]-vertices[i+2];
		edge2[0] = vertices[i+6]-vertices[i];
		edge2[1] = vertices[i+7]-vertices[i+1];
		edge2[2] = vertices[i+8]-vertices[i+2];

		edge1uv[0] = tex_coords[j+2]-tex_coords[j];
		edge1uv[1] = tex_coords[j+3]-tex_coords[j+1];
		edge2uv[0] = tex_coords[j+4]-tex_coords[j];
		edge2uv[1] = tex_coords[j+5]-tex_coords[j+1];

		cp = edge1uv[1]*edge2uv[0]-edge1uv[0]*edge2uv[1];

		if(cp != 0.0f)
		{
			mul = 1.0f/cp;
			tangents[i] = (edge1[0]*(-edge2uv[1])+edge2[0]*edge1uv[1])*mul;
			tangents[i+1] = (edge1[1]*(-edge2uv[1])+edge2[1]*edge1uv[1])*mul;
			tangents[i+2] = (edge1[2]*(-edge2uv[1])+edge2[2]*edge1uv[1])*mul;

			gfx_vec_normalize(&tangents[i]);

			memcpy(&tangents[i+3], &tangents[i], sizeof(float)*3);
			memcpy(&tangents[i+6], &tangents[i], sizeof(float)*3);
		}
	}

	model->tangents = gfx_create_vertex_data(3*model->vertice_count, GFX_FLOAT, GFX_VERTEX_DATA_STATIC);
	gfx_inc_ref((gfx_object*)model->tangents);

	tangent_buffer = gfx_get_vertex_data_buffer(model->tangents);

	// smooth tangents
	for(i = 0; i < (int)model->indice_count/3; i++)
	{
		tangent_buffer[model->index[i*3]*3] += tangents[i*9];
		tangent_buffer[model->index[i*3]*3+1] += tangents[i*9+1];
		tangent_buffer[model->index[i*3]*3+2] += tangents[i*9+2];
		tangent_buffer[model->index[i*3+1]*3] += tangents[i*9+3];
		tangent_buffer[model->index[i*3+1]*3+1] += tangents[i*9+4];
		tangent_buffer[model->index[i*3+1]*3+2] += tangents[i*9+5];
		tangent_buffer[model->index[i*3+2]*3] += tangents[i*9+6];
		tangent_buffer[model->index[i*3+2]*3+1] += tangents[i*9+7];
		tangent_buffer[model->index[i*3+2]*3+2] += tangents[i*9+8];
	}

	for(i = 0; i < (int)model->vertice_count*3; i+=3)
	{
		gfx_vec_normalize(&tangent_buffer[i]);
	}

	normal_buffer = gfx_get_vertex_data_buffer(model->normals);

	// orthogonize tangents
	for(i = 0; i < (int)model->vertice_count*3; i+=3)
	{
		gfx_vec_dot_vec(&tangent_buffer[i], &normal_buffer[i], &dot);
		tangent_buffer[i] -= normal_buffer[i]*dot;
		tangent_buffer[i+1] -= normal_buffer[i+1]*dot;
		tangent_buffer[i+2] -= normal_buffer[i+2]*dot;
		gfx_vec_normalize(&tangent_buffer[i]);
	}

	gfx_set_vertex_array_data(model->vertex_array, GFX_TANGENT, model->tangents);

	free(vertices);
	free(tex_coords);
	free(tangents);
}

void elf_destroy_model(elf_model *model)
{
	unsigned int i;

	if(model->name) elf_destroy_string(model->name);
	if(model->file_path) elf_destroy_string(model->file_path);

	if(model->vertex_array) gfx_dec_ref((gfx_object*)model->vertex_array);
	if(model->vertices) gfx_dec_ref((gfx_object*)model->vertices);
	if(model->normals) gfx_dec_ref((gfx_object*)model->normals);
	if(model->tex_coords) gfx_dec_ref((gfx_object*)model->tex_coords);
	if(model->tangents) gfx_dec_ref((gfx_object*)model->tangents);

	if(model->areas)
	{
		for(i = 0; i < model->area_count; i++)
		{
			if(model->areas[i].indice_count > 0)
			{
				gfx_dec_ref((gfx_object*)model->areas[i].index);
				gfx_dec_ref((gfx_object*)model->areas[i].vertex_index);
			}
		}
		free(model->areas);
	}

	if(model->index) free(model->index);
	if(model->weights) free(model->weights);
	if(model->boneids) free(model->boneids);
	if(model->tri_mesh) elf_dec_ref((elf_object*)model->tri_mesh);

	free(model);

	elf_dec_obj_count();
}

const char* elf_get_model_name(elf_model *model)
{
	return model->name;
}

const char* elf_get_model_file_path(elf_model *model)
{
	return model->file_path;
}

int elf_get_model_vertice_count(elf_model *model)
{
	return model->vertice_count;
}

int elf_get_model_indice_count(elf_model *model)
{
	return model->indice_count;
}

elf_vec3f elf_get_model_bounding_box_min(elf_model *model)
{
	return model->bb_min;
}

elf_vec3f elf_get_model_bounding_box_max(elf_model *model)
{
	return model->bb_max;
}

float* elf_get_model_vertices(elf_model *model)
{
	return (float*)gfx_get_vertex_data_buffer(model->vertices);
}

float* elf_get_model_normals(elf_model *model)
{
	if(!model->normals) return NULL;
	return (float*)gfx_get_vertex_data_buffer(model->normals);
}

float* elf_get_model_tex_coords(elf_model *model)
{
	if(!model->tex_coords) return NULL;
	return (float*)model->tex_coords;
}

float* elf_get_model_tangents(elf_model *model)
{
	if(!model->tangents) return NULL;
	return (float*)gfx_get_vertex_data_buffer(model->tangents);
}

unsigned int* elf_get_model_indices(elf_model *model)
{
	return model->index;
}

void elf_draw_model(elf_list *materials, elf_model *model, gfx_shader_params *shader_params, unsigned char *non_lit_flag)
{
	int i;
	elf_material *material;
	unsigned char light_type;

	if(!model->vertex_array) return;
	gfx_set_vertex_array(model->vertex_array);

	for(i = 0, material = (elf_material*)elf_begin_list(materials); i < (int)model->area_count;
		i++, material = (elf_material*)elf_next_in_list(materials))
	{
		if(model->areas[i].vertex_index)
		{
			if(material)
			{
				elf_set_material(material, shader_params);

				light_type = shader_params->light_params.type;
				if(!material->lighting)
				{
					if(material->non_lit_flag == eng->non_lit_flag && model->non_lit_flag == eng->non_lit_flag && *non_lit_flag == eng->non_lit_flag) continue;
					material->non_lit_flag = eng->non_lit_flag;

					shader_params->light_params.type = GFX_NONE;
				}

				gfx_set_shader_params(shader_params);

				shader_params->light_params.type = light_type;
			}
			gfx_draw_vertex_index(model->areas[i].vertex_index, GFX_TRIANGLES);
		}
	}

	model->non_lit_flag = *non_lit_flag = eng->non_lit_flag;
}

void elf_draw_model_ambient(elf_list *materials, elf_model *model, gfx_shader_params *shader_params)
{
	int i;
	elf_material *material;

	if(!model->vertex_array) return;

	gfx_set_vertex_array(model->vertex_array);

	for(i = 0, material = (elf_material*)elf_begin_list(materials); i < (int)model->area_count;
		i++, material = (elf_material*)elf_next_in_list(materials))
	{
		if(model->areas[i].vertex_index)
		{
			if(material)
			{
				elf_set_material(material, shader_params);
				shader_params->material_params.color.r = eng->ambient_color.r*material->ambient_color.r;
				shader_params->material_params.color.g = eng->ambient_color.g*material->ambient_color.g;
				shader_params->material_params.color.b = eng->ambient_color.b*material->ambient_color.b;
				gfx_set_shader_params(shader_params);
			}
			gfx_draw_vertex_index(model->areas[i].vertex_index, GFX_TRIANGLES);
		}
	}
}

void elf_draw_model_without_materials(elf_list *materials, elf_model *model, gfx_shader_params *shader_params)
{
	int i;
	elf_material *material;

	if(!model->vertex_array) return;

	gfx_set_vertex_array(model->vertex_array);

	for(i = 0, material = (elf_material*)elf_begin_list(materials); i < (int)model->area_count;
		i++, material = (elf_material*)elf_next_in_list(materials))
	{
		if(model->areas[i].vertex_index)
		{
			if(material) elf_set_material_alpha_texture(material, shader_params);
			gfx_set_shader_params(shader_params);
			gfx_draw_vertex_index(model->areas[i].vertex_index, GFX_TRIANGLES);
			gfx_set_texture_params_default(shader_params);
		}
	}
}

void elf_draw_model_bounding_box(elf_model *model, gfx_shader_params *shader_params)
{
	if(!model->vertex_array) return;

	gfx_set_shader_params(shader_params);
	gfx_draw_bounding_box(&model->bb_min.x, &model->bb_max.x);
}

