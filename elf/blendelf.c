
#include "default.h"

#include "gfx.h"
#include "blendelf.h"
#include "types.h"

elf_general *gen = NULL;
elf_context *ctx = NULL;
elf_engine *eng = NULL;

#include "general.h"
#include "config.h"
#include "log.h"
#include "resource.h"
#include "str.h"
#include "list.h"
#include "context.h"
#include "engine.h"
#include "frameplayer.h"
#include "timer.h"
#include "image.h"
#include "texture.h"
#include "material.h"
#include "ipo.h"
#include "camera.h"
#include "model.h"
#include "entity.h"
#include "light.h"
#include "scene.h"
#include "pak.h"
#include "postprocess.h"
#include "script.h"
#include "armature.h"
#include "property.h"
#include "actor.h"
#include "font.h"
#include "gui.h"
#include "sst.h"
#include "particles.h"
#include "sprite.h"

#ifdef ELF_PLAYER

int main()
{
	elf_config *config;
	elf_script *script;

	elf_init_general();

	if(!(config = elf_read_config("config.txt")))
		config = elf_create_config();

	if(!elf_init(config->window_size[0], config->window_size[1], "BlendELF", !config->fullscreen == ELF_FALSE, config->log))
	{
		elf_set_error(ELF_CANT_INITIALIZE, "error: can't initialize engine\n");
		elf_destroy_config(config);
		return -1;
	}

	elf_set_texture_anisotropy(config->texture_anisotropy);
	elf_set_shadow_map_size(config->shadow_map_size);

	script = elf_create_script_from_file("init.lua");
	if(script)
	{
		elf_inc_ref((elf_object*)script);
		elf_run_script(script);
		elf_dec_ref((elf_object*)script);
	}
	else
	{
		if(!elf_load_scene(config->start))
		{
			elf_destroy_config(config);
			elf_deinit();
			return -1;
		}

		while(elf_run());
	}

	elf_destroy_config(config);

	elf_deinit();

	return 0;
}

#endif	// ELF_PLAYER
