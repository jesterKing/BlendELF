#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "gfx.h"
#include "blendelf.h"
#include "binds.h"

#ifdef ELF_WINDOWS
	#ifndef ELF_PLAYER
ELF_API bool ELF_APIENTRY elfInitWithHWND(int width, int height, const char* title, bool fullscreen, HWND hwnd)
{
	return elf_init_with_hwnd(width, height, title, fullscreen, hwnd);
}
ELF_API HWND ELF_APIENTRY elfGetWindowHWND()
{
	return elf_get_window_hwnd();
}
	#endif
#endif

ELF_API elf_handle::elf_handle()
{
	obj = NULL;
}
elf_handle::elf_handle(elf_object *object)
{
	if(object) elf_inc_ref(object);
	obj = object;
}
ELF_API elf_handle::elf_handle(const elf_handle &handle)
{
	if(handle.obj) elf_inc_ref(handle.obj);
	obj = handle.obj;
}
ELF_API elf_handle::~elf_handle()
{
	if(obj) elf_dec_ref(obj);
}
elf_handle& elf_handle::operator=(elf_object *object)
{
	if(obj)
	{
		elf_dec_ref(obj);
		obj = NULL;
	}

	if(object) elf_inc_ref(object);
	obj = object;

	return *this;
}
ELF_API elf_handle& elf_handle::operator=(elf_handle &handle)
{
	if(obj)
	{
		elf_dec_ref(obj);
		obj = NULL;
	}

	if(handle.obj) elf_inc_ref(handle.obj);
	obj = handle.obj;

	return *this;
}
ELF_API elf_handle& elf_handle::operator=(const elf_handle &handle)
{
	if(obj)
	{
		elf_dec_ref(obj);
		obj = NULL;
	}

	if(handle.obj) elf_inc_ref(handle.obj);
	obj = handle.obj;

	return *this;
}

ELF_API bool ELF_APIENTRY elfIsObject(elf_handle obj)
{
	if(obj.get()) return true;
	return false;
}
ELF_API int ELF_APIENTRY elfGetObjectType(elf_handle obj)
{
	if(!obj.get())
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetObjectType() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetObjectType() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_object_type((elf_object*)obj.get());
}
ELF_API int ELF_APIENTRY elfGetObjectRefCount(elf_handle obj)
{
	if(!obj.get())
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetObjectRefCount() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetObjectRefCount() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_object_ref_count((elf_object*)obj.get());
}
ELF_API int ELF_APIENTRY elfGetGlobalRefCount()
{
	return elf_get_global_ref_count();
}
ELF_API int ELF_APIENTRY elfGetGlobalObjCount()
{
	return elf_get_global_obj_count();
}
ELF_API bool ELF_APIENTRY elfIsActor(elf_handle obj)
{
	if(!obj.get())
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: IsActor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "IsActor() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_is_actor((elf_object*)obj.get());
}
ELF_API bool ELF_APIENTRY elfIsGuiObject(elf_handle obj)
{
	if(!obj.get())
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: IsGuiObject() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "IsGuiObject() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_is_gui_object((elf_object*)obj.get());
}
ELF_API elf_handle ELF_APIENTRY elfCreateList()
{
	elf_handle handle;
	handle = (elf_object*)elf_create_list();
	return handle;
}
ELF_API int ELF_APIENTRY elfGetListLength(elf_handle list)
{
	if(!list.get() || elf_get_object_type(list.get()) != ELF_LIST)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetListLength() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetListLength() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_list_length((elf_list*)list.get());
}
ELF_API void ELF_APIENTRY elfInsertToList(elf_handle list, int idx, elf_handle obj)
{
	if(!list.get() || elf_get_object_type(list.get()) != ELF_LIST)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: InsertToList() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "InsertToList() -> invalid handle\n");
		}
		return;
	}
	if(!obj.get())
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: InsertToList() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "InsertToList() -> invalid handle\n");
		}
		return;
	}
	elf_insert_to_list((elf_list*)list.get(), idx, (elf_object*)obj.get());
}
ELF_API void ELF_APIENTRY elfAppendToList(elf_handle list, elf_handle obj)
{
	if(!list.get() || elf_get_object_type(list.get()) != ELF_LIST)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: AppendToList() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "AppendToList() -> invalid handle\n");
		}
		return;
	}
	if(!obj.get())
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: AppendToList() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "AppendToList() -> invalid handle\n");
		}
		return;
	}
	elf_append_to_list((elf_list*)list.get(), (elf_object*)obj.get());
}
ELF_API bool ELF_APIENTRY elfRemoveFromList(elf_handle list, elf_handle obj)
{
	if(!list.get() || elf_get_object_type(list.get()) != ELF_LIST)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveFromList() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveFromList() -> invalid handle\n");
		}
		return false;
	}
	if(!obj.get())
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveFromList() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveFromList() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_remove_from_list((elf_list*)list.get(), (elf_object*)obj.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetItemFromList(elf_handle list, int idx)
{
	elf_handle handle;
	if(!list.get() || elf_get_object_type(list.get()) != ELF_LIST)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetItemFromList() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetItemFromList() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_item_from_list((elf_list*)list.get(), idx);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfBeginList(elf_handle list)
{
	elf_handle handle;
	if(!list.get() || elf_get_object_type(list.get()) != ELF_LIST)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: BeginList() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "BeginList() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_begin_list((elf_list*)list.get());
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfNextInList(elf_handle list)
{
	elf_handle handle;
	if(!list.get() || elf_get_object_type(list.get()) != ELF_LIST)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: NextInList() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "NextInList() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_next_in_list((elf_list*)list.get());
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfRbeginList(elf_handle list)
{
	elf_handle handle;
	if(!list.get() || elf_get_object_type(list.get()) != ELF_LIST)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RbeginList() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RbeginList() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_rbegin_list((elf_list*)list.get());
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfRnextInList(elf_handle list)
{
	elf_handle handle;
	if(!list.get() || elf_get_object_type(list.get()) != ELF_LIST)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RnextInList() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RnextInList() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_rnext_in_list((elf_list*)list.get());
	return handle;
}
ELF_API void ELF_APIENTRY elfSeekList(elf_handle list, elf_handle ptr)
{
	if(!list.get() || elf_get_object_type(list.get()) != ELF_LIST)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SeekList() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SeekList() -> invalid handle\n");
		}
		return;
	}
	if(!ptr.get())
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SeekList() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SeekList() -> invalid handle\n");
		}
		return;
	}
	elf_seek_list((elf_list*)list.get(), (elf_object*)ptr.get());
}
ELF_API void ELF_APIENTRY elfRseekList(elf_handle list, elf_handle ptr)
{
	if(!list.get() || elf_get_object_type(list.get()) != ELF_LIST)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RseekList() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RseekList() -> invalid handle\n");
		}
		return;
	}
	if(!ptr.get())
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RseekList() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RseekList() -> invalid handle\n");
		}
		return;
	}
	elf_rseek_list((elf_list*)list.get(), (elf_object*)ptr.get());
}
ELF_API void ELF_APIENTRY elfSetTitle(const char* title)
{
	elf_set_title(title);
}
ELF_API int ELF_APIENTRY elfGetWindowWidth()
{
	return elf_get_window_width();
}
ELF_API int ELF_APIENTRY elfGetWindowHeight()
{
	return elf_get_window_height();
}
ELF_API bool ELF_APIENTRY elfIsFullscreen()
{
	return (bool)elf_is_fullscreen();
}
ELF_API const char* ELF_APIENTRY elfGetTitle()
{
	return elf_get_title();
}
ELF_API int ELF_APIENTRY elfGetMultisamples()
{
	return elf_get_multisamples();
}
ELF_API double ELF_APIENTRY elfGetTime()
{
	return elf_get_time();
}
ELF_API bool ELF_APIENTRY elfIsWindowOpened()
{
	return (bool)elf_is_window_opened();
}
ELF_API elf_vec2i ELF_APIENTRY elfGetMousePosition()
{
	return elf_get_mouse_position();
}
ELF_API elf_vec2i ELF_APIENTRY elfGetMouseForce()
{
	return elf_get_mouse_force();
}
ELF_API void ELF_APIENTRY elfSetMousePosition(int x, int y)
{
	elf_set_mouse_position(x, y);
}
ELF_API void ELF_APIENTRY elfHideMouse(bool hide)
{
	elf_hide_mouse(hide);
}
ELF_API bool ELF_APIENTRY elfIsMouseHidden()
{
	return (bool)elf_is_mouse_hidden();
}
ELF_API int ELF_APIENTRY elfGetMouseWheel()
{
	return elf_get_mouse_wheel();
}
ELF_API int ELF_APIENTRY elfGetMouseButtonState(int button)
{
	return elf_get_mouse_button_state(button);
}
ELF_API int ELF_APIENTRY elfGetKeyState(int key)
{
	return elf_get_key_state(key);
}
ELF_API bool ELF_APIENTRY elfGetJoystickPresent(int joy)
{
	return (bool)elf_get_joystick_present(joy);
}
ELF_API elf_vec2f ELF_APIENTRY elfGetJoystickAxis(int joy)
{
	elf_vec2f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec2f));
	_e_type = elf_get_joystick_axis(joy);
	return _e_type;
}
ELF_API int ELF_APIENTRY elfGetJoystickButtonState(int joy, int but)
{
	return elf_get_joystick_button_state(joy, but);
}
ELF_API int ELF_APIENTRY elfGetEventCount()
{
	return elf_get_event_count();
}
ELF_API elf_handle ELF_APIENTRY elfGetEvent(int idx)
{
	elf_handle handle;
	handle = (elf_object*)elf_get_event(idx);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfReadGameConfig(const char* file_path)
{
	elf_handle handle;
	handle = (elf_object*)elf_read_game_config(file_path);
	return handle;
}
ELF_API int ELF_APIENTRY elfGetGameConfigWindowWidth(elf_handle config)
{
	if(!config.get() || elf_get_object_type(config.get()) != ELF_GAME_CONFIG)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetGameConfigWindowWidth() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetGameConfigWindowWidth() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_game_config_window_width((elf_game_config*)config.get());
}
ELF_API int ELF_APIENTRY elfGetGameConfigWindowHeight(elf_handle config)
{
	if(!config.get() || elf_get_object_type(config.get()) != ELF_GAME_CONFIG)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetGameConfigWindowHeight() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetGameConfigWindowHeight() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_game_config_window_height((elf_game_config*)config.get());
}
ELF_API bool ELF_APIENTRY elfGetGameConfigFullscreen(elf_handle config)
{
	if(!config.get() || elf_get_object_type(config.get()) != ELF_GAME_CONFIG)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetGameConfigFullscreen() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetGameConfigFullscreen() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_get_game_config_fullscreen((elf_game_config*)config.get());
}
ELF_API float ELF_APIENTRY elfGetGameConfigTextureAnisotropy(elf_handle config)
{
	if(!config.get() || elf_get_object_type(config.get()) != ELF_GAME_CONFIG)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetGameConfigTextureAnisotropy() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetGameConfigTextureAnisotropy() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_game_config_texture_anisotropy((elf_game_config*)config.get());
}
ELF_API int ELF_APIENTRY elfGetGameConfigShadowMapSize(elf_handle config)
{
	if(!config.get() || elf_get_object_type(config.get()) != ELF_GAME_CONFIG)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetGameConfigShadowMapSize() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetGameConfigShadowMapSize() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_game_config_shadow_map_size((elf_game_config*)config.get());
}
ELF_API const char* ELF_APIENTRY elfGetGameConfigStart(elf_handle config)
{
	if(!config.get() || elf_get_object_type(config.get()) != ELF_GAME_CONFIG)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetGameConfigStart() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetGameConfigStart() -> invalid handle\n");
		}
		return "";
	}
	return elf_get_game_config_start((elf_game_config*)config.get());
}
ELF_API bool ELF_APIENTRY elfInit(int width, int height, const char* title, bool fullscreen)
{
	return (bool)elf_init(width, height, title, fullscreen);
}
ELF_API bool ELF_APIENTRY elfInitWithConfig(const char* file_path)
{
	return (bool)elf_init_with_config(file_path);
}
ELF_API void ELF_APIENTRY elfDeinit()
{
	elf_deinit();
}
ELF_API void ELF_APIENTRY elfResizeWindow(int width, int height)
{
	elf_resize_window(width, height);
}
ELF_API const char* ELF_APIENTRY elfGetCurrentWorkingDirectory()
{
	return elf_get_current_working_directory();
}
ELF_API const char* ELF_APIENTRY elfGetPlatform()
{
	return elf_get_platform();
}
ELF_API int ELF_APIENTRY elfGetVersionMajor()
{
	return elf_get_version_major();
}
ELF_API int ELF_APIENTRY elfGetVersionMinor()
{
	return elf_get_version_minor();
}
ELF_API const char* ELF_APIENTRY elfGetVersionRelease()
{
	return elf_get_version_release();
}
ELF_API const char* ELF_APIENTRY elfGetVersion()
{
	return elf_get_version();
}
ELF_API const char* ELF_APIENTRY elfGetErrorString()
{
	return elf_get_error_string();
}
ELF_API int ELF_APIENTRY elfGetError()
{
	return elf_get_error();
}
ELF_API bool ELF_APIENTRY elfRun()
{
	return (bool)elf_run();
}
ELF_API void ELF_APIENTRY elfQuit()
{
	elf_quit();
}
ELF_API void ELF_APIENTRY elfSetF10Exit(bool exit)
{
	elf_set_f10_exit(exit);
}
ELF_API bool ELF_APIENTRY elfGetF10Exit()
{
	return (bool)elf_get_f10_exit();
}
ELF_API elf_handle ELF_APIENTRY elfLoadScene(const char* file_path)
{
	elf_handle handle;
	handle = (elf_object*)elf_load_scene(file_path);
	return handle;
}
ELF_API void ELF_APIENTRY elfSetScene(elf_handle scene)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetScene() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetScene() -> invalid handle\n");
		}
		return;
	}
	elf_set_scene((elf_scene*)scene.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetScene()
{
	elf_handle handle;
	handle = (elf_object*)elf_get_scene();
	return handle;
}
ELF_API void ELF_APIENTRY elfSetGui(elf_handle gui)
{
	if(!gui.get() || elf_get_object_type(gui.get()) != ELF_GUI)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetGui() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetGui() -> invalid handle\n");
		}
		return;
	}
	elf_set_gui((elf_gui*)gui.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetGui()
{
	elf_handle handle;
	handle = (elf_object*)elf_get_gui();
	return handle;
}
ELF_API float ELF_APIENTRY elfGetSync()
{
	return elf_get_sync();
}
ELF_API int ELF_APIENTRY elfGetFps()
{
	return elf_get_fps();
}
ELF_API bool ELF_APIENTRY elfSaveScreenShot(const char* file_path)
{
	return (bool)elf_save_screen_shot(file_path);
}
ELF_API void ELF_APIENTRY elfSetFpsLimit(int fps_limit)
{
	elf_set_fps_limit(fps_limit);
}
ELF_API int ELF_APIENTRY elfGetFpsLimit()
{
	return elf_get_fps_limit();
}
ELF_API void ELF_APIENTRY elfSetTickRate(float tick_rate)
{
	elf_set_tick_rate(tick_rate);
}
ELF_API float ELF_APIENTRY elfGetTickRate()
{
	return elf_get_tick_rate();
}
ELF_API void ELF_APIENTRY elfSetSpeed(float speed)
{
	elf_set_speed(speed);
}
ELF_API float ELF_APIENTRY elfGetSpeed()
{
	return elf_get_speed();
}
ELF_API void ELF_APIENTRY elfSetTextureAnisotropy(float anisotropy)
{
	elf_set_texture_anisotropy(anisotropy);
}
ELF_API float ELF_APIENTRY elfGetTextureAnisotropy()
{
	return elf_get_texture_anisotropy();
}
ELF_API void ELF_APIENTRY elfSetShadowMapSize(int size)
{
	elf_set_shadow_map_size(size);
}
ELF_API int ELF_APIENTRY elfGetShadowMapSize()
{
	return elf_get_shadow_map_size();
}
ELF_API int ELF_APIENTRY elfGetPolygonsRendered()
{
	return elf_get_polygons_rendered();
}
ELF_API void ELF_APIENTRY elfSetBloom(float threshold)
{
	elf_set_bloom(threshold);
}
ELF_API void ELF_APIENTRY elfDisableBloom()
{
	elf_disable_bloom();
}
ELF_API float ELF_APIENTRY elfGetBloomThreshold()
{
	return elf_get_bloom_threshold();
}
ELF_API void ELF_APIENTRY elfSetDof(float focal_range, float focal_distance)
{
	elf_set_dof(focal_range, focal_distance);
}
ELF_API void ELF_APIENTRY elfDisableDof()
{
	elf_disable_dof();
}
ELF_API float ELF_APIENTRY elfGetDofFocalRange()
{
	return elf_get_dof_focal_range();
}
ELF_API float ELF_APIENTRY elfGetDofFocalDistance()
{
	return elf_get_dof_focal_distance();
}
ELF_API void ELF_APIENTRY elfSetSsao(float amount)
{
	elf_set_ssao(amount);
}
ELF_API void ELF_APIENTRY elfDisableSsao()
{
	elf_disable_ssao();
}
ELF_API float ELF_APIENTRY elfGetSsaoAmount()
{
	return elf_get_ssao_amount();
}
ELF_API void ELF_APIENTRY elfSetLightShafts(float intensity)
{
	elf_set_light_shafts(intensity);
}
ELF_API void ELF_APIENTRY elfDisableLightShafts()
{
	elf_disable_light_shafts();
}
ELF_API float ELF_APIENTRY elfGetLightShaftsInteisity()
{
	return elf_get_light_shafts_inteisity();
}
ELF_API bool ELF_APIENTRY elfIsBloom()
{
	return (bool)elf_is_bloom();
}
ELF_API bool ELF_APIENTRY elfIsSsao()
{
	return (bool)elf_is_ssao();
}
ELF_API bool ELF_APIENTRY elfIsDof()
{
	return (bool)elf_is_dof();
}
ELF_API bool ELF_APIENTRY elfIsLightShafts()
{
	return (bool)elf_is_light_shafts();
}
ELF_API void ELF_APIENTRY elfSetOcclusionCulling(bool cull)
{
	elf_set_occlusion_culling(cull);
}
ELF_API bool ELF_APIENTRY elfIsOcclusionCulling()
{
	return (bool)elf_is_occlusion_culling();
}
ELF_API void ELF_APIENTRY elfSetDebugDraw(bool debug_draw)
{
	elf_set_debug_draw(debug_draw);
}
ELF_API bool ELF_APIENTRY elfIsDebugDraw()
{
	return (bool)elf_is_debug_draw();
}
ELF_API elf_handle ELF_APIENTRY elfGetActor()
{
	elf_handle handle;
	handle = (elf_object*)elf_get_actor();
	return handle;
}
ELF_API elf_vec3f ELF_APIENTRY elfCreateVec3f()
{
	return elf_create_vec3f();
}
ELF_API elf_vec3f ELF_APIENTRY elfCreateVec3fFromValues(float x, float y, float z)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	_e_type = elf_create_vec3f_from_values(x, y, z);
	return _e_type;
}
ELF_API elf_vec4f ELF_APIENTRY elfCreateQua()
{
	return elf_create_qua();
}
ELF_API elf_vec4f ELF_APIENTRY elfCreateQuaFromEuler(float x, float y, float z)
{
	elf_vec4f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec4f));
	_e_type = elf_create_qua_from_euler(x, y, z);
	return _e_type;
}
ELF_API elf_vec3f ELF_APIENTRY elfMulQuaVec3f(elf_vec4f qua, elf_vec3f vec)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	_e_type = elf_mul_qua_vec3f(qua, vec);
	return _e_type;
}
ELF_API elf_vec4f ELF_APIENTRY elfMulQuaQua(elf_vec4f qua1, elf_vec4f qua2)
{
	elf_vec4f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec4f));
	_e_type = elf_mul_qua_qua(qua1, qua2);
	return _e_type;
}
ELF_API elf_vec4f ELF_APIENTRY elfGetQuaInverted(elf_vec4f qua)
{
	elf_vec4f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec4f));
	_e_type = elf_get_qua_inverted(qua);
	return _e_type;
}
ELF_API elf_vec3f ELF_APIENTRY elfSubVec3fVec3f(elf_vec3f vec1, elf_vec3f vec2)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	_e_type = elf_sub_vec3f_vec3f(vec1, vec2);
	return _e_type;
}
ELF_API elf_vec3f ELF_APIENTRY elfAddVec3fVec3f(elf_vec3f vec1, elf_vec3f vec2)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	_e_type = elf_add_vec3f_vec3f(vec1, vec2);
	return _e_type;
}
ELF_API float ELF_APIENTRY elfGetVec3fLength(elf_vec3f vec)
{
	return elf_get_vec3f_length(vec);
}
ELF_API bool ELF_APIENTRY elfAboutZero(float val)
{
	return (bool)elf_about_zero(val);
}
ELF_API float ELF_APIENTRY elfFloatAbs(float val)
{
	return elf_float_abs(val);
}
ELF_API float ELF_APIENTRY elfFloatMax(float a, float b)
{
	return elf_float_max(a, b);
}
ELF_API float ELF_APIENTRY elfRandomFloat()
{
	return elf_random_float();
}
ELF_API float ELF_APIENTRY elfRandomFloatRange(float min, float max)
{
	return elf_random_float_range(min, max);
}
ELF_API int ELF_APIENTRY elfRandomInt()
{
	return elf_random_int();
}
ELF_API int ELF_APIENTRY elfRandomIntRange(int min, int max)
{
	return elf_random_int_range(min, max);
}
ELF_API elf_handle ELF_APIENTRY elfCreateFramePlayer()
{
	elf_handle handle;
	handle = (elf_object*)elf_create_frame_player();
	return handle;
}
ELF_API void ELF_APIENTRY elfUpdateFramePlayer(elf_handle player)
{
	if(!player.get() || elf_get_object_type(player.get()) != ELF_FRAME_PLAYER)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: UpdateFramePlayer() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "UpdateFramePlayer() -> invalid handle\n");
		}
		return;
	}
	elf_update_frame_player((elf_frame_player*)player.get());
}
ELF_API void ELF_APIENTRY elfSetFramePlayerFrame(elf_handle player, float frame)
{
	if(!player.get() || elf_get_object_type(player.get()) != ELF_FRAME_PLAYER)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetFramePlayerFrame() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetFramePlayerFrame() -> invalid handle\n");
		}
		return;
	}
	elf_set_frame_player_frame((elf_frame_player*)player.get(), frame);
}
ELF_API void ELF_APIENTRY elfPlayFramePlayer(elf_handle player, float start, float end, float speed)
{
	if(!player.get() || elf_get_object_type(player.get()) != ELF_FRAME_PLAYER)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: PlayFramePlayer() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "PlayFramePlayer() -> invalid handle\n");
		}
		return;
	}
	elf_play_frame_player((elf_frame_player*)player.get(), start, end, speed);
}
ELF_API void ELF_APIENTRY elfLoopFramePlayer(elf_handle player, float start, float end, float speed)
{
	if(!player.get() || elf_get_object_type(player.get()) != ELF_FRAME_PLAYER)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: LoopFramePlayer() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "LoopFramePlayer() -> invalid handle\n");
		}
		return;
	}
	elf_loop_frame_player((elf_frame_player*)player.get(), start, end, speed);
}
ELF_API void ELF_APIENTRY elfStopFramePlayer(elf_handle player)
{
	if(!player.get() || elf_get_object_type(player.get()) != ELF_FRAME_PLAYER)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: StopFramePlayer() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "StopFramePlayer() -> invalid handle\n");
		}
		return;
	}
	elf_stop_frame_player((elf_frame_player*)player.get());
}
ELF_API void ELF_APIENTRY elfPauseFramePlayer(elf_handle player)
{
	if(!player.get() || elf_get_object_type(player.get()) != ELF_FRAME_PLAYER)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: PauseFramePlayer() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "PauseFramePlayer() -> invalid handle\n");
		}
		return;
	}
	elf_pause_frame_player((elf_frame_player*)player.get());
}
ELF_API void ELF_APIENTRY elfResumeFramePlayer(elf_handle player)
{
	if(!player.get() || elf_get_object_type(player.get()) != ELF_FRAME_PLAYER)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: ResumeFramePlayer() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "ResumeFramePlayer() -> invalid handle\n");
		}
		return;
	}
	elf_resume_frame_player((elf_frame_player*)player.get());
}
ELF_API float ELF_APIENTRY elfGetFramePlayerStart(elf_handle player)
{
	if(!player.get() || elf_get_object_type(player.get()) != ELF_FRAME_PLAYER)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetFramePlayerStart() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetFramePlayerStart() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_frame_player_start((elf_frame_player*)player.get());
}
ELF_API float ELF_APIENTRY elfGetFramePlayerEnd(elf_handle player)
{
	if(!player.get() || elf_get_object_type(player.get()) != ELF_FRAME_PLAYER)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetFramePlayerEnd() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetFramePlayerEnd() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_frame_player_end((elf_frame_player*)player.get());
}
ELF_API float ELF_APIENTRY elfGetFramePlayerSpeed(elf_handle player)
{
	if(!player.get() || elf_get_object_type(player.get()) != ELF_FRAME_PLAYER)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetFramePlayerSpeed() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetFramePlayerSpeed() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_frame_player_speed((elf_frame_player*)player.get());
}
ELF_API float ELF_APIENTRY elfGetFramePlayerFrame(elf_handle player)
{
	if(!player.get() || elf_get_object_type(player.get()) != ELF_FRAME_PLAYER)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetFramePlayerFrame() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetFramePlayerFrame() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_frame_player_frame((elf_frame_player*)player.get());
}
ELF_API bool ELF_APIENTRY elfIsFramePlayerPlaying(elf_handle player)
{
	if(!player.get() || elf_get_object_type(player.get()) != ELF_FRAME_PLAYER)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: IsFramePlayerPlaying() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "IsFramePlayerPlaying() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_is_frame_player_playing((elf_frame_player*)player.get());
}
ELF_API bool ELF_APIENTRY elfIsFramePlayerPaused(elf_handle player)
{
	if(!player.get() || elf_get_object_type(player.get()) != ELF_FRAME_PLAYER)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: IsFramePlayerPaused() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "IsFramePlayerPaused() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_is_frame_player_paused((elf_frame_player*)player.get());
}
ELF_API elf_handle ELF_APIENTRY elfCreateTimer()
{
	elf_handle handle;
	handle = (elf_object*)elf_create_timer();
	return handle;
}
ELF_API void ELF_APIENTRY elfStartTimer(elf_handle timer)
{
	if(!timer.get() || elf_get_object_type(timer.get()) != ELF_TIMER)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: StartTimer() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "StartTimer() -> invalid handle\n");
		}
		return;
	}
	elf_start_timer((elf_timer*)timer.get());
}
ELF_API double ELF_APIENTRY elfGetElapsedTime(elf_handle timer)
{
	if(!timer.get() || elf_get_object_type(timer.get()) != ELF_TIMER)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetElapsedTime() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetElapsedTime() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_elapsed_time((elf_timer*)timer.get());
}
ELF_API elf_handle ELF_APIENTRY elfCreateTextureFromFile(const char* file_path)
{
	elf_handle handle;
	handle = (elf_object*)elf_create_texture_from_file(file_path);
	return handle;
}
ELF_API const char* ELF_APIENTRY elfGetTextureName(elf_handle texture)
{
	if(!texture.get() || elf_get_object_type(texture.get()) != ELF_TEXTURE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetTextureName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetTextureName() -> invalid handle\n");
		}
		return "";
	}
	return elf_get_texture_name((elf_texture*)texture.get());
}
ELF_API const char* ELF_APIENTRY elfGetTextureFilePath(elf_handle texture)
{
	if(!texture.get() || elf_get_object_type(texture.get()) != ELF_TEXTURE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetTextureFilePath() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetTextureFilePath() -> invalid handle\n");
		}
		return "";
	}
	return elf_get_texture_file_path((elf_texture*)texture.get());
}
ELF_API int ELF_APIENTRY elfGetTextureWidth(elf_handle texture)
{
	if(!texture.get() || elf_get_object_type(texture.get()) != ELF_TEXTURE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetTextureWidth() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetTextureWidth() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_texture_width((elf_texture*)texture.get());
}
ELF_API int ELF_APIENTRY elfGetTextureHeight(elf_handle texture)
{
	if(!texture.get() || elf_get_object_type(texture.get()) != ELF_TEXTURE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetTextureHeight() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetTextureHeight() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_texture_height((elf_texture*)texture.get());
}
ELF_API int ELF_APIENTRY elfGetTextureFormat(elf_handle texture)
{
	if(!texture.get() || elf_get_object_type(texture.get()) != ELF_TEXTURE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetTextureFormat() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetTextureFormat() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_texture_format((elf_texture*)texture.get());
}
ELF_API int ELF_APIENTRY elfGetTextureDataFormat(elf_handle texture)
{
	if(!texture.get() || elf_get_object_type(texture.get()) != ELF_TEXTURE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetTextureDataFormat() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetTextureDataFormat() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_texture_data_format((elf_texture*)texture.get());
}
ELF_API elf_handle ELF_APIENTRY elfCreateMaterial(const char* name)
{
	elf_handle handle;
	handle = (elf_object*)elf_create_material(name);
	return handle;
}
ELF_API void ELF_APIENTRY elfSetMaterialTexture(elf_handle material, int slot, elf_handle texture)
{
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetMaterialTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetMaterialTexture() -> invalid handle\n");
		}
		return;
	}
	if(!texture.get() || elf_get_object_type(texture.get()) != ELF_TEXTURE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetMaterialTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetMaterialTexture() -> invalid handle\n");
		}
		return;
	}
	elf_set_material_texture((elf_material*)material.get(), slot, (elf_texture*)texture.get());
}
ELF_API void ELF_APIENTRY elfSetMaterialTextureType(elf_handle material, int slot, int type)
{
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetMaterialTextureType() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetMaterialTextureType() -> invalid handle\n");
		}
		return;
	}
	elf_set_material_texture_type((elf_material*)material.get(), slot, type);
}
ELF_API void ELF_APIENTRY elfSetMaterialTextureParallaxScale(elf_handle material, int slot, float scale)
{
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetMaterialTextureParallaxScale() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetMaterialTextureParallaxScale() -> invalid handle\n");
		}
		return;
	}
	elf_set_material_texture_parallax_scale((elf_material*)material.get(), slot, scale);
}
ELF_API void ELF_APIENTRY elfSetMaterialTextureAlphaTest(elf_handle material, int slot, float test)
{
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetMaterialTextureAlphaTest() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetMaterialTextureAlphaTest() -> invalid handle\n");
		}
		return;
	}
	elf_set_material_texture_alpha_test((elf_material*)material.get(), slot, test);
}
ELF_API void ELF_APIENTRY elfSetMaterialDiffuseColor(elf_handle material, float r, float g, float b, float a)
{
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetMaterialDiffuseColor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetMaterialDiffuseColor() -> invalid handle\n");
		}
		return;
	}
	elf_set_material_diffuse_color((elf_material*)material.get(), r, g, b, a);
}
ELF_API void ELF_APIENTRY elfSetMaterialSpecularColor(elf_handle material, float r, float g, float b, float a)
{
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetMaterialSpecularColor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetMaterialSpecularColor() -> invalid handle\n");
		}
		return;
	}
	elf_set_material_specular_color((elf_material*)material.get(), r, g, b, a);
}
ELF_API void ELF_APIENTRY elfSetMaterialAmbientColor(elf_handle material, float r, float g, float b, float a)
{
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetMaterialAmbientColor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetMaterialAmbientColor() -> invalid handle\n");
		}
		return;
	}
	elf_set_material_ambient_color((elf_material*)material.get(), r, g, b, a);
}
ELF_API void ELF_APIENTRY elfSetMaterialSpecularPower(elf_handle material, float power)
{
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetMaterialSpecularPower() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetMaterialSpecularPower() -> invalid handle\n");
		}
		return;
	}
	elf_set_material_specular_power((elf_material*)material.get(), power);
}
ELF_API void ELF_APIENTRY elfSetMaterialLighting(elf_handle material, bool lighting)
{
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetMaterialLighting() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetMaterialLighting() -> invalid handle\n");
		}
		return;
	}
	elf_set_material_lighting((elf_material*)material.get(), lighting);
}
ELF_API const char* ELF_APIENTRY elfGetMaterialName(elf_handle material)
{
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetMaterialName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetMaterialName() -> invalid handle\n");
		}
		return "";
	}
	return elf_get_material_name((elf_material*)material.get());
}
ELF_API const char* ELF_APIENTRY elfGetMaterialFilePath(elf_handle material)
{
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetMaterialFilePath() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetMaterialFilePath() -> invalid handle\n");
		}
		return "";
	}
	return elf_get_material_file_path((elf_material*)material.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetMaterialTexture(elf_handle material, int slot)
{
	elf_handle handle;
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetMaterialTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetMaterialTexture() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_material_texture((elf_material*)material.get(), slot);
	return handle;
}
ELF_API int ELF_APIENTRY elfGetMaterialTextureType(elf_handle material, int slot)
{
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetMaterialTextureType() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetMaterialTextureType() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_material_texture_type((elf_material*)material.get(), slot);
}
ELF_API float ELF_APIENTRY elfGetMaterialTextureParallaxScale(elf_handle material, int slot)
{
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetMaterialTextureParallaxScale() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetMaterialTextureParallaxScale() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_material_texture_parallax_scale((elf_material*)material.get(), slot);
}
ELF_API float ELF_APIENTRY elfGetMaterialTextureAlphaTexture(elf_handle material, int slot)
{
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetMaterialTextureAlphaTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetMaterialTextureAlphaTexture() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_material_texture_alpha_texture((elf_material*)material.get(), slot);
}
ELF_API elf_color ELF_APIENTRY elfGetMaterialDiffuseColor(elf_handle material)
{
	elf_color _e_type;
	memset(&_e_type, 0x0, sizeof(elf_color));
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetMaterialDiffuseColor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetMaterialDiffuseColor() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_material_diffuse_color((elf_material*)material.get());
	return _e_type;
}
ELF_API elf_color ELF_APIENTRY elfGetMaterialSpecularColor(elf_handle material)
{
	elf_color _e_type;
	memset(&_e_type, 0x0, sizeof(elf_color));
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetMaterialSpecularColor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetMaterialSpecularColor() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_material_specular_color((elf_material*)material.get());
	return _e_type;
}
ELF_API elf_color ELF_APIENTRY elfGetMaterialAmbientColor(elf_handle material)
{
	elf_color _e_type;
	memset(&_e_type, 0x0, sizeof(elf_color));
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetMaterialAmbientColor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetMaterialAmbientColor() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_material_ambient_color((elf_material*)material.get());
	return _e_type;
}
ELF_API float ELF_APIENTRY elfGetMaterialSpecularPower(elf_handle material)
{
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetMaterialSpecularPower() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetMaterialSpecularPower() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_material_specular_power((elf_material*)material.get());
}
ELF_API bool ELF_APIENTRY elfGetMaterialLighting(elf_handle material)
{
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetMaterialLighting() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetMaterialLighting() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_get_material_lighting((elf_material*)material.get());
}
ELF_API void ELF_APIENTRY elfAddPointToBezierCurve(elf_handle curve, elf_handle point)
{
	if(!curve.get() || elf_get_object_type(curve.get()) != ELF_BEZIER_CURVE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: AddPointToBezierCurve() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "AddPointToBezierCurve() -> invalid handle\n");
		}
		return;
	}
	if(!point.get() || elf_get_object_type(point.get()) != ELF_BEZIER_POINT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: AddPointToBezierCurve() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "AddPointToBezierCurve() -> invalid handle\n");
		}
		return;
	}
	elf_add_point_to_bezier_curve((elf_bezier_curve*)curve.get(), (elf_bezier_point*)point.get());
}
ELF_API bool ELF_APIENTRY elfAddCurveToIpo(elf_handle ipo, elf_handle curve)
{
	if(!ipo.get() || elf_get_object_type(ipo.get()) != ELF_IPO)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: AddCurveToIpo() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "AddCurveToIpo() -> invalid handle\n");
		}
		return false;
	}
	if(!curve.get() || elf_get_object_type(curve.get()) != ELF_BEZIER_CURVE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: AddCurveToIpo() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "AddCurveToIpo() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_add_curve_to_ipo((elf_ipo*)ipo.get(), (elf_bezier_curve*)curve.get());
}
ELF_API elf_vec3f ELF_APIENTRY elfGetIpoLoc(elf_handle ipo, float x)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	if(!ipo.get() || elf_get_object_type(ipo.get()) != ELF_IPO)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetIpoLoc() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetIpoLoc() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_ipo_loc((elf_ipo*)ipo.get(), x);
	return _e_type;
}
ELF_API elf_vec3f ELF_APIENTRY elfGetIpoRot(elf_handle ipo, float x)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	if(!ipo.get() || elf_get_object_type(ipo.get()) != ELF_IPO)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetIpoRot() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetIpoRot() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_ipo_rot((elf_ipo*)ipo.get(), x);
	return _e_type;
}
ELF_API elf_vec3f ELF_APIENTRY elfGetIpoScale(elf_handle ipo, float x)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	if(!ipo.get() || elf_get_object_type(ipo.get()) != ELF_IPO)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetIpoScale() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetIpoScale() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_ipo_scale((elf_ipo*)ipo.get(), x);
	return _e_type;
}
ELF_API elf_vec4f ELF_APIENTRY elfGetIpoQua(elf_handle ipo, float x)
{
	elf_vec4f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec4f));
	if(!ipo.get() || elf_get_object_type(ipo.get()) != ELF_IPO)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetIpoQua() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetIpoQua() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_ipo_qua((elf_ipo*)ipo.get(), x);
	return _e_type;
}
ELF_API elf_handle ELF_APIENTRY elfCreateProperty(const char* name)
{
	elf_handle handle;
	handle = (elf_object*)elf_create_property(name);
	return handle;
}
ELF_API int ELF_APIENTRY elfGetPropertyType(elf_handle property)
{
	if(!property.get() || elf_get_object_type(property.get()) != ELF_PROPERTY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetPropertyType() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetPropertyType() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_property_type((elf_property*)property.get());
}
ELF_API int ELF_APIENTRY elfGetPropertyInt(elf_handle property)
{
	if(!property.get() || elf_get_object_type(property.get()) != ELF_PROPERTY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetPropertyInt() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetPropertyInt() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_property_int((elf_property*)property.get());
}
ELF_API float ELF_APIENTRY elfGetPropertyFloat(elf_handle property)
{
	if(!property.get() || elf_get_object_type(property.get()) != ELF_PROPERTY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetPropertyFloat() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetPropertyFloat() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_property_float((elf_property*)property.get());
}
ELF_API const char* ELF_APIENTRY elfGetPropertyString(elf_handle property)
{
	if(!property.get() || elf_get_object_type(property.get()) != ELF_PROPERTY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetPropertyString() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetPropertyString() -> invalid handle\n");
		}
		return "";
	}
	return elf_get_property_string((elf_property*)property.get());
}
ELF_API bool ELF_APIENTRY elfGetPropertyBool(elf_handle property)
{
	if(!property.get() || elf_get_object_type(property.get()) != ELF_PROPERTY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetPropertyBool() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetPropertyBool() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_get_property_bool((elf_property*)property.get());
}
ELF_API void ELF_APIENTRY elfSetPropertyInt(elf_handle property, int ival)
{
	if(!property.get() || elf_get_object_type(property.get()) != ELF_PROPERTY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetPropertyInt() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetPropertyInt() -> invalid handle\n");
		}
		return;
	}
	elf_set_property_int((elf_property*)property.get(), ival);
}
ELF_API void ELF_APIENTRY elfSetPropertyFloat(elf_handle property, float fval)
{
	if(!property.get() || elf_get_object_type(property.get()) != ELF_PROPERTY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetPropertyFloat() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetPropertyFloat() -> invalid handle\n");
		}
		return;
	}
	elf_set_property_float((elf_property*)property.get(), fval);
}
ELF_API void ELF_APIENTRY elfSetPropertyString(elf_handle property, const char* sval)
{
	if(!property.get() || elf_get_object_type(property.get()) != ELF_PROPERTY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetPropertyString() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetPropertyString() -> invalid handle\n");
		}
		return;
	}
	elf_set_property_string((elf_property*)property.get(), sval);
}
ELF_API void ELF_APIENTRY elfSetPropertyBool(elf_handle property, bool bval)
{
	if(!property.get() || elf_get_object_type(property.get()) != ELF_PROPERTY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetPropertyBool() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetPropertyBool() -> invalid handle\n");
		}
		return;
	}
	elf_set_property_bool((elf_property*)property.get(), bval);
}
ELF_API const char* ELF_APIENTRY elfGetActorName(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorName() -> invalid handle\n");
		}
		return "";
	}
	return elf_get_actor_name((elf_actor*)actor.get());
}
ELF_API const char* ELF_APIENTRY elfGetActorFilePath(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorFilePath() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorFilePath() -> invalid handle\n");
		}
		return "";
	}
	return elf_get_actor_file_path((elf_actor*)actor.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetActorScript(elf_handle actor)
{
	elf_handle handle;
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorScript() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorScript() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_actor_script((elf_actor*)actor.get());
	return handle;
}
ELF_API void ELF_APIENTRY elfSetActorScript(elf_handle actor, elf_handle script)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetActorScript() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetActorScript() -> invalid handle\n");
		}
		return;
	}
	if(!script.get() || elf_get_object_type(script.get()) != ELF_SCRIPT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetActorScript() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetActorScript() -> invalid handle\n");
		}
		return;
	}
	elf_set_actor_script((elf_actor*)actor.get(), (elf_script*)script.get());
}
ELF_API void ELF_APIENTRY elfSetActorPosition(elf_handle actor, float x, float y, float z)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetActorPosition() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetActorPosition() -> invalid handle\n");
		}
		return;
	}
	elf_set_actor_position((elf_actor*)actor.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfSetActorRotation(elf_handle actor, float x, float y, float z)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetActorRotation() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetActorRotation() -> invalid handle\n");
		}
		return;
	}
	elf_set_actor_rotation((elf_actor*)actor.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfSetActorOrientation(elf_handle actor, float x, float y, float z, float w)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetActorOrientation() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetActorOrientation() -> invalid handle\n");
		}
		return;
	}
	elf_set_actor_orientation((elf_actor*)actor.get(), x, y, z, w);
}
ELF_API void ELF_APIENTRY elfRotateActor(elf_handle actor, float x, float y, float z)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RotateActor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RotateActor() -> invalid handle\n");
		}
		return;
	}
	elf_rotate_actor((elf_actor*)actor.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfRotateActorLocal(elf_handle actor, float x, float y, float z)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RotateActorLocal() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RotateActorLocal() -> invalid handle\n");
		}
		return;
	}
	elf_rotate_actor_local((elf_actor*)actor.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfMoveActor(elf_handle actor, float x, float y, float z)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: MoveActor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "MoveActor() -> invalid handle\n");
		}
		return;
	}
	elf_move_actor((elf_actor*)actor.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfMoveActorLocal(elf_handle actor, float x, float y, float z)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: MoveActorLocal() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "MoveActorLocal() -> invalid handle\n");
		}
		return;
	}
	elf_move_actor_local((elf_actor*)actor.get(), x, y, z);
}
ELF_API elf_vec3f ELF_APIENTRY elfGetActorPosition(elf_handle actor)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorPosition() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorPosition() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_actor_position((elf_actor*)actor.get());
	return _e_type;
}
ELF_API elf_vec3f ELF_APIENTRY elfGetActorRotation(elf_handle actor)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorRotation() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorRotation() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_actor_rotation((elf_actor*)actor.get());
	return _e_type;
}
ELF_API elf_vec4f ELF_APIENTRY elfGetActorOrientation(elf_handle actor)
{
	elf_vec4f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec4f));
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorOrientation() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorOrientation() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_actor_orientation((elf_actor*)actor.get());
	return _e_type;
}
ELF_API void ELF_APIENTRY elfSetActorBoundingLengths(elf_handle actor, float x, float y, float z)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetActorBoundingLengths() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetActorBoundingLengths() -> invalid handle\n");
		}
		return;
	}
	elf_set_actor_bounding_lengths((elf_actor*)actor.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfSetActorBoundingOffset(elf_handle actor, float x, float y, float z)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetActorBoundingOffset() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetActorBoundingOffset() -> invalid handle\n");
		}
		return;
	}
	elf_set_actor_bounding_offset((elf_actor*)actor.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfSetActorPhysics(elf_handle actor, int shape, float mass)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetActorPhysics() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetActorPhysics() -> invalid handle\n");
		}
		return;
	}
	elf_set_actor_physics((elf_actor*)actor.get(), shape, mass);
}
ELF_API void ELF_APIENTRY elfDisableActorPhysics(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: DisableActorPhysics() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "DisableActorPhysics() -> invalid handle\n");
		}
		return;
	}
	elf_disable_actor_physics((elf_actor*)actor.get());
}
ELF_API void ELF_APIENTRY elfSetActorAnisotropicFriction(elf_handle actor, float x, float y, float z)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetActorAnisotropicFriction() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetActorAnisotropicFriction() -> invalid handle\n");
		}
		return;
	}
	elf_set_actor_anisotropic_friction((elf_actor*)actor.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfSetActorDamping(elf_handle actor, float lin_damp, float ang_damp)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetActorDamping() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetActorDamping() -> invalid handle\n");
		}
		return;
	}
	elf_set_actor_damping((elf_actor*)actor.get(), lin_damp, ang_damp);
}
ELF_API void ELF_APIENTRY elfSetActorSleepThresholds(elf_handle actor, float lin_thrs, float ang_thrs)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetActorSleepThresholds() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetActorSleepThresholds() -> invalid handle\n");
		}
		return;
	}
	elf_set_actor_sleep_thresholds((elf_actor*)actor.get(), lin_thrs, ang_thrs);
}
ELF_API void ELF_APIENTRY elfSetActorRestitution(elf_handle actor, float restitution)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetActorRestitution() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetActorRestitution() -> invalid handle\n");
		}
		return;
	}
	elf_set_actor_restitution((elf_actor*)actor.get(), restitution);
}
ELF_API void ELF_APIENTRY elfAddForceToActor(elf_handle actor, float x, float y, float z)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: AddForceToActor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "AddForceToActor() -> invalid handle\n");
		}
		return;
	}
	elf_add_force_to_actor((elf_actor*)actor.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfAddTorqueToActor(elf_handle actor, float x, float y, float z)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: AddTorqueToActor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "AddTorqueToActor() -> invalid handle\n");
		}
		return;
	}
	elf_add_torque_to_actor((elf_actor*)actor.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfSetActorLinearVelocity(elf_handle actor, float x, float y, float z)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetActorLinearVelocity() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetActorLinearVelocity() -> invalid handle\n");
		}
		return;
	}
	elf_set_actor_linear_velocity((elf_actor*)actor.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfSetActorAngularVelocity(elf_handle actor, float x, float y, float z)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetActorAngularVelocity() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetActorAngularVelocity() -> invalid handle\n");
		}
		return;
	}
	elf_set_actor_angular_velocity((elf_actor*)actor.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfSetActorLinearFactor(elf_handle actor, float x, float y, float z)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetActorLinearFactor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetActorLinearFactor() -> invalid handle\n");
		}
		return;
	}
	elf_set_actor_linear_factor((elf_actor*)actor.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfSetActorAngularFactor(elf_handle actor, float x, float y, float z)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetActorAngularFactor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetActorAngularFactor() -> invalid handle\n");
		}
		return;
	}
	elf_set_actor_angular_factor((elf_actor*)actor.get(), x, y, z);
}
ELF_API elf_vec3f ELF_APIENTRY elfGetActorBoundingLengths(elf_handle actor)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorBoundingLengths() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorBoundingLengths() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_actor_bounding_lengths((elf_actor*)actor.get());
	return _e_type;
}
ELF_API elf_vec3f ELF_APIENTRY elfGetActorBoundingOffset(elf_handle actor)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorBoundingOffset() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorBoundingOffset() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_actor_bounding_offset((elf_actor*)actor.get());
	return _e_type;
}
ELF_API int ELF_APIENTRY elfGetActorPhysicsShape(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorPhysicsShape() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorPhysicsShape() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_actor_physics_shape((elf_actor*)actor.get());
}
ELF_API float ELF_APIENTRY elfGetActorPhysicsMass(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorPhysicsMass() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorPhysicsMass() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_actor_physics_mass((elf_actor*)actor.get());
}
ELF_API elf_vec3f ELF_APIENTRY elfGetActorLinearVelocity(elf_handle actor)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorLinearVelocity() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorLinearVelocity() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_actor_linear_velocity((elf_actor*)actor.get());
	return _e_type;
}
ELF_API elf_vec3f ELF_APIENTRY elfGetActorAngularVelocity(elf_handle actor)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorAngularVelocity() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorAngularVelocity() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_actor_angular_velocity((elf_actor*)actor.get());
	return _e_type;
}
ELF_API elf_vec3f ELF_APIENTRY elfGetActorLinearFactor(elf_handle actor)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorLinearFactor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorLinearFactor() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_actor_linear_factor((elf_actor*)actor.get());
	return _e_type;
}
ELF_API elf_vec3f ELF_APIENTRY elfGetActorAngularFactor(elf_handle actor)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorAngularFactor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorAngularFactor() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_actor_angular_factor((elf_actor*)actor.get());
	return _e_type;
}
ELF_API elf_vec3f ELF_APIENTRY elfGetActorAnisotropicFriction(elf_handle actor)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorAnisotropicFriction() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorAnisotropicFriction() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_actor_anisotropic_friction((elf_actor*)actor.get());
	return _e_type;
}
ELF_API float ELF_APIENTRY elfGetActorLinearDamping(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorLinearDamping() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorLinearDamping() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_actor_linear_damping((elf_actor*)actor.get());
}
ELF_API float ELF_APIENTRY elfGetActorAngularDamping(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorAngularDamping() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorAngularDamping() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_actor_angular_damping((elf_actor*)actor.get());
}
ELF_API float ELF_APIENTRY elfGetActorLinearSleepThreshold(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorLinearSleepThreshold() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorLinearSleepThreshold() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_actor_linear_sleep_threshold((elf_actor*)actor.get());
}
ELF_API float ELF_APIENTRY elfGetActorAngularSleepThreshold(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorAngularSleepThreshold() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorAngularSleepThreshold() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_actor_angular_sleep_threshold((elf_actor*)actor.get());
}
ELF_API float ELF_APIENTRY elfGetActorRestitution(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorRestitution() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorRestitution() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_actor_restitution((elf_actor*)actor.get());
}
ELF_API elf_handle ELF_APIENTRY elfAddHingeJointToActor(elf_handle actor, elf_handle actor2, const char* name, float px, float py, float pz, float ax, float ay, float az)
{
	elf_handle handle;
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: AddHingeJointToActor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "AddHingeJointToActor() -> invalid handle\n");
		}
		return handle;
	}
	if(!actor2.get() || !elf_is_actor(actor2.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: AddHingeJointToActor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "AddHingeJointToActor() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_add_hinge_joint_to_actor((elf_actor*)actor.get(), (elf_actor*)actor2.get(), name, px, py, pz, ax, ay, az);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfAddBallJointToActor(elf_handle actor, elf_handle actor2, const char* name, float px, float py, float pz)
{
	elf_handle handle;
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: AddBallJointToActor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "AddBallJointToActor() -> invalid handle\n");
		}
		return handle;
	}
	if(!actor2.get() || !elf_is_actor(actor2.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: AddBallJointToActor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "AddBallJointToActor() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_add_ball_joint_to_actor((elf_actor*)actor.get(), (elf_actor*)actor2.get(), name, px, py, pz);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfAddConeTwistJointToActor(elf_handle actor, elf_handle actor2, const char* name, float px, float py, float pz, float ax, float ay, float az)
{
	elf_handle handle;
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: AddConeTwistJointToActor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "AddConeTwistJointToActor() -> invalid handle\n");
		}
		return handle;
	}
	if(!actor2.get() || !elf_is_actor(actor2.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: AddConeTwistJointToActor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "AddConeTwistJointToActor() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_add_cone_twist_joint_to_actor((elf_actor*)actor.get(), (elf_actor*)actor2.get(), name, px, py, pz, ax, ay, az);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetActorJointByName(elf_handle actor, const char* name)
{
	elf_handle handle;
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorJointByName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorJointByName() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_actor_joint_by_name((elf_actor*)actor.get(), name);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetActorJointByIndex(elf_handle actor, int idx)
{
	elf_handle handle;
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorJointByIndex() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorJointByIndex() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_actor_joint_by_index((elf_actor*)actor.get(), idx);
	return handle;
}
ELF_API bool ELF_APIENTRY elfRemoveActorJointByName(elf_handle actor, const char* name)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveActorJointByName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveActorJointByName() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_remove_actor_joint_by_name((elf_actor*)actor.get(), name);
}
ELF_API bool ELF_APIENTRY elfRemoveActorJointByIndex(elf_handle actor, int idx)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveActorJointByIndex() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveActorJointByIndex() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_remove_actor_joint_by_index((elf_actor*)actor.get(), idx);
}
ELF_API bool ELF_APIENTRY elfRemoveActorJointByObject(elf_handle actor, elf_handle joint)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveActorJointByObject() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveActorJointByObject() -> invalid handle\n");
		}
		return false;
	}
	if(!joint.get() || elf_get_object_type(joint.get()) != ELF_JOINT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveActorJointByObject() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveActorJointByObject() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_remove_actor_joint_by_object((elf_actor*)actor.get(), (elf_joint*)joint.get());
}
ELF_API void ELF_APIENTRY elfSetActorIpoFrame(elf_handle actor, float frame)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetActorIpoFrame() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetActorIpoFrame() -> invalid handle\n");
		}
		return;
	}
	elf_set_actor_ipo_frame((elf_actor*)actor.get(), frame);
}
ELF_API void ELF_APIENTRY elfPlayActorIpo(elf_handle actor, float start, float end, float speed)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: PlayActorIpo() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "PlayActorIpo() -> invalid handle\n");
		}
		return;
	}
	elf_play_actor_ipo((elf_actor*)actor.get(), start, end, speed);
}
ELF_API void ELF_APIENTRY elfLoopActorIpo(elf_handle actor, float start, float end, float speed)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: LoopActorIpo() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "LoopActorIpo() -> invalid handle\n");
		}
		return;
	}
	elf_loop_actor_ipo((elf_actor*)actor.get(), start, end, speed);
}
ELF_API void ELF_APIENTRY elfStopActorIpo(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: StopActorIpo() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "StopActorIpo() -> invalid handle\n");
		}
		return;
	}
	elf_stop_actor_ipo((elf_actor*)actor.get());
}
ELF_API void ELF_APIENTRY elfPauseActorIpo(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: PauseActorIpo() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "PauseActorIpo() -> invalid handle\n");
		}
		return;
	}
	elf_pause_actor_ipo((elf_actor*)actor.get());
}
ELF_API void ELF_APIENTRY elfResumeActorIpo(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: ResumeActorIpo() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "ResumeActorIpo() -> invalid handle\n");
		}
		return;
	}
	elf_resume_actor_ipo((elf_actor*)actor.get());
}
ELF_API float ELF_APIENTRY elfGetActorIpoStart(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorIpoStart() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorIpoStart() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_actor_ipo_start((elf_actor*)actor.get());
}
ELF_API float ELF_APIENTRY elfGetActorIpoEnd(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorIpoEnd() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorIpoEnd() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_actor_ipo_end((elf_actor*)actor.get());
}
ELF_API float ELF_APIENTRY elfGetActorIpoSpeed(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorIpoSpeed() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorIpoSpeed() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_actor_ipo_speed((elf_actor*)actor.get());
}
ELF_API float ELF_APIENTRY elfGetActorIpoFrame(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorIpoFrame() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorIpoFrame() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_actor_ipo_frame((elf_actor*)actor.get());
}
ELF_API bool ELF_APIENTRY elfIsActorIpoPlaying(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: IsActorIpoPlaying() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "IsActorIpoPlaying() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_is_actor_ipo_playing((elf_actor*)actor.get());
}
ELF_API bool ELF_APIENTRY elfIsActorIpoPaused(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: IsActorIpoPaused() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "IsActorIpoPaused() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_is_actor_ipo_paused((elf_actor*)actor.get());
}
ELF_API int ELF_APIENTRY elfGetActorCollisionCount(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorCollisionCount() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorCollisionCount() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_actor_collision_count((elf_actor*)actor.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetActorCollision(elf_handle actor, int idx)
{
	elf_handle handle;
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorCollision() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorCollision() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_actor_collision((elf_actor*)actor.get(), idx);
	return handle;
}
ELF_API int ELF_APIENTRY elfGetActorPropertyCount(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorPropertyCount() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorPropertyCount() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_actor_property_count((elf_actor*)actor.get());
}
ELF_API void ELF_APIENTRY elfAddPropertyToActor(elf_handle actor, elf_handle property)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: AddPropertyToActor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "AddPropertyToActor() -> invalid handle\n");
		}
		return;
	}
	if(!property.get() || elf_get_object_type(property.get()) != ELF_PROPERTY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: AddPropertyToActor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "AddPropertyToActor() -> invalid handle\n");
		}
		return;
	}
	elf_add_property_to_actor((elf_actor*)actor.get(), (elf_property*)property.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetActorPropertyByName(elf_handle actor, const char* name)
{
	elf_handle handle;
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorPropertyByName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorPropertyByName() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_actor_property_by_name((elf_actor*)actor.get(), name);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetActorPropertyByIndex(elf_handle actor, int idx)
{
	elf_handle handle;
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorPropertyByIndex() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorPropertyByIndex() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_actor_property_by_index((elf_actor*)actor.get(), idx);
	return handle;
}
ELF_API bool ELF_APIENTRY elfRemoveActorPropertyByName(elf_handle actor, const char* name)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveActorPropertyByName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveActorPropertyByName() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_remove_actor_property_by_name((elf_actor*)actor.get(), name);
}
ELF_API bool ELF_APIENTRY elfRemoveActorPropertyByIndex(elf_handle actor, int idx)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveActorPropertyByIndex() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveActorPropertyByIndex() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_remove_actor_property_by_index((elf_actor*)actor.get(), idx);
}
ELF_API bool ELF_APIENTRY elfRemoveActorPropertyByObject(elf_handle actor, elf_handle property)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveActorPropertyByObject() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveActorPropertyByObject() -> invalid handle\n");
		}
		return false;
	}
	if(!property.get() || elf_get_object_type(property.get()) != ELF_PROPERTY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveActorPropertyByObject() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveActorPropertyByObject() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_remove_actor_property_by_object((elf_actor*)actor.get(), (elf_property*)property.get());
}
ELF_API void ELF_APIENTRY elfRemoveActorProperties(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveActorProperties() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveActorProperties() -> invalid handle\n");
		}
		return;
	}
	elf_remove_actor_properties((elf_actor*)actor.get());
}
ELF_API void ELF_APIENTRY elfSetActorSelected(elf_handle actor, bool selected)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetActorSelected() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetActorSelected() -> invalid handle\n");
		}
		return;
	}
	elf_set_actor_selected((elf_actor*)actor.get(), selected);
}
ELF_API bool ELF_APIENTRY elfGetActorSelected(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorSelected() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorSelected() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_get_actor_selected((elf_actor*)actor.get());
}
ELF_API elf_handle ELF_APIENTRY elfCreateCamera(const char* name)
{
	elf_handle handle;
	handle = (elf_object*)elf_create_camera(name);
	return handle;
}
ELF_API void ELF_APIENTRY elfSetCameraViewport(elf_handle camera, int x, int y, int width, int height)
{
	if(!camera.get() || elf_get_object_type(camera.get()) != ELF_CAMERA)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetCameraViewport() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetCameraViewport() -> invalid handle\n");
		}
		return;
	}
	elf_set_camera_viewport((elf_camera*)camera.get(), x, y, width, height);
}
ELF_API void ELF_APIENTRY elfSetCameraPerspective(elf_handle camera, float fov, float aspect, float clip_near, float clip_far)
{
	if(!camera.get() || elf_get_object_type(camera.get()) != ELF_CAMERA)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetCameraPerspective() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetCameraPerspective() -> invalid handle\n");
		}
		return;
	}
	elf_set_camera_perspective((elf_camera*)camera.get(), fov, aspect, clip_near, clip_far);
}
ELF_API void ELF_APIENTRY elfSetCameraOrthographic(elf_handle camera, int x, int y, int width, int height, float clip_near, float clip_far)
{
	if(!camera.get() || elf_get_object_type(camera.get()) != ELF_CAMERA)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetCameraOrthographic() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetCameraOrthographic() -> invalid handle\n");
		}
		return;
	}
	elf_set_camera_orthographic((elf_camera*)camera.get(), x, y, width, height, clip_near, clip_far);
}
ELF_API elf_vec2i ELF_APIENTRY elfGetCameraViewportSize(elf_handle camera)
{
	elf_vec2i _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec2i));
	if(!camera.get() || elf_get_object_type(camera.get()) != ELF_CAMERA)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetCameraViewportSize() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetCameraViewportSize() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_camera_viewport_size((elf_camera*)camera.get());
	return _e_type;
}
ELF_API elf_vec2i ELF_APIENTRY elfGetCameraViewportOffset(elf_handle camera)
{
	elf_vec2i _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec2i));
	if(!camera.get() || elf_get_object_type(camera.get()) != ELF_CAMERA)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetCameraViewportOffset() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetCameraViewportOffset() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_camera_viewport_offset((elf_camera*)camera.get());
	return _e_type;
}
ELF_API float ELF_APIENTRY elfGetCameraFov(elf_handle camera)
{
	if(!camera.get() || elf_get_object_type(camera.get()) != ELF_CAMERA)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetCameraFov() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetCameraFov() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_camera_fov((elf_camera*)camera.get());
}
ELF_API float ELF_APIENTRY elfGetCameraAspect(elf_handle camera)
{
	if(!camera.get() || elf_get_object_type(camera.get()) != ELF_CAMERA)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetCameraAspect() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetCameraAspect() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_camera_aspect((elf_camera*)camera.get());
}
ELF_API elf_vec2f ELF_APIENTRY elfGetCameraClip(elf_handle camera)
{
	elf_vec2f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec2f));
	if(!camera.get() || elf_get_object_type(camera.get()) != ELF_CAMERA)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetCameraClip() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetCameraClip() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_camera_clip((elf_camera*)camera.get());
	return _e_type;
}
ELF_API elf_vec2f ELF_APIENTRY elfGetCameraFarPlaneSize(elf_handle camera)
{
	elf_vec2f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec2f));
	if(!camera.get() || elf_get_object_type(camera.get()) != ELF_CAMERA)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetCameraFarPlaneSize() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetCameraFarPlaneSize() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_camera_far_plane_size((elf_camera*)camera.get());
	return _e_type;
}
ELF_API elf_vec3f ELF_APIENTRY elfUnProjectCameraPoint(elf_handle camera, float x, float y, float z)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	if(!camera.get() || elf_get_object_type(camera.get()) != ELF_CAMERA)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: UnProjectCameraPoint() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "UnProjectCameraPoint() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_un_project_camera_point((elf_camera*)camera.get(), x, y, z);
	return _e_type;
}
ELF_API const char* ELF_APIENTRY elfGetModelName(elf_handle model)
{
	if(!model.get() || elf_get_object_type(model.get()) != ELF_MODEL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetModelName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetModelName() -> invalid handle\n");
		}
		return "";
	}
	return elf_get_model_name((elf_model*)model.get());
}
ELF_API const char* ELF_APIENTRY elfGetModelFilePath(elf_handle model)
{
	if(!model.get() || elf_get_object_type(model.get()) != ELF_MODEL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetModelFilePath() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetModelFilePath() -> invalid handle\n");
		}
		return "";
	}
	return elf_get_model_file_path((elf_model*)model.get());
}
ELF_API int ELF_APIENTRY elfGetModelVerticeCount(elf_handle model)
{
	if(!model.get() || elf_get_object_type(model.get()) != ELF_MODEL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetModelVerticeCount() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetModelVerticeCount() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_model_vertice_count((elf_model*)model.get());
}
ELF_API int ELF_APIENTRY elfGetModelIndiceCount(elf_handle model)
{
	if(!model.get() || elf_get_object_type(model.get()) != ELF_MODEL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetModelIndiceCount() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetModelIndiceCount() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_model_indice_count((elf_model*)model.get());
}
ELF_API elf_vec3f ELF_APIENTRY elfGetModelBoundingBoxMin(elf_handle model)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	if(!model.get() || elf_get_object_type(model.get()) != ELF_MODEL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetModelBoundingBoxMin() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetModelBoundingBoxMin() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_model_bounding_box_min((elf_model*)model.get());
	return _e_type;
}
ELF_API elf_vec3f ELF_APIENTRY elfGetModelBoundingBoxMax(elf_handle model)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	if(!model.get() || elf_get_object_type(model.get()) != ELF_MODEL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetModelBoundingBoxMax() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetModelBoundingBoxMax() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_model_bounding_box_max((elf_model*)model.get());
	return _e_type;
}
ELF_API elf_handle ELF_APIENTRY elfCreateEntity(const char* name)
{
	elf_handle handle;
	handle = (elf_object*)elf_create_entity(name);
	return handle;
}
ELF_API void ELF_APIENTRY elfSetEntityScale(elf_handle entity, float x, float y, float z)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetEntityScale() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetEntityScale() -> invalid handle\n");
		}
		return;
	}
	elf_set_entity_scale((elf_entity*)entity.get(), x, y, z);
}
ELF_API elf_vec3f ELF_APIENTRY elfGetEntityScale(elf_handle entity)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetEntityScale() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetEntityScale() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_entity_scale((elf_entity*)entity.get());
	return _e_type;
}
ELF_API void ELF_APIENTRY elfSetEntityModel(elf_handle entity, elf_handle model)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetEntityModel() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetEntityModel() -> invalid handle\n");
		}
		return;
	}
	if(!model.get() || elf_get_object_type(model.get()) != ELF_MODEL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetEntityModel() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetEntityModel() -> invalid handle\n");
		}
		return;
	}
	elf_set_entity_model((elf_entity*)entity.get(), (elf_model*)model.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetEntityModel(elf_handle entity)
{
	elf_handle handle;
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetEntityModel() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetEntityModel() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_entity_model((elf_entity*)entity.get());
	return handle;
}
ELF_API int ELF_APIENTRY elfGetEntityMaterialCount(elf_handle entity)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetEntityMaterialCount() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetEntityMaterialCount() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_entity_material_count((elf_entity*)entity.get());
}
ELF_API void ELF_APIENTRY elfAddEntityMaterial(elf_handle entity, elf_handle material)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: AddEntityMaterial() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "AddEntityMaterial() -> invalid handle\n");
		}
		return;
	}
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: AddEntityMaterial() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "AddEntityMaterial() -> invalid handle\n");
		}
		return;
	}
	elf_add_entity_material((elf_entity*)entity.get(), (elf_material*)material.get());
}
ELF_API void ELF_APIENTRY elfSetEntityMaterial(elf_handle entity, int idx, elf_handle material)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetEntityMaterial() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetEntityMaterial() -> invalid handle\n");
		}
		return;
	}
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetEntityMaterial() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetEntityMaterial() -> invalid handle\n");
		}
		return;
	}
	elf_set_entity_material((elf_entity*)entity.get(), idx, (elf_material*)material.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetEntityMaterial(elf_handle entity, int idx)
{
	elf_handle handle;
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetEntityMaterial() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetEntityMaterial() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_entity_material((elf_entity*)entity.get(), idx);
	return handle;
}
ELF_API void ELF_APIENTRY elfSetEntityVisible(elf_handle entity, bool visible)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetEntityVisible() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetEntityVisible() -> invalid handle\n");
		}
		return;
	}
	elf_set_entity_visible((elf_entity*)entity.get(), visible);
}
ELF_API bool ELF_APIENTRY elfGetEntityVisible(elf_handle entity)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetEntityVisible() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetEntityVisible() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_get_entity_visible((elf_entity*)entity.get());
}
ELF_API void ELF_APIENTRY elfSetEntityPhysics(elf_handle entity, int type, float mass)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetEntityPhysics() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetEntityPhysics() -> invalid handle\n");
		}
		return;
	}
	elf_set_entity_physics((elf_entity*)entity.get(), type, mass);
}
ELF_API void ELF_APIENTRY elfDisableEntityPhysics(elf_handle entity)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: DisableEntityPhysics() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "DisableEntityPhysics() -> invalid handle\n");
		}
		return;
	}
	elf_disable_entity_physics((elf_entity*)entity.get());
}
ELF_API void ELF_APIENTRY elfSetEntityArmature(elf_handle entity, elf_handle armature)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetEntityArmature() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetEntityArmature() -> invalid handle\n");
		}
		return;
	}
	if(!armature.get() || elf_get_object_type(armature.get()) != ELF_ARMATURE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetEntityArmature() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetEntityArmature() -> invalid handle\n");
		}
		return;
	}
	elf_set_entity_armature((elf_entity*)entity.get(), (elf_armature*)armature.get());
}
ELF_API void ELF_APIENTRY elfSetEntityArmatureFrame(elf_handle entity, float frame)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetEntityArmatureFrame() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetEntityArmatureFrame() -> invalid handle\n");
		}
		return;
	}
	elf_set_entity_armature_frame((elf_entity*)entity.get(), frame);
}
ELF_API void ELF_APIENTRY elfPlayEntityArmature(elf_handle entity, float start, float end, float speed)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: PlayEntityArmature() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "PlayEntityArmature() -> invalid handle\n");
		}
		return;
	}
	elf_play_entity_armature((elf_entity*)entity.get(), start, end, speed);
}
ELF_API void ELF_APIENTRY elfLoopEntityArmature(elf_handle entity, float start, float end, float speed)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: LoopEntityArmature() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "LoopEntityArmature() -> invalid handle\n");
		}
		return;
	}
	elf_loop_entity_armature((elf_entity*)entity.get(), start, end, speed);
}
ELF_API void ELF_APIENTRY elfStopEntityArmature(elf_handle entity)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: StopEntityArmature() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "StopEntityArmature() -> invalid handle\n");
		}
		return;
	}
	elf_stop_entity_armature((elf_entity*)entity.get());
}
ELF_API void ELF_APIENTRY elfPauseEntityArmature(elf_handle entity)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: PauseEntityArmature() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "PauseEntityArmature() -> invalid handle\n");
		}
		return;
	}
	elf_pause_entity_armature((elf_entity*)entity.get());
}
ELF_API void ELF_APIENTRY elfResumeEntityArmature(elf_handle entity)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: ResumeEntityArmature() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "ResumeEntityArmature() -> invalid handle\n");
		}
		return;
	}
	elf_resume_entity_armature((elf_entity*)entity.get());
}
ELF_API float ELF_APIENTRY elfGetEntityArmatureStart(elf_handle entity)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetEntityArmatureStart() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetEntityArmatureStart() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_entity_armature_start((elf_entity*)entity.get());
}
ELF_API float ELF_APIENTRY elfGetEntityArmatureEnd(elf_handle entity)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetEntityArmatureEnd() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetEntityArmatureEnd() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_entity_armature_end((elf_entity*)entity.get());
}
ELF_API float ELF_APIENTRY elfGetEntityArmatureSpeed(elf_handle entity)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetEntityArmatureSpeed() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetEntityArmatureSpeed() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_entity_armature_speed((elf_entity*)entity.get());
}
ELF_API float ELF_APIENTRY elfGetEntityArmatureFrame(elf_handle entity)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetEntityArmatureFrame() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetEntityArmatureFrame() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_entity_armature_frame((elf_entity*)entity.get());
}
ELF_API bool ELF_APIENTRY elfIsEntityArmaturePlaying(elf_handle entity)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: IsEntityArmaturePlaying() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "IsEntityArmaturePlaying() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_is_entity_armature_playing((elf_entity*)entity.get());
}
ELF_API bool ELF_APIENTRY elfIsEntityArmaturePaused(elf_handle entity)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: IsEntityArmaturePaused() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "IsEntityArmaturePaused() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_is_entity_armature_paused((elf_entity*)entity.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetEntityArmature(elf_handle entity)
{
	elf_handle handle;
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetEntityArmature() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetEntityArmature() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_entity_armature((elf_entity*)entity.get());
	return handle;
}
ELF_API bool ELF_APIENTRY elfGetEntityChanged(elf_handle entity)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetEntityChanged() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetEntityChanged() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_get_entity_changed((elf_entity*)entity.get());
}
ELF_API elf_handle ELF_APIENTRY elfCreateLight(const char* name)
{
	elf_handle handle;
	handle = (elf_object*)elf_create_light(name);
	return handle;
}
ELF_API int ELF_APIENTRY elfGetLightType(elf_handle light)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetLightType() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetLightType() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_light_type((elf_light*)light.get());
}
ELF_API elf_color ELF_APIENTRY elfGetLightColor(elf_handle light)
{
	elf_color _e_type;
	memset(&_e_type, 0x0, sizeof(elf_color));
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetLightColor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetLightColor() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_light_color((elf_light*)light.get());
	return _e_type;
}
ELF_API float ELF_APIENTRY elfGetLightDistance(elf_handle light)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetLightDistance() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetLightDistance() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_light_distance((elf_light*)light.get());
}
ELF_API float ELF_APIENTRY elfGetLightFadeSpeed(elf_handle light)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetLightFadeSpeed() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetLightFadeSpeed() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_light_fade_speed((elf_light*)light.get());
}
ELF_API bool ELF_APIENTRY elfGetLightShadowCaster(elf_handle light)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetLightShadowCaster() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetLightShadowCaster() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_get_light_shadow_caster((elf_light*)light.get());
}
ELF_API bool ELF_APIENTRY elfGetLightVisible(elf_handle light)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetLightVisible() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetLightVisible() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_get_light_visible((elf_light*)light.get());
}
ELF_API elf_vec2f ELF_APIENTRY elfGetLightCone(elf_handle light)
{
	elf_vec2f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec2f));
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetLightCone() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetLightCone() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_light_cone((elf_light*)light.get());
	return _e_type;
}
ELF_API bool ELF_APIENTRY elfIsLightShaft(elf_handle light)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: IsLightShaft() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "IsLightShaft() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_is_light_shaft((elf_light*)light.get());
}
ELF_API float ELF_APIENTRY elfGetLightShaftSize(elf_handle light)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetLightShaftSize() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetLightShaftSize() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_light_shaft_size((elf_light*)light.get());
}
ELF_API float ELF_APIENTRY elfGetLightShaftIntensity(elf_handle light)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetLightShaftIntensity() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetLightShaftIntensity() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_light_shaft_intensity((elf_light*)light.get());
}
ELF_API float ELF_APIENTRY elfGetLightShaftFadeOff(elf_handle light)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetLightShaftFadeOff() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetLightShaftFadeOff() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_light_shaft_fade_off((elf_light*)light.get());
}
ELF_API void ELF_APIENTRY elfSetLightType(elf_handle light, int type)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetLightType() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetLightType() -> invalid handle\n");
		}
		return;
	}
	elf_set_light_type((elf_light*)light.get(), type);
}
ELF_API void ELF_APIENTRY elfSetLightColor(elf_handle light, float r, float g, float b, float a)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetLightColor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetLightColor() -> invalid handle\n");
		}
		return;
	}
	elf_set_light_color((elf_light*)light.get(), r, g, b, a);
}
ELF_API void ELF_APIENTRY elfSetLightDistance(elf_handle light, float distance)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetLightDistance() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetLightDistance() -> invalid handle\n");
		}
		return;
	}
	elf_set_light_distance((elf_light*)light.get(), distance);
}
ELF_API void ELF_APIENTRY elfSetLightFadeSpeed(elf_handle light, float fade_speed)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetLightFadeSpeed() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetLightFadeSpeed() -> invalid handle\n");
		}
		return;
	}
	elf_set_light_fade_speed((elf_light*)light.get(), fade_speed);
}
ELF_API void ELF_APIENTRY elfSetLightShadowCaster(elf_handle light, bool shadow_caster)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetLightShadowCaster() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetLightShadowCaster() -> invalid handle\n");
		}
		return;
	}
	elf_set_light_shadow_caster((elf_light*)light.get(), shadow_caster);
}
ELF_API void ELF_APIENTRY elfSetLightVisible(elf_handle light, bool visible)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetLightVisible() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetLightVisible() -> invalid handle\n");
		}
		return;
	}
	elf_set_light_visible((elf_light*)light.get(), visible);
}
ELF_API void ELF_APIENTRY elfSetLightCone(elf_handle light, float inner_cone, float outer_cone)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetLightCone() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetLightCone() -> invalid handle\n");
		}
		return;
	}
	elf_set_light_cone((elf_light*)light.get(), inner_cone, outer_cone);
}
ELF_API void ELF_APIENTRY elfSetLightShaft(elf_handle light, float size, float intensity, float fade_off)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetLightShaft() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetLightShaft() -> invalid handle\n");
		}
		return;
	}
	elf_set_light_shaft((elf_light*)light.get(), size, intensity, fade_off);
}
ELF_API void ELF_APIENTRY elfDisableLightShaft(elf_handle light)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: DisableLightShaft() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "DisableLightShaft() -> invalid handle\n");
		}
		return;
	}
	elf_disable_light_shaft((elf_light*)light.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetBoneArmature(elf_handle bone)
{
	elf_handle handle;
	if(!bone.get() || elf_get_object_type(bone.get()) != ELF_BONE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetBoneArmature() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetBoneArmature() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_bone_armature((elf_bone*)bone.get());
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetBoneParent(elf_handle bone)
{
	elf_handle handle;
	if(!bone.get() || elf_get_object_type(bone.get()) != ELF_BONE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetBoneParent() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetBoneParent() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_bone_parent((elf_bone*)bone.get());
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetBoneChildByName(elf_handle bone, const char* name)
{
	elf_handle handle;
	if(!bone.get() || elf_get_object_type(bone.get()) != ELF_BONE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetBoneChildByName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetBoneChildByName() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_bone_child_by_name((elf_bone*)bone.get(), name);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetBoneChildById(elf_handle bone, int id)
{
	elf_handle handle;
	if(!bone.get() || elf_get_object_type(bone.get()) != ELF_BONE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetBoneChildById() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetBoneChildById() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_bone_child_by_id((elf_bone*)bone.get(), id);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetBoneChildByIndex(elf_handle bone, int idx)
{
	elf_handle handle;
	if(!bone.get() || elf_get_object_type(bone.get()) != ELF_BONE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetBoneChildByIndex() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetBoneChildByIndex() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_bone_child_by_index((elf_bone*)bone.get(), idx);
	return handle;
}
ELF_API elf_vec3f ELF_APIENTRY elfGetBonePosition(elf_handle bone)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	if(!bone.get() || elf_get_object_type(bone.get()) != ELF_BONE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetBonePosition() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetBonePosition() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_bone_position((elf_bone*)bone.get());
	return _e_type;
}
ELF_API elf_vec3f ELF_APIENTRY elfGetBoneRotation(elf_handle bone)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	if(!bone.get() || elf_get_object_type(bone.get()) != ELF_BONE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetBoneRotation() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetBoneRotation() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_bone_rotation((elf_bone*)bone.get());
	return _e_type;
}
ELF_API elf_vec4f ELF_APIENTRY elfGetBoneOrientation(elf_handle bone)
{
	elf_vec4f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec4f));
	if(!bone.get() || elf_get_object_type(bone.get()) != ELF_BONE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetBoneOrientation() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetBoneOrientation() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_bone_orientation((elf_bone*)bone.get());
	return _e_type;
}
ELF_API elf_handle ELF_APIENTRY elfCreateArmature(const char* name)
{
	elf_handle handle;
	handle = (elf_object*)elf_create_armature(name);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetBoneFromArmatureByName(const char* name, elf_handle armature)
{
	elf_handle handle;
	if(!armature.get() || elf_get_object_type(armature.get()) != ELF_ARMATURE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetBoneFromArmatureByName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetBoneFromArmatureByName() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_bone_from_armature_by_name(name, (elf_armature*)armature.get());
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetBoneFromArmatureById(int id, elf_handle armature)
{
	elf_handle handle;
	if(!armature.get() || elf_get_object_type(armature.get()) != ELF_ARMATURE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetBoneFromArmatureById() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetBoneFromArmatureById() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_bone_from_armature_by_id(id, (elf_armature*)armature.get());
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfCreateParticles(const char* name, int max_count)
{
	elf_handle handle;
	handle = (elf_object*)elf_create_particles(name, max_count);
	return handle;
}
ELF_API const char* ELF_APIENTRY elfGetParticlesName(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetParticlesName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetParticlesName() -> invalid handle\n");
		}
		return "";
	}
	return elf_get_particles_name((elf_particles*)particles.get());
}
ELF_API const char* ELF_APIENTRY elfGetParticlesFilePath(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetParticlesFilePath() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetParticlesFilePath() -> invalid handle\n");
		}
		return "";
	}
	return elf_get_particles_file_path((elf_particles*)particles.get());
}
ELF_API void ELF_APIENTRY elfSetParticlesMaxCount(elf_handle particles, int max_count)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetParticlesMaxCount() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetParticlesMaxCount() -> invalid handle\n");
		}
		return;
	}
	elf_set_particles_max_count((elf_particles*)particles.get(), max_count);
}
ELF_API void ELF_APIENTRY elfSetParticlesDrawMode(elf_handle particles, int mode)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetParticlesDrawMode() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetParticlesDrawMode() -> invalid handle\n");
		}
		return;
	}
	elf_set_particles_draw_mode((elf_particles*)particles.get(), mode);
}
ELF_API void ELF_APIENTRY elfSetParticlesTexture(elf_handle particles, elf_handle texture)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetParticlesTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetParticlesTexture() -> invalid handle\n");
		}
		return;
	}
	if(!texture.get() || elf_get_object_type(texture.get()) != ELF_TEXTURE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetParticlesTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetParticlesTexture() -> invalid handle\n");
		}
		return;
	}
	elf_set_particles_texture((elf_particles*)particles.get(), (elf_texture*)texture.get());
}
ELF_API void ELF_APIENTRY elfSetParticlesModel(elf_handle particles, elf_handle model)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetParticlesModel() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetParticlesModel() -> invalid handle\n");
		}
		return;
	}
	if(!model.get() || elf_get_object_type(model.get()) != ELF_MODEL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetParticlesModel() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetParticlesModel() -> invalid handle\n");
		}
		return;
	}
	elf_set_particles_model((elf_particles*)particles.get(), (elf_model*)model.get());
}
ELF_API void ELF_APIENTRY elfSetParticlesEntity(elf_handle particles, elf_handle entity)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetParticlesEntity() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetParticlesEntity() -> invalid handle\n");
		}
		return;
	}
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetParticlesEntity() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetParticlesEntity() -> invalid handle\n");
		}
		return;
	}
	elf_set_particles_entity((elf_particles*)particles.get(), (elf_entity*)entity.get());
}
ELF_API void ELF_APIENTRY elfSetParticlesGravity(elf_handle particles, float x, float y, float z)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetParticlesGravity() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetParticlesGravity() -> invalid handle\n");
		}
		return;
	}
	elf_set_particles_gravity((elf_particles*)particles.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfSetParticlesSpawnDelay(elf_handle particles, float delay)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetParticlesSpawnDelay() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetParticlesSpawnDelay() -> invalid handle\n");
		}
		return;
	}
	elf_set_particles_spawn_delay((elf_particles*)particles.get(), delay);
}
ELF_API void ELF_APIENTRY elfSetParticlesSpawn(elf_handle particles, bool spawn)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetParticlesSpawn() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetParticlesSpawn() -> invalid handle\n");
		}
		return;
	}
	elf_set_particles_spawn((elf_particles*)particles.get(), spawn);
}
ELF_API void ELF_APIENTRY elfSetParticlesSize(elf_handle particles, float min, float max)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetParticlesSize() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetParticlesSize() -> invalid handle\n");
		}
		return;
	}
	elf_set_particles_size((elf_particles*)particles.get(), min, max);
}
ELF_API void ELF_APIENTRY elfSetParticlesSizeGrowth(elf_handle particles, float min, float max)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetParticlesSizeGrowth() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetParticlesSizeGrowth() -> invalid handle\n");
		}
		return;
	}
	elf_set_particles_size_growth((elf_particles*)particles.get(), min, max);
}
ELF_API void ELF_APIENTRY elfSetParticlesRotation(elf_handle particles, float min, float max)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetParticlesRotation() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetParticlesRotation() -> invalid handle\n");
		}
		return;
	}
	elf_set_particles_rotation((elf_particles*)particles.get(), min, max);
}
ELF_API void ELF_APIENTRY elfSetParticlesRotationGrowth(elf_handle particles, float min, float max)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetParticlesRotationGrowth() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetParticlesRotationGrowth() -> invalid handle\n");
		}
		return;
	}
	elf_set_particles_rotation_growth((elf_particles*)particles.get(), min, max);
}
ELF_API void ELF_APIENTRY elfSetParticlesLifeSpan(elf_handle particles, float min, float max)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetParticlesLifeSpan() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetParticlesLifeSpan() -> invalid handle\n");
		}
		return;
	}
	elf_set_particles_life_span((elf_particles*)particles.get(), min, max);
}
ELF_API void ELF_APIENTRY elfSetParticlesFadeSpeed(elf_handle particles, float min, float max)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetParticlesFadeSpeed() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetParticlesFadeSpeed() -> invalid handle\n");
		}
		return;
	}
	elf_set_particles_fade_speed((elf_particles*)particles.get(), min, max);
}
ELF_API void ELF_APIENTRY elfSetParticlesPositionMin(elf_handle particles, float x, float y, float z)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetParticlesPositionMin() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetParticlesPositionMin() -> invalid handle\n");
		}
		return;
	}
	elf_set_particles_position_min((elf_particles*)particles.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfSetParticlesPositionMax(elf_handle particles, float x, float y, float z)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetParticlesPositionMax() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetParticlesPositionMax() -> invalid handle\n");
		}
		return;
	}
	elf_set_particles_position_max((elf_particles*)particles.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfSetParticlesVelocityMin(elf_handle particles, float x, float y, float z)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetParticlesVelocityMin() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetParticlesVelocityMin() -> invalid handle\n");
		}
		return;
	}
	elf_set_particles_velocity_min((elf_particles*)particles.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfSetParticlesVelocityMax(elf_handle particles, float x, float y, float z)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetParticlesVelocityMax() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetParticlesVelocityMax() -> invalid handle\n");
		}
		return;
	}
	elf_set_particles_velocity_max((elf_particles*)particles.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfSetParticlesColorMin(elf_handle particles, float r, float g, float b, float a)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetParticlesColorMin() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetParticlesColorMin() -> invalid handle\n");
		}
		return;
	}
	elf_set_particles_color_min((elf_particles*)particles.get(), r, g, b, a);
}
ELF_API void ELF_APIENTRY elfSetParticlesColorMax(elf_handle particles, float r, float g, float b, float a)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetParticlesColorMax() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetParticlesColorMax() -> invalid handle\n");
		}
		return;
	}
	elf_set_particles_color_max((elf_particles*)particles.get(), r, g, b, a);
}
ELF_API int ELF_APIENTRY elfGetParticlesMaxCount(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetParticlesMaxCount() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetParticlesMaxCount() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_particles_max_count((elf_particles*)particles.get());
}
ELF_API int ELF_APIENTRY elfGetParticlesCount(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetParticlesCount() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetParticlesCount() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_particles_count((elf_particles*)particles.get());
}
ELF_API int ELF_APIENTRY elfGetParticlesDrawMode(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetParticlesDrawMode() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetParticlesDrawMode() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_particles_draw_mode((elf_particles*)particles.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetParticlesTexture(elf_handle particles)
{
	elf_handle handle;
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetParticlesTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetParticlesTexture() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_particles_texture((elf_particles*)particles.get());
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetParticlesModel(elf_handle particles)
{
	elf_handle handle;
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetParticlesModel() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetParticlesModel() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_particles_model((elf_particles*)particles.get());
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetParticlesEntity(elf_handle particles)
{
	elf_handle handle;
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetParticlesEntity() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetParticlesEntity() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_particles_entity((elf_particles*)particles.get());
	return handle;
}
ELF_API elf_vec3f ELF_APIENTRY elfGetParticlesGravity(elf_handle particles)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetParticlesGravity() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetParticlesGravity() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_particles_gravity((elf_particles*)particles.get());
	return _e_type;
}
ELF_API float ELF_APIENTRY elfGetParticlesSpawnDelay(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetParticlesSpawnDelay() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetParticlesSpawnDelay() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_particles_spawn_delay((elf_particles*)particles.get());
}
ELF_API bool ELF_APIENTRY elfGetParticlesSpawn(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetParticlesSpawn() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetParticlesSpawn() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_get_particles_spawn((elf_particles*)particles.get());
}
ELF_API float ELF_APIENTRY elfGetParticlesSizeMin(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetParticlesSizeMin() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetParticlesSizeMin() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_particles_size_min((elf_particles*)particles.get());
}
ELF_API float ELF_APIENTRY elfGetParticlesSizeMax(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetParticlesSizeMax() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetParticlesSizeMax() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_particles_size_max((elf_particles*)particles.get());
}
ELF_API float ELF_APIENTRY elfGetParticlesSizeGrowthMin(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetParticlesSizeGrowthMin() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetParticlesSizeGrowthMin() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_particles_size_growth_min((elf_particles*)particles.get());
}
ELF_API float ELF_APIENTRY elfGetParticlesSizeGrowthMax(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetParticlesSizeGrowthMax() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetParticlesSizeGrowthMax() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_particles_size_growth_max((elf_particles*)particles.get());
}
ELF_API float ELF_APIENTRY elfGetParticlesRotationMin(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetParticlesRotationMin() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetParticlesRotationMin() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_particles_rotation_min((elf_particles*)particles.get());
}
ELF_API float ELF_APIENTRY elfGetParticlesRotationMax(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetParticlesRotationMax() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetParticlesRotationMax() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_particles_rotation_max((elf_particles*)particles.get());
}
ELF_API float ELF_APIENTRY elfGetParticlesRotationGrowthMin(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetParticlesRotationGrowthMin() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetParticlesRotationGrowthMin() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_particles_rotation_growth_min((elf_particles*)particles.get());
}
ELF_API float ELF_APIENTRY elfGetParticlesRotationGrowthMax(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetParticlesRotationGrowthMax() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetParticlesRotationGrowthMax() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_particles_rotation_growth_max((elf_particles*)particles.get());
}
ELF_API float ELF_APIENTRY elfGetParticlesLifeSpanMin(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetParticlesLifeSpanMin() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetParticlesLifeSpanMin() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_particles_life_span_min((elf_particles*)particles.get());
}
ELF_API float ELF_APIENTRY elfGetParticlesLifeSpanMax(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetParticlesLifeSpanMax() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetParticlesLifeSpanMax() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_particles_life_span_max((elf_particles*)particles.get());
}
ELF_API float ELF_APIENTRY elfGetParticlesFadeSpeedMin(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetParticlesFadeSpeedMin() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetParticlesFadeSpeedMin() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_particles_fade_speed_min((elf_particles*)particles.get());
}
ELF_API float ELF_APIENTRY elfGetParticlesFadeSpeedMax(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetParticlesFadeSpeedMax() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetParticlesFadeSpeedMax() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_particles_fade_speed_max((elf_particles*)particles.get());
}
ELF_API elf_vec3f ELF_APIENTRY elfGetParticlesPositionMin(elf_handle particles)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetParticlesPositionMin() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetParticlesPositionMin() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_particles_position_min((elf_particles*)particles.get());
	return _e_type;
}
ELF_API elf_vec3f ELF_APIENTRY elfGetParticlesPositionMax(elf_handle particles)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetParticlesPositionMax() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetParticlesPositionMax() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_particles_position_max((elf_particles*)particles.get());
	return _e_type;
}
ELF_API elf_vec3f ELF_APIENTRY elfGetParticlesVelocityMin(elf_handle particles)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetParticlesVelocityMin() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetParticlesVelocityMin() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_particles_velocity_min((elf_particles*)particles.get());
	return _e_type;
}
ELF_API elf_vec3f ELF_APIENTRY elfGetParticlesVelocityMax(elf_handle particles)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetParticlesVelocityMax() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetParticlesVelocityMax() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_particles_velocity_max((elf_particles*)particles.get());
	return _e_type;
}
ELF_API elf_color ELF_APIENTRY elfGetParticlesColorMin(elf_handle particles)
{
	elf_color _e_type;
	memset(&_e_type, 0x0, sizeof(elf_color));
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetParticlesColorMin() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetParticlesColorMin() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_particles_color_min((elf_particles*)particles.get());
	return _e_type;
}
ELF_API elf_color ELF_APIENTRY elfGetParticlesColorMax(elf_handle particles)
{
	elf_color _e_type;
	memset(&_e_type, 0x0, sizeof(elf_color));
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetParticlesColorMax() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetParticlesColorMax() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_particles_color_max((elf_particles*)particles.get());
	return _e_type;
}
ELF_API elf_handle ELF_APIENTRY elfCreateSprite(const char* name)
{
	elf_handle handle;
	handle = (elf_object*)elf_create_sprite(name);
	return handle;
}
ELF_API void ELF_APIENTRY elfSetSpriteMaterial(elf_handle sprite, elf_handle material)
{
	if(!sprite.get() || elf_get_object_type(sprite.get()) != ELF_SPRITE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetSpriteMaterial() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetSpriteMaterial() -> invalid handle\n");
		}
		return;
	}
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetSpriteMaterial() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetSpriteMaterial() -> invalid handle\n");
		}
		return;
	}
	elf_set_sprite_material((elf_sprite*)sprite.get(), (elf_material*)material.get());
}
ELF_API void ELF_APIENTRY elfSetSpriteScale(elf_handle sprite, float x, float y)
{
	if(!sprite.get() || elf_get_object_type(sprite.get()) != ELF_SPRITE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetSpriteScale() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetSpriteScale() -> invalid handle\n");
		}
		return;
	}
	elf_set_sprite_scale((elf_sprite*)sprite.get(), x, y);
}
ELF_API void ELF_APIENTRY elfSetSpriteFaceCamera(elf_handle sprite, bool face_camera)
{
	if(!sprite.get() || elf_get_object_type(sprite.get()) != ELF_SPRITE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetSpriteFaceCamera() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetSpriteFaceCamera() -> invalid handle\n");
		}
		return;
	}
	elf_set_sprite_face_camera((elf_sprite*)sprite.get(), face_camera);
}
ELF_API elf_handle ELF_APIENTRY elfGetSpriteMaterial(elf_handle sprite)
{
	elf_handle handle;
	if(!sprite.get() || elf_get_object_type(sprite.get()) != ELF_SPRITE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetSpriteMaterial() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetSpriteMaterial() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_sprite_material((elf_sprite*)sprite.get());
	return handle;
}
ELF_API elf_vec2f ELF_APIENTRY elfGetSpriteScale(elf_handle sprite)
{
	elf_vec2f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec2f));
	if(!sprite.get() || elf_get_object_type(sprite.get()) != ELF_SPRITE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetSpriteScale() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetSpriteScale() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_sprite_scale((elf_sprite*)sprite.get());
	return _e_type;
}
ELF_API bool ELF_APIENTRY elfGetSpriteFaceCamera(elf_handle sprite)
{
	if(!sprite.get() || elf_get_object_type(sprite.get()) != ELF_SPRITE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetSpriteFaceCamera() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetSpriteFaceCamera() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_get_sprite_face_camera((elf_sprite*)sprite.get());
}
ELF_API elf_handle ELF_APIENTRY elfCreateSceneFromFile(const char* file_path)
{
	elf_handle handle;
	handle = (elf_object*)elf_create_scene_from_file(file_path);
	return handle;
}
ELF_API void ELF_APIENTRY elfSetSceneAmbientColor(elf_handle scene, float r, float g, float b, float a)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetSceneAmbientColor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetSceneAmbientColor() -> invalid handle\n");
		}
		return;
	}
	elf_set_scene_ambient_color((elf_scene*)scene.get(), r, g, b, a);
}
ELF_API elf_color ELF_APIENTRY elfGetSceneAmbientColor(elf_handle scene)
{
	elf_color _e_type;
	memset(&_e_type, 0x0, sizeof(elf_color));
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetSceneAmbientColor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetSceneAmbientColor() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_scene_ambient_color((elf_scene*)scene.get());
	return _e_type;
}
ELF_API void ELF_APIENTRY elfSetSceneGravity(elf_handle scene, float x, float y, float z)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetSceneGravity() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetSceneGravity() -> invalid handle\n");
		}
		return;
	}
	elf_set_scene_gravity((elf_scene*)scene.get(), x, y, z);
}
ELF_API elf_vec3f ELF_APIENTRY elfGetSceneGravity(elf_handle scene)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetSceneGravity() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetSceneGravity() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_scene_gravity((elf_scene*)scene.get());
	return _e_type;
}
ELF_API const char* ELF_APIENTRY elfGetSceneName(elf_handle scene)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetSceneName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetSceneName() -> invalid handle\n");
		}
		return "";
	}
	return elf_get_scene_name((elf_scene*)scene.get());
}
ELF_API const char* ELF_APIENTRY elfGetSceneFilePath(elf_handle scene)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetSceneFilePath() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetSceneFilePath() -> invalid handle\n");
		}
		return "";
	}
	return elf_get_scene_file_path((elf_scene*)scene.get());
}
ELF_API int ELF_APIENTRY elfGetSceneCameraCount(elf_handle scene)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetSceneCameraCount() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetSceneCameraCount() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_scene_camera_count((elf_scene*)scene.get());
}
ELF_API int ELF_APIENTRY elfGetSceneEntityCount(elf_handle scene)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetSceneEntityCount() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetSceneEntityCount() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_scene_entity_count((elf_scene*)scene.get());
}
ELF_API int ELF_APIENTRY elfGetSceneLightCount(elf_handle scene)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetSceneLightCount() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetSceneLightCount() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_scene_light_count((elf_scene*)scene.get());
}
ELF_API int ELF_APIENTRY elfGetSceneArmatureCount(elf_handle scene)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetSceneArmatureCount() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetSceneArmatureCount() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_scene_armature_count((elf_scene*)scene.get());
}
ELF_API int ELF_APIENTRY elfGetSceneParticlesCount(elf_handle scene)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetSceneParticlesCount() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetSceneParticlesCount() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_scene_particles_count((elf_scene*)scene.get());
}
ELF_API int ELF_APIENTRY elfGetSceneSpriteCount(elf_handle scene)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetSceneSpriteCount() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetSceneSpriteCount() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_scene_sprite_count((elf_scene*)scene.get());
}
ELF_API void ELF_APIENTRY elfAddCameraToScene(elf_handle scene, elf_handle camera)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: AddCameraToScene() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "AddCameraToScene() -> invalid handle\n");
		}
		return;
	}
	if(!camera.get() || elf_get_object_type(camera.get()) != ELF_CAMERA)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: AddCameraToScene() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "AddCameraToScene() -> invalid handle\n");
		}
		return;
	}
	elf_add_camera_to_scene((elf_scene*)scene.get(), (elf_camera*)camera.get());
}
ELF_API void ELF_APIENTRY elfAddEntityToScene(elf_handle scene, elf_handle entity)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: AddEntityToScene() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "AddEntityToScene() -> invalid handle\n");
		}
		return;
	}
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: AddEntityToScene() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "AddEntityToScene() -> invalid handle\n");
		}
		return;
	}
	elf_add_entity_to_scene((elf_scene*)scene.get(), (elf_entity*)entity.get());
}
ELF_API void ELF_APIENTRY elfAddLightToScene(elf_handle scene, elf_handle light)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: AddLightToScene() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "AddLightToScene() -> invalid handle\n");
		}
		return;
	}
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: AddLightToScene() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "AddLightToScene() -> invalid handle\n");
		}
		return;
	}
	elf_add_light_to_scene((elf_scene*)scene.get(), (elf_light*)light.get());
}
ELF_API void ELF_APIENTRY elfAddParticlesToScene(elf_handle scene, elf_handle particles)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: AddParticlesToScene() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "AddParticlesToScene() -> invalid handle\n");
		}
		return;
	}
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: AddParticlesToScene() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "AddParticlesToScene() -> invalid handle\n");
		}
		return;
	}
	elf_add_particles_to_scene((elf_scene*)scene.get(), (elf_particles*)particles.get());
}
ELF_API void ELF_APIENTRY elfAddSpriteToScene(elf_handle scene, elf_handle sprite)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: AddSpriteToScene() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "AddSpriteToScene() -> invalid handle\n");
		}
		return;
	}
	if(!sprite.get() || elf_get_object_type(sprite.get()) != ELF_SPRITE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: AddSpriteToScene() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "AddSpriteToScene() -> invalid handle\n");
		}
		return;
	}
	elf_add_sprite_to_scene((elf_scene*)scene.get(), (elf_sprite*)sprite.get());
}
ELF_API void ELF_APIENTRY elfSetSceneActiveCamera(elf_handle scene, elf_handle camera)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetSceneActiveCamera() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetSceneActiveCamera() -> invalid handle\n");
		}
		return;
	}
	if(!camera.get() || elf_get_object_type(camera.get()) != ELF_CAMERA)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetSceneActiveCamera() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetSceneActiveCamera() -> invalid handle\n");
		}
		return;
	}
	elf_set_scene_active_camera((elf_scene*)scene.get(), (elf_camera*)camera.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetSceneActiveCamera(elf_handle scene)
{
	elf_handle handle;
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetSceneActiveCamera() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetSceneActiveCamera() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_scene_active_camera((elf_scene*)scene.get());
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetSceneRayCastResult(elf_handle scene, float x, float y, float z, float dx, float dy, float dz)
{
	elf_handle handle;
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetSceneRayCastResult() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetSceneRayCastResult() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_scene_ray_cast_result((elf_scene*)scene.get(), x, y, z, dx, dy, dz);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetSceneRayCastResults(elf_handle scene, float x, float y, float z, float dx, float dy, float dz)
{
	elf_handle handle;
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetSceneRayCastResults() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetSceneRayCastResults() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_scene_ray_cast_results((elf_scene*)scene.get(), x, y, z, dx, dy, dz);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetDebugSceneRayCastResult(elf_handle scene, float x, float y, float z, float dx, float dy, float dz)
{
	elf_handle handle;
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetDebugSceneRayCastResult() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetDebugSceneRayCastResult() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_debug_scene_ray_cast_result((elf_scene*)scene.get(), x, y, z, dx, dy, dz);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetDebugSceneRayCastResults(elf_handle scene, float x, float y, float z, float dx, float dy, float dz)
{
	elf_handle handle;
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetDebugSceneRayCastResults() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetDebugSceneRayCastResults() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_debug_scene_ray_cast_results((elf_scene*)scene.get(), x, y, z, dx, dy, dz);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetCameraByIndex(elf_handle scene, int idx)
{
	elf_handle handle;
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetCameraByIndex() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetCameraByIndex() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_camera_by_index((elf_scene*)scene.get(), idx);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetEntityByIndex(elf_handle scene, int idx)
{
	elf_handle handle;
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetEntityByIndex() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetEntityByIndex() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_entity_by_index((elf_scene*)scene.get(), idx);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetLightByIndex(elf_handle scene, int idx)
{
	elf_handle handle;
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetLightByIndex() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetLightByIndex() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_light_by_index((elf_scene*)scene.get(), idx);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetArmatureByIndex(elf_handle scene, int idx)
{
	elf_handle handle;
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetArmatureByIndex() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetArmatureByIndex() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_armature_by_index((elf_scene*)scene.get(), idx);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetParticlesByIndex(elf_handle scene, int idx)
{
	elf_handle handle;
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetParticlesByIndex() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetParticlesByIndex() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_particles_by_index((elf_scene*)scene.get(), idx);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetSpriteByIndex(elf_handle scene, int idx)
{
	elf_handle handle;
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetSpriteByIndex() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetSpriteByIndex() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_sprite_by_index((elf_scene*)scene.get(), idx);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetTextureByName(elf_handle scene, const char* name)
{
	elf_handle handle;
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetTextureByName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetTextureByName() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_texture_by_name((elf_scene*)scene.get(), name);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetMaterialByName(elf_handle scene, const char* name)
{
	elf_handle handle;
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetMaterialByName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetMaterialByName() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_material_by_name((elf_scene*)scene.get(), name);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetModelByName(elf_handle scene, const char* name)
{
	elf_handle handle;
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetModelByName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetModelByName() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_model_by_name((elf_scene*)scene.get(), name);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetScriptByName(elf_handle scene, const char* name)
{
	elf_handle handle;
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetScriptByName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetScriptByName() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_script_by_name((elf_scene*)scene.get(), name);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetCameraByName(elf_handle scene, const char* name)
{
	elf_handle handle;
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetCameraByName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetCameraByName() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_camera_by_name((elf_scene*)scene.get(), name);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetEntityByName(elf_handle scene, const char* name)
{
	elf_handle handle;
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetEntityByName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetEntityByName() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_entity_by_name((elf_scene*)scene.get(), name);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetLightByName(elf_handle scene, const char* name)
{
	elf_handle handle;
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetLightByName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetLightByName() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_light_by_name((elf_scene*)scene.get(), name);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetArmatureByName(elf_handle scene, const char* name)
{
	elf_handle handle;
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetArmatureByName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetArmatureByName() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_armature_by_name((elf_scene*)scene.get(), name);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetParticlesByName(elf_handle scene, const char* name)
{
	elf_handle handle;
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetParticlesByName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetParticlesByName() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_particles_by_name((elf_scene*)scene.get(), name);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetSpriteByName(elf_handle scene, const char* name)
{
	elf_handle handle;
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetSpriteByName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetSpriteByName() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_sprite_by_name((elf_scene*)scene.get(), name);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetActorByName(elf_handle scene, const char* name)
{
	elf_handle handle;
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetActorByName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetActorByName() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_actor_by_name((elf_scene*)scene.get(), name);
	return handle;
}
ELF_API bool ELF_APIENTRY elfRemoveCameraByName(elf_handle scene, const char* name)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveCameraByName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveCameraByName() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_remove_camera_by_name((elf_scene*)scene.get(), name);
}
ELF_API bool ELF_APIENTRY elfRemoveEntityByName(elf_handle scene, const char* name)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveEntityByName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveEntityByName() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_remove_entity_by_name((elf_scene*)scene.get(), name);
}
ELF_API bool ELF_APIENTRY elfRemoveLightByName(elf_handle scene, const char* name)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveLightByName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveLightByName() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_remove_light_by_name((elf_scene*)scene.get(), name);
}
ELF_API bool ELF_APIENTRY elfRemoveParticlesByName(elf_handle scene, const char* name)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveParticlesByName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveParticlesByName() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_remove_particles_by_name((elf_scene*)scene.get(), name);
}
ELF_API bool ELF_APIENTRY elfRemoveCameraByIndex(elf_handle scene, int idx)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveCameraByIndex() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveCameraByIndex() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_remove_camera_by_index((elf_scene*)scene.get(), idx);
}
ELF_API bool ELF_APIENTRY elfRemoveEntityByIndex(elf_handle scene, int idx)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveEntityByIndex() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveEntityByIndex() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_remove_entity_by_index((elf_scene*)scene.get(), idx);
}
ELF_API bool ELF_APIENTRY elfRemoveLightByIndex(elf_handle scene, int idx)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveLightByIndex() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveLightByIndex() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_remove_light_by_index((elf_scene*)scene.get(), idx);
}
ELF_API bool ELF_APIENTRY elfRemoveParticlesByIndex(elf_handle scene, int idx)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveParticlesByIndex() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveParticlesByIndex() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_remove_particles_by_index((elf_scene*)scene.get(), idx);
}
ELF_API bool ELF_APIENTRY elfRemoveCameraByObject(elf_handle scene, elf_handle camera)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveCameraByObject() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveCameraByObject() -> invalid handle\n");
		}
		return false;
	}
	if(!camera.get() || elf_get_object_type(camera.get()) != ELF_CAMERA)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveCameraByObject() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveCameraByObject() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_remove_camera_by_object((elf_scene*)scene.get(), (elf_camera*)camera.get());
}
ELF_API bool ELF_APIENTRY elfRemoveEntityByObject(elf_handle scene, elf_handle entity)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveEntityByObject() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveEntityByObject() -> invalid handle\n");
		}
		return false;
	}
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveEntityByObject() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveEntityByObject() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_remove_entity_by_object((elf_scene*)scene.get(), (elf_entity*)entity.get());
}
ELF_API bool ELF_APIENTRY elfRemoveLightByObject(elf_handle scene, elf_handle light)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveLightByObject() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveLightByObject() -> invalid handle\n");
		}
		return false;
	}
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveLightByObject() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveLightByObject() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_remove_light_by_object((elf_scene*)scene.get(), (elf_light*)light.get());
}
ELF_API bool ELF_APIENTRY elfRemoveParticlesByObject(elf_handle scene, elf_handle particles)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveParticlesByObject() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveParticlesByObject() -> invalid handle\n");
		}
		return false;
	}
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveParticlesByObject() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveParticlesByObject() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_remove_particles_by_object((elf_scene*)scene.get(), (elf_particles*)particles.get());
}
ELF_API bool ELF_APIENTRY elfRemoveSpriteByObject(elf_handle scene, elf_handle sprite)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveSpriteByObject() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveSpriteByObject() -> invalid handle\n");
		}
		return false;
	}
	if(!sprite.get() || elf_get_object_type(sprite.get()) != ELF_SPRITE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveSpriteByObject() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveSpriteByObject() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_remove_sprite_by_object((elf_scene*)scene.get(), (elf_sprite*)sprite.get());
}
ELF_API bool ELF_APIENTRY elfRemoveActorByObject(elf_handle scene, elf_handle actor)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveActorByObject() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveActorByObject() -> invalid handle\n");
		}
		return false;
	}
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveActorByObject() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveActorByObject() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_remove_actor_by_object((elf_scene*)scene.get(), (elf_actor*)actor.get());
}
ELF_API elf_handle ELF_APIENTRY elfCreateScript()
{
	elf_handle handle;
	handle = (elf_object*)elf_create_script();
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfCreateScriptFromFile(const char* file_path)
{
	elf_handle handle;
	handle = (elf_object*)elf_create_script_from_file(file_path);
	return handle;
}
ELF_API const char* ELF_APIENTRY elfGetScriptName(elf_handle script)
{
	if(!script.get() || elf_get_object_type(script.get()) != ELF_SCRIPT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetScriptName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetScriptName() -> invalid handle\n");
		}
		return "";
	}
	return elf_get_script_name((elf_script*)script.get());
}
ELF_API const char* ELF_APIENTRY elfGetScriptFilePath(elf_handle script)
{
	if(!script.get() || elf_get_object_type(script.get()) != ELF_SCRIPT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetScriptFilePath() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetScriptFilePath() -> invalid handle\n");
		}
		return "";
	}
	return elf_get_script_file_path((elf_script*)script.get());
}
ELF_API void ELF_APIENTRY elfSetScriptText(elf_handle script, const char* text)
{
	if(!script.get() || elf_get_object_type(script.get()) != ELF_SCRIPT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetScriptText() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetScriptText() -> invalid handle\n");
		}
		return;
	}
	elf_set_script_text((elf_script*)script.get(), text);
}
ELF_API bool ELF_APIENTRY elfIsScriptError(elf_handle script)
{
	if(!script.get() || elf_get_object_type(script.get()) != ELF_SCRIPT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: IsScriptError() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "IsScriptError() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_is_script_error((elf_script*)script.get());
}
ELF_API bool ELF_APIENTRY elfRunString(const char* str)
{
	return (bool)elf_run_string(str);
}
ELF_API bool ELF_APIENTRY elfRunScript(elf_handle script)
{
	if(!script.get() || elf_get_object_type(script.get()) != ELF_SCRIPT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RunScript() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RunScript() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_run_script((elf_script*)script.get());
}
ELF_API void ELF_APIENTRY elfSetAudioVolume(float volume)
{
	elf_set_audio_volume(volume);
}
ELF_API float ELF_APIENTRY elfGetAudioVolume()
{
	return elf_get_audio_volume();
}
ELF_API void ELF_APIENTRY elfSetAudioRolloff(float rolloff)
{
	elf_set_audio_rolloff(rolloff);
}
ELF_API float ELF_APIENTRY elfGetAudioRolloff()
{
	return elf_get_audio_rolloff();
}
ELF_API elf_handle ELF_APIENTRY elfLoadSound(const char* file_path)
{
	elf_handle handle;
	handle = (elf_object*)elf_load_sound(file_path);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfLoadStreamedSound(const char* file_path)
{
	elf_handle handle;
	handle = (elf_object*)elf_load_streamed_sound(file_path);
	return handle;
}
ELF_API int ELF_APIENTRY elfGetSoundFileType(elf_handle sound)
{
	if(!sound.get() || elf_get_object_type(sound.get()) != ELF_SOUND)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetSoundFileType() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetSoundFileType() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_sound_file_type((elf_sound*)sound.get());
}
ELF_API elf_handle ELF_APIENTRY elfPlaySound(elf_handle sound, float volume)
{
	elf_handle handle;
	if(!sound.get() || elf_get_object_type(sound.get()) != ELF_SOUND)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: PlaySound() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "PlaySound() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_play_sound((elf_sound*)sound.get(), volume);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfPlayEntitySound(elf_handle entity, elf_handle sound, float volume)
{
	elf_handle handle;
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: PlayEntitySound() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "PlayEntitySound() -> invalid handle\n");
		}
		return handle;
	}
	if(!sound.get() || elf_get_object_type(sound.get()) != ELF_SOUND)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: PlayEntitySound() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "PlayEntitySound() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_play_entity_sound((elf_entity*)entity.get(), (elf_sound*)sound.get(), volume);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfLoopSound(elf_handle sound, float volume)
{
	elf_handle handle;
	if(!sound.get() || elf_get_object_type(sound.get()) != ELF_SOUND)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: LoopSound() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "LoopSound() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_loop_sound((elf_sound*)sound.get(), volume);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfLoopEntitySound(elf_handle entity, elf_handle sound, float volume)
{
	elf_handle handle;
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: LoopEntitySound() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "LoopEntitySound() -> invalid handle\n");
		}
		return handle;
	}
	if(!sound.get() || elf_get_object_type(sound.get()) != ELF_SOUND)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: LoopEntitySound() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "LoopEntitySound() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_loop_entity_sound((elf_entity*)entity.get(), (elf_sound*)sound.get(), volume);
	return handle;
}
ELF_API void ELF_APIENTRY elfSetSoundVolume(elf_handle source, float volume)
{
	if(!source.get() || elf_get_object_type(source.get()) != ELF_AUDIO_SOURCE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetSoundVolume() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetSoundVolume() -> invalid handle\n");
		}
		return;
	}
	elf_set_sound_volume((elf_audio_source*)source.get(), volume);
}
ELF_API float ELF_APIENTRY elfGetSoundVolume(elf_handle source)
{
	if(!source.get() || elf_get_object_type(source.get()) != ELF_AUDIO_SOURCE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetSoundVolume() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetSoundVolume() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_sound_volume((elf_audio_source*)source.get());
}
ELF_API void ELF_APIENTRY elfPauseSound(elf_handle source)
{
	if(!source.get() || elf_get_object_type(source.get()) != ELF_AUDIO_SOURCE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: PauseSound() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "PauseSound() -> invalid handle\n");
		}
		return;
	}
	elf_pause_sound((elf_audio_source*)source.get());
}
ELF_API void ELF_APIENTRY elfResumeSound(elf_handle source)
{
	if(!source.get() || elf_get_object_type(source.get()) != ELF_AUDIO_SOURCE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: ResumeSound() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "ResumeSound() -> invalid handle\n");
		}
		return;
	}
	elf_resume_sound((elf_audio_source*)source.get());
}
ELF_API void ELF_APIENTRY elfStopSound(elf_handle source)
{
	if(!source.get() || elf_get_object_type(source.get()) != ELF_AUDIO_SOURCE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: StopSound() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "StopSound() -> invalid handle\n");
		}
		return;
	}
	elf_stop_sound((elf_audio_source*)source.get());
}
ELF_API bool ELF_APIENTRY elfIsSoundPlaying(elf_handle source)
{
	if(!source.get() || elf_get_object_type(source.get()) != ELF_AUDIO_SOURCE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: IsSoundPlaying() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "IsSoundPlaying() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_is_sound_playing((elf_audio_source*)source.get());
}
ELF_API bool ELF_APIENTRY elfIsSoundPaused(elf_handle source)
{
	if(!source.get() || elf_get_object_type(source.get()) != ELF_AUDIO_SOURCE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: IsSoundPaused() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "IsSoundPaused() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_is_sound_paused((elf_audio_source*)source.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetCollisionActor(elf_handle collision)
{
	elf_handle handle;
	if(!collision.get() || elf_get_object_type(collision.get()) != ELF_COLLISION)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetCollisionActor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetCollisionActor() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_collision_actor((elf_collision*)collision.get());
	return handle;
}
ELF_API elf_vec3f ELF_APIENTRY elfGetCollisionPosition(elf_handle collision)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	if(!collision.get() || elf_get_object_type(collision.get()) != ELF_COLLISION)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetCollisionPosition() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetCollisionPosition() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_collision_position((elf_collision*)collision.get());
	return _e_type;
}
ELF_API elf_vec3f ELF_APIENTRY elfGetCollisionNormal(elf_handle collision)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	if(!collision.get() || elf_get_object_type(collision.get()) != ELF_COLLISION)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetCollisionNormal() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetCollisionNormal() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_collision_normal((elf_collision*)collision.get());
	return _e_type;
}
ELF_API float ELF_APIENTRY elfGetCollisionDepth(elf_handle collision)
{
	if(!collision.get() || elf_get_object_type(collision.get()) != ELF_COLLISION)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetCollisionDepth() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetCollisionDepth() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_collision_depth((elf_collision*)collision.get());
}
ELF_API const char* ELF_APIENTRY elfGetJointName(elf_handle joint)
{
	if(!joint.get() || elf_get_object_type(joint.get()) != ELF_JOINT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetJointName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetJointName() -> invalid handle\n");
		}
		return "";
	}
	return elf_get_joint_name((elf_joint*)joint.get());
}
ELF_API int ELF_APIENTRY elfGetJointType(elf_handle joint)
{
	if(!joint.get() || elf_get_object_type(joint.get()) != ELF_JOINT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetJointType() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetJointType() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_joint_type((elf_joint*)joint.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetJointActorA(elf_handle joint)
{
	elf_handle handle;
	if(!joint.get() || elf_get_object_type(joint.get()) != ELF_JOINT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetJointActorA() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetJointActorA() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_joint_actor_a((elf_joint*)joint.get());
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetJointActorB(elf_handle joint)
{
	elf_handle handle;
	if(!joint.get() || elf_get_object_type(joint.get()) != ELF_JOINT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetJointActorB() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetJointActorB() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_joint_actor_b((elf_joint*)joint.get());
	return handle;
}
ELF_API elf_vec3f ELF_APIENTRY elfGetJointPivot(elf_handle joint)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	if(!joint.get() || elf_get_object_type(joint.get()) != ELF_JOINT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetJointPivot() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetJointPivot() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_joint_pivot((elf_joint*)joint.get());
	return _e_type;
}
ELF_API elf_vec3f ELF_APIENTRY elfGetJointAxis(elf_handle joint)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	if(!joint.get() || elf_get_object_type(joint.get()) != ELF_JOINT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetJointAxis() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetJointAxis() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_joint_axis((elf_joint*)joint.get());
	return _e_type;
}
ELF_API elf_handle ELF_APIENTRY elfCreateFontFromFile(const char* file_path, int size)
{
	elf_handle handle;
	handle = (elf_object*)elf_create_font_from_file(file_path, size);
	return handle;
}
ELF_API const char* ELF_APIENTRY elfGetFontName(elf_handle font)
{
	if(!font.get() || elf_get_object_type(font.get()) != ELF_FONT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetFontName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetFontName() -> invalid handle\n");
		}
		return "";
	}
	return elf_get_font_name((elf_font*)font.get());
}
ELF_API const char* ELF_APIENTRY elfGetFontFilePath(elf_handle font)
{
	if(!font.get() || elf_get_object_type(font.get()) != ELF_FONT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetFontFilePath() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetFontFilePath() -> invalid handle\n");
		}
		return "";
	}
	return elf_get_font_file_path((elf_font*)font.get());
}
ELF_API int ELF_APIENTRY elfGetFontSize(elf_handle font)
{
	if(!font.get() || elf_get_object_type(font.get()) != ELF_FONT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetFontSize() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetFontSize() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_font_size((elf_font*)font.get());
}
ELF_API int ELF_APIENTRY elfGetStringWidth(elf_handle font, const char* str)
{
	if(!font.get() || elf_get_object_type(font.get()) != ELF_FONT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetStringWidth() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetStringWidth() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_string_width((elf_font*)font.get(), str);
}
ELF_API int ELF_APIENTRY elfGetStringHeight(elf_handle font, const char* str)
{
	if(!font.get() || elf_get_object_type(font.get()) != ELF_FONT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetStringHeight() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetStringHeight() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_string_height((elf_font*)font.get(), str);
}
ELF_API const char* ELF_APIENTRY elfGetGuiObjectName(elf_handle object)
{
	if(!object.get() || !elf_is_gui_object(object.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetGuiObjectName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetGuiObjectName() -> invalid handle\n");
		}
		return "";
	}
	return elf_get_gui_object_name((elf_gui_object*)object.get());
}
ELF_API elf_vec2i ELF_APIENTRY elfGetGuiObjectPosition(elf_handle object)
{
	elf_vec2i _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec2i));
	if(!object.get() || !elf_is_gui_object(object.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetGuiObjectPosition() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetGuiObjectPosition() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_gui_object_position((elf_gui_object*)object.get());
	return _e_type;
}
ELF_API elf_vec2i ELF_APIENTRY elfGetGuiObjectSize(elf_handle object)
{
	elf_vec2i _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec2i));
	if(!object.get() || !elf_is_gui_object(object.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetGuiObjectSize() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetGuiObjectSize() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_gui_object_size((elf_gui_object*)object.get());
	return _e_type;
}
ELF_API elf_color ELF_APIENTRY elfGetGuiObjectColor(elf_handle object)
{
	elf_color _e_type;
	memset(&_e_type, 0x0, sizeof(elf_color));
	if(!object.get() || !elf_is_gui_object(object.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetGuiObjectColor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetGuiObjectColor() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_gui_object_color((elf_gui_object*)object.get());
	return _e_type;
}
ELF_API bool ELF_APIENTRY elfGetGuiObjectVisible(elf_handle object)
{
	if(!object.get() || !elf_is_gui_object(object.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetGuiObjectVisible() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetGuiObjectVisible() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_get_gui_object_visible((elf_gui_object*)object.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetGuiObjectScript(elf_handle object)
{
	elf_handle handle;
	if(!object.get() || !elf_is_gui_object(object.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetGuiObjectScript() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetGuiObjectScript() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_gui_object_script((elf_gui_object*)object.get());
	return handle;
}
ELF_API int ELF_APIENTRY elfGetGuiObjectEvent(elf_handle object)
{
	if(!object.get() || !elf_is_gui_object(object.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetGuiObjectEvent() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetGuiObjectEvent() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_gui_object_event((elf_gui_object*)object.get());
}
ELF_API void ELF_APIENTRY elfSetGuiObjectPosition(elf_handle object, float x, float y)
{
	if(!object.get() || !elf_is_gui_object(object.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetGuiObjectPosition() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetGuiObjectPosition() -> invalid handle\n");
		}
		return;
	}
	elf_set_gui_object_position((elf_gui_object*)object.get(), x, y);
}
ELF_API void ELF_APIENTRY elfSetGuiObjectColor(elf_handle object, float r, float g, float b, float a)
{
	if(!object.get() || !elf_is_gui_object(object.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetGuiObjectColor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetGuiObjectColor() -> invalid handle\n");
		}
		return;
	}
	elf_set_gui_object_color((elf_gui_object*)object.get(), r, g, b, a);
}
ELF_API void ELF_APIENTRY elfSetGuiObjectVisible(elf_handle object, bool visible)
{
	if(!object.get() || !elf_is_gui_object(object.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetGuiObjectVisible() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetGuiObjectVisible() -> invalid handle\n");
		}
		return;
	}
	elf_set_gui_object_visible((elf_gui_object*)object.get(), visible);
}
ELF_API void ELF_APIENTRY elfSetGuiObjectScript(elf_handle object, elf_handle script)
{
	if(!object.get() || !elf_is_gui_object(object.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetGuiObjectScript() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetGuiObjectScript() -> invalid handle\n");
		}
		return;
	}
	if(!script.get() || elf_get_object_type(script.get()) != ELF_SCRIPT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetGuiObjectScript() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetGuiObjectScript() -> invalid handle\n");
		}
		return;
	}
	elf_set_gui_object_script((elf_gui_object*)object.get(), (elf_script*)script.get());
}
ELF_API elf_handle ELF_APIENTRY elfCreateLabel(const char* name)
{
	elf_handle handle;
	handle = (elf_object*)elf_create_label(name);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetLabelFont(elf_handle label)
{
	elf_handle handle;
	if(!label.get() || elf_get_object_type(label.get()) != ELF_LABEL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetLabelFont() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetLabelFont() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_label_font((elf_label*)label.get());
	return handle;
}
ELF_API const char* ELF_APIENTRY elfGetLabelText(elf_handle label)
{
	if(!label.get() || elf_get_object_type(label.get()) != ELF_LABEL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetLabelText() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetLabelText() -> invalid handle\n");
		}
		return "";
	}
	return elf_get_label_text((elf_label*)label.get());
}
ELF_API void ELF_APIENTRY elfSetLabelFont(elf_handle label, elf_handle font)
{
	if(!label.get() || elf_get_object_type(label.get()) != ELF_LABEL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetLabelFont() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetLabelFont() -> invalid handle\n");
		}
		return;
	}
	if(!font.get() || elf_get_object_type(font.get()) != ELF_FONT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetLabelFont() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetLabelFont() -> invalid handle\n");
		}
		return;
	}
	elf_set_label_font((elf_label*)label.get(), (elf_font*)font.get());
}
ELF_API void ELF_APIENTRY elfSetLabelText(elf_handle label, const char* text)
{
	if(!label.get() || elf_get_object_type(label.get()) != ELF_LABEL)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetLabelText() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetLabelText() -> invalid handle\n");
		}
		return;
	}
	elf_set_label_text((elf_label*)label.get(), text);
}
ELF_API elf_handle ELF_APIENTRY elfCreateButton(const char* name)
{
	elf_handle handle;
	handle = (elf_object*)elf_create_button(name);
	return handle;
}
ELF_API bool ELF_APIENTRY elfGetButtonState(elf_handle button)
{
	if(!button.get() || elf_get_object_type(button.get()) != ELF_BUTTON)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetButtonState() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetButtonState() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_get_button_state((elf_button*)button.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetButtonOffTexture(elf_handle button)
{
	elf_handle handle;
	if(!button.get() || elf_get_object_type(button.get()) != ELF_BUTTON)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetButtonOffTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetButtonOffTexture() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_button_off_texture((elf_button*)button.get());
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetButtonOverTexture(elf_handle button)
{
	elf_handle handle;
	if(!button.get() || elf_get_object_type(button.get()) != ELF_BUTTON)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetButtonOverTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetButtonOverTexture() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_button_over_texture((elf_button*)button.get());
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetButtonOnTexture(elf_handle button)
{
	elf_handle handle;
	if(!button.get() || elf_get_object_type(button.get()) != ELF_BUTTON)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetButtonOnTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetButtonOnTexture() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_button_on_texture((elf_button*)button.get());
	return handle;
}
ELF_API void ELF_APIENTRY elfSetButtonOffTexture(elf_handle button, elf_handle off)
{
	if(!button.get() || elf_get_object_type(button.get()) != ELF_BUTTON)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetButtonOffTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetButtonOffTexture() -> invalid handle\n");
		}
		return;
	}
	if(!off.get() || elf_get_object_type(off.get()) != ELF_TEXTURE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetButtonOffTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetButtonOffTexture() -> invalid handle\n");
		}
		return;
	}
	elf_set_button_off_texture((elf_button*)button.get(), (elf_texture*)off.get());
}
ELF_API void ELF_APIENTRY elfSetButtonOverTexture(elf_handle button, elf_handle over)
{
	if(!button.get() || elf_get_object_type(button.get()) != ELF_BUTTON)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetButtonOverTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetButtonOverTexture() -> invalid handle\n");
		}
		return;
	}
	if(!over.get() || elf_get_object_type(over.get()) != ELF_TEXTURE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetButtonOverTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetButtonOverTexture() -> invalid handle\n");
		}
		return;
	}
	elf_set_button_over_texture((elf_button*)button.get(), (elf_texture*)over.get());
}
ELF_API void ELF_APIENTRY elfSetButtonOnTexture(elf_handle button, elf_handle on)
{
	if(!button.get() || elf_get_object_type(button.get()) != ELF_BUTTON)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetButtonOnTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetButtonOnTexture() -> invalid handle\n");
		}
		return;
	}
	if(!on.get() || elf_get_object_type(on.get()) != ELF_TEXTURE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetButtonOnTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetButtonOnTexture() -> invalid handle\n");
		}
		return;
	}
	elf_set_button_on_texture((elf_button*)button.get(), (elf_texture*)on.get());
}
ELF_API elf_handle ELF_APIENTRY elfCreatePicture(const char* name)
{
	elf_handle handle;
	handle = (elf_object*)elf_create_picture(name);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetPictureTexture(elf_handle picture)
{
	elf_handle handle;
	if(!picture.get() || elf_get_object_type(picture.get()) != ELF_PICTURE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetPictureTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetPictureTexture() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_picture_texture((elf_picture*)picture.get());
	return handle;
}
ELF_API void ELF_APIENTRY elfSetPictureTexture(elf_handle picture, elf_handle texture)
{
	if(!picture.get() || elf_get_object_type(picture.get()) != ELF_PICTURE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetPictureTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetPictureTexture() -> invalid handle\n");
		}
		return;
	}
	if(!texture.get() || elf_get_object_type(texture.get()) != ELF_TEXTURE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetPictureTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetPictureTexture() -> invalid handle\n");
		}
		return;
	}
	elf_set_picture_texture((elf_picture*)picture.get(), (elf_texture*)texture.get());
}
ELF_API elf_handle ELF_APIENTRY elfCreateTextField(const char* name)
{
	elf_handle handle;
	handle = (elf_object*)elf_create_text_field(name);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetTextFieldTexture(elf_handle text_field)
{
	elf_handle handle;
	if(!text_field.get() || elf_get_object_type(text_field.get()) != ELF_TEXT_FIELD)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetTextFieldTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetTextFieldTexture() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_text_field_texture((elf_text_field*)text_field.get());
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetTextFieldFont(elf_handle text_field)
{
	elf_handle handle;
	if(!text_field.get() || elf_get_object_type(text_field.get()) != ELF_TEXT_FIELD)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetTextFieldFont() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetTextFieldFont() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_text_field_font((elf_text_field*)text_field.get());
	return handle;
}
ELF_API elf_color ELF_APIENTRY elfGetTextFieldTextColor(elf_handle text_field)
{
	elf_color _e_type;
	memset(&_e_type, 0x0, sizeof(elf_color));
	if(!text_field.get() || elf_get_object_type(text_field.get()) != ELF_TEXT_FIELD)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetTextFieldTextColor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetTextFieldTextColor() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_text_field_text_color((elf_text_field*)text_field.get());
	return _e_type;
}
ELF_API elf_vec2i ELF_APIENTRY elfGetTextFieldOffset(elf_handle text_field)
{
	elf_vec2i _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec2i));
	if(!text_field.get() || elf_get_object_type(text_field.get()) != ELF_TEXT_FIELD)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetTextFieldOffset() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetTextFieldOffset() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_text_field_offset((elf_text_field*)text_field.get());
	return _e_type;
}
ELF_API const char* ELF_APIENTRY elfGetTextFieldText(elf_handle text_field)
{
	if(!text_field.get() || elf_get_object_type(text_field.get()) != ELF_TEXT_FIELD)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetTextFieldText() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetTextFieldText() -> invalid handle\n");
		}
		return "";
	}
	return elf_get_text_field_text((elf_text_field*)text_field.get());
}
ELF_API void ELF_APIENTRY elfSetTextFieldTexture(elf_handle text_field, elf_handle texture)
{
	if(!text_field.get() || elf_get_object_type(text_field.get()) != ELF_TEXT_FIELD)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetTextFieldTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetTextFieldTexture() -> invalid handle\n");
		}
		return;
	}
	if(!texture.get() || elf_get_object_type(texture.get()) != ELF_TEXTURE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetTextFieldTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetTextFieldTexture() -> invalid handle\n");
		}
		return;
	}
	elf_set_text_field_texture((elf_text_field*)text_field.get(), (elf_texture*)texture.get());
}
ELF_API void ELF_APIENTRY elfSetTextFieldFont(elf_handle text_field, elf_handle font)
{
	if(!text_field.get() || elf_get_object_type(text_field.get()) != ELF_TEXT_FIELD)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetTextFieldFont() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetTextFieldFont() -> invalid handle\n");
		}
		return;
	}
	if(!font.get() || elf_get_object_type(font.get()) != ELF_FONT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetTextFieldFont() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetTextFieldFont() -> invalid handle\n");
		}
		return;
	}
	elf_set_text_field_font((elf_text_field*)text_field.get(), (elf_font*)font.get());
}
ELF_API void ELF_APIENTRY elfSetTextFieldTextColor(elf_handle text_field, float r, float g, float b, float a)
{
	if(!text_field.get() || elf_get_object_type(text_field.get()) != ELF_TEXT_FIELD)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetTextFieldTextColor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetTextFieldTextColor() -> invalid handle\n");
		}
		return;
	}
	elf_set_text_field_text_color((elf_text_field*)text_field.get(), r, g, b, a);
}
ELF_API void ELF_APIENTRY elfSetTextFieldOffset(elf_handle text_field, int offset_x, int offset_y)
{
	if(!text_field.get() || elf_get_object_type(text_field.get()) != ELF_TEXT_FIELD)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetTextFieldOffset() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetTextFieldOffset() -> invalid handle\n");
		}
		return;
	}
	elf_set_text_field_offset((elf_text_field*)text_field.get(), offset_x, offset_y);
}
ELF_API void ELF_APIENTRY elfSetTextFieldText(elf_handle text_field, const char* text)
{
	if(!text_field.get() || elf_get_object_type(text_field.get()) != ELF_TEXT_FIELD)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetTextFieldText() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetTextFieldText() -> invalid handle\n");
		}
		return;
	}
	elf_set_text_field_text((elf_text_field*)text_field.get(), text);
}
ELF_API elf_handle ELF_APIENTRY elfCreateSlider(const char* name)
{
	elf_handle handle;
	handle = (elf_object*)elf_create_slider(name);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetSliderBackgroundTexture(elf_handle slider)
{
	elf_handle handle;
	if(!slider.get() || elf_get_object_type(slider.get()) != ELF_SLIDER)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetSliderBackgroundTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetSliderBackgroundTexture() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_slider_background_texture((elf_slider*)slider.get());
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetSliderSliderTexture(elf_handle slider)
{
	elf_handle handle;
	if(!slider.get() || elf_get_object_type(slider.get()) != ELF_SLIDER)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetSliderSliderTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetSliderSliderTexture() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_slider_slider_texture((elf_slider*)slider.get());
	return handle;
}
ELF_API float ELF_APIENTRY elfGetSliderValue(elf_handle slider)
{
	if(!slider.get() || elf_get_object_type(slider.get()) != ELF_SLIDER)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetSliderValue() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetSliderValue() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_slider_value((elf_slider*)slider.get());
}
ELF_API void ELF_APIENTRY elfSetSliderBackgroundTexture(elf_handle slider, elf_handle background)
{
	if(!slider.get() || elf_get_object_type(slider.get()) != ELF_SLIDER)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetSliderBackgroundTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetSliderBackgroundTexture() -> invalid handle\n");
		}
		return;
	}
	if(!background.get() || elf_get_object_type(background.get()) != ELF_TEXTURE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetSliderBackgroundTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetSliderBackgroundTexture() -> invalid handle\n");
		}
		return;
	}
	elf_set_slider_background_texture((elf_slider*)slider.get(), (elf_texture*)background.get());
}
ELF_API void ELF_APIENTRY elfSetSliderSliderTexture(elf_handle slider, elf_handle slider_texture)
{
	if(!slider.get() || elf_get_object_type(slider.get()) != ELF_SLIDER)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetSliderSliderTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetSliderSliderTexture() -> invalid handle\n");
		}
		return;
	}
	if(!slider_texture.get() || elf_get_object_type(slider_texture.get()) != ELF_TEXTURE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetSliderSliderTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetSliderSliderTexture() -> invalid handle\n");
		}
		return;
	}
	elf_set_slider_slider_texture((elf_slider*)slider.get(), (elf_texture*)slider_texture.get());
}
ELF_API void ELF_APIENTRY elfSetSliderValue(elf_handle slider, float value)
{
	if(!slider.get() || elf_get_object_type(slider.get()) != ELF_SLIDER)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetSliderValue() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetSliderValue() -> invalid handle\n");
		}
		return;
	}
	elf_set_slider_value((elf_slider*)slider.get(), value);
}
ELF_API elf_handle ELF_APIENTRY elfCreateScreen(const char* name)
{
	elf_handle handle;
	handle = (elf_object*)elf_create_screen(name);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetScreenTexture(elf_handle screen)
{
	elf_handle handle;
	if(!screen.get() || elf_get_object_type(screen.get()) != ELF_SCREEN)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetScreenTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetScreenTexture() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_screen_texture((elf_screen*)screen.get());
	return handle;
}
ELF_API void ELF_APIENTRY elfSetScreenTexture(elf_handle screen, elf_handle texture)
{
	if(!screen.get() || elf_get_object_type(screen.get()) != ELF_SCREEN)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetScreenTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetScreenTexture() -> invalid handle\n");
		}
		return;
	}
	if(!texture.get() || elf_get_object_type(texture.get()) != ELF_TEXTURE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetScreenTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetScreenTexture() -> invalid handle\n");
		}
		return;
	}
	elf_set_screen_texture((elf_screen*)screen.get(), (elf_texture*)texture.get());
}
ELF_API elf_handle ELF_APIENTRY elfCreateTextList(const char* name)
{
	elf_handle handle;
	handle = (elf_object*)elf_create_text_list(name);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetTextListFont(elf_handle text_list)
{
	elf_handle handle;
	if(!text_list.get() || elf_get_object_type(text_list.get()) != ELF_TEXT_LIST)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetTextListFont() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetTextListFont() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_text_list_font((elf_text_list*)text_list.get());
	return handle;
}
ELF_API elf_color ELF_APIENTRY elfGetTextListSelectionColor(elf_handle text_list)
{
	elf_color _e_type;
	memset(&_e_type, 0x0, sizeof(elf_color));
	if(!text_list.get() || elf_get_object_type(text_list.get()) != ELF_TEXT_LIST)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetTextListSelectionColor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetTextListSelectionColor() -> invalid handle\n");
		}
		return _e_type;
	}
	_e_type = elf_get_text_list_selection_color((elf_text_list*)text_list.get());
	return _e_type;
}
ELF_API int ELF_APIENTRY elfGetTextListRowCount(elf_handle text_list)
{
	if(!text_list.get() || elf_get_object_type(text_list.get()) != ELF_TEXT_LIST)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetTextListRowCount() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetTextListRowCount() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_text_list_row_count((elf_text_list*)text_list.get());
}
ELF_API int ELF_APIENTRY elfGetTextListItemCount(elf_handle text_list)
{
	if(!text_list.get() || elf_get_object_type(text_list.get()) != ELF_TEXT_LIST)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetTextListItemCount() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetTextListItemCount() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_text_list_item_count((elf_text_list*)text_list.get());
}
ELF_API int ELF_APIENTRY elfGetTextListSelectionIndex(elf_handle text_list)
{
	if(!text_list.get() || elf_get_object_type(text_list.get()) != ELF_TEXT_LIST)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetTextListSelectionIndex() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetTextListSelectionIndex() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_text_list_selection_index((elf_text_list*)text_list.get());
}
ELF_API int ELF_APIENTRY elfGetTextListOffset(elf_handle text_list)
{
	if(!text_list.get() || elf_get_object_type(text_list.get()) != ELF_TEXT_LIST)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetTextListOffset() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetTextListOffset() -> invalid handle\n");
		}
		return 0;
	}
	return elf_get_text_list_offset((elf_text_list*)text_list.get());
}
ELF_API const char* ELF_APIENTRY elfGetTextListItem(elf_handle text_list, int idx)
{
	if(!text_list.get() || elf_get_object_type(text_list.get()) != ELF_TEXT_LIST)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetTextListItem() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetTextListItem() -> invalid handle\n");
		}
		return "";
	}
	return elf_get_text_list_item((elf_text_list*)text_list.get(), idx);
}
ELF_API const char* ELF_APIENTRY elfGetTextListSelectedItem(elf_handle text_list)
{
	if(!text_list.get() || elf_get_object_type(text_list.get()) != ELF_TEXT_LIST)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetTextListSelectedItem() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetTextListSelectedItem() -> invalid handle\n");
		}
		return "";
	}
	return elf_get_text_list_selected_item((elf_text_list*)text_list.get());
}
ELF_API void ELF_APIENTRY elfSetTextListFont(elf_handle text_list, elf_handle font)
{
	if(!text_list.get() || elf_get_object_type(text_list.get()) != ELF_TEXT_LIST)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetTextListFont() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetTextListFont() -> invalid handle\n");
		}
		return;
	}
	if(!font.get() || elf_get_object_type(font.get()) != ELF_FONT)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetTextListFont() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetTextListFont() -> invalid handle\n");
		}
		return;
	}
	elf_set_text_list_font((elf_text_list*)text_list.get(), (elf_font*)font.get());
}
ELF_API void ELF_APIENTRY elfSetTextListSelectionColor(elf_handle text_list, float r, float g, float b, float a)
{
	if(!text_list.get() || elf_get_object_type(text_list.get()) != ELF_TEXT_LIST)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetTextListSelectionColor() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetTextListSelectionColor() -> invalid handle\n");
		}
		return;
	}
	elf_set_text_list_selection_color((elf_text_list*)text_list.get(), r, g, b, a);
}
ELF_API void ELF_APIENTRY elfSetTextListSize(elf_handle text_list, int rows, int width)
{
	if(!text_list.get() || elf_get_object_type(text_list.get()) != ELF_TEXT_LIST)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetTextListSize() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetTextListSize() -> invalid handle\n");
		}
		return;
	}
	elf_set_text_list_size((elf_text_list*)text_list.get(), rows, width);
}
ELF_API void ELF_APIENTRY elfAddTextListItem(elf_handle text_list, const char* text)
{
	if(!text_list.get() || elf_get_object_type(text_list.get()) != ELF_TEXT_LIST)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: AddTextListItem() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "AddTextListItem() -> invalid handle\n");
		}
		return;
	}
	elf_add_text_list_item((elf_text_list*)text_list.get(), text);
}
ELF_API bool ELF_APIENTRY elfRemoveTextListItem(elf_handle text_list, int idx)
{
	if(!text_list.get() || elf_get_object_type(text_list.get()) != ELF_TEXT_LIST)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveTextListItem() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveTextListItem() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_remove_text_list_item((elf_text_list*)text_list.get(), idx);
}
ELF_API void ELF_APIENTRY elfRemoveTextListItems(elf_handle text_list)
{
	if(!text_list.get() || elf_get_object_type(text_list.get()) != ELF_TEXT_LIST)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveTextListItems() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveTextListItems() -> invalid handle\n");
		}
		return;
	}
	elf_remove_text_list_items((elf_text_list*)text_list.get());
}
ELF_API void ELF_APIENTRY elfSetTextListOffset(elf_handle text_list, int offset)
{
	if(!text_list.get() || elf_get_object_type(text_list.get()) != ELF_TEXT_LIST)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetTextListOffset() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetTextListOffset() -> invalid handle\n");
		}
		return;
	}
	elf_set_text_list_offset((elf_text_list*)text_list.get(), offset);
}
ELF_API void ELF_APIENTRY elfSetTextListSelection(elf_handle text_list, int selection)
{
	if(!text_list.get() || elf_get_object_type(text_list.get()) != ELF_TEXT_LIST)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetTextListSelection() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetTextListSelection() -> invalid handle\n");
		}
		return;
	}
	elf_set_text_list_selection((elf_text_list*)text_list.get(), selection);
}
ELF_API elf_handle ELF_APIENTRY elfCreateCheckBox(const char* name)
{
	elf_handle handle;
	handle = (elf_object*)elf_create_check_box(name);
	return handle;
}
ELF_API bool ELF_APIENTRY elfGetCheckBoxState(elf_handle check_box)
{
	if(!check_box.get() || elf_get_object_type(check_box.get()) != ELF_CHECK_BOX)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetCheckBoxState() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetCheckBoxState() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_get_check_box_state((elf_check_box*)check_box.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetCheckBoxOffTexture(elf_handle check_box)
{
	elf_handle handle;
	if(!check_box.get() || elf_get_object_type(check_box.get()) != ELF_CHECK_BOX)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetCheckBoxOffTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetCheckBoxOffTexture() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_check_box_off_texture((elf_check_box*)check_box.get());
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetCheckBoxOnTexture(elf_handle check_box)
{
	elf_handle handle;
	if(!check_box.get() || elf_get_object_type(check_box.get()) != ELF_CHECK_BOX)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetCheckBoxOnTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetCheckBoxOnTexture() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_check_box_on_texture((elf_check_box*)check_box.get());
	return handle;
}
ELF_API void ELF_APIENTRY elfSetCheckBoxOffTexture(elf_handle check_box, elf_handle off)
{
	if(!check_box.get() || elf_get_object_type(check_box.get()) != ELF_CHECK_BOX)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetCheckBoxOffTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetCheckBoxOffTexture() -> invalid handle\n");
		}
		return;
	}
	if(!off.get() || elf_get_object_type(off.get()) != ELF_TEXTURE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetCheckBoxOffTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetCheckBoxOffTexture() -> invalid handle\n");
		}
		return;
	}
	elf_set_check_box_off_texture((elf_check_box*)check_box.get(), (elf_texture*)off.get());
}
ELF_API void ELF_APIENTRY elfSetCheckBoxOnTexture(elf_handle check_box, elf_handle on)
{
	if(!check_box.get() || elf_get_object_type(check_box.get()) != ELF_CHECK_BOX)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetCheckBoxOnTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetCheckBoxOnTexture() -> invalid handle\n");
		}
		return;
	}
	if(!on.get() || elf_get_object_type(on.get()) != ELF_TEXTURE)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetCheckBoxOnTexture() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetCheckBoxOnTexture() -> invalid handle\n");
		}
		return;
	}
	elf_set_check_box_on_texture((elf_check_box*)check_box.get(), (elf_texture*)on.get());
}
ELF_API void ELF_APIENTRY elfSetCheckBoxState(elf_handle check_box, bool state)
{
	if(!check_box.get() || elf_get_object_type(check_box.get()) != ELF_CHECK_BOX)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: SetCheckBoxState() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "SetCheckBoxState() -> invalid handle\n");
		}
		return;
	}
	elf_set_check_box_state((elf_check_box*)check_box.get(), state);
}
ELF_API elf_handle ELF_APIENTRY elfCreateGui()
{
	elf_handle handle;
	handle = (elf_object*)elf_create_gui();
	return handle;
}
ELF_API bool ELF_APIENTRY elfAddGuiObject(elf_handle parent, elf_handle object)
{
	if(!parent.get() || !elf_is_gui_object(parent.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: AddGuiObject() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "AddGuiObject() -> invalid handle\n");
		}
		return false;
	}
	if(!object.get() || !elf_is_gui_object(object.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: AddGuiObject() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "AddGuiObject() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_add_gui_object((elf_gui_object*)parent.get(), (elf_gui_object*)object.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetGuiObjectByName(elf_handle parent, const char* name)
{
	elf_handle handle;
	if(!parent.get() || !elf_is_gui_object(parent.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetGuiObjectByName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetGuiObjectByName() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_gui_object_by_name((elf_gui_object*)parent.get(), name);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetGuiObjectByIndex(elf_handle parent, int idx)
{
	elf_handle handle;
	if(!parent.get() || !elf_is_gui_object(parent.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: GetGuiObjectByIndex() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "GetGuiObjectByIndex() -> invalid handle\n");
		}
		return handle;
	}
	handle = (elf_object*)elf_get_gui_object_by_index((elf_gui_object*)parent.get(), idx);
	return handle;
}
ELF_API bool ELF_APIENTRY elfRemoveGuiObjectByName(elf_handle parent, const char* name)
{
	if(!parent.get() || !elf_is_gui_object(parent.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveGuiObjectByName() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveGuiObjectByName() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_remove_gui_object_by_name((elf_gui_object*)parent.get(), name);
}
ELF_API bool ELF_APIENTRY elfRemoveGuiObjectByIndex(elf_handle parent, int idx)
{
	if(!parent.get() || !elf_is_gui_object(parent.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveGuiObjectByIndex() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveGuiObjectByIndex() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_remove_gui_object_by_index((elf_gui_object*)parent.get(), idx);
}
ELF_API bool ELF_APIENTRY elfRemoveGuiObjectByObject(elf_handle parent, elf_handle object)
{
	if(!parent.get() || !elf_is_gui_object(parent.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveGuiObjectByObject() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveGuiObjectByObject() -> invalid handle\n");
		}
		return false;
	}
	if(!object.get() || !elf_is_gui_object(object.get()))
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: RemoveGuiObjectByObject() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "RemoveGuiObjectByObject() -> invalid handle\n");
		}
		return false;
	}
	return (bool)elf_remove_gui_object_by_object((elf_gui_object*)parent.get(), (elf_gui_object*)object.get());
}
ELF_API void ELF_APIENTRY elfEmptyGui(elf_handle gui)
{
	if(!gui.get() || elf_get_object_type(gui.get()) != ELF_GUI)
	{
		elf_script *script = elf_get_current_script();
		if(script)
		{
			int line = elf_get_current_script_line();
			elf_set_error_no_save(ELF_INVALID_HANDLE, "[script \"%s\" %s]:%d: EmptyGui() -> invalid handle\n", elf_get_script_name(script), elf_get_script_file_path(script), line);
		}
		else
		{
			elf_set_error_no_save(ELF_INVALID_HANDLE, "EmptyGui() -> invalid handle\n");
		}
		return;
	}
	elf_empty_gui((elf_gui*)gui.get());
}
ELF_API bool ELF_APIENTRY elfCreateSession(const char* address, unsigned short port)
{
	return (bool)elf_create_session(address, port);
}
ELF_API bool ELF_APIENTRY elfConnectSession(const char* address, unsigned short port)
{
	return (bool)elf_connect_session(address, port);
}
ELF_API bool ELF_APIENTRY elfDisconnectSession()
{
	return (bool)elf_disconnect_session();
}
ELF_API void ELF_APIENTRY elfSendStringToClients(const char* message)
{
	elf_send_string_to_clients(message);
}
ELF_API void ELF_APIENTRY elfSendStringToServer(const char* message)
{
	elf_send_string_to_server(message);
}
ELF_API const char* ELF_APIENTRY elfGetServerDataAsString()
{
	return elf_get_server_data_as_string();
}
ELF_API const char* ELF_APIENTRY elfGetClientDataAsString()
{
	return elf_get_client_data_as_string();
}
ELF_API int ELF_APIENTRY elfGetServerEvent()
{
	return elf_get_server_event();
}
ELF_API int ELF_APIENTRY elfGetClientEvent()
{
	return elf_get_client_event();
}
ELF_API int ELF_APIENTRY elfGetCurrentClient()
{
	return elf_get_current_client();
}
ELF_API bool ELF_APIENTRY elfIsServer()
{
	return (bool)elf_is_server();
}
ELF_API bool ELF_APIENTRY elfIsClient()
{
	return (bool)elf_is_client();
}

