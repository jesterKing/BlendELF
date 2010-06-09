
elf_resource* elf_get_resource_by_name(elf_list *named_objects, const char *name)
{
	elf_resource *obj;

	for(obj = (elf_resource*)elf_begin_list(named_objects); obj;
		obj = (elf_resource*)elf_next_in_list(named_objects))
	{
		if(!strcmp(obj->name, name)) return obj;
	}

	return NULL;
}

void elf_set_unique_name_for_resource(elf_list *named_objects, elf_resource *object)
{
	char *tname;
	char *nname;
	int dot_pos;
	int num;

	if(object->name && strlen(object->name))
	{
		if(!elf_get_resource_by_name(named_objects, object->name))
		{
			printf("-: %s :: %d\n", object->name, object->type);
			return;
		}

		dot_pos = elf_rfind_char_from_string(object->name, '.');

		if(dot_pos == strlen(object->name)-1)
		{
			tname = (char*)malloc(sizeof(char)*(strlen(object->name)+1));
			memcpy(tname, object->name, sizeof(char)*strlen(object->name));
			tname[strlen(object->name)] = '\0';
		}
		else if(dot_pos == -1 || !elf_is_string_positive_int(&object->name[dot_pos+1]))
		{
			tname = (char*)malloc(sizeof(char)*(strlen(object->name)+2));
			sprintf(tname, "%s.", object->name);
			tname[strlen(object->name)+1] = '\0';
		}
		else
		{
			tname = (char*)malloc(sizeof(char)*(dot_pos+2));
			memcpy(tname, object->name, sizeof(char)*(dot_pos+1));
			tname[dot_pos+1] = '\0';
		}
	}
	else
	{
		tname = malloc(sizeof(char)*8);
		memcpy(tname, "Object.", sizeof(char)*7);
		tname[7] = '\0';
	}

	nname = (char*)malloc(sizeof(char)*(strlen(tname)+12));
	memset(nname, 0x0, sizeof(char)*(strlen(tname)+12));

	num = 1;
	sprintf(nname, "%s%d", tname, num);

	while(elf_get_resource_by_name(named_objects, nname))
	{
		memset(nname, 0x0, sizeof(char)*(strlen(tname)+12));
		num++;
		sprintf(nname, "%s%d", tname, num);
		if(num < -1) break;
	}

	if(object->name) elf_destroy_string(object->name);
	object->name = elf_create_string(nname);

	printf("+: %s :: %d\n", object->name, object->type);

	free(nname);
	free(tname);
}

