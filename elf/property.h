
elf_property* elf_create_property(const char *name)
{
	elf_property *property;

	property = (elf_property*)malloc(sizeof(elf_property));
	memset(property, 0x0, sizeof(elf_property));
	property->type = ELF_PROPERTY;

	property->property_type = ELF_PROPERTY_INT;

	if(property) property->name = elf_create_string(name);

	global_obj_count++;

	return property;
}

void elf_destroy_property(elf_property *property)
{
	if(property->name) elf_destroy_string(property->name);
	if(property->sval) elf_destroy_string(property->sval);

	free(property);

	global_obj_count--;
}

int elf_get_property_type(elf_property *property)
{
	return property->property_type;
}

int elf_get_property_int(elf_property *property)
{
	return property->ival;
}

float elf_get_property_float(elf_property *property)
{
	return property->fval;
}

const char* elf_get_property_string(elf_property *property)
{
	return property->sval;
}

unsigned char elf_get_property_bool(elf_property *property)
{
	return property->bval;
}

void elf_set_property_int(elf_property *property, int ival)
{
	property->property_type = ELF_PROPERTY_INT;
	property->ival = ival;
	property->fval = 0.0;
	if(property->sval) elf_destroy_string(property->sval);
	property->sval = NULL;
	property->bval = ELF_FALSE;
}

void elf_set_property_float(elf_property *property, float fval)
{
	property->property_type = ELF_PROPERTY_FLOAT;
	property->ival = 0;
	property->fval = fval;
	if(property->sval) elf_destroy_string(property->sval);
	property->sval = NULL;
	property->bval = ELF_FALSE;
}

void elf_set_property_string(elf_property *property, const char *sval)
{
	property->property_type = ELF_PROPERTY_STRING;
	property->ival = 0;
	property->fval = 0.0;
	if(property->sval) elf_destroy_string(property->sval);
	property->sval = elf_create_string(sval);
	property->bval = ELF_FALSE;
}

void elf_set_property_bool(elf_property *property, unsigned char bval)
{
	property->property_type = ELF_PROPERTY_BOOL;
	property->ival = 0;
	property->fval = 0.0;
	if(property->sval) elf_destroy_string(property->sval);
	property->sval = NULL;
	property->bval = !bval == ELF_FALSE;
}

