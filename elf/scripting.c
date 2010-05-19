
#include "default.h"

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "gfx.h"
#include "blendelf.h"
#include "types.h"

int luaopen_elf(lua_State* L);

struct elf_scripting {
	ELF_OBJECT_HEADER;
	struct lua_State *L;
};

elf_scripting *scr = NULL;

elf_scripting* elf_create_scripting()
{
	elf_scripting *scripting;

	scripting = malloc(sizeof(elf_scripting));
	memset(scripting, 0x0, sizeof(elf_scripting));
	scripting->type = ELF_SCRIPTING;

	scripting->L = lua_open();
	if(!scripting->L)
	{
		elf_set_error(ELF_CANT_INITIALIZE, "error: failed to initialize lua\n");
		elf_destroy_scripting(scripting);
		return NULL;
	}
	luaL_openlibs(scripting->L);
	luaopen_elf(scripting->L);

	return scripting;
}

void elf_destroy_scripting(elf_scripting *scripting)
{
	if(scripting->L) lua_close(scripting->L);

	free(scripting);
}

unsigned char elf_init_scripting()
{
	if(scr)
	{
		elf_write_to_log("warning: cannot initialize scripting twice\n");
		return ELF_TRUE;
	}

	scr = elf_create_scripting();
	if(!scr) return ELF_FALSE;

	elf_inc_ref((elf_object*)scr);

	return ELF_TRUE;
}

void elf_update_scripting()
{
	if(scr) lua_gc(scr->L, LUA_GCCOLLECT, 0);
}

void elf_deinit_scripting()
{
	if(!scr) return;

	elf_dec_ref((elf_object*)scr);
	scr = NULL;
}

unsigned char elf_run_string(const char *str)
{
	if(!scr) return ELF_FALSE;

	int err;

	if(!str || !strlen(str))

	err = 0;
	err = luaL_dostring(scr->L, str);
	if(err)
	{
		elf_set_error(ELF_CANT_RUN_STRING, "error: can't run string \"%s\"\n%s\n", str, lua_tostring(scr->L, -1));
		return ELF_FALSE;
	}

	return ELF_TRUE;
}

