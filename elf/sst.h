
char* elf_read_next(const char *text, int *pos)
{
	int start, end;
	char *str;

	start = *pos;

	while(text[start] == ' ' || text[start] == '\t' || text[start] == '\n' || text[start] == '\r')
	{
		(*pos)++;
		start++;
	}

	end = start;

	while(text[end] != ' ' && text[end] != '\t' && text[end] != '\n' && text[end] != '\r' && text[end] != '\0')
	{
		(*pos)++;
		end++;
	}

	if(start == end) return NULL;

	str = (char*)malloc(sizeof(char)*(end-start+1));
	memcpy(str, &text[start], sizeof(char)*(end-start));
	str[end-start] = '\0';

	global_obj_count++;

	return str;
}

char* elf_read_sst_string(const char* text, int *pos)
{
	return elf_read_next(text, pos);
}

float elf_read_sst_float(const char* text, int *pos)
{
	char *str;
	float val;

	str = elf_read_next(text, pos);
	val = (float)atof(str);
	elf_destroy_string(str);

	return val;
}

void elf_read_sst_floats(const char* text, int *pos, int n, float *params)
{
	int i;
	char *str;

	for(i = 0; i < n && (str = elf_read_next(text, pos)); i++)
	{
		params[i] = (float)atof(str);
		elf_destroy_string(str);
	}
}

int elf_read_sst_int(const char* text, int *pos)
{
	char *str;
	int val;

	str = elf_read_next(text, pos);
	val = atoi(str);
	elf_destroy_string(str);

	return val;
}

void elf_read_sst_ints(const char* text, int *pos, int n, int *params)
{
	int i;
	char *str;

	for(i = 0; i < n && (str = elf_read_next(text, pos)); i++)
	{
		params[i] = atoi(str);
		elf_destroy_string(str);
	}
}

unsigned char elf_read_sst_bool(const char* text, int *pos)
{
	char *str;
	unsigned char result;

	str = elf_read_next(text, pos);

	if(!strcmp(str, "FALSE")) result = ELF_FALSE;
	else result = ELF_TRUE;

	elf_destroy_string(str);

	return result;
}

