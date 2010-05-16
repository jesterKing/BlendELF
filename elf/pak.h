
elf_pak_index* elf_create_pak_index()
{
	elf_pak_index *index;

	index = (elf_pak_index*)malloc(sizeof(elf_pak_index));
	memset(index, 0x0, sizeof(elf_pak_index));
	index->type = ELF_PAK_INDEX;

	global_obj_count++;

	return index;
}

void elf_destroy_pak_index(elf_pak_index *index)
{
	if(index->name) elf_destroy_string(index->name);

	free(index);

	global_obj_count--;
}

elf_pak* elf_create_pak_from_file(const char *file_path)
{
	elf_pak *pak;
	elf_pak_index *index;
	FILE *file;
	int magic;
	int index_count;
	int i;

	unsigned char type;
	char name[64];
	unsigned int offset;

	file = fopen(file_path, "rb");
	if(!file)
	{
		elf_write_to_log("error: could not open \"%s\"\n", file_path);
		return NULL;
	}

	magic = 0;
	fread((char*)&magic, sizeof(int), 1, file);

	if(magic != 179532100)
	{
		elf_write_to_log("error: \"%s\" is not a elf pak file\n", file_path);
		fclose(file);
		return NULL;
	}

	pak = (elf_pak*)malloc(sizeof(elf_pak));
	memset(pak, 0x0, sizeof(elf_pak));
	pak->type = ELF_PAK;

	global_obj_count++;

	pak->file_path = elf_create_string(file_path);

	pak->indexes = elf_create_list();
	elf_inc_ref((elf_object*)pak->indexes);

	index_count = 0;
	fread((char*)&index_count, sizeof(unsigned int), 1, file);

	for(i = 0; i < index_count; i++)
	{
		type = 0;
		offset = 0;
		fread((char*)&type, sizeof(unsigned char), 1, file);

		switch(type)
		{
			case ELF_TEXTURE: pak->texture_count++; break;
			case ELF_MATERIAL: pak->material_count++; break;
			case ELF_MODEL: pak->model_count++; break;
			case ELF_CAMERA: pak->camera_count++; break;
			case ELF_ENTITY: pak->entity_count++; break;
			case ELF_LIGHT: pak->light_count++; break;
			case ELF_ARMATURE: pak->armature_count++; break;
			case ELF_SCENE:	pak->scene_count++; break;
			case ELF_SCRIPT: pak->script_count++; break;
		}

		fread(name, sizeof(char), 64, file);
		fread((char*)&offset, sizeof(unsigned int), 1, file);

		index = elf_create_pak_index();
		index->index_type = type;
		index->name = elf_create_string(name);
		index->offset = offset;

		elf_append_to_list(pak->indexes, (elf_object*)index);
	}

	fclose(file);

	return pak;
}

void elf_destroy_pak(elf_pak *pak)
{
	if(pak->file_path) elf_destroy_string(pak->file_path);

	elf_dec_ref((elf_object*)pak->indexes);

	free(pak);

	global_obj_count--;
}

const char* elf_get_pak_file_path(elf_pak *pak)
{
	return pak->file_path;
}

int elf_get_pak_index_count(elf_pak *pak)
{
	return elf_get_list_length(pak->indexes);
}

elf_pak_index* elf_get_pak_index_by_name(elf_pak *pak, const char *name, unsigned char type)
{
	elf_pak_index *index;

	for(index = (elf_pak_index*)elf_begin_list(pak->indexes); index;
		index = (elf_pak_index*)elf_next_in_list(pak->indexes))
	{
		if(!strcmp(index->name, name) && index->index_type == type) return index;
	}

	return NULL;
}

elf_pak_index* elf_get_pak_index_by_index(elf_pak *pak, int idx)
{
	if(idx < 0 || idx > elf_get_list_length(pak->indexes)-1) return NULL;
	return (elf_pak_index*)elf_get_item_from_list(pak->indexes, idx);
}

unsigned char elf_get_pak_index_type(elf_pak_index *index)
{
	return index->type;
}

const char* elf_get_pak_index_name(elf_pak_index *index)
{
	return index->name;
}

int elf_get_pak_index_offset(elf_pak_index *index)
{
	return index->offset;
}

