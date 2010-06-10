
elf_pak_index* elf_create_pak_index()
{
	elf_pak_index *index;

	index = (elf_pak_index*)malloc(sizeof(elf_pak_index));
	memset(index, 0x0, sizeof(elf_pak_index));
	index->type = ELF_PAK_INDEX;

	global_obj_count++;

	return index;
}

void elf_destroy_pak_index(elf_pak_index *index)
{
	if(index->name) elf_destroy_string(index->name);

	free(index);

	global_obj_count--;
}

elf_pak* elf_create_pak_from_file(const char *file_path)
{
	elf_pak *pak;
	elf_pak_index *index;
	FILE *file;
	int magic;
	int index_count;
	int i;

	unsigned char type;
	char name[64];
	unsigned int offset;

	file = fopen(file_path, "rb");
	if(!file)
	{
		elf_set_error(ELF_CANT_OPEN_FILE, "error: could not open \"%s\"\n", file_path);
		return NULL;
	}

	magic = 0;
	fread((char*)&magic, sizeof(int), 1, file);

	if(magic != 179532100)
	{
		elf_set_error(ELF_CANT_OPEN_FILE, "error: \"%s\" is not a elf pak file\n", file_path);
		fclose(file);
		return NULL;
	}

	pak = (elf_pak*)malloc(sizeof(elf_pak));
	memset(pak, 0x0, sizeof(elf_pak));
	pak->type = ELF_PAK;

	global_obj_count++;

	pak->file_path = elf_create_string(file_path);

	pak->indexes = elf_create_list();
	elf_inc_ref((elf_object*)pak->indexes);

	index_count = 0;
	fread((char*)&index_count, sizeof(unsigned int), 1, file);

	for(i = 0; i < index_count; i++)
	{
		type = 0;
		offset = 0;
		fread((char*)&type, sizeof(unsigned char), 1, file);

		switch(type)
		{
			case ELF_TEXTURE: pak->texture_count++; break;
			case ELF_MATERIAL: pak->material_count++; break;
			case ELF_MODEL: pak->model_count++; break;
			case ELF_CAMERA: pak->camera_count++; break;
			case ELF_ENTITY: pak->entity_count++; break;
			case ELF_LIGHT: pak->light_count++; break;
			case ELF_ARMATURE: pak->armature_count++; break;
			case ELF_SCENE:	pak->scene_count++; break;
			case ELF_SCRIPT: pak->script_count++; break;
		}

		fread(name, sizeof(char), 64, file);
		fread((char*)&offset, sizeof(unsigned int), 1, file);

		index = elf_create_pak_index();
		index->index_type = type;
		index->name = elf_create_string(name);
		index->offset = offset;

		elf_append_to_list(pak->indexes, (elf_object*)index);
	}

	fclose(file);

	return pak;
}

void elf_destroy_pak(elf_pak *pak)
{
	if(pak->file_path) elf_destroy_string(pak->file_path);

	elf_dec_ref((elf_object*)pak->indexes);

	free(pak);

	global_obj_count--;
}

const char* elf_get_pak_file_path(elf_pak *pak)
{
	return pak->file_path;
}

int elf_get_pak_index_count(elf_pak *pak)
{
	return elf_get_list_length(pak->indexes);
}

elf_pak_index* elf_get_pak_index_by_name(elf_pak *pak, const char *name, unsigned char type)
{
	elf_pak_index *index;

	for(index = (elf_pak_index*)elf_begin_list(pak->indexes); index;
		index = (elf_pak_index*)elf_next_in_list(pak->indexes))
	{
		if(!strcmp(index->name, name) && index->index_type == type) return index;
	}

	return NULL;
}

elf_pak_index* elf_get_pak_index_by_index(elf_pak *pak, int idx)
{
	if(idx < 0 || idx > elf_get_list_length(pak->indexes)-1) return NULL;
	return (elf_pak_index*)elf_get_item_from_list(pak->indexes, idx);
}

unsigned char elf_get_pak_index_type(elf_pak_index *index)
{
	return index->type;
}

const char* elf_get_pak_index_name(elf_pak_index *index)
{
	return index->name;
}

int elf_get_pak_index_offset(elf_pak_index *index)
{
	return index->offset;
}

int elf_get_scene_size_bytes(elf_scene *scene)
{
	int size_bytes;

	size_bytes = 0;

	size_bytes += sizeof(int);	// magic
	size_bytes += sizeof(char)*64;	// name
	size_bytes += sizeof(float)*4;	// ambience

	return size_bytes;
}

int elf_get_script_size_bytes(elf_script *script)
{
	int size_bytes;

	size_bytes = 0;

	size_bytes += sizeof(int);	// magic
	size_bytes += sizeof(char)*64;	// name
	size_bytes += sizeof(int);	// length
	if(script->text) size_bytes += sizeof(char)*strlen(script->text);	// text

	return size_bytes;
}

void elf_get_actor_header_size_bytes(elf_actor *actor)
{
	int size_bytes;
	elf_bezier_curve *curve;

	size_bytes = 0;

	size_bytes += sizeof(char)*64;	// name
	size_bytes += sizeof(char)*64;	// parent
	size_bytes += sizeof(char)*64;	// script
	size_bytes += sizeof(float)*3;	// position
	size_bytes += sizeof(float)*3;	// rotation
	size_bytes += sizeof(unsigned char);	// curve count

	if(actor->ipo)
	{
		for(curve = (elf_bezier_curve*)elf_begin_list(actor->ipo->curves); curve;
			curve = (elf_bezier_curve*)elf_begin_list(actor->ipo->curves))
		{
			size_bytes += sizeof(unsigned char);	// type
			size_bytes += sizeof(unsigned char);	// interpolation
			size_bytes += sizeof(int);	// point count
			size_bytes += sizeof(float)*6*elf_get_list_length(curve->points);	// points
		}
	}
}

void elf_read_actor_header(elf_actor *actor, FILE *file, elf_scene *scene)
{
	char name[64];
	char parent_name[64];
	char script_name[64];
	float position[3];
	float rotation[3];
	elf_script *script;
	int i, j;
	elf_bezier_point *point;
	elf_bezier_curve *curve;
	unsigned char curve_count;
	int point_count;

	fread(name, sizeof(char), 64, file);
	fread(parent_name, sizeof(char), 64, file);
	fread(script_name, sizeof(char), 64, file);

	actor->name = elf_create_string(name);
	if(scene) actor->file_path = elf_create_string(elf_get_scene_file_path(scene));

	fread((char*)position, sizeof(float), 3, file);
	fread((char*)rotation, sizeof(float), 3, file);

	elf_set_actor_position(actor, position[0], position[1], position[2]);
	elf_set_actor_rotation(actor, rotation[0], rotation[1], rotation[2]);

	script = NULL;

	if(scene && strlen(script_name) > 0)
	{
		script = elf_get_or_load_script_by_name(scene, script_name);
		if(script) elf_set_actor_script(actor, script);
	}

	curve_count = 0;
	fread((char*)&curve_count, sizeof(unsigned char), 1, file);
	for(i = 0; i < curve_count; i++)
	{
		curve = elf_create_bezier_curve();
		fread((char*)&curve->curve_type, sizeof(unsigned char), 1, file);
		fread((char*)&curve->interpolation, sizeof(unsigned char), 1, file);

		point_count = 0;
		fread((char*)&point_count, sizeof(int), 1, file);
		for(j = 0; j < point_count; j++)
		{
			point = elf_create_bezier_point();
			fread((char*)&point->c1.x, sizeof(float), 2, file);
			fread((char*)&point->p.x, sizeof(float), 2, file);
			fread((char*)&point->c2.x, sizeof(float), 2, file);

			elf_add_point_to_bezier_curve(curve, point);
		}

		elf_add_curve_to_ipo(actor->ipo, curve);
	}
}

elf_armature* elf_create_armature_from_pak(FILE *file, const char *name, elf_scene *scene)
{
	elf_armature *armature;
	char rname[64];
	char parent[64];
	int magic;
	elf_bone *bone;
	elf_bone *parent_bone;
	elf_list *bones;
	elf_list *bone_parents;
	elf_string *str_obj;
	int i, j;
	float bone_inv_qua[4];

	fread((char*)&magic, sizeof(int), 1, file);

	if(magic != 179532122)
	{
		elf_set_error(ELF_INVALID_FILE, "error: invalid armature \"%s//%s\", wrong magic number\n", elf_get_scene_file_path(scene), name);
		return NULL;
	}

	armature = elf_create_armature(NULL);

	fread(rname, sizeof(char), 64, file);
	fread((char*)&armature->frame_count, sizeof(int), 1, file);
	fread((char*)&armature->bone_count, sizeof(int), 1, file);

	armature->name = elf_create_string(rname);
	armature->file_path = elf_create_string(elf_get_scene_file_path(scene));

	// lol...
	armature->bb_min.x = armature->bb_min.y = armature->bb_min.z = 999999.0;
	armature->bb_max.x = armature->bb_max.y = armature->bb_max.z = -999999.0;

	if(armature->bone_count > 0)
	{
		bones = elf_create_list();
		bone_parents = elf_create_list();

		for(i = 0; i < (int)armature->bone_count; i++)
		{
			bone = elf_create_bone(NULL);

			fread(rname, sizeof(char), 64, file);
			fread(parent, sizeof(char), 64, file);
			fread((char*)&bone->id, sizeof(int), 1, file);
			fread((char*)&bone->pos.x, sizeof(float), 3, file);
			fread((char*)&bone->qua.x, sizeof(float), 4, file);

			gfx_qua_get_inverse(&bone->qua.x, bone_inv_qua);

			bone->name = elf_create_string(rname);

			if(armature->frame_count > 0)
			{
				bone->frames = (elf_bone_frame*)malloc(sizeof(elf_bone_frame)*armature->frame_count);
				for(j = 0; j < armature->frame_count; j++)
				{
					fread((char*)&bone->frames[j].pos.x, sizeof(float), 3, file);
					fread((char*)&bone->frames[j].qua.x, sizeof(float), 4, file);
					gfx_mul_qua_qua(&bone->frames[j].qua.x, bone_inv_qua, &bone->frames[j].offset_qua.x);
					bone->frames[j].offset_pos.x = bone->frames[j].pos.x-bone->pos.x;
					bone->frames[j].offset_pos.y = bone->frames[j].pos.y-bone->pos.y;
					bone->frames[j].offset_pos.z = bone->frames[j].pos.z-bone->pos.z;
					if(bone->frames[j].pos.x < armature->bb_min.x) armature->bb_min.x = bone->frames[j].pos.x;
					if(bone->frames[j].pos.y < armature->bb_min.y) armature->bb_min.y = bone->frames[j].pos.y;
					if(bone->frames[j].pos.z < armature->bb_min.z) armature->bb_min.z = bone->frames[j].pos.z;
					if(bone->frames[j].pos.x > armature->bb_max.x) armature->bb_max.x = bone->frames[j].pos.x;
					if(bone->frames[j].pos.y > armature->bb_max.y) armature->bb_max.y = bone->frames[j].pos.y;
					if(bone->frames[j].pos.z > armature->bb_max.z) armature->bb_max.z = bone->frames[j].pos.z;
				}
			}

			str_obj = elf_create_string_object();
			if(strlen(parent) > 0) str_obj->str = elf_create_string(parent);
			elf_append_to_list(bone_parents, (elf_object*)str_obj);
			elf_append_to_list(bones, (elf_object*)bone);
		}

		for(bone = (elf_bone*)elf_begin_list(bones), str_obj = (elf_string*)elf_begin_list(bone_parents); bone && str_obj;
			bone = (elf_bone*)elf_next_in_list(bones), str_obj = (elf_string*)elf_next_in_list(bone_parents))
		{
			if(str_obj->str)
			{
				parent_bone = (elf_bone*)elf_get_resource_by_name(bones, str_obj->str);
				if(parent_bone)
				{
					bone->parent = parent_bone;
					elf_append_to_list(bone->parent->children, (elf_object*)bone);
				}
				elf_seek_list(bones, (elf_object*)bone);
			}
		}

		for(bone = (elf_bone*)elf_begin_list(bones); bone;
			bone = (elf_bone*)elf_next_in_list(bones))
		{
			if(!bone->parent)
			{
				elf_add_root_bone_to_armature(armature, bone);
			}
		}

		elf_destroy_list(bone_parents);
		elf_destroy_list(bones);
	}

	return armature;
}

elf_camera* elf_create_camera_from_pak(FILE *file, const char *name, elf_scene *scene)
{
	elf_camera *camera;
	int magic;
	float fov = 0.0;
	float clip_near = 0.0;
	float clip_far = 0.0;

	fread((char*)&magic, sizeof(int), 1, file);

	if(magic != 179532111)
	{
		elf_set_error(ELF_INVALID_FILE, "error: invalid camera \"%s//%s\", wrong magic number\n", elf_get_scene_file_path(scene), name);
		return NULL;
	}

	camera = elf_create_camera(NULL);
	elf_read_actor_header((elf_actor*)camera, file, scene);

	fread((char*)&fov, sizeof(float), 1, file);
	fread((char*)&clip_near, sizeof(float), 1, file);
	fread((char*)&clip_far, sizeof(float), 1, file);

	elf_set_camera_perspective(camera, fov, -1.0, clip_near, clip_far);

	return camera;
}

elf_entity* elf_create_entity_from_pak(FILE *file, const char *name, elf_scene *scene)
{
	elf_entity *entity;
	elf_model *rmodel;
	elf_armature *rarmature;
	elf_material *rmaterial;
	unsigned int material_count;
	int i, j;
	int magic = 0;
	float scale[3] = {0.0, 0.0, 0.0};
	char model[64];
	char armature[64];
	char material[64];
	unsigned char shape;
	float mass, lin_damp, ang_damp;

	fread((char*)&magic, sizeof(int), 1, file);

	if(magic != 179532112)
	{
		elf_set_error(ELF_INVALID_FILE, "error: invalid entity \"%s//%s\", wrong magic number\n", elf_get_scene_file_path(scene), name);
		return NULL;
	}

	entity = elf_create_entity(NULL);
	elf_read_actor_header((elf_actor*)entity, file, scene);

	fread((char*)scale, sizeof(float), 3, file);

	fread(model, sizeof(char), 64, file);
	if(strlen(model))
	{
		rmodel = elf_get_or_load_model_by_name(scene, model);
		elf_set_entity_model(entity, rmodel);
	}

	fread(armature, sizeof(char), 64, file);
	if(strlen(armature))
	{
		rarmature = elf_get_or_load_armature_by_name(scene, armature);
		if(rarmature) elf_set_entity_armature(entity, rarmature);
	}

	// scale must be set after setting a model, setting a model resets the scale
	elf_set_entity_scale(entity, scale[0], scale[1], scale[2]);

	fread((char*)&shape, sizeof(unsigned char), 1, file);
	fread((char*)&mass, sizeof(float), 1, file);
	fread((char*)&lin_damp, sizeof(float), 1, file);
	fread((char*)&ang_damp, sizeof(float), 1, file);

	if(shape == ELF_BOX || shape == ELF_SPHERE || shape == ELF_MESH || shape == ELF_CAPSULE)
	{
		elf_set_actor_physics((elf_actor*)entity, shape, mass);
		elf_set_actor_damping((elf_actor*)entity, lin_damp, ang_damp);
	}

	fread((char*)&material_count, sizeof(unsigned int), 1, file);
	for(i = 0, j = 0; i < (int)material_count; i++)
	{
		memset(material, 0x0, sizeof(char)*64);
		fread(material, sizeof(char), 64, file);

		rmaterial = NULL;
		if(strlen(material)) rmaterial = elf_get_or_load_material_by_name(scene, material);
		if(rmaterial)
		{
			if(elf_get_entity_material_count(entity) > j)
				elf_set_entity_material(entity, j, rmaterial);
			else elf_add_entity_material(entity, rmaterial);
			j++;
		}
	}

	// check if normal/displacement maps are needed
	elf_generate_entity_tangent_vectors(entity);

	return entity;
}

elf_light* elf_create_light_from_pak(FILE *file, const char *name, elf_scene *scene)
{
	elf_light *light;
	int magic = 0;
	unsigned int junk;

	fread((char*)&magic, sizeof(int), 1, file);

	if(magic != 179532113)
	{
		elf_set_error(ELF_INVALID_FILE, "error: invalid light \"%s//%s\", wrong magic number\n", elf_get_scene_file_path(scene), name);
		return NULL;
	}

	light = elf_create_light(NULL);
	elf_read_actor_header((elf_actor*)light, file, scene);

	fread((char*)&light->light_type, sizeof(unsigned char), 1, file);
	if(light->light_type == ELF_SPOT_LIGHT) elf_set_light_shadow_caster(light, ELF_TRUE);
	fread((char*)&light->color.r, sizeof(float), 4, file);
	fread((char*)&light->distance, sizeof(float), 1, file);
	fread((char*)&light->fade_speed, sizeof(float), 1, file);
	fread((char*)&light->inner_cone, sizeof(float), 1, file);
	fread((char*)&light->outer_cone, sizeof(float), 1, file);
	fread((char*)&junk, sizeof(unsigned int), 1, file);
	fread((char*)&light->shadow_caster, sizeof(unsigned char), 1, file);

	elf_set_light_type(light, light->light_type);
	elf_set_light_color(light, light->color.r, light->color.g, light->color.b, light->color.a);
	elf_set_light_distance(light, light->distance);
	elf_set_light_fade_speed(light, light->fade_speed);
	elf_set_light_shadow_caster(light, light->shadow_caster);
	elf_set_light_cone(light, light->inner_cone, light->outer_cone);

	return light;
}

elf_material* elf_create_material_from_pak(FILE *file, const char *name, elf_scene *scene)
{
	elf_material *material;
	elf_texture *rtexture;
	int magic;
	char rname[64];
	unsigned char texture_count = 0;
	char texture[64];
	unsigned char texture_type;
	float parallax_scale;
	int i;

	fread((char*)&magic, sizeof(int), 1, file);

	if(magic != 179532109)
	{
		elf_set_error(ELF_INVALID_FILE, "error: invalid material \"%s//%s\", wrong magic number\n", elf_get_scene_file_path(scene), name);
		return NULL;
	}

	memset(rname, 0x0, sizeof(char)*64);
	fread(rname, sizeof(char), 64, file);

	material = elf_create_material(NULL);

	material->name = elf_create_string(rname);
	material->file_path = elf_create_string(elf_get_scene_file_path(scene));

	fread((char*)&material->diffuse_color.r, sizeof(float), 4, file);
	fread((char*)&material->ambient_color.r, sizeof(float), 4, file);
	fread((char*)&material->specular_color.r, sizeof(float), 4, file);
	fread((char*)&material->spec_power, sizeof(float), 1, file);

	elf_set_material_diffuse_color(material, material->diffuse_color.r, material->diffuse_color.g, material->diffuse_color.b, material->diffuse_color.a);
	elf_set_material_specular_color(material, material->specular_color.r, material->specular_color.g, material->specular_color.b, material->specular_color.a);
	elf_set_material_ambient_color(material, material->ambient_color.r, material->ambient_color.g, material->ambient_color.b, material->ambient_color.a);
	elf_set_material_specular_power(material, material->spec_power);

	fread((char*)&texture_count, sizeof(unsigned char), 1, file);

	for(i = 0; i < texture_count; i++)
	{
		if(i > GFX_MAX_TEXTURES-1) break;

		memset(texture, 0x0, sizeof(char)*64);
		fread(texture, sizeof(char), 64, file);
		fread((char*)&texture_type, sizeof(unsigned char), 1, file);
		fread((char*)&parallax_scale, sizeof(float), 1, file);

		rtexture = elf_get_or_load_texture_by_name(scene, texture);
		if(rtexture)
		{
			elf_set_material_texture(material, i, rtexture);
			elf_set_material_texture_type(material, i, texture_type);
			elf_set_material_texture_parallax_scale(material, i, parallax_scale);
		}
	}

	return material;
}

elf_model* elf_create_model_from_pak(FILE *file, const char *name, elf_scene *scene)
{
	elf_model *model = NULL;
	int magic = 0;
	unsigned int i = 0;
	char rname[64];
	unsigned int indices_read = 0;
	unsigned char is_normals;
	unsigned char is_tex_coords;
	unsigned char is_weights_and_boneids;
	unsigned char junk;
	float weights[4];
	float length;
	short int boneids[4];
	float *vertex_buffer;

	// read magic
	fread((char*)&magic, sizeof(int), 1, file);

	if(magic != 179532110)
	{
		elf_set_error(ELF_INVALID_FILE, "error: invalid model \"%s\", wrong magic number\n", name);
		return NULL;
	}

	model = (elf_model*)malloc(sizeof(elf_model));
	memset(model, 0x0, sizeof(elf_model));
	model->type = ELF_MODEL;

	model->id = ++model_id_counter;

	global_obj_count++;

	// read name
	fread(rname, sizeof(char), 64, file);

	model->name = elf_create_string(rname);
	model->file_path = elf_create_string(elf_get_scene_file_path(scene));

	// read header
	fread((char*)&model->vertice_count, sizeof(int), 1, file);
	fread((char*)&model->frame_count, sizeof(int), 1, file);
	fread((char*)&model->indice_count, sizeof(int), 1, file);
	fread((char*)&model->area_count, sizeof(int), 1, file);
	fread((char*)&is_normals, sizeof(unsigned char), 1, file);
	fread((char*)&is_tex_coords, sizeof(unsigned char), 1, file);
	fread((char*)&is_weights_and_boneids, sizeof(unsigned char), 1, file);
	fread((char*)&junk, sizeof(unsigned char), 1, file);

	if(model->vertice_count < 3)
	{
		elf_set_error(ELF_INVALID_FILE, "error: invalid model \"%s\", invalid vertex count\n", name);
		elf_destroy_model(model);
		return NULL;
	}
	if(model->frame_count < 1)
	{
		elf_set_error(ELF_INVALID_FILE, "error: invalid model \"%s\", invalid frame count\n", name);
		elf_destroy_model(model);
		return NULL;
	}
	if(model->indice_count < 3)
	{
		elf_set_error(ELF_INVALID_FILE, "error: invalid model \"%s\", invalid indice count\n", name);
		elf_destroy_model(model);
		return NULL;
	}
	if(model->area_count < 1)
	{
		elf_set_error(ELF_INVALID_FILE, "error: invalid model \"%s\", invalid area count\n", name);
		elf_destroy_model(model);
		return NULL;
	}

	// read vertices
	model->vertices = gfx_create_vertex_data(3*model->vertice_count, GFX_FLOAT, GFX_VERTEX_DATA_STATIC);
	gfx_inc_ref((gfx_object*)model->vertices);

	fread((char*)gfx_get_vertex_data_buffer(model->vertices), sizeof(float), 3*model->vertice_count, file);

	// read index
	model->index = (unsigned int*)malloc(sizeof(unsigned int)*model->indice_count);

	model->areas = (elf_model_area*)malloc(sizeof(elf_model_area)*model->area_count);
	memset(model->areas, 0x0, sizeof(elf_model_area)*model->area_count);

	for(i = 0; i < model->area_count; i++)
	{
		fread((char*)&model->areas[i].indice_count, sizeof(unsigned int), 1, file);
		if(model->areas[i].indice_count)
		{
			model->areas[i].index = gfx_create_vertex_data(model->areas[i].indice_count, GFX_UINT, GFX_VERTEX_DATA_STATIC);
			gfx_inc_ref((gfx_object*)model->areas[i].index);

			fread((char*)gfx_get_vertex_data_buffer(model->areas[i].index),
				sizeof(unsigned int), model->areas[i].indice_count, file);

			memcpy(&model->index[indices_read], gfx_get_vertex_data_buffer(model->areas[i].index),
				gfx_get_vertex_data_size_bytes(model->areas[i].index));

			indices_read += model->areas[i].indice_count;
		}
	}

	model->normals = gfx_create_vertex_data(3*model->vertice_count, GFX_FLOAT, GFX_VERTEX_DATA_STATIC);
	gfx_inc_ref((gfx_object*)model->normals);

	fread((char*)gfx_get_vertex_data_buffer(model->normals), sizeof(float), 3*model->vertice_count, file);

	// read tex coords
	if(is_tex_coords > 0)
	{
		model->tex_coords = gfx_create_vertex_data(2*model->vertice_count, GFX_FLOAT, GFX_VERTEX_DATA_STATIC);
		gfx_inc_ref((gfx_object*)model->tex_coords);

		fread((char*)gfx_get_vertex_data_buffer(model->tex_coords), sizeof(float), 2*model->vertice_count, file);
	}

	// read weights and bone ids
	if(is_weights_and_boneids > 0)
	{
		model->weights = (float*)malloc(sizeof(float)*4*model->vertice_count);
		for(i = 0; i < model->vertice_count; i++)
		{
			fread((char*)weights, sizeof(float), 4, file);
			if(weights[0] > 1.0) weights[0] = 1.0;
			if(weights[1] > 1.0) weights[1] = 1.0;
			if(weights[2] > 1.0) weights[2] = 1.0;
			if(weights[3] > 1.0) weights[3] = 1.0;
			if(weights[0] < 0.0) weights[0] = 0.0;
			if(weights[1] < 0.0) weights[1] = 0.0;
			if(weights[2] < 0.0) weights[2] = 0.0;
			if(weights[3] < 0.0) weights[3] = 0.0;
			length = 1.0f/(weights[0]+weights[1]+weights[2]+weights[3]);
			model->weights[i*4] = weights[0]*length;
			model->weights[i*4+1] = weights[1]*length;
			model->weights[i*4+2] = weights[2]*length;
			model->weights[i*4+3] = weights[3]*length;
		}
		model->boneids = (int*)malloc(sizeof(int)*4*model->vertice_count);
		for(i = 0; i < model->vertice_count; i++)
		{
			fread((char*)boneids, sizeof(short int), 4, file);
			model->boneids[i*4] = boneids[0];
			model->boneids[i*4+1] = boneids[1];
			model->boneids[i*4+2] = boneids[2];
			model->boneids[i*4+3] = boneids[3];
		}
	}

	vertex_buffer = gfx_get_vertex_data_buffer(model->vertices);

	// get bounding box values
	memcpy(&model->bb_min.x, vertex_buffer, sizeof(float)*3);
	memcpy(&model->bb_max.x, vertex_buffer, sizeof(float)*3);

	for(i = 3; i < model->vertice_count*3; i+=3)
	{
		if(vertex_buffer[i] < model->bb_min.x) model->bb_min.x = vertex_buffer[i];
		if(vertex_buffer[i+1] < model->bb_min.y) model->bb_min.y = vertex_buffer[i+1];
		if(vertex_buffer[i+2] < model->bb_min.z) model->bb_min.z = vertex_buffer[i+2];

		if(vertex_buffer[i] > model->bb_max.x) model->bb_max.x = vertex_buffer[i];
		if(vertex_buffer[i+1] > model->bb_max.y) model->bb_max.y = vertex_buffer[i+1];
		if(vertex_buffer[i+2] > model->bb_max.z) model->bb_max.z = vertex_buffer[i+2];
	}

	model->vertex_array = gfx_create_vertex_array(GFX_TRUE);
	gfx_inc_ref((gfx_object*)model->vertex_array);

	gfx_set_vertex_array_data(model->vertex_array, GFX_VERTEX, model->vertices);
	gfx_set_vertex_array_data(model->vertex_array, GFX_NORMAL, model->normals);
	if(is_tex_coords > 0) gfx_set_vertex_array_data(model->vertex_array, GFX_TEX_COORD, model->tex_coords);

	for(i = 0; i < model->area_count; i++)
	{
		if(model->areas[i].indice_count > 0)
		{
			model->areas[i].vertex_index = gfx_create_vertex_index(GFX_TRUE, model->areas[i].index);
			gfx_inc_ref((gfx_object*)model->areas[i].vertex_index);
		}
	}

	return model;
}

elf_script* elf_create_script_from_pak(FILE *file, const char *name, elf_scene *scene)
{
	elf_script *script;
	int magic = 0;
	char rname[64];
	unsigned int length;
	char *text;

	fread((char*)&magic, sizeof(int), 1, file);

	if(magic != 179532121)
	{
		elf_set_error(ELF_INVALID_FILE, "error: invalid script \"%s\", wrong magic number\n", name);
		return NULL;
	}

	memset(rname, 0x0, sizeof(char)*64);
	fread(rname, sizeof(char), 64, file);

	script = elf_create_script();

	script->name = elf_create_string(name);
	script->file_path = elf_create_string(elf_get_scene_file_path(scene));

	fread((char*)&length, sizeof(unsigned int), 1, file);
	if(length > 0)
	{
		text = (char*)malloc(sizeof(char)*length+1);
		memset(text, 0x0, sizeof(char)*length+1);
		fread(text, sizeof(char), length, file);
		text[length] = '\0';
		elf_set_script_text(script, text);
		free(text);
	}

	return script;
}

elf_texture *elf_create_texture_from_pak(FILE *file, const char *name, elf_scene *scene)
{
	elf_texture *texture;
	FIMEMORY *fi_mem;
	FIBITMAP *fi_bitmap;
	char *mem;
	FREE_IMAGE_FORMAT fi_format;
	int magic;
	char rname[64];
	unsigned char type;
	int width;
	int height;
	unsigned char bpp;
	unsigned int length;
	int format;
	int internal_format;
	int data_format;
	unsigned char *data;

	fread((char*)&magic, sizeof(int), 1, file);

	if(magic != 179532108)
	{
		elf_set_error(ELF_INVALID_FILE, "error: invalid texture \"%s//%s\", wrong magic number\n", elf_get_scene_file_path(scene), name);
		return NULL;
	}

	fread(rname, sizeof(char), 64, file);
	fread((char*)&type, sizeof(unsigned char), 1, file);

	if(type == 0)
	{
		fread((char*)&width, sizeof(int), 1, file);
		fread((char*)&height, sizeof(int), 1, file);
		fread((char*)&bpp, sizeof(unsigned char), 1, file);
		data = (unsigned char*)malloc(sizeof(unsigned char)*width*height*bpp);
		fread(data, sizeof(unsigned char), width*height*bpp, file);
	}
	else if(type == 1)
	{
		// mbg: sizeof(unsigned int) fixes a bug on 64-bit platforms
		fread((char*)&length, sizeof(unsigned int), 1, file);
 
		mem = (char*)malloc(sizeof(char)*length);
		fread(mem, sizeof(char), length, file);

		fi_mem = FreeImage_OpenMemory((BYTE*)mem, length);
		fi_format = FreeImage_GetFileTypeFromMemory(fi_mem, 0);
		fi_bitmap = FreeImage_LoadFromMemory(fi_format, fi_mem, 0);

		width = FreeImage_GetWidth(fi_bitmap);
		height = FreeImage_GetHeight(fi_bitmap);
		bpp = FreeImage_GetBPP(fi_bitmap);

		data = (unsigned char*)malloc(sizeof(char)*width*height*(bpp/8));
		FreeImage_ConvertToRawBits((BYTE*)data, fi_bitmap, width*(bpp/8), bpp,
			FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, FALSE);

		FreeImage_Unload(fi_bitmap);
		FreeImage_CloseMemory(fi_mem);

		free(mem);
	}
	else
	{
		elf_set_error(ELF_UNKNOWN_FORMAT, "error: can't load texture \"%s//%s\", unknown format\n", elf_get_scene_file_path(scene), rname);
		return NULL;
	}

	switch(bpp)
	{
		case 8: format = GFX_LUMINANCE; internal_format = GFX_LUMINANCE; data_format = GFX_UBYTE; break;
		case 16: format = GFX_LUMINANCE_ALPHA; internal_format = GFX_LUMINANCE_ALPHA; data_format = GFX_UBYTE; break;
		case 24: format = GFX_BGR; internal_format = GFX_COMPRESSED_RGB; data_format = GFX_UBYTE; break;
		case 32: format = GFX_BGRA; internal_format = GFX_COMPRESSED_RGBA; data_format = GFX_UBYTE; break;
		case 48: format = GFX_BGR; internal_format = GFX_COMPRESSED_RGB; data_format = GFX_USHORT; break;
		default:
			elf_set_error(ELF_INVALID_FILE, "error: unsupported bits per pixel value [%d] in texture \"%s//%s\"\n", (int)bpp, elf_get_scene_file_path(scene), rname);
			free(data);
			return NULL;
	}

	texture = elf_create_texture();

	texture->name = elf_create_string(rname);
	texture->file_path = elf_create_string(elf_get_scene_file_path(scene));
	texture->texture = gfx_create_2d_texture(width, height, eng->texture_anisotropy, GFX_REPEAT, GFX_LINEAR, format, internal_format, data_format, data);

	free(data);

	if(!texture->texture)
	{
		elf_set_error(ELF_CANT_CREATE, "error: can't create texture \"%s//%s\"\n", elf_get_scene_file_path(scene), rname);
		elf_destroy_texture(texture);
		return NULL;
	}

	return texture;
}

unsigned char elf_save_scene_to_pak(elf_scene *scene, const char *file_path)
{
	int i;
	FILE *file;

	elf_list *scenes;
	elf_list *scripts;
	elf_list *textures;
	elf_list *materials;
	elf_list *models;
	elf_list *cameras;
	elf_list *entities;
	elf_list *lights;
	elf_list *armatures;
	elf_list *particles;
	elf_list *sprites;

	elf_material *mat;
	elf_camera *cam;
	elf_entity *ent;
	elf_light *lig;
	elf_particles *par;
	elf_sprite *spr;

	scenes = elf_create_list();
	scripts = elf_create_list();
	textures = elf_create_list();
	materials = elf_create_list();
	models = elf_create_list();
	cameras = elf_create_list();
	entities = elf_create_list();
	lights = elf_create_list();
	armatures = elf_create_list();
	particles = elf_create_list();
	sprites = elf_create_list();

	elf_inc_ref((elf_object*)scenes);
	elf_inc_ref((elf_object*)scripts);
	elf_inc_ref((elf_object*)textures);
	elf_inc_ref((elf_object*)materials);
	elf_inc_ref((elf_object*)models);
	elf_inc_ref((elf_object*)cameras);
	elf_inc_ref((elf_object*)entities);
	elf_inc_ref((elf_object*)lights);
	elf_inc_ref((elf_object*)armatures);
	elf_inc_ref((elf_object*)particles);
	elf_inc_ref((elf_object*)sprites);

	elf_append_to_list(scenes, (elf_object*)scene);

	for(cam = (elf_camera*)elf_begin_list(scene->cameras); cam;
		cam = (elf_camera*)elf_next_in_list(scene->cameras))
	{
		if(cam->script && !elf_get_resource_by_id(scripts, cam->script->id))
		{
			elf_set_unique_name_for_resource(scripts, (elf_resource*)cam->script);
			elf_append_to_list(scripts, (elf_object*)cam->script);
		}

		elf_set_unique_name_for_resource(cameras, (elf_resource*)cam);
		elf_append_to_list(cameras, (elf_object*)cam);
	}

	for(ent = (elf_entity*)elf_begin_list(scene->entities); ent;
		ent = (elf_entity*)elf_next_in_list(scene->entities))
	{
		if(ent->script && !elf_get_resource_by_id(scripts, ent->script->id))
		{
			elf_set_unique_name_for_resource(scripts, (elf_resource*)ent->script);
			elf_append_to_list(scripts, (elf_object*)ent->script);
		}

		if(ent->model && !elf_get_resource_by_id(models, ent->model->id))
		{
			elf_set_unique_name_for_resource(models, (elf_resource*)ent->model);
			elf_append_to_list(models, (elf_object*)ent->model);
		}

		if(ent->armature && !elf_get_resource_by_id(armatures, ent->armature->id))
		{
			elf_set_unique_name_for_resource(armatures, (elf_resource*)ent->armature);
			elf_append_to_list(armatures, (elf_object*)ent->armature);
		}

		for(mat = (elf_material*)elf_begin_list(ent->materials); mat;
			mat = (elf_material*)elf_next_in_list(ent->materials))
		{
			if(!elf_get_resource_by_id(materials, mat->id))
			{
				elf_set_unique_name_for_resource(materials, (elf_resource*)mat);
				elf_append_to_list(materials, (elf_object*)mat);

				for(i = 0; i < GFX_MAX_TEXTURES; i++)
				{
					if(mat->textures[i] && !elf_get_resource_by_id(textures, mat->textures[i]->id))
					{
						elf_set_unique_name_for_resource(textures, (elf_resource*)mat->textures[i]);
						elf_append_to_list(textures, (elf_object*)mat->textures[i]);
					}
				}
			}
		}

		elf_set_unique_name_for_resource(entities, (elf_resource*)ent);
		elf_append_to_list(entities, (elf_object*)ent);
	}

	for(lig = (elf_light*)elf_begin_list(scene->lights); lig;
		lig = (elf_light*)elf_next_in_list(scene->lights))
	{
		if(lig->script && !elf_get_resource_by_id(scripts, lig->script->id))
		{
			elf_set_unique_name_for_resource(scripts, (elf_resource*)lig->script);
			elf_append_to_list(scripts, (elf_object*)lig->script);
		}

		elf_set_unique_name_for_resource(lights, (elf_resource*)lig);
		elf_append_to_list(lights, (elf_object*)lig);
	}

	for(par = (elf_particles*)elf_begin_list(scene->particles); par;
		par = (elf_particles*)elf_next_in_list(scene->particles))
	{
		if(par->script && !elf_get_resource_by_id(scripts, par->script->id))
		{
			elf_set_unique_name_for_resource(scripts, (elf_resource*)par->script);
			elf_append_to_list(scripts, (elf_object*)par->script);
		}

		elf_set_unique_name_for_resource(particles, (elf_resource*)par);
		elf_append_to_list(particles, (elf_object*)par);
	}

	for(spr = (elf_sprite*)elf_begin_list(scene->sprites); spr;
		spr = (elf_sprite*)elf_next_in_list(scene->sprites))
	{
		if(spr->script && !elf_get_resource_by_id(scripts, spr->script->id))
		{
			elf_set_unique_name_for_resource(scripts, (elf_resource*)spr->script);
			elf_append_to_list(scripts, (elf_object*)spr->script);
		}

		elf_set_unique_name_for_resource(sprites, (elf_resource*)spr);
		elf_append_to_list(sprites, (elf_object*)spr);
	}

	file = fopen(file_path, "wb");
	if(!file)
	{
		elf_set_error(ELF_CANT_OPEN_FILE, "error: can't open file \"%s\" for writing", file_path);

		elf_dec_ref((elf_object*)scenes);
		elf_dec_ref((elf_object*)scripts);
		elf_dec_ref((elf_object*)textures);
		elf_dec_ref((elf_object*)materials);
		elf_dec_ref((elf_object*)models);
		elf_dec_ref((elf_object*)cameras);
		elf_dec_ref((elf_object*)entities);
		elf_dec_ref((elf_object*)lights);
		elf_dec_ref((elf_object*)armatures);
		elf_dec_ref((elf_object*)particles);
		elf_dec_ref((elf_object*)sprites);

		return ELF_FALSE;
	}



	fclose(file);

	elf_dec_ref((elf_object*)scenes);
	elf_dec_ref((elf_object*)scripts);
	elf_dec_ref((elf_object*)textures);
	elf_dec_ref((elf_object*)materials);
	elf_dec_ref((elf_object*)models);
	elf_dec_ref((elf_object*)cameras);
	elf_dec_ref((elf_object*)entities);
	elf_dec_ref((elf_object*)lights);
	elf_dec_ref((elf_object*)armatures);
	elf_dec_ref((elf_object*)particles);
	elf_dec_ref((elf_object*)sprites);

	return ELF_TRUE;
}

