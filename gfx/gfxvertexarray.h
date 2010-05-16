
gfx_vertex_data* gfx_create_vertex_data(int count, int format, int data_type)
{
	gfx_vertex_data *data;

	if(count <= 0) return NULL;
	if(!(format >= GFX_FLOAT && format < GFX_MAX_FORMATS)) return NULL;
	if(!(data_type >= GFX_VERTEX_DATA_STATIC && data_type < GFX_MAX_VERTEX_DATA_TYPES)) return NULL;

	data = (gfx_vertex_data*)malloc(sizeof(gfx_vertex_data));
	memset(data, 0x0, sizeof(gfx_vertex_data));
	data->type = GFX_VERTEX_DATA;

	data->count = count;
	data->format = format;
	data->size_bytes = driver->format_sizes[format]*count;
	data->data_type = data_type;
	data->data = malloc(data->size_bytes);
	memset(data->data, 0x0, data->size_bytes);

	gfx_global_obj_count++;

	return data;
}

void gfx_destroy_vertex_data(gfx_vertex_data *data)
{
	if(data->vbo) glDeleteBuffers(1, &data->vbo);

	free(data->data);
	free(data);

	gfx_global_obj_count--;
}

int gfx_get_vertex_data_count(gfx_vertex_data *data)
{
	return data->count;
}

int gfx_get_vertex_data_format(gfx_vertex_data *data)
{
	return data->format;
}

void* gfx_get_vertex_data_buffer(gfx_vertex_data *data)
{
	return data->data;
}

int gfx_get_vertex_data_size_bytes(gfx_vertex_data *data)
{
	return data->size_bytes;
}

void gfx_update_vertex_data(gfx_vertex_data *data)
{
	if(data->vbo)
	{
		glBindBuffer(GL_ARRAY_BUFFER, data->vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, data->size_bytes, data->data);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	data->changed = GFX_FALSE;
}

void gfx_update_vertex_data_sub_data(gfx_vertex_data *data, int start, int length)
{
	if(start > data->size_bytes) return;
	if(start+length > data->size_bytes) length -= (start+length)-data->size_bytes;

	if(data->vbo)
	{
		glBindBuffer(GL_ARRAY_BUFFER, data->vbo);
		glBufferSubData(GL_ARRAY_BUFFER, start, length, &((char*)data->data)[start]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	data->changed = GFX_FALSE;
}

void gfx_init_vertex_data_vbo(gfx_vertex_data *data)
{
	if(!data->vbo)
	{
		glGenBuffers(1, &data->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, data->vbo);
		glBufferData(GL_ARRAY_BUFFER, data->size_bytes, data->data,
			driver->vertex_data_draw_modes[data->data_type]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	data->changed = GFX_FALSE;
}

gfx_vertex_array* gfx_create_vertex_array(unsigned char gpu_data)
{
	gfx_vertex_array *vertex_array = NULL;

	vertex_array = (gfx_vertex_array*)malloc(sizeof(gfx_vertex_array));
	memset(vertex_array, 0x0, sizeof(gfx_vertex_array));
	vertex_array->type = GFX_VERTEX_ARRAY;

	vertex_array->gpu_data = !gpu_data == GFX_FALSE;

	gfx_global_obj_count++;

	return vertex_array;
}

void gfx_destroy_vertex_array(gfx_vertex_array *vertex_array)
{
	int i;

	for(i = 0; i < GFX_MAX_VERTEX_ARRAYS; i++)
	{
		if(vertex_array->varrs[i].data)
		{
			gfx_dec_ref((gfx_object*)vertex_array->varrs[i].data);
		}
	}

	gfx_global_obj_count--;

	free(vertex_array);
}

int gfx_get_vertex_array_vertex_count(gfx_vertex_array *vertex_array)
{
	return vertex_array->vertex_count;
}

void gfx_set_vertex_array_data(gfx_vertex_array *vertex_array, unsigned int target, gfx_vertex_data *data)
{
	gfx_varr *varr;
	int i;
	int varr_count;

	if(!(target >= GFX_VERTEX && target < GFX_MAX_VERTEX_ARRAYS))
	{
		printf("error: invalid target when setting vertex array data\n");
		return;
	}

	varr = &vertex_array->varrs[target];

	switch(target)
	{
		case GFX_VERTEX: varr->element_count = 3; break;
		case GFX_NORMAL: varr->element_count = 3; break;
		case GFX_TEX_COORD: varr->element_count = 2; break;
		case GFX_COLOR: varr->element_count = 4; break;
		case GFX_TANGENT: varr->element_count = 3; break;
		case GFX_WEIGHTS: varr->element_count = 4; break;
		case GFX_BONEIDS: varr->element_count = 4; break;
	}

	varr->vertex_count = data->count/varr->element_count;
	varr->vertex_size_bytes = driver->format_sizes[data->format]*varr->element_count;

	if(vertex_array->vertex_count == 0) vertex_array->vertex_count = varr->vertex_count;
	else if(varr->vertex_count < vertex_array->vertex_count) vertex_array->vertex_count = varr->vertex_count;
	else
	{
		varr_count = 0;
		for(i = 0; i < GFX_MAX_VERTEX_ARRAYS; i++)
			if(vertex_array->varrs[i].data) varr_count++;
		if(varr_count < 2) vertex_array->vertex_count = varr->vertex_count;
	}

	if(varr->data) gfx_dec_ref((gfx_object*)varr->data);
	varr->data = data;
	gfx_inc_ref((gfx_object*)varr->data);

	if(vertex_array->gpu_data) gfx_init_vertex_data_vbo(varr->data);
}

void gfx_set_vertex_array(gfx_vertex_array *vertex_array)
{
	int i;

	if(vertex_array->gpu_data)
	{
		for(i = 0; i < GFX_MAX_VERTEX_ARRAYS; i++)
		{
			if(vertex_array->varrs[i].data)
			{
				glEnableVertexAttribArray(i);
				glBindBuffer(GL_ARRAY_BUFFER, vertex_array->varrs[i].data->vbo);
				glVertexAttribPointer(i, vertex_array->varrs[i].element_count,
					driver->formats[vertex_array->varrs[i].data->format], GL_FALSE, 0, 0);
			}
			else
			{
				glDisableVertexAttribArray(i);
			}
		}

		driver->dirty_vertex_arrays = GFX_TRUE;
	}
	else
	{
		for(i = 0; i < GFX_MAX_VERTEX_ARRAYS; i++)
		{
			if(vertex_array->varrs[i].data)
			{
				glEnableVertexAttribArray(i);
				if(driver->dirty_vertex_arrays) glBindBuffer(GL_ARRAY_BUFFER, 0);
				glVertexAttribPointer(i, vertex_array->varrs[i].element_count,
					driver->formats[vertex_array->varrs[i].data->format],
					GL_FALSE, 0, vertex_array->varrs[i].data->data);
			}
			else
			{
				glDisableVertexAttribArray(i);
			}
		}

		driver->dirty_vertex_arrays = GFX_FALSE;
	}
}

void gfx_draw_vertex_array(gfx_vertex_array *vertex_array, unsigned int count, unsigned int draw_mode)
{
	if(count > vertex_array->vertex_count) count -= count-vertex_array->vertex_count;

	gfx_set_vertex_array(vertex_array);

	glDrawArrays(driver->draw_modes[draw_mode], 0, count);

	driver->vertices_drawn[draw_mode] += count;
}

gfx_vertex_index* gfx_create_vertex_index(unsigned char gpu_data, gfx_vertex_data *data)
{
	gfx_vertex_index *vertex_index = NULL;

	vertex_index = (gfx_vertex_index*)malloc(sizeof(gfx_vertex_index));
	memset(vertex_index, 0x0, sizeof(gfx_vertex_index));
	vertex_index->type = GFX_VERTEX_INDEX;

	vertex_index->data = data;
	gfx_inc_ref((gfx_object*)vertex_index->data);
	vertex_index->indice_count = gfx_get_vertex_data_count(data);

	vertex_index->gpu_data = !gpu_data == GFX_FALSE;

	if(vertex_index->gpu_data) gfx_init_vertex_data_vbo(data);

	gfx_global_obj_count++;

	return vertex_index;
}

void gfx_destroy_vertex_index(gfx_vertex_index *vertex_index)
{
	if(vertex_index->data) gfx_dec_ref((gfx_object*)vertex_index->data);

	free(vertex_index);

	gfx_global_obj_count--;
}

int gfx_get_vertex_index_indice_count(gfx_vertex_index *vertex_index)
{
	return vertex_index->indice_count;
}

void gfx_draw_vertex_index(gfx_vertex_index *vertex_index, unsigned int draw_mode)
{
	if(vertex_index->gpu_data)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_index->data->vbo);
		glDrawElements(driver->draw_modes[draw_mode], vertex_index->indice_count,
			driver->formats[vertex_index->data->format], 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	else
	{
		glDrawElements(driver->draw_modes[draw_mode],  vertex_index->indice_count,
			driver->formats[vertex_index->data->format], vertex_index->data->data);
	}

	driver->vertices_drawn[draw_mode] += vertex_index->indice_count;
}

