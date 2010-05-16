
#define GFX_OBJECT_HEADER \
	int type; \
	int ref_count

struct gfx_object {
	GFX_OBJECT_HEADER;
};

struct gfx_driver {
	int format_sizes[GFX_MAX_FORMATS];
	int formats[GFX_MAX_FORMATS];
	int draw_modes[GFX_MAX_DRAW_MODES];
	int texture_internal_formats[GFX_MAX_TEXTURE_FORMATS];
	int texture_data_formats[GFX_MAX_TEXTURE_FORMATS];
	int vertex_data_draw_modes[GFX_MAX_VERTEX_DATA_TYPES];

	gfx_render_target *render_target;
	gfx_shader_program *shader_programs;
	gfx_shader_params shader_params;

	int version;
	int max_texture_size;
	int max_texture_image_units;
	int max_draw_buffers;
	int max_color_attachments;
	float max_anisotropy;
	unsigned char dirty_vertex_arrays;
	unsigned int vertices_drawn[GFX_MAX_DRAW_MODES];

	gfx_vertex_data* quad_vertex_data;
	gfx_vertex_data* quad_tex_coord_data;
	gfx_vertex_data* quad_normal_data;
	gfx_vertex_array *quad_vertex_array;

	gfx_vertex_data *bb_vertex_data;
	gfx_vertex_data *bb_index_data;
	gfx_vertex_array *bb_vertex_array;
	gfx_vertex_index *bb_vertex_index;

	gfx_vertex_array *line_vertex_array;

	gfx_vertex_data *circle_vertex_data;
	gfx_vertex_array *circle_vertex_array;
	int prev_circle_vertice_count;
	float prev_circle_size;

	gfx_shader_config shader_config;
};

struct gfx_transform {
	float position[3];
	float rotation[3];
	float scale[3];
	float orient[4];
	float matrix[16];
	unsigned char recalc_matrix;
	unsigned char camera_mode;
};

struct gfx_vertex_data {
	GFX_OBJECT_HEADER;
	unsigned int vbo;
	int count;
	int format;
	int size_bytes;
	int data_type;
	void *data;
	unsigned char changed;
};

typedef struct gfx_varr {
	gfx_vertex_data *data;
	int vertex_count;
	int element_count;
	int vertex_size_bytes;
} gfx_varr;

struct gfx_vertex_array {
	GFX_OBJECT_HEADER;
	unsigned int vertex_count;
	gfx_varr varrs[GFX_MAX_VERTEX_ARRAYS];
	unsigned char gpu_data;
};

struct gfx_vertex_index {
	GFX_OBJECT_HEADER;
	int indice_count;
	gfx_vertex_data *data;
	unsigned char gpu_data;
};

struct gfx_texture {
	unsigned int id;
	unsigned int width;
	unsigned int height;
	unsigned int format;
	unsigned int data_format;
};

struct gfx_shader_program {
	gfx_shader_program *next;
	unsigned int id;
	int projection_matrix_loc;
	int modelview_matrix_loc;
	int texture0_loc;
	int texture1_loc;
	int texture2_loc;
	int texture3_loc;
	int color_map_loc;
	int normal_map_loc;
	int height_map_loc;
	int specular_map_loc;
	int color_ramp_map_loc;
	int light_map_loc;
	int shadow_projection_matrix_loc;
	int shadow_map_loc;
	int color_loc;
	int specular_color_loc;
	int spec_power_loc;
	int light_position_loc;
	int light_color_loc;
	int light_spot_direction_loc;
	int light_distance_loc;
	int light_fade_speed_loc;
	int light_inner_cone_cos_loc;
	int light_outer_cone_cos_loc;
	int clip_start_loc;
	int clip_end_loc;
	int viewport_width_loc;
	int viewport_height_loc;
	int parallax_scale_loc;
	int alpha_threshold_loc;
	gfx_shader_config config;
};

struct gfx_render_target {
	unsigned int fb;
	unsigned int rb;
	unsigned int width, height;
	unsigned char targets[16];
};

struct gfx_query {
	unsigned int id;
};

