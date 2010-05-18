#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <AL/alc.h>
#include <AL/al.h>

#include "gfx.h"
#include "blendelf.h"
#include "binds.h"

#if defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__)
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
		printf("GetObjectType() -> invalid handle\n");
		return 0;
	}
	return elf_get_object_type((elf_object*)obj.get());
}
ELF_API int ELF_APIENTRY elfGetObjectRefCount(elf_handle obj)
{
	if(!obj.get())
	{
		printf("GetObjectRefCount() -> invalid handle\n");
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
		printf("IsActor() -> invalid handle\n");
		return false;
	}
	return (bool)elf_is_actor((elf_object*)obj.get());
}
ELF_API bool ELF_APIENTRY elfIsGuiObject(elf_handle obj)
{
	if(!obj.get())
	{
		printf("IsGuiObject() -> invalid handle\n");
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
		printf("GetListLength() -> invalid handle\n");
		return 0;
	}
	return elf_get_list_length((elf_list*)list.get());
}
ELF_API void ELF_APIENTRY elfInsertToList(elf_handle list, int idx, elf_handle obj)
{
	if(!list.get() || elf_get_object_type(list.get()) != ELF_LIST)
	{
		printf("InsertToList() -> invalid handle\n");
		return;
	}
	if(!obj.get())
	{
		printf("InsertToList() -> invalid handle\n");
		return;
	}
	elf_insert_to_list((elf_list*)list.get(), idx, (elf_object*)obj.get());
}
ELF_API void ELF_APIENTRY elfAppendToList(elf_handle list, elf_handle obj)
{
	if(!list.get() || elf_get_object_type(list.get()) != ELF_LIST)
	{
		printf("AppendToList() -> invalid handle\n");
		return;
	}
	if(!obj.get())
	{
		printf("AppendToList() -> invalid handle\n");
		return;
	}
	elf_append_to_list((elf_list*)list.get(), (elf_object*)obj.get());
}
ELF_API bool ELF_APIENTRY elfRemoveFromList(elf_handle list, elf_handle obj)
{
	if(!list.get() || elf_get_object_type(list.get()) != ELF_LIST)
	{
		printf("RemoveFromList() -> invalid handle\n");
		return false;
	}
	if(!obj.get())
	{
		printf("RemoveFromList() -> invalid handle\n");
		return false;
	}
	return (bool)elf_remove_from_list((elf_list*)list.get(), (elf_object*)obj.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetItemFromList(elf_handle list, int idx)
{
	elf_handle handle;
	if(!list.get() || elf_get_object_type(list.get()) != ELF_LIST)
	{
		printf("GetItemFromList() -> invalid handle\n");
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
		printf("BeginList() -> invalid handle\n");
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
		printf("NextInList() -> invalid handle\n");
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
		printf("RbeginList() -> invalid handle\n");
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
		printf("RnextInList() -> invalid handle\n");
		return handle;
	}
	handle = (elf_object*)elf_rnext_in_list((elf_list*)list.get());
	return handle;
}
ELF_API void ELF_APIENTRY elfSeekList(elf_handle list, elf_handle ptr)
{
	if(!list.get() || elf_get_object_type(list.get()) != ELF_LIST)
	{
		printf("SeekList() -> invalid handle\n");
		return;
	}
	if(!ptr.get())
	{
		printf("SeekList() -> invalid handle\n");
		return;
	}
	elf_seek_list((elf_list*)list.get(), (elf_object*)ptr.get());
}
ELF_API void ELF_APIENTRY elfRseekList(elf_handle list, elf_handle ptr)
{
	if(!list.get() || elf_get_object_type(list.get()) != ELF_LIST)
	{
		printf("RseekList() -> invalid handle\n");
		return;
	}
	if(!ptr.get())
	{
		printf("RseekList() -> invalid handle\n");
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
		printf("GetGameConfigWindowWidth() -> invalid handle\n");
		return 0;
	}
	return elf_get_game_config_window_width((elf_game_config*)config.get());
}
ELF_API int ELF_APIENTRY elfGetGameConfigWindowHeight(elf_handle config)
{
	if(!config.get() || elf_get_object_type(config.get()) != ELF_GAME_CONFIG)
	{
		printf("GetGameConfigWindowHeight() -> invalid handle\n");
		return 0;
	}
	return elf_get_game_config_window_height((elf_game_config*)config.get());
}
ELF_API bool ELF_APIENTRY elfGetGameConfigFullscreen(elf_handle config)
{
	if(!config.get() || elf_get_object_type(config.get()) != ELF_GAME_CONFIG)
	{
		printf("GetGameConfigFullscreen() -> invalid handle\n");
		return false;
	}
	return (bool)elf_get_game_config_fullscreen((elf_game_config*)config.get());
}
ELF_API float ELF_APIENTRY elfGetGameConfigTextureAnisotropy(elf_handle config)
{
	if(!config.get() || elf_get_object_type(config.get()) != ELF_GAME_CONFIG)
	{
		printf("GetGameConfigTextureAnisotropy() -> invalid handle\n");
		return 0;
	}
	return elf_get_game_config_texture_anisotropy((elf_game_config*)config.get());
}
ELF_API int ELF_APIENTRY elfGetGameConfigShadowMapSize(elf_handle config)
{
	if(!config.get() || elf_get_object_type(config.get()) != ELF_GAME_CONFIG)
	{
		printf("GetGameConfigShadowMapSize() -> invalid handle\n");
		return 0;
	}
	return elf_get_game_config_shadow_map_size((elf_game_config*)config.get());
}
ELF_API const char* ELF_APIENTRY elfGetGameConfigStart(elf_handle config)
{
	if(!config.get() || elf_get_object_type(config.get()) != ELF_GAME_CONFIG)
	{
		printf("GetGameConfigStart() -> invalid handle\n");
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
		printf("SetScene() -> invalid handle\n");
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
		printf("SetGui() -> invalid handle\n");
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
		printf("UpdateFramePlayer() -> invalid handle\n");
		return;
	}
	elf_update_frame_player((elf_frame_player*)player.get());
}
ELF_API void ELF_APIENTRY elfSetFramePlayerFrame(elf_handle player, float frame)
{
	if(!player.get() || elf_get_object_type(player.get()) != ELF_FRAME_PLAYER)
	{
		printf("SetFramePlayerFrame() -> invalid handle\n");
		return;
	}
	elf_set_frame_player_frame((elf_frame_player*)player.get(), frame);
}
ELF_API void ELF_APIENTRY elfPlayFramePlayer(elf_handle player, float start, float end, float speed)
{
	if(!player.get() || elf_get_object_type(player.get()) != ELF_FRAME_PLAYER)
	{
		printf("PlayFramePlayer() -> invalid handle\n");
		return;
	}
	elf_play_frame_player((elf_frame_player*)player.get(), start, end, speed);
}
ELF_API void ELF_APIENTRY elfLoopFramePlayer(elf_handle player, float start, float end, float speed)
{
	if(!player.get() || elf_get_object_type(player.get()) != ELF_FRAME_PLAYER)
	{
		printf("LoopFramePlayer() -> invalid handle\n");
		return;
	}
	elf_loop_frame_player((elf_frame_player*)player.get(), start, end, speed);
}
ELF_API void ELF_APIENTRY elfStopFramePlayer(elf_handle player)
{
	if(!player.get() || elf_get_object_type(player.get()) != ELF_FRAME_PLAYER)
	{
		printf("StopFramePlayer() -> invalid handle\n");
		return;
	}
	elf_stop_frame_player((elf_frame_player*)player.get());
}
ELF_API void ELF_APIENTRY elfPauseFramePlayer(elf_handle player)
{
	if(!player.get() || elf_get_object_type(player.get()) != ELF_FRAME_PLAYER)
	{
		printf("PauseFramePlayer() -> invalid handle\n");
		return;
	}
	elf_pause_frame_player((elf_frame_player*)player.get());
}
ELF_API void ELF_APIENTRY elfResumeFramePlayer(elf_handle player)
{
	if(!player.get() || elf_get_object_type(player.get()) != ELF_FRAME_PLAYER)
	{
		printf("ResumeFramePlayer() -> invalid handle\n");
		return;
	}
	elf_resume_frame_player((elf_frame_player*)player.get());
}
ELF_API float ELF_APIENTRY elfGetFramePlayerStart(elf_handle player)
{
	if(!player.get() || elf_get_object_type(player.get()) != ELF_FRAME_PLAYER)
	{
		printf("GetFramePlayerStart() -> invalid handle\n");
		return 0;
	}
	return elf_get_frame_player_start((elf_frame_player*)player.get());
}
ELF_API float ELF_APIENTRY elfGetFramePlayerEnd(elf_handle player)
{
	if(!player.get() || elf_get_object_type(player.get()) != ELF_FRAME_PLAYER)
	{
		printf("GetFramePlayerEnd() -> invalid handle\n");
		return 0;
	}
	return elf_get_frame_player_end((elf_frame_player*)player.get());
}
ELF_API float ELF_APIENTRY elfGetFramePlayerSpeed(elf_handle player)
{
	if(!player.get() || elf_get_object_type(player.get()) != ELF_FRAME_PLAYER)
	{
		printf("GetFramePlayerSpeed() -> invalid handle\n");
		return 0;
	}
	return elf_get_frame_player_speed((elf_frame_player*)player.get());
}
ELF_API float ELF_APIENTRY elfGetFramePlayerFrame(elf_handle player)
{
	if(!player.get() || elf_get_object_type(player.get()) != ELF_FRAME_PLAYER)
	{
		printf("GetFramePlayerFrame() -> invalid handle\n");
		return 0;
	}
	return elf_get_frame_player_frame((elf_frame_player*)player.get());
}
ELF_API bool ELF_APIENTRY elfIsFramePlayerPlaying(elf_handle player)
{
	if(!player.get() || elf_get_object_type(player.get()) != ELF_FRAME_PLAYER)
	{
		printf("IsFramePlayerPlaying() -> invalid handle\n");
		return false;
	}
	return (bool)elf_is_frame_player_playing((elf_frame_player*)player.get());
}
ELF_API bool ELF_APIENTRY elfIsFramePlayerPaused(elf_handle player)
{
	if(!player.get() || elf_get_object_type(player.get()) != ELF_FRAME_PLAYER)
	{
		printf("IsFramePlayerPaused() -> invalid handle\n");
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
		printf("StartTimer() -> invalid handle\n");
		return;
	}
	elf_start_timer((elf_timer*)timer.get());
}
ELF_API double ELF_APIENTRY elfGetElapsedTime(elf_handle timer)
{
	if(!timer.get() || elf_get_object_type(timer.get()) != ELF_TIMER)
	{
		printf("GetElapsedTime() -> invalid handle\n");
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
		printf("GetTextureName() -> invalid handle\n");
		return "";
	}
	return elf_get_texture_name((elf_texture*)texture.get());
}
ELF_API const char* ELF_APIENTRY elfGetTextureFilePath(elf_handle texture)
{
	if(!texture.get() || elf_get_object_type(texture.get()) != ELF_TEXTURE)
	{
		printf("GetTextureFilePath() -> invalid handle\n");
		return "";
	}
	return elf_get_texture_file_path((elf_texture*)texture.get());
}
ELF_API int ELF_APIENTRY elfGetTextureWidth(elf_handle texture)
{
	if(!texture.get() || elf_get_object_type(texture.get()) != ELF_TEXTURE)
	{
		printf("GetTextureWidth() -> invalid handle\n");
		return 0;
	}
	return elf_get_texture_width((elf_texture*)texture.get());
}
ELF_API int ELF_APIENTRY elfGetTextureHeight(elf_handle texture)
{
	if(!texture.get() || elf_get_object_type(texture.get()) != ELF_TEXTURE)
	{
		printf("GetTextureHeight() -> invalid handle\n");
		return 0;
	}
	return elf_get_texture_height((elf_texture*)texture.get());
}
ELF_API int ELF_APIENTRY elfGetTextureFormat(elf_handle texture)
{
	if(!texture.get() || elf_get_object_type(texture.get()) != ELF_TEXTURE)
	{
		printf("GetTextureFormat() -> invalid handle\n");
		return 0;
	}
	return elf_get_texture_format((elf_texture*)texture.get());
}
ELF_API int ELF_APIENTRY elfGetTextureDataFormat(elf_handle texture)
{
	if(!texture.get() || elf_get_object_type(texture.get()) != ELF_TEXTURE)
	{
		printf("GetTextureDataFormat() -> invalid handle\n");
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
		printf("SetMaterialTexture() -> invalid handle\n");
		return;
	}
	if(!texture.get() || elf_get_object_type(texture.get()) != ELF_TEXTURE)
	{
		printf("SetMaterialTexture() -> invalid handle\n");
		return;
	}
	elf_set_material_texture((elf_material*)material.get(), slot, (elf_texture*)texture.get());
}
ELF_API void ELF_APIENTRY elfSetMaterialTextureType(elf_handle material, int slot, int type)
{
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		printf("SetMaterialTextureType() -> invalid handle\n");
		return;
	}
	elf_set_material_texture_type((elf_material*)material.get(), slot, type);
}
ELF_API void ELF_APIENTRY elfSetMaterialTextureParallaxScale(elf_handle material, int slot, float scale)
{
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		printf("SetMaterialTextureParallaxScale() -> invalid handle\n");
		return;
	}
	elf_set_material_texture_parallax_scale((elf_material*)material.get(), slot, scale);
}
ELF_API void ELF_APIENTRY elfSetMaterialDiffuseColor(elf_handle material, float r, float g, float b, float a)
{
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		printf("SetMaterialDiffuseColor() -> invalid handle\n");
		return;
	}
	elf_set_material_diffuse_color((elf_material*)material.get(), r, g, b, a);
}
ELF_API void ELF_APIENTRY elfSetMaterialSpecularColor(elf_handle material, float r, float g, float b, float a)
{
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		printf("SetMaterialSpecularColor() -> invalid handle\n");
		return;
	}
	elf_set_material_specular_color((elf_material*)material.get(), r, g, b, a);
}
ELF_API void ELF_APIENTRY elfSetMaterialAmbientColor(elf_handle material, float r, float g, float b, float a)
{
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		printf("SetMaterialAmbientColor() -> invalid handle\n");
		return;
	}
	elf_set_material_ambient_color((elf_material*)material.get(), r, g, b, a);
}
ELF_API void ELF_APIENTRY elfSetMaterialSpecularPower(elf_handle material, float power)
{
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		printf("SetMaterialSpecularPower() -> invalid handle\n");
		return;
	}
	elf_set_material_specular_power((elf_material*)material.get(), power);
}
ELF_API void ELF_APIENTRY elfSetMaterialLighting(elf_handle material, bool lighting)
{
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		printf("SetMaterialLighting() -> invalid handle\n");
		return;
	}
	elf_set_material_lighting((elf_material*)material.get(), lighting);
}
ELF_API const char* ELF_APIENTRY elfGetMaterialName(elf_handle material)
{
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		printf("GetMaterialName() -> invalid handle\n");
		return "";
	}
	return elf_get_material_name((elf_material*)material.get());
}
ELF_API const char* ELF_APIENTRY elfGetMaterialFilePath(elf_handle material)
{
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		printf("GetMaterialFilePath() -> invalid handle\n");
		return "";
	}
	return elf_get_material_file_path((elf_material*)material.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetMaterialTexture(elf_handle material, int slot)
{
	elf_handle handle;
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		printf("GetMaterialTexture() -> invalid handle\n");
		return handle;
	}
	handle = (elf_object*)elf_get_material_texture((elf_material*)material.get(), slot);
	return handle;
}
ELF_API int ELF_APIENTRY elfGetMaterialTextureType(elf_handle material, int slot)
{
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		printf("GetMaterialTextureType() -> invalid handle\n");
		return 0;
	}
	return elf_get_material_texture_type((elf_material*)material.get(), slot);
}
ELF_API float ELF_APIENTRY elfGetMaterialTextureParallaxScale(elf_handle material, int slot)
{
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		printf("GetMaterialTextureParallaxScale() -> invalid handle\n");
		return 0;
	}
	return elf_get_material_texture_parallax_scale((elf_material*)material.get(), slot);
}
ELF_API elf_color ELF_APIENTRY elfGetMaterialDiffuseColor(elf_handle material)
{
	elf_color _e_type;
	memset(&_e_type, 0x0, sizeof(elf_color));
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		printf("GetMaterialDiffuseColor() -> invalid handle\n");
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
		printf("GetMaterialSpecularColor() -> invalid handle\n");
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
		printf("GetMaterialAmbientColor() -> invalid handle\n");
		return _e_type;
	}
	_e_type = elf_get_material_ambient_color((elf_material*)material.get());
	return _e_type;
}
ELF_API float ELF_APIENTRY elfGetMaterialSpecularPower(elf_handle material)
{
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		printf("GetMaterialSpecularPower() -> invalid handle\n");
		return 0;
	}
	return elf_get_material_specular_power((elf_material*)material.get());
}
ELF_API bool ELF_APIENTRY elfGetMaterialLighting(elf_handle material)
{
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		printf("GetMaterialLighting() -> invalid handle\n");
		return false;
	}
	return (bool)elf_get_material_lighting((elf_material*)material.get());
}
ELF_API void ELF_APIENTRY elfAddPointToBezierCurve(elf_handle curve, elf_handle point)
{
	if(!curve.get() || elf_get_object_type(curve.get()) != ELF_BEZIER_CURVE)
	{
		printf("AddPointToBezierCurve() -> invalid handle\n");
		return;
	}
	if(!point.get() || elf_get_object_type(point.get()) != ELF_BEZIER_POINT)
	{
		printf("AddPointToBezierCurve() -> invalid handle\n");
		return;
	}
	elf_add_point_to_bezier_curve((elf_bezier_curve*)curve.get(), (elf_bezier_point*)point.get());
}
ELF_API bool ELF_APIENTRY elfAddCurveToIpo(elf_handle ipo, elf_handle curve)
{
	if(!ipo.get() || elf_get_object_type(ipo.get()) != ELF_IPO)
	{
		printf("AddCurveToIpo() -> invalid handle\n");
		return false;
	}
	if(!curve.get() || elf_get_object_type(curve.get()) != ELF_BEZIER_CURVE)
	{
		printf("AddCurveToIpo() -> invalid handle\n");
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
		printf("GetIpoLoc() -> invalid handle\n");
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
		printf("GetIpoRot() -> invalid handle\n");
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
		printf("GetIpoScale() -> invalid handle\n");
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
		printf("GetIpoQua() -> invalid handle\n");
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
		printf("GetPropertyType() -> invalid handle\n");
		return 0;
	}
	return elf_get_property_type((elf_property*)property.get());
}
ELF_API int ELF_APIENTRY elfGetPropertyInt(elf_handle property)
{
	if(!property.get() || elf_get_object_type(property.get()) != ELF_PROPERTY)
	{
		printf("GetPropertyInt() -> invalid handle\n");
		return 0;
	}
	return elf_get_property_int((elf_property*)property.get());
}
ELF_API float ELF_APIENTRY elfGetPropertyFloat(elf_handle property)
{
	if(!property.get() || elf_get_object_type(property.get()) != ELF_PROPERTY)
	{
		printf("GetPropertyFloat() -> invalid handle\n");
		return 0;
	}
	return elf_get_property_float((elf_property*)property.get());
}
ELF_API const char* ELF_APIENTRY elfGetPropertyString(elf_handle property)
{
	if(!property.get() || elf_get_object_type(property.get()) != ELF_PROPERTY)
	{
		printf("GetPropertyString() -> invalid handle\n");
		return "";
	}
	return elf_get_property_string((elf_property*)property.get());
}
ELF_API bool ELF_APIENTRY elfGetPropertyBool(elf_handle property)
{
	if(!property.get() || elf_get_object_type(property.get()) != ELF_PROPERTY)
	{
		printf("GetPropertyBool() -> invalid handle\n");
		return false;
	}
	return (bool)elf_get_property_bool((elf_property*)property.get());
}
ELF_API void ELF_APIENTRY elfSetPropertyInt(elf_handle property, int ival)
{
	if(!property.get() || elf_get_object_type(property.get()) != ELF_PROPERTY)
	{
		printf("SetPropertyInt() -> invalid handle\n");
		return;
	}
	elf_set_property_int((elf_property*)property.get(), ival);
}
ELF_API void ELF_APIENTRY elfSetPropertyFloat(elf_handle property, float fval)
{
	if(!property.get() || elf_get_object_type(property.get()) != ELF_PROPERTY)
	{
		printf("SetPropertyFloat() -> invalid handle\n");
		return;
	}
	elf_set_property_float((elf_property*)property.get(), fval);
}
ELF_API void ELF_APIENTRY elfSetPropertyString(elf_handle property, const char* sval)
{
	if(!property.get() || elf_get_object_type(property.get()) != ELF_PROPERTY)
	{
		printf("SetPropertyString() -> invalid handle\n");
		return;
	}
	elf_set_property_string((elf_property*)property.get(), sval);
}
ELF_API void ELF_APIENTRY elfSetPropertyBool(elf_handle property, bool bval)
{
	if(!property.get() || elf_get_object_type(property.get()) != ELF_PROPERTY)
	{
		printf("SetPropertyBool() -> invalid handle\n");
		return;
	}
	elf_set_property_bool((elf_property*)property.get(), bval);
}
ELF_API const char* ELF_APIENTRY elfGetActorName(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("GetActorName() -> invalid handle\n");
		return "";
	}
	return elf_get_actor_name((elf_actor*)actor.get());
}
ELF_API const char* ELF_APIENTRY elfGetActorFilePath(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("GetActorFilePath() -> invalid handle\n");
		return "";
	}
	return elf_get_actor_file_path((elf_actor*)actor.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetActorParent(elf_handle actor)
{
	elf_handle handle;
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("GetActorParent() -> invalid handle\n");
		return handle;
	}
	handle = (elf_object*)elf_get_actor_parent((elf_actor*)actor.get());
	return handle;
}
ELF_API void ELF_APIENTRY elfSetActorParent(elf_handle actor, elf_handle parent)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("SetActorParent() -> invalid handle\n");
		return;
	}
	if(!parent.get() || !elf_is_actor(parent.get()))
	{
		printf("SetActorParent() -> invalid handle\n");
		return;
	}
	elf_set_actor_parent((elf_actor*)actor.get(), (elf_actor*)parent.get());
}
ELF_API int ELF_APIENTRY elfGetActorChildCount(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("GetActorChildCount() -> invalid handle\n");
		return 0;
	}
	return elf_get_actor_child_count((elf_actor*)actor.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetActorChildByName(elf_handle actor, const char* name)
{
	elf_handle handle;
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("GetActorChildByName() -> invalid handle\n");
		return handle;
	}
	handle = (elf_object*)elf_get_actor_child_by_name((elf_actor*)actor.get(), name);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetActorChildByIndex(elf_handle actor, int idx)
{
	elf_handle handle;
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("GetActorChildByIndex() -> invalid handle\n");
		return handle;
	}
	handle = (elf_object*)elf_get_actor_child_by_index((elf_actor*)actor.get(), idx);
	return handle;
}
ELF_API void ELF_APIENTRY elfRemoveActorChildren(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("RemoveActorChildren() -> invalid handle\n");
		return;
	}
	elf_remove_actor_children((elf_actor*)actor.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetActorScript(elf_handle actor)
{
	elf_handle handle;
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("GetActorScript() -> invalid handle\n");
		return handle;
	}
	handle = (elf_object*)elf_get_actor_script((elf_actor*)actor.get());
	return handle;
}
ELF_API void ELF_APIENTRY elfSetActorScript(elf_handle actor, elf_handle script)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("SetActorScript() -> invalid handle\n");
		return;
	}
	if(!script.get() || elf_get_object_type(script.get()) != ELF_SCRIPT)
	{
		printf("SetActorScript() -> invalid handle\n");
		return;
	}
	elf_set_actor_script((elf_actor*)actor.get(), (elf_script*)script.get());
}
ELF_API void ELF_APIENTRY elfSetActorPosition(elf_handle actor, float x, float y, float z)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("SetActorPosition() -> invalid handle\n");
		return;
	}
	elf_set_actor_position((elf_actor*)actor.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfSetActorRotation(elf_handle actor, float x, float y, float z)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("SetActorRotation() -> invalid handle\n");
		return;
	}
	elf_set_actor_rotation((elf_actor*)actor.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfSetActorOrientation(elf_handle actor, float x, float y, float z, float w)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("SetActorOrientation() -> invalid handle\n");
		return;
	}
	elf_set_actor_orientation((elf_actor*)actor.get(), x, y, z, w);
}
ELF_API void ELF_APIENTRY elfRotateActor(elf_handle actor, float x, float y, float z)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("RotateActor() -> invalid handle\n");
		return;
	}
	elf_rotate_actor((elf_actor*)actor.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfRotateActorLocal(elf_handle actor, float x, float y, float z)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("RotateActorLocal() -> invalid handle\n");
		return;
	}
	elf_rotate_actor_local((elf_actor*)actor.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfMoveActor(elf_handle actor, float x, float y, float z)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("MoveActor() -> invalid handle\n");
		return;
	}
	elf_move_actor((elf_actor*)actor.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfMoveActorLocal(elf_handle actor, float x, float y, float z)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("MoveActorLocal() -> invalid handle\n");
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
		printf("GetActorPosition() -> invalid handle\n");
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
		printf("GetActorRotation() -> invalid handle\n");
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
		printf("GetActorOrientation() -> invalid handle\n");
		return _e_type;
	}
	_e_type = elf_get_actor_orientation((elf_actor*)actor.get());
	return _e_type;
}
ELF_API void ELF_APIENTRY elfSetActorDamping(elf_handle actor, float lin_damp, float ang_damp)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("SetActorDamping() -> invalid handle\n");
		return;
	}
	elf_set_actor_damping((elf_actor*)actor.get(), lin_damp, ang_damp);
}
ELF_API void ELF_APIENTRY elfSetActorSleepThresholds(elf_handle actor, float lin_thrs, float ang_thrs)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("SetActorSleepThresholds() -> invalid handle\n");
		return;
	}
	elf_set_actor_sleep_thresholds((elf_actor*)actor.get(), lin_thrs, ang_thrs);
}
ELF_API void ELF_APIENTRY elfSetActorRestitution(elf_handle actor, float restitution)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("SetActorRestitution() -> invalid handle\n");
		return;
	}
	elf_set_actor_restitution((elf_actor*)actor.get(), restitution);
}
ELF_API void ELF_APIENTRY elfAddForceToActor(elf_handle actor, float x, float y, float z)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("AddForceToActor() -> invalid handle\n");
		return;
	}
	elf_add_force_to_actor((elf_actor*)actor.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfAddTorqueToActor(elf_handle actor, float x, float y, float z)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("AddTorqueToActor() -> invalid handle\n");
		return;
	}
	elf_add_torque_to_actor((elf_actor*)actor.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfSetActorLinearVelocity(elf_handle actor, float x, float y, float z)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("SetActorLinearVelocity() -> invalid handle\n");
		return;
	}
	elf_set_actor_linear_velocity((elf_actor*)actor.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfSetActorAngularVelocity(elf_handle actor, float x, float y, float z)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("SetActorAngularVelocity() -> invalid handle\n");
		return;
	}
	elf_set_actor_angular_velocity((elf_actor*)actor.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfSetActorLinearFactor(elf_handle actor, float x, float y, float z)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("SetActorLinearFactor() -> invalid handle\n");
		return;
	}
	elf_set_actor_linear_factor((elf_actor*)actor.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfSetActorAngularFactor(elf_handle actor, float x, float y, float z)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("SetActorAngularFactor() -> invalid handle\n");
		return;
	}
	elf_set_actor_angular_factor((elf_actor*)actor.get(), x, y, z);
}
ELF_API elf_vec3f ELF_APIENTRY elfGetActorLinearVelocity(elf_handle actor)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("GetActorLinearVelocity() -> invalid handle\n");
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
		printf("GetActorAngularVelocity() -> invalid handle\n");
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
		printf("GetActorLinearFactor() -> invalid handle\n");
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
		printf("GetActorAngularFactor() -> invalid handle\n");
		return _e_type;
	}
	_e_type = elf_get_actor_angular_factor((elf_actor*)actor.get());
	return _e_type;
}
ELF_API float ELF_APIENTRY elfGetActorLinearDamping(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("GetActorLinearDamping() -> invalid handle\n");
		return 0;
	}
	return elf_get_actor_linear_damping((elf_actor*)actor.get());
}
ELF_API float ELF_APIENTRY elfGetActorAngularDamping(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("GetActorAngularDamping() -> invalid handle\n");
		return 0;
	}
	return elf_get_actor_angular_damping((elf_actor*)actor.get());
}
ELF_API float ELF_APIENTRY elfGetActorLinearSleepThreshold(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("GetActorLinearSleepThreshold() -> invalid handle\n");
		return 0;
	}
	return elf_get_actor_linear_sleep_threshold((elf_actor*)actor.get());
}
ELF_API float ELF_APIENTRY elfGetActorAngularSleepThreshold(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("GetActorAngularSleepThreshold() -> invalid handle\n");
		return 0;
	}
	return elf_get_actor_angular_sleep_threshold((elf_actor*)actor.get());
}
ELF_API float ELF_APIENTRY elfGetActorRestitution(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("GetActorRestitution() -> invalid handle\n");
		return 0;
	}
	return elf_get_actor_restitution((elf_actor*)actor.get());
}
ELF_API elf_handle ELF_APIENTRY elfAddHingeJointToActor(elf_handle actor, elf_handle actor2, const char* name, float px, float py, float pz, float ax, float ay, float az)
{
	elf_handle handle;
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("AddHingeJointToActor() -> invalid handle\n");
		return handle;
	}
	if(!actor2.get() || !elf_is_actor(actor2.get()))
	{
		printf("AddHingeJointToActor() -> invalid handle\n");
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
		printf("AddBallJointToActor() -> invalid handle\n");
		return handle;
	}
	if(!actor2.get() || !elf_is_actor(actor2.get()))
	{
		printf("AddBallJointToActor() -> invalid handle\n");
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
		printf("AddConeTwistJointToActor() -> invalid handle\n");
		return handle;
	}
	if(!actor2.get() || !elf_is_actor(actor2.get()))
	{
		printf("AddConeTwistJointToActor() -> invalid handle\n");
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
		printf("GetActorJointByName() -> invalid handle\n");
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
		printf("GetActorJointByIndex() -> invalid handle\n");
		return handle;
	}
	handle = (elf_object*)elf_get_actor_joint_by_index((elf_actor*)actor.get(), idx);
	return handle;
}
ELF_API bool ELF_APIENTRY elfRemoveActorJointByName(elf_handle actor, const char* name)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("RemoveActorJointByName() -> invalid handle\n");
		return false;
	}
	return (bool)elf_remove_actor_joint_by_name((elf_actor*)actor.get(), name);
}
ELF_API bool ELF_APIENTRY elfRemoveActorJointByIndex(elf_handle actor, int idx)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("RemoveActorJointByIndex() -> invalid handle\n");
		return false;
	}
	return (bool)elf_remove_actor_joint_by_index((elf_actor*)actor.get(), idx);
}
ELF_API bool ELF_APIENTRY elfRemoveActorJointByObject(elf_handle actor, elf_handle joint)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("RemoveActorJointByObject() -> invalid handle\n");
		return false;
	}
	if(!joint.get() || elf_get_object_type(joint.get()) != ELF_JOINT)
	{
		printf("RemoveActorJointByObject() -> invalid handle\n");
		return false;
	}
	return (bool)elf_remove_actor_joint_by_object((elf_actor*)actor.get(), (elf_joint*)joint.get());
}
ELF_API void ELF_APIENTRY elfSetActorIpoFrame(elf_handle actor, float frame)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("SetActorIpoFrame() -> invalid handle\n");
		return;
	}
	elf_set_actor_ipo_frame((elf_actor*)actor.get(), frame);
}
ELF_API void ELF_APIENTRY elfSetActorHierarchyIpoFrame(elf_handle actor, float frame)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("SetActorHierarchyIpoFrame() -> invalid handle\n");
		return;
	}
	elf_set_actor_hierarchy_ipo_frame((elf_actor*)actor.get(), frame);
}
ELF_API void ELF_APIENTRY elfPlayActorIpo(elf_handle actor, float start, float end, float speed)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("PlayActorIpo() -> invalid handle\n");
		return;
	}
	elf_play_actor_ipo((elf_actor*)actor.get(), start, end, speed);
}
ELF_API void ELF_APIENTRY elfLoopActorIpo(elf_handle actor, float start, float end, float speed)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("LoopActorIpo() -> invalid handle\n");
		return;
	}
	elf_loop_actor_ipo((elf_actor*)actor.get(), start, end, speed);
}
ELF_API void ELF_APIENTRY elfStopActorIpo(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("StopActorIpo() -> invalid handle\n");
		return;
	}
	elf_stop_actor_ipo((elf_actor*)actor.get());
}
ELF_API void ELF_APIENTRY elfPauseActorIpo(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("PauseActorIpo() -> invalid handle\n");
		return;
	}
	elf_pause_actor_ipo((elf_actor*)actor.get());
}
ELF_API void ELF_APIENTRY elfResumeActorIpo(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("ResumeActorIpo() -> invalid handle\n");
		return;
	}
	elf_resume_actor_ipo((elf_actor*)actor.get());
}
ELF_API float ELF_APIENTRY elfGetActorIpoStart(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("GetActorIpoStart() -> invalid handle\n");
		return 0;
	}
	return elf_get_actor_ipo_start((elf_actor*)actor.get());
}
ELF_API float ELF_APIENTRY elfGetActorIpoEnd(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("GetActorIpoEnd() -> invalid handle\n");
		return 0;
	}
	return elf_get_actor_ipo_end((elf_actor*)actor.get());
}
ELF_API float ELF_APIENTRY elfGetActorIpoSpeed(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("GetActorIpoSpeed() -> invalid handle\n");
		return 0;
	}
	return elf_get_actor_ipo_speed((elf_actor*)actor.get());
}
ELF_API float ELF_APIENTRY elfGetActorIpoFrame(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("GetActorIpoFrame() -> invalid handle\n");
		return 0;
	}
	return elf_get_actor_ipo_frame((elf_actor*)actor.get());
}
ELF_API bool ELF_APIENTRY elfIsActorIpoPlaying(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("IsActorIpoPlaying() -> invalid handle\n");
		return false;
	}
	return (bool)elf_is_actor_ipo_playing((elf_actor*)actor.get());
}
ELF_API bool ELF_APIENTRY elfIsActorIpoPaused(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("IsActorIpoPaused() -> invalid handle\n");
		return false;
	}
	return (bool)elf_is_actor_ipo_paused((elf_actor*)actor.get());
}
ELF_API int ELF_APIENTRY elfGetActorCollisionCount(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("GetActorCollisionCount() -> invalid handle\n");
		return 0;
	}
	return elf_get_actor_collision_count((elf_actor*)actor.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetActorCollision(elf_handle actor, int idx)
{
	elf_handle handle;
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("GetActorCollision() -> invalid handle\n");
		return handle;
	}
	handle = (elf_object*)elf_get_actor_collision((elf_actor*)actor.get(), idx);
	return handle;
}
ELF_API int ELF_APIENTRY elfGetActorPropertyCount(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("GetActorPropertyCount() -> invalid handle\n");
		return 0;
	}
	return elf_get_actor_property_count((elf_actor*)actor.get());
}
ELF_API void ELF_APIENTRY elfAddPropertyToActor(elf_handle actor, elf_handle property)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("AddPropertyToActor() -> invalid handle\n");
		return;
	}
	if(!property.get() || elf_get_object_type(property.get()) != ELF_PROPERTY)
	{
		printf("AddPropertyToActor() -> invalid handle\n");
		return;
	}
	elf_add_property_to_actor((elf_actor*)actor.get(), (elf_property*)property.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetActorPropertyByName(elf_handle actor, const char* name)
{
	elf_handle handle;
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("GetActorPropertyByName() -> invalid handle\n");
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
		printf("GetActorPropertyByIndex() -> invalid handle\n");
		return handle;
	}
	handle = (elf_object*)elf_get_actor_property_by_index((elf_actor*)actor.get(), idx);
	return handle;
}
ELF_API bool ELF_APIENTRY elfRemoveActorPropertyByName(elf_handle actor, const char* name)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("RemoveActorPropertyByName() -> invalid handle\n");
		return false;
	}
	return (bool)elf_remove_actor_property_by_name((elf_actor*)actor.get(), name);
}
ELF_API bool ELF_APIENTRY elfRemoveActorPropertyByIndex(elf_handle actor, int idx)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("RemoveActorPropertyByIndex() -> invalid handle\n");
		return false;
	}
	return (bool)elf_remove_actor_property_by_index((elf_actor*)actor.get(), idx);
}
ELF_API bool ELF_APIENTRY elfRemoveActorPropertyByObject(elf_handle actor, elf_handle property)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("RemoveActorPropertyByObject() -> invalid handle\n");
		return false;
	}
	if(!property.get() || elf_get_object_type(property.get()) != ELF_PROPERTY)
	{
		printf("RemoveActorPropertyByObject() -> invalid handle\n");
		return false;
	}
	return (bool)elf_remove_actor_property_by_object((elf_actor*)actor.get(), (elf_property*)property.get());
}
ELF_API void ELF_APIENTRY elfRemoveActorProperties(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("RemoveActorProperties() -> invalid handle\n");
		return;
	}
	elf_remove_actor_properties((elf_actor*)actor.get());
}
ELF_API void ELF_APIENTRY elfSetActorSelected(elf_handle actor, bool selected)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("SetActorSelected() -> invalid handle\n");
		return;
	}
	elf_set_actor_selected((elf_actor*)actor.get(), selected);
}
ELF_API bool ELF_APIENTRY elfGetActorSelected(elf_handle actor)
{
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("GetActorSelected() -> invalid handle\n");
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
		printf("SetCameraViewport() -> invalid handle\n");
		return;
	}
	elf_set_camera_viewport((elf_camera*)camera.get(), x, y, width, height);
}
ELF_API void ELF_APIENTRY elfSetCameraPerspective(elf_handle camera, float fov, float aspect, float clip_near, float clip_far)
{
	if(!camera.get() || elf_get_object_type(camera.get()) != ELF_CAMERA)
	{
		printf("SetCameraPerspective() -> invalid handle\n");
		return;
	}
	elf_set_camera_perspective((elf_camera*)camera.get(), fov, aspect, clip_near, clip_far);
}
ELF_API void ELF_APIENTRY elfSetCameraOrthographic(elf_handle camera, int x, int y, int width, int height, float clip_near, float clip_far)
{
	if(!camera.get() || elf_get_object_type(camera.get()) != ELF_CAMERA)
	{
		printf("SetCameraOrthographic() -> invalid handle\n");
		return;
	}
	elf_set_camera_orthographic((elf_camera*)camera.get(), x, y, width, height, clip_near, clip_far);
}
ELF_API float ELF_APIENTRY elfGetCameraFov(elf_handle camera)
{
	if(!camera.get() || elf_get_object_type(camera.get()) != ELF_CAMERA)
	{
		printf("GetCameraFov() -> invalid handle\n");
		return 0;
	}
	return elf_get_camera_fov((elf_camera*)camera.get());
}
ELF_API float ELF_APIENTRY elfGetCameraAspect(elf_handle camera)
{
	if(!camera.get() || elf_get_object_type(camera.get()) != ELF_CAMERA)
	{
		printf("GetCameraAspect() -> invalid handle\n");
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
		printf("GetCameraClip() -> invalid handle\n");
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
		printf("GetCameraFarPlaneSize() -> invalid handle\n");
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
		printf("UnProjectCameraPoint() -> invalid handle\n");
		return _e_type;
	}
	_e_type = elf_un_project_camera_point((elf_camera*)camera.get(), x, y, z);
	return _e_type;
}
ELF_API const char* ELF_APIENTRY elfGetModelName(elf_handle model)
{
	if(!model.get() || elf_get_object_type(model.get()) != ELF_MODEL)
	{
		printf("GetModelName() -> invalid handle\n");
		return "";
	}
	return elf_get_model_name((elf_model*)model.get());
}
ELF_API const char* ELF_APIENTRY elfGetModelFilePath(elf_handle model)
{
	if(!model.get() || elf_get_object_type(model.get()) != ELF_MODEL)
	{
		printf("GetModelFilePath() -> invalid handle\n");
		return "";
	}
	return elf_get_model_file_path((elf_model*)model.get());
}
ELF_API int ELF_APIENTRY elfGetModelVerticeCount(elf_handle model)
{
	if(!model.get() || elf_get_object_type(model.get()) != ELF_MODEL)
	{
		printf("GetModelVerticeCount() -> invalid handle\n");
		return 0;
	}
	return elf_get_model_vertice_count((elf_model*)model.get());
}
ELF_API int ELF_APIENTRY elfGetModelIndiceCount(elf_handle model)
{
	if(!model.get() || elf_get_object_type(model.get()) != ELF_MODEL)
	{
		printf("GetModelIndiceCount() -> invalid handle\n");
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
		printf("GetModelBoundingBoxMin() -> invalid handle\n");
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
		printf("GetModelBoundingBoxMax() -> invalid handle\n");
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
		printf("SetEntityScale() -> invalid handle\n");
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
		printf("GetEntityScale() -> invalid handle\n");
		return _e_type;
	}
	_e_type = elf_get_entity_scale((elf_entity*)entity.get());
	return _e_type;
}
ELF_API void ELF_APIENTRY elfSetEntityModel(elf_handle entity, elf_handle model)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		printf("SetEntityModel() -> invalid handle\n");
		return;
	}
	if(!model.get() || elf_get_object_type(model.get()) != ELF_MODEL)
	{
		printf("SetEntityModel() -> invalid handle\n");
		return;
	}
	elf_set_entity_model((elf_entity*)entity.get(), (elf_model*)model.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetEntityModel(elf_handle entity)
{
	elf_handle handle;
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		printf("GetEntityModel() -> invalid handle\n");
		return handle;
	}
	handle = (elf_object*)elf_get_entity_model((elf_entity*)entity.get());
	return handle;
}
ELF_API int ELF_APIENTRY elfGetEntityMaterialCount(elf_handle entity)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		printf("GetEntityMaterialCount() -> invalid handle\n");
		return 0;
	}
	return elf_get_entity_material_count((elf_entity*)entity.get());
}
ELF_API void ELF_APIENTRY elfAddEntityMaterial(elf_handle entity, elf_handle material)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		printf("AddEntityMaterial() -> invalid handle\n");
		return;
	}
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		printf("AddEntityMaterial() -> invalid handle\n");
		return;
	}
	elf_add_entity_material((elf_entity*)entity.get(), (elf_material*)material.get());
}
ELF_API void ELF_APIENTRY elfSetEntityMaterial(elf_handle entity, int idx, elf_handle material)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		printf("SetEntityMaterial() -> invalid handle\n");
		return;
	}
	if(!material.get() || elf_get_object_type(material.get()) != ELF_MATERIAL)
	{
		printf("SetEntityMaterial() -> invalid handle\n");
		return;
	}
	elf_set_entity_material((elf_entity*)entity.get(), idx, (elf_material*)material.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetEntityMaterial(elf_handle entity, int idx)
{
	elf_handle handle;
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		printf("GetEntityMaterial() -> invalid handle\n");
		return handle;
	}
	handle = (elf_object*)elf_get_entity_material((elf_entity*)entity.get(), idx);
	return handle;
}
ELF_API void ELF_APIENTRY elfSetEntityVisible(elf_handle entity, bool visible)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		printf("SetEntityVisible() -> invalid handle\n");
		return;
	}
	elf_set_entity_visible((elf_entity*)entity.get(), visible);
}
ELF_API bool ELF_APIENTRY elfGetEntityVisible(elf_handle entity)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		printf("GetEntityVisible() -> invalid handle\n");
		return false;
	}
	return (bool)elf_get_entity_visible((elf_entity*)entity.get());
}
ELF_API void ELF_APIENTRY elfSetEntityPhysics(elf_handle entity, int type, float mass)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		printf("SetEntityPhysics() -> invalid handle\n");
		return;
	}
	elf_set_entity_physics((elf_entity*)entity.get(), type, mass);
}
ELF_API void ELF_APIENTRY elfDisableEntityPhysics(elf_handle entity)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		printf("DisableEntityPhysics() -> invalid handle\n");
		return;
	}
	elf_disable_entity_physics((elf_entity*)entity.get());
}
ELF_API void ELF_APIENTRY elfSetEntityArmature(elf_handle entity, elf_handle armature)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		printf("SetEntityArmature() -> invalid handle\n");
		return;
	}
	if(!armature.get() || elf_get_object_type(armature.get()) != ELF_ARMATURE)
	{
		printf("SetEntityArmature() -> invalid handle\n");
		return;
	}
	elf_set_entity_armature((elf_entity*)entity.get(), (elf_armature*)armature.get());
}
ELF_API void ELF_APIENTRY elfSetEntityArmatureFrame(elf_handle entity, float frame)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		printf("SetEntityArmatureFrame() -> invalid handle\n");
		return;
	}
	elf_set_entity_armature_frame((elf_entity*)entity.get(), frame);
}
ELF_API void ELF_APIENTRY elfPlayEntityArmature(elf_handle entity, float start, float end, float speed)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		printf("PlayEntityArmature() -> invalid handle\n");
		return;
	}
	elf_play_entity_armature((elf_entity*)entity.get(), start, end, speed);
}
ELF_API void ELF_APIENTRY elfLoopEntityArmature(elf_handle entity, float start, float end, float speed)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		printf("LoopEntityArmature() -> invalid handle\n");
		return;
	}
	elf_loop_entity_armature((elf_entity*)entity.get(), start, end, speed);
}
ELF_API void ELF_APIENTRY elfStopEntityArmature(elf_handle entity)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		printf("StopEntityArmature() -> invalid handle\n");
		return;
	}
	elf_stop_entity_armature((elf_entity*)entity.get());
}
ELF_API void ELF_APIENTRY elfPauseEntityArmature(elf_handle entity)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		printf("PauseEntityArmature() -> invalid handle\n");
		return;
	}
	elf_pause_entity_armature((elf_entity*)entity.get());
}
ELF_API void ELF_APIENTRY elfResumeEntityArmature(elf_handle entity)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		printf("ResumeEntityArmature() -> invalid handle\n");
		return;
	}
	elf_resume_entity_armature((elf_entity*)entity.get());
}
ELF_API float ELF_APIENTRY elfGetEntityArmatureStart(elf_handle entity)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		printf("GetEntityArmatureStart() -> invalid handle\n");
		return 0;
	}
	return elf_get_entity_armature_start((elf_entity*)entity.get());
}
ELF_API float ELF_APIENTRY elfGetEntityArmatureEnd(elf_handle entity)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		printf("GetEntityArmatureEnd() -> invalid handle\n");
		return 0;
	}
	return elf_get_entity_armature_end((elf_entity*)entity.get());
}
ELF_API float ELF_APIENTRY elfGetEntityArmatureSpeed(elf_handle entity)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		printf("GetEntityArmatureSpeed() -> invalid handle\n");
		return 0;
	}
	return elf_get_entity_armature_speed((elf_entity*)entity.get());
}
ELF_API float ELF_APIENTRY elfGetEntityArmatureFrame(elf_handle entity)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		printf("GetEntityArmatureFrame() -> invalid handle\n");
		return 0;
	}
	return elf_get_entity_armature_frame((elf_entity*)entity.get());
}
ELF_API bool ELF_APIENTRY elfIsEntityArmaturePlaying(elf_handle entity)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		printf("IsEntityArmaturePlaying() -> invalid handle\n");
		return false;
	}
	return (bool)elf_is_entity_armature_playing((elf_entity*)entity.get());
}
ELF_API bool ELF_APIENTRY elfIsEntityArmaturePaused(elf_handle entity)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		printf("IsEntityArmaturePaused() -> invalid handle\n");
		return false;
	}
	return (bool)elf_is_entity_armature_paused((elf_entity*)entity.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetEntityArmature(elf_handle entity)
{
	elf_handle handle;
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		printf("GetEntityArmature() -> invalid handle\n");
		return handle;
	}
	handle = (elf_object*)elf_get_entity_armature((elf_entity*)entity.get());
	return handle;
}
ELF_API bool ELF_APIENTRY elfGetEntityChanged(elf_handle entity)
{
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		printf("GetEntityChanged() -> invalid handle\n");
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
		printf("GetLightType() -> invalid handle\n");
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
		printf("GetLightColor() -> invalid handle\n");
		return _e_type;
	}
	_e_type = elf_get_light_color((elf_light*)light.get());
	return _e_type;
}
ELF_API float ELF_APIENTRY elfGetLightDistance(elf_handle light)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		printf("GetLightDistance() -> invalid handle\n");
		return 0;
	}
	return elf_get_light_distance((elf_light*)light.get());
}
ELF_API float ELF_APIENTRY elfGetLightFadeSpeed(elf_handle light)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		printf("GetLightFadeSpeed() -> invalid handle\n");
		return 0;
	}
	return elf_get_light_fade_speed((elf_light*)light.get());
}
ELF_API bool ELF_APIENTRY elfGetLightShadowCaster(elf_handle light)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		printf("GetLightShadowCaster() -> invalid handle\n");
		return false;
	}
	return (bool)elf_get_light_shadow_caster((elf_light*)light.get());
}
ELF_API bool ELF_APIENTRY elfGetLightVisible(elf_handle light)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		printf("GetLightVisible() -> invalid handle\n");
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
		printf("GetLightCone() -> invalid handle\n");
		return _e_type;
	}
	_e_type = elf_get_light_cone((elf_light*)light.get());
	return _e_type;
}
ELF_API bool ELF_APIENTRY elfIsLightShaft(elf_handle light)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		printf("IsLightShaft() -> invalid handle\n");
		return false;
	}
	return (bool)elf_is_light_shaft((elf_light*)light.get());
}
ELF_API float ELF_APIENTRY elfGetLightShaftSize(elf_handle light)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		printf("GetLightShaftSize() -> invalid handle\n");
		return 0;
	}
	return elf_get_light_shaft_size((elf_light*)light.get());
}
ELF_API float ELF_APIENTRY elfGetLightShaftIntensity(elf_handle light)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		printf("GetLightShaftIntensity() -> invalid handle\n");
		return 0;
	}
	return elf_get_light_shaft_intensity((elf_light*)light.get());
}
ELF_API float ELF_APIENTRY elfGetLightShaftFadeOff(elf_handle light)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		printf("GetLightShaftFadeOff() -> invalid handle\n");
		return 0;
	}
	return elf_get_light_shaft_fade_off((elf_light*)light.get());
}
ELF_API void ELF_APIENTRY elfSetLightType(elf_handle light, int type)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		printf("SetLightType() -> invalid handle\n");
		return;
	}
	elf_set_light_type((elf_light*)light.get(), type);
}
ELF_API void ELF_APIENTRY elfSetLightColor(elf_handle light, float r, float g, float b, float a)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		printf("SetLightColor() -> invalid handle\n");
		return;
	}
	elf_set_light_color((elf_light*)light.get(), r, g, b, a);
}
ELF_API void ELF_APIENTRY elfSetLightDistance(elf_handle light, float distance)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		printf("SetLightDistance() -> invalid handle\n");
		return;
	}
	elf_set_light_distance((elf_light*)light.get(), distance);
}
ELF_API void ELF_APIENTRY elfSetLightFadeSpeed(elf_handle light, float fade_speed)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		printf("SetLightFadeSpeed() -> invalid handle\n");
		return;
	}
	elf_set_light_fade_speed((elf_light*)light.get(), fade_speed);
}
ELF_API void ELF_APIENTRY elfSetLightShadowCaster(elf_handle light, bool shadow_caster)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		printf("SetLightShadowCaster() -> invalid handle\n");
		return;
	}
	elf_set_light_shadow_caster((elf_light*)light.get(), shadow_caster);
}
ELF_API void ELF_APIENTRY elfSetLightVisible(elf_handle light, bool visible)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		printf("SetLightVisible() -> invalid handle\n");
		return;
	}
	elf_set_light_visible((elf_light*)light.get(), visible);
}
ELF_API void ELF_APIENTRY elfSetLightCone(elf_handle light, float inner_cone, float outer_cone)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		printf("SetLightCone() -> invalid handle\n");
		return;
	}
	elf_set_light_cone((elf_light*)light.get(), inner_cone, outer_cone);
}
ELF_API void ELF_APIENTRY elfSetLightShaft(elf_handle light, float size, float intensity, float fade_off)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		printf("SetLightShaft() -> invalid handle\n");
		return;
	}
	elf_set_light_shaft((elf_light*)light.get(), size, intensity, fade_off);
}
ELF_API void ELF_APIENTRY elfDisableLightShaft(elf_handle light)
{
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		printf("DisableLightShaft() -> invalid handle\n");
		return;
	}
	elf_disable_light_shaft((elf_light*)light.get());
}
ELF_API elf_handle ELF_APIENTRY elfCreateBone(const char* name)
{
	elf_handle handle;
	handle = (elf_object*)elf_create_bone(name);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetBoneArmature(elf_handle bone)
{
	elf_handle handle;
	if(!bone.get() || elf_get_object_type(bone.get()) != ELF_BONE)
	{
		printf("GetBoneArmature() -> invalid handle\n");
		return handle;
	}
	handle = (elf_object*)elf_get_bone_armature((elf_bone*)bone.get());
	return handle;
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
		printf("GetBoneFromArmatureByName() -> invalid handle\n");
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
		printf("GetBoneFromArmatureById() -> invalid handle\n");
		return handle;
	}
	handle = (elf_object*)elf_get_bone_from_armature_by_id(id, (elf_armature*)armature.get());
	return handle;
}
ELF_API void ELF_APIENTRY elfAddRootBoneToArmature(elf_handle armature, elf_handle bone)
{
	if(!armature.get() || elf_get_object_type(armature.get()) != ELF_ARMATURE)
	{
		printf("AddRootBoneToArmature() -> invalid handle\n");
		return;
	}
	if(!bone.get() || elf_get_object_type(bone.get()) != ELF_BONE)
	{
		printf("AddRootBoneToArmature() -> invalid handle\n");
		return;
	}
	elf_add_root_bone_to_armature((elf_armature*)armature.get(), (elf_bone*)bone.get());
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
		printf("GetParticlesName() -> invalid handle\n");
		return "";
	}
	return elf_get_particles_name((elf_particles*)particles.get());
}
ELF_API const char* ELF_APIENTRY elfGetParticlesFilePath(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("GetParticlesFilePath() -> invalid handle\n");
		return "";
	}
	return elf_get_particles_file_path((elf_particles*)particles.get());
}
ELF_API void ELF_APIENTRY elfSetParticlesMaxCount(elf_handle particles, int max_count)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("SetParticlesMaxCount() -> invalid handle\n");
		return;
	}
	elf_set_particles_max_count((elf_particles*)particles.get(), max_count);
}
ELF_API void ELF_APIENTRY elfSetParticlesDrawMode(elf_handle particles, int mode)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("SetParticlesDrawMode() -> invalid handle\n");
		return;
	}
	elf_set_particles_draw_mode((elf_particles*)particles.get(), mode);
}
ELF_API void ELF_APIENTRY elfSetParticlesTexture(elf_handle particles, elf_handle texture)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("SetParticlesTexture() -> invalid handle\n");
		return;
	}
	if(!texture.get() || elf_get_object_type(texture.get()) != ELF_TEXTURE)
	{
		printf("SetParticlesTexture() -> invalid handle\n");
		return;
	}
	elf_set_particles_texture((elf_particles*)particles.get(), (elf_texture*)texture.get());
}
ELF_API void ELF_APIENTRY elfSetParticlesModel(elf_handle particles, elf_handle model)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("SetParticlesModel() -> invalid handle\n");
		return;
	}
	if(!model.get() || elf_get_object_type(model.get()) != ELF_MODEL)
	{
		printf("SetParticlesModel() -> invalid handle\n");
		return;
	}
	elf_set_particles_model((elf_particles*)particles.get(), (elf_model*)model.get());
}
ELF_API void ELF_APIENTRY elfSetParticlesGravity(elf_handle particles, float x, float y, float z)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("SetParticlesGravity() -> invalid handle\n");
		return;
	}
	elf_set_particles_gravity((elf_particles*)particles.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfSetParticlesSpawnDelay(elf_handle particles, float delay)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("SetParticlesSpawnDelay() -> invalid handle\n");
		return;
	}
	elf_set_particles_spawn_delay((elf_particles*)particles.get(), delay);
}
ELF_API void ELF_APIENTRY elfSetParticlesSize(elf_handle particles, float min, float max)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("SetParticlesSize() -> invalid handle\n");
		return;
	}
	elf_set_particles_size((elf_particles*)particles.get(), min, max);
}
ELF_API void ELF_APIENTRY elfSetParticlesSizeGrowth(elf_handle particles, float min, float max)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("SetParticlesSizeGrowth() -> invalid handle\n");
		return;
	}
	elf_set_particles_size_growth((elf_particles*)particles.get(), min, max);
}
ELF_API void ELF_APIENTRY elfSetParticlesRotation(elf_handle particles, float min, float max)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("SetParticlesRotation() -> invalid handle\n");
		return;
	}
	elf_set_particles_rotation((elf_particles*)particles.get(), min, max);
}
ELF_API void ELF_APIENTRY elfSetParticlesRotationGrowth(elf_handle particles, float min, float max)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("SetParticlesRotationGrowth() -> invalid handle\n");
		return;
	}
	elf_set_particles_rotation_growth((elf_particles*)particles.get(), min, max);
}
ELF_API void ELF_APIENTRY elfSetParticlesLifeSpan(elf_handle particles, float min, float max)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("SetParticlesLifeSpan() -> invalid handle\n");
		return;
	}
	elf_set_particles_life_span((elf_particles*)particles.get(), min, max);
}
ELF_API void ELF_APIENTRY elfSetParticlesFadeSpeed(elf_handle particles, float min, float max)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("SetParticlesFadeSpeed() -> invalid handle\n");
		return;
	}
	elf_set_particles_fade_speed((elf_particles*)particles.get(), min, max);
}
ELF_API void ELF_APIENTRY elfSetParticlesPositionMin(elf_handle particles, float x, float y, float z)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("SetParticlesPositionMin() -> invalid handle\n");
		return;
	}
	elf_set_particles_position_min((elf_particles*)particles.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfSetParticlesPositionMax(elf_handle particles, float x, float y, float z)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("SetParticlesPositionMax() -> invalid handle\n");
		return;
	}
	elf_set_particles_position_max((elf_particles*)particles.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfSetParticlesVelocityMin(elf_handle particles, float x, float y, float z)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("SetParticlesVelocityMin() -> invalid handle\n");
		return;
	}
	elf_set_particles_velocity_min((elf_particles*)particles.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfSetParticlesVelocityMax(elf_handle particles, float x, float y, float z)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("SetParticlesVelocityMax() -> invalid handle\n");
		return;
	}
	elf_set_particles_velocity_max((elf_particles*)particles.get(), x, y, z);
}
ELF_API void ELF_APIENTRY elfSetParticlesColorMin(elf_handle particles, float r, float g, float b, float a)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("SetParticlesColorMin() -> invalid handle\n");
		return;
	}
	elf_set_particles_color_min((elf_particles*)particles.get(), r, g, b, a);
}
ELF_API void ELF_APIENTRY elfSetParticlesColorMax(elf_handle particles, float r, float g, float b, float a)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("SetParticlesColorMax() -> invalid handle\n");
		return;
	}
	elf_set_particles_color_max((elf_particles*)particles.get(), r, g, b, a);
}
ELF_API int ELF_APIENTRY elfGetParticlesMaxCount(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("GetParticlesMaxCount() -> invalid handle\n");
		return 0;
	}
	return elf_get_particles_max_count((elf_particles*)particles.get());
}
ELF_API int ELF_APIENTRY elfGetParticlesCount(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("GetParticlesCount() -> invalid handle\n");
		return 0;
	}
	return elf_get_particles_count((elf_particles*)particles.get());
}
ELF_API int ELF_APIENTRY elfGetParticlesDrawMode(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("GetParticlesDrawMode() -> invalid handle\n");
		return 0;
	}
	return elf_get_particles_draw_mode((elf_particles*)particles.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetParticlesTexture(elf_handle particles)
{
	elf_handle handle;
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("GetParticlesTexture() -> invalid handle\n");
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
		printf("GetParticlesModel() -> invalid handle\n");
		return handle;
	}
	handle = (elf_object*)elf_get_particles_model((elf_particles*)particles.get());
	return handle;
}
ELF_API elf_vec3f ELF_APIENTRY elfGetParticlesGravity(elf_handle particles)
{
	elf_vec3f _e_type;
	memset(&_e_type, 0x0, sizeof(elf_vec3f));
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("GetParticlesGravity() -> invalid handle\n");
		return _e_type;
	}
	_e_type = elf_get_particles_gravity((elf_particles*)particles.get());
	return _e_type;
}
ELF_API float ELF_APIENTRY elfGetParticlesSpawnDelay(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("GetParticlesSpawnDelay() -> invalid handle\n");
		return 0;
	}
	return elf_get_particles_spawn_delay((elf_particles*)particles.get());
}
ELF_API float ELF_APIENTRY elfGetParticlesSizeMin(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("GetParticlesSizeMin() -> invalid handle\n");
		return 0;
	}
	return elf_get_particles_size_min((elf_particles*)particles.get());
}
ELF_API float ELF_APIENTRY elfGetParticlesSizeMax(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("GetParticlesSizeMax() -> invalid handle\n");
		return 0;
	}
	return elf_get_particles_size_max((elf_particles*)particles.get());
}
ELF_API float ELF_APIENTRY elfGetParticlesSizeGrowthMin(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("GetParticlesSizeGrowthMin() -> invalid handle\n");
		return 0;
	}
	return elf_get_particles_size_growth_min((elf_particles*)particles.get());
}
ELF_API float ELF_APIENTRY elfGetParticlesSizeGrowthMax(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("GetParticlesSizeGrowthMax() -> invalid handle\n");
		return 0;
	}
	return elf_get_particles_size_growth_max((elf_particles*)particles.get());
}
ELF_API float ELF_APIENTRY elfGetParticlesRotationMin(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("GetParticlesRotationMin() -> invalid handle\n");
		return 0;
	}
	return elf_get_particles_rotation_min((elf_particles*)particles.get());
}
ELF_API float ELF_APIENTRY elfGetParticlesRotationMax(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("GetParticlesRotationMax() -> invalid handle\n");
		return 0;
	}
	return elf_get_particles_rotation_max((elf_particles*)particles.get());
}
ELF_API float ELF_APIENTRY elfGetParticlesRotationGrowthMin(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("GetParticlesRotationGrowthMin() -> invalid handle\n");
		return 0;
	}
	return elf_get_particles_rotation_growth_min((elf_particles*)particles.get());
}
ELF_API float ELF_APIENTRY elfGetParticlesRotationGrowthMax(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("GetParticlesRotationGrowthMax() -> invalid handle\n");
		return 0;
	}
	return elf_get_particles_rotation_growth_max((elf_particles*)particles.get());
}
ELF_API float ELF_APIENTRY elfGetParticlesLifeSpanMin(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("GetParticlesLifeSpanMin() -> invalid handle\n");
		return 0;
	}
	return elf_get_particles_life_span_min((elf_particles*)particles.get());
}
ELF_API float ELF_APIENTRY elfGetParticlesLifeSpanMax(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("GetParticlesLifeSpanMax() -> invalid handle\n");
		return 0;
	}
	return elf_get_particles_life_span_max((elf_particles*)particles.get());
}
ELF_API float ELF_APIENTRY elfGetParticlesFadeSpeedMin(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("GetParticlesFadeSpeedMin() -> invalid handle\n");
		return 0;
	}
	return elf_get_particles_fade_speed_min((elf_particles*)particles.get());
}
ELF_API float ELF_APIENTRY elfGetParticlesFadeSpeedMax(elf_handle particles)
{
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("GetParticlesFadeSpeedMax() -> invalid handle\n");
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
		printf("GetParticlesPositionMin() -> invalid handle\n");
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
		printf("GetParticlesPositionMax() -> invalid handle\n");
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
		printf("GetParticlesVelocityMin() -> invalid handle\n");
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
		printf("GetParticlesVelocityMax() -> invalid handle\n");
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
		printf("GetParticlesColorMin() -> invalid handle\n");
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
		printf("GetParticlesColorMax() -> invalid handle\n");
		return _e_type;
	}
	_e_type = elf_get_particles_color_max((elf_particles*)particles.get());
	return _e_type;
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
		printf("SetSceneAmbientColor() -> invalid handle\n");
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
		printf("GetSceneAmbientColor() -> invalid handle\n");
		return _e_type;
	}
	_e_type = elf_get_scene_ambient_color((elf_scene*)scene.get());
	return _e_type;
}
ELF_API void ELF_APIENTRY elfSetSceneGravity(elf_handle scene, float x, float y, float z)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		printf("SetSceneGravity() -> invalid handle\n");
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
		printf("GetSceneGravity() -> invalid handle\n");
		return _e_type;
	}
	_e_type = elf_get_scene_gravity((elf_scene*)scene.get());
	return _e_type;
}
ELF_API const char* ELF_APIENTRY elfGetSceneName(elf_handle scene)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		printf("GetSceneName() -> invalid handle\n");
		return "";
	}
	return elf_get_scene_name((elf_scene*)scene.get());
}
ELF_API const char* ELF_APIENTRY elfGetSceneFilePath(elf_handle scene)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		printf("GetSceneFilePath() -> invalid handle\n");
		return "";
	}
	return elf_get_scene_file_path((elf_scene*)scene.get());
}
ELF_API int ELF_APIENTRY elfGetSceneCameraCount(elf_handle scene)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		printf("GetSceneCameraCount() -> invalid handle\n");
		return 0;
	}
	return elf_get_scene_camera_count((elf_scene*)scene.get());
}
ELF_API int ELF_APIENTRY elfGetSceneEntityCount(elf_handle scene)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		printf("GetSceneEntityCount() -> invalid handle\n");
		return 0;
	}
	return elf_get_scene_entity_count((elf_scene*)scene.get());
}
ELF_API int ELF_APIENTRY elfGetSceneLightCount(elf_handle scene)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		printf("GetSceneLightCount() -> invalid handle\n");
		return 0;
	}
	return elf_get_scene_light_count((elf_scene*)scene.get());
}
ELF_API int ELF_APIENTRY elfGetSceneArmatureCount(elf_handle scene)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		printf("GetSceneArmatureCount() -> invalid handle\n");
		return 0;
	}
	return elf_get_scene_armature_count((elf_scene*)scene.get());
}
ELF_API int ELF_APIENTRY elfGetSceneParticlesCount(elf_handle scene)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		printf("GetSceneParticlesCount() -> invalid handle\n");
		return 0;
	}
	return elf_get_scene_particles_count((elf_scene*)scene.get());
}
ELF_API void ELF_APIENTRY elfAddCameraToScene(elf_handle scene, elf_handle camera)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		printf("AddCameraToScene() -> invalid handle\n");
		return;
	}
	if(!camera.get() || elf_get_object_type(camera.get()) != ELF_CAMERA)
	{
		printf("AddCameraToScene() -> invalid handle\n");
		return;
	}
	elf_add_camera_to_scene((elf_scene*)scene.get(), (elf_camera*)camera.get());
}
ELF_API void ELF_APIENTRY elfAddEntityToScene(elf_handle scene, elf_handle entity)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		printf("AddEntityToScene() -> invalid handle\n");
		return;
	}
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		printf("AddEntityToScene() -> invalid handle\n");
		return;
	}
	elf_add_entity_to_scene((elf_scene*)scene.get(), (elf_entity*)entity.get());
}
ELF_API void ELF_APIENTRY elfAddLightToScene(elf_handle scene, elf_handle light)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		printf("AddLightToScene() -> invalid handle\n");
		return;
	}
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		printf("AddLightToScene() -> invalid handle\n");
		return;
	}
	elf_add_light_to_scene((elf_scene*)scene.get(), (elf_light*)light.get());
}
ELF_API void ELF_APIENTRY elfAddParticlesToScene(elf_handle scene, elf_handle particles)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		printf("AddParticlesToScene() -> invalid handle\n");
		return;
	}
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("AddParticlesToScene() -> invalid handle\n");
		return;
	}
	elf_add_particles_to_scene((elf_scene*)scene.get(), (elf_particles*)particles.get());
}
ELF_API void ELF_APIENTRY elfSetSceneActiveCamera(elf_handle scene, elf_handle camera)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		printf("SetSceneActiveCamera() -> invalid handle\n");
		return;
	}
	if(!camera.get() || elf_get_object_type(camera.get()) != ELF_CAMERA)
	{
		printf("SetSceneActiveCamera() -> invalid handle\n");
		return;
	}
	elf_set_scene_active_camera((elf_scene*)scene.get(), (elf_camera*)camera.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetSceneActiveCamera(elf_handle scene)
{
	elf_handle handle;
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		printf("GetSceneActiveCamera() -> invalid handle\n");
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
		printf("GetSceneRayCastResult() -> invalid handle\n");
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
		printf("GetSceneRayCastResults() -> invalid handle\n");
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
		printf("GetDebugSceneRayCastResult() -> invalid handle\n");
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
		printf("GetDebugSceneRayCastResults() -> invalid handle\n");
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
		printf("GetCameraByIndex() -> invalid handle\n");
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
		printf("GetEntityByIndex() -> invalid handle\n");
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
		printf("GetLightByIndex() -> invalid handle\n");
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
		printf("GetArmatureByIndex() -> invalid handle\n");
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
		printf("GetParticlesByIndex() -> invalid handle\n");
		return handle;
	}
	handle = (elf_object*)elf_get_particles_by_index((elf_scene*)scene.get(), idx);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetTextureByName(elf_handle scene, const char* name)
{
	elf_handle handle;
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		printf("GetTextureByName() -> invalid handle\n");
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
		printf("GetMaterialByName() -> invalid handle\n");
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
		printf("GetModelByName() -> invalid handle\n");
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
		printf("GetScriptByName() -> invalid handle\n");
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
		printf("GetCameraByName() -> invalid handle\n");
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
		printf("GetEntityByName() -> invalid handle\n");
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
		printf("GetLightByName() -> invalid handle\n");
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
		printf("GetArmatureByName() -> invalid handle\n");
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
		printf("GetParticlesByName() -> invalid handle\n");
		return handle;
	}
	handle = (elf_object*)elf_get_particles_by_name((elf_scene*)scene.get(), name);
	return handle;
}
ELF_API elf_handle ELF_APIENTRY elfGetActorByName(elf_handle scene, const char* name)
{
	elf_handle handle;
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		printf("GetActorByName() -> invalid handle\n");
		return handle;
	}
	handle = (elf_object*)elf_get_actor_by_name((elf_scene*)scene.get(), name);
	return handle;
}
ELF_API bool ELF_APIENTRY elfRemoveCameraByName(elf_handle scene, const char* name)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		printf("RemoveCameraByName() -> invalid handle\n");
		return false;
	}
	return (bool)elf_remove_camera_by_name((elf_scene*)scene.get(), name);
}
ELF_API bool ELF_APIENTRY elfRemoveEntityByName(elf_handle scene, const char* name)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		printf("RemoveEntityByName() -> invalid handle\n");
		return false;
	}
	return (bool)elf_remove_entity_by_name((elf_scene*)scene.get(), name);
}
ELF_API bool ELF_APIENTRY elfRemoveLightByName(elf_handle scene, const char* name)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		printf("RemoveLightByName() -> invalid handle\n");
		return false;
	}
	return (bool)elf_remove_light_by_name((elf_scene*)scene.get(), name);
}
ELF_API bool ELF_APIENTRY elfRemoveParticlesByName(elf_handle scene, const char* name)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		printf("RemoveParticlesByName() -> invalid handle\n");
		return false;
	}
	return (bool)elf_remove_particles_by_name((elf_scene*)scene.get(), name);
}
ELF_API bool ELF_APIENTRY elfRemoveCameraByIndex(elf_handle scene, int idx)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		printf("RemoveCameraByIndex() -> invalid handle\n");
		return false;
	}
	return (bool)elf_remove_camera_by_index((elf_scene*)scene.get(), idx);
}
ELF_API bool ELF_APIENTRY elfRemoveEntityByIndex(elf_handle scene, int idx)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		printf("RemoveEntityByIndex() -> invalid handle\n");
		return false;
	}
	return (bool)elf_remove_entity_by_index((elf_scene*)scene.get(), idx);
}
ELF_API bool ELF_APIENTRY elfRemoveLightByIndex(elf_handle scene, int idx)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		printf("RemoveLightByIndex() -> invalid handle\n");
		return false;
	}
	return (bool)elf_remove_light_by_index((elf_scene*)scene.get(), idx);
}
ELF_API bool ELF_APIENTRY elfRemoveParticlesByIndex(elf_handle scene, int idx)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		printf("RemoveParticlesByIndex() -> invalid handle\n");
		return false;
	}
	return (bool)elf_remove_particles_by_index((elf_scene*)scene.get(), idx);
}
ELF_API bool ELF_APIENTRY elfRemoveCameraByObject(elf_handle scene, elf_handle camera)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		printf("RemoveCameraByObject() -> invalid handle\n");
		return false;
	}
	if(!camera.get() || elf_get_object_type(camera.get()) != ELF_CAMERA)
	{
		printf("RemoveCameraByObject() -> invalid handle\n");
		return false;
	}
	return (bool)elf_remove_camera_by_object((elf_scene*)scene.get(), (elf_camera*)camera.get());
}
ELF_API bool ELF_APIENTRY elfRemoveEntityByObject(elf_handle scene, elf_handle entity)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		printf("RemoveEntityByObject() -> invalid handle\n");
		return false;
	}
	if(!entity.get() || elf_get_object_type(entity.get()) != ELF_ENTITY)
	{
		printf("RemoveEntityByObject() -> invalid handle\n");
		return false;
	}
	return (bool)elf_remove_entity_by_object((elf_scene*)scene.get(), (elf_entity*)entity.get());
}
ELF_API bool ELF_APIENTRY elfRemoveLightByObject(elf_handle scene, elf_handle light)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		printf("RemoveLightByObject() -> invalid handle\n");
		return false;
	}
	if(!light.get() || elf_get_object_type(light.get()) != ELF_LIGHT)
	{
		printf("RemoveLightByObject() -> invalid handle\n");
		return false;
	}
	return (bool)elf_remove_light_by_object((elf_scene*)scene.get(), (elf_light*)light.get());
}
ELF_API bool ELF_APIENTRY elfRemoveParticlesByObject(elf_handle scene, elf_handle particles)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		printf("RemoveParticlesByObject() -> invalid handle\n");
		return false;
	}
	if(!particles.get() || elf_get_object_type(particles.get()) != ELF_PARTICLES)
	{
		printf("RemoveParticlesByObject() -> invalid handle\n");
		return false;
	}
	return (bool)elf_remove_particles_by_object((elf_scene*)scene.get(), (elf_particles*)particles.get());
}
ELF_API bool ELF_APIENTRY elfRemoveActorByObject(elf_handle scene, elf_handle actor)
{
	if(!scene.get() || elf_get_object_type(scene.get()) != ELF_SCENE)
	{
		printf("RemoveActorByObject() -> invalid handle\n");
		return false;
	}
	if(!actor.get() || !elf_is_actor(actor.get()))
	{
		printf("RemoveActorByObject() -> invalid handle\n");
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
		printf("GetScriptName() -> invalid handle\n");
		return "";
	}
	return elf_get_script_name((elf_script*)script.get());
}
ELF_API const char* ELF_APIENTRY elfGetScriptFilePath(elf_handle script)
{
	if(!script.get() || elf_get_object_type(script.get()) != ELF_SCRIPT)
	{
		printf("GetScriptFilePath() -> invalid handle\n");
		return "";
	}
	return elf_get_script_file_path((elf_script*)script.get());
}
ELF_API void ELF_APIENTRY elfSetScriptText(elf_handle script, const char* text)
{
	if(!script.get() || elf_get_object_type(script.get()) != ELF_SCRIPT)
	{
		printf("SetScriptText() -> invalid handle\n");
		return;
	}
	elf_set_script_text((elf_script*)script.get(), text);
}
ELF_API void ELF_APIENTRY elfRunScript(elf_handle script)
{
	if(!script.get() || elf_get_object_type(script.get()) != ELF_SCRIPT)
	{
		printf("RunScript() -> invalid handle\n");
		return;
	}
	elf_run_script((elf_script*)script.get());
}
ELF_API void ELF_APIENTRY elfRunString(const char* str)
{
	elf_run_string(str);
}
ELF_API bool ELF_APIENTRY elfIsScriptError(elf_handle script)
{
	if(!script.get() || elf_get_object_type(script.get()) != ELF_SCRIPT)
	{
		printf("IsScriptError() -> invalid handle\n");
		return false;
	}
	return (bool)elf_is_script_error((elf_script*)script.get());
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
		printf("GetSoundFileType() -> invalid handle\n");
		return 0;
	}
	return elf_get_sound_file_type((elf_sound*)sound.get());
}
ELF_API elf_handle ELF_APIENTRY elfPlaySound(elf_handle sound, float volume)
{
	elf_handle handle;
	if(!sound.get() || elf_get_object_type(sound.get()) != ELF_SOUND)
	{
		printf("PlaySound() -> invalid handle\n");
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
		printf("PlayEntitySound() -> invalid handle\n");
		return handle;
	}
	if(!sound.get() || elf_get_object_type(sound.get()) != ELF_SOUND)
	{
		printf("PlayEntitySound() -> invalid handle\n");
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
		printf("LoopSound() -> invalid handle\n");
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
		printf("LoopEntitySound() -> invalid handle\n");
		return handle;
	}
	if(!sound.get() || elf_get_object_type(sound.get()) != ELF_SOUND)
	{
		printf("LoopEntitySound() -> invalid handle\n");
		return handle;
	}
	handle = (elf_object*)elf_loop_entity_sound((elf_entity*)entity.get(), (elf_sound*)sound.get(), volume);
	return handle;
}
ELF_API void ELF_APIENTRY elfSetSoundVolume(elf_handle source, float volume)
{
	if(!source.get() || elf_get_object_type(source.get()) != ELF_AUDIO_SOURCE)
	{
		printf("SetSoundVolume() -> invalid handle\n");
		return;
	}
	elf_set_sound_volume((elf_audio_source*)source.get(), volume);
}
ELF_API float ELF_APIENTRY elfGetSoundVolume(elf_handle source)
{
	if(!source.get() || elf_get_object_type(source.get()) != ELF_AUDIO_SOURCE)
	{
		printf("GetSoundVolume() -> invalid handle\n");
		return 0;
	}
	return elf_get_sound_volume((elf_audio_source*)source.get());
}
ELF_API void ELF_APIENTRY elfPauseSound(elf_handle source)
{
	if(!source.get() || elf_get_object_type(source.get()) != ELF_AUDIO_SOURCE)
	{
		printf("PauseSound() -> invalid handle\n");
		return;
	}
	elf_pause_sound((elf_audio_source*)source.get());
}
ELF_API void ELF_APIENTRY elfResumeSound(elf_handle source)
{
	if(!source.get() || elf_get_object_type(source.get()) != ELF_AUDIO_SOURCE)
	{
		printf("ResumeSound() -> invalid handle\n");
		return;
	}
	elf_resume_sound((elf_audio_source*)source.get());
}
ELF_API void ELF_APIENTRY elfStopSound(elf_handle source)
{
	if(!source.get() || elf_get_object_type(source.get()) != ELF_AUDIO_SOURCE)
	{
		printf("StopSound() -> invalid handle\n");
		return;
	}
	elf_stop_sound((elf_audio_source*)source.get());
}
ELF_API bool ELF_APIENTRY elfIsSoundPlaying(elf_handle source)
{
	if(!source.get() || elf_get_object_type(source.get()) != ELF_AUDIO_SOURCE)
	{
		printf("IsSoundPlaying() -> invalid handle\n");
		return false;
	}
	return (bool)elf_is_sound_playing((elf_audio_source*)source.get());
}
ELF_API bool ELF_APIENTRY elfIsSoundPaused(elf_handle source)
{
	if(!source.get() || elf_get_object_type(source.get()) != ELF_AUDIO_SOURCE)
	{
		printf("IsSoundPaused() -> invalid handle\n");
		return false;
	}
	return (bool)elf_is_sound_paused((elf_audio_source*)source.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetCollisionActor(elf_handle collision)
{
	elf_handle handle;
	if(!collision.get() || elf_get_object_type(collision.get()) != ELF_COLLISION)
	{
		printf("GetCollisionActor() -> invalid handle\n");
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
		printf("GetCollisionPosition() -> invalid handle\n");
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
		printf("GetCollisionNormal() -> invalid handle\n");
		return _e_type;
	}
	_e_type = elf_get_collision_normal((elf_collision*)collision.get());
	return _e_type;
}
ELF_API float ELF_APIENTRY elfGetCollisionDepth(elf_handle collision)
{
	if(!collision.get() || elf_get_object_type(collision.get()) != ELF_COLLISION)
	{
		printf("GetCollisionDepth() -> invalid handle\n");
		return 0;
	}
	return elf_get_collision_depth((elf_collision*)collision.get());
}
ELF_API const char* ELF_APIENTRY elfGetJointName(elf_handle joint)
{
	if(!joint.get() || elf_get_object_type(joint.get()) != ELF_JOINT)
	{
		printf("GetJointName() -> invalid handle\n");
		return "";
	}
	return elf_get_joint_name((elf_joint*)joint.get());
}
ELF_API int ELF_APIENTRY elfGetJointType(elf_handle joint)
{
	if(!joint.get() || elf_get_object_type(joint.get()) != ELF_JOINT)
	{
		printf("GetJointType() -> invalid handle\n");
		return 0;
	}
	return elf_get_joint_type((elf_joint*)joint.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetJointActorA(elf_handle joint)
{
	elf_handle handle;
	if(!joint.get() || elf_get_object_type(joint.get()) != ELF_JOINT)
	{
		printf("GetJointActorA() -> invalid handle\n");
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
		printf("GetJointActorB() -> invalid handle\n");
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
		printf("GetJointPivot() -> invalid handle\n");
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
		printf("GetJointAxis() -> invalid handle\n");
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
		printf("GetFontName() -> invalid handle\n");
		return "";
	}
	return elf_get_font_name((elf_font*)font.get());
}
ELF_API const char* ELF_APIENTRY elfGetFontFilePath(elf_handle font)
{
	if(!font.get() || elf_get_object_type(font.get()) != ELF_FONT)
	{
		printf("GetFontFilePath() -> invalid handle\n");
		return "";
	}
	return elf_get_font_file_path((elf_font*)font.get());
}
ELF_API int ELF_APIENTRY elfGetFontSize(elf_handle font)
{
	if(!font.get() || elf_get_object_type(font.get()) != ELF_FONT)
	{
		printf("GetFontSize() -> invalid handle\n");
		return 0;
	}
	return elf_get_font_size((elf_font*)font.get());
}
ELF_API int ELF_APIENTRY elfGetStringWidth(elf_handle font, const char* str)
{
	if(!font.get() || elf_get_object_type(font.get()) != ELF_FONT)
	{
		printf("GetStringWidth() -> invalid handle\n");
		return 0;
	}
	return elf_get_string_width((elf_font*)font.get(), str);
}
ELF_API int ELF_APIENTRY elfGetStringHeight(elf_handle font, const char* str)
{
	if(!font.get() || elf_get_object_type(font.get()) != ELF_FONT)
	{
		printf("GetStringHeight() -> invalid handle\n");
		return 0;
	}
	return elf_get_string_height((elf_font*)font.get(), str);
}
ELF_API const char* ELF_APIENTRY elfGetGuiObjectName(elf_handle object)
{
	if(!object.get() || !elf_is_gui_object(object.get()))
	{
		printf("GetGuiObjectName() -> invalid handle\n");
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
		printf("GetGuiObjectPosition() -> invalid handle\n");
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
		printf("GetGuiObjectSize() -> invalid handle\n");
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
		printf("GetGuiObjectColor() -> invalid handle\n");
		return _e_type;
	}
	_e_type = elf_get_gui_object_color((elf_gui_object*)object.get());
	return _e_type;
}
ELF_API bool ELF_APIENTRY elfGetGuiObjectVisible(elf_handle object)
{
	if(!object.get() || !elf_is_gui_object(object.get()))
	{
		printf("GetGuiObjectVisible() -> invalid handle\n");
		return false;
	}
	return (bool)elf_get_gui_object_visible((elf_gui_object*)object.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetGuiObjectScript(elf_handle object)
{
	elf_handle handle;
	if(!object.get() || !elf_is_gui_object(object.get()))
	{
		printf("GetGuiObjectScript() -> invalid handle\n");
		return handle;
	}
	handle = (elf_object*)elf_get_gui_object_script((elf_gui_object*)object.get());
	return handle;
}
ELF_API int ELF_APIENTRY elfGetGuiObjectEvent(elf_handle object)
{
	if(!object.get() || !elf_is_gui_object(object.get()))
	{
		printf("GetGuiObjectEvent() -> invalid handle\n");
		return 0;
	}
	return elf_get_gui_object_event((elf_gui_object*)object.get());
}
ELF_API void ELF_APIENTRY elfSetGuiObjectPosition(elf_handle object, float x, float y)
{
	if(!object.get() || !elf_is_gui_object(object.get()))
	{
		printf("SetGuiObjectPosition() -> invalid handle\n");
		return;
	}
	elf_set_gui_object_position((elf_gui_object*)object.get(), x, y);
}
ELF_API void ELF_APIENTRY elfSetGuiObjectColor(elf_handle object, float r, float g, float b, float a)
{
	if(!object.get() || !elf_is_gui_object(object.get()))
	{
		printf("SetGuiObjectColor() -> invalid handle\n");
		return;
	}
	elf_set_gui_object_color((elf_gui_object*)object.get(), r, g, b, a);
}
ELF_API void ELF_APIENTRY elfSetGuiObjectVisible(elf_handle object, bool visible)
{
	if(!object.get() || !elf_is_gui_object(object.get()))
	{
		printf("SetGuiObjectVisible() -> invalid handle\n");
		return;
	}
	elf_set_gui_object_visible((elf_gui_object*)object.get(), visible);
}
ELF_API void ELF_APIENTRY elfSetGuiObjectScript(elf_handle object, elf_handle script)
{
	if(!object.get() || !elf_is_gui_object(object.get()))
	{
		printf("SetGuiObjectScript() -> invalid handle\n");
		return;
	}
	if(!script.get() || elf_get_object_type(script.get()) != ELF_SCRIPT)
	{
		printf("SetGuiObjectScript() -> invalid handle\n");
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
		printf("GetLabelFont() -> invalid handle\n");
		return handle;
	}
	handle = (elf_object*)elf_get_label_font((elf_label*)label.get());
	return handle;
}
ELF_API const char* ELF_APIENTRY elfGetLabelText(elf_handle label)
{
	if(!label.get() || elf_get_object_type(label.get()) != ELF_LABEL)
	{
		printf("GetLabelText() -> invalid handle\n");
		return "";
	}
	return elf_get_label_text((elf_label*)label.get());
}
ELF_API void ELF_APIENTRY elfSetLabelFont(elf_handle label, elf_handle font)
{
	if(!label.get() || elf_get_object_type(label.get()) != ELF_LABEL)
	{
		printf("SetLabelFont() -> invalid handle\n");
		return;
	}
	if(!font.get() || elf_get_object_type(font.get()) != ELF_FONT)
	{
		printf("SetLabelFont() -> invalid handle\n");
		return;
	}
	elf_set_label_font((elf_label*)label.get(), (elf_font*)font.get());
}
ELF_API void ELF_APIENTRY elfSetLabelText(elf_handle label, const char* text)
{
	if(!label.get() || elf_get_object_type(label.get()) != ELF_LABEL)
	{
		printf("SetLabelText() -> invalid handle\n");
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
		printf("GetButtonState() -> invalid handle\n");
		return false;
	}
	return (bool)elf_get_button_state((elf_button*)button.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetButtonOffTexture(elf_handle button)
{
	elf_handle handle;
	if(!button.get() || elf_get_object_type(button.get()) != ELF_BUTTON)
	{
		printf("GetButtonOffTexture() -> invalid handle\n");
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
		printf("GetButtonOverTexture() -> invalid handle\n");
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
		printf("GetButtonOnTexture() -> invalid handle\n");
		return handle;
	}
	handle = (elf_object*)elf_get_button_on_texture((elf_button*)button.get());
	return handle;
}
ELF_API void ELF_APIENTRY elfSetButtonOffTexture(elf_handle button, elf_handle off)
{
	if(!button.get() || elf_get_object_type(button.get()) != ELF_BUTTON)
	{
		printf("SetButtonOffTexture() -> invalid handle\n");
		return;
	}
	if(!off.get() || elf_get_object_type(off.get()) != ELF_TEXTURE)
	{
		printf("SetButtonOffTexture() -> invalid handle\n");
		return;
	}
	elf_set_button_off_texture((elf_button*)button.get(), (elf_texture*)off.get());
}
ELF_API void ELF_APIENTRY elfSetButtonOverTexture(elf_handle button, elf_handle over)
{
	if(!button.get() || elf_get_object_type(button.get()) != ELF_BUTTON)
	{
		printf("SetButtonOverTexture() -> invalid handle\n");
		return;
	}
	if(!over.get() || elf_get_object_type(over.get()) != ELF_TEXTURE)
	{
		printf("SetButtonOverTexture() -> invalid handle\n");
		return;
	}
	elf_set_button_over_texture((elf_button*)button.get(), (elf_texture*)over.get());
}
ELF_API void ELF_APIENTRY elfSetButtonOnTexture(elf_handle button, elf_handle on)
{
	if(!button.get() || elf_get_object_type(button.get()) != ELF_BUTTON)
	{
		printf("SetButtonOnTexture() -> invalid handle\n");
		return;
	}
	if(!on.get() || elf_get_object_type(on.get()) != ELF_TEXTURE)
	{
		printf("SetButtonOnTexture() -> invalid handle\n");
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
		printf("GetPictureTexture() -> invalid handle\n");
		return handle;
	}
	handle = (elf_object*)elf_get_picture_texture((elf_picture*)picture.get());
	return handle;
}
ELF_API void ELF_APIENTRY elfSetPictureTexture(elf_handle picture, elf_handle texture)
{
	if(!picture.get() || elf_get_object_type(picture.get()) != ELF_PICTURE)
	{
		printf("SetPictureTexture() -> invalid handle\n");
		return;
	}
	if(!texture.get() || elf_get_object_type(texture.get()) != ELF_TEXTURE)
	{
		printf("SetPictureTexture() -> invalid handle\n");
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
		printf("GetTextFieldTexture() -> invalid handle\n");
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
		printf("GetTextFieldFont() -> invalid handle\n");
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
		printf("GetTextFieldTextColor() -> invalid handle\n");
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
		printf("GetTextFieldOffset() -> invalid handle\n");
		return _e_type;
	}
	_e_type = elf_get_text_field_offset((elf_text_field*)text_field.get());
	return _e_type;
}
ELF_API const char* ELF_APIENTRY elfGetTextFieldText(elf_handle text_field)
{
	if(!text_field.get() || elf_get_object_type(text_field.get()) != ELF_TEXT_FIELD)
	{
		printf("GetTextFieldText() -> invalid handle\n");
		return "";
	}
	return elf_get_text_field_text((elf_text_field*)text_field.get());
}
ELF_API void ELF_APIENTRY elfSetTextFieldTexture(elf_handle text_field, elf_handle texture)
{
	if(!text_field.get() || elf_get_object_type(text_field.get()) != ELF_TEXT_FIELD)
	{
		printf("SetTextFieldTexture() -> invalid handle\n");
		return;
	}
	if(!texture.get() || elf_get_object_type(texture.get()) != ELF_TEXTURE)
	{
		printf("SetTextFieldTexture() -> invalid handle\n");
		return;
	}
	elf_set_text_field_texture((elf_text_field*)text_field.get(), (elf_texture*)texture.get());
}
ELF_API void ELF_APIENTRY elfSetTextFieldFont(elf_handle text_field, elf_handle font)
{
	if(!text_field.get() || elf_get_object_type(text_field.get()) != ELF_TEXT_FIELD)
	{
		printf("SetTextFieldFont() -> invalid handle\n");
		return;
	}
	if(!font.get() || elf_get_object_type(font.get()) != ELF_FONT)
	{
		printf("SetTextFieldFont() -> invalid handle\n");
		return;
	}
	elf_set_text_field_font((elf_text_field*)text_field.get(), (elf_font*)font.get());
}
ELF_API void ELF_APIENTRY elfSetTextFieldTextColor(elf_handle text_field, float r, float g, float b, float a)
{
	if(!text_field.get() || elf_get_object_type(text_field.get()) != ELF_TEXT_FIELD)
	{
		printf("SetTextFieldTextColor() -> invalid handle\n");
		return;
	}
	elf_set_text_field_text_color((elf_text_field*)text_field.get(), r, g, b, a);
}
ELF_API void ELF_APIENTRY elfSetTextFieldOffset(elf_handle text_field, int offset_x, int offset_y)
{
	if(!text_field.get() || elf_get_object_type(text_field.get()) != ELF_TEXT_FIELD)
	{
		printf("SetTextFieldOffset() -> invalid handle\n");
		return;
	}
	elf_set_text_field_offset((elf_text_field*)text_field.get(), offset_x, offset_y);
}
ELF_API void ELF_APIENTRY elfSetTextFieldText(elf_handle text_field, const char* text)
{
	if(!text_field.get() || elf_get_object_type(text_field.get()) != ELF_TEXT_FIELD)
	{
		printf("SetTextFieldText() -> invalid handle\n");
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
		printf("GetSliderBackgroundTexture() -> invalid handle\n");
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
		printf("GetSliderSliderTexture() -> invalid handle\n");
		return handle;
	}
	handle = (elf_object*)elf_get_slider_slider_texture((elf_slider*)slider.get());
	return handle;
}
ELF_API float ELF_APIENTRY elfGetSliderValue(elf_handle slider)
{
	if(!slider.get() || elf_get_object_type(slider.get()) != ELF_SLIDER)
	{
		printf("GetSliderValue() -> invalid handle\n");
		return 0;
	}
	return elf_get_slider_value((elf_slider*)slider.get());
}
ELF_API void ELF_APIENTRY elfSetSliderBackgroundTexture(elf_handle slider, elf_handle background)
{
	if(!slider.get() || elf_get_object_type(slider.get()) != ELF_SLIDER)
	{
		printf("SetSliderBackgroundTexture() -> invalid handle\n");
		return;
	}
	if(!background.get() || elf_get_object_type(background.get()) != ELF_TEXTURE)
	{
		printf("SetSliderBackgroundTexture() -> invalid handle\n");
		return;
	}
	elf_set_slider_background_texture((elf_slider*)slider.get(), (elf_texture*)background.get());
}
ELF_API void ELF_APIENTRY elfSetSliderSliderTexture(elf_handle slider, elf_handle slider_texture)
{
	if(!slider.get() || elf_get_object_type(slider.get()) != ELF_SLIDER)
	{
		printf("SetSliderSliderTexture() -> invalid handle\n");
		return;
	}
	if(!slider_texture.get() || elf_get_object_type(slider_texture.get()) != ELF_TEXTURE)
	{
		printf("SetSliderSliderTexture() -> invalid handle\n");
		return;
	}
	elf_set_slider_slider_texture((elf_slider*)slider.get(), (elf_texture*)slider_texture.get());
}
ELF_API void ELF_APIENTRY elfSetSliderValue(elf_handle slider, float value)
{
	if(!slider.get() || elf_get_object_type(slider.get()) != ELF_SLIDER)
	{
		printf("SetSliderValue() -> invalid handle\n");
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
		printf("GetScreenTexture() -> invalid handle\n");
		return handle;
	}
	handle = (elf_object*)elf_get_screen_texture((elf_screen*)screen.get());
	return handle;
}
ELF_API void ELF_APIENTRY elfSetScreenTexture(elf_handle screen, elf_handle texture)
{
	if(!screen.get() || elf_get_object_type(screen.get()) != ELF_SCREEN)
	{
		printf("SetScreenTexture() -> invalid handle\n");
		return;
	}
	if(!texture.get() || elf_get_object_type(texture.get()) != ELF_TEXTURE)
	{
		printf("SetScreenTexture() -> invalid handle\n");
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
		printf("GetTextListFont() -> invalid handle\n");
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
		printf("GetTextListSelectionColor() -> invalid handle\n");
		return _e_type;
	}
	_e_type = elf_get_text_list_selection_color((elf_text_list*)text_list.get());
	return _e_type;
}
ELF_API int ELF_APIENTRY elfGetTextListRowCount(elf_handle text_list)
{
	if(!text_list.get() || elf_get_object_type(text_list.get()) != ELF_TEXT_LIST)
	{
		printf("GetTextListRowCount() -> invalid handle\n");
		return 0;
	}
	return elf_get_text_list_row_count((elf_text_list*)text_list.get());
}
ELF_API int ELF_APIENTRY elfGetTextListItemCount(elf_handle text_list)
{
	if(!text_list.get() || elf_get_object_type(text_list.get()) != ELF_TEXT_LIST)
	{
		printf("GetTextListItemCount() -> invalid handle\n");
		return 0;
	}
	return elf_get_text_list_item_count((elf_text_list*)text_list.get());
}
ELF_API int ELF_APIENTRY elfGetTextListSelectionIndex(elf_handle text_list)
{
	if(!text_list.get() || elf_get_object_type(text_list.get()) != ELF_TEXT_LIST)
	{
		printf("GetTextListSelectionIndex() -> invalid handle\n");
		return 0;
	}
	return elf_get_text_list_selection_index((elf_text_list*)text_list.get());
}
ELF_API int ELF_APIENTRY elfGetTextListOffset(elf_handle text_list)
{
	if(!text_list.get() || elf_get_object_type(text_list.get()) != ELF_TEXT_LIST)
	{
		printf("GetTextListOffset() -> invalid handle\n");
		return 0;
	}
	return elf_get_text_list_offset((elf_text_list*)text_list.get());
}
ELF_API const char* ELF_APIENTRY elfGetTextListItem(elf_handle text_list, int idx)
{
	if(!text_list.get() || elf_get_object_type(text_list.get()) != ELF_TEXT_LIST)
	{
		printf("GetTextListItem() -> invalid handle\n");
		return "";
	}
	return elf_get_text_list_item((elf_text_list*)text_list.get(), idx);
}
ELF_API const char* ELF_APIENTRY elfGetTextListSelectedItem(elf_handle text_list)
{
	if(!text_list.get() || elf_get_object_type(text_list.get()) != ELF_TEXT_LIST)
	{
		printf("GetTextListSelectedItem() -> invalid handle\n");
		return "";
	}
	return elf_get_text_list_selected_item((elf_text_list*)text_list.get());
}
ELF_API void ELF_APIENTRY elfSetTextListFont(elf_handle text_list, elf_handle font)
{
	if(!text_list.get() || elf_get_object_type(text_list.get()) != ELF_TEXT_LIST)
	{
		printf("SetTextListFont() -> invalid handle\n");
		return;
	}
	if(!font.get() || elf_get_object_type(font.get()) != ELF_FONT)
	{
		printf("SetTextListFont() -> invalid handle\n");
		return;
	}
	elf_set_text_list_font((elf_text_list*)text_list.get(), (elf_font*)font.get());
}
ELF_API void ELF_APIENTRY elfSetTextListSelectionColor(elf_handle text_list, float r, float g, float b, float a)
{
	if(!text_list.get() || elf_get_object_type(text_list.get()) != ELF_TEXT_LIST)
	{
		printf("SetTextListSelectionColor() -> invalid handle\n");
		return;
	}
	elf_set_text_list_selection_color((elf_text_list*)text_list.get(), r, g, b, a);
}
ELF_API void ELF_APIENTRY elfSetTextListSize(elf_handle text_list, int rows, int width)
{
	if(!text_list.get() || elf_get_object_type(text_list.get()) != ELF_TEXT_LIST)
	{
		printf("SetTextListSize() -> invalid handle\n");
		return;
	}
	elf_set_text_list_size((elf_text_list*)text_list.get(), rows, width);
}
ELF_API void ELF_APIENTRY elfAddTextListItem(elf_handle text_list, const char* text)
{
	if(!text_list.get() || elf_get_object_type(text_list.get()) != ELF_TEXT_LIST)
	{
		printf("AddTextListItem() -> invalid handle\n");
		return;
	}
	elf_add_text_list_item((elf_text_list*)text_list.get(), text);
}
ELF_API bool ELF_APIENTRY elfRemoveTextListItem(elf_handle text_list, int idx)
{
	if(!text_list.get() || elf_get_object_type(text_list.get()) != ELF_TEXT_LIST)
	{
		printf("RemoveTextListItem() -> invalid handle\n");
		return false;
	}
	return (bool)elf_remove_text_list_item((elf_text_list*)text_list.get(), idx);
}
ELF_API void ELF_APIENTRY elfRemoveTextListItems(elf_handle text_list)
{
	if(!text_list.get() || elf_get_object_type(text_list.get()) != ELF_TEXT_LIST)
	{
		printf("RemoveTextListItems() -> invalid handle\n");
		return;
	}
	elf_remove_text_list_items((elf_text_list*)text_list.get());
}
ELF_API void ELF_APIENTRY elfSetTextListOffset(elf_handle text_list, int offset)
{
	if(!text_list.get() || elf_get_object_type(text_list.get()) != ELF_TEXT_LIST)
	{
		printf("SetTextListOffset() -> invalid handle\n");
		return;
	}
	elf_set_text_list_offset((elf_text_list*)text_list.get(), offset);
}
ELF_API void ELF_APIENTRY elfSetTextListSelection(elf_handle text_list, int selection)
{
	if(!text_list.get() || elf_get_object_type(text_list.get()) != ELF_TEXT_LIST)
	{
		printf("SetTextListSelection() -> invalid handle\n");
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
		printf("GetCheckBoxState() -> invalid handle\n");
		return false;
	}
	return (bool)elf_get_check_box_state((elf_check_box*)check_box.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetCheckBoxOffTexture(elf_handle check_box)
{
	elf_handle handle;
	if(!check_box.get() || elf_get_object_type(check_box.get()) != ELF_CHECK_BOX)
	{
		printf("GetCheckBoxOffTexture() -> invalid handle\n");
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
		printf("GetCheckBoxOnTexture() -> invalid handle\n");
		return handle;
	}
	handle = (elf_object*)elf_get_check_box_on_texture((elf_check_box*)check_box.get());
	return handle;
}
ELF_API void ELF_APIENTRY elfSetCheckBoxOffTexture(elf_handle check_box, elf_handle off)
{
	if(!check_box.get() || elf_get_object_type(check_box.get()) != ELF_CHECK_BOX)
	{
		printf("SetCheckBoxOffTexture() -> invalid handle\n");
		return;
	}
	if(!off.get() || elf_get_object_type(off.get()) != ELF_TEXTURE)
	{
		printf("SetCheckBoxOffTexture() -> invalid handle\n");
		return;
	}
	elf_set_check_box_off_texture((elf_check_box*)check_box.get(), (elf_texture*)off.get());
}
ELF_API void ELF_APIENTRY elfSetCheckBoxOnTexture(elf_handle check_box, elf_handle on)
{
	if(!check_box.get() || elf_get_object_type(check_box.get()) != ELF_CHECK_BOX)
	{
		printf("SetCheckBoxOnTexture() -> invalid handle\n");
		return;
	}
	if(!on.get() || elf_get_object_type(on.get()) != ELF_TEXTURE)
	{
		printf("SetCheckBoxOnTexture() -> invalid handle\n");
		return;
	}
	elf_set_check_box_on_texture((elf_check_box*)check_box.get(), (elf_texture*)on.get());
}
ELF_API void ELF_APIENTRY elfSetCheckBoxState(elf_handle check_box, bool state)
{
	if(!check_box.get() || elf_get_object_type(check_box.get()) != ELF_CHECK_BOX)
	{
		printf("SetCheckBoxState() -> invalid handle\n");
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
		printf("AddGuiObject() -> invalid handle\n");
		return false;
	}
	if(!object.get() || !elf_is_gui_object(object.get()))
	{
		printf("AddGuiObject() -> invalid handle\n");
		return false;
	}
	return (bool)elf_add_gui_object((elf_gui_object*)parent.get(), (elf_gui_object*)object.get());
}
ELF_API elf_handle ELF_APIENTRY elfGetGuiObjectByName(elf_handle parent, const char* name)
{
	elf_handle handle;
	if(!parent.get() || !elf_is_gui_object(parent.get()))
	{
		printf("GetGuiObjectByName() -> invalid handle\n");
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
		printf("GetGuiObjectByIndex() -> invalid handle\n");
		return handle;
	}
	handle = (elf_object*)elf_get_gui_object_by_index((elf_gui_object*)parent.get(), idx);
	return handle;
}
ELF_API bool ELF_APIENTRY elfRemoveGuiObjectByName(elf_handle parent, const char* name)
{
	if(!parent.get() || !elf_is_gui_object(parent.get()))
	{
		printf("RemoveGuiObjectByName() -> invalid handle\n");
		return false;
	}
	return (bool)elf_remove_gui_object_by_name((elf_gui_object*)parent.get(), name);
}
ELF_API bool ELF_APIENTRY elfRemoveGuiObjectByIndex(elf_handle parent, int idx)
{
	if(!parent.get() || !elf_is_gui_object(parent.get()))
	{
		printf("RemoveGuiObjectByIndex() -> invalid handle\n");
		return false;
	}
	return (bool)elf_remove_gui_object_by_index((elf_gui_object*)parent.get(), idx);
}
ELF_API bool ELF_APIENTRY elfRemoveGuiObjectByObject(elf_handle parent, elf_handle object)
{
	if(!parent.get() || !elf_is_gui_object(parent.get()))
	{
		printf("RemoveGuiObjectByObject() -> invalid handle\n");
		return false;
	}
	if(!object.get() || !elf_is_gui_object(object.get()))
	{
		printf("RemoveGuiObjectByObject() -> invalid handle\n");
		return false;
	}
	return (bool)elf_remove_gui_object_by_object((elf_gui_object*)parent.get(), (elf_gui_object*)object.get());
}
ELF_API void ELF_APIENTRY elfEmptyGui(elf_handle gui)
{
	if(!gui.get() || elf_get_object_type(gui.get()) != ELF_GUI)
	{
		printf("EmptyGui() -> invalid handle\n");
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

