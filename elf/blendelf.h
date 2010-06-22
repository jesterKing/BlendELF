
// <!!
#ifndef BLENDELF_H
#define BLENDELF_H

#ifdef __cplusplus
extern "C" {
#endif
// !!>

#define ELF_FALSE					0x0000	// <mdoc> TRUTH VALUES <mdocc> The thruth value flags. Usually you can just use normal Lua true and false but there are some special cases.
#define ELF_TRUE					0x0001

#define ELF_NONE					0x0000 // <mdoc> NONE VALUE

#define ELF_KEY_ESC					135	// <mdoc> KEY CODES <mdocc> The key codes used by elf.GetKeyState
#define ELF_KEY_F1					136
#define ELF_KEY_F2					137
#define ELF_KEY_F3					138
#define ELF_KEY_F4					139
#define ELF_KEY_F5					140
#define ELF_KEY_F6					141
#define ELF_KEY_F7					142
#define ELF_KEY_F8					143
#define ELF_KEY_F9					144
#define ELF_KEY_F10					145
#define ELF_KEY_F11					146
#define ELF_KEY_F12					147
#define ELF_KEY_UP					148
#define ELF_KEY_DOWN					149
#define ELF_KEY_LEFT					150
#define ELF_KEY_RIGHT					151
#define ELF_KEY_LSHIFT					152
#define ELF_KEY_RSHIFT					153
#define ELF_KEY_LCTRL					154
#define ELF_KEY_RCTRL					155
#define ELF_KEY_LALT					156
#define ELF_KEY_RALT					157
#define ELF_KEY_TAB					158
#define ELF_KEY_ENTER					159
#define ELF_KEY_BACKSPACE				160
#define ELF_KEY_INSERT					161
#define ELF_KEY_DEL					162
#define ELF_KEY_PAGEUP					163
#define ELF_KEY_PAGEDOWN				164
#define ELF_KEY_HOME					165
#define ELF_KEY_END					166
#define ELF_KEY_KP_0					167
#define ELF_KEY_KP_1					168
#define ELF_KEY_KP_2					169
#define ELF_KEY_KP_3					170
#define ELF_KEY_KP_4					171
#define ELF_KEY_KP_5					172
#define ELF_KEY_KP_6					173
#define ELF_KEY_KP_7					174
#define ELF_KEY_KP_8					175
#define ELF_KEY_KP_9					176
#define ELF_KEY_KP_DIVIDE				177
#define ELF_KEY_KP_MULTIPLY				178
#define ELF_KEY_KP_SUBTRACT				179
#define ELF_KEY_KP_ADD					180
#define ELF_KEY_KP_DECIMAL				181
#define ELF_KEY_KP_EQUAL				182
#define ELF_KEY_KP_ENTER				183
#define ELF_KEY_SPACE					32
#define ELF_KEY_0					48
#define ELF_KEY_1					49
#define ELF_KEY_2					50
#define ELF_KEY_3					51
#define ELF_KEY_4					52
#define ELF_KEY_5					53
#define ELF_KEY_6					54
#define ELF_KEY_7					55
#define ELF_KEY_8					56
#define ELF_KEY_9					57
#define ELF_KEY_A					65
#define ELF_KEY_B					66
#define ELF_KEY_C					67
#define ELF_KEY_D					68
#define ELF_KEY_E					69
#define ELF_KEY_F					70
#define ELF_KEY_G					71
#define ELF_KEY_H					72
#define ELF_KEY_I					73
#define ELF_KEY_J					74
#define ELF_KEY_K					75
#define ELF_KEY_L					76
#define ELF_KEY_M					77
#define ELF_KEY_N					78
#define ELF_KEY_O					79
#define ELF_KEY_P					80
#define ELF_KEY_Q					81
#define ELF_KEY_R					82
#define ELF_KEY_S					83
#define ELF_KEY_T					84
#define ELF_KEY_U					85
#define ELF_KEY_V					86
#define ELF_KEY_W					87
#define ELF_KEY_X					88
#define ELF_KEY_Y					89
#define ELF_KEY_Z					90

#define ELF_UP						0x0000	// <mdoc> KEY AND MOUSE STATES <mdocc> The state codes returned by elf.GetKeyState and elf.GetMouseButtonState
#define ELF_PRESSED					0x0001
#define ELF_DOWN					0x0002
#define ELF_RELEASED					0x0003

#define ELF_OFF						0x0000	// <mdoc> GUI BUTTON STATE <mdocc> The gui button states returned by elf.GetButtonState
#define ELF_ON						0x0001
#define ELF_OVER					0x0002

#define ELF_BUTTON_LEFT					0x0000	// <mdoc> MOUSE BUTTON CODES <mdocc> The mouse button codes used by elf.GetMouseButtonState
#define ELF_BUTTON_MIDDLE				0x0001
#define ELF_BUTTON_RIGHT				0x0002

#define ELF_TEXTURE					0x0000	// <mdoc> ELF OBJECT TYEPS <mdocc> The  object types returned by elf.GetObjectType
#define ELF_MATERIAL					0x0001
#define ELF_MODEL					0x0002
#define ELF_CAMERA					0x0003
#define ELF_ENTITY					0x0004
#define ELF_LIGHT					0x0005
#define ELF_SCENE					0x0006
#define ELF_LIST					0x0007
#define ELF_PAK						0x0008
#define ELF_PAK_INDEX					0x0009
#define ELF_KEY_EVENT					0x000A
#define ELF_CHAR_EVENT					0x000B
#define ELF_CONTEXT					0x000C
#define ELF_ENGINE					0x000D
#define ELF_TIMER					0x000E
#define ELF_IMAGE					0x000F
#define ELF_POST_PROCESS				0x0010
#define ELF_SCRIPT					0x0011
#define ELF_AUDIO_DEVICE				0x0012
#define ELF_AUDIO_SOURCE				0x0013
#define ELF_SOUND					0x0014
#define ELF_BONE					0x0015
#define ELF_ARMATURE					0x0016
#define ELF_STRING					0x0017
#define ELF_FONT					0x0018
#define ELF_LABEL					0x0019
#define ELF_BUTTON					0x001A
#define ELF_PICTURE					0x001B
#define ELF_TEXT_FIELD					0x001C
#define ELF_SLIDER					0x001D
#define ELF_SCREEN					0x001E
#define ELF_TEXT_LIST					0x001F
#define ELF_CHECK_BOX					0x0020
#define ELF_GUI						0x0021
#define ELF_DIRECTORY_ITEM				0x0022
#define ELF_DIRECTORY					0x0023
#define ELF_COLLISION					0x0034
#define ELF_PHYSICS_OBJECT				0x0035
#define ELF_PHYSICS_WORLD				0x0036
#define ELF_JOINT					0x0037
#define ELF_PARTICLE					0x0038
#define ELF_PARTICLES					0x0039
#define ELF_GAME_CONFIG					0x003A
#define ELF_BEZIER_POINT				0x003B
#define ELF_BEZIER_CURVE				0x003C
#define ELF_IPO						0x003D
#define ELF_FRAME_PLAYER				0x003E
#define ELF_PROPERTY					0x004F
#define ELF_CLIENT					0x0040
#define ELF_SCRIPTING					0x0041
#define ELF_PHYSICS_TRI_MESH				0x0042
#define ELF_SPRITE					0x0043
#define ELF_VIDEO_MODE					0x0044
#define ELF_OBJECT_TYPE_COUNT				0x0045	// <mdoc> NUMBER OF OBJECT TYPES

#define ELF_PERSPECTIVE					0x0000	// <mdoc> CAMERA MODE <mdocc> The camera modes used by camera internal functions
#define ELF_ORTHOGRAPHIC				0x0001

#define ELF_BOX						0x0001	// <mdoc> PHYSICS SHAPES <mdocc> The physics shapes used by elf.SetEntityPhysics
#define ELF_SPHERE					0x0002
#define ELF_MESH					0x0003
#define ELF_CAPSULE					0x0004

#define ELF_HINGE					0x0001	// <mdoc> JOINT TYPES <mdocc> The joint types returned by elf.GetJointType
#define ELF_BALL					0x0002
#define ELF_CONE_TWIST					0x0003

#define ELF_POINT_LIGHT					0x0001	// <mdoc> LIGHT TYPES <mdocc> The light types used by elf.SetLightType
#define ELF_SUN_LIGHT					0x0002
#define ELF_SPOT_LIGHT					0x0003

#define ELF_LUMINANCE					0x0000	// <mdoc> TEXTURE FORMATS <mdocc> The texture formats returned by elf.GetTextureFormat
#define ELF_LUMINANCE_ALPHA				0x0001
#define ELF_RGB						0x0002
#define ELF_RGBA					0x0003
#define ELF_BGR						0x0004
#define ELF_BGRA					0x0005
#define ELF_RGB16F					0x0006
#define ELF_RGB32F					0x0007
#define ELF_RGBA16F					0x0008
#define ELF_RGBA32F					0x0009
#define ELF_ALPHA32F					0x000A
#define ELF_DEPTH_COMPONENT				0x000B
#define ELF_COMPRESSED_RGB				0x000C
#define ELF_COMPRESSED_RGBA				0x000D

#define ELF_FLOAT					0x0000	// <mdoc> TEXTURE DATA FORMATS <mdocc> The texture formats returned by elf.GetTextureDataFormat
#define ELF_INT						0x0001
#define ELF_UINT					0x0002
#define ELF_SHORT					0x0003
#define ELF_USHORT					0x0004
#define ELF_BYTE					0x0005
#define ELF_UBYTE					0x0006

#define ELF_PROPERTY_INT				0x0001	// <mdoc> PROPERTY TYPES <mdocc> The property types returned by elf.GetPropertyType
#define ELF_PROPERTY_FLOAT				0x0002
#define ELF_PROPERTY_STRING				0x0003
#define ELF_PROPERTY_BOOL				0x0004

#define ELF_COLOR_MAP					0x0001	// <mdoc> TEXTURE TYPES <mdocc> The textures types used by elf.SetMaterialTextureType
#define ELF_NORMAL_MAP					0x0002
#define ELF_HEIGHT_MAP					0x0004
#define ELF_SPECULAR_MAP				0x0008
#define ELF_DETAIL_MAP					0x0010
#define ELF_SHADOW_MAP					0x0020
#define ELF_COLOR_RAMP_MAP				0x0040
#define ELF_LIGHT_MAP					0x0080

#define ELF_TRANSPARENT					0x0001	// <mdoc> PARTICLE DRAW MODES <mdocc> The draw modes used by elf.SetParticlesDrawMode
#define ELF_ADD						0x0002

#define ELF_DIR						0x0001	// <mdoc> DIRECTORY ITEM TYPES <mdocc> The directory item types used by elf.GetDirectoryItemType
#define ELF_FILE					0x0002

#define ELF_CLICKED					0x0001	// <mdoc> GUI EVENTS <mdocc> The gui events triggered by gui objects. The gui events are automatically set to the "event" variable when in a gui action.
#define ELF_VALUE_CHANGED				0x0002
#define ELF_GAIN_FOCUS					0x0003
#define ELF_LOSE_FOCUS					0x0004
#define ELF_CHAR_INPUT					0x0005
#define ELF_SELECTION_CHANGED				0x0006
#define ELF_STATE_CHANGED				0x0007

#define ELF_JOYSTICK_BUTTON_1				0x0000	// <mdoc> JOYSTICK BUTTONS <mdocc> The joystick buttons used by elf.GetJoystickButtonState
#define ELF_JOYSTICK_BUTTON_2				0x0001
#define ELF_JOYSTICK_BUTTON_3				0x0002
#define ELF_JOYSTICK_BUTTON_4				0x0003
#define ELF_JOYSTICK_BUTTON_5				0x0004
#define ELF_JOYSTICK_BUTTON_6				0x0005
#define ELF_JOYSTICK_BUTTON_7				0x0006
#define ELF_JOYSTICK_BUTTON_8				0x0007
#define ELF_JOYSTICK_BUTTON_9				0x0008
#define ELF_JOYSTICK_BUTTON_10				0x0009
#define ELF_JOYSTICK_BUTTON_11				0x000A
#define ELF_JOYSTICK_BUTTON_12				0x000B
#define ELF_JOYSTICK_BUTTON_13				0x000C
#define ELF_JOYSTICK_BUTTON_14				0x000D
#define ELF_JOYSTICK_BUTTON_15				0x000E
#define ELF_JOYSTICK_BUTTON_16				0x000F

#define ELF_LOC_X					0x0000	// <mdoc> BEZIER CURVE TYPES
#define ELF_LOC_Y					0x0001
#define ELF_LOC_Z					0x0002
#define ELF_ROT_X					0x0003
#define ELF_ROT_Y					0x0004
#define ELF_ROT_Z					0x0005
#define ELF_SCALE_X					0x0006
#define ELF_SCALE_Y					0x0007
#define ELF_SCALE_Z					0x0008
#define ELF_QUA_X					0x0009
#define ELF_QUA_Y					0x000A
#define ELF_QUA_Z					0x000B
#define ELF_QUA_W					0x000C

#define ELF_NET_NONE					0x0000	// <mdoc> NETWORKING EVENTS
#define ELF_NET_CONNECT					0x0001
#define ELF_NET_RECEIVE					0x0002
#define ELF_NET_DISCONNECT				0x0003

#define ELF_OGG						0x0001	// <mdoc> SOUND FILE TYPES <mdocc> The sound file types returned by elf.GetSoundFileType
#define ELF_WAV						0x0002

#define ELF_NO_ERROR					0x0000 // <mdoc> ERROR CODES <mdocc> error codes returned by elf.GetError
#define ELF_INVALID_FILE				0x0001
#define ELF_CANT_OPEN_FILE				0x0002
#define ELF_CANT_OPEN_DIRECTORY				0x0003
#define ELF_CANT_INITIALIZE				0x0004
#define ELF_CANT_RUN_STRING				0x0005
#define ELF_CANT_RUN_SCRIPT				0x0006
#define ELF_CANT_CREATE					0x0007
#define ELF_CANT_RESIZE					0x0008
#define ELF_INVALID_SIZE				0x0009
#define ELF_UNKNOWN_FORMAT				0x000A
#define ELF_UNKNOWN_TYPE				0x000B
#define ELF_INVALID_HANDLE				0x000C
#define ELF_MISSING_FEATURE				0x000D

// <!!
#define ELF_X_PLUS					0x0001	// <mdoc> AXIS TYPES <mdocc> axis codes used by elf.DirectActorAt
#define ELF_X_MINUS					0x0002
#define ELF_Y_PLUS					0x0003
#define ELF_Y_MINUS					0x0004
#define ELF_Z_PLUS					0x0005
#define ELF_Z_MINUS					0x0000
// !!>

// <!!
#define ELF_ARMATURE_MAGIC				179532122
#define ELF_CAMERA_MAGIC				179532111
#define ELF_ENTITY_MAGIC				179532112
#define ELF_LIGHT_MAGIC					179532113
#define ELF_MATERIAL_MAGIC				179532109
#define ELF_MODEL_MAGIC					179532110
#define ELF_PARTICLES_MAGIC				179532141
#define ELF_SCENE_MAGIC					179532120
#define ELF_SCRIPT_MAGIC				179532121
#define ELF_SPRITE_MAGIC				179532140
#define ELF_TEXTURE_MAGIC				179532108
// !!>

typedef struct elf_vec2i				elf_vec2i;
typedef struct elf_vec2f				elf_vec2f;
typedef struct elf_vec3f				elf_vec3f;
typedef struct elf_vec4f				elf_vec4f;
typedef struct elf_color				elf_color;

typedef struct elf_object				elf_object;
typedef struct elf_resource				elf_resource;
typedef struct elf_gui_object				elf_gui_object;
typedef struct elf_list					elf_list;
typedef struct elf_key_event				elf_key_event;
typedef struct elf_char_event				elf_char_event;
typedef struct elf_context				elf_context;
typedef struct elf_engine				elf_engine;
typedef struct elf_timer				elf_timer;

typedef struct elf_image				elf_image;
typedef struct elf_texture				elf_texture;
typedef struct elf_material				elf_material;
typedef struct elf_bezier_point				elf_bezier_point;
typedef struct elf_bezier_curve				elf_bezier_curve;
typedef struct elf_ipo					elf_ipo;
typedef struct elf_actor				elf_actor;
typedef struct elf_camera				elf_camera;
typedef struct elf_model				elf_model;
typedef struct elf_entity				elf_entity;
typedef struct elf_light				elf_light;
typedef struct elf_scene				elf_scene;
typedef struct elf_pak_index				elf_pak_index;
typedef struct elf_pak					elf_pak;
typedef struct elf_post_process				elf_post_process;
typedef struct elf_script				elf_script;
typedef struct elf_audio_device				elf_audio_device;
typedef struct elf_audio_source				elf_audio_source;
typedef struct elf_sound				elf_sound;
typedef struct elf_bone					elf_bone;
typedef struct elf_armature				elf_armature;
typedef struct elf_string				elf_string;
typedef struct elf_font					elf_font;
typedef struct elf_area					elf_area;
typedef struct elf_label				elf_label;
typedef struct elf_button				elf_button;
typedef struct elf_picture				elf_picture;
typedef struct elf_text_field				elf_text_field;
typedef struct elf_slider				elf_slider;
typedef struct elf_screen				elf_screen;
typedef struct elf_text_list				elf_text_list;
typedef struct elf_check_box				elf_check_box;
typedef struct elf_gui					elf_gui;
typedef struct elf_directory_item			elf_directory_item;
typedef struct elf_directory				elf_directory;
typedef struct elf_collision				elf_collision;
typedef struct elf_physics_tri_mesh			elf_physics_tri_mesh;
typedef struct elf_physics_object			elf_physics_object;
typedef struct elf_physics_world			elf_physics_world;
typedef struct elf_joint				elf_joint;
typedef struct elf_resources				elf_resources;
typedef struct elf_particle				elf_particle;
typedef struct elf_particles				elf_particles;
typedef struct elf_game_config				elf_game_config;
typedef struct elf_frame_player				elf_frame_player;
typedef struct elf_property				elf_property;
typedef struct elf_server				elf_server;
typedef struct elf_client				elf_client;
typedef struct elf_scripting				elf_scripting;
typedef struct elf_sprite				elf_sprite;
typedef struct elf_video_mode				elf_video_mode;

// <!!
struct elf_vec2i {
	int x;
	int y;
};

struct elf_vec2f {
	float x;
	float y;
};

struct elf_vec3f {
	float x;
	float y;
	float z;
};

struct elf_vec4f {
	float x;
	float y;
	float z;
	float w;
};

struct elf_color {
	float r;
	float g;
	float b;
	float a;
};
// !!>

//////////////////////////////// OBJECT ////////////////////////////////

// <!!
void elf_init_objects();
void elf_deinit_objects();
void elf_inc_ref(elf_object *obj);
void elf_dec_ref(elf_object *obj);
void elf_log_ref_count_table();
// !!>
int elf_get_object_type(elf_object *obj);	// <mdoc> OBJECT FUNCTIONS <mdocc> The object functions can be performed on any generic ELF objects.
int elf_get_object_ref_count(elf_object *obj);
int elf_get_global_ref_count();
int elf_get_global_obj_count();
unsigned char elf_is_actor(elf_object *obj);
unsigned char elf_is_gui_object(elf_object *obj);

//////////////////////////////// RESOURCE ////////////////////////////////

// <!!
elf_resource* elf_get_resource_by_id(elf_list *resources, int id);
elf_resource* elf_get_resource_by_name(elf_list *resources, const char *name);
void elf_set_unique_name_for_resource(elf_list *named_objects, elf_resource *object);
// !!>

//////////////////////////////// STRING ////////////////////////////////

// <!!
elf_string* elf_create_string_object();
void elf_destroy_string_object(elf_string *string);
char* elf_create_string(const char *str);
void elf_destroy_string(char *str);
char* elf_remove_char_from_string(char *str, int idx);
char* elf_append_char_to_string(char *str, char c);
char* elf_insert_char_to_string(char *str, int idx, char c);
char* elf_merge_strings(const char *str1, const char *str2);
char* elf_merge_strings3(const char *str1, const char *str2, const char *str3);
char* elf_sub_string(char *str, int start, int len);
unsigned char elf_is_char_number(char c);
unsigned char elf_is_string_number(const char *str);
unsigned char elf_is_string_positive_int(const char *str);
int elf_rfind_char_from_string(const char *str, char chr);
int elf_rfind_chars_from_string(const char *str, char *chrs);
// !!>

//////////////////////////////// LIST ////////////////////////////////

elf_list* elf_create_list();	// <mdoc> LIST FUNCTIONS
/* <!> */ void elf_destroy_list(elf_list *list);
int elf_get_list_length(elf_list *list);
void elf_insert_to_list(elf_list *list, int idx, elf_object *obj);
void elf_append_to_list(elf_list *list, elf_object *obj);
unsigned char elf_remove_from_list(elf_list *list, elf_object *obj);
elf_object* elf_get_item_from_list(elf_list *list, int idx);
elf_object* elf_begin_list(elf_list *list);
elf_object* elf_next_in_list(elf_list *list);
elf_object* elf_rbegin_list(elf_list *list);
elf_object* elf_rnext_in_list(elf_list *list);
/* <!> */void elf_set_list_cur_ptr(elf_list *list, elf_object *ptr);
void elf_seek_list(elf_list *list, elf_object *ptr);
void elf_rseek_list(elf_list *list, elf_object *ptr);

//////////////////////////////// CONTEXT ////////////////////////////////

// <!!
elf_video_mode* elf_create_video_mode();
void elf_destroy_video_mode(elf_video_mode *video_mode);

elf_key_event* elf_create_key_event();
void elf_destroy_key_event(elf_key_event *key_event);

elf_char_event* elf_create_char_event();
void elf_destroy_char_event(elf_char_event *char_event);

elf_context* elf_create_context();
void elf_destroy_context(elf_context *context);

unsigned char elf_init_context(int width, int height,
		const char *title, unsigned char fullscreen);
void elf_close_window();

unsigned char elf_resize_context(int width, int height);

#ifdef ELF_WINDOWS
	#ifndef ELF_PLAYER
		#include "windows.h"
		unsigned char elf_init_context_with_hwnd(int width, int height, const char *title, unsigned char fullscreen, HWND hwnd);
		HWND elf_get_window_hwnd();
	#endif
#endif
// !!>

void elf_set_title(const char *title);	// <mdoc> CONTEXT FUNCTIONS

int elf_get_window_width();
int elf_get_window_height();
int elf_get_video_mode_count();
elf_vec2i elf_get_video_mode(int idx);
unsigned char elf_is_fullscreen();
const char* elf_get_title();
int elf_get_multisamples();
double elf_get_time();
unsigned char elf_is_window_opened();
/* <!> */void elf_swap_buffers();
elf_vec2i elf_get_mouse_position();
elf_vec2i elf_get_mouse_force();
void elf_set_mouse_position(int x, int y);
void elf_hide_mouse(unsigned char hide);
unsigned char elf_is_mouse_hidden();
int elf_get_mouse_wheel();
int elf_get_mouse_button_state(int button);
int elf_get_key_state(int key);
unsigned char elf_get_joystick_present(int joy);
elf_vec2f elf_get_joystick_axis(int joy);
int elf_get_joystick_button_state(int joy, int but);
int elf_get_event_count();
elf_object* elf_get_event(int idx);
int elf_get_key_event_key(elf_key_event *key_event);
int elf_get_key_event_state(elf_key_event *key_event);

//////////////////////////////// ENGINE ////////////////////////////////

// <!!
void elf_write_to_log(const char *fmt, ...);
void elf_set_error(int code, const char *fmt, ...);
void elf_set_error_no_save(int code, const char *fmt, ...);

elf_game_config* elf_create_game_config();
void elf_destroy_game_config(elf_game_config *config);

elf_engine* elf_create_engine();
void elf_destroy_engine(elf_engine *engine);

unsigned char elf_init_engine();
void elf_deinit_engine();

#ifdef ELF_WINDOWS
	#ifndef ELF_PLAYER
		#include "windows.h"
		unsigned char elf_init_with_hwnd(int width, int height, const char *title, unsigned char fullscreen, HWND hwnd);
	#endif
#endif
// !!>

elf_game_config* elf_read_game_config(const char *file_path);	// <mdoc> GAME CONFIGURATION FUNCTIONS
int elf_get_game_config_window_width(elf_game_config *config);
int elf_get_game_config_window_height(elf_game_config *config);
unsigned char elf_get_game_config_fullscreen(elf_game_config *config);
float elf_get_game_config_texture_anisotropy(elf_game_config *config);
int elf_get_game_config_shadow_map_size(elf_game_config *config);
const char* elf_get_game_config_start(elf_game_config *config);

unsigned char elf_init(int width, int height, const char *title, unsigned char fullscreen);	// <mdoc> ENGINE FUNCTIONS
unsigned char elf_init_with_config(const char *file_path);
void elf_deinit();

void elf_resize_window(int width, int height);

const char* elf_get_platform();

int elf_get_version_major();
int elf_get_version_minor();
const char* elf_get_version_release();
const char* elf_get_version();

/* <!> */ char* elf_get_directory_from_path(const char *file_path);
const char* elf_get_current_directory();

const char* elf_get_error_string();
int elf_get_error();

unsigned char elf_run();
void elf_quit();

void elf_set_f10_exit(unsigned char exit);
unsigned char elf_get_f10_exit();

elf_scene* elf_load_scene(const char *file_path);
void elf_set_scene(elf_scene *scene);
elf_scene* elf_get_scene();

void elf_set_gui(elf_gui *gui);
elf_gui* elf_get_gui();

float elf_get_sync();
int elf_get_fps();

unsigned char elf_save_screen_shot(const char *file_path);

void elf_set_fps_limit(int fps_limit);
int elf_get_fps_limit();

void elf_set_tick_rate(float tick_rate);
float elf_get_tick_rate();

void elf_set_speed(float speed);
float elf_get_speed();

void elf_set_texture_anisotropy(float anisotropy);
float elf_get_texture_anisotropy();

void elf_set_shadow_map_size(int size);
int elf_get_shadow_map_size();

int elf_get_polygons_rendered();

void elf_set_bloom(float threshold);
void elf_disable_bloom();
float elf_get_bloom_threshold();

void elf_set_dof(float focal_range, float focal_distance);
void elf_disable_dof();
float elf_get_dof_focal_range();
float elf_get_dof_focal_distance();

void elf_set_ssao(float amount);
void elf_disable_ssao();
float elf_get_ssao_amount();

void elf_set_light_shafts(float intensity);
void elf_disable_light_shafts();
float elf_get_light_shafts_inteisity();

unsigned char elf_is_bloom();
unsigned char elf_is_ssao();
unsigned char elf_is_dof();
unsigned char elf_is_light_shafts();

void elf_set_occlusion_culling(unsigned char cull);
unsigned char elf_is_occlusion_culling();

void elf_set_debug_draw(unsigned char debug_draw);
unsigned char elf_is_debug_draw();

elf_object* elf_get_actor();

// <!!
elf_directory* elf_create_directory();
elf_directory_item* elf_create_directory_item();
void elf_destroy_directory(elf_directory *directory);
void elf_destroy_directory_item(elf_directory_item *directory_item);
// !!>

elf_directory* elf_read_directory(const char *path);

const char* elf_get_directory_path(elf_directory *directory);
int elf_get_directory_item_count(elf_directory *directory);
elf_directory_item* elf_get_directory_item(elf_directory *directory, int idx);
const char* elf_get_directory_item_name(elf_directory_item *dir_item);
int elf_get_directory_item_type(elf_directory_item *dir_item);

elf_vec3f elf_create_vec3f();
elf_vec3f elf_create_vec3f_from_values(float x, float y, float z);
elf_vec4f elf_create_qua();
elf_vec4f elf_create_qua_from_euler(float x, float y, float z);
elf_vec3f elf_mul_qua_vec3f(elf_vec4f qua, elf_vec3f vec);
elf_vec4f elf_mul_qua_qua(elf_vec4f qua1, elf_vec4f qua2);
elf_vec4f elf_get_qua_inverted(elf_vec4f qua);
elf_vec3f elf_sub_vec3f_vec3f(elf_vec3f vec1, elf_vec3f vec2);
elf_vec3f elf_add_vec3f_vec3f(elf_vec3f vec1, elf_vec3f vec2);
float elf_get_vec3f_length(elf_vec3f vec);

unsigned char elf_about_zero(float val);
float elf_float_abs(float val);
float elf_float_max(float a, float b);
float elf_random_float();
float elf_random_float_range(float min, float max);
int elf_random_int();
int elf_random_int_range(int min, int max);

//////////////////////////////// TIMER ////////////////////////////////

/* <!> */ void elf_destroy_frame_player(elf_frame_player *player);

elf_frame_player *elf_create_frame_player();	// <mdoc> FRAME PLAYER FUNCTIONS
void elf_update_frame_player(elf_frame_player *player);
void elf_set_frame_player_frame(elf_frame_player *player, float frame);
void elf_play_frame_player(elf_frame_player *player, float start, float end, float speed);
void elf_loop_frame_player(elf_frame_player *player, float start, float end, float speed);
void elf_stop_frame_player(elf_frame_player *player);
void elf_pause_frame_player(elf_frame_player *player);
void elf_resume_frame_player(elf_frame_player *player);
float elf_get_frame_player_start(elf_frame_player *player);
float elf_get_frame_player_end(elf_frame_player *player);
float elf_get_frame_player_speed(elf_frame_player *player);
float elf_get_frame_player_frame(elf_frame_player *player);
unsigned char elf_is_frame_player_playing(elf_frame_player *player);
unsigned char elf_is_frame_player_paused(elf_frame_player *player);
// <!!
void elf_set_frame_player_user_data(elf_frame_player *player, void *user_data);
void* elf_get_frame_player_user_data(elf_frame_player *player);
void elf_set_frame_player_callback(elf_frame_player *player, void (*callback)(elf_frame_player*));
// !!>

elf_timer *elf_create_timer();	// <mdoc> TIMER FUNCTIONS
/* <!> */ void elf_destroy_timer(elf_timer *timer);
void elf_start_timer(elf_timer *timer);
double elf_get_elapsed_time(elf_timer *timer);

//////////////////////////////// IMAGE ////////////////////////////////

elf_image* elf_create_image_from_file(const char *file_path);	// <mdoc> IMAGE FUNCTIONS
/* <!> */ void elf_destroy_image(elf_image *image);

int elf_get_image_width(elf_image *image);
int elf_get_image_height(elf_image *image);
int elf_get_image_bits_per_pixel(elf_image *image);
// <!!
void* elf_get_image_data(elf_image *image);
unsigned char elf_save_image_data(const char *file_path, int width, int height, unsigned char bpp, void *data);
// !!>

//////////////////////////////// TEXTURE ////////////////////////////////

// <!!
elf_texture *elf_create_texture();
void elf_destroy_texture(elf_texture *texture);
// !!>

elf_texture* elf_create_texture_from_file(const char *file_path);	// <mdoc> TEXTURE FUNCTIONS

const char* elf_get_texture_name(elf_texture *texture);
const char* elf_get_texture_file_path(elf_texture *texture);
int elf_get_texture_width(elf_texture *texture);
int elf_get_texture_height(elf_texture *texture);
int elf_get_texture_format(elf_texture *texture);
int elf_get_texture_data_format(elf_texture *texture);
// <!!
gfx_texture* elf_get_gfx_texture(elf_texture* texture);
void elf_set_texture(int slot, elf_texture *texture, gfx_shader_params *shader_params);
// !!>

//////////////////////////////// MATERIAL ////////////////////////////////

// <!!
void elf_destroy_material(elf_material *material);
// !!>

elf_material* elf_create_material(const char *name);	// <mdoc> MATERIAL FUNCTIONS

void elf_set_material_texture(elf_material *material, int slot, elf_texture *texture);
void elf_set_material_texture_type(elf_material *material, int slot, int type);
void elf_set_material_texture_parallax_scale(elf_material *material, int slot, float scale);
void elf_set_material_texture_alpha_test(elf_material *material, int slot, float test);
void elf_set_material_diffuse_color(elf_material *material, float r, float g, float b, float a);
void elf_set_material_specular_color(elf_material *material, float r, float g, float b, float a);
void elf_set_material_ambient_color(elf_material *material, float r, float g, float b, float a);
void elf_set_material_specular_power(elf_material *material, float power);
void elf_set_material_lighting(elf_material *material, unsigned char lighting);

const char* elf_get_material_name(elf_material *material);
const char* elf_get_material_file_path(elf_material *material);
elf_texture* elf_get_material_texture(elf_material *material, int slot);
int elf_get_material_texture_type(elf_material *material, int slot);
int elf_get_material_texture_count(elf_material *material);
float elf_get_material_texture_parallax_scale(elf_material *material, int slot);
float elf_get_material_texture_alpha_texture(elf_material *material, int slot);
elf_color elf_get_material_diffuse_color(elf_material *material);
elf_color elf_get_material_specular_color(elf_material *material);
elf_color elf_get_material_ambient_color(elf_material *material);
float elf_get_material_specular_power(elf_material *material);
unsigned char elf_get_material_lighting(elf_material *material);

// <!!
void elf_set_material_alpha_textures(elf_material *material, gfx_shader_params *shader_params);
void elf_set_material(elf_material *material, gfx_shader_params *shader_params);
// !!>

//////////////////////////////// IPOS ////////////////////////////////

// <!!
elf_bezier_point* elf_create_bezier_point();
void elf_destroy_bezier_point(elf_bezier_point *point);
elf_bezier_curve* elf_create_bezier_curve();
void elf_destroy_bezier_curve(elf_bezier_curve *curve);
elf_ipo* elf_create_ipo();
void elf_destroy_ipo(elf_ipo *ipo);
// !!>

void elf_add_point_to_bezier_curve(elf_bezier_curve *curve, elf_bezier_point *point);

unsigned char elf_add_curve_to_ipo(elf_ipo *ipo, elf_bezier_curve *curve);
elf_vec3f elf_get_ipo_loc(elf_ipo *ipo, float x);
elf_vec3f elf_get_ipo_rot(elf_ipo *ipo, float x);
elf_vec3f elf_get_ipo_scale(elf_ipo *ipo, float x);
elf_vec4f elf_get_ipo_qua(elf_ipo *ipo, float x);

//////////////////////////////// PROPERTY ////////////////////////////////


elf_property* elf_create_property(const char *name);
/* <!> */ void elf_destroy_property(elf_property *property);

int elf_get_property_type(elf_property *property);

int elf_get_property_int(elf_property *property);
float elf_get_property_float(elf_property *property);
const char* elf_get_property_string(elf_property *property);
unsigned char elf_get_property_bool(elf_property *property);

void elf_set_property_int(elf_property *property, int ival);
void elf_set_property_float(elf_property *property, float fval);
void elf_set_property_string(elf_property *property, const char *sval);
void elf_set_property_bool(elf_property *property, unsigned char bval);

//////////////////////////////// ACTOR ////////////////////////////////

// <!!
void elf_init_actor(elf_actor *actor, unsigned char camera);
void elf_update_actor(elf_actor *actor);
void elf_actor_pre_draw(elf_actor *actor);
void elf_actor_post_draw(elf_actor *actor);
void elf_clean_actor(elf_actor *actor);
// !!>

const char* elf_get_actor_name(elf_actor *actor);	// <mdoc> ACTOR FUNCTIONS <mdocc> Actor functions can be performed on all actor types. [Camera|Entity|Light|Armature]
const char* elf_get_actor_file_path(elf_actor *actor);
elf_script* elf_get_actor_script(elf_actor *actor);

void elf_set_actor_name(elf_actor *actor, const char *name);
void elf_set_actor_script(elf_actor *actor, elf_script *script);

void elf_set_actor_position(elf_actor *actor, float x, float y, float z);
void elf_set_actor_rotation(elf_actor *actor, float x, float y, float z);
void elf_set_actor_orientation(elf_actor *actor, float x, float y, float z, float w);
void elf_rotate_actor(elf_actor *actor, float x, float y, float z);
void elf_rotate_actor_local(elf_actor *actor, float x, float y, float z);
void elf_move_actor(elf_actor *actor, float x, float y, float z);
void elf_move_actor_local(elf_actor *actor, float x, float y, float z);
elf_vec3f elf_get_actor_position(elf_actor *actor);
elf_vec3f elf_get_actor_rotation(elf_actor *actor);
elf_vec4f elf_get_actor_orientation(elf_actor *actor);

// <!!
void elf_get_actor_position_(elf_actor *actor, float *params);
void elf_get_actor_rotation_(elf_actor *actor, float *params);
void elf_get_actor_orientation_(elf_actor *actor, float *params);
// !!>

void elf_set_actor_bounding_lengths(elf_actor *actor, float x, float y, float z);
void elf_set_actor_bounding_offset(elf_actor *actor, float x, float y, float z);
void elf_set_actor_physics(elf_actor *actor, int shape, float mass);
unsigned char elf_is_actor_physics(elf_actor *actor);
void elf_disable_actor_physics(elf_actor *actor);

void elf_set_actor_damping(elf_actor *actor, float lin_damp, float ang_damp);
void elf_set_actor_sleep_thresholds(elf_actor *actor, float lin_thrs, float ang_thrs);
void elf_set_actor_restitution(elf_actor *actor, float restitution);
void elf_set_actor_anisotropic_friction(elf_actor *actor, float x, float y, float z);
void elf_set_actor_linear_factor(elf_actor *actor, float x, float y, float z);
void elf_set_actor_angular_factor(elf_actor *actor, float x, float y, float z);

void elf_add_force_to_actor(elf_actor *actor, float x, float y, float z);
void elf_add_torque_to_actor(elf_actor *actor, float x, float y, float z);
void elf_set_actor_linear_velocity(elf_actor *actor, float x, float y, float z);
void elf_set_actor_angular_velocity(elf_actor *actor, float x, float y, float z);

elf_vec3f elf_get_actor_bounding_lengths(elf_actor *actor);
elf_vec3f elf_get_actor_bounding_offset(elf_actor *actor);
int elf_get_actor_shape(elf_actor *actor);
float elf_get_actor_mass(elf_actor *actor);

float elf_get_actor_linear_damping(elf_actor *actor);
float elf_get_actor_angular_damping(elf_actor *actor);
float elf_get_actor_linear_sleep_threshold(elf_actor *actor);
float elf_get_actor_angular_sleep_threshold(elf_actor *actor);
float elf_get_actor_restitution(elf_actor *actor);
elf_vec3f elf_get_actor_anisotropic_friction(elf_actor *actor);
elf_vec3f elf_get_actor_linear_factor(elf_actor *actor);
elf_vec3f elf_get_actor_angular_factor(elf_actor *actor);
elf_vec3f elf_get_actor_linear_velocity(elf_actor *actor);
elf_vec3f elf_get_actor_angular_velocity(elf_actor *actor);

elf_joint* elf_add_hinge_joint_to_actor(elf_actor *actor, elf_actor *actor2, const char *name, float px, float py, float pz, float ax, float ay, float az);
elf_joint* elf_add_ball_joint_to_actor(elf_actor *actor, elf_actor *actor2, const char *name, float px, float py, float pz);
elf_joint* elf_add_cone_twist_joint_to_actor(elf_actor *actor, elf_actor *actor2, const char *name, float px, float py, float pz, float ax, float ay, float az);
elf_joint* elf_get_actor_joint_by_name(elf_actor *actor, const char *name);
elf_joint* elf_get_actor_joint_by_index(elf_actor *actor, int idx);
unsigned char elf_remove_actor_joint_by_name(elf_actor *actor, const char *name);
unsigned char elf_remove_actor_joint_by_index(elf_actor *actor, int idx);
unsigned char elf_remove_actor_joint_by_object(elf_actor *actor, elf_joint *joint);

void elf_set_actor_ipo_frame(elf_actor *actor, float frame);
void elf_play_actor_ipo(elf_actor *actor, float start, float end, float speed);
void elf_loop_actor_ipo(elf_actor *actor, float start, float end, float speed);
void elf_stop_actor_ipo(elf_actor *actor);
void elf_pause_actor_ipo(elf_actor *actor);
void elf_resume_actor_ipo(elf_actor *actor);
float elf_get_actor_ipo_start(elf_actor *actor);
float elf_get_actor_ipo_end(elf_actor *actor);
float elf_get_actor_ipo_speed(elf_actor *actor);
float elf_get_actor_ipo_frame(elf_actor *actor);
unsigned char elf_is_actor_ipo_playing(elf_actor *actor);
unsigned char elf_is_actor_ipo_paused(elf_actor *actor);

int elf_get_actor_collision_count(elf_actor *actor);
elf_collision* elf_get_actor_collision(elf_actor *actor, int idx);

int elf_get_actor_property_count(elf_actor *actor);
void elf_add_property_to_actor(elf_actor *actor, elf_property *property);
elf_property* elf_get_actor_property_by_name(elf_actor *actor, const char *name);
elf_property* elf_get_actor_property_by_index(elf_actor *actor, int idx);
unsigned char elf_remove_actor_property_by_name(elf_actor *actor, const char *name);
unsigned char elf_remove_actor_property_by_index(elf_actor *actor, int idx);
unsigned char elf_remove_actor_property_by_object(elf_actor *actor, elf_property *property);
void elf_remove_actor_properties(elf_actor *actor);

/* <!> */ void elf_direct_actor_at(elf_actor *actor, elf_vec3f at, int axis);

void elf_set_actor_selected(elf_actor *actor, unsigned char selected);
unsigned char elf_get_actor_selected(elf_actor *actor);

//////////////////////////////// CAMERA ////////////////////////////////

// <!!
void elf_update_camera(elf_camera *camera);
void elf_camera_pre_draw(elf_camera *camera);
void elf_camear_post_draw(elf_camera *camera);
void elf_destroy_camera(elf_camera *camera);
// !!>

elf_camera* elf_create_camera(const char *name);	// <mdoc> CAMERA FUNCTIONS

void elf_set_camera_viewport(elf_camera *camera, int x, int y, int width, int height);
void elf_set_camera_perspective(elf_camera *camera, float fov, float aspect, float clip_near, float clip_far);
void elf_set_camera_orthographic(elf_camera *camera, int x, int y, int width, int height, float clip_near, float clip_far);

elf_vec2i elf_get_camera_viewport_size(elf_camera *camera);
elf_vec2i elf_get_camera_viewport_offset(elf_camera *camera);
float elf_get_camera_fov(elf_camera *camera);
float elf_get_camera_aspect(elf_camera *camera);
elf_vec2f elf_get_camera_clip(elf_camera *camera);
elf_vec2f elf_get_camera_far_plane_size(elf_camera *camera);

// <!!
float* elf_get_camera_projection_matrix(elf_camera *camera);
float* elf_get_camera_modelview_matrix(elf_camera *camera);
void elf_set_camera(elf_camera *camera, gfx_shader_params *shader_params);
unsigned char elf_aabb_inside_frustum(elf_camera *camera, float *min, float *max);
unsigned char elf_sphere_inside_frustum(elf_camera *camera, float *pos, float radius);
unsigned char elf_camera_inside_aabb(elf_camera *camera, float *min, float *max);
unsigned char elf_camera_inside_sphere(elf_camera *camera, float *pos, float radius);
void elf_draw_camera_debug(elf_camera *camera, gfx_shader_params *shader_params);
// !!>

elf_vec3f elf_un_project_camera_point(elf_camera *camera, float x, float y, float z);

//////////////////////////////// MODEL ////////////////////////////////

// <!!
void elf_destroy_model(elf_model *model);
void elf_generate_model_tangent_vectors(elf_model *model);
// !!>

const char* elf_get_model_name(elf_model *model);	// <mdoc> MODEL FUNCTIONS
const char* elf_get_model_file_path(elf_model *model);
int elf_get_model_vertice_count(elf_model *model);
int elf_get_model_indice_count(elf_model *model);
elf_vec3f elf_get_model_bounding_box_min(elf_model *model);
elf_vec3f elf_get_model_bounding_box_max(elf_model *model);

// <!!
float* elf_get_model_vertices(elf_model *model);
float* elf_get_model_normals(elf_model *model);
float* elf_get_model_tex_coords(elf_model *model);
float* elf_get_model_tangents(elf_model *model);
unsigned int* elf_get_model_indices(elf_model *model);

void elf_draw_model(elf_list *material, elf_model *model, gfx_shader_params *shader_params, unsigned char *non_lit_flag);
void elf_draw_model_ambient(elf_list *material, elf_model *model, gfx_shader_params *shader_params);
void elf_draw_model_without_materials(elf_list *materials, elf_model *model, gfx_shader_params *shader_params);
void elf_draw_model_bouding_box(elf_model *model, gfx_shader_params *shader_params);
// !!>

//////////////////////////////// ENTITY ////////////////////////////////

// <!!
void elf_update_entity(elf_entity *entity);
void elf_entity_pre_draw(elf_entity *entity);
void elf_entity_post_draw(elf_entity *entity);
void elf_destroy_entity(elf_entity *entity);
void elf_calc_entity_aabb(elf_entity *entity);
void elf_calc_entity_bounding_volumes(elf_entity *entity, unsigned char new_model);
// !!>

elf_entity* elf_create_entity(const char *name);	// <mdoc> ENTITY FUNCTIONS

void elf_set_entity_scale(elf_entity *entity, float x, float y, float z);
elf_vec3f elf_get_entity_scale(elf_entity *entity);
/* <!> */ void elf_get_entity_scale_(elf_entity *entity, float *scale);

void elf_set_entity_model(elf_entity *entity, elf_model *model);
elf_model* elf_get_entity_model(elf_entity *entity);
int elf_get_entity_material_count(elf_entity *entity);
void elf_add_entity_material(elf_entity *entity, elf_material *material);
void elf_set_entity_material(elf_entity *entity, int idx, elf_material *material);
elf_material* elf_get_entity_material(elf_entity *entity, int idx);

void elf_set_entity_visible(elf_entity *entity, unsigned char visible);
unsigned char elf_get_entity_visible(elf_entity *entity);

void elf_set_entity_physics(elf_entity *entity, int type, float mass);
void elf_disable_entity_physics(elf_entity *entity);

void elf_set_entity_armature(elf_entity *entity, elf_armature *armature);
void elf_set_entity_armature_frame(elf_entity *entity, float frame);
void elf_play_entity_armature(elf_entity *entity, float start, float end, float speed);
void elf_loop_entity_armature(elf_entity *entity, float start, float end, float speed);
void elf_stop_entity_armature(elf_entity *entity);
void elf_pause_entity_armature(elf_entity *entity);
void elf_resume_entity_armature(elf_entity *entity);
float elf_get_entity_armature_start(elf_entity *entity);
float elf_get_entity_armature_end(elf_entity *entity);
float elf_get_entity_armature_speed(elf_entity *entity);
float elf_get_entity_armature_frame(elf_entity *entity);
unsigned char elf_is_entity_armature_playing(elf_entity *entity);
unsigned char elf_is_entity_armature_paused(elf_entity *entity);

elf_armature* elf_get_entity_armature(elf_entity *entity);

// <!!
void elf_reset_entity_debug_physics_object(elf_entity *entity);
void elf_draw_entity(elf_entity *entity, gfx_shader_params *shader_params);
void elf_draw_entity_ambient(elf_entity *entity, gfx_shader_params *shader_params);
void elf_draw_entity_without_materials(elf_entity *entity, gfx_shader_params *shader_params);
void elf_draw_entity_bounding_box(elf_entity *entity, gfx_shader_params *shader_params);
void elf_draw_entity_debug(elf_entity *entity, gfx_shader_params *shader_params);
unsigned char elf_cull_entity(elf_entity *entity, elf_camera *camera);
// !!>

unsigned char elf_get_entity_changed(elf_entity *entity);

//////////////////////////////// LIGHT ////////////////////////////////

// <!!
void elf_update_light(elf_light *light);
void elf_light_pre_draw(elf_light *light);
void elf_light_post_draw(elf_light *light);
void elf_destroy_light(elf_light *light);
// !!>

elf_light* elf_create_light(const char *name);	// <mdoc> LIGHT FUNCTIONS

int elf_get_light_type(elf_light *light);
elf_color elf_get_light_color(elf_light *light);
float elf_get_light_distance(elf_light *light);
float elf_get_light_fade_speed(elf_light *light);
unsigned char elf_get_light_shadow_caster(elf_light *light);
unsigned char elf_get_light_visible(elf_light *light);
elf_vec2f elf_get_light_cone(elf_light *light);
unsigned char elf_is_light_shaft(elf_light *light);
float elf_get_light_shaft_size(elf_light *light);
float elf_get_light_shaft_intensity(elf_light *light);
float elf_get_light_shaft_fade_off(elf_light *light);

void elf_set_light_type(elf_light *light, int type);
void elf_set_light_color(elf_light *light, float r, float g, float b, float a);
void elf_set_light_distance(elf_light *light, float distance);
void elf_set_light_fade_speed(elf_light *light, float fade_speed);
void elf_set_light_shadow_caster(elf_light *light, unsigned char shadow_caster);
void elf_set_light_visible(elf_light *light, unsigned char visible);
void elf_set_light_cone(elf_light *light, float inner_cone, float outer_cone);
void elf_set_light_shaft(elf_light *light, float size, float intensity, float fade_off);
void elf_disable_light_shaft(elf_light *light);

// <!!
void elf_set_light(elf_light *light, elf_camera *camera, gfx_shader_params *shader_params);
void elf_draw_light_debug(elf_light *light, gfx_shader_params *shader_params);
unsigned char elf_get_light_changed(elf_light *light);
// !!>

//////////////////////////////// ARMATURE ////////////////////////////////

// <!!
elf_bone* elf_create_bone(const char *name);
void elf_destroy_bone(elf_bone *bone);
// !!>

elf_armature* elf_get_bone_armature(elf_bone *bone);	// <mdoc> BONE FUNCTIONS
elf_bone* elf_get_bone_parent(elf_bone *bone);
elf_bone* elf_get_bone_child_by_name(elf_bone *bone, const char *name);
elf_bone* elf_get_bone_child_by_id(elf_bone *bone, int id);
elf_bone* elf_get_bone_child_by_index(elf_bone *bone, int idx);
elf_vec3f elf_get_bone_position(elf_bone *bone);
elf_vec3f elf_get_bone_rotation(elf_bone *bone);
elf_vec4f elf_get_bone_orientation(elf_bone *bone);

// <!!
void elf_update_armature(elf_armature *armature);
void elf_destroy_armature(elf_armature *armature);
// !!>

elf_armature* elf_create_armature(const char *name);	// <mdoc> ARMATURE FUNCTIONS

elf_bone* elf_get_bone_from_armature_by_name(const char *name, elf_armature *armature);
elf_bone* elf_get_bone_from_armature_by_id(int id, elf_armature *armature);

// <!!
void elf_add_root_bone_to_armature(elf_armature *armature, elf_bone *bone);

void elf_deform_entity_with_armature(elf_armature *armature, elf_entity *entity, float frame);
void elf_draw_armature_debug(elf_armature *armature, gfx_shader_params *shader_params);
// !!>

//////////////////////////////// PARTICLES ////////////////////////////////

// <!!
elf_particle* elf_create_particle();
void elf_destroy_particle(elf_particle *particle);
void elf_update_particles(elf_particles *particles, float sync);
void elf_destroy_particles(elf_particles *particles);
// !!>

elf_particles* elf_create_particles(const char *name, int max_count);	// <mdoc> PARTICLES FUNCTIONS

const char* elf_get_particles_name(elf_particles *particles);
const char* elf_get_particles_file_path(elf_particles *particles);

void elf_set_particles_max_count(elf_particles *particles, int max_count);
void elf_set_particles_draw_mode(elf_particles *particles, int mode);
void elf_set_particles_texture(elf_particles *particles, elf_texture *texture);
void elf_set_particles_model(elf_particles *particles, elf_model *model);
void elf_set_particles_entity(elf_particles *particles, elf_entity *entity);
void elf_set_particles_gravity(elf_particles *particles, float x, float y, float z);
void elf_set_particles_spawn_delay(elf_particles *particles, float delay);
void elf_set_particles_spawn(elf_particles *particles, unsigned char spawn);

void elf_set_particles_size(elf_particles *particles, float min, float max);
void elf_set_particles_size_growth(elf_particles *particles, float min, float max);
void elf_set_particles_rotation(elf_particles *particles, float min, float max);
void elf_set_particles_rotation_growth(elf_particles *particles, float min, float max);
void elf_set_particles_life_span(elf_particles *particles, float min, float max);
void elf_set_particles_fade_speed(elf_particles *particles, float min, float max);
void elf_set_particles_position_min(elf_particles *particles, float x, float y, float z);
void elf_set_particles_position_max(elf_particles *particles, float x, float y, float z);
void elf_set_particles_velocity_min(elf_particles *particles, float x, float y, float z);
void elf_set_particles_velocity_max(elf_particles *particles, float x, float y, float z);
void elf_set_particles_color_min(elf_particles *particles, float r, float g, float b, float a);
void elf_set_particles_color_max(elf_particles *particles, float r, float g, float b, float a);

int elf_get_particles_max_count(elf_particles *particles);
int elf_get_particles_count(elf_particles *particles);
int elf_get_particles_draw_mode(elf_particles *particles);
elf_texture* elf_get_particles_texture(elf_particles *particles);
elf_model* elf_get_particles_model(elf_particles *particles);
elf_entity* elf_get_particles_entity(elf_particles *particles);
elf_vec3f elf_get_particles_gravity(elf_particles *particles);
float elf_get_particles_spawn_delay(elf_particles *particles);
unsigned char elf_get_particles_spawn(elf_particles *particles);

float elf_get_particles_size_min(elf_particles *particles);
float elf_get_particles_size_max(elf_particles *particles);
float elf_get_particles_size_growth_min(elf_particles *particles);
float elf_get_particles_size_growth_max(elf_particles *particles);
float elf_get_particles_rotation_min(elf_particles *particles);
float elf_get_particles_rotation_max(elf_particles *particles);
float elf_get_particles_rotation_growth_min(elf_particles *particles);
float elf_get_particles_rotation_growth_max(elf_particles *particles);
float elf_get_particles_life_span_min(elf_particles *particles);
float elf_get_particles_life_span_max(elf_particles *particles);
float elf_get_particles_fade_speed_min(elf_particles *particles);
float elf_get_particles_fade_speed_max(elf_particles *particles);
elf_vec3f elf_get_particles_position_min(elf_particles *particles);
elf_vec3f elf_get_particles_position_max(elf_particles *particles);
elf_vec3f elf_get_particles_velocity_min(elf_particles *particles);
elf_vec3f elf_get_particles_velocity_max(elf_particles *particles);
elf_color elf_get_particles_color_min(elf_particles *particles);
elf_color elf_get_particles_color_max(elf_particles *particles);

// <!!
void elf_draw_particles(elf_particles *particles, elf_camera *camera, gfx_shader_params *shader_params);
// !!>

//////////////////////////////// SPRITE ////////////////////////////////

// <!!
void elf_update_sprite(elf_sprite *sprite);
void elf_sprite_pre_draw(elf_sprite *sprite, elf_camera *camera);
void elf_sprite_post_draw(elf_sprite *sprite);
void elf_destroy_sprite(elf_sprite *sprite);
// !!>

elf_sprite* elf_create_sprite(const char *name);

void elf_set_sprite_material(elf_sprite *sprite, elf_material *material);
void elf_set_sprite_scale(elf_sprite *sprite, float x, float y);
void elf_set_sprite_face_camera(elf_sprite *sprite, unsigned char face_camera);

elf_material* elf_get_sprite_material(elf_sprite *sprite);
elf_vec2f elf_get_sprite_scale(elf_sprite *sprite);
unsigned char elf_get_sprite_face_camera(elf_sprite *sprite);

// <!!
unsigned char elf_cull_sprite(elf_sprite *sprite, elf_camera *camera);
void elf_draw_sprite(elf_sprite *sprite, gfx_shader_params *shader_params);
void elf_draw_sprite_without_materials(elf_sprite *sprite, gfx_shader_params *shader_params);
void elf_draw_sprite_ambient(elf_sprite *sprite, gfx_shader_params *shader_params);
void elf_draw_sprite_debug(elf_sprite *sprite, gfx_shader_params *shader_params);
// !!>

//////////////////////////////// SCENE ////////////////////////////////

// <!!
void elf_update_scene(elf_scene *scene, float sync);
void elf_scene_pre_draw(elf_scene *scene);
void elf_scene_post_draw(elf_scene *scene);
void elf_destroy_scene(elf_scene *scene);
// !!>

elf_scene* elf_create_scene(const char *name);
elf_scene* elf_create_scene_from_file(const char *file_path);	// <mdoc> SCENE FUNCTIONS
unsigned char elf_save_scene(elf_scene *scene, const char *file_path);

void elf_set_scene_ambient_color(elf_scene *scene, float r, float g, float b, float a);
elf_color elf_get_scene_ambient_color(elf_scene *scene);

void elf_set_scene_gravity(elf_scene *scene, float x, float y, float z);
elf_vec3f elf_get_scene_gravity(elf_scene *scene);

void elf_set_scene_physics(elf_scene *scene, unsigned char physics);
unsigned char elf_get_scene_physics(elf_scene *scene);

void elf_set_scene_run_scripts(elf_scene *scene, unsigned char run_scripts);
unsigned char elf_get_scene_run_scripts(elf_scene *scene, unsigned char run_scripts);

const char* elf_get_scene_name(elf_scene *scene);
const char* elf_get_scene_file_path(elf_scene *scene);

int elf_get_scene_camera_count(elf_scene *scene);
int elf_get_scene_entity_count(elf_scene *scene);
int elf_get_scene_light_count(elf_scene *scene);
int elf_get_scene_armature_count(elf_scene *scene);
int elf_get_scene_particles_count(elf_scene *scene);
int elf_get_scene_sprite_count(elf_scene *scene);

void elf_add_camera_to_scene(elf_scene *scene, elf_camera *camera);
void elf_add_entity_to_scene(elf_scene *scene, elf_entity *entity);
void elf_add_light_to_scene(elf_scene *scene, elf_light *light);
void elf_add_particles_to_scene(elf_scene *scene, elf_particles *particles);
void elf_add_sprite_to_scene(elf_scene *scene, elf_sprite *sprite);

void elf_set_scene_active_camera(elf_scene *scene, elf_camera *camera);
elf_camera* elf_get_scene_active_camera(elf_scene *scene);
elf_collision* elf_get_scene_ray_cast_result(elf_scene *scene, float x, float y, float z, float dx, float dy, float dz);
elf_list* elf_get_scene_ray_cast_results(elf_scene *scene, float x, float y, float z, float dx, float dy, float dz);
elf_collision* elf_get_debug_scene_ray_cast_result(elf_scene *scene, float x, float y, float z, float dx, float dy, float dz);
elf_list* elf_get_debug_scene_ray_cast_results(elf_scene *scene, float x, float y, float z, float dx, float dy, float dz);

elf_camera* elf_get_camera_by_index(elf_scene *scene, int idx);
elf_entity* elf_get_entity_by_index(elf_scene *scene, int idx);
elf_light* elf_get_light_by_index(elf_scene *scene, int idx);
elf_armature* elf_get_armature_by_index(elf_scene *scene, int idx);
elf_particles* elf_get_particles_by_index(elf_scene *scene, int idx);
elf_sprite* elf_get_sprite_by_index(elf_scene *scene, int idx);

elf_texture *elf_get_texture_by_name(elf_scene *scene, const char *name);
elf_material *elf_get_material_by_name(elf_scene *scene, const char *name);
elf_model* elf_get_model_by_name(elf_scene *scene, const char *name);
elf_script *elf_get_script_by_name(elf_scene *scene, const char *name);
elf_camera* elf_get_camera_by_name(elf_scene *scene, const char *name);
elf_entity* elf_get_entity_by_name(elf_scene *scene, const char *name);
elf_light* elf_get_light_by_name(elf_scene *scene, const char *name);
elf_armature* elf_get_armature_by_name(elf_scene *scene, const char *name);
elf_particles* elf_get_particles_by_name(elf_scene *scene, const char *name);
elf_sprite* elf_get_sprite_by_name(elf_scene *scene, const char *name);
elf_actor *elf_get_actor_by_name(elf_scene *scene, const char *name);

// <!!
elf_texture* elf_get_or_load_texture_by_name(elf_scene *scene, const char *name);
elf_model* elf_get_or_load_model_by_name(elf_scene *scene, const char *name);
elf_script* elf_get_or_load_script_by_name(elf_scene *scene, const char *name);
elf_camera* elf_get_or_load_camera_by_name(elf_scene *scene, const char *name);
elf_entity* elf_get_or_load_entity_by_name(elf_scene *scene, const char *name);
elf_particles *elf_get_or_load_particles_by_name(elf_scene *scene, const char *name);
elf_sprite *elf_get_or_load_sprite_by_name(elf_scene *scene, const char *name);
elf_light* elf_get_or_load_light_by_name(elf_scene *scene, const char *name);
elf_armature* elf_get_or_load_armature_by_name(elf_scene *scene, const char *name);
elf_material *elf_get_or_load_material_by_name(elf_scene *scene, const char *name);
elf_actor *elf_get_or_load_actor_by_name(elf_scene *scene, const char *name);

void elf_remove_actor(elf_actor *actor);
// !!>
unsigned char elf_remove_camera_by_name(elf_scene *scene, const char *name);
unsigned char elf_remove_entity_by_name(elf_scene *scene, const char *name);
unsigned char elf_remove_light_by_name(elf_scene *scene, const char *name);
unsigned char elf_remove_particles_by_name(elf_scene *scene, const char *name);
unsigned char elf_remove_sprite_by_name(elf_scene *scene, const char *name);

unsigned char elf_remove_camera_by_index(elf_scene *scene, int idx);
unsigned char elf_remove_entity_by_index(elf_scene *scene, int idx);
unsigned char elf_remove_light_by_index(elf_scene *scene, int idx);
unsigned char elf_remove_particles_by_index(elf_scene *scene, int idx);
unsigned char elf_remove_sprite_by_index(elf_scene *scene, int idx);

unsigned char elf_remove_camera_by_object(elf_scene *scene, elf_camera *camera);
unsigned char elf_remove_entity_by_object(elf_scene *scene, elf_entity *entity);
unsigned char elf_remove_light_by_object(elf_scene *scene, elf_light *light);
unsigned char elf_remove_particles_by_object(elf_scene *scene, elf_particles *particles);
unsigned char elf_remove_sprite_by_object(elf_scene *scene, elf_sprite *sprite);

unsigned char elf_remove_actor_by_object(elf_scene *scene, elf_actor *actor);

// <!!
void elf_draw_scene(elf_scene *scene);
void elf_draw_scene_debug(elf_scene *scene);
// !!>

//////////////////////////////// PAK ////////////////////////////////

// <!!
void elf_destroy_pak_index(elf_pak_index *index);
elf_pak* elf_create_pak_from_file(const char *file_path);
void elf_destroy_pak(elf_pak *pak);

const char* elf_get_pak_file_path(elf_pak *pak);
int elf_get_pak_index_count(elf_pak *pak);

elf_pak_index* elf_get_pak_index_by_name(elf_pak *pak, const char *name, unsigned char type);
elf_pak_index* elf_get_pak_index_by_index(elf_pak *pak, int idx);
unsigned char elf_get_pak_index_type(elf_pak_index *index);
const char* elf_get_pak_index_name(elf_pak_index *index);
int elf_get_pak_index_offset(elf_pak_index *index);

int elf_get_actor_header_size_bytes(elf_actor *actor);
int elf_get_armature_size_bytes(elf_armature *armature);
int elf_get_camera_size_bytes(elf_camera *camera);
int elf_get_entity_size_bytes(elf_entity *entity);
int elf_get_light_size_bytes(elf_light *light);
int elf_get_material_size_bytes(elf_material *material);
int elf_get_model_size_bytes(elf_model *model);
int elf_get_particles_size_bytes(elf_particles *particles);
int elf_get_scene_size_bytes(elf_scene *scene);
int elf_get_script_size_bytes(elf_script *script);
int elf_get_sprite_size_bytes(elf_sprite *sprite);
int elf_get_texture_size_bytes(elf_texture *texture);

void elf_read_actor_header(elf_actor *actor, FILE *file, elf_scene *scene);
elf_armature* elf_create_armature_from_pak(FILE *file, const char *name, elf_scene *scene);
elf_camera* elf_create_camera_from_pak(FILE *file, const char *name, elf_scene *scene);
elf_entity* elf_create_entity_from_pak(FILE *file, const char *name, elf_scene *scene);
elf_light* elf_create_light_from_pak(FILE *file, const char *name, elf_scene *scene);
elf_material* elf_create_material_from_pak(FILE *file, const char *name, elf_scene *scene);
elf_model* elf_create_model_from_pak(FILE *file, const char *name, elf_scene *scene);
elf_particles* elf_create_particles_from_pak(FILE *file, const char *name, elf_scene *scene);
elf_script* elf_create_script_from_pak(FILE *file, const char *name, elf_scene *scene);
elf_sprite* elf_create_sprite_from_pak(FILE *file, const char *name, elf_scene *scene);
elf_texture *elf_create_texture_from_pak(FILE *file, const char *name, elf_scene *scene);
unsigned char elf_load_texture_data_from_pak(elf_texture *texture);

elf_scene *elf_create_scene_from_pak(elf_pak *pak);

void elf_write_actor_header(elf_actor *actor, FILE *file);
void elf_write_armature_to_file(elf_armature *armature, FILE *file);
void elf_write_camera_to_file(elf_camera *camera, FILE *file);
void elf_write_entity_to_file(elf_entity *entity, FILE *file);
void elf_write_light_to_file(elf_light *light, FILE *file);
void elf_write_material_to_file(elf_material *material, FILE *file);
void elf_write_model_to_file(elf_model *model, FILE *file);
void elf_write_particles_to_file(elf_particles *particles, FILE *file);
void elf_write_script_to_file(elf_script *script, FILE *file);
void elf_write_sprite_to_file(elf_sprite *sprite, FILE *file);
void elf_write_texture_to_file(elf_texture *texture, FILE *file);

unsigned char elf_save_scene_to_pak(elf_scene *scene, const char *file_path);
// !!>

//////////////////////////////// POST PROCESS ////////////////////////////////

// <!!
elf_post_process* elf_create_post_process();
void elf_destroy_post_process(elf_post_process *post_process);

void elf_init_post_process_buffers(elf_post_process *post_process);

void elf_begin_post_process(elf_post_process *post_process, elf_scene *scene);
void elf_end_post_process(elf_post_process *post_process, elf_scene *scene);

void elf_set_post_process_bloom(elf_post_process *post_process, float threshold);
void elf_disable_post_process_bloom(elf_post_process *post_process);
float elf_get_post_process_bloom_threshold(elf_post_process *post_process);

void elf_set_post_process_dof(elf_post_process *post_process, float focal_range, float focal_distance);
void elf_disable_post_process_dof(elf_post_process *post_process);
float elf_get_post_process_dof_focal_range(elf_post_process *post_process);
float elf_get_post_process_dof_focal_distance(elf_post_process *post_process);

void elf_set_post_process_ssao(elf_post_process *post_process, float amount);
void elf_disable_post_process_ssao(elf_post_process *post_process);
float elf_get_post_process_ssao_amount(elf_post_process *post_process);

void elf_set_post_process_light_shafts(elf_post_process *post_process, float intensity);
void elf_disable_post_process_light_shafts(elf_post_process *post_process);
float elf_get_post_process_light_shafts_intensity(elf_post_process *post_process);

unsigned char elf_is_post_process_bloom(elf_post_process *post_process);
unsigned char elf_is_post_process_ssao(elf_post_process *post_process);
unsigned char elf_is_post_process_dof(elf_post_process *post_process);
unsigned char elf_is_post_process_light_shafts(elf_post_process *post_process);
// !!>

//////////////////////////////// SCRIPT ////////////////////////////////

// <!!
void elf_destroy_script(elf_script *script);
// !!>

elf_script* elf_create_script();	// <mdoc> SCRIPT FUNCTIONS
elf_script* elf_create_script_from_file(const char *file_path);

const char* elf_get_script_name(elf_script *script);
const char* elf_get_script_file_path(elf_script *script);

void elf_set_script_text(elf_script *script, const char *text);
unsigned char elf_is_script_error(elf_script *script);

//////////////////////////////// SCRIPTING ////////////////////////////////

// <!!
elf_scripting* elf_create_scripting();
void elf_destroy_scripting(elf_scripting *scripting);

unsigned char elf_init_scripting();
void elf_update_scripting();
void elf_deinit_scripting();

int elf_get_current_script_line();
elf_script* elf_get_current_script();
// !!>

unsigned char elf_run_string(const char *str);
unsigned char elf_run_script(elf_script *script);

//////////////////////////////// AUDIO ////////////////////////////////

// <!!
elf_audio_device* elf_create_audio_device();
void elf_destroy_audio_device(elf_audio_device *device);
unsigned char elf_init_audio();
void elf_update_audio();
void elf_deinit_audio();
// !!>
void elf_set_audio_volume(float volume);	// <mdoc> AUDIO FUNCTIONS
float elf_get_audio_volume();
void elf_set_audio_rolloff(float rolloff);
float elf_get_audio_rolloff();
// <!!
void elf_set_audio_listener_position(float x, float y, float z);
void elf_set_audio_listener_orientation(float *params);
// !!>

// <!!
elf_sound* elf_create_sound();
void elf_destroy_sound(elf_sound *sound);
// !!>

elf_sound* elf_load_sound(const char *file_path);
elf_sound* elf_load_streamed_sound(const char *file_path);
int elf_get_sound_file_type(elf_sound *sound);

elf_audio_source* elf_play_sound(elf_sound *sound, float volume);
elf_audio_source* elf_play_entity_sound(elf_entity *entity, elf_sound *sound, float volume);
elf_audio_source* elf_loop_sound(elf_sound *sound, float volume);
elf_audio_source* elf_loop_entity_sound(elf_entity *entity, elf_sound *sound, float volume);
// <!!
elf_audio_source* elf_create_audio_source();
void elf_stream_audio_source(elf_audio_source *source);
void elf_destroy_audio_source(elf_audio_source *source);
// !!>
void elf_set_sound_volume(elf_audio_source *source, float volume);
float elf_get_sound_volume(elf_audio_source *source);
void elf_pause_sound(elf_audio_source *source);
void elf_resume_sound(elf_audio_source *source);
void elf_stop_sound(elf_audio_source *source);

unsigned char elf_is_sound_playing(elf_audio_source *source);
unsigned char elf_is_sound_paused(elf_audio_source *source);
// <!!
void elf_set_sound_position(elf_audio_source *source, float x, float y, float z);
// !!>

//////////////////////////////// PHYSICS ////////////////////////////////

// <!!
elf_collision* elf_create_collision();
void elf_destroy_collision(elf_collision *collision);

elf_physics_world* elf_create_physics_world();
void elf_destroy_physics_world(elf_physics_world *world);
void elf_update_physics_world(elf_physics_world *world, float time);

void elf_set_physics_world_gravity(elf_physics_world *world, float x, float y, float z);
elf_vec3f elf_get_physics_world_gravity(elf_physics_world *world);

elf_collision* elf_get_ray_cast_result(elf_physics_world *world, float x, float y, float z, float dx, float dy, float dz);
elf_list* elf_get_ray_cast_results(elf_physics_world *world, float x, float y, float z, float dx, float dy, float dz);
// !!>

elf_actor* elf_get_collision_actor(elf_collision *collision);	// <mdoc> COLLISION FUNCTIONS
elf_vec3f elf_get_collision_position(elf_collision *collision);
elf_vec3f elf_get_collision_normal(elf_collision *collision);
float elf_get_collision_depth(elf_collision *collision);

// <!!
elf_joint* elf_create_joint();
void elf_set_joint_world(elf_joint *joint, elf_physics_world *world);
elf_joint* elf_create_hinge_joint(elf_actor *actor1, elf_actor *actor2, const char *name, float px, float py, float pz, float ax, float ay, float az);
elf_joint* elf_create_ball_joint(elf_actor *actor1, elf_actor *actor2, const char *name, float px, float py, float pz);
elf_joint* elf_create_cone_twist_joint(elf_actor *actor1, elf_actor *actor2, const char *name, float px, float py, float pz, float ax, float ay, float az);
void elf_clear_joint(elf_joint *joint);
void elf_destroy_joint(elf_joint *joint);
// !!>

const char* elf_get_joint_name(elf_joint *joint);	// <mdoc> JOINT FUNCTIONS
int elf_get_joint_type(elf_joint *joint);
elf_actor* elf_get_joint_actor_a(elf_joint *joint);
elf_actor* elf_get_joint_actor_b(elf_joint *joint);
elf_vec3f elf_get_joint_pivot(elf_joint *joint);
elf_vec3f elf_get_joint_axis(elf_joint *joint);

// <!!
elf_physics_tri_mesh* elf_create_physics_tri_mesh(float *verts, unsigned int *idx, int indice_count);
void elf_destroy_physics_tri_mesh(elf_physics_tri_mesh *tri_mesh);

elf_physics_object* elf_create_physics_object();
elf_physics_object* elf_create_physics_object_mesh(elf_physics_tri_mesh *tri_mesh, float mass);
elf_physics_object* elf_create_physics_object_sphere(float radius, float mass, float ox, float oy, float oz);
elf_physics_object* elf_create_physics_object_box(float hx, float hy, float hz, float mass, float ox, float oy, float oz);
elf_physics_object* elf_create_physics_object_capsule(float length, float radius, float mass, float ox, float oy, float oz);
void elf_set_physics_object_world(elf_physics_object *object, elf_physics_world *world);
void elf_destroy_physics_object(elf_physics_object *object);

void elf_set_physics_object_actor(elf_physics_object *object, elf_actor *actor);
void elf_remove_physics_object_collisions(elf_physics_object *objects);
void elf_clear_physics_object_collisions(elf_physics_object *objects);

int elf_get_physics_object_collision_count(elf_physics_object *object);
elf_collision* elf_get_physics_object_collision(elf_physics_object *object, int idx);

void elf_set_physics_object_position(elf_physics_object *object, float x, float y, float z);
void elf_set_physics_object_orientation(elf_physics_object *object, float x, float y, float z, float w);
void elf_set_physics_object_scale(elf_physics_object *object, float x, float y, float z);

int elf_get_physics_object_shape(elf_physics_object *object);
float elf_get_physics_object_mass(elf_physics_object *object);
void elf_get_physics_object_position(elf_physics_object *object, float *params);
void elf_get_physics_object_orientation(elf_physics_object *object, float *params);
void elf_get_physics_object_scale(elf_physics_object *object, float *params);
unsigned char elf_is_physics_object_static(elf_physics_object *object);

void elf_set_physics_object_damping(elf_physics_object *object, float lin_damp, float ang_damp);
void elf_set_physics_object_anisotropic_friction(elf_physics_object *object, float x, float y, float z);
void elf_set_physics_object_sleep_thresholds(elf_physics_object *object, float lin_thrs, float ang_thrs);
void elf_set_physics_object_restitution(elf_physics_object *object, float restitution);
void elf_add_force_to_physics_object(elf_physics_object *object, float x, float y, float z);
void elf_add_torque_to_physics_object(elf_physics_object *object, float x, float y, float z);
void elf_set_physics_object_linear_velocity(elf_physics_object *object, float x, float y, float z);
void elf_set_physics_object_angular_velocity(elf_physics_object *object, float x, float y, float z);
void elf_set_physics_object_linear_factor(elf_physics_object *object, float x, float y, float z);
void elf_set_physics_object_angular_factor(elf_physics_object *object, float x, float y, float z);

void elf_get_physics_object_linear_velocity(elf_physics_object *object, float *params);
void elf_get_physics_object_angular_velocity(elf_physics_object *object, float *params);
void elf_get_physics_object_linear_factor(elf_physics_object *object, float *params);
void elf_get_physics_object_angular_factor(elf_physics_object *object, float *params);
float elf_get_physics_object_linear_damping(elf_physics_object *object);
float elf_get_physics_object_angular_damping(elf_physics_object *object);
void elf_get_physics_object_anisotropic_friction(elf_physics_object *object, float *params);
float elf_get_physics_object_linear_sleep_threshold(elf_physics_object *object);
float elf_get_physics_object_angular_sleep_threshold(elf_physics_object *object);
float elf_get_physics_object_restitution(elf_physics_object *object);
// !!>

//////////////////////////////// FONT ////////////////////////////////

// <!!
elf_font* elf_create_font();
void elf_destroy_font(elf_font *font);
// !!>

elf_font* elf_create_font_from_file(const char *file_path, int size);	// <mdoc> FONT FUNCTIONS

const char* elf_get_font_name(elf_font *font);
const char* elf_get_font_file_path(elf_font *font);
int elf_get_font_size(elf_font *font);
int elf_get_string_width(elf_font *font, const char *str);
int elf_get_string_height(elf_font *font, const char *str);

/* <!> */ void elf_draw_string(elf_font *font, const char *str, int x, int y, gfx_shader_params *shader_params);

//////////////////////////////// GUI ////////////////////////////////

const char* elf_get_gui_object_name(elf_gui_object *object);	// <mdoc> GUI OBJECT FUNCTIONS <mdocc> Gui object functions can be performed on all gui object types [Labels|Buttons|Pictures|etc...]
elf_vec2i elf_get_gui_object_position(elf_gui_object *object);
elf_vec2i elf_get_gui_object_size(elf_gui_object *object);
elf_color elf_get_gui_object_color(elf_gui_object *object);
unsigned char elf_get_gui_object_visible(elf_gui_object *object);
elf_script *elf_get_gui_object_script(elf_gui_object *object);
int elf_get_gui_object_event(elf_gui_object *object);

void elf_set_gui_object_position(elf_gui_object *object, float x, float y);
void elf_set_gui_object_color(elf_gui_object *object, float r, float g, float b, float a);
void elf_set_gui_object_visible(elf_gui_object *object, unsigned char visible);
void elf_set_gui_object_script(elf_gui_object *object, elf_script *script);

// <!!
void elf_destroy_label(elf_label *label);
void elf_draw_label(elf_label *label, gfx_shader_params *shader_params);
void elf_recalc_label(elf_label *label);
// !!>

elf_label* elf_create_label(const char *name);	// <mdoc> LABEL FUNCTIONS

elf_font* elf_get_label_font(elf_label *label);
const char* elf_get_label_text(elf_label *label);

void elf_set_label_font(elf_label *label, elf_font *font);
void elf_set_label_text(elf_label *label, const char *text);

// <!!
void elf_destroy_button(elf_button *button);
void elf_draw_button(elf_button *button, gfx_shader_params *shader_params);
void elf_recalc_button(elf_button *button);
// !!>

elf_button* elf_create_button(const char *name);	// <mdoc> BUTTON FUNCTIONS

unsigned char elf_get_button_state(elf_button *button);
elf_texture* elf_get_button_off_texture(elf_button *button);
elf_texture* elf_get_button_over_texture(elf_button *button);
elf_texture* elf_get_button_on_texture(elf_button *button);

void elf_set_button_off_texture(elf_button *button, elf_texture *off);
void elf_set_button_over_texture(elf_button *button, elf_texture *over);
void elf_set_button_on_texture(elf_button *button, elf_texture *on);

// <!!
void elf_destroy_picture(elf_picture *picture);
void elf_draw_picture(elf_picture *picture, gfx_shader_params *shader_params);
void elf_recalc_picture(elf_picture *picture);
// !!>

elf_picture* elf_create_picture(const char *name);	// <mdoc> PICTURE FUNCTIONS

elf_texture* elf_get_picture_texture(elf_picture *picture);
elf_vec2f elf_get_picture_scale(elf_picture *picture);

void elf_set_picture_texture(elf_picture *picture, elf_texture *texture);
void elf_set_picture_scale(elf_picture *picture, float x, float y);

// <!!
void elf_destroy_text_field(elf_text_field *text_field);
void elf_draw_text_field(elf_text_field *text_field, elf_area *area, gfx_shader_params *shader_params);
void elf_recalc_text_field(elf_text_field *text_field);
// !!>

elf_text_field* elf_create_text_field(const char *name);	// <mdoc> TEXT FIELD FUNCTIONS

elf_texture* elf_get_text_field_texture(elf_text_field *text_field);
elf_font* elf_get_text_field_font(elf_text_field *text_field);
elf_color elf_get_text_field_text_color(elf_text_field *text_field);
elf_vec2i elf_get_text_field_offset(elf_text_field *text_field);
const char* elf_get_text_field_text(elf_text_field *text_field);

void elf_set_text_field_texture(elf_text_field *text_field, elf_texture *texture);
void elf_set_text_field_font(elf_text_field *text_field, elf_font *font);
void elf_set_text_field_text_color(elf_text_field *text_field, float r, float g, float b, float a);
void elf_set_text_field_offset(elf_text_field *text_field, int offset_x, int offset_y);
void elf_set_text_field_cursor_position(elf_text_field *text_field, int idx);
void elf_set_text_field_text(elf_text_field *text_field, const char *text);

// <!!
void elf_destroy_slider(elf_slider *slider);
void elf_draw_slider(elf_slider *slider, gfx_shader_params *shader_params);
void elf_recalc_slider(elf_slider *slider);
// !!>

elf_slider* elf_create_slider(const char *name);	// <mdoc> SLIDER FUNCTIONS

elf_texture* elf_get_slider_background_texture(elf_slider *slider);
elf_texture* elf_get_slider_slider_texture(elf_slider *slider);
float elf_get_slider_value(elf_slider *slider);

void elf_set_slider_background_texture(elf_slider *slider, elf_texture *background);
void elf_set_slider_slider_texture(elf_slider *slider, elf_texture *slider_texture);
void elf_set_slider_value(elf_slider *slider, float value);

// <!!
void elf_destroy_screen(elf_screen *screen);
void elf_draw_screen(elf_screen *screen, elf_area *area, gfx_shader_params *shader_params);
void elf_recalc_screen(elf_screen *screen);
// !!>

elf_screen* elf_create_screen(const char *name);	// <mdoc> SCREEN FUNCTIONS

elf_texture* elf_get_screen_texture(elf_screen *screen);
void elf_set_screen_texture(elf_screen *screen, elf_texture *texture);
void elf_set_screen_to_top(elf_screen *screen);
void elf_force_focus_to_screen(elf_screen *screen);
void elf_release_focus_from_screen(elf_screen *screen);

// <!!
void elf_destroy_text_list(elf_text_list *text_list);
void elf_draw_text_list(elf_text_list *text_list, elf_area *area, gfx_shader_params *shader_params);
void elf_recalc_text_list(elf_text_list *text_list);
// !!>

elf_text_list* elf_create_text_list(const char *name);	// <mdoc> TEXT LIST FUNCTIONS

elf_font* elf_get_text_list_font(elf_text_list *text_list);
elf_color elf_get_text_list_selection_color(elf_text_list *text_list);
elf_color elf_get_text_list_light_color(elf_text_list *text_list);
elf_color elf_get_text_list_dark_color(elf_text_list *text_list);
int elf_get_text_list_row_count(elf_text_list *text_list);
int elf_get_text_list_item_count(elf_text_list *text_list);
int elf_get_text_list_selection_index(elf_text_list *text_list);
int elf_get_text_list_offset(elf_text_list *text_list);
const char* elf_get_text_list_item(elf_text_list *text_list, int idx);
const char* elf_get_text_list_selected_item(elf_text_list *text_list);

void elf_set_text_list_font(elf_text_list *text_list, elf_font *font);
void elf_set_text_list_selection_color(elf_text_list *text_list, float r, float g, float b, float a);
void elf_set_text_list_light_color(elf_text_list *text_list, float r, float g, float b, float a);
void elf_set_text_list_dark_color(elf_text_list *text_list, float r, float g, float b, float a);
void elf_set_text_list_size(elf_text_list *text_list, int rows, int width);
void elf_add_text_list_item(elf_text_list *text_list, const char *text);
void elf_set_text_list_item(elf_text_list *text_list, int idx, const char *text);
unsigned char elf_remove_text_list_item(elf_text_list *text_list, int idx);
void elf_remove_text_list_items(elf_text_list *text_list);
void elf_set_text_list_offset(elf_text_list *text_list, int offset);
void elf_set_text_list_selection(elf_text_list *text_list, int selection);

// <!!
void elf_destroy_check_box(elf_check_box *check_box);
void elf_draw_check_box(elf_check_box *check_box, gfx_shader_params *shader_params);
void elf_recalc_check_box(elf_check_box *check_box);
// !!>

elf_check_box* elf_create_check_box(const char *name);	// <mdoc> CHECK BOX FUNCTIONS

unsigned char elf_get_check_box_state(elf_check_box *check_box);
elf_texture* elf_get_check_box_off_texture(elf_check_box *check_box);
elf_texture* elf_get_check_box_on_texture(elf_check_box *check_box);

void elf_set_check_box_off_texture(elf_check_box *check_box, elf_texture *off);
void elf_set_check_box_on_texture(elf_check_box *check_box, elf_texture *on);
void elf_set_check_box_state(elf_check_box *check_box, unsigned char state);

/* <!> */ void elf_recalc_gui_object(elf_gui_object *object);

// <!!
void elf_destroy_gui(elf_gui *gui);
void elf_update_gui(elf_gui *gui, float step);
void elf_draw_gui(elf_gui *gui);
// !!>

elf_gui* elf_create_gui();	// <mdoc> GUI FUNCTIONS

unsigned char elf_add_gui_object(elf_gui_object *parent, elf_gui_object *object);
elf_gui_object* elf_get_gui_object_by_name(elf_gui_object *parent, const char *name);
elf_gui_object* elf_get_gui_object_by_index(elf_gui_object *parent, int idx);
unsigned char elf_remove_gui_object_by_name(elf_gui_object *parent, const char *name);
unsigned char elf_remove_gui_object_by_index(elf_gui_object *parent, int idx);
unsigned char elf_remove_gui_object_by_object(elf_gui_object *parent, elf_gui_object *object);

elf_gui_object* elf_get_gui_trace(elf_gui *gui);
elf_gui_object* elf_get_gui_focus(elf_gui *gui);

void elf_empty_gui(elf_gui *gui);

//////////////////////////////// NET ////////////////////////////////

/* <!> */ unsigned char elf_init_networking();
/* <!> */ void elf_deinit_networking();
/* <!> */ void elf_run_networking();
unsigned char elf_create_session(const char* address, unsigned short port); // <mdoc> NETWORKING FUNCTIONS
unsigned char elf_connect_session(const char* address, unsigned short port);
unsigned char elf_disconnect_session();
void elf_send_string_to_clients(const char* message);
void elf_send_string_to_server(const char* message);
const char* elf_get_server_data_as_string();
const char* elf_get_client_data_as_string();
int elf_get_server_event();
int elf_get_client_event();
int elf_get_current_client();
unsigned char elf_is_server();
unsigned char elf_is_client();

//////////////////////////////// SST ////////////////////////////////

// <!!
char* elf_read_next(const char *text, int *pos);
char* elf_read_sst_string(const char* text, int *pos);
float elf_read_sst_float(const char* text, int *pos);
void elf_read_sst_floats(const char* text, int *pos, int n, float *params);
int elf_read_sst_int(const char* text, int *pos);
void elf_read_sst_ints(const char* text, int *pos, int n, int *params);
unsigned char elf_read_sst_bool(const char* text, int *pos);
// !!>

// <!!
#ifdef __cplusplus
}
#endif

#endif	// BLENDELF_H
// !!>

