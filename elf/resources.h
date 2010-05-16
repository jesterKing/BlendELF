
elf_resources* elf_create_resources()
{
	elf_resources *resources;

	resources = (elf_resources*)malloc(sizeof(elf_resources));
	memset(resources, 0x0, sizeof(elf_resources));
	resources->type = ELF_RESOURCES;

	resources->textures = elf_create_list();
	resources->models = elf_create_list();

	elf_inc_ref((elf_object*)resources->textures);
	elf_inc_ref((elf_object*)resources->models);

	global_obj_count++;

	return resources;
}

void elf_update_resources(elf_resources *resources)
{
	elf_texture *tex;
	elf_model *mdl;

	for(tex = (elf_texture*)elf_begin_list(resources->textures); tex;
		tex = (elf_texture*)elf_next_in_list(resources->textures))
	{
		if(elf_get_object_ref_count((elf_object*)tex) < 2)
		{
			elf_remove_from_list(resources->textures, (elf_object*)tex);
		}
	}

	for(mdl = (elf_model*)elf_begin_list(resources->models); mdl;
		mdl = (elf_model*)elf_next_in_list(resources->models))
	{
		if(elf_get_object_ref_count((elf_object*)mdl) < 2)
		{
			elf_remove_from_list(resources->models, (elf_object*)mdl);
		}
	}
}

void elf_destroy_resources(elf_resources *resources)
{
	elf_dec_ref((elf_object*)resources->textures);
	elf_dec_ref((elf_object*)resources->models);

	free(resources);

	global_obj_count--;
}

