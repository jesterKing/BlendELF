
#define ELF_OBJECT_HEADER \
	int type; \
	int ref_count

#define ELF_ACTOR_HEADER \
	ELF_OBJECT_HEADER; \
	char *name; \
	char *file_path; \
	elf_scene *scene; \
	gfx_transform *transform; \
	elf_physics_object *object; \
	elf_physics_object *dobject; \
	elf_list *joints; \
	elf_script *script; \
	elf_actor *parent; \
	elf_list *children; \
	elf_list *sources; \
	elf_ipo *ipo; \
	elf_frame_player *ipo_player; \
	elf_list *properties; \
	unsigned char moved; \
	unsigned char selected

#define ELF_GUI_OBJECT_HEADER \
	ELF_OBJECT_HEADER; \
	char *name; \
	elf_vec2i pos; \
	elf_vec2i rel_pos; \
	int width, height; \
	elf_color color; \
	unsigned char visible; \
	elf_gui *root; \
	elf_gui_object *parent; \
	elf_list *children; \
	elf_list *screens; \
	elf_script *script; \
	int event

struct elf_object {
	ELF_OBJECT_HEADER;
};

struct elf_string {
	ELF_OBJECT_HEADER;
	char *str;
};

typedef struct elf_list_ptr {
	struct elf_list_ptr *prev;
	struct elf_list_ptr *next;
	elf_object *obj;
} elf_list_ptr;

struct elf_list {
	ELF_OBJECT_HEADER;
	elf_list_ptr *first;
	elf_list_ptr *last;
	elf_list_ptr *cur;
	elf_list_ptr *next;
	int length;
};

struct elf_key_event {
	ELF_OBJECT_HEADER;
	int key;
	unsigned char state;
};

struct elf_char_event {
	ELF_OBJECT_HEADER;
	int code;
	unsigned char state;
};

typedef struct elf_joystick {
	unsigned char present;
	float axis_pos[2];
	unsigned char cur_buts[16];
	unsigned char prv_buts[16];
} elf_joystick;

struct elf_context {
	ELF_OBJECT_HEADER;
	int width;
	int height;
	unsigned char fullscreen;
	char *title;
	int multisamples;
	unsigned char cur_keys[256];
	unsigned char prv_keys[256];
	int mouse_position[2];
	int prv_mouse_position[2];
	unsigned char hide_mouse;
	int mouse_wheel;
	unsigned char cur_mbuts[3];
	unsigned char prv_mbuts[3];
	elf_joystick joysticks[16];
	elf_list *events;
};

struct elf_engine {
	ELF_OBJECT_HEADER;
	struct lua_State *L;

	unsigned char free_run;

	int fps;
	elf_timer *fps_timer;
	elf_timer *fps_limit_timer;
	elf_timer *time_sync_timer;
	unsigned int frames;
	int fps_limit;
	float tick_rate;
	float speed;
	float sync;
	unsigned char f10_exit;
	unsigned char quit;

	elf_post_process *post_process;

	gfx_texture *shadow_map;
	gfx_render_target *shadow_target;
	int shadow_map_size;
	float texture_anisotropy;
	unsigned char occlusion_culling;
	unsigned char debug_draw;
	unsigned char non_lit_flag;
	elf_color ambient_color;

	gfx_vertex_data *lines;

	elf_scene *scene;
	elf_gui *gui;

	elf_object *actor;
};

struct elf_game_config {
	ELF_OBJECT_HEADER;
	int window_size[2];
	unsigned char fullscreen;
	float texture_anisotropy;
	int shadow_map_size;
	char *start;
};

struct elf_resources {
	ELF_OBJECT_HEADER;
	elf_list *textures;
	elf_list *models;
};

struct elf_directory {
	ELF_OBJECT_HEADER;
	char *path;
	elf_list *items;
};

struct elf_directory_item {
	ELF_OBJECT_HEADER;
	char *name;
	unsigned char item_type;
};

struct elf_frame_player {
	ELF_OBJECT_HEADER;
	float start;
	float end;
	float cur_frame;
	float speed;
	unsigned char pause;
	unsigned char loop;
	void *user_data;
	void (*callback)(elf_frame_player *);
};

struct elf_timer {
	ELF_OBJECT_HEADER;
	double start;
};

struct elf_image {
	ELF_OBJECT_HEADER;
	int width;
	int height;
	unsigned char bpp;
	void *data;
};

struct elf_texture {
	ELF_OBJECT_HEADER;
	char *name;
	char *file_path;
	gfx_texture *texture;
};

struct elf_material {
	ELF_OBJECT_HEADER;
	char *name;
	char *file_path;
	elf_color diffuse_color;
	elf_color ambient_color;
	elf_color specular_color;
	float spec_power;
	unsigned char lighting;
	unsigned char non_lit_flag;
	int texture_types[GFX_MAX_TEXTURES];
	float texture_parallax_scales[GFX_MAX_TEXTURES];
	elf_texture *textures[GFX_MAX_TEXTURES];
};

struct elf_bezier_point {
	ELF_OBJECT_HEADER;
	elf_vec2f c1;
	elf_vec2f p;
	elf_vec2f c2;
};

struct elf_bezier_curve {
	ELF_OBJECT_HEADER;
	unsigned char curve_type;
	unsigned char interpolation;
	elf_list *points;
};

struct elf_ipo {
	ELF_OBJECT_HEADER;
	elf_list *curves;
	unsigned char loc;
	unsigned char rot;
	unsigned char scale;
	unsigned char qua;
};

struct elf_property {
	ELF_OBJECT_HEADER;
	char *name;
	unsigned char property_type;
	int ival;
	float fval;
	char *sval;
	unsigned char bval;
};

struct elf_actor {
	ELF_ACTOR_HEADER;
};

struct elf_camera {
	ELF_ACTOR_HEADER;
	int mode;
	int viewp_x, viewp_y;
	int viewp_width, viewp_height;
	elf_vec3f position;
	float fov;
	float aspect;
	int ortho_x, ortho_y;
	int ortho_width, ortho_height;
	float clip_far, clip_near;
	float far_plane_width, far_plane_height;
	float frustum[6][4];
	float projection_matrix[16];
	float modelview_matrix[16];
};

typedef struct elf_frame {
	gfx_vertex_data *vertices;
} elf_frame;

typedef struct elf_model_area {
	unsigned int indice_count;
	gfx_vertex_data *index;
	gfx_vertex_index *vertex_index;
	unsigned int material_number;
} elf_model_area;

struct elf_model {
	ELF_OBJECT_HEADER;
	char *name;
	char *file_path;
	int vertice_count;
	int frame_count;
	int area_count;
	int indice_count;
	elf_frame *frames;
	gfx_vertex_array *vertex_array;
	gfx_vertex_data *normals;
	gfx_vertex_data *tex_coords;
	gfx_vertex_data *tangents;
	unsigned int *index;
	float *weights;
	int *boneids;
	elf_physics_tri_mesh *tri_mesh;
	elf_model_area *areas;
	elf_vec3f bb_min;
	elf_vec3f bb_max;
	unsigned char non_lit_flag;
};

struct elf_entity {
	ELF_ACTOR_HEADER;

	elf_model *model;

	elf_armature *armature;
	gfx_vertex_data *vertices;
	gfx_vertex_data *normals;

	elf_list *materials;
	elf_frame_player *armature_player;
	float prev_armature_frame;

	elf_vec3f position;
	elf_vec3f scale;
	elf_vec3f bb_min;
	elf_vec3f bb_max;
	elf_vec3f bb_offset;
	elf_vec3f bb_half_length;
	elf_vec3f cull_aabb_min;
	elf_vec3f cull_aabb_max;
	float cull_radius;

	gfx_query *query;
	unsigned char visible;
	unsigned char culled;
	unsigned char non_lit_flag;
};

struct elf_light {
	ELF_ACTOR_HEADER;
	int light_type;
	elf_color color;
	float distance;
	float fade_speed;
	float inner_cone;
	float outer_cone;
	unsigned char visible;
	unsigned char shaft;
	float shaft_size;
	float shaft_intensity;
	float shaft_fade_off;

	unsigned char shadow_caster;
	elf_camera *shadow_camera;

	float projection_matrix[16];
};

typedef struct elf_bone_frame {
	elf_vec3f pos;
	elf_vec4f qua;
	elf_vec3f offset_pos;
	elf_vec4f offset_qua;
} elf_bone_frame;

struct elf_bone {
	ELF_OBJECT_HEADER;
	char *name;
	elf_bone *parent;
	int id;
	elf_vec3f pos;
	elf_vec4f qua;
	elf_vec3f cur_pos;
	elf_vec4f cur_qua;
	elf_vec3f cur_offset_pos;
	elf_vec4f cur_offset_qua;
	elf_bone_frame *frames;
	elf_list *children;
	elf_armature *armature;
};

struct elf_armature {
	ELF_OBJECT_HEADER;
	char *name;
	char *file_path;
	int frame_count;
	int bone_count;
	elf_list *root_bones;
	elf_bone **bones;
	float cur_frame;
	elf_vec3f bb_min;
	elf_vec3f bb_max;
};

struct elf_particle {
	ELF_OBJECT_HEADER;
	float size;
	float size_growth;
	float rotation;
	float rotation_growth;
	elf_color color;
	float life_span;
	float fade_speed;
	elf_vec3f position;
	elf_vec3f velocity;
};

struct elf_particles {
	ELF_ACTOR_HEADER;

	int max_count;
	unsigned char draw_mode;
	elf_list *particles;
	elf_texture *texture;
	elf_model *model;
	gfx_vertex_array *vertex_array;
	gfx_vertex_data *vertices;
	gfx_vertex_data *tex_coords;
	gfx_vertex_data *colors;

	float spawn_delay;
	float cur_time;
	elf_vec3f gravity;
	float size_min;
	float size_max;
	float size_growth_min;
	float size_growth_max;
	float rotation_min;
	float rotation_max;
	float rotation_growth_min;
	float rotation_growth_max;
	float life_span_min;
	float life_span_max;
	float fade_speed_min;
	float fade_speed_max;
	elf_vec3f velocity_min;
	elf_vec3f velocity_max;
	elf_vec3f position_min;
	elf_vec3f position_max;
	elf_color color_min;
	elf_color color_max;
};

struct elf_scene {
	ELF_OBJECT_HEADER;
	char *name;
	char *file_path;
	elf_color ambient_color;

	elf_list *models;
	elf_list *scripts;
	elf_list *materials;
	elf_list *textures;
	elf_list *cameras;
	elf_list *entities;
	elf_list *lights;
	elf_list *armatures;
	elf_list *particles;

	elf_list *entity_queue;
	int entity_queue_count;

	elf_physics_world *world;
	elf_physics_world *dworld;

	elf_camera *cur_camera;

	gfx_shader_params shader_params;

	gfx_shader_program *dof_depth_write;
	gfx_shader_program *dof_depth_write_alpha;

	elf_pak *pak;
};

struct elf_pak_index {
	ELF_OBJECT_HEADER;
	unsigned char index_type;
	char *name;
	unsigned int offset;
};

struct elf_pak {
	ELF_OBJECT_HEADER;
	char *file_path;
	elf_list *indexes;

	int texture_count;
	int material_count;
	int model_count;
	int camera_count;
	int entity_count;
	int light_count;
	int armature_count;
	int scene_count;
	int script_count;
};

struct elf_post_process {
	ELF_OBJECT_HEADER;

	gfx_texture *main_rt_color[2];
	gfx_texture *main_rt_depth;
	gfx_texture *main_rt_ssao_depth;
	gfx_render_target *main_rt;

	gfx_texture *rt_tex_high_1;
	gfx_texture *rt_tex_high_2;
	gfx_texture *rt_tex_high_depth;
	gfx_texture *rt_tex_med_1;
	gfx_texture *rt_tex_med_2;
	gfx_texture *rt_tex_med_3;
	gfx_texture *rt_tex_low_1;
	gfx_texture *rt_tex_low_2;
	gfx_texture *rt_tex_tiny_1;
	gfx_texture *rt_tex_tiny_2;

	gfx_render_target *rt_high;
	gfx_render_target *rt_med;
	gfx_render_target *rt_low;
	gfx_render_target *rt_tiny;
	
	gfx_shader_program *hipass_shdr;
	gfx_shader_program *blur_shdr;
	gfx_shader_program *bloom_combine_shdr;
	gfx_shader_program *dof_combine_shdr;
	gfx_shader_program *ssao_shdr;
	gfx_shader_program *ssao_combine_shdr;
	gfx_shader_program *light_shaft_shdr;

	unsigned char dof;
	float dof_focal_range;
	float dof_focal_distance;
	unsigned char dof_auto_focus;

	unsigned char ssao;
	float ssao_amount;

	unsigned char bloom;
	float bloom_threshold;

	unsigned char light_shafts;
	float light_shafts_intensity;
	gfx_transform *light_shaft_transform;

	int buffer_width;
	int buffer_height;

	gfx_shader_params shader_params;
};

struct elf_script {
	ELF_OBJECT_HEADER;
	char *name;
	char *file_path;
	char *text;
	unsigned char error;
};

typedef struct elf_character {
	char code;
	gfx_texture *texture;
	int offset_x, offset_y;
} elf_character;

struct elf_font {
	ELF_OBJECT_HEADER;
	char *name;
	char *file_path;
	int size;
	elf_character chars[128];
	int offset_y;
};

struct elf_area {
	elf_vec2i pos;
	elf_vec2i size;
};

struct elf_gui_object {
	ELF_GUI_OBJECT_HEADER;
};

struct elf_label {
	ELF_GUI_OBJECT_HEADER;
	elf_font *font;
	char *text;
};

struct elf_button {
	ELF_GUI_OBJECT_HEADER;
	unsigned char state;
	elf_texture *off;
	elf_texture *over;
	elf_texture *on;
};

struct elf_picture {
	ELF_GUI_OBJECT_HEADER;
	elf_texture *texture;
};

struct elf_text_field {
	ELF_GUI_OBJECT_HEADER;
	elf_texture *texture;
	elf_font *font;
	int offset_x, offset_y;
	elf_color text_color;
	int cursor_pos;
	int draw_pos;
	int draw_offset;
	char *text;
};

struct elf_slider {
	ELF_GUI_OBJECT_HEADER;
	elf_texture *background;
	elf_texture *slider;
	float value;
};

struct elf_screen {
	ELF_GUI_OBJECT_HEADER;
	elf_texture *texture;
};

struct elf_text_list {
	ELF_GUI_OBJECT_HEADER;
	elf_color selection_color;
	elf_font *font;
	elf_list *items;
	int rows;
	int list_width;
	int selection;
	int offset;
};

struct elf_check_box {
	ELF_GUI_OBJECT_HEADER;
	unsigned char state;
	elf_texture *off;
	elf_texture *on;
};

struct elf_gui {
	ELF_GUI_OBJECT_HEADER;
	elf_font *def_font;
	gfx_shader_params shader_params;
	elf_gui_object *trace;
	elf_gui_object *target;
	elf_text_field *active_text_field;

	int cur_key;
	float key_step;
	unsigned char key_repeat;

	char cur_char;
	float char_step;
	unsigned char char_repeat;
};

struct elf_server {
	ENetHost* host;
	ENetAddress address;
	ENetEvent event;
	GLFWthread thread;
};

struct elf_client {
	ELF_OBJECT_HEADER;
};

struct elf_networking_event {
	ELF_OBJECT_HEADER;
	int event_type;
	elf_client* client;
};

