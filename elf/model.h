
elf_model* elf_create_model_from_pak(FILE *file, const char *name, elf_scene *scene)
{
	elf_model *model = NULL;
	int magic = 0;
	unsigned int i = 0;
	char rname[64];
	unsigned int indices_read = 0;
	unsigned char is_normals;
	unsigned char is_tex_coords;
	unsigned char is_weights_and_boneids;
	unsigned char junk;
	float weights[4];
	float length;
	short int boneids[4];
	float *vertex_buffer;

	// read magic
	fread((char*)&magic, sizeof(int), 1, file);

	if(magic != 179532110)
	{
		elf_set_error(ELF_INVALID_FILE, "error: invalid model \"%s\", wrong magic number\n", name);
		return NULL;
	}

	model = (elf_model*)malloc(sizeof(elf_model));
	memset(model, 0x0, sizeof(elf_model));
	model->type = ELF_MODEL;

	global_obj_count++;

	// read name
	fread(rname, sizeof(char), 64, file);

	model->name = elf_create_string(rname);
	model->file_path = elf_create_string(elf_get_scene_file_path(scene));

	// read header
	fread((char*)&model->vertice_count, sizeof(int), 1, file);
	fread((char*)&model->frame_count, sizeof(int), 1, file);
	fread((char*)&model->indice_count, sizeof(int), 1, file);
	fread((char*)&model->area_count, sizeof(int), 1, file);
	fread((char*)&is_normals, sizeof(unsigned char), 1, file);
	fread((char*)&is_tex_coords, sizeof(unsigned char), 1, file);
	fread((char*)&is_weights_and_boneids, sizeof(unsigned char), 1, file);
	fread((char*)&junk, sizeof(unsigned char), 1, file);

	if(model->vertice_count < 3)
	{
		elf_set_error(ELF_INVALID_FILE, "error: invalid model \"%s\", invalid vertex count\n", name);
		elf_destroy_model(model);
		return NULL;
	}
	if(model->frame_count < 1)
	{
		elf_set_error(ELF_INVALID_FILE, "error: invalid model \"%s\", invalid frame count\n", name);
		elf_destroy_model(model);
		return NULL;
	}
	if(model->indice_count < 3)
	{
		elf_set_error(ELF_INVALID_FILE, "error: invalid model \"%s\", invalid indice count\n", name);
		elf_destroy_model(model);
		return NULL;
	}
	if(model->area_count < 1)
	{
		elf_set_error(ELF_INVALID_FILE, "error: invalid model \"%s\", invalid area count\n", name);
		elf_destroy_model(model);
		return NULL;
	}

	// read vertices
	model->vertices = gfx_create_vertex_data(3*model->vertice_count, GFX_FLOAT, GFX_VERTEX_DATA_STATIC);
	gfx_inc_ref((gfx_object*)model->vertices);

	fread((char*)gfx_get_vertex_data_buffer(model->vertices), sizeof(float), 3*model->vertice_count, file);

	// read index
	model->index = (unsigned int*)malloc(sizeof(unsigned int)*model->indice_count);

	model->areas = (elf_model_area*)malloc(sizeof(elf_model_area)*model->area_count);
	memset(model->areas, 0x0, sizeof(elf_model_area)*model->area_count);

	for(i = 0; i < model->area_count; i++)
	{
		fread((char*)&model->areas[i].indice_count, sizeof(unsigned int), 1, file);
		if(model->areas[i].indice_count)
		{
			model->areas[i].index = gfx_create_vertex_data(model->areas[i].indice_count, GFX_UINT, GFX_VERTEX_DATA_STATIC);
			gfx_inc_ref((gfx_object*)model->areas[i].index);

			fread((char*)gfx_get_vertex_data_buffer(model->areas[i].index),
				sizeof(unsigned int), model->areas[i].indice_count, file);

			memcpy(&model->index[indices_read], gfx_get_vertex_data_buffer(model->areas[i].index),
				gfx_get_vertex_data_size_bytes(model->areas[i].index));

			indices_read += model->areas[i].indice_count;
		}
	}

	model->normals = gfx_create_vertex_data(3*model->vertice_count, GFX_FLOAT, GFX_VERTEX_DATA_STATIC);
	gfx_inc_ref((gfx_object*)model->normals);

	fread((char*)gfx_get_vertex_data_buffer(model->normals), sizeof(float), 3*model->vertice_count, file);

	// read tex coords
	if(is_tex_coords > 0)
	{
		model->tex_coords = gfx_create_vertex_data(2*model->vertice_count, GFX_FLOAT, GFX_VERTEX_DATA_STATIC);
		gfx_inc_ref((gfx_object*)model->tex_coords);

		fread((char*)gfx_get_vertex_data_buffer(model->tex_coords), sizeof(float), 2*model->vertice_count, file);
	}

	// read weights and bone ids
	if(is_weights_and_boneids > 0)
	{
		model->weights = (float*)malloc(sizeof(float)*4*model->vertice_count);
		for(i = 0; i < model->vertice_count; i++)
		{
			fread((char*)weights, sizeof(float), 4, file);
			if(weights[0] > 1.0) weights[0] = 1.0;
			if(weights[1] > 1.0) weights[1] = 1.0;
			if(weights[2] > 1.0) weights[2] = 1.0;
			if(weights[3] > 1.0) weights[3] = 1.0;
			if(weights[0] < 0.0) weights[0] = 0.0;
			if(weights[1] < 0.0) weights[1] = 0.0;
			if(weights[2] < 0.0) weights[2] = 0.0;
			if(weights[3] < 0.0) weights[3] = 0.0;
			length = 1.0f/(weights[0]+weights[1]+weights[2]+weights[3]);
			model->weights[i*4] = weights[0]*length;
			model->weights[i*4+1] = weights[1]*length;
			model->weights[i*4+2] = weights[2]*length;
			model->weights[i*4+3] = weights[3]*length;
		}
		model->boneids = (int*)malloc(sizeof(int)*4*model->vertice_count);
		for(i = 0; i < model->vertice_count; i++)
		{
			fread((char*)boneids, sizeof(short int), 4, file);
			model->boneids[i*4] = boneids[0];
			model->boneids[i*4+1] = boneids[1];
			model->boneids[i*4+2] = boneids[2];
			model->boneids[i*4+3] = boneids[3];
		}
	}

	vertex_buffer = gfx_get_vertex_data_buffer(model->vertices);

	// get bounding box values
	memcpy(&model->bb_min.x, vertex_buffer, sizeof(float)*3);
	memcpy(&model->bb_max.x, vertex_buffer, sizeof(float)*3);

	for(i = 3; i < model->vertice_count*3; i+=3)
	{
		if(vertex_buffer[i] < model->bb_min.x) model->bb_min.x = vertex_buffer[i];
		if(vertex_buffer[i+1] < model->bb_min.y) model->bb_min.y = vertex_buffer[i+1];
		if(vertex_buffer[i+2] < model->bb_min.z) model->bb_min.z = vertex_buffer[i+2];

		if(vertex_buffer[i] > model->bb_max.x) model->bb_max.x = vertex_buffer[i];
		if(vertex_buffer[i+1] > model->bb_max.y) model->bb_max.y = vertex_buffer[i+1];
		if(vertex_buffer[i+2] > model->bb_max.z) model->bb_max.z = vertex_buffer[i+2];
	}

	model->vertex_array = gfx_create_vertex_array(GFX_TRUE);
	gfx_inc_ref((gfx_object*)model->vertex_array);

	gfx_set_vertex_array_data(model->vertex_array, GFX_VERTEX, model->vertices);
	gfx_set_vertex_array_data(model->vertex_array, GFX_NORMAL, model->normals);
	if(is_tex_coords > 0) gfx_set_vertex_array_data(model->vertex_array, GFX_TEX_COORD, model->tex_coords);

	for(i = 0; i < model->area_count; i++)
	{
		if(model->areas[i].indice_count > 0)
		{
			model->areas[i].vertex_index = gfx_create_vertex_index(GFX_TRUE, model->areas[i].index);
			gfx_inc_ref((gfx_object*)model->areas[i].vertex_index);
		}
	}

	return model;
}

void elf_generate_model_tangent_vectors(elf_model *model)
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
	if(model->tri_mesh) elf_dec_ref((elf_object*)model->tri_mesh);

	free(model);

	global_obj_count--;
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
					material->non_lit_flag = model->non_lit_flag = *non_lit_flag = eng->non_lit_flag;

					shader_params->light_params.type = GFX_NONE;
				}

				gfx_set_shader_params(shader_params);

				shader_params->light_params.type = light_type;
			}
			gfx_draw_vertex_index(model->areas[i].vertex_index, GFX_TRIANGLES);
		}
	}
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
			if(material) elf_set_material_alpha_textures(material, shader_params);
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

