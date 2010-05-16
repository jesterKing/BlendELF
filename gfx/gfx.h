
#ifdef __cplusplus
extern "C" {
#endif

#define GFX_NONE					0x0000

#define GFX_FALSE					0x0000
#define GFX_TRUE					0x0001

#define GFX_VERTEX_DATA					0x0000
#define GFX_VERTEX_ARRAY				0x0001
#define GFX_VERTEX_INDEX				0x0002
#define GFX_OBJECT_TYPE_COUNT				0x0003

#define GFX_FLOAT					0x0000
#define GFX_INT						0x0001
#define GFX_UINT					0x0002
#define GFX_SHORT					0x0003
#define GFX_USHORT					0x0004
#define GFX_BYTE					0x0005
#define GFX_UBYTE					0x0006
#define GFX_MAX_FORMATS					0x0007

#define GFX_VERTEX					0x0000
#define GFX_NORMAL					0x0001
#define GFX_TEX_COORD					0x0002
#define GFX_COLOR					0x0003
#define GFX_TANGENT					0x0004
#define GFX_WEIGHTS					0x0005
#define GFX_BONEIDS					0x0006
#define GFX_MAX_VERTEX_ARRAYS				0x0007

#define GFX_POINTS					0x0000
#define GFX_LINES					0x0001
#define GFX_LINE_LOOP					0x0002
#define GFX_LINE_STRIP					0x0003
#define GFX_TRIANGLES					0x0004
#define GFX_TRIANGLE_STRIP				0x0005
#define GFX_TRIANGLE_FAN				0x0006
#define GFX_MAX_DRAW_MODES				0x0007

#define GFX_LUMINANCE					0x0000
#define GFX_LUMINANCE_ALPHA				0x0001
#define GFX_RGB						0x0002
#define GFX_RGBA					0x0003
#define GFX_BGR						0x0004
#define GFX_BGRA					0x0005
#define GFX_RGB16F					0x0006
#define GFX_RGB32F					0x0007
#define GFX_RGBA16F					0x0008
#define GFX_RGBA32F					0x0009
#define GFX_ALPHA32F					0x000A
#define GFX_DEPTH_COMPONENT				0x000B
#define GFX_COMPRESSED_RGB				0x000C
#define GFX_COMPRESSED_RGBA				0x000D
#define GFX_MAX_TEXTURE_FORMATS				0x000E

#define GFX_CLAMP					0x0000
#define GFX_REPEAT					0x0001

#define GFX_NEAREST					0x0000
#define GFX_LINEAR					0x0001

#define GFX_NONE					0x0000
#define GFX_SHADOW_PROJECTION				0x0001

#define GFX_MAX_TEXTURES				0x0008

#define GFX_NEVER					0x0000
#define GFX_LESS					0x0001
#define GFX_EQUAL					0x0002
#define GFX_LEQUAL					0x0003
#define GFX_GREATER					0x0004
#define GFX_NOTEQUAL					0x0005
#define GFX_GEQUAL					0x0006
#define GFX_ALWAYS					0x0007

#define GFX_BACK					0x0000
#define GFX_FRONT					0x0001

#define GFX_COUNTER_CLOCK_WISE				0x0000
#define GFX_CLOCK_WISE					0x0001

//#define GFX_NONE					0x0000
#define GFX_TRANSPARENT					0x0001
#define GFX_ADD						0x0002
#define GFX_MULTIPLY					0x0003
#define GFX_SUBTRACT					0x0004

#define GFX_COLOR_MAP					0x0001
#define GFX_NORMAL_MAP					0x0002
#define GFX_HEIGHT_MAP					0x0004
#define GFX_SPECULAR_MAP				0x0008
#define GFX_DETAIL_MAP					0x0010
#define GFX_SHADOW_MAP					0x0020
#define GFX_COLOR_RAMP_MAP				0x0040
#define GFX_LIGHT_MAP					0x0080

#define GFX_NONE					0x0000
#define GFX_POINT_LIGHT					0x0001
#define GFX_SUN_LIGHT					0x0002
#define GFX_SPOT_LIGHT					0x0003

#define GFX_VERTEX_DATA_STATIC				0x0000
#define GFX_VERTEX_DATA_DYNAMIC				0x0001
#define GFX_MAX_VERTEX_DATA_TYPES			0x0002

#define GFX_MAX_CIRCLE_VERTICES				255

#define GFX_PI 3.14159265
#define GFX_PI_DIV_180					GFX_PI/180.0
#define GFX_180_DIV_PI					180.0/GFX_PI

typedef struct gfx_object				gfx_object;
typedef struct gfx_driver				gfx_driver;
typedef struct gfx_transform				gfx_transform;
typedef struct gfx_vertex_data				gfx_vertex_data;
typedef struct gfx_vertex_array				gfx_vertex_array;
typedef struct gfx_vertex_index				gfx_vertex_index;
typedef struct gfx_texture				gfx_texture;
typedef struct gfx_shader_program			gfx_shader_program;
typedef struct gfx_render_target			gfx_render_target;
typedef struct gfx_query				gfx_query;

typedef struct gfx_color {
	float r, g, b, a;
} gfx_color;

typedef struct gfx_position {
	float x, y, z;
} gfx_position;

typedef struct gfx_normal {
	float x, y, z;
} gfx_normal;

typedef struct gfx_render_params {
	unsigned char depth_test;
	unsigned char depth_write;
	unsigned char depth_func;
	unsigned char color_write;
	unsigned char alpha_write;
	unsigned char alpha_test;
	float alpha_threshold;
	unsigned char alpha_test_in_shader;
	unsigned char cull_face;
	unsigned char blend_mode;
	float offset_scale;
	float offset_bias;
	float line_width;
	unsigned char line_smooth;
	unsigned char cull_face_mode;
	unsigned char front_face;
	unsigned char wireframe;
	unsigned char vertex_color;
} gfx_render_params;

typedef struct gfx_material_params {
	gfx_color color;
	gfx_color specular_color;
	float spec_power;
} gfx_material_params;

typedef struct gfx_texture_params {
	int type;
	gfx_texture *texture;
	int projection_mode;
	float parallax_scale;
	float matrix[16];
} gfx_texture_params;

typedef struct gfx_light_params {
	unsigned char type;
	unsigned char mode;
	gfx_position position;
	gfx_normal direction;
	gfx_color color;
	float distance;
	float fade_speed;
	float inner_cone;
	float outer_cone;
} gfx_light_params;

typedef struct gfx_shader_params {
	gfx_render_params render_params;
	gfx_material_params material_params;
	gfx_texture_params texture_params[GFX_MAX_TEXTURES];
	gfx_light_params light_params;
	float camera_matrix[16];
	float clip_start;
	float clip_end;
	int viewport_width;
	int viewport_height;
	float projection_matrix[16];
	float modelview_matrix[16];
	gfx_shader_program *shader_program;
} gfx_shader_params;

typedef struct gfx_shader_config {
	unsigned int textures;
	unsigned char light;
	unsigned char vertex_color;
	unsigned char alpha_test_in_shader;
} gfx_shader_config;

//////////////////////////////// LOG ////////////////////////////////

void gfx_write_to_log(const char *fmt, ...);

//////////////////////////////// OBJECT ////////////////////////////////

void gfx_inc_ref(gfx_object *obj);
void gfx_dec_ref(gfx_object *obj);
void gfx_log_ref_count_table();
int gfx_get_object_type(gfx_object *obj);
int gfx_get_object_ref_count(gfx_object *obj);
int gfx_get_global_ref_count();
int gfx_get_global_obj_count();

//////////////////////////////// MATH ////////////////////////////////

void gfx_vec_normalize(float *vec);
void gfx_vec_dot_vec(float *vec1, float *vec2, float *dot);
void gfx_vec_cross_product(float *vec1, float *vec2, float *vec3);
void gfx_qua_normalize(float *qua, float *result);
void gfx_mul_qua_vec(float *qua, float *vec1, float *vec2);
void gfx_mul_qua_qua(float *qua1, float *qua2, float *qua3);
void gfx_qua_slerp(float *qa, float* qb, double t, float *result);
void gfx_mul_matrix4_vec3(float *m1, float *vec1, float *vec2);
void gfx_mul_matrix4_vec4(float *m1, float *vec1, float *vec2);
void gfx_mul_matrix4_matrix4(float *m1, float *m2, float *m3);
void gfx_qua_from_angle_axis(float angle, float *axis, float *qua);
void gfx_qua_to_matrix4(float *qua, float *mat);
void gfx_qua_to_euler(float *qua, float *euler);
void gfx_qua_get_inverse(float *qua, float *invqua);
void gfx_qua_set_identity(float *qua);
void gfx_matrix4_set_identity(float *mat);
void gfx_matrix4_transpose(float *mat1, float *mat2);
void gfx_matrix4_lol_invert(float *mat1, float *mat2);
void gfx_matrix4_get_inverse(float *mat1, float *mat2);
void gfx_set_qua_rotation(float x, float y, float z, float *qua);
void gfx_rotate_qua(float x, float y, float z, float* qua);
void gfx_rotate_qua_local(float x, float y, float z, float* qua);
void gfx_mul_qua_vec(float *qua, float *vec1, float *vec2);
float gfx_vec_length(float *vec);

//////////////////////////////// TRANSFORM ////////////////////////////////

void gfx_set_viewport(int x, int y, int width, int height);
void gfx_get_perspective_projection_matrix(float fov, float aspect, float near, float far, float *mat);
void gfx_get_orthographic_projection_matrix(float left, float right, float bottom, float top, float near, float far, float *matrix);
void gfx_get_frustum(float proj[16], float modl[16], float frustum[6][4]);
void gfx_set_perspective(float fov, float aspect, float near, float far);
void gfx_set_orthographic(float left, float right, float bottom, float top, float near, float far);

void gfx_project(float x, float y, float z, float modl[16], float proj[16], int viewport[4], float win_coord[3]);
void gfx_un_project(float x, float y, float z, float modl[16], float proj[16], int viewport[4], float obj_coord[3]);

void gfx_recalc_transform_matrix(gfx_transform *transform);
float* gfx_get_transform_matrix(gfx_transform *transform);
gfx_transform* gfx_create_camera_transform();
gfx_transform* gfx_create_object_transform();
void gfx_destroy_transform(gfx_transform *transform);

void gfx_set_transform_position(gfx_transform *transform, float x, float y, float z);
void gfx_set_transform_rotation(gfx_transform *transform, float x, float y, float z);
void gfx_set_transform_scale(gfx_transform *transform, float x, float y, float z);
void gfx_set_transform_orientation(gfx_transform *transform, float x, float y, float z, float w);
void gfx_rotate_transform(gfx_transform *transform, float x, float y, float z);
void gfx_rotate_transform_local(gfx_transform *transform, float x, float y, float z);
void gfx_move_transform(gfx_transform *transform, float x, float y, float z);
void gfx_move_transform_local(gfx_transform *transform, float x, float y, float z);
unsigned char gfx_get_transform_camera_mode(gfx_transform *transform);
void gfx_get_transform_position(gfx_transform *transform, float *params);
void gfx_get_transform_rotation(gfx_transform *transform, float *params);
void gfx_get_transform_scale(gfx_transform *transofrm, float *paramt);
void gfx_get_transform_orientation(gfx_transform *transform, float *params);

//////////////////////////////// DRIVER ////////////////////////////////

unsigned char gfx_init();
void gfx_deinit();

void gfx_clear_buffers(float r, float g, float b, float a, float d);
void gfx_clear_color_buffer(float r, float g, float b, float a);
void gfx_clear_depth_buffer(float d);

void gfx_read_pixels(int x, int y, unsigned int width, unsigned int height, unsigned int format, unsigned int data_format, void *data);

void gfx_reset_vertices_drawn();
int gfx_get_vertices_drawn(unsigned int draw_mode);

//////////////////////////////// VERTEX ARRAY/INDEX ////////////////////////////////

gfx_vertex_data* gfx_create_vertex_data(int count, int format, int data_type);
void gfx_destroy_vertex_data(gfx_vertex_data *data);
int gfx_get_vertex_data_count(gfx_vertex_data *data);
int gfx_get_vertex_data_format(gfx_vertex_data *data);
int gfx_get_vertex_data_size_bytes(gfx_vertex_data *data);

void* gfx_get_vertex_data_buffer(gfx_vertex_data *data);
void gfx_update_vertex_data(gfx_vertex_data *data);
void gfx_update_vertex_data_sub_data(gfx_vertex_data *data, int start, int length);

gfx_vertex_array* gfx_create_vertex_array(unsigned char gpu_data);
void gfx_destroy_vertex_array(gfx_vertex_array *vertex_array);

int gfx_get_vertex_array_vertex_count(gfx_vertex_array *vertex_array);
void gfx_set_vertex_array_data(gfx_vertex_array *vertex_array, unsigned int target, gfx_vertex_data *data);
void gfx_reset_vertex_array(gfx_vertex_array *vertex_array);
void gfx_set_vertex_array(gfx_vertex_array *vertex_array);
void gfx_draw_vertex_array(gfx_vertex_array *vertex_array, unsigned int count, unsigned int draw_mode);

gfx_vertex_index* gfx_create_vertex_index(unsigned char gpu_data, gfx_vertex_data *data);
void gfx_destroy_vertex_index(gfx_vertex_index *vertex_index);

int gfx_get_vertex_index_indice_count(gfx_vertex_index *vertex_index);
void gfx_draw_vertex_index(gfx_vertex_index *vertex_index, unsigned int draw_mode);

//////////////////////////////// TEXTURE ////////////////////////////////

int gfx_get_max_texture_size();

gfx_texture* gfx_create_2d_texture(unsigned int width, unsigned int height, float anisotropy, int mode, int filter, int format, int internal_format, int data_format, void *data);
void gfx_destroy_texture(gfx_texture *texture);

int gfx_get_texture_width(gfx_texture *texture);
int gfx_get_texture_height(gfx_texture *texture);
int gfx_get_texture_format(gfx_texture *texture);
int gfx_get_texture_data_format(gfx_texture *texture);
void gfx_copy_framebuffer_to_texture(gfx_texture *texture);

//////////////////////////////// SHADER PROGRAM ////////////////////////////////

gfx_shader_program* gfx_create_shader_program(const char* vertex, const char* fragment);
void gfx_destroy_shader_program(gfx_shader_program *shader_program);
void gfx_destroy_shader_programs(gfx_shader_program *shader_program);

void gfx_set_shader_program_uniform_1i(const char *name, int i);
void gfx_set_shader_program_uniform_1f(const char *name, float f);
void gfx_set_shader_program_uniform_vec2(const char *name, float x, float y);
void gfx_set_shader_program_uniform_vec3(const char *name, float x, float y, float z);
void gfx_set_shader_program_uniform_vec4(const char *name, float x, float y, float z, float w);
void gfx_set_shader_program_uniform_mat4(const char *name, float *matrix);

//////////////////////////////// RENDER TARGET ////////////////////////////////

gfx_render_target* gfx_create_render_target(unsigned int width, unsigned int height);
void gfx_destroy_render_target(gfx_render_target *render_target);

gfx_render_target* gfx_get_cur_render_target();
unsigned char gfx_set_render_target_color_texture(gfx_render_target *render_target, unsigned int n, gfx_texture *color);
unsigned char gfx_set_render_target_depth_texture(gfx_render_target *render_target, gfx_texture *depth);
unsigned char gfx_set_render_target(gfx_render_target *render_target);
void gfx_disable_render_target();

//////////////////////////////// SHADER PARAMS ////////////////////////////////

void gfx_set_color(gfx_color *color, float r, float g, float b, float a);
void gfx_set_shader_params_default(gfx_shader_params *shader_params);
void gfx_set_material_params_default(gfx_shader_params *shader_params);
void gfx_set_texture_params_default(gfx_shader_params *shader_params);
void gfx_set_shader_params(gfx_shader_params *shader_params);

//////////////////////////////// DRAW ////////////////////////////////

void gfx_draw_2d_quad(float x, float y, float width, float height);
void gfx_draw_textured_2d_quad(float x, float y, float width, float height);
void gfx_draw_textured_2d_quad_region(float x, float y, float width, float height, float tx, float ty, float twidth, float theight);

void gfx_draw_bounding_box(float min[3], float max[3]);
void gfx_draw_lines(int count, gfx_vertex_data *vertices);
void gfx_draw_circle(int vertices, float size);

//////////////////////////////// SHADER GEN ////////////////////////////////

void gfx_get_shader_program_config(gfx_shader_params *shader_params, gfx_shader_config *shader_config);
gfx_shader_program* gfx_get_shader_program(gfx_shader_config *config);

//////////////////////////////// QUERY ////////////////////////////////

gfx_query *gfx_create_query();
void gfx_destroy_query(gfx_query *query);

void gfx_begin_query(gfx_query *query);
void gfx_end_query(gfx_query *query);
unsigned char gfx_is_query_result(gfx_query *query);
int gfx_get_query_result(gfx_query *query);

#ifdef __cplusplus
}
#endif
