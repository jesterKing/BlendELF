#ifndef ELF_BINDS_H
#define ELF_BINDS_H
#define ELF_FALSE 0x0000
#define ELF_TRUE 0x0001
#define ELF_NONE 0x0000
#define ELF_KEY_ESC 135
#define ELF_KEY_F1 136
#define ELF_KEY_F2 137
#define ELF_KEY_F3 138
#define ELF_KEY_F4 139
#define ELF_KEY_F5 140
#define ELF_KEY_F6 141
#define ELF_KEY_F7 142
#define ELF_KEY_F8 143
#define ELF_KEY_F9 144
#define ELF_KEY_F10 145
#define ELF_KEY_F11 146
#define ELF_KEY_F12 147
#define ELF_KEY_UP 148
#define ELF_KEY_DOWN 149
#define ELF_KEY_LEFT 150
#define ELF_KEY_RIGHT 151
#define ELF_KEY_LSHIFT 152
#define ELF_KEY_RSHIFT 153
#define ELF_KEY_LCTRL 154
#define ELF_KEY_RCTRL 155
#define ELF_KEY_LALT 156
#define ELF_KEY_RALT 157
#define ELF_KEY_TAB 158
#define ELF_KEY_ENTER 159
#define ELF_KEY_BACKSPACE 160
#define ELF_KEY_INSERT 161
#define ELF_KEY_DEL 162
#define ELF_KEY_PAGEUP 163
#define ELF_KEY_PAGEDOWN 164
#define ELF_KEY_HOME 165
#define ELF_KEY_END 166
#define ELF_KEY_KP_0 167
#define ELF_KEY_KP_1 168
#define ELF_KEY_KP_2 169
#define ELF_KEY_KP_3 170
#define ELF_KEY_KP_4 171
#define ELF_KEY_KP_5 172
#define ELF_KEY_KP_6 173
#define ELF_KEY_KP_7 174
#define ELF_KEY_KP_8 175
#define ELF_KEY_KP_9 176
#define ELF_KEY_KP_DIVIDE 177
#define ELF_KEY_KP_MULTIPLY 178
#define ELF_KEY_KP_SUBTRACT 179
#define ELF_KEY_KP_ADD 180
#define ELF_KEY_KP_DECIMAL 181
#define ELF_KEY_KP_EQUAL 182
#define ELF_KEY_KP_ENTER 183
#define ELF_KEY_SPACE 32
#define ELF_KEY_0 48
#define ELF_KEY_1 49
#define ELF_KEY_2 50
#define ELF_KEY_3 51
#define ELF_KEY_4 52
#define ELF_KEY_5 53
#define ELF_KEY_6 54
#define ELF_KEY_7 55
#define ELF_KEY_8 56
#define ELF_KEY_9 57
#define ELF_KEY_A 65
#define ELF_KEY_B 66
#define ELF_KEY_C 67
#define ELF_KEY_D 68
#define ELF_KEY_E 69
#define ELF_KEY_F 70
#define ELF_KEY_G 71
#define ELF_KEY_H 72
#define ELF_KEY_I 73
#define ELF_KEY_J 74
#define ELF_KEY_K 75
#define ELF_KEY_L 76
#define ELF_KEY_M 77
#define ELF_KEY_N 78
#define ELF_KEY_O 79
#define ELF_KEY_P 80
#define ELF_KEY_Q 81
#define ELF_KEY_R 82
#define ELF_KEY_S 83
#define ELF_KEY_T 84
#define ELF_KEY_U 85
#define ELF_KEY_V 86
#define ELF_KEY_W 87
#define ELF_KEY_X 88
#define ELF_KEY_Y 89
#define ELF_KEY_Z 90
#define ELF_UP 0x0000
#define ELF_PRESSED 0x0001
#define ELF_DOWN 0x0002
#define ELF_RELEASED 0x0003
#define ELF_OFF 0x0000
#define ELF_ON 0x0001
#define ELF_OVER 0x0002
#define ELF_BUTTON_LEFT 0x0000
#define ELF_BUTTON_MIDDLE 0x0001
#define ELF_BUTTON_RIGHT 0x0002
#define ELF_TEXTURE 0x0000
#define ELF_MATERIAL 0x0001
#define ELF_MODEL 0x0002
#define ELF_CAMERA 0x0003
#define ELF_ENTITY 0x0004
#define ELF_LIGHT 0x0005
#define ELF_SCENE 0x0006
#define ELF_LIST 0x0007
#define ELF_PAK 0x0008
#define ELF_PAK_INDEX 0x0009
#define ELF_KEY_EVENT 0x000A
#define ELF_CHAR_EVENT 0x000B
#define ELF_CONTEXT 0x000C
#define ELF_ENGINE 0x000D
#define ELF_TIMER 0x000E
#define ELF_IMAGE 0x000F
#define ELF_POST_PROCESS 0x0010
#define ELF_SCRIPT 0x0011
#define ELF_AUDIO_DEVICE 0x0012
#define ELF_AUDIO_SOURCE 0x0013
#define ELF_SOUND 0x0014
#define ELF_BONE 0x0015
#define ELF_ARMATURE 0x0016
#define ELF_STRING 0x0017
#define ELF_FONT 0x0018
#define ELF_LABEL 0x0019
#define ELF_BUTTON 0x001A
#define ELF_PICTURE 0x001B
#define ELF_TEXT_FIELD 0x001C
#define ELF_SLIDER 0x001D
#define ELF_SCREEN 0x001E
#define ELF_TEXT_LIST 0x001F
#define ELF_CHECK_BOX 0x0020
#define ELF_GUI 0x0021
#define ELF_DIRECTORY_ITEM 0x0022
#define ELF_DIRECTORY 0x0023
#define ELF_COLLISION 0x0034
#define ELF_PHYSICS_OBJECT 0x0035
#define ELF_PHYSICS_WORLD 0x0036
#define ELF_JOINT 0x0037
#define ELF_RESOURCES 0x0038
#define ELF_PARTICLE 0x0039
#define ELF_PARTICLES 0x003A
#define ELF_GAME_CONFIG 0x003B
#define ELF_BEZIER_POINT 0x003C
#define ELF_BEZIER_CURVE 0x003D
#define ELF_IPO 0x003E
#define ELF_FRAME_PLAYER 0x003F
#define ELF_PROPERTY 0x0040
#define ELF_CLIENT 0x0041
#define ELF_SCRIPTING 0x0042
#define ELF_PHYSICS_TRI_MESH 0x0043
#define ELF_SPRITE 0x0044
#define ELF_OBJECT_TYPE_COUNT 0x0045
#define ELF_PERSPECTIVE 0x0000
#define ELF_ORTHOGRAPHIC 0x0001
#define ELF_BOX 0x0001
#define ELF_SPHERE 0x0002
#define ELF_MESH 0x0003
#define ELF_CAPSULE 0x0004
#define ELF_HINGE 0x0001
#define ELF_BALL 0x0002
#define ELF_CONE_TWIST 0x0003
#define ELF_POINT_LIGHT 0x0001
#define ELF_SUN_LIGHT 0x0002
#define ELF_SPOT_LIGHT 0x0003
#define ELF_LUMINANCE 0x0000
#define ELF_LUMINANCE_ALPHA 0x0001
#define ELF_RGB 0x0002
#define ELF_RGBA 0x0003
#define ELF_BGR 0x0004
#define ELF_BGRA 0x0005
#define ELF_RGB16F 0x0006
#define ELF_RGB32F 0x0007
#define ELF_RGBA16F 0x0008
#define ELF_RGBA32F 0x0009
#define ELF_ALPHA32F 0x000A
#define ELF_DEPTH_COMPONENT 0x000B
#define ELF_COMPRESSED_RGB 0x000C
#define ELF_COMPRESSED_RGBA 0x000D
#define ELF_FLOAT 0x0000
#define ELF_INT 0x0001
#define ELF_UINT 0x0002
#define ELF_SHORT 0x0003
#define ELF_USHORT 0x0004
#define ELF_BYTE 0x0005
#define ELF_UBYTE 0x0006
#define ELF_PROPERTY_INT 0x0001
#define ELF_PROPERTY_FLOAT 0x0002
#define ELF_PROPERTY_STRING 0x0003
#define ELF_PROPERTY_BOOL 0x0004
#define ELF_COLOR_MAP 0x0001
#define ELF_NORMAL_MAP 0x0002
#define ELF_HEIGHT_MAP 0x0004
#define ELF_SPECULAR_MAP 0x0008
#define ELF_DETAIL_MAP 0x0010
#define ELF_SHADOW_MAP 0x0020
#define ELF_COLOR_RAMP_MAP 0x0040
#define ELF_LIGHT_MAP 0x0080
#define ELF_TRANSPARENT 0x0001
#define ELF_ADD 0x0002
#define ELF_DIR 0x0001
#define ELF_FILE 0x0002
#define ELF_CLICKED 0x0001
#define ELF_VALUE_CHANGED 0x0002
#define ELF_GAIN_FOCUS 0x0003
#define ELF_LOSE_FOCUS 0x0004
#define ELF_CHAR_INPUT 0x0005
#define ELF_SELECTION_CHANGED 0x0006
#define ELF_STATE_CHANGED 0x0007
#define ELF_JOYSTICK_BUTTON_1 0x0000
#define ELF_JOYSTICK_BUTTON_2 0x0001
#define ELF_JOYSTICK_BUTTON_3 0x0002
#define ELF_JOYSTICK_BUTTON_4 0x0003
#define ELF_JOYSTICK_BUTTON_5 0x0004
#define ELF_JOYSTICK_BUTTON_6 0x0005
#define ELF_JOYSTICK_BUTTON_7 0x0006
#define ELF_JOYSTICK_BUTTON_8 0x0007
#define ELF_JOYSTICK_BUTTON_9 0x0008
#define ELF_JOYSTICK_BUTTON_10 0x0009
#define ELF_JOYSTICK_BUTTON_11 0x000A
#define ELF_JOYSTICK_BUTTON_12 0x000B
#define ELF_JOYSTICK_BUTTON_13 0x000C
#define ELF_JOYSTICK_BUTTON_14 0x000D
#define ELF_JOYSTICK_BUTTON_15 0x000E
#define ELF_JOYSTICK_BUTTON_16 0x000F
#define ELF_LOC_X 0x0000
#define ELF_LOC_Y 0x0001
#define ELF_LOC_Z 0x0002
#define ELF_ROT_X 0x0003
#define ELF_ROT_Y 0x0004
#define ELF_ROT_Z 0x0005
#define ELF_SCALE_X 0x0006
#define ELF_SCALE_Y 0x0007
#define ELF_SCALE_Z 0x0008
#define ELF_QUA_X 0x0009
#define ELF_QUA_Y 0x000A
#define ELF_QUA_Z 0x000B
#define ELF_QUA_W 0x000C
#define ELF_NET_NONE 0x0000
#define ELF_NET_CONNECT 0x0001
#define ELF_NET_RECEIVE 0x0002
#define ELF_NET_DISCONNECT 0x0003
#define ELF_OGG 0x0001
#define ELF_WAV 0x0002
#define ELF_NO_ERROR 0x0000
#define ELF_INVALID_FILE 0x0001
#define ELF_CANT_OPEN_FILE 0x0002
#define ELF_CANT_OPEN_DIRECTORY 0x0003
#define ELF_CANT_INITIALIZE 0x0004
#define ELF_CANT_RUN_STRING 0x0005
#define ELF_CANT_RUN_SCRIPT 0x0006
#define ELF_CANT_CREATE 0x0007
#define ELF_CANT_RESIZE 0x0008
#define ELF_INVALID_SIZE 0x0009
#define ELF_UNKNOWN_FORMAT 0x000A
#define ELF_UNKNOWN_TYPE 0x000B
#define ELF_INVALID_HANDLE 0x000C
#define ELF_MISSING_FEATURE 0x000D
#if defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__)
	#ifndef ELF_PLAYER
		#define ELF_APIENTRY __stdcall
		#ifdef ELF_DLL
				#define ELF_API __declspec(dllexport)
		#else
				#define ELF_API __declspec(dllimport)
		#endif
		#include "windows.h"
		ELF_API bool ELF_APIENTRY elfInitWithHWND(int width, int height, const char* title, bool fullscreen, HWND hwnd);
		ELF_API HWND ELF_APIENTRY elfGetWindowHWND();
	#else
		#define ELF_API
		#define ELF_APIENTRY
	#endif
#else
	#define ELF_API
	#define ELF_APIENTRY
#endif
typedef struct elf_object		elf_object;
class ELF_API elf_handle {
	elf_object *obj;
public:
	elf_handle();
	elf_handle(elf_object *object);
	elf_handle(const elf_handle &handle);
	~elf_handle();

	elf_object* get() {return obj;}

	elf_handle& operator=(elf_object *object);
	elf_handle& operator=(elf_handle &handle);
	elf_handle& operator=(const elf_handle &handle);
};
#ifndef BLENDELF_H
extern "C" {
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
}
#endif
ELF_API bool ELF_APIENTRY elfIsObject(elf_handle obj);
ELF_API int ELF_APIENTRY elfGetObjectType(elf_handle obj);
ELF_API int ELF_APIENTRY elfGetObjectRefCount(elf_handle obj);
ELF_API int ELF_APIENTRY elfGetGlobalRefCount();
ELF_API int ELF_APIENTRY elfGetGlobalObjCount();
ELF_API bool ELF_APIENTRY elfIsActor(elf_handle obj);
ELF_API bool ELF_APIENTRY elfIsGuiObject(elf_handle obj);
ELF_API elf_handle ELF_APIENTRY elfCreateList();
ELF_API int ELF_APIENTRY elfGetListLength(elf_handle list);
ELF_API void ELF_APIENTRY elfInsertToList(elf_handle list, int idx, elf_handle obj);
ELF_API void ELF_APIENTRY elfAppendToList(elf_handle list, elf_handle obj);
ELF_API bool ELF_APIENTRY elfRemoveFromList(elf_handle list, elf_handle obj);
ELF_API elf_handle ELF_APIENTRY elfGetItemFromList(elf_handle list, int idx);
ELF_API elf_handle ELF_APIENTRY elfBeginList(elf_handle list);
ELF_API elf_handle ELF_APIENTRY elfNextInList(elf_handle list);
ELF_API elf_handle ELF_APIENTRY elfRbeginList(elf_handle list);
ELF_API elf_handle ELF_APIENTRY elfRnextInList(elf_handle list);
ELF_API void ELF_APIENTRY elfSeekList(elf_handle list, elf_handle ptr);
ELF_API void ELF_APIENTRY elfRseekList(elf_handle list, elf_handle ptr);
ELF_API void ELF_APIENTRY elfSetTitle(const char* title);
ELF_API int ELF_APIENTRY elfGetWindowWidth();
ELF_API int ELF_APIENTRY elfGetWindowHeight();
ELF_API bool ELF_APIENTRY elfIsFullscreen();
ELF_API const char* ELF_APIENTRY elfGetTitle();
ELF_API int ELF_APIENTRY elfGetMultisamples();
ELF_API double ELF_APIENTRY elfGetTime();
ELF_API bool ELF_APIENTRY elfIsWindowOpened();
ELF_API elf_vec2i ELF_APIENTRY elfGetMousePosition();
ELF_API elf_vec2i ELF_APIENTRY elfGetMouseForce();
ELF_API void ELF_APIENTRY elfSetMousePosition(int x, int y);
ELF_API void ELF_APIENTRY elfHideMouse(bool hide);
ELF_API bool ELF_APIENTRY elfIsMouseHidden();
ELF_API int ELF_APIENTRY elfGetMouseWheel();
ELF_API int ELF_APIENTRY elfGetMouseButtonState(int button);
ELF_API int ELF_APIENTRY elfGetKeyState(int key);
ELF_API bool ELF_APIENTRY elfGetJoystickPresent(int joy);
ELF_API elf_vec2f ELF_APIENTRY elfGetJoystickAxis(int joy);
ELF_API int ELF_APIENTRY elfGetJoystickButtonState(int joy, int but);
ELF_API int ELF_APIENTRY elfGetEventCount();
ELF_API elf_handle ELF_APIENTRY elfGetEvent(int idx);
ELF_API elf_handle ELF_APIENTRY elfReadGameConfig(const char* file_path);
ELF_API int ELF_APIENTRY elfGetGameConfigWindowWidth(elf_handle config);
ELF_API int ELF_APIENTRY elfGetGameConfigWindowHeight(elf_handle config);
ELF_API bool ELF_APIENTRY elfGetGameConfigFullscreen(elf_handle config);
ELF_API float ELF_APIENTRY elfGetGameConfigTextureAnisotropy(elf_handle config);
ELF_API int ELF_APIENTRY elfGetGameConfigShadowMapSize(elf_handle config);
ELF_API const char* ELF_APIENTRY elfGetGameConfigStart(elf_handle config);
ELF_API bool ELF_APIENTRY elfInit(int width, int height, const char* title, bool fullscreen);
ELF_API bool ELF_APIENTRY elfInitWithConfig(const char* file_path);
ELF_API void ELF_APIENTRY elfDeinit();
ELF_API void ELF_APIENTRY elfResizeWindow(int width, int height);
ELF_API const char* ELF_APIENTRY elfGetPlatform();
ELF_API int ELF_APIENTRY elfGetVersionMajor();
ELF_API int ELF_APIENTRY elfGetVersionMinor();
ELF_API const char* ELF_APIENTRY elfGetVersionRelease();
ELF_API const char* ELF_APIENTRY elfGetVersion();
ELF_API const char* ELF_APIENTRY elfGetErrorString();
ELF_API int ELF_APIENTRY elfGetError();
ELF_API bool ELF_APIENTRY elfRun();
ELF_API void ELF_APIENTRY elfQuit();
ELF_API void ELF_APIENTRY elfSetF10Exit(bool exit);
ELF_API bool ELF_APIENTRY elfGetF10Exit();
ELF_API elf_handle ELF_APIENTRY elfLoadScene(const char* file_path);
ELF_API void ELF_APIENTRY elfSetScene(elf_handle scene);
ELF_API elf_handle ELF_APIENTRY elfGetScene();
ELF_API void ELF_APIENTRY elfSetGui(elf_handle gui);
ELF_API elf_handle ELF_APIENTRY elfGetGui();
ELF_API float ELF_APIENTRY elfGetSync();
ELF_API int ELF_APIENTRY elfGetFps();
ELF_API bool ELF_APIENTRY elfSaveScreenShot(const char* file_path);
ELF_API void ELF_APIENTRY elfSetFpsLimit(int fps_limit);
ELF_API int ELF_APIENTRY elfGetFpsLimit();
ELF_API void ELF_APIENTRY elfSetTickRate(float tick_rate);
ELF_API float ELF_APIENTRY elfGetTickRate();
ELF_API void ELF_APIENTRY elfSetSpeed(float speed);
ELF_API float ELF_APIENTRY elfGetSpeed();
ELF_API void ELF_APIENTRY elfSetTextureAnisotropy(float anisotropy);
ELF_API float ELF_APIENTRY elfGetTextureAnisotropy();
ELF_API void ELF_APIENTRY elfSetShadowMapSize(int size);
ELF_API int ELF_APIENTRY elfGetShadowMapSize();
ELF_API int ELF_APIENTRY elfGetPolygonsRendered();
ELF_API void ELF_APIENTRY elfSetBloom(float threshold);
ELF_API void ELF_APIENTRY elfDisableBloom();
ELF_API float ELF_APIENTRY elfGetBloomThreshold();
ELF_API void ELF_APIENTRY elfSetDof(float focal_range, float focal_distance);
ELF_API void ELF_APIENTRY elfDisableDof();
ELF_API float ELF_APIENTRY elfGetDofFocalRange();
ELF_API float ELF_APIENTRY elfGetDofFocalDistance();
ELF_API void ELF_APIENTRY elfSetSsao(float amount);
ELF_API void ELF_APIENTRY elfDisableSsao();
ELF_API float ELF_APIENTRY elfGetSsaoAmount();
ELF_API void ELF_APIENTRY elfSetLightShafts(float intensity);
ELF_API void ELF_APIENTRY elfDisableLightShafts();
ELF_API float ELF_APIENTRY elfGetLightShaftsInteisity();
ELF_API bool ELF_APIENTRY elfIsBloom();
ELF_API bool ELF_APIENTRY elfIsSsao();
ELF_API bool ELF_APIENTRY elfIsDof();
ELF_API bool ELF_APIENTRY elfIsLightShafts();
ELF_API void ELF_APIENTRY elfSetOcclusionCulling(bool cull);
ELF_API bool ELF_APIENTRY elfIsOcclusionCulling();
ELF_API void ELF_APIENTRY elfSetDebugDraw(bool debug_draw);
ELF_API bool ELF_APIENTRY elfIsDebugDraw();
ELF_API elf_handle ELF_APIENTRY elfGetActor();
ELF_API elf_vec3f ELF_APIENTRY elfCreateVec3f();
ELF_API elf_vec3f ELF_APIENTRY elfCreateVec3fFromValues(float x, float y, float z);
ELF_API elf_vec4f ELF_APIENTRY elfCreateQua();
ELF_API elf_vec4f ELF_APIENTRY elfCreateQuaFromEuler(float x, float y, float z);
ELF_API elf_vec3f ELF_APIENTRY elfMulQuaVec3f(elf_vec4f qua, elf_vec3f vec);
ELF_API elf_vec4f ELF_APIENTRY elfMulQuaQua(elf_vec4f qua1, elf_vec4f qua2);
ELF_API elf_vec4f ELF_APIENTRY elfGetQuaInverted(elf_vec4f qua);
ELF_API elf_vec3f ELF_APIENTRY elfSubVec3fVec3f(elf_vec3f vec1, elf_vec3f vec2);
ELF_API elf_vec3f ELF_APIENTRY elfAddVec3fVec3f(elf_vec3f vec1, elf_vec3f vec2);
ELF_API float ELF_APIENTRY elfGetVec3fLength(elf_vec3f vec);
ELF_API bool ELF_APIENTRY elfAboutZero(float val);
ELF_API float ELF_APIENTRY elfFloatAbs(float val);
ELF_API float ELF_APIENTRY elfFloatMax(float a, float b);
ELF_API float ELF_APIENTRY elfRandomFloat();
ELF_API float ELF_APIENTRY elfRandomFloatRange(float min, float max);
ELF_API int ELF_APIENTRY elfRandomInt();
ELF_API int ELF_APIENTRY elfRandomIntRange(int min, int max);
ELF_API elf_handle ELF_APIENTRY elfCreateFramePlayer();
ELF_API void ELF_APIENTRY elfUpdateFramePlayer(elf_handle player);
ELF_API void ELF_APIENTRY elfSetFramePlayerFrame(elf_handle player, float frame);
ELF_API void ELF_APIENTRY elfPlayFramePlayer(elf_handle player, float start, float end, float speed);
ELF_API void ELF_APIENTRY elfLoopFramePlayer(elf_handle player, float start, float end, float speed);
ELF_API void ELF_APIENTRY elfStopFramePlayer(elf_handle player);
ELF_API void ELF_APIENTRY elfPauseFramePlayer(elf_handle player);
ELF_API void ELF_APIENTRY elfResumeFramePlayer(elf_handle player);
ELF_API float ELF_APIENTRY elfGetFramePlayerStart(elf_handle player);
ELF_API float ELF_APIENTRY elfGetFramePlayerEnd(elf_handle player);
ELF_API float ELF_APIENTRY elfGetFramePlayerSpeed(elf_handle player);
ELF_API float ELF_APIENTRY elfGetFramePlayerFrame(elf_handle player);
ELF_API bool ELF_APIENTRY elfIsFramePlayerPlaying(elf_handle player);
ELF_API bool ELF_APIENTRY elfIsFramePlayerPaused(elf_handle player);
ELF_API elf_handle ELF_APIENTRY elfCreateTimer();
ELF_API void ELF_APIENTRY elfStartTimer(elf_handle timer);
ELF_API double ELF_APIENTRY elfGetElapsedTime(elf_handle timer);
ELF_API elf_handle ELF_APIENTRY elfCreateTextureFromFile(const char* file_path);
ELF_API const char* ELF_APIENTRY elfGetTextureName(elf_handle texture);
ELF_API const char* ELF_APIENTRY elfGetTextureFilePath(elf_handle texture);
ELF_API int ELF_APIENTRY elfGetTextureWidth(elf_handle texture);
ELF_API int ELF_APIENTRY elfGetTextureHeight(elf_handle texture);
ELF_API int ELF_APIENTRY elfGetTextureFormat(elf_handle texture);
ELF_API int ELF_APIENTRY elfGetTextureDataFormat(elf_handle texture);
ELF_API elf_handle ELF_APIENTRY elfCreateMaterial(const char* name);
ELF_API void ELF_APIENTRY elfSetMaterialTexture(elf_handle material, int slot, elf_handle texture);
ELF_API void ELF_APIENTRY elfSetMaterialTextureType(elf_handle material, int slot, int type);
ELF_API void ELF_APIENTRY elfSetMaterialTextureParallaxScale(elf_handle material, int slot, float scale);
ELF_API void ELF_APIENTRY elfSetMaterialTextureAlphaTest(elf_handle material, int slot, float test);
ELF_API void ELF_APIENTRY elfSetMaterialDiffuseColor(elf_handle material, float r, float g, float b, float a);
ELF_API void ELF_APIENTRY elfSetMaterialSpecularColor(elf_handle material, float r, float g, float b, float a);
ELF_API void ELF_APIENTRY elfSetMaterialAmbientColor(elf_handle material, float r, float g, float b, float a);
ELF_API void ELF_APIENTRY elfSetMaterialSpecularPower(elf_handle material, float power);
ELF_API void ELF_APIENTRY elfSetMaterialLighting(elf_handle material, bool lighting);
ELF_API const char* ELF_APIENTRY elfGetMaterialName(elf_handle material);
ELF_API const char* ELF_APIENTRY elfGetMaterialFilePath(elf_handle material);
ELF_API elf_handle ELF_APIENTRY elfGetMaterialTexture(elf_handle material, int slot);
ELF_API int ELF_APIENTRY elfGetMaterialTextureType(elf_handle material, int slot);
ELF_API float ELF_APIENTRY elfGetMaterialTextureParallaxScale(elf_handle material, int slot);
ELF_API float ELF_APIENTRY elfGetMaterialTextureAlphaTexture(elf_handle material, int slot);
ELF_API elf_color ELF_APIENTRY elfGetMaterialDiffuseColor(elf_handle material);
ELF_API elf_color ELF_APIENTRY elfGetMaterialSpecularColor(elf_handle material);
ELF_API elf_color ELF_APIENTRY elfGetMaterialAmbientColor(elf_handle material);
ELF_API float ELF_APIENTRY elfGetMaterialSpecularPower(elf_handle material);
ELF_API bool ELF_APIENTRY elfGetMaterialLighting(elf_handle material);
ELF_API void ELF_APIENTRY elfAddPointToBezierCurve(elf_handle curve, elf_handle point);
ELF_API bool ELF_APIENTRY elfAddCurveToIpo(elf_handle ipo, elf_handle curve);
ELF_API elf_vec3f ELF_APIENTRY elfGetIpoLoc(elf_handle ipo, float x);
ELF_API elf_vec3f ELF_APIENTRY elfGetIpoRot(elf_handle ipo, float x);
ELF_API elf_vec3f ELF_APIENTRY elfGetIpoScale(elf_handle ipo, float x);
ELF_API elf_vec4f ELF_APIENTRY elfGetIpoQua(elf_handle ipo, float x);
ELF_API elf_handle ELF_APIENTRY elfCreateProperty(const char* name);
ELF_API int ELF_APIENTRY elfGetPropertyType(elf_handle property);
ELF_API int ELF_APIENTRY elfGetPropertyInt(elf_handle property);
ELF_API float ELF_APIENTRY elfGetPropertyFloat(elf_handle property);
ELF_API const char* ELF_APIENTRY elfGetPropertyString(elf_handle property);
ELF_API bool ELF_APIENTRY elfGetPropertyBool(elf_handle property);
ELF_API void ELF_APIENTRY elfSetPropertyInt(elf_handle property, int ival);
ELF_API void ELF_APIENTRY elfSetPropertyFloat(elf_handle property, float fval);
ELF_API void ELF_APIENTRY elfSetPropertyString(elf_handle property, const char* sval);
ELF_API void ELF_APIENTRY elfSetPropertyBool(elf_handle property, bool bval);
ELF_API const char* ELF_APIENTRY elfGetActorName(elf_handle actor);
ELF_API const char* ELF_APIENTRY elfGetActorFilePath(elf_handle actor);
ELF_API elf_handle ELF_APIENTRY elfGetActorParent(elf_handle actor);
ELF_API void ELF_APIENTRY elfSetActorParent(elf_handle actor, elf_handle parent);
ELF_API int ELF_APIENTRY elfGetActorChildCount(elf_handle actor);
ELF_API elf_handle ELF_APIENTRY elfGetActorChildByName(elf_handle actor, const char* name);
ELF_API elf_handle ELF_APIENTRY elfGetActorChildByIndex(elf_handle actor, int idx);
ELF_API void ELF_APIENTRY elfRemoveActorChildren(elf_handle actor);
ELF_API elf_handle ELF_APIENTRY elfGetActorScript(elf_handle actor);
ELF_API void ELF_APIENTRY elfSetActorScript(elf_handle actor, elf_handle script);
ELF_API void ELF_APIENTRY elfSetActorPosition(elf_handle actor, float x, float y, float z);
ELF_API void ELF_APIENTRY elfSetActorRotation(elf_handle actor, float x, float y, float z);
ELF_API void ELF_APIENTRY elfSetActorOrientation(elf_handle actor, float x, float y, float z, float w);
ELF_API void ELF_APIENTRY elfRotateActor(elf_handle actor, float x, float y, float z);
ELF_API void ELF_APIENTRY elfRotateActorLocal(elf_handle actor, float x, float y, float z);
ELF_API void ELF_APIENTRY elfMoveActor(elf_handle actor, float x, float y, float z);
ELF_API void ELF_APIENTRY elfMoveActorLocal(elf_handle actor, float x, float y, float z);
ELF_API elf_vec3f ELF_APIENTRY elfGetActorPosition(elf_handle actor);
ELF_API elf_vec3f ELF_APIENTRY elfGetActorRotation(elf_handle actor);
ELF_API elf_vec4f ELF_APIENTRY elfGetActorOrientation(elf_handle actor);
ELF_API void ELF_APIENTRY elfSetActorBoundingLengths(elf_handle actor, float x, float y, float z);
ELF_API void ELF_APIENTRY elfSetActorBoundingOffset(elf_handle actor, float x, float y, float z);
ELF_API void ELF_APIENTRY elfSetActorPhysics(elf_handle actor, int shape, float mass);
ELF_API void ELF_APIENTRY elfDisableActorPhysics(elf_handle actor);
ELF_API void ELF_APIENTRY elfSetActorAnisotropicFriction(elf_handle actor, float x, float y, float z);
ELF_API void ELF_APIENTRY elfSetActorDamping(elf_handle actor, float lin_damp, float ang_damp);
ELF_API void ELF_APIENTRY elfSetActorSleepThresholds(elf_handle actor, float lin_thrs, float ang_thrs);
ELF_API void ELF_APIENTRY elfSetActorRestitution(elf_handle actor, float restitution);
ELF_API void ELF_APIENTRY elfAddForceToActor(elf_handle actor, float x, float y, float z);
ELF_API void ELF_APIENTRY elfAddTorqueToActor(elf_handle actor, float x, float y, float z);
ELF_API void ELF_APIENTRY elfSetActorLinearVelocity(elf_handle actor, float x, float y, float z);
ELF_API void ELF_APIENTRY elfSetActorAngularVelocity(elf_handle actor, float x, float y, float z);
ELF_API void ELF_APIENTRY elfSetActorLinearFactor(elf_handle actor, float x, float y, float z);
ELF_API void ELF_APIENTRY elfSetActorAngularFactor(elf_handle actor, float x, float y, float z);
ELF_API elf_vec3f ELF_APIENTRY elfGetActorBoundingLengths(elf_handle actor);
ELF_API elf_vec3f ELF_APIENTRY elfGetActorBoundingOffset(elf_handle actor);
ELF_API int ELF_APIENTRY elfGetActorPhysicsShape(elf_handle actor);
ELF_API float ELF_APIENTRY elfGetActorPhysicsMass(elf_handle actor);
ELF_API elf_vec3f ELF_APIENTRY elfGetActorLinearVelocity(elf_handle actor);
ELF_API elf_vec3f ELF_APIENTRY elfGetActorAngularVelocity(elf_handle actor);
ELF_API elf_vec3f ELF_APIENTRY elfGetActorLinearFactor(elf_handle actor);
ELF_API elf_vec3f ELF_APIENTRY elfGetActorAngularFactor(elf_handle actor);
ELF_API elf_vec3f ELF_APIENTRY elfGetActorAnisotropicFriction(elf_handle actor);
ELF_API float ELF_APIENTRY elfGetActorLinearDamping(elf_handle actor);
ELF_API float ELF_APIENTRY elfGetActorAngularDamping(elf_handle actor);
ELF_API float ELF_APIENTRY elfGetActorLinearSleepThreshold(elf_handle actor);
ELF_API float ELF_APIENTRY elfGetActorAngularSleepThreshold(elf_handle actor);
ELF_API float ELF_APIENTRY elfGetActorRestitution(elf_handle actor);
ELF_API elf_handle ELF_APIENTRY elfAddHingeJointToActor(elf_handle actor, elf_handle actor2, const char* name, float px, float py, float pz, float ax, float ay, float az);
ELF_API elf_handle ELF_APIENTRY elfAddBallJointToActor(elf_handle actor, elf_handle actor2, const char* name, float px, float py, float pz);
ELF_API elf_handle ELF_APIENTRY elfAddConeTwistJointToActor(elf_handle actor, elf_handle actor2, const char* name, float px, float py, float pz, float ax, float ay, float az);
ELF_API elf_handle ELF_APIENTRY elfGetActorJointByName(elf_handle actor, const char* name);
ELF_API elf_handle ELF_APIENTRY elfGetActorJointByIndex(elf_handle actor, int idx);
ELF_API bool ELF_APIENTRY elfRemoveActorJointByName(elf_handle actor, const char* name);
ELF_API bool ELF_APIENTRY elfRemoveActorJointByIndex(elf_handle actor, int idx);
ELF_API bool ELF_APIENTRY elfRemoveActorJointByObject(elf_handle actor, elf_handle joint);
ELF_API void ELF_APIENTRY elfSetActorIpoFrame(elf_handle actor, float frame);
ELF_API void ELF_APIENTRY elfSetActorHierarchyIpoFrame(elf_handle actor, float frame);
ELF_API void ELF_APIENTRY elfPlayActorIpo(elf_handle actor, float start, float end, float speed);
ELF_API void ELF_APIENTRY elfLoopActorIpo(elf_handle actor, float start, float end, float speed);
ELF_API void ELF_APIENTRY elfStopActorIpo(elf_handle actor);
ELF_API void ELF_APIENTRY elfPauseActorIpo(elf_handle actor);
ELF_API void ELF_APIENTRY elfResumeActorIpo(elf_handle actor);
ELF_API float ELF_APIENTRY elfGetActorIpoStart(elf_handle actor);
ELF_API float ELF_APIENTRY elfGetActorIpoEnd(elf_handle actor);
ELF_API float ELF_APIENTRY elfGetActorIpoSpeed(elf_handle actor);
ELF_API float ELF_APIENTRY elfGetActorIpoFrame(elf_handle actor);
ELF_API bool ELF_APIENTRY elfIsActorIpoPlaying(elf_handle actor);
ELF_API bool ELF_APIENTRY elfIsActorIpoPaused(elf_handle actor);
ELF_API int ELF_APIENTRY elfGetActorCollisionCount(elf_handle actor);
ELF_API elf_handle ELF_APIENTRY elfGetActorCollision(elf_handle actor, int idx);
ELF_API int ELF_APIENTRY elfGetActorPropertyCount(elf_handle actor);
ELF_API void ELF_APIENTRY elfAddPropertyToActor(elf_handle actor, elf_handle property);
ELF_API elf_handle ELF_APIENTRY elfGetActorPropertyByName(elf_handle actor, const char* name);
ELF_API elf_handle ELF_APIENTRY elfGetActorPropertyByIndex(elf_handle actor, int idx);
ELF_API bool ELF_APIENTRY elfRemoveActorPropertyByName(elf_handle actor, const char* name);
ELF_API bool ELF_APIENTRY elfRemoveActorPropertyByIndex(elf_handle actor, int idx);
ELF_API bool ELF_APIENTRY elfRemoveActorPropertyByObject(elf_handle actor, elf_handle property);
ELF_API void ELF_APIENTRY elfRemoveActorProperties(elf_handle actor);
ELF_API void ELF_APIENTRY elfSetActorSelected(elf_handle actor, bool selected);
ELF_API bool ELF_APIENTRY elfGetActorSelected(elf_handle actor);
ELF_API elf_handle ELF_APIENTRY elfCreateCamera(const char* name);
ELF_API void ELF_APIENTRY elfSetCameraViewport(elf_handle camera, int x, int y, int width, int height);
ELF_API void ELF_APIENTRY elfSetCameraPerspective(elf_handle camera, float fov, float aspect, float clip_near, float clip_far);
ELF_API void ELF_APIENTRY elfSetCameraOrthographic(elf_handle camera, int x, int y, int width, int height, float clip_near, float clip_far);
ELF_API elf_vec2i ELF_APIENTRY elfGetCameraViewportSize(elf_handle camera);
ELF_API elf_vec2i ELF_APIENTRY elfGetCameraViewportOffset(elf_handle camera);
ELF_API float ELF_APIENTRY elfGetCameraFov(elf_handle camera);
ELF_API float ELF_APIENTRY elfGetCameraAspect(elf_handle camera);
ELF_API elf_vec2f ELF_APIENTRY elfGetCameraClip(elf_handle camera);
ELF_API elf_vec2f ELF_APIENTRY elfGetCameraFarPlaneSize(elf_handle camera);
ELF_API elf_vec3f ELF_APIENTRY elfUnProjectCameraPoint(elf_handle camera, float x, float y, float z);
ELF_API const char* ELF_APIENTRY elfGetModelName(elf_handle model);
ELF_API const char* ELF_APIENTRY elfGetModelFilePath(elf_handle model);
ELF_API int ELF_APIENTRY elfGetModelVerticeCount(elf_handle model);
ELF_API int ELF_APIENTRY elfGetModelIndiceCount(elf_handle model);
ELF_API elf_vec3f ELF_APIENTRY elfGetModelBoundingBoxMin(elf_handle model);
ELF_API elf_vec3f ELF_APIENTRY elfGetModelBoundingBoxMax(elf_handle model);
ELF_API elf_handle ELF_APIENTRY elfCreateEntity(const char* name);
ELF_API void ELF_APIENTRY elfSetEntityScale(elf_handle entity, float x, float y, float z);
ELF_API elf_vec3f ELF_APIENTRY elfGetEntityScale(elf_handle entity);
ELF_API void ELF_APIENTRY elfSetEntityModel(elf_handle entity, elf_handle model);
ELF_API elf_handle ELF_APIENTRY elfGetEntityModel(elf_handle entity);
ELF_API int ELF_APIENTRY elfGetEntityMaterialCount(elf_handle entity);
ELF_API void ELF_APIENTRY elfAddEntityMaterial(elf_handle entity, elf_handle material);
ELF_API void ELF_APIENTRY elfSetEntityMaterial(elf_handle entity, int idx, elf_handle material);
ELF_API elf_handle ELF_APIENTRY elfGetEntityMaterial(elf_handle entity, int idx);
ELF_API void ELF_APIENTRY elfSetEntityVisible(elf_handle entity, bool visible);
ELF_API bool ELF_APIENTRY elfGetEntityVisible(elf_handle entity);
ELF_API void ELF_APIENTRY elfSetEntityPhysics(elf_handle entity, int type, float mass);
ELF_API void ELF_APIENTRY elfDisableEntityPhysics(elf_handle entity);
ELF_API void ELF_APIENTRY elfSetEntityArmature(elf_handle entity, elf_handle armature);
ELF_API void ELF_APIENTRY elfSetEntityArmatureFrame(elf_handle entity, float frame);
ELF_API void ELF_APIENTRY elfPlayEntityArmature(elf_handle entity, float start, float end, float speed);
ELF_API void ELF_APIENTRY elfLoopEntityArmature(elf_handle entity, float start, float end, float speed);
ELF_API void ELF_APIENTRY elfStopEntityArmature(elf_handle entity);
ELF_API void ELF_APIENTRY elfPauseEntityArmature(elf_handle entity);
ELF_API void ELF_APIENTRY elfResumeEntityArmature(elf_handle entity);
ELF_API float ELF_APIENTRY elfGetEntityArmatureStart(elf_handle entity);
ELF_API float ELF_APIENTRY elfGetEntityArmatureEnd(elf_handle entity);
ELF_API float ELF_APIENTRY elfGetEntityArmatureSpeed(elf_handle entity);
ELF_API float ELF_APIENTRY elfGetEntityArmatureFrame(elf_handle entity);
ELF_API bool ELF_APIENTRY elfIsEntityArmaturePlaying(elf_handle entity);
ELF_API bool ELF_APIENTRY elfIsEntityArmaturePaused(elf_handle entity);
ELF_API elf_handle ELF_APIENTRY elfGetEntityArmature(elf_handle entity);
ELF_API bool ELF_APIENTRY elfGetEntityChanged(elf_handle entity);
ELF_API elf_handle ELF_APIENTRY elfCreateLight(const char* name);
ELF_API int ELF_APIENTRY elfGetLightType(elf_handle light);
ELF_API elf_color ELF_APIENTRY elfGetLightColor(elf_handle light);
ELF_API float ELF_APIENTRY elfGetLightDistance(elf_handle light);
ELF_API float ELF_APIENTRY elfGetLightFadeSpeed(elf_handle light);
ELF_API bool ELF_APIENTRY elfGetLightShadowCaster(elf_handle light);
ELF_API bool ELF_APIENTRY elfGetLightVisible(elf_handle light);
ELF_API elf_vec2f ELF_APIENTRY elfGetLightCone(elf_handle light);
ELF_API bool ELF_APIENTRY elfIsLightShaft(elf_handle light);
ELF_API float ELF_APIENTRY elfGetLightShaftSize(elf_handle light);
ELF_API float ELF_APIENTRY elfGetLightShaftIntensity(elf_handle light);
ELF_API float ELF_APIENTRY elfGetLightShaftFadeOff(elf_handle light);
ELF_API void ELF_APIENTRY elfSetLightType(elf_handle light, int type);
ELF_API void ELF_APIENTRY elfSetLightColor(elf_handle light, float r, float g, float b, float a);
ELF_API void ELF_APIENTRY elfSetLightDistance(elf_handle light, float distance);
ELF_API void ELF_APIENTRY elfSetLightFadeSpeed(elf_handle light, float fade_speed);
ELF_API void ELF_APIENTRY elfSetLightShadowCaster(elf_handle light, bool shadow_caster);
ELF_API void ELF_APIENTRY elfSetLightVisible(elf_handle light, bool visible);
ELF_API void ELF_APIENTRY elfSetLightCone(elf_handle light, float inner_cone, float outer_cone);
ELF_API void ELF_APIENTRY elfSetLightShaft(elf_handle light, float size, float intensity, float fade_off);
ELF_API void ELF_APIENTRY elfDisableLightShaft(elf_handle light);
ELF_API elf_handle ELF_APIENTRY elfGetBoneArmature(elf_handle bone);
ELF_API elf_handle ELF_APIENTRY elfGetBoneParent(elf_handle bone);
ELF_API elf_handle ELF_APIENTRY elfGetBoneChildByName(elf_handle bone, const char* name);
ELF_API elf_handle ELF_APIENTRY elfGetBoneChildById(elf_handle bone, int id);
ELF_API elf_handle ELF_APIENTRY elfGetBoneChildByIndex(elf_handle bone, int idx);
ELF_API elf_vec3f ELF_APIENTRY elfGetBonePosition(elf_handle bone);
ELF_API elf_vec3f ELF_APIENTRY elfGetBoneRotation(elf_handle bone);
ELF_API elf_vec4f ELF_APIENTRY elfGetBoneOrientation(elf_handle bone);
ELF_API elf_handle ELF_APIENTRY elfCreateArmature(const char* name);
ELF_API elf_handle ELF_APIENTRY elfGetBoneFromArmatureByName(const char* name, elf_handle armature);
ELF_API elf_handle ELF_APIENTRY elfGetBoneFromArmatureById(int id, elf_handle armature);
ELF_API elf_handle ELF_APIENTRY elfCreateParticles(const char* name, int max_count);
ELF_API const char* ELF_APIENTRY elfGetParticlesName(elf_handle particles);
ELF_API const char* ELF_APIENTRY elfGetParticlesFilePath(elf_handle particles);
ELF_API void ELF_APIENTRY elfSetParticlesMaxCount(elf_handle particles, int max_count);
ELF_API void ELF_APIENTRY elfSetParticlesDrawMode(elf_handle particles, int mode);
ELF_API void ELF_APIENTRY elfSetParticlesTexture(elf_handle particles, elf_handle texture);
ELF_API void ELF_APIENTRY elfSetParticlesModel(elf_handle particles, elf_handle model);
ELF_API void ELF_APIENTRY elfSetParticlesEntity(elf_handle particles, elf_handle entity);
ELF_API void ELF_APIENTRY elfSetParticlesGravity(elf_handle particles, float x, float y, float z);
ELF_API void ELF_APIENTRY elfSetParticlesSpawnDelay(elf_handle particles, float delay);
ELF_API void ELF_APIENTRY elfSetParticlesSize(elf_handle particles, float min, float max);
ELF_API void ELF_APIENTRY elfSetParticlesSizeGrowth(elf_handle particles, float min, float max);
ELF_API void ELF_APIENTRY elfSetParticlesRotation(elf_handle particles, float min, float max);
ELF_API void ELF_APIENTRY elfSetParticlesRotationGrowth(elf_handle particles, float min, float max);
ELF_API void ELF_APIENTRY elfSetParticlesLifeSpan(elf_handle particles, float min, float max);
ELF_API void ELF_APIENTRY elfSetParticlesFadeSpeed(elf_handle particles, float min, float max);
ELF_API void ELF_APIENTRY elfSetParticlesPositionMin(elf_handle particles, float x, float y, float z);
ELF_API void ELF_APIENTRY elfSetParticlesPositionMax(elf_handle particles, float x, float y, float z);
ELF_API void ELF_APIENTRY elfSetParticlesVelocityMin(elf_handle particles, float x, float y, float z);
ELF_API void ELF_APIENTRY elfSetParticlesVelocityMax(elf_handle particles, float x, float y, float z);
ELF_API void ELF_APIENTRY elfSetParticlesColorMin(elf_handle particles, float r, float g, float b, float a);
ELF_API void ELF_APIENTRY elfSetParticlesColorMax(elf_handle particles, float r, float g, float b, float a);
ELF_API int ELF_APIENTRY elfGetParticlesMaxCount(elf_handle particles);
ELF_API int ELF_APIENTRY elfGetParticlesCount(elf_handle particles);
ELF_API int ELF_APIENTRY elfGetParticlesDrawMode(elf_handle particles);
ELF_API elf_handle ELF_APIENTRY elfGetParticlesTexture(elf_handle particles);
ELF_API elf_handle ELF_APIENTRY elfGetParticlesModel(elf_handle particles);
ELF_API elf_handle ELF_APIENTRY elfGetParticlesEntity(elf_handle particles);
ELF_API elf_vec3f ELF_APIENTRY elfGetParticlesGravity(elf_handle particles);
ELF_API float ELF_APIENTRY elfGetParticlesSpawnDelay(elf_handle particles);
ELF_API float ELF_APIENTRY elfGetParticlesSizeMin(elf_handle particles);
ELF_API float ELF_APIENTRY elfGetParticlesSizeMax(elf_handle particles);
ELF_API float ELF_APIENTRY elfGetParticlesSizeGrowthMin(elf_handle particles);
ELF_API float ELF_APIENTRY elfGetParticlesSizeGrowthMax(elf_handle particles);
ELF_API float ELF_APIENTRY elfGetParticlesRotationMin(elf_handle particles);
ELF_API float ELF_APIENTRY elfGetParticlesRotationMax(elf_handle particles);
ELF_API float ELF_APIENTRY elfGetParticlesRotationGrowthMin(elf_handle particles);
ELF_API float ELF_APIENTRY elfGetParticlesRotationGrowthMax(elf_handle particles);
ELF_API float ELF_APIENTRY elfGetParticlesLifeSpanMin(elf_handle particles);
ELF_API float ELF_APIENTRY elfGetParticlesLifeSpanMax(elf_handle particles);
ELF_API float ELF_APIENTRY elfGetParticlesFadeSpeedMin(elf_handle particles);
ELF_API float ELF_APIENTRY elfGetParticlesFadeSpeedMax(elf_handle particles);
ELF_API elf_vec3f ELF_APIENTRY elfGetParticlesPositionMin(elf_handle particles);
ELF_API elf_vec3f ELF_APIENTRY elfGetParticlesPositionMax(elf_handle particles);
ELF_API elf_vec3f ELF_APIENTRY elfGetParticlesVelocityMin(elf_handle particles);
ELF_API elf_vec3f ELF_APIENTRY elfGetParticlesVelocityMax(elf_handle particles);
ELF_API elf_color ELF_APIENTRY elfGetParticlesColorMin(elf_handle particles);
ELF_API elf_color ELF_APIENTRY elfGetParticlesColorMax(elf_handle particles);
ELF_API elf_handle ELF_APIENTRY elfCreateSprite(const char* name);
ELF_API void ELF_APIENTRY elfSetSpriteMaterial(elf_handle sprite, elf_handle material);
ELF_API elf_handle ELF_APIENTRY elfGetSpriteMaterial(elf_handle sprite);
ELF_API void ELF_APIENTRY elfSetSpriteScale(elf_handle sprite, float x, float y, float z);
ELF_API elf_vec3f ELF_APIENTRY elfGetSpriteScale(elf_handle sprite);
ELF_API elf_handle ELF_APIENTRY elfCreateSceneFromFile(const char* file_path);
ELF_API void ELF_APIENTRY elfSetSceneAmbientColor(elf_handle scene, float r, float g, float b, float a);
ELF_API elf_color ELF_APIENTRY elfGetSceneAmbientColor(elf_handle scene);
ELF_API void ELF_APIENTRY elfSetSceneGravity(elf_handle scene, float x, float y, float z);
ELF_API elf_vec3f ELF_APIENTRY elfGetSceneGravity(elf_handle scene);
ELF_API const char* ELF_APIENTRY elfGetSceneName(elf_handle scene);
ELF_API const char* ELF_APIENTRY elfGetSceneFilePath(elf_handle scene);
ELF_API int ELF_APIENTRY elfGetSceneCameraCount(elf_handle scene);
ELF_API int ELF_APIENTRY elfGetSceneEntityCount(elf_handle scene);
ELF_API int ELF_APIENTRY elfGetSceneLightCount(elf_handle scene);
ELF_API int ELF_APIENTRY elfGetSceneArmatureCount(elf_handle scene);
ELF_API int ELF_APIENTRY elfGetSceneParticlesCount(elf_handle scene);
ELF_API int ELF_APIENTRY elfGetSceneSpriteCount(elf_handle scene);
ELF_API void ELF_APIENTRY elfAddCameraToScene(elf_handle scene, elf_handle camera);
ELF_API void ELF_APIENTRY elfAddEntityToScene(elf_handle scene, elf_handle entity);
ELF_API void ELF_APIENTRY elfAddLightToScene(elf_handle scene, elf_handle light);
ELF_API void ELF_APIENTRY elfAddParticlesToScene(elf_handle scene, elf_handle particles);
ELF_API void ELF_APIENTRY elfAddSpriteToScene(elf_handle scene, elf_handle sprite);
ELF_API void ELF_APIENTRY elfSetSceneActiveCamera(elf_handle scene, elf_handle camera);
ELF_API elf_handle ELF_APIENTRY elfGetSceneActiveCamera(elf_handle scene);
ELF_API elf_handle ELF_APIENTRY elfGetSceneRayCastResult(elf_handle scene, float x, float y, float z, float dx, float dy, float dz);
ELF_API elf_handle ELF_APIENTRY elfGetSceneRayCastResults(elf_handle scene, float x, float y, float z, float dx, float dy, float dz);
ELF_API elf_handle ELF_APIENTRY elfGetDebugSceneRayCastResult(elf_handle scene, float x, float y, float z, float dx, float dy, float dz);
ELF_API elf_handle ELF_APIENTRY elfGetDebugSceneRayCastResults(elf_handle scene, float x, float y, float z, float dx, float dy, float dz);
ELF_API elf_handle ELF_APIENTRY elfGetCameraByIndex(elf_handle scene, int idx);
ELF_API elf_handle ELF_APIENTRY elfGetEntityByIndex(elf_handle scene, int idx);
ELF_API elf_handle ELF_APIENTRY elfGetLightByIndex(elf_handle scene, int idx);
ELF_API elf_handle ELF_APIENTRY elfGetArmatureByIndex(elf_handle scene, int idx);
ELF_API elf_handle ELF_APIENTRY elfGetParticlesByIndex(elf_handle scene, int idx);
ELF_API elf_handle ELF_APIENTRY elfGetSpriteByIndex(elf_handle scene, int idx);
ELF_API elf_handle ELF_APIENTRY elfGetTextureByName(elf_handle scene, const char* name);
ELF_API elf_handle ELF_APIENTRY elfGetMaterialByName(elf_handle scene, const char* name);
ELF_API elf_handle ELF_APIENTRY elfGetModelByName(elf_handle scene, const char* name);
ELF_API elf_handle ELF_APIENTRY elfGetScriptByName(elf_handle scene, const char* name);
ELF_API elf_handle ELF_APIENTRY elfGetCameraByName(elf_handle scene, const char* name);
ELF_API elf_handle ELF_APIENTRY elfGetEntityByName(elf_handle scene, const char* name);
ELF_API elf_handle ELF_APIENTRY elfGetLightByName(elf_handle scene, const char* name);
ELF_API elf_handle ELF_APIENTRY elfGetArmatureByName(elf_handle scene, const char* name);
ELF_API elf_handle ELF_APIENTRY elfGetParticlesByName(elf_handle scene, const char* name);
ELF_API elf_handle ELF_APIENTRY elfGetSpriteByName(elf_handle scene, const char* name);
ELF_API elf_handle ELF_APIENTRY elfGetActorByName(elf_handle scene, const char* name);
ELF_API bool ELF_APIENTRY elfRemoveCameraByName(elf_handle scene, const char* name);
ELF_API bool ELF_APIENTRY elfRemoveEntityByName(elf_handle scene, const char* name);
ELF_API bool ELF_APIENTRY elfRemoveLightByName(elf_handle scene, const char* name);
ELF_API bool ELF_APIENTRY elfRemoveParticlesByName(elf_handle scene, const char* name);
ELF_API bool ELF_APIENTRY elfRemoveCameraByIndex(elf_handle scene, int idx);
ELF_API bool ELF_APIENTRY elfRemoveEntityByIndex(elf_handle scene, int idx);
ELF_API bool ELF_APIENTRY elfRemoveLightByIndex(elf_handle scene, int idx);
ELF_API bool ELF_APIENTRY elfRemoveParticlesByIndex(elf_handle scene, int idx);
ELF_API bool ELF_APIENTRY elfRemoveCameraByObject(elf_handle scene, elf_handle camera);
ELF_API bool ELF_APIENTRY elfRemoveEntityByObject(elf_handle scene, elf_handle entity);
ELF_API bool ELF_APIENTRY elfRemoveLightByObject(elf_handle scene, elf_handle light);
ELF_API bool ELF_APIENTRY elfRemoveParticlesByObject(elf_handle scene, elf_handle particles);
ELF_API bool ELF_APIENTRY elfRemoveSpriteByObject(elf_handle scene, elf_handle sprite);
ELF_API bool ELF_APIENTRY elfRemoveActorByObject(elf_handle scene, elf_handle actor);
ELF_API elf_handle ELF_APIENTRY elfCreateScript();
ELF_API elf_handle ELF_APIENTRY elfCreateScriptFromFile(const char* file_path);
ELF_API const char* ELF_APIENTRY elfGetScriptName(elf_handle script);
ELF_API const char* ELF_APIENTRY elfGetScriptFilePath(elf_handle script);
ELF_API void ELF_APIENTRY elfSetScriptText(elf_handle script, const char* text);
ELF_API bool ELF_APIENTRY elfIsScriptError(elf_handle script);
ELF_API bool ELF_APIENTRY elfRunString(const char* str);
ELF_API bool ELF_APIENTRY elfRunScript(elf_handle script);
ELF_API void ELF_APIENTRY elfSetAudioVolume(float volume);
ELF_API float ELF_APIENTRY elfGetAudioVolume();
ELF_API void ELF_APIENTRY elfSetAudioRolloff(float rolloff);
ELF_API float ELF_APIENTRY elfGetAudioRolloff();
ELF_API elf_handle ELF_APIENTRY elfLoadSound(const char* file_path);
ELF_API elf_handle ELF_APIENTRY elfLoadStreamedSound(const char* file_path);
ELF_API int ELF_APIENTRY elfGetSoundFileType(elf_handle sound);
ELF_API elf_handle ELF_APIENTRY elfPlaySound(elf_handle sound, float volume);
ELF_API elf_handle ELF_APIENTRY elfPlayEntitySound(elf_handle entity, elf_handle sound, float volume);
ELF_API elf_handle ELF_APIENTRY elfLoopSound(elf_handle sound, float volume);
ELF_API elf_handle ELF_APIENTRY elfLoopEntitySound(elf_handle entity, elf_handle sound, float volume);
ELF_API void ELF_APIENTRY elfSetSoundVolume(elf_handle source, float volume);
ELF_API float ELF_APIENTRY elfGetSoundVolume(elf_handle source);
ELF_API void ELF_APIENTRY elfPauseSound(elf_handle source);
ELF_API void ELF_APIENTRY elfResumeSound(elf_handle source);
ELF_API void ELF_APIENTRY elfStopSound(elf_handle source);
ELF_API bool ELF_APIENTRY elfIsSoundPlaying(elf_handle source);
ELF_API bool ELF_APIENTRY elfIsSoundPaused(elf_handle source);
ELF_API elf_handle ELF_APIENTRY elfGetCollisionActor(elf_handle collision);
ELF_API elf_vec3f ELF_APIENTRY elfGetCollisionPosition(elf_handle collision);
ELF_API elf_vec3f ELF_APIENTRY elfGetCollisionNormal(elf_handle collision);
ELF_API float ELF_APIENTRY elfGetCollisionDepth(elf_handle collision);
ELF_API const char* ELF_APIENTRY elfGetJointName(elf_handle joint);
ELF_API int ELF_APIENTRY elfGetJointType(elf_handle joint);
ELF_API elf_handle ELF_APIENTRY elfGetJointActorA(elf_handle joint);
ELF_API elf_handle ELF_APIENTRY elfGetJointActorB(elf_handle joint);
ELF_API elf_vec3f ELF_APIENTRY elfGetJointPivot(elf_handle joint);
ELF_API elf_vec3f ELF_APIENTRY elfGetJointAxis(elf_handle joint);
ELF_API elf_handle ELF_APIENTRY elfCreateFontFromFile(const char* file_path, int size);
ELF_API const char* ELF_APIENTRY elfGetFontName(elf_handle font);
ELF_API const char* ELF_APIENTRY elfGetFontFilePath(elf_handle font);
ELF_API int ELF_APIENTRY elfGetFontSize(elf_handle font);
ELF_API int ELF_APIENTRY elfGetStringWidth(elf_handle font, const char* str);
ELF_API int ELF_APIENTRY elfGetStringHeight(elf_handle font, const char* str);
ELF_API const char* ELF_APIENTRY elfGetGuiObjectName(elf_handle object);
ELF_API elf_vec2i ELF_APIENTRY elfGetGuiObjectPosition(elf_handle object);
ELF_API elf_vec2i ELF_APIENTRY elfGetGuiObjectSize(elf_handle object);
ELF_API elf_color ELF_APIENTRY elfGetGuiObjectColor(elf_handle object);
ELF_API bool ELF_APIENTRY elfGetGuiObjectVisible(elf_handle object);
ELF_API elf_handle ELF_APIENTRY elfGetGuiObjectScript(elf_handle object);
ELF_API int ELF_APIENTRY elfGetGuiObjectEvent(elf_handle object);
ELF_API void ELF_APIENTRY elfSetGuiObjectPosition(elf_handle object, float x, float y);
ELF_API void ELF_APIENTRY elfSetGuiObjectColor(elf_handle object, float r, float g, float b, float a);
ELF_API void ELF_APIENTRY elfSetGuiObjectVisible(elf_handle object, bool visible);
ELF_API void ELF_APIENTRY elfSetGuiObjectScript(elf_handle object, elf_handle script);
ELF_API elf_handle ELF_APIENTRY elfCreateLabel(const char* name);
ELF_API elf_handle ELF_APIENTRY elfGetLabelFont(elf_handle label);
ELF_API const char* ELF_APIENTRY elfGetLabelText(elf_handle label);
ELF_API void ELF_APIENTRY elfSetLabelFont(elf_handle label, elf_handle font);
ELF_API void ELF_APIENTRY elfSetLabelText(elf_handle label, const char* text);
ELF_API elf_handle ELF_APIENTRY elfCreateButton(const char* name);
ELF_API bool ELF_APIENTRY elfGetButtonState(elf_handle button);
ELF_API elf_handle ELF_APIENTRY elfGetButtonOffTexture(elf_handle button);
ELF_API elf_handle ELF_APIENTRY elfGetButtonOverTexture(elf_handle button);
ELF_API elf_handle ELF_APIENTRY elfGetButtonOnTexture(elf_handle button);
ELF_API void ELF_APIENTRY elfSetButtonOffTexture(elf_handle button, elf_handle off);
ELF_API void ELF_APIENTRY elfSetButtonOverTexture(elf_handle button, elf_handle over);
ELF_API void ELF_APIENTRY elfSetButtonOnTexture(elf_handle button, elf_handle on);
ELF_API elf_handle ELF_APIENTRY elfCreatePicture(const char* name);
ELF_API elf_handle ELF_APIENTRY elfGetPictureTexture(elf_handle picture);
ELF_API void ELF_APIENTRY elfSetPictureTexture(elf_handle picture, elf_handle texture);
ELF_API elf_handle ELF_APIENTRY elfCreateTextField(const char* name);
ELF_API elf_handle ELF_APIENTRY elfGetTextFieldTexture(elf_handle text_field);
ELF_API elf_handle ELF_APIENTRY elfGetTextFieldFont(elf_handle text_field);
ELF_API elf_color ELF_APIENTRY elfGetTextFieldTextColor(elf_handle text_field);
ELF_API elf_vec2i ELF_APIENTRY elfGetTextFieldOffset(elf_handle text_field);
ELF_API const char* ELF_APIENTRY elfGetTextFieldText(elf_handle text_field);
ELF_API void ELF_APIENTRY elfSetTextFieldTexture(elf_handle text_field, elf_handle texture);
ELF_API void ELF_APIENTRY elfSetTextFieldFont(elf_handle text_field, elf_handle font);
ELF_API void ELF_APIENTRY elfSetTextFieldTextColor(elf_handle text_field, float r, float g, float b, float a);
ELF_API void ELF_APIENTRY elfSetTextFieldOffset(elf_handle text_field, int offset_x, int offset_y);
ELF_API void ELF_APIENTRY elfSetTextFieldText(elf_handle text_field, const char* text);
ELF_API elf_handle ELF_APIENTRY elfCreateSlider(const char* name);
ELF_API elf_handle ELF_APIENTRY elfGetSliderBackgroundTexture(elf_handle slider);
ELF_API elf_handle ELF_APIENTRY elfGetSliderSliderTexture(elf_handle slider);
ELF_API float ELF_APIENTRY elfGetSliderValue(elf_handle slider);
ELF_API void ELF_APIENTRY elfSetSliderBackgroundTexture(elf_handle slider, elf_handle background);
ELF_API void ELF_APIENTRY elfSetSliderSliderTexture(elf_handle slider, elf_handle slider_texture);
ELF_API void ELF_APIENTRY elfSetSliderValue(elf_handle slider, float value);
ELF_API elf_handle ELF_APIENTRY elfCreateScreen(const char* name);
ELF_API elf_handle ELF_APIENTRY elfGetScreenTexture(elf_handle screen);
ELF_API void ELF_APIENTRY elfSetScreenTexture(elf_handle screen, elf_handle texture);
ELF_API elf_handle ELF_APIENTRY elfCreateTextList(const char* name);
ELF_API elf_handle ELF_APIENTRY elfGetTextListFont(elf_handle text_list);
ELF_API elf_color ELF_APIENTRY elfGetTextListSelectionColor(elf_handle text_list);
ELF_API int ELF_APIENTRY elfGetTextListRowCount(elf_handle text_list);
ELF_API int ELF_APIENTRY elfGetTextListItemCount(elf_handle text_list);
ELF_API int ELF_APIENTRY elfGetTextListSelectionIndex(elf_handle text_list);
ELF_API int ELF_APIENTRY elfGetTextListOffset(elf_handle text_list);
ELF_API const char* ELF_APIENTRY elfGetTextListItem(elf_handle text_list, int idx);
ELF_API const char* ELF_APIENTRY elfGetTextListSelectedItem(elf_handle text_list);
ELF_API void ELF_APIENTRY elfSetTextListFont(elf_handle text_list, elf_handle font);
ELF_API void ELF_APIENTRY elfSetTextListSelectionColor(elf_handle text_list, float r, float g, float b, float a);
ELF_API void ELF_APIENTRY elfSetTextListSize(elf_handle text_list, int rows, int width);
ELF_API void ELF_APIENTRY elfAddTextListItem(elf_handle text_list, const char* text);
ELF_API bool ELF_APIENTRY elfRemoveTextListItem(elf_handle text_list, int idx);
ELF_API void ELF_APIENTRY elfRemoveTextListItems(elf_handle text_list);
ELF_API void ELF_APIENTRY elfSetTextListOffset(elf_handle text_list, int offset);
ELF_API void ELF_APIENTRY elfSetTextListSelection(elf_handle text_list, int selection);
ELF_API elf_handle ELF_APIENTRY elfCreateCheckBox(const char* name);
ELF_API bool ELF_APIENTRY elfGetCheckBoxState(elf_handle check_box);
ELF_API elf_handle ELF_APIENTRY elfGetCheckBoxOffTexture(elf_handle check_box);
ELF_API elf_handle ELF_APIENTRY elfGetCheckBoxOnTexture(elf_handle check_box);
ELF_API void ELF_APIENTRY elfSetCheckBoxOffTexture(elf_handle check_box, elf_handle off);
ELF_API void ELF_APIENTRY elfSetCheckBoxOnTexture(elf_handle check_box, elf_handle on);
ELF_API void ELF_APIENTRY elfSetCheckBoxState(elf_handle check_box, bool state);
ELF_API elf_handle ELF_APIENTRY elfCreateGui();
ELF_API bool ELF_APIENTRY elfAddGuiObject(elf_handle parent, elf_handle object);
ELF_API elf_handle ELF_APIENTRY elfGetGuiObjectByName(elf_handle parent, const char* name);
ELF_API elf_handle ELF_APIENTRY elfGetGuiObjectByIndex(elf_handle parent, int idx);
ELF_API bool ELF_APIENTRY elfRemoveGuiObjectByName(elf_handle parent, const char* name);
ELF_API bool ELF_APIENTRY elfRemoveGuiObjectByIndex(elf_handle parent, int idx);
ELF_API bool ELF_APIENTRY elfRemoveGuiObjectByObject(elf_handle parent, elf_handle object);
ELF_API void ELF_APIENTRY elfEmptyGui(elf_handle gui);
ELF_API bool ELF_APIENTRY elfCreateSession(const char* address, unsigned short port);
ELF_API bool ELF_APIENTRY elfConnectSession(const char* address, unsigned short port);
ELF_API bool ELF_APIENTRY elfDisconnectSession();
ELF_API void ELF_APIENTRY elfSendStringToClients(const char* message);
ELF_API void ELF_APIENTRY elfSendStringToServer(const char* message);
ELF_API const char* ELF_APIENTRY elfGetServerDataAsString();
ELF_API const char* ELF_APIENTRY elfGetClientDataAsString();
ELF_API int ELF_APIENTRY elfGetServerEvent();
ELF_API int ELF_APIENTRY elfGetClientEvent();
ELF_API int ELF_APIENTRY elfGetCurrentClient();
ELF_API bool ELF_APIENTRY elfIsServer();
ELF_API bool ELF_APIENTRY elfIsClient();
#endif
