
const char *dof_depth_write_vert =
"uniform mat4 elf_ProjectionMatrix;\n"
"uniform mat4 elf_ModelviewMatrix;\n"
"attribute vec3 elf_VertexAttr;\n"
"uniform float elf_FocalRange;\n"
"uniform float elf_FocalDistance;\n"
"varying float elf_Blur;\n"
"\n"
"void main()\n"
"{\n"
"\tvec4 vertex = elf_ModelviewMatrix*vec4(elf_VertexAttr, 1.0);\n"
"\telf_Blur = clamp(abs(-vertex.z-elf_FocalDistance)/elf_FocalRange, 0.0, 1.0);\n"
"\tgl_Position = elf_ProjectionMatrix*vertex;\n"
"}\n";

const char *dof_depth_write_frag =
"varying float elf_Blur;\n"
"\n"
"void main()\n"
"{\n"
"\tgl_FragColor.a = elf_Blur;\n"
"}\n";

const char *dof_depth_write_alpha_vert =
"uniform mat4 elf_ProjectionMatrix;\n"
"uniform mat4 elf_ModelviewMatrix;\n"
"attribute vec3 elf_VertexAttr;\n"
"attribute vec2 elf_TexCoordAttr;\n"
"uniform float elf_FocalRange;\n"
"uniform float elf_FocalDistance;\n"
"varying vec2 elf_TexCoord;\n"
"varying float elf_Blur;\n"
"\n"
"void main()\n"
"{\n"
"\tvec4 vertex = elf_ModelviewMatrix*vec4(elf_VertexAttr, 1.0);\n"
"\telf_TexCoord = elf_TexCoordAttr;\n"
"\telf_Blur = clamp(abs(-vertex.z-elf_FocalDistance)/elf_FocalRange, 0.0, 1.0);\n"
"\tgl_Position = elf_ProjectionMatrix*vertex;\n"
"}\n";

const char *dof_depth_write_alpha_frag =
"uniform sampler2D elf_ColorMap;\n"
"uniform float elf_AlphaThreshold;\n"
"varying vec2 elf_TexCoord;\n"
"varying float elf_Blur;\n"
"\n"
"void main()\n"
"{\n"
"\tif(texture2D(elf_ColorMap, elf_TexCoord).a < elf_AlphaThreshold) discard;\n"
"\tgl_FragColor.a = elf_Blur;\n"
"}\n";

elf_scene* elf_create_scene()
{
	elf_scene *scene;

	scene = (elf_scene*)malloc(sizeof(elf_scene));
	memset(scene, 0x0, sizeof(elf_scene));

	scene->type = ELF_SCENE;

	scene->models = elf_create_list();
	scene->scripts = elf_create_list();
	scene->textures = elf_create_list();
	scene->materials = elf_create_list();
	scene->cameras = elf_create_list();
	scene->entities = elf_create_list();
	scene->lights = elf_create_list();
	scene->armatures = elf_create_list();
	scene->particles = elf_create_list();
	scene->entity_queue = elf_create_list();

	elf_inc_ref((elf_object*)scene->models);
	elf_inc_ref((elf_object*)scene->scripts);
	elf_inc_ref((elf_object*)scene->materials);
	elf_inc_ref((elf_object*)scene->textures);
	elf_inc_ref((elf_object*)scene->cameras);
	elf_inc_ref((elf_object*)scene->entities);
	elf_inc_ref((elf_object*)scene->lights);
	elf_inc_ref((elf_object*)scene->armatures);
	elf_inc_ref((elf_object*)scene->particles);
	elf_inc_ref((elf_object*)scene->entity_queue);

	gfx_set_shader_params_default(&scene->shader_params);

	scene->world = elf_create_physics_world();
	scene->dworld = elf_create_physics_world();

	scene->dof_depth_write = gfx_create_shader_program(dof_depth_write_vert, dof_depth_write_frag);
	scene->dof_depth_write_alpha = gfx_create_shader_program(dof_depth_write_alpha_vert, dof_depth_write_alpha_frag);

	return scene;
}

elf_scene *elf_create_scene_from_pak(elf_pak *pak)
{
	elf_scene *scene;
	elf_camera *camera;
	elf_entity *entity;
	elf_light *light;
	elf_pak_index *index;
	FILE *file;
	int magic;
	char name[64];
	float ambient_color[4];
	unsigned char scene_read;

	scene = elf_create_scene();

	scene->name = elf_create_string(elf_get_pak_file_path(pak));
	scene->file_path = elf_create_string(elf_get_pak_file_path(pak));

	scene->pak = pak;
	elf_inc_ref((elf_object*)pak);

	scene_read = ELF_FALSE;
	for(index = (elf_pak_index*)elf_begin_list(pak->indexes); index;
		index = (elf_pak_index*)elf_next_in_list(pak->indexes))
	{
		if(index->index_type == ELF_CAMERA) camera = elf_get_or_load_camera_by_name(scene, index->name);
		else if(index->index_type == ELF_ENTITY) entity = elf_get_or_load_entity_by_name(scene, index->name);
		else if(index->index_type == ELF_LIGHT) light = elf_get_or_load_light_by_name(scene, index->name);
		else if(index->index_type == ELF_SCENE && !scene_read)
		{
			file = fopen(elf_get_pak_file_path(pak), "rb");
			if(file)
			{
				scene_read = ELF_TRUE;
				fseek(file, elf_get_pak_index_offset(index), SEEK_SET);

				fread((char*)&magic, sizeof(int), 1, file);
				if(magic != 179532120)
				{
					printf("warning: scene header section of \"%s\" is invalid\n", elf_get_pak_file_path(pak));
					continue;
				}

				fread(name, sizeof(char), 64, file);
				if(scene->name) elf_destroy_string(scene->name);
				scene->name = elf_create_string(name);

				fread((char*)ambient_color, sizeof(float), 4, file);

				elf_set_scene_ambient_color(scene, ambient_color[0], ambient_color[1], ambient_color[2], ambient_color[3]);

				fclose(file);
			}
		}
		elf_seek_list(pak->indexes, (elf_object*)index);
	}

	return scene;
}

elf_scene* elf_create_scene_from_file(const char *file_path)
{
	elf_pak *pak;
	elf_scene *scene;

	pak = elf_create_pak_from_file(file_path);
	if(!pak) return NULL;

	scene = elf_create_scene_from_pak(pak);

	return scene;
}

void elf_update_scene(elf_scene *scene, float sync)
{
	elf_camera *cam;
	elf_entity *ent;
	elf_light *light;
	elf_particles *par;
	float position[3];
	float orient[4];
	float vec_z[3] = {0.0, 0.0, -1.0};
	float vec_y[3] = {0.0, 1.0, -1.0};
	float front_up_vec[6];

	if(sync > 0.0)
	{
		elf_update_physics_world(scene->world, sync);
		//elf_update_physics_world(scene->dworld, sync);
	}

	if(scene->cur_camera)
	{
		elf_get_actor_position_((elf_actor*)scene->cur_camera, position);
		elf_get_actor_orientation_((elf_actor*)scene->cur_camera, orient);
		gfx_mul_qua_vec(orient, vec_z, &front_up_vec[0]);
		gfx_mul_qua_vec(orient, vec_y, &front_up_vec[3]);
		elf_set_audio_listener_position(position[0], position[1], position[2]);
		elf_set_audio_listener_orientation(front_up_vec);
	}

	// logics update pass
	for(cam = (elf_camera*)elf_begin_list(scene->cameras); cam != NULL;
		cam = (elf_camera*)elf_next_in_list(scene->cameras))
	{
		elf_update_camera(cam);
	}

	for(ent = (elf_entity*)elf_begin_list(scene->entities); ent != NULL;
		ent = (elf_entity*)elf_next_in_list(scene->entities))
	{
		elf_update_entity(ent);
	}

	for(light = (elf_light*)elf_begin_list(scene->lights); light != NULL;
		light = (elf_light*)elf_next_in_list(scene->lights))
	{
		elf_update_light(light);
	}

	for(par = (elf_particles*)elf_begin_list(scene->particles); par != NULL;
		par = (elf_particles*)elf_next_in_list(scene->particles))
	{
		elf_update_particles(par, sync);
	}

	// pre draw pass
	for(cam = (elf_camera*)elf_begin_list(scene->cameras); cam != NULL;
		cam = (elf_camera*)elf_next_in_list(scene->cameras))
	{
		elf_camera_pre_draw(cam);
	}

	for(ent = (elf_entity*)elf_begin_list(scene->entities); ent != NULL;
		ent = (elf_entity*)elf_next_in_list(scene->entities))
	{
		elf_entity_pre_draw(ent);
	}

	for(light = (elf_light*)elf_begin_list(scene->lights); light != NULL;
		light = (elf_light*)elf_next_in_list(scene->lights))
	{
		elf_light_pre_draw(light);
	}
}

void elf_destroy_scene(elf_scene *scene)
{
	elf_actor *actor;

	if(scene->name) elf_destroy_string(scene->name);
	if(scene->file_path) elf_destroy_string(scene->file_path);

	if(scene->entity_queue) elf_dec_ref((elf_object*)scene->entity_queue);

	for(actor = (elf_actor*)elf_begin_list(scene->cameras); actor;
		actor = (elf_actor*)elf_next_in_list(scene->cameras)) elf_remove_actor(actor);
	for(actor = (elf_actor*)elf_begin_list(scene->entities); actor;
		actor = (elf_actor*)elf_next_in_list(scene->entities)) elf_remove_actor(actor);
	for(actor = (elf_actor*)elf_begin_list(scene->lights); actor;
		actor = (elf_actor*)elf_next_in_list(scene->lights)) elf_remove_actor(actor);
	for(actor = (elf_actor*)elf_begin_list(scene->particles); actor;
		actor = (elf_actor*)elf_next_in_list(scene->particles)) elf_remove_actor(actor);

	if(scene->models) elf_dec_ref((elf_object*)scene->models);
	if(scene->scripts) elf_dec_ref((elf_object*)scene->scripts);
	if(scene->materials) elf_dec_ref((elf_object*)scene->materials);
	if(scene->textures) elf_dec_ref((elf_object*)scene->textures);
	if(scene->cameras) elf_dec_ref((elf_object*)scene->cameras);
	if(scene->entities) elf_dec_ref((elf_object*)scene->entities);
	if(scene->lights) elf_dec_ref((elf_object*)scene->lights);
	if(scene->armatures) elf_dec_ref((elf_object*)scene->armatures);
	if(scene->particles) elf_dec_ref((elf_object*)scene->particles);

	elf_destroy_physics_world(scene->world);
	elf_destroy_physics_world(scene->dworld);

	if(scene->dof_depth_write) gfx_destroy_shader_program(scene->dof_depth_write);
	if(scene->dof_depth_write_alpha) gfx_destroy_shader_program(scene->dof_depth_write_alpha);

	if(scene->pak) elf_dec_ref((elf_object*)scene->pak);

	free(scene);
}

void elf_set_scene_ambient_color(elf_scene *scene, float r, float g, float b, float a)
{
	scene->ambient_color.r = r;
	scene->ambient_color.g = g;
	scene->ambient_color.b = b;
	scene->ambient_color.a = a;
}

elf_color elf_get_scene_ambient_color(elf_scene *scene)
{
	return scene->ambient_color;
}


void elf_set_scene_gravity(elf_scene *scene, float x, float y, float z)
{
	elf_set_physics_world_gravity(scene->world, x, y, z);
}

elf_vec3f elf_get_scene_gravity(elf_scene *scene)
{
	return elf_get_physics_world_gravity(scene->world);
}

const char* elf_get_scene_name(elf_scene *scene)
{
	return scene->name;
}

const char* elf_get_scene_file_path(elf_scene *scene)
{
	return scene->file_path;
}

int elf_get_scene_camera_count(elf_scene *scene)
{
	return elf_get_list_length(scene->cameras);
}

int elf_get_scene_entity_count(elf_scene *scene)
{
	return elf_get_list_length(scene->entities);
}

int elf_get_scene_light_count(elf_scene *scene)
{
	return elf_get_list_length(scene->lights);
}

int elf_get_scene_armature_count(elf_scene *scene)
{
	return elf_get_list_length(scene->armatures);
}

int elf_get_scene_particles_count(elf_scene *scene)
{
	return elf_get_list_length(scene->particles);
}

void elf_set_actor_scene(elf_scene *scene, elf_actor *actor)
{
	elf_joint *joint;
	elf_actor *cact;

	if(actor->scene) elf_remove_actor_by_object(actor->scene, actor);

	actor->scene = scene;

	if(actor->object) elf_set_physics_object_world(actor->object, scene->world);
	if(actor->dobject) elf_set_physics_object_world(actor->dobject, scene->dworld);

	for(cact = (elf_actor*)elf_begin_list(actor->children); cact;
		cact = (elf_actor*)elf_next_in_list(actor->children))
	{
		elf_set_actor_scene(scene, cact);
	}

	for(joint = (elf_joint*)elf_begin_list(actor->joints); joint;
		joint = (elf_joint*)elf_next_in_list(actor->joints))
	{
		elf_set_joint_world(joint, scene->world);
	}
}

void elf_add_camera_to_scene(elf_scene *scene, elf_camera *camera)
{
	if(!camera) return;
	elf_set_actor_scene(scene, (elf_actor*)camera);
	elf_append_to_list(scene->cameras, (elf_object*)camera);
	if(elf_get_list_length(scene->cameras) == 1) scene->cur_camera = camera;
}

void elf_add_entity_to_scene(elf_scene *scene, elf_entity *entity)
{
	if(!entity) return;
	elf_set_actor_scene(scene, (elf_actor*)entity);
	elf_append_to_list(scene->entities, (elf_object*)entity);
}

void elf_add_light_to_scene(elf_scene *scene, elf_light *light)
{
	if(!light) return;
	elf_set_actor_scene(scene, (elf_actor*)light);
	elf_append_to_list(scene->lights, (elf_object*)light);
}

void elf_add_particles_to_scene(elf_scene *scene, elf_particles *particles)
{
	if(!particles) return;
	elf_set_actor_scene(scene, (elf_actor*)particles);
	elf_append_to_list(scene->particles, (elf_object*)particles);
}

void elf_set_scene_active_camera(elf_scene *scene, elf_camera *camera)
{
	scene->cur_camera = camera;
}

elf_camera* elf_get_scene_active_camera(elf_scene *scene)
{
	return scene->cur_camera;
}

elf_collision* elf_get_scene_ray_cast_result(elf_scene *scene, float x, float y, float z, float dx, float dy, float dz)
{
	return elf_get_ray_cast_result(scene->world, x, y, z, dx, dy, dz);
}

elf_list* elf_get_scene_ray_cast_results(elf_scene *scene, float x, float y, float z, float dx, float dy, float dz)
{
	return elf_get_ray_cast_results(scene->world, x, y, z, dx, dy, dz);
}

elf_collision* elf_get_debug_scene_ray_cast_result(elf_scene *scene, float x, float y, float z, float dx, float dy, float dz)
{
	return elf_get_ray_cast_result(scene->dworld, x, y, z, dx, dy, dz);
}

elf_list* elf_get_debug_scene_ray_cast_results(elf_scene *scene, float x, float y, float z, float dx, float dy, float dz)
{
	return elf_get_ray_cast_results(scene->dworld, x, y, z, dx, dy, dz);
}

elf_camera* elf_get_camera_by_index(elf_scene *scene, int idx)
{
	return (elf_camera*)elf_get_item_from_list(scene->cameras, idx);
}

elf_entity* elf_get_entity_by_index(elf_scene *scene, int idx)
{
	return (elf_entity*)elf_get_item_from_list(scene->entities, idx);
}

elf_light* elf_get_light_by_index(elf_scene *scene, int idx)
{
	return (elf_light*)elf_get_item_from_list(scene->lights, idx);
}

elf_armature* elf_get_armature_by_index(elf_scene *scene, int idx)
{
	return (elf_armature*)elf_get_item_from_list(scene->armatures, idx);
}

elf_particles* elf_get_particles_by_index(elf_scene *scene, int idx)
{
	return (elf_particles*)elf_get_item_from_list(scene->particles, idx);
}

elf_texture *elf_get_texture_by_name(elf_scene *scene, const char *name)
{
	elf_texture *texture;

	for(texture = (elf_texture*)elf_begin_list(scene->textures); texture;
		texture = (elf_texture*)elf_next_in_list(scene->textures))
	{
		if(!strcmp(texture->name, name)) return texture;
	}

	return NULL;
}

elf_model *elf_get_model_by_name(elf_scene *scene, const char *name)
{
	elf_model *model;

	for(model = (elf_model*)elf_begin_list(scene->models); model;
		model = (elf_model*)elf_next_in_list(scene->models))
	{
		if(!strcmp(model->name, name)) return model;
	}

	return NULL;
}

elf_script *elf_get_script_by_name(elf_scene *scene, const char *name)
{
	elf_script *script;

	for(script = (elf_script*)elf_begin_list(scene->scripts); script;
		script = (elf_script*)elf_next_in_list(scene->scripts))
	{
		if(!strcmp(script->name, name)) return script;
	}

	return NULL;
}

elf_material *elf_get_material_by_name(elf_scene *scene, const char *name)
{
	elf_material *material;

	for(material = (elf_material*)elf_begin_list(scene->materials); material;
		material = (elf_material*)elf_next_in_list(scene->materials))
	{
		if(!strcmp(material->name, name)) return material;
	}

	return NULL;
}

elf_actor *elf_get_actor_by_name(elf_scene *scene, const char *name)
{
	elf_actor *actor;

	actor = (elf_actor*)elf_get_camera_by_name(scene, name);
	if(actor) return actor;
	actor = (elf_actor*)elf_get_entity_by_name(scene, name);
	if(actor) return actor;
	actor = (elf_actor*)elf_get_light_by_name(scene, name);
	if(actor) return actor;
	actor = (elf_actor*)elf_get_armature_by_name(scene, name);
	if(actor) return actor;
	actor = (elf_actor*)elf_get_particles_by_name(scene, name);
	if(actor) return actor;

	return NULL;
}

elf_camera *elf_get_camera_by_name(elf_scene *scene, const char *name)
{
	elf_camera *camera;

	for(camera = (elf_camera*)elf_begin_list(scene->cameras); camera;
		camera = (elf_camera*)elf_next_in_list(scene->cameras))
	{
		if(!strcmp(camera->name, name)) return camera;
	}

	return NULL;
}

elf_entity *elf_get_entity_by_name(elf_scene *scene, const char *name)
{
	elf_entity *entity;

	for(entity = (elf_entity*)elf_begin_list(scene->entities); entity;
		entity = (elf_entity*)elf_next_in_list(scene->entities))
	{
		if(!strcmp(entity->name, name)) return entity;
	}

	return NULL;
}

elf_light *elf_get_light_by_name(elf_scene *scene, const char *name)
{
	elf_light *light;

	for(light = (elf_light*)elf_begin_list(scene->lights); light;
		light = (elf_light*)elf_next_in_list(scene->lights))
	{
		if(!strcmp(light->name, name)) return light;
	}

	return NULL;
}

elf_armature *elf_get_armature_by_name(elf_scene *scene, const char *name)
{
	elf_armature *armature;

	for(armature = (elf_armature*)elf_begin_list(scene->armatures); armature;
		armature = (elf_armature*)elf_next_in_list(scene->armatures))
	{
		if(!strcmp(armature->name, name)) return armature;
	}

	return NULL;
}

elf_particles *elf_get_particles_by_name(elf_scene *scene, const char *name)
{
	elf_particles *particles;

	for(particles = (elf_particles*)elf_begin_list(scene->particles); particles;
		particles = (elf_particles*)elf_next_in_list(scene->particles))
	{
		if(!strcmp(particles->name, name)) return particles;
	}

	return NULL;
}

elf_texture* elf_get_or_load_texture_by_name(elf_scene *scene, const char *name)
{
	elf_texture *texture;
	elf_pak_index *index;
	FILE *file;

	for(texture = (elf_texture*)elf_begin_list(scene->textures); texture;
		texture = (elf_texture*)elf_next_in_list(scene->textures))
	{
		if(!strcmp(texture->name, name)) return texture;
	}

	if(scene->pak)
	{
		index = elf_get_pak_index_by_name(scene->pak, name, ELF_TEXTURE);
		if(index)
		{
			file = fopen(scene->pak->file_path, "rb");
			fseek(file, elf_get_pak_index_offset(index), SEEK_SET);
			if(feof(file)) return NULL;

			texture = elf_create_texture_from_pak(file, name, scene);
			if(texture) elf_append_to_list(scene->textures, (elf_object*)texture);
			fclose(file);
			return texture;
		}
	}

	return NULL;
}

elf_model* elf_get_or_load_model_by_name(elf_scene *scene, const char *name)
{
	elf_model *model;
	elf_pak_index *index;
	FILE *file;

	for(model = (elf_model*)elf_begin_list(scene->models); model;
		model = (elf_model*)elf_next_in_list(scene->models))
	{
		if(!strcmp(model->name, name)) return model;
	}

	if(scene->pak)
	{
		index = elf_get_pak_index_by_name(scene->pak, name, ELF_MODEL);
		if(index)
		{
			file = fopen(scene->pak->file_path, "rb");
			fseek(file, elf_get_pak_index_offset(index), SEEK_SET);
			if(feof(file)) return NULL;

			model = elf_create_model_from_pak(file, name, scene);
			if(model) elf_append_to_list(scene->models, (elf_object*)model);
			fclose(file);
			return model;
		}
	}

	return NULL;
}

elf_script* elf_get_or_load_script_by_name(elf_scene *scene, const char *name)
{
	elf_script *script;
	elf_pak_index *index;
	FILE *file;

	for(script = (elf_script*)elf_begin_list(scene->scripts); script;
		script = (elf_script*)elf_next_in_list(scene->scripts))
	{
		if(!strcmp(script->name, name)) return script;
	}

	if(scene->pak)
	{
		index = elf_get_pak_index_by_name(scene->pak, name, ELF_SCRIPT);
		if(index)
		{
			file = fopen(scene->pak->file_path, "rb");
			fseek(file, elf_get_pak_index_offset(index), SEEK_SET);
			if(feof(file)) return NULL;

			script = elf_create_script_from_pak(file, name, scene);
			if(script) elf_append_to_list(scene->scripts, (elf_object*)script);
			fclose(file);
			return script;
		}
	}

	return NULL;
}

elf_material *elf_get_or_load_material_by_name(elf_scene *scene, const char *name)
{
	elf_material *material;
	elf_pak_index *index;
	FILE *file;

	for(material = (elf_material*)elf_begin_list(scene->materials); material;
		material = (elf_material*)elf_next_in_list(scene->materials))
	{
		if(!strcmp(material->name, name)) return material;
	}

	if(scene->pak)
	{
		index = elf_get_pak_index_by_name(scene->pak, name, ELF_MATERIAL);
		if(index)
		{
			file = fopen(scene->pak->file_path, "rb");
			fseek(file, elf_get_pak_index_offset(index), SEEK_SET);
			if(feof(file)) return NULL;

			material = elf_create_material_from_pak(file, name, scene);
			if(material) elf_append_to_list(scene->materials, (elf_object*)material);
			fclose(file);
			return material;
		}
	}

	return NULL;
}

elf_camera *elf_get_or_load_camera_by_name(elf_scene *scene, const char *name)
{
	elf_camera *camera;
	elf_pak_index *index;
	FILE *file;

	for(camera = (elf_camera*)elf_begin_list(scene->cameras); camera;
		camera = (elf_camera*)elf_next_in_list(scene->cameras))
	{
		if(!strcmp(camera->name, name)) return camera;
	}

	if(scene->pak)
	{
		index = elf_get_pak_index_by_name(scene->pak, name, ELF_CAMERA);
		if(index)
		{
			file = fopen(scene->pak->file_path, "rb");
			fseek(file, elf_get_pak_index_offset(index), SEEK_SET);
			if(feof(file)) return NULL;

			camera = elf_create_camera_from_pak(file, name, scene);
			if(camera) elf_add_camera_to_scene(scene, camera);
			fclose(file);
			return camera;
		}
	}

	return NULL;
}

elf_entity *elf_get_or_load_entity_by_name(elf_scene *scene, const char *name)
{
	elf_entity *entity;
	elf_pak_index *index;
	FILE *file;

	for(entity = (elf_entity*)elf_begin_list(scene->entities); entity;
		entity = (elf_entity*)elf_next_in_list(scene->entities))
	{
		if(!strcmp(entity->name, name)) return entity;
	}

	if(scene->pak)
	{
		index = elf_get_pak_index_by_name(scene->pak, name, ELF_ENTITY);
		if(index)
		{
			file = fopen(scene->pak->file_path, "rb");
			fseek(file, elf_get_pak_index_offset(index), SEEK_SET);
			if(feof(file)) return NULL;

			entity = elf_create_entity_from_pak(file, name, scene);
			if(entity) elf_add_entity_to_scene(scene, entity);
			fclose(file);
			return entity;
		}
	}

	return NULL;
}

elf_light *elf_get_or_load_light_by_name(elf_scene *scene, const char *name)
{
	elf_light *light;
	elf_pak_index *index;
	FILE *file;

	for(light = (elf_light*)elf_begin_list(scene->lights); light;
		light = (elf_light*)elf_next_in_list(scene->lights))
	{
		if(!strcmp(light->name, name)) return light;
	}

	if(scene->pak)
	{
		index = elf_get_pak_index_by_name(scene->pak, name, ELF_LIGHT);
		if(index)
		{
			file = fopen(scene->pak->file_path, "rb");
			fseek(file, elf_get_pak_index_offset(index), SEEK_SET);
			if(feof(file)) return NULL;

			light = elf_create_light_from_pak(file, name, scene);
			if(light) elf_add_light_to_scene(scene, light);
			fclose(file);
			return light;
		}
	}

	return NULL;
}

elf_armature *elf_get_or_load_armature_by_name(elf_scene *scene, const char *name)
{
	elf_armature *armature;
	elf_pak_index *index;
	FILE *file;

	for(armature = (elf_armature*)elf_begin_list(scene->armatures); armature;
		armature = (elf_armature*)elf_next_in_list(scene->armatures))
	{
		if(!strcmp(armature->name, name)) return armature;
	}

	if(scene->pak)
	{
		index = elf_get_pak_index_by_name(scene->pak, name, ELF_ARMATURE);
		if(index)
		{
			file = fopen(scene->pak->file_path, "rb");
			fseek(file, elf_get_pak_index_offset(index), SEEK_SET);
			if(feof(file)) return NULL;

			armature = elf_create_armature_from_pak(file, name, scene);
			elf_append_to_list(scene->armatures, (elf_object*)armature);
			fclose(file);
			return armature;
		}
	}

	return NULL;
}

elf_particles *elf_get_or_load_particles_by_name(elf_scene *scene, const char *name)
{
	elf_particles *particles;

	for(particles = (elf_particles*)elf_begin_list(scene->particles); particles;
		particles = (elf_particles*)elf_next_in_list(scene->particles))
	{
		if(!strcmp(particles->name, name)) return particles;
	}

	return NULL;
}

elf_actor *elf_get_or_load_actor_by_name(elf_scene *scene, const char *name)
{
	elf_actor *actor;

	actor = (elf_actor*)elf_get_or_load_camera_by_name(scene, name);
	if(actor) return actor;
	actor = (elf_actor*)elf_get_or_load_entity_by_name(scene, name);
	if(actor) return actor;
	actor = (elf_actor*)elf_get_or_load_light_by_name(scene, name);
	if(actor) return actor;
	actor = (elf_actor*)elf_get_or_load_particles_by_name(scene, name);
	if(actor) return actor;

	return NULL;
}

void elf_remove_actor(elf_actor *actor)
{
	elf_joint *joint;

	elf_set_actor_parent(actor, NULL);
	elf_remove_actor_children(actor);
	actor->scene = NULL;

	if(actor->object)
	{
		elf_remove_physics_object_collisions(actor->object);
		elf_set_physics_object_world(actor->object, NULL);
	}
	if(actor->dobject)
	{
		elf_remove_physics_object_collisions(actor->dobject);
		elf_set_physics_object_world(actor->dobject, NULL);
	}

	for(joint = (elf_joint*)elf_begin_list(actor->joints); joint;
		joint = (elf_joint*)elf_next_in_list(actor->joints))
	{
		elf_set_joint_world(joint, NULL);
	}

	elf_dec_ref((elf_object*)actor->joints);
	actor->joints = elf_create_list();
	elf_inc_ref((elf_object*)actor->joints);
}

unsigned char elf_remove_camera_by_name(elf_scene *scene, const char *name)
{
	elf_camera *cam;

	for(cam = (elf_camera*)elf_begin_list(scene->cameras); cam != NULL;
		cam = (elf_camera*)elf_next_in_list(scene->cameras))
	{
		if(!strcmp(cam->name, name))
		{
			elf_remove_actor((elf_actor*)cam);
			elf_remove_from_list(scene->cameras, (elf_object*)cam);
			if(scene->cur_camera == cam) scene->cur_camera = elf_get_camera_by_index(scene, 0);
			return ELF_TRUE;
		}
	}

	return ELF_FALSE;
}

unsigned char elf_remove_entity_by_name(elf_scene *scene, const char *name)
{
	elf_entity *ent;

	for(ent = (elf_entity*)elf_begin_list(scene->entities); ent != NULL;
		ent = (elf_entity*)elf_next_in_list(scene->entities))
	{
		if(!strcmp(ent->name, name))
		{
			elf_remove_actor((elf_actor*)ent);
			elf_remove_from_list(scene->entities, (elf_object*)ent);
			return ELF_TRUE;
		}
	}

	return ELF_FALSE;
}

unsigned char elf_remove_light_by_name(elf_scene *scene, const char *name)
{
	elf_light *lig;

	for(lig = (elf_light*)elf_begin_list(scene->lights); lig != NULL;
		lig = (elf_light*)elf_next_in_list(scene->lights))
	{
		if(!strcmp(lig->name, name))
		{
			elf_remove_actor((elf_actor*)lig);
			elf_remove_from_list(scene->lights, (elf_object*)lig);
			return ELF_TRUE;
		}
	}

	return ELF_FALSE;
}

unsigned char elf_remove_particles_by_name(elf_scene *scene, const char *name)
{
	elf_particles *par;

	for(par = (elf_particles*)elf_begin_list(scene->particles); par != NULL;
		par = (elf_particles*)elf_next_in_list(scene->particles))
	{
		if(!strcmp(par->name, name))
		{
			elf_remove_actor((elf_actor*)par);
			elf_remove_from_list(scene->particles, (elf_object*)par);
			return ELF_TRUE;
		}
	}

	return ELF_FALSE;
}

unsigned char elf_remove_camera_by_index(elf_scene *scene, int idx)
{
	elf_camera *cam;
	int i;

	if(idx < 0 || idx > elf_get_list_length(scene->cameras)-1) return ELF_FALSE;

	for(i = 0, cam = (elf_camera*)elf_begin_list(scene->cameras); cam != NULL;
		cam = (elf_camera*)elf_next_in_list(scene->cameras), i++)
	{
		if(i == idx)
		{
			elf_remove_actor((elf_actor*)cam);
			elf_remove_from_list(scene->cameras, (elf_object*)cam);
			if(scene->cur_camera == cam)
			{
				scene->cur_camera = elf_get_camera_by_index(scene, 0);
			}
			return ELF_TRUE;
		}
	}

	return ELF_FALSE;
}

unsigned char elf_remove_entity_by_index(elf_scene *scene, int idx)
{
	elf_entity *ent;
	int i;

	if(idx < 0 || idx > elf_get_list_length(scene->entities)-1) return ELF_FALSE;

	for(i = 0, ent = (elf_entity*)elf_begin_list(scene->entities); ent != NULL;
		ent = (elf_entity*)elf_next_in_list(scene->entities), i++)
	{
		if(i == idx)
		{
			elf_remove_actor((elf_actor*)ent);
			elf_remove_from_list(scene->entities, (elf_object*)ent);
			return ELF_TRUE;
		}
	}

	return ELF_FALSE;
}

unsigned char elf_remove_light_by_index(elf_scene *scene, int idx)
{
	elf_light *lig;
	int i;

	if(idx < 0 || idx > elf_get_list_length(scene->lights)-1) return ELF_FALSE;

	for(i = 0, lig = (elf_light*)elf_begin_list(scene->lights); lig != NULL;
		lig = (elf_light*)elf_next_in_list(scene->lights), i++)
	{
		if(i == idx)
		{
			elf_remove_actor((elf_actor*)lig);
			elf_remove_from_list(scene->lights, (elf_object*)lig);
			return ELF_TRUE;
		}
	}

	return ELF_FALSE;
}

unsigned char elf_remove_particles_by_index(elf_scene *scene, int idx)
{
	elf_particles *par;
	int i;

	if(idx < 0 || idx > elf_get_list_length(scene->particles)-1) return ELF_FALSE;

	for(i = 0, par = (elf_particles*)elf_begin_list(scene->particles); par != NULL;
		par = (elf_particles*)elf_next_in_list(scene->particles), i++)
	{
		if(i == idx)
		{
			elf_remove_actor((elf_actor*)par);
			elf_remove_from_list(scene->particles, (elf_object*)par);
			return ELF_TRUE;
		}
	}

	return ELF_FALSE;
}

unsigned char elf_remove_camera_by_object(elf_scene *scene, elf_camera *camera)
{
	elf_remove_actor((elf_actor*)camera);
	if(scene->cur_camera == camera) scene->cur_camera = elf_get_camera_by_index(scene, 0);
	return elf_remove_from_list(scene->cameras, (elf_object*)camera);
}

unsigned char elf_remove_entity_by_object(elf_scene *scene, elf_entity *entity)
{
	elf_remove_actor((elf_actor*)entity);
	return elf_remove_from_list(scene->entities, (elf_object*)entity);
}

unsigned char elf_remove_light_by_object(elf_scene *scene, elf_light *light)
{
	elf_remove_actor((elf_actor*)light);
	return elf_remove_from_list(scene->lights, (elf_object*)light);
}

unsigned char elf_remove_particles_by_object(elf_scene *scene, elf_particles *particles)
{
	elf_remove_actor((elf_actor*)particles);
	return elf_remove_from_list(scene->particles, (elf_object*)particles);
}

unsigned char elf_remove_actor_by_object(elf_scene *scene, elf_actor *actor)
{
	unsigned char result;

	result = elf_remove_camera_by_object(scene, (elf_camera*)actor);
	if(result) return result;
	result = elf_remove_entity_by_object(scene, (elf_entity*)actor);
	if(result) return result;
	result = elf_remove_light_by_object(scene, (elf_light*)actor);
	if(result) return result;
	result = elf_remove_particles_by_object(scene, (elf_particles*)actor);
	if(result) return result;

	return ELF_FALSE;
}

void elf_draw_scene(elf_scene *scene)
{
	elf_light *light;
	elf_entity *ent;
	elf_particles *par;
	float bias[16] = {0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0};
	float temp_mat1[16];
	float temp_mat2[16];
	gfx_render_target *render_target;
	int i, j;
	elf_vec3f lpos;
	elf_vec3f epos;
	elf_vec3f dvec;
	float dist, att;
	unsigned char found;

	if(!scene->cur_camera) return;

	render_target = gfx_get_cur_render_target();

	// draw depth buffer
	gfx_set_shader_params_default(&scene->shader_params);
	elf_set_camera(scene->cur_camera, &scene->shader_params);
	scene->shader_params.render_params.color_write = ELF_FALSE;
	scene->shader_params.render_params.alpha_write = ELF_FALSE;

	scene->entity_queue_count = 0;
	elf_begin_list(scene->entity_queue);

	for(ent = (elf_entity*)elf_begin_list(scene->entities); ent != NULL;
		ent = (elf_entity*)elf_next_in_list(scene->entities))
	{
		if(!elf_cull_entity(ent, scene->cur_camera))
		{
			if(scene->entity_queue_count < elf_get_list_length(scene->entity_queue))
			{
				elf_set_list_cur_ptr(scene->entity_queue, (elf_object*)ent);
				elf_next_in_list(scene->entity_queue);
			}
			else
			{
				elf_append_to_list(scene->entity_queue, (elf_object*)ent);
			}
			scene->entity_queue_count++;
			elf_draw_entity_without_materials(ent, &scene->shader_params);
			ent->culled = ELF_FALSE;
		}
		else
		{
			ent->culled = ELF_TRUE;
		}
	}

	// initiate occlusion queries
	if(eng->occlusion_culling)
	{
		gfx_set_shader_params_default(&scene->shader_params);
		elf_set_camera(scene->cur_camera, &scene->shader_params);
		scene->shader_params.render_params.depth_write = GFX_FALSE;
		scene->shader_params.render_params.depth_func = GFX_LEQUAL;
		scene->shader_params.render_params.color_write = GFX_FALSE;
		scene->shader_params.render_params.alpha_write = GFX_FALSE;
		scene->shader_params.render_params.cull_face = GFX_FALSE;
		scene->shader_params.render_params.offset_bias = -1.0f;
		scene->shader_params.render_params.offset_scale = -1.0f;

		for(i = 0, ent = (elf_entity*)elf_begin_list(scene->entity_queue);
			i < scene->entity_queue_count && ent != NULL;
			i++, ent = (elf_entity*)elf_next_in_list(scene->entity_queue))
		{
			gfx_begin_query(ent->query);
			elf_draw_entity_bounding_box(ent, &scene->shader_params);
			gfx_end_query(ent->query);
		}

		scene->shader_params.render_params.offset_bias = 0.0f;
		scene->shader_params.render_params.offset_scale = 0.0f;
	}

	eng->ambient_color = scene->ambient_color;

	// draw ambient pass
	if(!elf_about_zero(eng->ambient_color.r) ||
		!elf_about_zero(eng->ambient_color.g) ||
		!elf_about_zero(eng->ambient_color.b) )
	{
		gfx_set_shader_params_default(&scene->shader_params);
		elf_set_camera(scene->cur_camera, &scene->shader_params);

		scene->shader_params.render_params.depth_write = GFX_FALSE;
		scene->shader_params.render_params.depth_func = GFX_EQUAL;
		scene->shader_params.render_params.blend_mode = GFX_ADD;

		for(i = 0, ent = (elf_entity*)elf_begin_list(scene->entity_queue);
			i < scene->entity_queue_count && ent != NULL;
			i++, ent = (elf_entity*)elf_next_in_list(scene->entity_queue))
		{
			elf_draw_entity_ambient(ent, &scene->shader_params);
		}
	}

	// used for detecting if some non lit geometry has been rendered already
	eng->non_lit_flag = !eng->non_lit_flag;

	// render lighting
	for(light = (elf_light*)elf_begin_list(scene->lights); light != NULL;
		light = (elf_light*)elf_next_in_list(scene->lights))
	{
		if(!elf_get_light_visible(light)) continue;

		// need to call this for frustum culling...
		if(light->light_type == ELF_SPOT_LIGHT)
		{
			elf_set_camera_viewport(light->shadow_camera, 0, 0, elf_get_shadow_map_size(), elf_get_shadow_map_size());
			elf_set_camera(light->shadow_camera, &scene->shader_params);

			// check are there any entities visible for the spot, if there aren't don't bother continuing, just skip to the next light
			found = ELF_FALSE;
			for(i = 0, ent = (elf_entity*)elf_begin_list(scene->entity_queue);
				i < scene->entity_queue_count && ent != NULL;
				i++, ent = (elf_entity*)elf_next_in_list(scene->entity_queue))
			{
				if(!elf_cull_entity(ent, light->shadow_camera))
				{
					found = ELF_TRUE;
					break;
				}
			}
			if(!found) continue;
		}

		// render shadow map if needed
		if(light->light_type == ELF_SPOT_LIGHT && elf_get_light_shadow_caster(light))
		{
			gfx_set_shader_params_default(&scene->shader_params);
			scene->shader_params.render_params.color_write = GFX_FALSE;
			scene->shader_params.render_params.alpha_write = GFX_FALSE;
			scene->shader_params.render_params.offset_bias = 2.0f;
			scene->shader_params.render_params.offset_scale = 4.0f;
			elf_set_camera(light->shadow_camera, &scene->shader_params);
			gfx_set_shader_params(&scene->shader_params);

			gfx_set_render_target(eng->shadow_target);
			gfx_clear_depth_buffer(1.0);

			for(ent = (elf_entity*)elf_begin_list(scene->entities); ent != NULL;
				ent = (elf_entity*)elf_next_in_list(scene->entities))
			{
				if(!elf_cull_entity(ent, light->shadow_camera))
				{
					elf_draw_entity_without_materials(ent, &scene->shader_params);
				}
			}

			gfx_mul_matrix4_matrix4(elf_get_camera_projection_matrix(light->shadow_camera), bias, temp_mat1);
			gfx_mul_matrix4_matrix4(elf_get_camera_modelview_matrix(light->shadow_camera), temp_mat1, temp_mat2);
			gfx_matrix4_lol_invert(elf_get_camera_modelview_matrix(scene->cur_camera), temp_mat1);
			gfx_mul_matrix4_matrix4(temp_mat1, temp_mat2, light->projection_matrix);

			if(render_target) gfx_set_render_target(render_target);
			else gfx_disable_render_target();
		}

		// render lighting
		gfx_set_shader_params_default(&scene->shader_params);
		scene->shader_params.render_params.depth_write = GFX_FALSE;
		scene->shader_params.render_params.depth_func = GFX_EQUAL;
		scene->shader_params.render_params.color_write = GFX_TRUE;
		scene->shader_params.render_params.alpha_write = GFX_TRUE;
		scene->shader_params.render_params.blend_mode = GFX_ADD;
		elf_set_camera(scene->cur_camera, &scene->shader_params);

		if(light->light_type == ELF_SPOT_LIGHT && elf_get_light_shadow_caster(light))
		{
			scene->shader_params.texture_params[GFX_MAX_TEXTURES-1].type = GFX_SHADOW_MAP;
			scene->shader_params.texture_params[GFX_MAX_TEXTURES-1].texture = eng->shadow_map;
			scene->shader_params.texture_params[GFX_MAX_TEXTURES-1].projection_mode = GFX_SHADOW_PROJECTION;
			memcpy(scene->shader_params.texture_params[GFX_MAX_TEXTURES-1].matrix,
				light->projection_matrix, sizeof(float)*16);
		}
		else
		{
			scene->shader_params.texture_params[GFX_MAX_TEXTURES-1].type = GFX_COLOR_MAP;
			scene->shader_params.texture_params[GFX_MAX_TEXTURES-1].texture = NULL;
			scene->shader_params.texture_params[GFX_MAX_TEXTURES-1].projection_mode = GFX_NONE;
			gfx_matrix4_set_identity(scene->shader_params.texture_params[GFX_MAX_TEXTURES-1].matrix);
		}

		elf_set_light(light, scene->cur_camera, &scene->shader_params);

		// get the light position for culling point light entities
		lpos = elf_get_actor_position((elf_actor*)light);
		for(i = 0, ent = (elf_entity*)elf_begin_list(scene->entity_queue);
			i < scene->entity_queue_count && ent != NULL;
			i++, ent = (elf_entity*)elf_next_in_list(scene->entity_queue))
		{
			// get the entity position for culling point light entities and testing against bounding sphere
			epos = elf_get_actor_position((elf_actor*)ent);
			if((eng->occlusion_culling && gfx_get_query_result(ent->query) > 0) || !eng->occlusion_culling ||
				elf_camera_inside_sphere(scene->cur_camera, &ent->position.x, ent->cull_radius))
			{
				if(light->light_type == ELF_SPOT_LIGHT)
				{
					if(!elf_cull_entity(ent, light->shadow_camera))
					{
						elf_draw_entity(ent, &scene->shader_params);
					}
				}
				else if(light->light_type == ELF_POINT_LIGHT)
				{
					dvec = elf_sub_vec3f_vec3f(epos, lpos);
					dist = elf_get_vec3f_length(dvec);
					dist -= ent->cull_radius;
					att = 1.0-elf_float_max(dist-light->distance, 0.0)*light->fade_speed;
					if(att > 0.0)
					{
						elf_draw_entity(ent, &scene->shader_params);
					}
				}
				else
				{
					elf_draw_entity(ent, &scene->shader_params);
				}
			}
			else
			{
				ent->culled = ELF_TRUE;
			}
		}
	}

	// render particles
	gfx_set_shader_params_default(&scene->shader_params);
	scene->shader_params.render_params.depth_write = GFX_FALSE;
	scene->shader_params.render_params.depth_func = GFX_LEQUAL;
	scene->shader_params.render_params.color_write = GFX_TRUE;
	scene->shader_params.render_params.alpha_write = GFX_TRUE;
	elf_set_camera(scene->cur_camera, &scene->shader_params);
	
	for(par = (elf_particles*)elf_begin_list(scene->particles); par;
		par = (elf_particles*)elf_next_in_list(scene->particles))
	{
		elf_draw_particles(par, scene->cur_camera, &scene->shader_params);
	}

	// render stuff for dof...
	if(elf_is_dof())
	{
		gfx_set_shader_params_default(&scene->shader_params);
		scene->shader_params.render_params.depth_write = GFX_FALSE;
		scene->shader_params.render_params.depth_func = GFX_EQUAL;
		scene->shader_params.render_params.color_write = GFX_FALSE;
		scene->shader_params.shader_program = scene->dof_depth_write;
		elf_set_camera(scene->cur_camera, &scene->shader_params);

		gfx_set_shader_params(&scene->shader_params);
		gfx_set_shader_program_uniform_1f("elf_FocalRange", elf_get_dof_focal_range());
		gfx_set_shader_program_uniform_1f("elf_FocalDistance", elf_get_dof_focal_distance());

		for(i = 0, ent = (elf_entity*)elf_begin_list(scene->entity_queue);
			i < scene->entity_queue_count && ent != NULL;
			i++, ent = (elf_entity*)elf_next_in_list(scene->entity_queue))
		{
			if((!ent->culled || !eng->occlusion_culling) && ent->model && ent->model->vertex_array && ent->visible)
			{
				elf_pre_draw_entity(ent);

				gfx_mul_matrix4_matrix4(gfx_get_transform_matrix(ent->transform),
					scene->shader_params.camera_matrix, scene->shader_params.modelview_matrix);

				gfx_set_vertex_array(ent->model->vertex_array);
				gfx_set_shader_params(&scene->shader_params);

				for(j = 0; j < (int)ent->model->area_count; j++)
				{
					if(ent->model->areas[j].vertex_index)
					{
						gfx_draw_vertex_index(ent->model->areas[j].vertex_index, GFX_TRIANGLES);
					}
				}
			}
		}
	}

	// reset state just to be sure...
	gfx_set_shader_params_default(&scene->shader_params);
	gfx_set_shader_params(&scene->shader_params);

	// keep the query lists compact
	if(elf_get_list_length(scene->entity_queue) > scene->entity_queue_count)
	{
		ent = (elf_entity*)elf_rbegin_list(scene->entity_queue);
		if(ent) elf_remove_from_list(scene->entity_queue, (elf_object*)ent);
	}
}

void elf_draw_scene_debug(elf_scene *scene)
{
	elf_entity *ent;
	elf_light *lig;
	elf_camera *cam;

	if(!scene->cur_camera) return;

	gfx_set_shader_params_default(&scene->shader_params);
	scene->shader_params.render_params.depth_write = GFX_FALSE;
	scene->shader_params.render_params.depth_test = GFX_FALSE;
	scene->shader_params.render_params.blend_mode = GFX_ADD;
	elf_set_camera(scene->cur_camera, &scene->shader_params);

	for(ent = (elf_entity*)elf_begin_list(scene->entities); ent != NULL;
		ent = (elf_entity*)elf_next_in_list(scene->entities))
	{
		elf_draw_entity_debug(ent, &scene->shader_params);
	}

	gfx_set_shader_params_default(&scene->shader_params);
	scene->shader_params.render_params.depth_write = GFX_FALSE;
	scene->shader_params.render_params.depth_test = GFX_FALSE;
	scene->shader_params.render_params.blend_mode = GFX_ADD;
	gfx_set_color(&scene->shader_params.material_params.color, 0.5, 0.5, 0.2, 1.0);
	elf_set_camera(scene->cur_camera, &scene->shader_params);

	for(lig = (elf_light*)elf_begin_list(scene->lights); lig != NULL;
		lig = (elf_light*)elf_next_in_list(scene->lights))
	{
		elf_draw_light_debug(lig, &scene->shader_params);
	}

	gfx_set_shader_params_default(&scene->shader_params);
	scene->shader_params.render_params.depth_write = GFX_FALSE;
	scene->shader_params.render_params.depth_test = GFX_FALSE;
	elf_set_camera(scene->cur_camera, &scene->shader_params);

	for(cam = (elf_camera*)elf_begin_list(scene->cameras); cam != NULL;
		cam = (elf_camera*)elf_next_in_list(scene->cameras))
	{
		if(cam == scene->cur_camera) continue;
		elf_draw_camera_debug(cam, &scene->shader_params);
	}

	// reset state just to be sure...
	gfx_set_shader_params_default(&scene->shader_params);
	gfx_set_shader_params(&scene->shader_params);
}

