
const char *vert_shader =
"uniform mat4 elf_ProjectionMatrix;\n"
"uniform mat4 elf_ModelviewMatrix;\n"
"varying vec2 elf_TexCoord;\n"
"attribute vec2 elf_TexCoordAttr;\n"
"\n"
"void main()\n"
"{\n"
"\telf_TexCoord = elf_TexCoordAttr;\n"
"\tgl_Position = elf_ProjectionMatrix*elf_ModelviewMatrix*gl_Vertex;\n"
"}\n";

const char *hipass_shader =
"uniform sampler2D elf_Texture0;\n"
"varying vec2 elf_TexCoord;\n"
"uniform float threshold;\n"
"\n"
"void main()\n"
"{\n"
"\tvec4 tex = texture2D(elf_Texture0, elf_TexCoord);\n"
"\tgl_FragColor = max((tex-threshold)/(1.0-threshold), 0.0);\n"
"}\n";

const char *blur_shader = 
"uniform sampler2D elf_Texture0;\n"
"varying vec2 elf_TexCoord;\n"
"uniform vec2 offset;\n"
"\n"
"void main()\n"
"{\n"
"\tvec4 tex;\n"
"\ttex = texture2D(elf_Texture0, elf_TexCoord)*0.2;\n"
"\ttex += texture2D(elf_Texture0, elf_TexCoord+offset)*0.16;\n"
"\ttex += texture2D(elf_Texture0, elf_TexCoord+offset*2.0)*0.12;\n"
"\ttex += texture2D(elf_Texture0, elf_TexCoord+offset*3.0)*0.07;\n"
"\ttex += texture2D(elf_Texture0, elf_TexCoord+offset*4.0)*0.05;\n"
"\ttex += texture2D(elf_Texture0, elf_TexCoord-offset)*0.16;\n"
"\ttex += texture2D(elf_Texture0, elf_TexCoord-offset*2.0)*0.12;\n"
"\ttex += texture2D(elf_Texture0, elf_TexCoord-offset*3.0)*0.07;\n"
"\ttex += texture2D(elf_Texture0, elf_TexCoord-offset*4.0)*0.05;\n"
"\tgl_FragColor = tex;\n"
"}\n";

const char *bloom_combine_shader = 
"uniform sampler2D elf_Texture0;\n"
"uniform sampler2D elf_Texture1;\n"
"uniform sampler2D elf_Texture2;\n"
"uniform sampler2D elf_Texture3;\n"
"varying vec2 elf_TexCoord;\n"
"\n"
"void main()\n"
"{\n"
"\tvec4 col = texture2D(elf_Texture0, elf_TexCoord);\n"
"\t//col += texture2D(elf_Texture1, elf_TexCoord);\n"
"\tcol += texture2D(elf_Texture2, elf_TexCoord);\n"
"\tcol += texture2D(elf_Texture3, elf_TexCoord);\n"
"\tgl_FragColor = col;\n"
"}\n";

const char *dof_combine_shader = 
"uniform sampler2D elf_Texture0;\n"
"uniform sampler2D elf_Texture1;\n"
"varying vec2 elf_TexCoord;\n"
"\n"
"void main()\n"
"{\n"
"\tvec4 col = texture2D(elf_Texture0, elf_TexCoord);\n"
"\tvec4 blur = texture2D(elf_Texture1, elf_TexCoord);\n"
"\tgl_FragColor = vec4(col.rgb, 0.0)+col.a*vec4(blur.rgb-col.rgb, 1.0);\n"
"}\n";

const char *ssao_shader =
"uniform sampler2D elf_Texture0;\n"
"uniform sampler2D elf_Texture1;\n"
"uniform int elf_ViewportWidth;\n"
"uniform int elf_ViewportHeight;\n"
"uniform float elf_ClipStart;\n"
"uniform float elf_ClipEnd;\n"
"uniform float amount;\n"
"varying vec2 elf_TexCoord;\n"
"#define PI 3.14159265\n"
"float width = float(elf_ViewportWidth);\n"
"float height = float(elf_ViewportHeight);\n"
"float near = elf_ClipStart;\n"
"float far = elf_ClipEnd;\n"
"int samples = 7;\n"
"int rings = 3;\n"
"vec2 rand(in vec2 coord)\n"
"{\n"
"\tfloat noiseX = (fract(sin(dot(coord ,vec2(12.9898,78.233))) * 43758.5453));\n"
"\tfloat noiseY = (fract(sin(dot(coord ,vec2(12.9898,78.233)*2.0)) * 43758.5453));\n"
"\treturn vec2(noiseX,noiseY)*0.004;\n"
"}\n"
"float readDepth(in vec2 coord)\n"
"{\n"
"\treturn (2.0 * near) / (far + near - texture2D(elf_Texture1, coord).x * (far-near));\n"
"}\n"
"float compareDepths(in float depth1, in float depth2)\n"
"{\n"
"\tfloat aoCap = 1.0;\n"
"\tfloat aoMultiplier = 100.0;\n"
"\tfloat depthTolerance = 0.0000;\n"
"\tfloat aorange = 60.0;\n"
"\tfloat diff = sqrt(clamp(1.0-(depth1-depth2) / (aorange/(far-near)),0.0,1.0));\n"
"\tfloat ao = min(aoCap,max(0.0,depth1-depth2-depthTolerance) * aoMultiplier) * diff;\n"
"\treturn ao;\n"
"}\n"
"void main(void)\n"
"{\n"
"\tfloat depth = readDepth(elf_TexCoord);\n"
"\tfloat d;\n"	
"\tfloat aspect = width/height;\n"
"\tvec2 noise = rand(elf_TexCoord);\n"
"\tfloat w = (1.0 / width)/clamp(depth,0.05,1.0)+(noise.x*(1.0-noise.x));\n"
"\tfloat h = (1.0 / height)/clamp(depth,0.05,1.0)+(noise.y*(1.0-noise.y));\n"
"\tfloat pw;\n"
"\tfloat ph;\n"
"\tfloat ao;\n"
"\tfloat s;\n"
"\tfloat fade = 1.0;\n"
"\tfor (int i = 0 ; i < rings; i += 1)\n"
"\t{\n"
"\t\tfade *= 0.5;\n"
"\t\tfor (int j = 0 ; j < samples*i; j += 1)\n"
"\t\t{\n"
"\t\t\tfloat step = PI*2.0 / (float(samples*i));\n"
"\t\t\tpw = (cos(float(j)*step)*float(i));\n"
"\t\t\tph = (sin(float(j)*step)*float(i))*aspect;\n"
"\t\t\td = readDepth( vec2(elf_TexCoord.s+pw*w,elf_TexCoord.t+ph*h));\n"
"\t\t\tao += compareDepths(depth,d)*fade;\n"
"\t\t\ts += 1.0*fade;\n"
"\t\t}\n"
"\t}\n"
"\tao /= s;\n"
"\tao = 1.0-ao;\n"	
"\tvec3 color = texture2D(elf_Texture0,elf_TexCoord).rgb;\n"
"\tvec3 luminance = vec3(color.r*0.3+color.g*0.59+color.b*0.11)-(amount-1.0);\n"
"\tvec3 black = vec3(0.0,0.0,0.0);\n"
"\tvec3 treshold = vec3(0.2,0.2,0.2);\n"
"\tluminance = clamp(max(black,luminance-treshold)+max(black,luminance-treshold)+max(black,luminance-treshold),0.0,1.0);\n"
"\tgl_FragColor = vec4(color.rgb*mix(vec3(ao,ao,ao).rgb,vec3(1.0,1.0,1.0),luminance),1.0);\n"
"}\n";

const char *ssao_combine_shader = 
"uniform sampler2D elf_Texture0;\n"
"uniform sampler2D elf_Texture1;\n"
"varying vec2 elf_TexCoord;\n"
"\n"
"void main()\n"
"{\n"
"\tvec4 ssao = texture2D(elf_Texture1, elf_TexCoord);\n"
"\tvec4 col = texture2D(elf_Texture0, elf_TexCoord);\n"
"\tgl_FragColor = vec4(col.rgb*ssao.rgb, col.a);\n"
"}\n";

const char *light_shaft_shader = 
"uniform sampler2D elf_Texture0;\n"
"uniform float exposure;\n"
"uniform float decay;\n"
"uniform float density;\n"
"uniform float weight;\n"
"uniform vec2 light_position;\n"
"varying vec2 elf_TexCoord;\n"
"const int NUM_SAMPLES = 100;\n"
"void main()\n"
"{\n"
"\tvec4 final_color = vec4(0.0, 0.0, 0.0, 1.0);\n"
"\tvec2 delta_text_coord = vec2(elf_TexCoord.st - light_position.xy);\n"
"\tvec2 text_coord = elf_TexCoord.st;\n"
"\tdelta_text_coord *= 1.0 / float(NUM_SAMPLES) * density;\n"
"\tfloat illumination_decay = 1.0;\n"
"\tfor(int i=0; i < NUM_SAMPLES ; i++)\n"
"\t{\n"
"\t\ttext_coord -= delta_text_coord;\n"
"\t\tvec4 sample = texture2D(elf_Texture0, text_coord);\n"
"\t\tsample *= illumination_decay * weight;\n"
"\t\tfinal_color += sample;\n"
"\t\tillumination_decay *= decay;\n"
"\t}\n"
"\tgl_FragColor = final_color*exposure;\n"
"}\n";

elf_post_process* elf_create_post_process()
{
	elf_post_process *post_process;

	post_process = (elf_post_process*)malloc(sizeof(elf_post_process));
	memset(post_process, 0x0, sizeof(elf_post_process));
	post_process->type = ELF_POST_PROCESS;

	elf_init_post_process_buffers(post_process);

	post_process->hipass_shdr = gfx_create_shader_program(vert_shader, hipass_shader);
	post_process->blur_shdr = gfx_create_shader_program(vert_shader, blur_shader);
	post_process->bloom_combine_shdr = gfx_create_shader_program(vert_shader, bloom_combine_shader);
	post_process->dof_combine_shdr = gfx_create_shader_program(vert_shader, dof_combine_shader);
	post_process->ssao_shdr = gfx_create_shader_program(vert_shader, ssao_shader);
	post_process->ssao_combine_shdr = gfx_create_shader_program(vert_shader, ssao_combine_shader);
	post_process->light_shaft_shdr = gfx_create_shader_program(vert_shader, light_shaft_shader);

	post_process->light_shaft_transform = gfx_create_object_transform();

	gfx_set_shader_params_default(&post_process->shader_params);

	return post_process;
}

void elf_destroy_post_process(elf_post_process *post_process)
{
	gfx_destroy_render_target(post_process->main_rt);
	gfx_destroy_texture(post_process->main_rt_color[0]);
	if(post_process->main_rt_color[1]) gfx_destroy_texture(post_process->main_rt_color[1]);
	gfx_destroy_texture(post_process->main_rt_depth);

	gfx_destroy_render_target(post_process->rt_high);
	gfx_destroy_render_target(post_process->rt_med);
	gfx_destroy_render_target(post_process->rt_low);
	gfx_destroy_render_target(post_process->rt_tiny);

	gfx_destroy_texture(post_process->rt_tex_high_1);
	gfx_destroy_texture(post_process->rt_tex_high_2);
	gfx_destroy_texture(post_process->rt_tex_high_depth);
	gfx_destroy_texture(post_process->rt_tex_med_1);
	gfx_destroy_texture(post_process->rt_tex_med_2);
	gfx_destroy_texture(post_process->rt_tex_med_3);
	gfx_destroy_texture(post_process->rt_tex_low_1);
	gfx_destroy_texture(post_process->rt_tex_low_2);
	gfx_destroy_texture(post_process->rt_tex_tiny_1);
	gfx_destroy_texture(post_process->rt_tex_tiny_2);

	if(post_process->hipass_shdr) gfx_destroy_shader_program(post_process->hipass_shdr);
	if(post_process->blur_shdr) gfx_destroy_shader_program(post_process->blur_shdr);
	if(post_process->bloom_combine_shdr) gfx_destroy_shader_program(post_process->bloom_combine_shdr);
	if(post_process->dof_combine_shdr) gfx_destroy_shader_program(post_process->dof_combine_shdr);
	if(post_process->ssao_shdr) gfx_destroy_shader_program(post_process->ssao_shdr);
	if(post_process->ssao_combine_shdr) gfx_destroy_shader_program(post_process->ssao_combine_shdr);
	if(post_process->light_shaft_shdr) gfx_destroy_shader_program(post_process->light_shaft_shdr);

	gfx_destroy_transform(post_process->light_shaft_transform);

	free(post_process);
}

void elf_init_post_process_buffers(elf_post_process *post_process)
{

	if(post_process->main_rt) gfx_destroy_render_target(post_process->main_rt);
	if(post_process->main_rt_color[0]) gfx_destroy_texture(post_process->main_rt_color[0]);
	if(post_process->main_rt_color[1]) gfx_destroy_texture(post_process->main_rt_color[1]);
	if(post_process->main_rt_depth) gfx_destroy_texture(post_process->main_rt_depth);

	if(post_process->rt_high) gfx_destroy_render_target(post_process->rt_high);
	if(post_process->rt_med) gfx_destroy_render_target(post_process->rt_med);
	if(post_process->rt_low) gfx_destroy_render_target(post_process->rt_low);
	if(post_process->rt_tiny) gfx_destroy_render_target(post_process->rt_tiny);

	if(post_process->rt_tex_high_1) gfx_destroy_texture(post_process->rt_tex_high_1);
	if(post_process->rt_tex_high_2) gfx_destroy_texture(post_process->rt_tex_high_2);
	if(post_process->rt_tex_high_depth) gfx_destroy_texture(post_process->rt_tex_high_depth);
	if(post_process->rt_tex_med_1) gfx_destroy_texture(post_process->rt_tex_med_1);
	if(post_process->rt_tex_med_2) gfx_destroy_texture(post_process->rt_tex_med_2);
	if(post_process->rt_tex_med_3) gfx_destroy_texture(post_process->rt_tex_med_3);
	if(post_process->rt_tex_low_1) gfx_destroy_texture(post_process->rt_tex_low_1);
	if(post_process->rt_tex_low_2) gfx_destroy_texture(post_process->rt_tex_low_2);
	if(post_process->rt_tex_tiny_1) gfx_destroy_texture(post_process->rt_tex_tiny_1);
	if(post_process->rt_tex_tiny_2) gfx_destroy_texture(post_process->rt_tex_tiny_2);

	post_process->buffer_width = elf_get_window_width()/4;
	post_process->buffer_height = elf_get_window_height()/4;

	post_process->main_rt_color[0] = gfx_create_2d_texture(
			elf_get_window_width(), elf_get_window_height(), 0.0f,
			GFX_CLAMP, GFX_LINEAR, GFX_RGBA, GFX_RGBA, GFX_UBYTE, NULL);
	post_process->main_rt_depth = gfx_create_2d_texture(
			elf_get_window_width(), elf_get_window_height(), 0.0f,
			GFX_CLAMP, GFX_LINEAR, GFX_DEPTH_COMPONENT, GFX_DEPTH_COMPONENT, GFX_UBYTE, NULL);
	post_process->main_rt = gfx_create_render_target(elf_get_window_width(), elf_get_window_height());

	gfx_set_render_target_color_texture(post_process->main_rt, 0, post_process->main_rt_color[0]);
	gfx_set_render_target_depth_texture(post_process->main_rt, post_process->main_rt_depth);

	post_process->rt_tex_high_1 = gfx_create_2d_texture(post_process->buffer_width*2, post_process->buffer_height*2, 0.0, GFX_CLAMP, GFX_LINEAR, GFX_RGB, GFX_RGB, GFX_UBYTE, NULL);
	post_process->rt_tex_high_2 = gfx_create_2d_texture(post_process->buffer_width*2, post_process->buffer_height*2, 0.0, GFX_CLAMP, GFX_LINEAR, GFX_RGB, GFX_RGB, GFX_UBYTE, NULL);
	post_process->rt_tex_high_depth = gfx_create_2d_texture(post_process->buffer_width*2, post_process->buffer_height*2, 0.0, GFX_CLAMP, GFX_LINEAR, GFX_DEPTH_COMPONENT, GFX_DEPTH_COMPONENT, GFX_UBYTE, NULL);
	post_process->rt_tex_med_1 = gfx_create_2d_texture(post_process->buffer_width, post_process->buffer_height, 0.0, GFX_CLAMP, GFX_LINEAR, GFX_RGB, GFX_RGB, GFX_UBYTE, NULL);
	post_process->rt_tex_med_2 = gfx_create_2d_texture(post_process->buffer_width, post_process->buffer_height, 0.0, GFX_CLAMP, GFX_LINEAR, GFX_RGB, GFX_RGB, GFX_UBYTE, NULL);
	post_process->rt_tex_med_3 = gfx_create_2d_texture(post_process->buffer_width, post_process->buffer_height, 0.0, GFX_CLAMP, GFX_LINEAR, GFX_RGB, GFX_RGB, GFX_UBYTE, NULL);
	post_process->rt_tex_low_1 = gfx_create_2d_texture(post_process->buffer_width/2, post_process->buffer_height/2, 0.0, GFX_CLAMP, GFX_LINEAR, GFX_RGB, GFX_RGB, GFX_UBYTE, NULL);
	post_process->rt_tex_low_2 = gfx_create_2d_texture(post_process->buffer_width/2, post_process->buffer_height/2, 0.0, GFX_CLAMP, GFX_LINEAR, GFX_RGB, GFX_RGB, GFX_UBYTE, NULL);
	post_process->rt_tex_tiny_1 = gfx_create_2d_texture(post_process->buffer_width/4, post_process->buffer_height/4, 0.0, GFX_CLAMP, GFX_LINEAR, GFX_RGB, GFX_RGB, GFX_UBYTE, NULL);
	post_process->rt_tex_tiny_2 = gfx_create_2d_texture(post_process->buffer_width/4, post_process->buffer_height/4, 0.0, GFX_CLAMP, GFX_LINEAR, GFX_RGB, GFX_RGB, GFX_UBYTE, NULL);

	post_process->rt_high = gfx_create_render_target(post_process->buffer_width*2, post_process->buffer_height*2);
	post_process->rt_med = gfx_create_render_target(post_process->buffer_width, post_process->buffer_height);
	post_process->rt_low = gfx_create_render_target(post_process->buffer_width/2, post_process->buffer_height/2);
	post_process->rt_tiny = gfx_create_render_target(post_process->buffer_width/4, post_process->buffer_height/4);

	gfx_set_render_target_color_texture(post_process->rt_high, 0, post_process->rt_tex_high_1);
	gfx_set_render_target_depth_texture(post_process->rt_high, post_process->rt_tex_high_depth);
	gfx_set_render_target_color_texture(post_process->rt_med, 0, post_process->rt_tex_med_1);
	gfx_set_render_target_color_texture(post_process->rt_low, 0, post_process->rt_tex_low_1);
	gfx_set_render_target_color_texture(post_process->rt_tiny, 0, post_process->rt_tex_tiny_1);

	if(!post_process->main_rt_color[1] && (int)post_process->bloom+(int)post_process->dof+(int)post_process->ssao > 1)
	{
		post_process->main_rt_color[1] = gfx_create_2d_texture(
			elf_get_window_width(), elf_get_window_height(), 0.0f,
			GFX_CLAMP, GFX_LINEAR, GFX_RGBA, GFX_RGBA, GFX_UBYTE, NULL);
	}
}

void elf_begin_post_process(elf_post_process *post_process, elf_scene *scene)
{
	gfx_set_shader_params_default(&post_process->shader_params);
	gfx_set_shader_params(&post_process->shader_params);

	gfx_set_render_target(post_process->main_rt);

	//if(post_process->ssao)
	//{
	//	gfx_set_render_target_color_texture(post_process->main_rt, 0, post_process->main_rt_ssao_depth);
	//	gfx_clear_color_buffer(1.0, 1.0, 1.0, 1.0);
	//}

	gfx_set_render_target_color_texture(post_process->main_rt, 0, post_process->main_rt_color[0]);
	
	gfx_clear_buffers(0.0, 0.0, 0.0, 1.0, 1.0);
}

void elf_end_post_process(elf_post_process *post_process, elf_scene *scene)
{
	unsigned int source_rt = 0;
	elf_camera *cam;
	elf_entity *ent;
	elf_light *light;
	int i;
	elf_vec3f light_pos;
	elf_vec3f light_screen_pos;
	elf_vec3f cam_pos;
	elf_vec4f cam_orient;
	int viewport[4];
	unsigned char first_shaft;

	gfx_disable_render_target();

	gfx_set_shader_params_default(&post_process->shader_params);
	post_process->shader_params.render_params.depth_test = GFX_FALSE;
	post_process->shader_params.render_params.depth_write = GFX_FALSE;
	post_process->shader_params.render_params.alpha_test = GFX_FALSE;

	// SSAO
	if(post_process->ssao && scene->cur_camera)
	{
		if(post_process->dof || post_process->bloom)
		{
			gfx_set_render_target(post_process->main_rt);
			gfx_set_render_target_color_texture(post_process->main_rt, 0, post_process->main_rt_color[!source_rt]);
		}
		else
		{
			gfx_disable_render_target();
		}

		gfx_set_viewport(0, 0, elf_get_window_width(), elf_get_window_height());
		gfx_get_orthographic_projection_matrix(0.0f, (float)elf_get_window_width(),
			0.0f, (float)elf_get_window_height(), -1.0f, 1.0f,
			post_process->shader_params.projection_matrix);

		if((cam = elf_get_scene_active_camera(scene)))
		{
			post_process->shader_params.clip_start = elf_get_camera_clip(cam).x;
			post_process->shader_params.clip_end = elf_get_camera_clip(cam).y;
			post_process->shader_params.viewport_width = elf_get_window_width()*2;
			post_process->shader_params.viewport_height = elf_get_window_height()*2;
		}

		post_process->shader_params.shader_program = post_process->ssao_shdr;

		post_process->shader_params.texture_params[0].texture = post_process->main_rt_color[source_rt];
		post_process->shader_params.texture_params[1].texture = post_process->main_rt_depth;
		gfx_set_shader_params(&post_process->shader_params);
		gfx_set_shader_program_uniform_1f("amount", post_process->ssao_amount);

		gfx_draw_textured_2d_quad(0.0f, 0.0f, (float)elf_get_window_width(), (float)elf_get_window_height());

		source_rt = !source_rt;
		
		post_process->shader_params.texture_params[0].texture = NULL;
		post_process->shader_params.texture_params[1].texture = NULL;
	}

	// DOF
	if(post_process->dof)
	{
		gfx_set_viewport(0, 0, post_process->buffer_width*2, post_process->buffer_height*2);
		gfx_get_orthographic_projection_matrix(0, post_process->buffer_width*2, 0, post_process->buffer_height*2, -1.0, 1.0,
			post_process->shader_params.projection_matrix);

		gfx_set_render_target(post_process->rt_high);
		gfx_set_render_target_color_texture(post_process->rt_high, 0, post_process->rt_tex_high_1);

		post_process->shader_params.texture_params[0].texture = post_process->main_rt_color[source_rt];
		gfx_set_shader_params(&post_process->shader_params);
		gfx_draw_textured_2d_quad(0.0, 0.0, post_process->buffer_width*2, post_process->buffer_height*2);

		gfx_set_render_target_color_texture(post_process->rt_high, 0, post_process->rt_tex_high_2);

		post_process->shader_params.shader_program = post_process->blur_shdr;
		post_process->shader_params.texture_params[0].texture = post_process->rt_tex_high_1;
		gfx_set_shader_params(&post_process->shader_params);
		gfx_set_shader_program_uniform_vec2("offset", 1.0/((float)(post_process->buffer_width*2)), 0.0);
		gfx_draw_textured_2d_quad(0.0, 0.0, post_process->buffer_width*2, post_process->buffer_height*2);

		gfx_set_render_target_color_texture(post_process->rt_high, 0, post_process->rt_tex_high_1);

		post_process->shader_params.texture_params[0].texture = post_process->rt_tex_high_2;
		gfx_set_shader_params(&post_process->shader_params);
		gfx_set_shader_program_uniform_vec2("offset", 0.0, 1.0/((float)(post_process->buffer_height*2)));
		gfx_draw_textured_2d_quad(0.0, 0.0, post_process->buffer_width*2, post_process->buffer_height*2);

		if(post_process->bloom)
		{
			gfx_set_render_target(post_process->main_rt);
			gfx_set_render_target_color_texture(post_process->main_rt, 0, post_process->main_rt_color[!source_rt]);
		}
		else
		{
			gfx_disable_render_target();
		}

		gfx_set_viewport(0, 0, elf_get_window_width(), elf_get_window_height());
		gfx_get_orthographic_projection_matrix(0.0, (float)elf_get_window_width(),
			0.0, (float)elf_get_window_height(), -1.0, 1.0,
			post_process->shader_params.projection_matrix);

		post_process->shader_params.shader_program = post_process->dof_combine_shdr;
		post_process->shader_params.texture_params[0].texture = post_process->main_rt_color[source_rt];
		post_process->shader_params.texture_params[1].texture = post_process->rt_tex_high_1;
		gfx_set_shader_params(&post_process->shader_params);

		gfx_draw_textured_2d_quad(0.0, 0.0, (float)elf_get_window_width(), (float)elf_get_window_height());

		source_rt = !source_rt;
		
		post_process->shader_params.texture_params[0].texture = NULL;
	}

	// BLOOM
	if(post_process->bloom)
	{
		gfx_set_viewport(0, 0, post_process->buffer_width, post_process->buffer_height);
		gfx_get_orthographic_projection_matrix(0, post_process->buffer_width, 0, post_process->buffer_height, -1.0, 1.0,
			post_process->shader_params.projection_matrix);

		gfx_set_render_target(post_process->rt_med);
		gfx_set_render_target_color_texture(post_process->rt_med, 0, post_process->rt_tex_med_1);

		post_process->shader_params.shader_program = post_process->hipass_shdr;
		post_process->shader_params.texture_params[0].texture = post_process->main_rt_color[source_rt];
		gfx_set_shader_params(&post_process->shader_params);
		gfx_set_shader_program_uniform_1f("threshold", post_process->bloom_threshold);
		gfx_draw_textured_2d_quad(0.0, 0.0, post_process->buffer_width, post_process->buffer_height);

		gfx_set_render_target_color_texture(post_process->rt_med, 0, post_process->rt_tex_med_2);

		post_process->shader_params.shader_program = post_process->blur_shdr;
		post_process->shader_params.texture_params[0].texture = post_process->rt_tex_med_1;
		gfx_set_shader_params(&post_process->shader_params);
		gfx_set_shader_program_uniform_vec2("offset", 1.0/((float)post_process->buffer_width), 0.0);
		gfx_draw_textured_2d_quad(0.0, 0.0, post_process->buffer_width, post_process->buffer_height);

		gfx_set_render_target_color_texture(post_process->rt_med, 0, post_process->rt_tex_med_3);

		post_process->shader_params.texture_params[0].texture = post_process->rt_tex_med_2;
		gfx_set_shader_params(&post_process->shader_params);
		gfx_set_shader_program_uniform_vec2("offset", 0.0, 1.0/((float)post_process->buffer_height));
		gfx_draw_textured_2d_quad(0.0, 0.0, post_process->buffer_width, post_process->buffer_height);

		gfx_set_viewport(0, 0, post_process->buffer_width/2, post_process->buffer_height/2);
		gfx_get_orthographic_projection_matrix(0.0, post_process->buffer_width/2, 0.0, post_process->buffer_height/2, -1.0, 1.0,
			post_process->shader_params.projection_matrix);

		gfx_set_render_target(post_process->rt_low);
		gfx_set_render_target_color_texture(post_process->rt_low, 0, post_process->rt_tex_low_1);

		post_process->shader_params.shader_program = post_process->blur_shdr;
		post_process->shader_params.texture_params[0].texture = post_process->rt_tex_med_3;
		gfx_set_shader_params(&post_process->shader_params);
		gfx_set_shader_program_uniform_vec2("offset", 1.0/((float)post_process->buffer_width/2), 0.0);
		gfx_draw_textured_2d_quad(0.0, 0.0, post_process->buffer_width/2, post_process->buffer_height/2);

		gfx_set_render_target_color_texture(post_process->rt_low, 0, post_process->rt_tex_low_2);

		post_process->shader_params.texture_params[0].texture = post_process->rt_tex_low_1;
		gfx_set_shader_params(&post_process->shader_params);
		gfx_set_shader_program_uniform_vec2("offset", 0.0, 1.0/((float)post_process->buffer_height/2));
		gfx_draw_textured_2d_quad(0.0, 0.0, post_process->buffer_width/2, post_process->buffer_height/2);

		gfx_set_viewport(0, 0, post_process->buffer_width/4, post_process->buffer_height/4);
		gfx_get_orthographic_projection_matrix(0.0, post_process->buffer_width/4, 0.0, post_process->buffer_height/4, -1.0, 1.0,
			post_process->shader_params.projection_matrix);

		gfx_set_render_target(post_process->rt_tiny);
		gfx_set_render_target_color_texture(post_process->rt_tiny, 0, post_process->rt_tex_tiny_1);

		post_process->shader_params.shader_program = post_process->blur_shdr;
		post_process->shader_params.texture_params[0].texture = post_process->rt_tex_low_2;
		gfx_set_shader_params(&post_process->shader_params);
		gfx_set_shader_program_uniform_vec2("offset", 1.0/((float)post_process->buffer_width/4), 0.0);
		gfx_draw_textured_2d_quad(0.0, 0.0, post_process->buffer_width/4, post_process->buffer_height/4);

		gfx_set_render_target_color_texture(post_process->rt_tiny, 0, post_process->rt_tex_tiny_2);

		post_process->shader_params.texture_params[0].texture = post_process->rt_tex_tiny_1;
		gfx_set_shader_params(&post_process->shader_params);
		gfx_set_shader_program_uniform_vec2("offset", 0.0, 1.0/((float)post_process->buffer_height/4));
		gfx_draw_textured_2d_quad(0.0, 0.0, post_process->buffer_width/4, post_process->buffer_height/4);

		gfx_disable_render_target();

		gfx_set_viewport(0, 0, elf_get_window_width(), elf_get_window_height());
		gfx_get_orthographic_projection_matrix(0.0, (float)elf_get_window_width(),
			0.0, (float)elf_get_window_height(), -1.0, 1.0,
			post_process->shader_params.projection_matrix);

		post_process->shader_params.shader_program = post_process->bloom_combine_shdr;
		post_process->shader_params.texture_params[0].texture = post_process->main_rt_color[source_rt];
		post_process->shader_params.texture_params[1].texture = post_process->rt_tex_med_3;
		post_process->shader_params.texture_params[2].texture = post_process->rt_tex_low_2;
		post_process->shader_params.texture_params[3].texture = post_process->rt_tex_tiny_2;
		gfx_set_shader_params(&post_process->shader_params);

		gfx_draw_textured_2d_quad(0.0, 0.0, (float)elf_get_window_width(), (float)elf_get_window_height());

		post_process->shader_params.texture_params[0].texture = NULL;
		post_process->shader_params.texture_params[1].texture = NULL;
		post_process->shader_params.texture_params[2].texture = NULL;
		post_process->shader_params.texture_params[3].texture = NULL;
	}

	if(!post_process->bloom && !post_process->dof && !post_process->ssao)
	{
		gfx_set_viewport(0, 0, elf_get_window_width(), elf_get_window_height());
		gfx_get_orthographic_projection_matrix(0.0, (float)elf_get_window_width(),
			0.0, (float)elf_get_window_height(), -1.0, 1.0,
			post_process->shader_params.projection_matrix);

		post_process->shader_params.texture_params[0].texture = post_process->main_rt_color[source_rt];
		gfx_set_shader_params(&post_process->shader_params);

		gfx_draw_textured_2d_quad(0.0, 0.0, (float)elf_get_window_width(), (float)elf_get_window_height());

		post_process->shader_params.texture_params[0].texture = NULL;
	}

	// LIGHT SHAFTS
	first_shaft = ELF_TRUE;

	if(post_process->light_shafts && scene->cur_camera)
	{
		for(light = (elf_light*)elf_begin_list(scene->lights); light;
			light = (elf_light*)elf_next_in_list(scene->lights))
		{
			light_pos = elf_get_actor_position((elf_actor*)light);
			if(elf_is_light_shaft(light) && elf_sphere_inside_frustum(scene->cur_camera, &light_pos.x, light->shaft_size))
			{
				if(first_shaft)
				{
					gfx_set_shader_params_default(&scene->shader_params);
					elf_set_camera(scene->cur_camera, &scene->shader_params);
					gfx_set_viewport(0, 0, post_process->buffer_width*2, post_process->buffer_height*2);
					gfx_set_shader_params(&scene->shader_params);

					gfx_set_render_target(post_process->rt_high);
					gfx_set_render_target_color_texture(post_process->rt_high, 0, post_process->rt_tex_high_1);
					gfx_clear_buffers(0.0, 0.0, 0.0, 1.0, 1.0);

					scene->shader_params.render_params.color_write = ELF_FALSE;
					scene->shader_params.render_params.alpha_write = ELF_FALSE;

					for(i = 0, ent = (elf_entity*)elf_begin_list(scene->entity_queue);
						i < scene->entity_queue_count && ent != NULL;
						i++, ent = (elf_entity*)elf_next_in_list(scene->entity_queue))
					{
						elf_draw_entity_without_materials(ent, &scene->shader_params);
					}

					first_shaft = ELF_FALSE;
				}

				cam_pos = elf_get_actor_position((elf_actor*)scene->cur_camera);
				cam_orient = elf_get_actor_orientation((elf_actor*)scene->cur_camera);
				viewport[0] = 0; viewport[1] = 0; viewport[2] = elf_get_window_width(); viewport[3] = elf_get_window_height();
				gfx_project(light_pos.x, light_pos.y, light_pos.z,
					elf_get_camera_modelview_matrix(scene->cur_camera),
					elf_get_camera_projection_matrix(scene->cur_camera),
					viewport, &light_screen_pos.x);

				light_pos = elf_sub_vec3f_vec3f(light_pos, cam_pos);
				cam_orient = elf_get_qua_inverted(cam_orient);
				light_pos = elf_mul_qua_vec3f(cam_orient, light_pos);

				// render the light beacon
				gfx_set_shader_params_default(&scene->shader_params);

				elf_set_camera(scene->cur_camera, &scene->shader_params);
				gfx_set_viewport(0, 0, post_process->buffer_width*2, post_process->buffer_height*2);

				scene->shader_params.render_params.depth_write = GFX_FALSE;

				gfx_set_transform_position(post_process->light_shaft_transform, light_pos.x, light_pos.y, light_pos.z);
				memcpy(scene->shader_params.modelview_matrix, gfx_get_transform_matrix(post_process->light_shaft_transform), sizeof(float)*16);
				gfx_set_color(&scene->shader_params.material_params.color, light->color.r, light->color.g, light->color.b, 1.0);

				gfx_set_shader_params(&scene->shader_params);

				gfx_set_render_target(post_process->rt_high);
				gfx_set_render_target_color_texture(post_process->rt_high, 0, post_process->rt_tex_high_1);

				gfx_draw_circle(32, light->shaft_size);

				// shaft the light...
				gfx_set_render_target_color_texture(post_process->rt_high, 0, post_process->rt_tex_high_2);
				
				gfx_set_shader_params_default(&scene->shader_params);
				gfx_set_viewport(0, 0, post_process->buffer_width*2, post_process->buffer_height*2);
				gfx_get_orthographic_projection_matrix(0.0, (float)post_process->buffer_width*2,
					0.0, (float)post_process->buffer_height*2, -1.0, 1.0,
					scene->shader_params.projection_matrix);

				scene->shader_params.render_params.depth_test = ELF_FALSE;
				scene->shader_params.render_params.depth_write = ELF_FALSE;
				scene->shader_params.shader_program = post_process->light_shaft_shdr;
				scene->shader_params.texture_params[0].texture = post_process->rt_tex_high_1;

				gfx_set_shader_params(&scene->shader_params);
				gfx_set_shader_program_uniform_1f("exposure", 0.0034);
				gfx_set_shader_program_uniform_1f("decay", 1.0);
				gfx_set_shader_program_uniform_1f("density", 1.0-light->shaft_fade_off);
				gfx_set_shader_program_uniform_1f("weight", light->shaft_intensity*5.0*post_process->light_shafts_intensity);
				gfx_set_shader_program_uniform_vec2("light_position",
					light_screen_pos.x/(float)elf_get_window_width(),
					light_screen_pos.y/(float)elf_get_window_height());

				gfx_draw_textured_2d_quad(0.0, 0.0, (float)post_process->buffer_width*2, (float)post_process->buffer_height*2);

				gfx_disable_render_target();

				// add light shaft to scene
				gfx_set_shader_params_default(&scene->shader_params);
				gfx_set_viewport(0, 0, elf_get_window_width(), elf_get_window_height());
				gfx_get_orthographic_projection_matrix(0.0, (float)elf_get_window_width(),
					0.0, (float)elf_get_window_height(), -1.0, 1.0,
					scene->shader_params.projection_matrix);

				scene->shader_params.render_params.depth_test = GFX_FALSE;
				scene->shader_params.render_params.depth_write = GFX_FALSE;
				scene->shader_params.render_params.blend_mode = GFX_ADD;
				scene->shader_params.texture_params[0].texture = post_process->rt_tex_high_2;

				gfx_set_shader_params(&scene->shader_params);

				gfx_draw_textured_2d_quad(0.0, 0.0, (float)elf_get_window_width(), (float)elf_get_window_height());

				scene->shader_params.texture_params[0].texture = NULL;
			}
		}
	}

	// reset state just to be sure...
	gfx_set_shader_params_default(&post_process->shader_params);
	gfx_set_shader_params(&post_process->shader_params);

	/*gfx_set_viewport(0, 0, elf_get_window_width(), elf_get_window_height());
	gfx_get_orthographic_projection_matrix(0.0f, (float)elf_get_window_width(),
		0.0f, (float)elf_get_window_height(), -1.0f, 1.0f,
		post_process->shader_params.projection_matrix);
	
	post_process->shader_params.texture_params[0].texture = post_process->main_rt_color[cur_main_rt];
	gfx_set_shader_params(&post_process->shader_params);

	gfx_draw_textured_2d_quad(0.0f, 0.0f, (float)elf_get_window_width(), (float)elf_get_window_height());*/
}

void elf_set_post_process_bloom(elf_post_process *post_process, float threshold)
{
	post_process->bloom = ELF_TRUE;
	post_process->bloom_threshold = threshold;
	if(post_process->bloom_threshold < 0.00001) post_process->bloom_threshold = 0.00001;
	if(post_process->bloom_threshold > 0.99999) post_process->bloom_threshold = 0.99999;
	if(!post_process->main_rt_color[1] && (int)post_process->bloom+(int)post_process->dof+(int)post_process->ssao > 1)
	{
		post_process->main_rt_color[1] = gfx_create_2d_texture(
			elf_get_window_width(), elf_get_window_height(), 0.0,
			GFX_CLAMP, GFX_LINEAR, GFX_RGBA, GFX_RGBA, GFX_UBYTE, NULL);
	}
}

void elf_disable_post_process_bloom(elf_post_process *post_process)
{
	post_process->bloom = ELF_FALSE;
}

float elf_get_post_process_bloom_threshold(elf_post_process *post_process)
{
	return post_process->bloom_threshold;
}

void elf_set_post_process_dof(elf_post_process *post_process, float focal_range, float focal_distance)
{
	post_process->dof = ELF_TRUE;
	post_process->dof_focal_range = focal_range;
	if(post_process->dof_focal_range < 0.0) post_process->dof_focal_range = 0.0;
	post_process->dof_focal_distance = focal_distance;
	if(post_process->dof_focal_distance < 0.0) post_process->dof_focal_distance = 0.0;
	if(!post_process->main_rt_color[1] && (int)post_process->bloom+(int)post_process->dof+(int)post_process->ssao > 1)
	{
		post_process->main_rt_color[1] = gfx_create_2d_texture(
			elf_get_window_width(), elf_get_window_height(), 0.0f,
			GFX_CLAMP, GFX_LINEAR, GFX_RGBA, GFX_RGBA, GFX_UBYTE, NULL);
	}
}

void elf_disable_post_process_dof(elf_post_process *post_process)
{
	post_process->dof = ELF_FALSE;
}

float elf_get_post_process_dof_focal_range(elf_post_process *post_process)
{
	return post_process->dof_focal_range;
}

float elf_get_post_process_dof_focal_distance(elf_post_process *post_process)
{
	return post_process->dof_focal_distance;
}

void elf_set_post_process_ssao(elf_post_process *post_process, float amount)
{
	post_process->ssao = ELF_TRUE;
	post_process->ssao_amount = amount;

	if(!post_process->main_rt_color[1] && (int)post_process->bloom+(int)post_process->dof+(int)post_process->ssao > 1)
	{
		post_process->main_rt_color[1] = gfx_create_2d_texture(
			elf_get_window_width(), elf_get_window_height(), 0.0f,
			GFX_CLAMP, GFX_LINEAR, GFX_RGBA, GFX_RGBA, GFX_UBYTE, NULL);
	}
}

void elf_disable_post_process_ssao(elf_post_process *post_process)
{
	post_process->ssao = ELF_FALSE;
	if(post_process->main_rt_ssao_depth)
	{
		gfx_destroy_texture(post_process->main_rt_ssao_depth);
		post_process->main_rt_ssao_depth = NULL;
	}
}

float elf_get_post_process_ssao_amount(elf_post_process *post_process)
{
	return post_process->ssao_amount;
}

void elf_set_post_process_light_shafts(elf_post_process *post_process, float intensity)
{
	post_process->light_shafts = ELF_TRUE;
	post_process->light_shafts_intensity = intensity;
	if(post_process->light_shafts_intensity < 0.0) post_process->light_shafts_intensity = 0.0;

	if(!post_process->main_rt_color[1] && (int)post_process->bloom+(int)post_process->dof+(int)post_process->ssao > 1)
	{
		post_process->main_rt_color[1] = gfx_create_2d_texture(
			elf_get_window_width(), elf_get_window_height(), 0.0f,
			GFX_CLAMP, GFX_LINEAR, GFX_RGBA, GFX_RGBA, GFX_UBYTE, NULL);
	}
}

void elf_disable_post_process_light_shafts(elf_post_process *post_process)
{
	post_process->light_shafts = ELF_FALSE;
}

float elf_get_post_process_light_shafts_intensity(elf_post_process *post_process)
{
	return post_process->light_shafts_intensity;
}

unsigned char elf_is_post_process_bloom(elf_post_process *post_process)
{
	return post_process->bloom;
}

unsigned char elf_is_post_process_ssao(elf_post_process *post_process)
{
	return post_process->ssao;
}

unsigned char elf_is_post_process_dof(elf_post_process *post_process)
{
	return post_process->dof;
}

unsigned char elf_is_post_process_light_shafts(elf_post_process *post_process)
{
	return post_process->light_shafts;
}
