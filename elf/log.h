
void elf_start_log(const char *text)
{
	FILE *file;

	file = fopen(gen->log, "w");
	if(!file) return;

	fwrite(text, sizeof(char), strlen(text), file);
	fclose(file);
}

void elf_write_to_log(const char *fmt, ...)
{
	va_list list;
	const char *p, *s;
	int d;
	double f;
	FILE *file;

	va_start(list, fmt);

	file = fopen(gen->log, "a");
	if(!file) fopen(gen->log, "w");

	for(p = fmt; *p; ++p)
	{
		if(*p != '%')
		{
			putc(*p, stdout);
			if(file) putc(*p, file);
		}
		else
		{
			switch(*++p)
			{
				case 's':
					s = va_arg(list, char*);
					if(s == NULL) continue;
					printf("%s", s);
					if(file) fprintf(file, "%s", s);
					continue;
				case 'd':
					d = va_arg(list, int);
					printf("%d", d);
					if(file) fprintf(file, "%d", d);
					continue;
				case 'f':
					f = va_arg(list, double);
					printf("%f", f);
					if(file) fprintf(file, "%f", f);
					continue;
			}
		}
	}

	if(file) fclose(file);
}

void elf_set_error(int code, const char *fmt, ...)
{
	va_list list;
	const char *p, *s;
	int d;
	double f;
	char* err_str;
	char* tmp_str;
	char num[32];
	FILE *file;

	va_start(list, fmt);

	file = fopen(gen->log, "a");
	if(!file) fopen(gen->log, "w");

	err_str = elf_create_string("");

	for(p = fmt; *p; ++p)
	{
		if(*p != '%')
		{
			putc(*p, stdout);
			if(file) putc(*p, file);
			tmp_str = elf_append_char_to_string(err_str, *p);
			elf_destroy_string(err_str);
			err_str = tmp_str;
		}
		else
		{
			switch(*++p)
			{
				case 's':
					s = va_arg(list, char*);
					if(s == NULL) continue;
					printf("%s", s);
					if(file) fprintf(file, "%s", s);

					tmp_str = elf_merge_strings(err_str, s);
					elf_destroy_string(err_str);
					err_str = tmp_str;
					continue;
				case 'd':
					d = va_arg(list, int);
					printf("%d", d);
					if(file) fprintf(file, "%d", d);

					memset(num, 0x0, sizeof(char)*32);
					sprintf(num, "%d", d);

					tmp_str = elf_merge_strings(err_str, num);
					elf_destroy_string(err_str);
					err_str = tmp_str;
					continue;
				case 'f':
					f = va_arg(list, double);
					printf("%f", f);
					if(file) fprintf(file, "%f", f);

					memset(num, 0x0, sizeof(char)*32);
					sprintf(num, "%f", f);

					tmp_str = elf_merge_strings(err_str, num);
					elf_destroy_string(err_str);
					err_str = tmp_str;
					continue;
			}
		}
	}

	gen->err_code = code;

	if(gen->err_str) elf_destroy_string(gen->err_str);
	gen->err_str = elf_create_string(err_str);
	elf_destroy_string(err_str);

	if(file) fclose(file);
}

void elf_set_error_no_save(int code, const char *fmt, ...)
{
	va_list list;
	const char *p, *s;
	int d;
	double f;
	char* err_str;
	char* tmp_str;
	char num[32];

	va_start(list, fmt);

	err_str = elf_create_string("");

	for(p = fmt; *p; ++p)
	{
		if(*p != '%')
		{
			putc(*p, stdout);

			tmp_str = elf_append_char_to_string(err_str, *p);
			elf_destroy_string(err_str);
			err_str = tmp_str;
		}
		else
		{
			switch(*++p)
			{
				case 's':
					s = va_arg(list, char*);
					if(s == NULL) continue;
					printf("%s", s);

					tmp_str = elf_merge_strings(err_str, s);
					elf_destroy_string(err_str);
					err_str = tmp_str;
					continue;
				case 'd':
					d = va_arg(list, int);
					printf("%d", d);

					memset(num, 0x0, sizeof(char)*32);
					sprintf(num, "%d", d);

					tmp_str = elf_merge_strings(err_str, num);
					elf_destroy_string(err_str);
					err_str = tmp_str;
					continue;
				case 'f':
					f = va_arg(list, double);
					printf("%f", f);

					memset(num, 0x0, sizeof(char)*32);
					sprintf(num, "%f", f);

					tmp_str = elf_merge_strings(err_str, num);
					elf_destroy_string(err_str);
					err_str = tmp_str;
					continue;
			}
		}
	}

	gen->err_code = code;

	if(gen->err_str) elf_destroy_string(gen->err_str);
	gen->err_str = elf_create_string(err_str);
	elf_destroy_string(err_str);
}

