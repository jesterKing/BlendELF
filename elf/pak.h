
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
	int offset;

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
	fread((char*)&index_count, sizeof(int), 1, file);

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
		fread((char*)&offset, sizeof(int), 1, file);

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

int elf_get_actor_header_size_bytes(elf_actor *actor)
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

	for(curve = (elf_bezier_curve*)elf_begin_list(actor->ipo->curves); curve;
		curve = (elf_bezier_curve*)elf_next_in_list(actor->ipo->curves))
	{
		size_bytes += sizeof(unsigned char);	// type
		size_bytes += sizeof(unsigned char);	// interpolation
		size_bytes += sizeof(int);	// point count
		size_bytes += sizeof(float)*6*elf_get_list_length(curve->points);	// points
	}

	size_bytes += sizeof(float)*3;	// bounding lengths
	size_bytes += sizeof(float)*3;	// bounding offset
	size_bytes += sizeof(unsigned char);	// shape
	size_bytes += sizeof(float);	// mass
	size_bytes += sizeof(float);	// linear damp
	size_bytes += sizeof(float);	// angular damp
	size_bytes += sizeof(float);	// linear sleep threshold
	size_bytes += sizeof(float);	// angular sleep threshold
	size_bytes += sizeof(float);	// restitution
	size_bytes += sizeof(float)*3;	// anisotropic friction
	size_bytes += sizeof(float)*3;	// linear factor
	size_bytes += sizeof(float)*3;	// angular factor

	return size_bytes;
}

int elf_get_armature_size_bytes(elf_armature *armature)
{
	int size_bytes;
	int i;

	size_bytes = 0;

	size_bytes += sizeof(int);	// magic
	size_bytes += sizeof(int);	// frame count
	size_bytes += sizeof(int);	// bone count

	for(i = 0; i < (int)armature->bone_count; i++)
	{
		size_bytes += sizeof(char)*64;	// name
		size_bytes += sizeof(char)*64;	// parent
		size_bytes += sizeof(int);	// id
		size_bytes += sizeof(float)*3;	// position
		size_bytes += sizeof(float)*4;	// quaternion

		size_bytes += sizeof(float)*3*armature->frame_count;	// positions
		size_bytes += sizeof(float)*4*armature->frame_count;	// quaternions
	}

	return size_bytes;
}

int elf_get_camera_size_bytes(elf_camera *camera)
{
	int size_bytes;

	size_bytes = 0;

	size_bytes += sizeof(int);	// magic
	size_bytes += elf_get_actor_header_size_bytes((elf_actor*)camera);	// actor header
	size_bytes += sizeof(float);	// fov
	size_bytes += sizeof(float);	// clip near
	size_bytes += sizeof(float);	// clip far

	return size_bytes;
}

int elf_get_entity_size_bytes(elf_entity *entity)
{
	int size_bytes;

	size_bytes = 0;

	size_bytes += sizeof(int);	// magic
	size_bytes += elf_get_actor_header_size_bytes((elf_actor*)entity);	// actor header
	size_bytes += sizeof(float)*3;	// scale
	size_bytes += sizeof(char)*64;	// model
	size_bytes += sizeof(char)*64;	// armature
	size_bytes += sizeof(int);	// material count
	size_bytes += sizeof(char)*64*elf_get_entity_material_count(entity);	// materials

	return size_bytes;
}

int elf_get_light_size_bytes(elf_light *light)
{
	int size_bytes;

	size_bytes = 0;

	size_bytes += sizeof(int);	// magic
	size_bytes += elf_get_actor_header_size_bytes((elf_actor*)light);	// actor header
	size_bytes += sizeof(unsigned char);	// type
	size_bytes += sizeof(float)*4;	// color
	size_bytes += sizeof(float);	// distance
	size_bytes += sizeof(float);	// fade speed
	size_bytes += sizeof(float);	// inner cone
	size_bytes += sizeof(float);	// outer cone
	size_bytes += sizeof(unsigned int);	// junk???
	size_bytes += sizeof(unsigned char);	// shadow caster

	return size_bytes;
}

int elf_get_material_size_bytes(elf_material *material)
{
	int size_bytes;

	size_bytes = 0;

	size_bytes += sizeof(int);	// magic
	size_bytes += sizeof(char)*64;	// name
	size_bytes += sizeof(float)*4;	// diffuse color
	size_bytes += sizeof(float)*4;	// ambient color
	size_bytes += sizeof(float)*4;	// specular color
	size_bytes += sizeof(float);	// specular power
	size_bytes += sizeof(unsigned char);	// texture count

	size_bytes += sizeof(char)*64*elf_get_material_texture_count(material);	// texture name
	size_bytes += sizeof(unsigned char)*elf_get_material_texture_count(material);	// texture type
	size_bytes += sizeof(float)*elf_get_material_texture_count(material);	// texture parallax scale

	return size_bytes;
}

int elf_get_model_size_bytes(elf_model *model)
{
	int size_bytes;

	size_bytes = 0;

	size_bytes += sizeof(int);	// magic
	size_bytes += sizeof(char)*64;	// name
	size_bytes += sizeof(int);	// vertice count
	size_bytes += sizeof(int);	// frame count
	size_bytes += sizeof(int);	// indice count
	size_bytes += sizeof(int);	// area count
	size_bytes += sizeof(unsigned char);	// normals
	size_bytes += sizeof(unsigned char);	// tex coords
	size_bytes += sizeof(unsigned char);	// weights & boneids
	size_bytes += sizeof(unsigned char);	// junk

	size_bytes += sizeof(float)*3*model->vertice_count;	// vertices

	size_bytes += sizeof(unsigned int)*model->area_count;	// area indice counts
	size_bytes += sizeof(unsigned int)*model->indice_count;	// indices

	size_bytes += sizeof(float)*3*model->vertice_count;	// normals
	if(model->tex_coords) size_bytes += sizeof(float)*2*model->vertice_count;	// texcoords

	if(model->weights && model->boneids)
	{
		size_bytes += sizeof(float)*4*model->vertice_count;	// weights
		size_bytes += sizeof(short int)*4*model->vertice_count;	// boneids
	}

	return size_bytes;
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

int elf_get_particles_size_bytes(elf_particles *particles)
{
	int size_bytes;

	size_bytes = 0;

	size_bytes += sizeof(int);	// magic
	size_bytes += elf_get_actor_header_size_bytes((elf_actor*)particles);
	size_bytes += sizeof(char)*64;	// texture
	size_bytes += sizeof(char)*64;	// model
	size_bytes += sizeof(char)*64;	// entity
	size_bytes += sizeof(int);	// max count
	size_bytes += sizeof(unsigned char);	// draw mode
	size_bytes += sizeof(float);	// spawn delay
	size_bytes += sizeof(unsigned char);	// spawn
	size_bytes += sizeof(float)*3;	// gravity
	size_bytes += sizeof(float);	// size min
	size_bytes += sizeof(float);	// size max
	size_bytes += sizeof(float);	// size growth mix
	size_bytes += sizeof(float);	// size growth max
	size_bytes += sizeof(float);	// rotation min
	size_bytes += sizeof(float);	// rotation max
	size_bytes += sizeof(float);	// rotation growth min
	size_bytes += sizeof(float);	// rotation growth max
	size_bytes += sizeof(float);	// life span min
	size_bytes += sizeof(float);	// life span max
	size_bytes += sizeof(float);	// fade speed min
	size_bytes += sizeof(float);	// fade speed max
	size_bytes += sizeof(float)*3;	// velocity min
	size_bytes += sizeof(float)*3;	// velocity max
	size_bytes += sizeof(float)*3;	// position min
	size_bytes += sizeof(float)*3;	// position max
	size_bytes += sizeof(float)*4;	// color min
	size_bytes += sizeof(float)*4;	// color max

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

int elf_get_sprite_size_bytes(elf_sprite *sprite)
{
	int size_bytes;

	size_bytes = 0;

	size_bytes += sizeof(int);	// magic
	size_bytes += elf_get_actor_header_size_bytes((elf_actor*)sprite);
	size_bytes += sizeof(float)*2;	// scale
	size_bytes += sizeof(char)*64;	// material
	size_bytes += sizeof(unsigned char);	// face camera

	return size_bytes;
}

int elf_get_texture_size_bytes(elf_texture *texture)
{
	int size_bytes;

	size_bytes = 0;

	size_bytes += sizeof(int);	// magic
	size_bytes += sizeof(char)*64;	// name
	size_bytes += sizeof(unsigned char);	// type
	size_bytes += sizeof(int);	// data length
	size_bytes += texture->data_size;

	return size_bytes;
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
	float bounding_lengths[3];
	float bounding_offset[3];
	unsigned char shape;
	float mass;
	float lin_damp;
	float ang_damp;
	float lin_sleep;
	float ang_sleep;
	float restitution;
	float anis_fric[3];
	float linear_factor[3];
	float angular_factor[3];

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

	fread((char*)bounding_lengths, sizeof(float), 3, file);
	fread((char*)bounding_offset, sizeof(float), 3, file);

	elf_set_actor_bounding_lengths(actor, bounding_lengths[0], bounding_lengths[1], bounding_lengths[2]);
	elf_set_actor_bounding_offset(actor, bounding_offset[0], bounding_offset[1], bounding_offset[2]);

	fread((char*)&shape, sizeof(unsigned char), 1, file);
	fread((char*)&mass, sizeof(float), 1, file);
	fread((char*)&lin_damp, sizeof(float), 1, file);
	fread((char*)&ang_damp, sizeof(float), 1, file);
	fread((char*)&lin_sleep, sizeof(float), 1, file);
	fread((char*)&ang_sleep, sizeof(float), 1, file);
	fread((char*)&restitution, sizeof(float), 1, file);
	fread((char*)anis_fric, sizeof(float), 3, file);
	fread((char*)linear_factor, sizeof(float), 3, file);
	fread((char*)angular_factor, sizeof(float), 3, file);

	if(shape == ELF_BOX || shape == ELF_SPHERE || shape == ELF_MESH || shape == ELF_CAPSULE)
	{
		elf_set_actor_physics(actor, shape, mass);
		elf_set_actor_damping(actor, lin_damp, ang_damp);
		elf_set_actor_sleep_thresholds(actor, lin_sleep, ang_sleep);
		elf_set_actor_restitution(actor, restitution);
		elf_set_actor_anisotropic_friction(actor, anis_fric[0], anis_fric[1], anis_fric[2]);
		elf_set_actor_linear_factor(actor, linear_factor[0], linear_factor[1], linear_factor[2]);
		elf_set_actor_angular_factor(actor, angular_factor[0], angular_factor[1], angular_factor[2]);
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

	if(magic != ELF_ARMATURE_MAGIC)
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

	if(magic != ELF_CAMERA_MAGIC)
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
	elf_vec3f bounding_lengths;
	elf_vec3f bounding_offset;

	fread((char*)&magic, sizeof(int), 1, file);

	if(magic != ELF_ENTITY_MAGIC)
	{
		elf_set_error(ELF_INVALID_FILE, "error: invalid entity \"%s//%s\", wrong magic number\n", elf_get_scene_file_path(scene), name);
		return NULL;
	}

	entity = elf_create_entity(NULL);
	elf_read_actor_header((elf_actor*)entity, file, scene);

	fread((char*)scale, sizeof(float), 3, file);

	bounding_lengths = elf_get_actor_bounding_lengths((elf_actor*)entity);
	bounding_offset = elf_get_actor_bounding_offset((elf_actor*)entity);

	fread(model, sizeof(char), 64, file);
	if(strlen(model))
	{
		rmodel = elf_get_or_load_model_by_name(scene, model);
		elf_set_entity_model(entity, rmodel);
	}

	if(!elf_about_zero(bounding_lengths.x) || !elf_about_zero(bounding_lengths.y) || !elf_about_zero(bounding_lengths.z))
		elf_set_actor_bounding_lengths((elf_actor*)entity, bounding_lengths.x, bounding_lengths.y, bounding_lengths.y);
	if(!elf_about_zero(bounding_offset.x) || !elf_about_zero(bounding_offset.y) || !elf_about_zero(bounding_offset.z))
		elf_set_actor_bounding_offset((elf_actor*)entity, bounding_offset.x, bounding_offset.y, bounding_offset.y);

	fread(armature, sizeof(char), 64, file);
	if(strlen(armature))
	{
		rarmature = elf_get_or_load_armature_by_name(scene, armature);
		if(rarmature) elf_set_entity_armature(entity, rarmature);
	}

	// scale must be set after setting a model, setting a model resets the scale
	elf_set_entity_scale(entity, scale[0], scale[1], scale[2]);

	fread((char*)&material_count, sizeof(int), 1, file);

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

	if(magic != ELF_LIGHT_MAGIC)
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

	if(magic != ELF_MATERIAL_MAGIC)
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

	if(magic != ELF_MODEL_MAGIC)
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
		fread((char*)&model->areas[i].indice_count, sizeof(int), 1, file);
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

elf_particles* elf_create_particles_from_pak(FILE *file, const char *name, elf_scene *scene)
{
	elf_particles *particles;
	elf_texture *rtexture;
	elf_model *rmodel;
	elf_entity *rentity;
	int magic = 0;
	char texture[64];
	char model[64];
	char entity[64];

	fread((char*)&magic, sizeof(int), 1, file);

	if(magic != ELF_PARTICLES_MAGIC)
	{
		elf_set_error(ELF_INVALID_FILE, "error: invalid particles \"%s//%s\", wrong magic number\n", elf_get_scene_file_path(scene), name);
		return NULL;
	}

	particles = elf_create_particles(NULL, 10);
	elf_read_actor_header((elf_actor*)particles, file, scene);

	fread(texture, sizeof(char), 64, file);
	if(strlen(texture))
	{
		rtexture = elf_get_or_load_texture_by_name(scene, texture);
		elf_set_particles_texture(particles, rtexture);
	}

	fread(model, sizeof(char), 64, file);
	if(strlen(model))
	{
		rmodel = elf_get_or_load_model_by_name(scene, model);
		elf_set_particles_model(particles, rmodel);
	}

	fread(entity, sizeof(char), 64, file);
	if(strlen(entity))
	{
		rentity = elf_get_or_load_entity_by_name(scene, entity);
		elf_set_particles_entity(particles, rentity);
	}

	fread((char*)&particles->max_count, sizeof(int), 1, file);
	fread((char*)&particles->draw_mode, sizeof(unsigned char), 1, file);

	elf_set_particles_max_count(particles, particles->max_count);
	elf_set_particles_draw_mode(particles, particles->draw_mode);

	fread((char*)&particles->spawn_delay, sizeof(float), 1, file);
	fread((char*)&particles->spawn, sizeof(unsigned char), 1, file);
	fread((char*)&particles->gravity.x, sizeof(float), 3, file);
	fread((char*)&particles->size_min, sizeof(float), 1, file);
	fread((char*)&particles->size_max, sizeof(float), 1, file);
	fread((char*)&particles->size_growth_min, sizeof(float), 1, file);
	fread((char*)&particles->size_growth_max, sizeof(float), 1, file);
	fread((char*)&particles->rotation_min, sizeof(float), 1, file);
	fread((char*)&particles->rotation_max, sizeof(float), 1, file);
	fread((char*)&particles->rotation_growth_min, sizeof(float), 1, file);
	fread((char*)&particles->rotation_growth_max, sizeof(float), 1, file);
	fread((char*)&particles->life_span_min, sizeof(float), 1, file);
	fread((char*)&particles->life_span_max, sizeof(float), 1, file);
	fread((char*)&particles->fade_speed_min, sizeof(float), 1, file);
	fread((char*)&particles->fade_speed_max, sizeof(float), 1, file);
	fread((char*)&particles->velocity_min.x, sizeof(float), 3, file);
	fread((char*)&particles->velocity_max.x, sizeof(float), 3, file);
	fread((char*)&particles->position_min.x, sizeof(float), 3, file);
	fread((char*)&particles->position_max.x, sizeof(float), 3, file);
	fread((char*)&particles->color_min.r, sizeof(float), 4, file);
	fread((char*)&particles->color_max.r, sizeof(float), 4, file);

	elf_set_particles_spawn_delay(particles, particles->spawn_delay);
	elf_set_particles_spawn(particles, particles->spawn);
	elf_set_particles_gravity(particles, particles->gravity.x, particles->gravity.y, particles->gravity.z);
	elf_set_particles_size(particles, particles->size_min, particles->size_max);
	elf_set_particles_size_growth(particles, particles->size_growth_min, particles->size_growth_max);
	elf_set_particles_rotation(particles, particles->rotation_min, particles->rotation_max);
	elf_set_particles_rotation_growth(particles, particles->rotation_growth_min, particles->rotation_growth_max);
	elf_set_particles_life_span(particles, particles->life_span_min, particles->life_span_max);
	elf_set_particles_fade_speed(particles, particles->fade_speed_min, particles->fade_speed_max);
	elf_set_particles_velocity_min(particles, particles->velocity_min.x, particles->velocity_min.y, particles->velocity_min.z);
	elf_set_particles_velocity_max(particles, particles->velocity_max.x, particles->velocity_max.y, particles->velocity_max.z);
	elf_set_particles_position_min(particles, particles->position_min.x, particles->position_min.y, particles->position_min.z);
	elf_set_particles_position_max(particles, particles->position_max.x, particles->position_max.y, particles->position_max.z);
	elf_set_particles_color_min(particles, particles->color_min.r, particles->color_min.g, particles->color_min.b, particles->color_min.a);
	elf_set_particles_color_max(particles, particles->color_max.r, particles->color_max.g, particles->color_max.b, particles->color_max.a);

	return particles;
}

elf_script* elf_create_script_from_pak(FILE *file, const char *name, elf_scene *scene)
{
	elf_script *script;
	int magic = 0;
	char rname[64];
	unsigned int length;
	char *text;

	fread((char*)&magic, sizeof(int), 1, file);

	if(magic != ELF_SCRIPT_MAGIC)
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

elf_sprite* elf_create_sprite_from_pak(FILE *file, const char *name, elf_scene *scene)
{
	elf_sprite *sprite;
	elf_material *rmaterial;
	int magic = 0;
	float scale[2] = {0.0, 0.0};
	char material[64];

	fread((char*)&magic, sizeof(int), 1, file);

	if(magic != ELF_SPRITE_MAGIC)
	{
		elf_set_error(ELF_INVALID_FILE, "error: invalid sprite \"%s//%s\", wrong magic number\n", elf_get_scene_file_path(scene), name);
		return NULL;
	}

	sprite = elf_create_sprite(NULL);
	elf_read_actor_header((elf_actor*)sprite, file, scene);

	fread((char*)scale, sizeof(float), 2, file);

	fread(material, sizeof(char), 64, file);
	if(strlen(material))
	{
		rmaterial = elf_get_or_load_material_by_name(scene, material);
		elf_set_sprite_material(sprite, rmaterial);
	}

	elf_set_sprite_scale(sprite, scale[0], scale[1]);

	fread((char*)&sprite->face_camera, sizeof(unsigned char), 1, file);
	elf_set_sprite_face_camera(sprite, sprite->face_camera);

	return sprite;
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

	if(magic != ELF_TEXTURE_MAGIC)
	{
		elf_set_error(ELF_INVALID_FILE, "error: invalid texture \"%s//%s\", wrong magic number\n", elf_get_scene_file_path(scene), name);
		return NULL;
	}

	fread(rname, sizeof(char), 64, file);
	fread((char*)&type, sizeof(unsigned char), 1, file);

	if(type == 1)
	{
		fread((char*)&length, sizeof(int), 1, file);
 
		mem = (char*)malloc(length);
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

elf_scene* elf_create_scene_from_pak(elf_pak *pak)
{
	elf_scene *scene;
	elf_camera *camera;
	elf_entity *entity;
	elf_light *light;
	elf_sprite *sprite;
	elf_particles *particles;
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
		else if(index->index_type == ELF_SPRITE) sprite = elf_get_or_load_sprite_by_name(scene, index->name);
		else if(index->index_type == ELF_PARTICLES) particles = elf_get_or_load_particles_by_name(scene, index->name);
		else if(index->index_type == ELF_SCENE && !scene_read)
		{
			file = fopen(elf_get_pak_file_path(pak), "rb");
			if(file)
			{
				scene_read = ELF_TRUE;
				fseek(file, elf_get_pak_index_offset(index), SEEK_SET);

				fread((char*)&magic, sizeof(int), 1, file);
				if(magic != ELF_SCENE_MAGIC)
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

void elf_write_name_to_file(const char *name, FILE *file)
{
	int empty;
	int length;
	char estr[64];

	memset(estr, 0x0, sizeof(char)*64);

	length = strlen(name);
	if(length > 63) length = 63;

	empty = 64-length;

	fwrite(name, sizeof(char), length, file);
	fwrite(estr, sizeof(char), empty, file);
}

void elf_write_actor_header(elf_actor *actor, FILE *file)
{
	float position[3];
	float rotation[3];
	elf_bezier_point *point;
	elf_bezier_curve *curve;
	unsigned char curve_count;
	int point_count;
	elf_vec3f bounding_lengths;
	elf_vec3f bounding_offset;
	unsigned char shape;
	float mass;
	float lin_damp;
	float ang_damp;
	float lin_sleep;
	float ang_sleep;
	float restitution;
	elf_vec3f anis_fric;
	elf_vec3f linear_factor;
	elf_vec3f angular_factor;

	elf_write_name_to_file(actor->name, file);
	elf_write_name_to_file("", file);
	if(actor->script) elf_write_name_to_file(actor->script->name, file);
	else elf_write_name_to_file("", file);

	elf_get_actor_position_(actor, position);
	elf_get_actor_rotation_(actor, rotation);

	fwrite((char*)position, sizeof(float), 3, file);
	fwrite((char*)rotation, sizeof(float), 3, file);

	curve_count = elf_get_list_length(actor->ipo->curves);
	fwrite((char*)&curve_count, sizeof(unsigned char), 1, file);

	for(curve = (elf_bezier_curve*)elf_begin_list(actor->ipo->curves); curve;
		curve = (elf_bezier_curve*)elf_next_in_list(actor->ipo->curves))
	{
		fwrite((char*)&curve->curve_type, sizeof(unsigned char), 1, file);
		fwrite((char*)&curve->interpolation, sizeof(unsigned char), 1, file);

		point_count = elf_get_list_length(curve->points);
		fwrite((char*)&point_count, sizeof(int), 1, file);
		for(point = (elf_bezier_point*)elf_begin_list(curve->points); point;
			point = (elf_bezier_point*)elf_next_in_list(curve->points))
		{
			fwrite((char*)&point->c1.x, sizeof(float), 2, file);
			fwrite((char*)&point->p.x, sizeof(float), 2, file);
			fwrite((char*)&point->c2.x, sizeof(float), 2, file);
		}
	}

	bounding_lengths = elf_get_actor_bounding_lengths(actor);
	bounding_offset = elf_get_actor_bounding_offset(actor);

	fwrite((char*)&bounding_lengths.x, sizeof(float), 3, file);
	fwrite((char*)&bounding_offset.x, sizeof(float), 3, file);

	shape = (unsigned char)elf_get_actor_shape(actor);
	mass = elf_get_actor_mass(actor);
	lin_damp = elf_get_actor_linear_damping(actor);
	ang_damp = elf_get_actor_angular_damping(actor);
	lin_sleep = elf_get_actor_linear_sleep_threshold(actor);
	ang_sleep = elf_get_actor_angular_sleep_threshold(actor);
	restitution = elf_get_actor_restitution(actor);
	anis_fric = elf_get_actor_anisotropic_friction(actor);
	linear_factor = elf_get_actor_linear_factor(actor);
	angular_factor = elf_get_actor_angular_factor(actor);

	fwrite((char*)&shape, sizeof(unsigned char), 1, file);
	fwrite((char*)&mass, sizeof(float), 1, file);
	fwrite((char*)&lin_damp, sizeof(float), 1, file);
	fwrite((char*)&ang_damp, sizeof(float), 1, file);
	fwrite((char*)&lin_sleep, sizeof(float), 1, file);
	fwrite((char*)&ang_sleep, sizeof(float), 1, file);
	fwrite((char*)&restitution, sizeof(float), 1, file);
	fwrite((char*)&anis_fric.x, sizeof(float), 3, file);
	fwrite((char*)&linear_factor.x, sizeof(float), 3, file);
	fwrite((char*)&angular_factor.x, sizeof(float), 3, file);
}

void elf_write_armature_to_file(elf_armature *armature, FILE *file)
{
	int magic;
	elf_bone *bone;
	int i, j;

	magic = ELF_ARMATURE_MAGIC;
	fwrite((char*)&magic, sizeof(int), 1, file);

	elf_write_name_to_file(armature->name, file);
	fwrite((char*)&armature->frame_count, sizeof(int), 1, file);
	fwrite((char*)&armature->bone_count, sizeof(int), 1, file);

	for(i = 0; i < (int)armature->bone_count; i++)
	{
		bone = armature->bones[i];
		if(!bone) continue;

		elf_write_name_to_file(bone->name, file);
		if(bone->parent) elf_write_name_to_file(bone->parent->name, file);
		else elf_write_name_to_file("", file);
		fwrite((char*)&bone->id, sizeof(int), 1, file);
		fwrite((char*)&bone->pos.x, sizeof(float), 3, file);
		fwrite((char*)&bone->qua.x, sizeof(float), 4, file);

		for(j = 0; j < armature->frame_count; j++)
		{
			fwrite((char*)&bone->frames[j].pos.x, sizeof(float), 3, file);
			fwrite((char*)&bone->frames[j].qua.x, sizeof(float), 4, file);
		}
	}
}

void elf_write_camera_to_file(elf_camera *camera, FILE *file)
{
	int magic;

	magic = 179532111;
	fwrite((char*)&magic, sizeof(int), 1, file);

	elf_write_actor_header((elf_actor*)camera, file);

	fwrite((char*)&camera->fov, sizeof(float), 1, file);
	fwrite((char*)&camera->clip_near, sizeof(float), 1, file);
	fwrite((char*)&camera->clip_far, sizeof(float), 1, file);
}

void elf_write_entity_to_file(elf_entity *entity, FILE *file)
{
	int magic = 0;
	float scale[3];
	unsigned int material_count;
	elf_material *mat;

	magic = ELF_ENTITY_MAGIC;
	fwrite((char*)&magic, sizeof(int), 1, file);

	elf_write_actor_header((elf_actor*)entity, file);

	elf_get_entity_scale_(entity, scale);
	fwrite((char*)scale, sizeof(float), 3, file);

	if(entity->model) elf_write_name_to_file(entity->model->name, file);
	else elf_write_name_to_file("", file);

	if(entity->armature) elf_write_name_to_file(entity->armature->name, file);
	else elf_write_name_to_file("", file);

	material_count = elf_get_entity_material_count(entity);
	fwrite((char*)&material_count, sizeof(unsigned int), 1, file);

	for(mat = (elf_material*)elf_begin_list(entity->materials); mat;
		mat = (elf_material*)elf_next_in_list(entity->materials))
	{
		elf_write_name_to_file(mat->name, file);
	}
}

void elf_write_light_to_file(elf_light *light, FILE *file)
{
	int magic = 0;
	int junk = 0;

	magic = ELF_LIGHT_MAGIC;
	fwrite((char*)&magic, sizeof(int), 1, file);

	elf_write_actor_header((elf_actor*)light, file);

	fwrite((char*)&light->light_type, sizeof(unsigned char), 1, file);
	fwrite((char*)&light->color.r, sizeof(float), 4, file);
	fwrite((char*)&light->distance, sizeof(float), 1, file);
	fwrite((char*)&light->fade_speed, sizeof(float), 1, file);
	fwrite((char*)&light->inner_cone, sizeof(float), 1, file);
	fwrite((char*)&light->outer_cone, sizeof(float), 1, file);
	fwrite((char*)&junk, sizeof(int), 1, file);
	fwrite((char*)&light->shadow_caster, sizeof(unsigned char), 1, file);
}

void elf_write_material_to_file(elf_material *material, FILE *file)
{
	int magic;
	unsigned char texture_count;
	int i;

	magic = ELF_MATERIAL_MAGIC;
	fwrite((char*)&magic, sizeof(int), 1, file);

	elf_write_name_to_file(material->name, file);

	fwrite((char*)&material->diffuse_color.r, sizeof(float), 4, file);
	fwrite((char*)&material->ambient_color.r, sizeof(float), 4, file);
	fwrite((char*)&material->specular_color.r, sizeof(float), 4, file);
	fwrite((char*)&material->spec_power, sizeof(float), 1, file);

	texture_count = elf_get_material_texture_count(material);
	fwrite((char*)&texture_count, sizeof(unsigned char), 1, file);

	for(i = 0; i < GFX_MAX_TEXTURES; i++)
	{
		if(!material->textures[i]) continue;

		elf_write_name_to_file(material->textures[i]->name, file);
		fwrite((char*)&material->texture_types[i], sizeof(unsigned char), 1, file);
		fwrite((char*)&material->texture_parallax_scales[i], sizeof(float), 1, file);
	}
}

void elf_write_model_to_file(elf_model *model, FILE *file)
{
	int magic = 0;
	unsigned char is_normals;
	unsigned char is_tex_coords;
	unsigned char is_weights_and_boneids;
	unsigned char junk;
	int i = 0;
	short int boneids[4];

	magic = ELF_MODEL_MAGIC;
	fwrite((char*)&magic, sizeof(int), 1, file);

	elf_write_name_to_file(model->name, file);

	is_normals = 1;
	is_tex_coords = 0;
	is_weights_and_boneids = 0;
	junk = 0;
	if(model->tex_coords) is_tex_coords = 1;
	if(model->weights && model->boneids) is_weights_and_boneids = 1;
	
	fwrite((char*)&model->vertice_count, sizeof(int), 1, file);
	fwrite((char*)&model->frame_count, sizeof(int), 1, file);
	fwrite((char*)&model->indice_count, sizeof(int), 1, file);
	fwrite((char*)&model->area_count, sizeof(int), 1, file);
	fwrite((char*)&is_normals, sizeof(unsigned char), 1, file);
	fwrite((char*)&is_tex_coords, sizeof(unsigned char), 1, file);
	fwrite((char*)&is_weights_and_boneids, sizeof(unsigned char), 1, file);
	fwrite((char*)&junk, sizeof(unsigned char), 1, file);

	fwrite((char*)gfx_get_vertex_data_buffer(model->vertices), sizeof(float), 3*model->vertice_count, file);

	for(i = 0; i < model->area_count; i++)
	{
		fwrite((char*)&model->areas[i].indice_count, sizeof(int), 1, file);
		if(model->areas[i].indice_count)
		{
			fwrite((char*)gfx_get_vertex_data_buffer(model->areas[i].index),
				sizeof(unsigned int), model->areas[i].indice_count, file);
		}
	}

	fwrite((char*)gfx_get_vertex_data_buffer(model->normals), sizeof(float), 3*model->vertice_count, file);

	// read tex coords
	if(is_tex_coords > 0)
		fwrite((char*)gfx_get_vertex_data_buffer(model->tex_coords), sizeof(float), 2*model->vertice_count, file);

	// read weights and bone ids
	if(is_weights_and_boneids > 0)
	{
		fwrite((char*)model->weights, sizeof(float), 4*model->vertice_count, file);

		for(i = 0; i < model->vertice_count; i++)
		{
			boneids[0] = model->boneids[i*4];
			boneids[1] = model->boneids[i*4+1];
			boneids[2] = model->boneids[i*4+2];
			boneids[3] = model->boneids[i*4+3];

			fwrite((char*)boneids, sizeof(short int), 4, file);
		}
	}
}

void elf_write_particles_to_file(elf_particles *particles, FILE *file)
{
	int magic = 0;

	magic = ELF_PARTICLES_MAGIC;
	fwrite((char*)&magic, sizeof(int), 1, file);

	elf_write_actor_header((elf_actor*)particles, file);

	if(particles->texture) elf_write_name_to_file(particles->texture->name, file);
	else elf_write_name_to_file("", file);

	if(particles->model) elf_write_name_to_file(particles->model->name, file);
	else elf_write_name_to_file("", file);

	if(particles->entity) elf_write_name_to_file(particles->entity->name, file);
	else elf_write_name_to_file("", file);

	fwrite((char*)&particles->max_count, sizeof(int), 1, file);
	fwrite((char*)&particles->draw_mode, sizeof(unsigned char), 1, file);

	fwrite((char*)&particles->spawn_delay, sizeof(float), 1, file);
	fwrite((char*)&particles->spawn, sizeof(unsigned char), 1, file);
	fwrite((char*)&particles->gravity.x, sizeof(float), 3, file);
	fwrite((char*)&particles->size_min, sizeof(float), 1, file);
	fwrite((char*)&particles->size_max, sizeof(float), 1, file);
	fwrite((char*)&particles->size_growth_min, sizeof(float), 1, file);
	fwrite((char*)&particles->size_growth_max, sizeof(float), 1, file);
	fwrite((char*)&particles->rotation_min, sizeof(float), 1, file);
	fwrite((char*)&particles->rotation_max, sizeof(float), 1, file);
	fwrite((char*)&particles->rotation_growth_min, sizeof(float), 1, file);
	fwrite((char*)&particles->rotation_growth_max, sizeof(float), 1, file);
	fwrite((char*)&particles->life_span_min, sizeof(float), 1, file);
	fwrite((char*)&particles->life_span_max, sizeof(float), 1, file);
	fwrite((char*)&particles->fade_speed_min, sizeof(float), 1, file);
	fwrite((char*)&particles->fade_speed_max, sizeof(float), 1, file);
	fwrite((char*)&particles->velocity_min.x, sizeof(float), 3, file);
	fwrite((char*)&particles->velocity_max.x, sizeof(float), 3, file);
	fwrite((char*)&particles->position_min.x, sizeof(float), 3, file);
	fwrite((char*)&particles->position_max.x, sizeof(float), 3, file);
	fwrite((char*)&particles->color_min.r, sizeof(float), 4, file);
	fwrite((char*)&particles->color_max.r, sizeof(float), 4, file);
}

void elf_write_scene_to_file(elf_scene *scene, FILE *file)
{
	int magic;

	magic = ELF_SCENE_MAGIC;
	fwrite((char*)&magic, sizeof(int), 1, file);

	elf_write_name_to_file(scene->name, file);

	fwrite((char*)&scene->ambient_color.r, sizeof(float), 4, file);
}

void elf_write_script_to_file(elf_script *script, FILE *file)
{
	int magic = 0;
	unsigned int length;

	magic = ELF_SCRIPT_MAGIC;
	fwrite((char*)&magic, sizeof(int), 1, file);

	elf_write_name_to_file(script->name, file);

	length = strlen(script->text);
	fwrite((char*)&length, sizeof(unsigned int), 1, file);
	if(length > 0)
	{
		fwrite(script->text, sizeof(char), length, file);
	}
}

void elf_write_sprite_to_file(elf_sprite *sprite, FILE *file)
{
	int magic = 0;

	magic = ELF_SPRITE_MAGIC;
	fwrite((char*)&magic, sizeof(int), 1, file);

	elf_write_actor_header((elf_actor*)sprite, file);

	fwrite((char*)&sprite->scale.x, sizeof(float), 2, file);

	if(sprite->material) elf_write_name_to_file(sprite->material->name, file);
	else elf_write_name_to_file("", file);

	fwrite((char*)&sprite->face_camera, sizeof(unsigned char), 1, file);
}

void elf_write_texture_to_file(elf_texture *texture, FILE *file)
{
	int magic;
	unsigned char type;

	magic = ELF_TEXTURE_MAGIC;
	fwrite((char*)&magic, sizeof(int), 1, file);

	elf_write_name_to_file(texture->name, file);

	type = 1;
	fwrite((char*)&type, sizeof(unsigned char), 1, file);

	fwrite((char*)&texture->data_size, sizeof(int), 1, file);
	fwrite((char*)texture->data, 1, texture->data_size, file);
}

void elf_write_resource_index_to_file(elf_resource *resource, unsigned int *offset, FILE *file)
{
	unsigned char ucval;
	unsigned int ival;

	ucval = resource->type;
	fwrite((char*)&ucval, sizeof(unsigned char), 1, file);

	elf_write_name_to_file(resource->name, file);

	ival = *offset;
	fwrite((char*)&ival, sizeof(int), 1, file);

	switch(resource->type)
	{
		case ELF_SCENE: *offset += elf_get_scene_size_bytes((elf_scene*)resource); break;
		case ELF_SCRIPT: *offset += elf_get_script_size_bytes((elf_script*)resource); break;
		case ELF_TEXTURE: *offset += elf_get_texture_size_bytes((elf_texture*)resource); break;
		case ELF_MODEL: *offset += elf_get_model_size_bytes((elf_model*)resource); break;
		case ELF_MATERIAL: *offset += elf_get_material_size_bytes((elf_material*)resource); break;
		case ELF_CAMERA: *offset += elf_get_camera_size_bytes((elf_camera*)resource); break;
		case ELF_ENTITY: *offset += elf_get_entity_size_bytes((elf_entity*)resource); break;
		case ELF_LIGHT: *offset += elf_get_light_size_bytes((elf_light*)resource); break;
		case ELF_PARTICLES: *offset += elf_get_particles_size_bytes((elf_particles*)resource); break;
		case ELF_SPRITE: *offset += elf_get_sprite_size_bytes((elf_sprite*)resource); break;
		case ELF_ARMATURE: *offset += elf_get_armature_size_bytes((elf_armature*)resource); break;
	}
}

void elf_write_resource_indexes_to_file(elf_list *resources, unsigned int *offset, FILE *file)
{
	elf_resource *res;

	for(res = (elf_resource*)elf_begin_list(resources); res;
		res = (elf_resource*)elf_next_in_list(resources))
	{
		elf_write_resource_index_to_file(res, offset, file);
	}
}

void elf_write_resources_to_file(elf_list *resources, FILE *file)
{
	elf_resource *res;

	for(res = (elf_resource*)elf_begin_list(resources); res;
		res = (elf_resource*)elf_next_in_list(resources))
	{
		switch(res->type)
		{
			case ELF_SCENE: elf_write_scene_to_file((elf_scene*)res, file); break;
			case ELF_SCRIPT: elf_write_script_to_file((elf_script*)res, file); break;
			case ELF_TEXTURE: elf_write_texture_to_file((elf_texture*)res, file); break;
			case ELF_MODEL: elf_write_model_to_file((elf_model*)res, file); break;
			case ELF_MATERIAL: elf_write_material_to_file((elf_material*)res, file); break;
			case ELF_CAMERA: elf_write_camera_to_file((elf_camera*)res, file); break;
			case ELF_ENTITY: elf_write_entity_to_file((elf_entity*)res, file); break;
			case ELF_LIGHT: elf_write_light_to_file((elf_light*)res, file); break;
			case ELF_PARTICLES: elf_write_particles_to_file((elf_particles*)res, file); break;
			case ELF_SPRITE: elf_write_sprite_to_file((elf_sprite*)res, file); break;
			case ELF_ARMATURE: elf_write_armature_to_file((elf_armature*)res, file); break;
		}
	}
}

unsigned char elf_save_scene_to_pak(elf_scene *scene, const char *file_path)
{
	int i;
	unsigned int offset;
	int ival;
	
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
						if(elf_load_texture_data(mat->textures[i]))
						{
							elf_set_unique_name_for_resource(textures, (elf_resource*)mat->textures[i]);
							elf_append_to_list(textures, (elf_object*)mat->textures[i]);
						}
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

		if(par->texture && !elf_get_resource_by_id(textures, par->texture->id))
		{
			if(elf_load_texture_data(par->texture))
			{
				elf_set_unique_name_for_resource(textures, (elf_resource*)par->texture);
				elf_append_to_list(textures, (elf_object*)par->texture);
			}
		}

		if(par->model && !elf_get_resource_by_id(models, par->model->id))
		{
			elf_set_unique_name_for_resource(models, (elf_resource*)par->model);
			elf_append_to_list(models, (elf_object*)par->model);
		}

		if(par->entity && !elf_get_resource_by_id(entities, par->entity->id))
		{
			elf_set_unique_name_for_resource(entities, (elf_resource*)par->entity);
			elf_append_to_list(entities, (elf_object*)par->entity);
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

		mat = spr->material;

		if(mat && !elf_get_resource_by_id(materials, mat->id))
		{
			elf_set_unique_name_for_resource(materials, (elf_resource*)mat);
			elf_append_to_list(materials, (elf_object*)mat);

			for(i = 0; i < GFX_MAX_TEXTURES; i++)
			{
				if(mat->textures[i] && !elf_get_resource_by_id(textures, mat->textures[i]->id))
				{
					if(elf_load_texture_data(mat->textures[i]))
					{
						elf_set_unique_name_for_resource(textures, (elf_resource*)mat->textures[i]);
						elf_append_to_list(textures, (elf_object*)mat->textures[i]);
					}
				}
			}
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

	offset = 0;
	offset += sizeof(unsigned char);	// index type
	offset += sizeof(char)*64;	// index name
	offset += sizeof(int);	// index offset
	offset *= elf_get_list_length(scenes) +
		elf_get_list_length(scripts) +
		elf_get_list_length(textures) +
		elf_get_list_length(materials) +
		elf_get_list_length(models) +
		elf_get_list_length(cameras) +
		elf_get_list_length(entities) +
		elf_get_list_length(lights) +
		elf_get_list_length(armatures) +
		elf_get_list_length(particles) +
		elf_get_list_length(sprites);

	offset += sizeof(int);	// magic
	offset += sizeof(int);	// number of indexes

	ival = 179532100;

	fwrite((char*)&ival, sizeof(int), 1, file);	// magic

	ival = elf_get_list_length(scenes) +
		elf_get_list_length(scripts) +
		elf_get_list_length(textures) +
		elf_get_list_length(materials) +
		elf_get_list_length(models) +
		elf_get_list_length(cameras) +
		elf_get_list_length(entities) +
		elf_get_list_length(lights) +
		elf_get_list_length(armatures) +
		elf_get_list_length(particles) +
		elf_get_list_length(sprites);

	fwrite((char*)&ival, sizeof(int), 1, file);	// index count

	elf_write_resource_indexes_to_file(scenes, &offset, file);
	elf_write_resource_indexes_to_file(scripts, &offset, file);
	elf_write_resource_indexes_to_file(textures, &offset, file);
	elf_write_resource_indexes_to_file(materials, &offset, file);
	elf_write_resource_indexes_to_file(models, &offset, file);
	elf_write_resource_indexes_to_file(cameras, &offset, file);
	elf_write_resource_indexes_to_file(entities, &offset, file);
	elf_write_resource_indexes_to_file(lights, &offset, file);
	elf_write_resource_indexes_to_file(armatures, &offset, file);
	elf_write_resource_indexes_to_file(particles, &offset, file);
	elf_write_resource_indexes_to_file(sprites, &offset, file);

	elf_write_resources_to_file(scenes, file);
	elf_write_resources_to_file(scripts, file);
	elf_write_resources_to_file(textures, file);
	elf_write_resources_to_file(materials, file);
	elf_write_resources_to_file(models, file);
	elf_write_resources_to_file(cameras, file);
	elf_write_resources_to_file(entities, file);
	elf_write_resources_to_file(lights, file);
	elf_write_resources_to_file(armatures, file);
	elf_write_resources_to_file(particles, file);
	elf_write_resources_to_file(sprites, file);

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

