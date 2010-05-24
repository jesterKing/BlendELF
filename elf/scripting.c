
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
	elf_script *cur_script;
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

int elf_get_current_script_line()
{
	lua_Debug ar;

	if(!scr) return 0;

	lua_getstack(scr->L, 0 + 1, &ar);
	lua_getinfo(scr->L, "l", &ar);
	//luaL_where(scr->L, 0);

	return ar.currentline;
}

elf_script* elf_get_current_script()
{
	return scr->cur_script;
}

unsigned char elf_run_string(const char *str)
{
	int err;

	if(!scr || !str) return ELF_FALSE;

	err = 0;
	err = luaL_dostring(scr->L, str);
	if(err)
	{
		elf_set_error(ELF_CANT_RUN_STRING, "error: can't run string \"%s\"\n%s\n", str, lua_tostring(scr->L, -1));
		return ELF_FALSE;
	}

	return ELF_TRUE;
}

unsigned char elf_run_script(elf_script *script)
{
	int err;

	if(!scr || !script->text || script->error) return ELF_FALSE;

	if(scr->cur_script) elf_dec_ref((elf_object*)scr->cur_script);
	scr->cur_script = script;
	elf_inc_ref((elf_object*)scr->cur_script);

	err = 0;
	err = luaL_dostring(scr->L, script->text);
	if(err)
	{
		elf_set_error(ELF_CANT_RUN_SCRIPT, "error: can't run script \"%s\"\n%s\n", script->name, lua_tostring(scr->L, -1));

		script->error = ELF_TRUE;
		elf_dec_ref((elf_object*)scr->cur_script);
		scr->cur_script = NULL;
		return ELF_FALSE;
	}

	elf_dec_ref((elf_object*)scr->cur_script);
	scr->cur_script = NULL;
	return ELF_TRUE;
}

