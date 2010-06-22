
char* elf_create_string(const char *str)
{
	char *nstr;

	if(!str) return NULL;

	nstr = (char*)malloc(strlen(str)+1);
	memset(nstr, 0x0, sizeof(char)*(strlen(str)+1));

	memcpy(nstr, str, sizeof(char)*strlen(str));

	elf_inc_obj_count();

	return nstr;
}

void elf_destroy_string(char *str)
{
	free(str);

	elf_dec_obj_count();
}

elf_string* elf_create_string_object()
{
	elf_string *string;

	string = (elf_string*)malloc(sizeof(elf_string));
	memset(string, 0x0, sizeof(elf_string));
	string->type = ELF_STRING;

	return string;
}

void elf_destroy_string_object(elf_string *string)
{
	if(string->str) elf_destroy_string(string->str);

	free(string);
}

char* elf_remove_char_from_string(char *str, int idx)
{
	char *nstr;

	if(!strlen(str) || idx > (int)strlen(str)-1) return NULL;

	nstr = (char*)malloc(sizeof(char)*(strlen(str)));

	if(sizeof(char)*idx > 0)
		memcpy(nstr, str, sizeof(char)*idx);
	if(sizeof(char)*(strlen(str)-idx) > 0)
		memcpy(&nstr[idx], &str[idx+1], sizeof(char)*(strlen(str)-idx));

	elf_inc_obj_count();

	return nstr;
}

char* elf_append_char_to_string(char *str, char c)
{
	char *nstr;

	nstr = (char*)malloc(sizeof(char)*(strlen(str)+2));
	memcpy(nstr, str, sizeof(char)*strlen(str));
	nstr[strlen(str)] = c;
	nstr[strlen(str)+1] = '\0';

	elf_inc_obj_count();

	return nstr;
}

char* elf_insert_char_to_string(char *str, int idx, char c)
{
	char *nstr;

	if(idx < 0 || idx > (int)strlen(str)) return NULL;

	nstr = (char*)malloc(sizeof(char)*(strlen(str)+2));

	if(sizeof(char)*idx > 0)
		memcpy(nstr, str, sizeof(char)*idx);
	if(sizeof(char)*(strlen(str)-idx) > 0)
		memcpy(&nstr[idx+1], &str[idx], sizeof(char)*(strlen(str)-idx));
	nstr[idx] = c;
	nstr[strlen(str)+1] = '\0';

	elf_inc_obj_count();

	return nstr;
}

char* elf_merge_strings(const char *str1, const char *str2)
{
	char *nstr;

	if(!str1 || !str2) return NULL;

	nstr = (char*)malloc(sizeof(char)*(strlen(str1)+strlen(str2)+1));

	memcpy(nstr, str1, sizeof(char)*strlen(str1));
	memcpy(&nstr[strlen(str1)], str2, sizeof(char)*strlen(str2));
	nstr[strlen(str1)+strlen(str2)] = '\0';

	elf_inc_obj_count();

	return nstr;
}

char* elf_merge_strings3(const char *str1, const char *str2, const char *str3)
{
	char *nstr;
	int bytes;

	if(!str1 || !str2 || !str3 || (!strlen(str1) && !strlen(str2) && !strlen(str3))) return NULL;

	bytes = strlen(str1)+strlen(str2)+strlen(str3)+1;

	nstr = (char*)malloc(sizeof(char)*bytes);
	memset(nstr, 0x0, sizeof(char)*bytes);

	memcpy(nstr, str1, sizeof(char)*strlen(str1));
	memcpy(&nstr[strlen(str1)], str2, sizeof(char)*strlen(str2));
	memcpy(&nstr[strlen(str1)+strlen(str2)], str3, sizeof(char)*strlen(str3));

	elf_inc_obj_count();

	return nstr;
}

char* elf_sub_string(char *str, int start, int len)
{
	char *nstr;

	if(start+len < 1)
	{
		nstr = (char*)malloc(sizeof(char)*1);
		nstr[0] = '\0';
		elf_inc_obj_count();
		return nstr;
	}

	if(start < 0 || start > (int)strlen(str)-1 ||
		len < 0 || start+len > (int)strlen(str))
	{
		nstr = (char*)malloc(sizeof(char)*1);
		nstr[0] = '\0';
		elf_inc_obj_count();
		return nstr;
	}

	nstr = (char*)malloc(sizeof(char)*len+1);
	memcpy(nstr, &str[start], sizeof(char)*len);
	nstr[len] = '\0';

	elf_inc_obj_count();

	return nstr;
}

unsigned char elf_is_char_number(char c)
{
	char num_chars[14] = "0123456789.-+";
	int i;

	for(i = 0; i < 13; i++)
	{
		if(c == num_chars[i]) return ELF_TRUE;
	}

	return ELF_FALSE;
}

unsigned char elf_is_string_number(const char *str)
{
	char num_chars[12] = "0123456789.";
	int dots;
	int i;
	int offset;

	if((str[0] == '-' || str[0] == '+') && strlen(str) > 1)
		offset = 1;

	dots = 0;
	for(i = offset; i < (int)strlen(str); i++)
	{
		if(str[i] == '.') dots++;
	}

	if(dots > 1) return ELF_FALSE;

	num_chars[11] = '\0';

	for(i = offset; i < (int)strlen(str); i++)
	{
		if(!strchr(num_chars, str[i])) return ELF_FALSE;
	}

	return ELF_TRUE;
}

unsigned char elf_is_string_positive_int(const char *str)
{
	char num_chars[11] = "0123456789";
	int i;

	num_chars[10] = '\0';

	for(i = 0; i < (int)strlen(str); i++)
	{
		if(!strchr(num_chars, str[i])) return ELF_FALSE;
	}

	return ELF_TRUE;
}

int elf_rfind_char_from_string(const char *str, char chr)
{
	int result = -1;

	for(result = strlen(str)-1; result > -1; result--)
	{
		if(str[result] == chr) break;
	}

	return result;
}

int elf_rfind_chars_from_string(const char *str, char *chrs)
{
	int i;
	int result = -1;

	for(i = 0; i < strlen(chrs); i++)
	{
		for(result = strlen(str)-1; result > -1; result--)
		{
			if(str[result] == chrs[i]) return result;
		}
	}

	return result;
}

char* elf_get_file_folder(const char *file_path)
{
	char *str;
	char *nstr;

	str = elf_create_string(file_path);

	if(elf_rfind_char_from_string(file_path, '/') != -1)
	{
		nstr = elf_sub_string(str, 0, elf_rfind_char_from_string(str, '/')+1);
		elf_destroy_string(str);
		str = nstr;
		nstr = NULL;
	}

	if(elf_rfind_char_from_string(file_path, '\\') != -1)
	{
		nstr = elf_sub_string(str, 0, elf_rfind_char_from_string(str, '\\')+1);
		elf_destroy_string(str);
		str = nstr;
		nstr = NULL;
	}

	return str;
}

