
void gfx_init_objects()
{
	memset(gfx_global_ref_count_table, 0x0, sizeof(int)*GFX_OBJECT_TYPE_COUNT);
}

void gfx_deinit_objects()
{
	if(gfx_get_global_ref_count() > 0)
	{
		elf_write_to_log("error: possible memory leak in GFX, [%d] references not dereferenced\n",
			gfx_get_global_ref_count());
		gfx_log_ref_count_table();
	}

	if(gfx_get_global_ref_count() < 0)
	{
		elf_write_to_log("error: possible double free in GFX, [%d] negative reference count\n",
			gfx_get_global_ref_count());
		gfx_log_ref_count_table();
	}

	if(gfx_get_global_obj_count() > 0)
		elf_write_to_log("error: possible memory leak in GFX, [%d] objects not destroyed\n",
			gfx_get_global_obj_count());

	if(gfx_get_global_obj_count() < 0)
		elf_write_to_log("error: possible double free in GFX, [%d] negative object count\n",
			gfx_get_global_obj_count());
}

void gfx_log_ref_count_table()
{
	int i;

	elf_write_to_log("---------- REF COUNT TABLE ----------\n");

	for(i = 0; i < GFX_OBJECT_TYPE_COUNT; i++)
	{
		elf_write_to_log("%d : %d\n", i, gfx_global_ref_count_table[i]);
	}

	elf_write_to_log("-------------------------------------\n");
}

void gfx_inc_ref(gfx_object *obj)
{
	gfx_global_ref_count++;
	gfx_global_ref_count_table[obj->type]++;
	obj->ref_count++;
}

void gfx_dec_ref(gfx_object *obj)
{
	gfx_global_ref_count--;
	gfx_global_ref_count_table[obj->type]--;
	obj->ref_count--;

	if(obj->ref_count < 1)
	{
		switch(obj->type)
		{
			case GFX_VERTEX_DATA: gfx_destroy_vertex_data((gfx_vertex_data*)obj); break;
			case GFX_VERTEX_ARRAY: gfx_destroy_vertex_array((gfx_vertex_array*)obj); break;
			case GFX_VERTEX_INDEX: gfx_destroy_vertex_index((gfx_vertex_index*)obj); break;
			case GFX_TEXTURE: gfx_destroy_texture((gfx_texture*)obj); break;
			default: elf_write_to_log("error: can not destroy unknown type\n"); break;
		}
	}
}

int gfx_get_object_type(gfx_object *obj)
{
	return obj->type;
}

int gfx_get_object_ref_count(gfx_object *obj)
{
	return obj->ref_count;
}

int gfx_get_global_ref_count()
{
	return gfx_global_ref_count;
}

int gfx_get_global_obj_count()
{
	return gfx_global_obj_count;
}

