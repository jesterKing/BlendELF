
void elf_actor_ipo_callback(elf_frame_player *player)
{
	elf_actor *actor;
	float frame;
	elf_vec3f pos;
	elf_vec3f rot;
	elf_vec3f scale;
	elf_vec4f qua;

	actor = (elf_actor*)elf_get_frame_player_user_data(player);
	frame = elf_get_frame_player_frame(player);

	if(actor->ipo->loc)
	{
		pos = elf_get_ipo_loc(actor->ipo, frame);
		elf_set_actor_position(actor, pos.x, pos.y, pos.z);
	}
	if(actor->ipo->rot)
	{
		rot = elf_get_ipo_rot(actor->ipo, frame);
		elf_set_actor_rotation(actor, rot.x, rot.y, rot.z);
	}
	if(actor->ipo->scale)
	{
		scale = elf_get_ipo_scale(actor->ipo, frame);
		if(actor->type == ELF_ENTITY) elf_set_entity_scale((elf_entity*)actor, scale.x, scale.y, scale.z);
	}
	if(actor->ipo->qua)
	{
		qua = elf_get_ipo_qua(actor->ipo, frame);
		elf_set_actor_orientation(actor, qua.x, qua.y, qua.z, qua.w);
	}
}

void elf_init_actor(elf_actor *actor, unsigned char camera)
{
	if(!camera) actor->transform = gfx_create_object_transform();
	else actor->transform = gfx_create_camera_transform();

	actor->joints = elf_create_list();
	actor->children = elf_create_list();
	actor->sources = elf_create_list();
	actor->properties = elf_create_list();

	elf_inc_ref((elf_object*)actor->joints);
	elf_inc_ref((elf_object*)actor->children);
	elf_inc_ref((elf_object*)actor->sources);
	elf_inc_ref((elf_object*)actor->properties);

	actor->ipo = elf_create_ipo();
	elf_inc_ref((elf_object*)actor->ipo);

	actor->ipo_player = elf_create_frame_player();
	elf_inc_ref((elf_object*)actor->ipo_player);
	elf_set_frame_player_user_data(actor->ipo_player, actor);
	elf_set_frame_player_callback(actor->ipo_player, elf_actor_ipo_callback);

	actor->moved = ELF_TRUE;
}

void elf_read_actor_header(elf_actor *actor, FILE *file, elf_scene *scene)
{
	char name[64];
	char parent_name[64];
	char script_name[64];
	float position[3];
	float rotation[3];
	elf_actor *parent;
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

	parent = NULL;
	script = NULL;

	if(scene && strlen(parent_name) > 0)
	{
		parent = elf_get_or_load_actor_by_name(scene, parent_name);
		if(parent) elf_set_actor_parent(actor, parent);
	}

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

void elf_update_actor(elf_actor *actor)
{
	static float oposition[3];
	static float oorient[4];
	static float position[3];
	static float orient[4];
	static elf_audio_source *source;

	actor->moved = ELF_FALSE;

	if(actor->object && !elf_is_physics_object_static(actor->object))
	{
		gfx_get_transform_position(actor->transform, oposition);
		gfx_get_transform_orientation(actor->transform, oorient);

		elf_get_physics_object_position(actor->object, position);
		elf_get_physics_object_orientation(actor->object, orient);

		gfx_set_transform_position(actor->transform, position[0], position[1], position[2]);
		gfx_set_transform_orientation(actor->transform, orient[0], orient[1], orient[2], orient[3]);

		if(actor->dobject)
		{
			elf_set_physics_object_position(actor->dobject, position[0], position[1], position[2]);
			elf_set_physics_object_orientation(actor->dobject, orient[0], orient[1], orient[2], orient[3]);
		}

		if(memcmp(&oposition, &position, sizeof(float)*3)) actor->moved = ELF_TRUE;
		if(memcmp(&oorient, &orient, sizeof(float)*4)) actor->moved = ELF_TRUE;
	}
	else
	{
		elf_get_actor_position_(actor, position);
	}

	if(actor->script)
	{
		eng->actor = (elf_object*)actor;
		elf_inc_ref((elf_object*)actor);

		elf_run_string("me = elf.GetActor()");
		elf_run_script(actor->script);
		elf_run_string("me = nil");

		elf_dec_ref((elf_object*)actor);
		eng->actor = NULL;
	}

	if(actor->object) elf_clear_physics_object_collisions(actor->object);

	for(source = (elf_audio_source*)elf_begin_list(actor->sources); source;
		source = (elf_audio_source*)elf_next_in_list(actor->sources))
	{
		if(elf_get_object_ref_count((elf_object*)source) < 2 &&
			!elf_is_sound_playing(source) &&
			!elf_is_sound_paused(source))
		{
			elf_remove_from_list(actor->sources, (elf_object*)source);
		}
		else
		{
			elf_set_sound_position(source, position[0], position[1], position[2]);
		}
	}

	elf_update_frame_player(actor->ipo_player);
}

void elf_clean_actor(elf_actor *actor)
{
	elf_joint *joint;

	if(actor->name) elf_destroy_string(actor->name);
	if(actor->file_path) elf_destroy_string(actor->file_path);
	if(actor->transform) gfx_destroy_transform(actor->transform);
	if(actor->object)
	{
		elf_set_physics_object_actor(actor->object, NULL);
		elf_set_physics_object_world(actor->object, NULL);
		elf_dec_ref((elf_object*)actor->object);
	}
	if(actor->dobject)
	{
		elf_set_physics_object_actor(actor->dobject, NULL);
		elf_set_physics_object_world(actor->dobject, NULL);
		elf_dec_ref((elf_object*)actor->dobject);
	}
	if(actor->script) elf_dec_ref((elf_object*)actor->script);

	//elf_set_actor_parent(actor, NULL); -- doesn't make sense to call this here
	//elf_clean_actor doesn't have any chance of being called if the actor still
	//exists in the parents child list, thus having a reference count greater than 0
	elf_remove_actor_children(actor);

	for(joint = (elf_joint*)elf_begin_list(actor->joints); joint;
		joint = (elf_joint*)elf_next_in_list(actor->joints))
	{
		if(elf_get_joint_actor_a(joint) == actor)
			elf_remove_from_list(elf_get_joint_actor_b(joint)->joints, (elf_object*)joint);
		else elf_remove_from_list(elf_get_joint_actor_a(joint)->joints, (elf_object*)joint);
		elf_clear_joint(joint);
	}

	elf_dec_ref((elf_object*)actor->joints);
	elf_dec_ref((elf_object*)actor->children);
	elf_dec_ref((elf_object*)actor->sources);
	elf_dec_ref((elf_object*)actor->properties);

	elf_dec_ref((elf_object*)actor->ipo);
	elf_dec_ref((elf_object*)actor->ipo_player);
}

const char* elf_get_actor_name(elf_actor *actor)
{
	return actor->name;
}

const char* elf_get_actor_file_path(elf_actor *actor)
{
	return actor->file_path;
}

elf_actor* elf_get_actor_parent(elf_actor *actor)
{
	return actor->parent;
}

void elf_set_actor_parent(elf_actor *actor, elf_actor *parent)
{
	if(actor->parent) elf_remove_from_list(actor->parent->children, (elf_object*)actor);
	actor->parent = parent;
	if(actor->parent) elf_append_to_list(actor->parent->children, (elf_object*)actor);
}

int elf_get_actor_child_count(elf_actor *actor)
{
	return elf_get_list_length(actor->children);
}

elf_actor* elf_get_actor_child_by_name(elf_actor *actor, const char *name)
{
	elf_actor *cactor;

	for(cactor = (elf_actor*)elf_begin_list(actor->children); cactor;
			cactor = (elf_actor*)elf_next_in_list(actor->children))
	{
			if(!strcmp(cactor->name, name)) return cactor;
	}

	return NULL;
}

elf_actor* elf_get_actor_child_by_index(elf_actor *actor, int idx)
{
	int i;
	elf_actor *cactor;

	if(idx < 0 || idx < elf_get_list_length(actor->children)-1) return NULL;

	for(i = 0, cactor = (elf_actor*)elf_begin_list(actor->children); cactor;
			i++, cactor = (elf_actor*)elf_next_in_list(actor->children))
	{
			if(idx == i) return cactor;
	}

	return NULL;
}

void elf_remove_actor_children(elf_actor* actor)
{
	elf_dec_ref((elf_object*)actor->children);
	actor->children = elf_create_list();
	elf_inc_ref((elf_object*)actor->children);
}

elf_script* elf_get_actor_script(elf_actor *actor)
{
	return actor->script;
}

void elf_set_actor_script(elf_actor *actor, elf_script *script)
{
	if(actor->script) elf_dec_ref((elf_object*)actor->script);
	actor->script = script;
	if(actor->script) elf_inc_ref((elf_object*)actor->script);
}

void elf_set_actor_position(elf_actor *actor, float x, float y, float z)
{
	elf_actor *cactor;
	float position[3];
	float disp[3];

	actor->moved = ELF_TRUE;

	elf_get_actor_position_(actor, position);

	for(cactor = (elf_actor*)elf_begin_list(actor->children); cactor;
		cactor = (elf_actor*)elf_next_in_list(actor->children))
	{
		if(!cactor->object || elf_get_physics_object_mass(cactor->object) < 0.00001)
		{
			elf_get_actor_position_(cactor, disp);
			disp[0] -= position[0];
			disp[1] -= position[1];
			disp[2] -= position[2];
			elf_set_actor_position(cactor, x+disp[0], y+disp[1], z+disp[2]);
		}
	}

	gfx_set_transform_position(actor->transform, x, y, z);

	if(actor->object) elf_set_physics_object_position(actor->object, x, y, z);
	if(actor->dobject) elf_set_physics_object_position(actor->dobject, x, y, z);

	if(actor->type == ELF_LIGHT) elf_set_actor_position((elf_actor*)((elf_light*)actor)->shadow_camera, x, y, z);
}

void elf_recursive_reference_rotation(elf_actor *actor, float *ref, float *orient)
{
	float temp_vec1[3];
	float temp_vec2[3];
	float temp_qua1[4];
	float temp_qua2[4];
	elf_actor *cactor;

	elf_get_actor_position_(actor, temp_vec1);
	elf_get_actor_orientation_(actor, temp_qua1);

	temp_vec1[0] -= ref[0];
	temp_vec1[1] -= ref[1];
	temp_vec1[2] -= ref[2];

	gfx_mul_qua_vec(orient, temp_vec1, temp_vec2);
	gfx_mul_qua_qua(orient, temp_qua1, temp_qua2);

	gfx_set_transform_position(actor->transform, ref[0]+temp_vec2[0],
		ref[1]+temp_vec2[1], ref[2]+temp_vec2[2]);
	gfx_set_transform_orientation(actor->transform, temp_qua2[0],
		temp_qua2[1], temp_qua2[2], temp_qua2[3]);

	if(actor->object)
	{
		elf_set_physics_object_position(actor->object, ref[0]+temp_vec2[0], ref[1]+temp_vec2[1], ref[2]+temp_vec2[2]);
		elf_set_physics_object_orientation(actor->object, temp_qua2[0], temp_qua2[1], temp_qua2[2], temp_qua2[3]);
	}
	if(actor->dobject)
	{
		elf_set_physics_object_position(actor->dobject, ref[0]+temp_vec2[0], ref[1]+temp_vec2[1], ref[2]+temp_vec2[2]);
		elf_set_physics_object_orientation(actor->dobject, temp_qua2[0], temp_qua2[1], temp_qua2[2], temp_qua2[3]);
	}

	actor->moved = ELF_TRUE;

	for(cactor = (elf_actor*)elf_begin_list(actor->children); cactor;
		cactor = (elf_actor*)elf_next_in_list(actor->children))
	{
		if(!cactor->object || elf_get_physics_object_mass(cactor->object) < 0.00001)
		{
			elf_recursive_reference_rotation(cactor, ref, orient);
		}
	}
}

void elf_set_actor_rotation(elf_actor *actor, float x, float y, float z)
{
	float orient[4];
	float rotation[3];
	float position[3];
	elf_actor *cactor;

	elf_get_actor_position_(actor, position);
	elf_get_actor_rotation_(actor, rotation);

	gfx_set_qua_rotation(x, y, z, orient);

	gfx_set_transform_orientation(actor->transform, orient[0], orient[1], orient[2], orient[3]);

	if(actor->object) elf_set_physics_object_orientation(actor->object, orient[0], orient[1], orient[2], orient[3]);
	if(actor->dobject) elf_set_physics_object_orientation(actor->dobject, orient[0], orient[1], orient[2], orient[3]);

	actor->moved = ELF_TRUE;

	gfx_set_qua_rotation(x-rotation[0], y-rotation[1], z-rotation[2], orient);

	for(cactor = (elf_actor*)elf_begin_list(actor->children); cactor;
		cactor = (elf_actor*)elf_next_in_list(actor->children))
	{
		if(!cactor->object || elf_get_physics_object_mass(cactor->object) < 0.00001)
		{
			elf_recursive_reference_rotation(cactor, position, orient);
		}
	}

	if(actor->type == ELF_LIGHT) elf_set_actor_rotation((elf_actor*)((elf_light*)actor)->shadow_camera, x, y, z);
}

void elf_set_actor_orientation(elf_actor *actor, float x, float y, float z, float w)
{
	float diff_qua[4];
	float temp_qua1[4];
	float temp_qua2[4];
	float position[3];
	elf_actor *cactor;

	elf_get_actor_orientation_(actor, temp_qua1);
	elf_get_actor_position_(actor, position);
	gfx_qua_get_inverse(temp_qua1, temp_qua2);

	temp_qua1[0] = x;
	temp_qua1[1] = y;
	temp_qua1[2] = z;
	temp_qua1[3] = w;

	gfx_mul_qua_qua(temp_qua1, temp_qua2, diff_qua);

	gfx_set_transform_orientation(actor->transform, x, y, z, w);

	if(actor->object) elf_set_physics_object_orientation(actor->object, x, y, z, w);
	if(actor->dobject) elf_set_physics_object_orientation(actor->dobject, x, y, z, w);

	actor->moved = ELF_TRUE;

	for(cactor = (elf_actor*)elf_begin_list(actor->children); cactor;
		cactor = (elf_actor*)elf_next_in_list(actor->children))
	{
		if(!cactor->object || elf_get_physics_object_mass(cactor->object) < 0.00001)
		{
			elf_recursive_reference_rotation(cactor, position, diff_qua);
		}
	}

	if(actor->type == ELF_LIGHT) elf_set_actor_orientation((elf_actor*)((elf_light*)actor)->shadow_camera, x, y, z, w);
}

void elf_rotate_actor(elf_actor *actor, float x, float y, float z)
{
	float orient[4];
	float position[3];
	elf_actor *cactor;

	gfx_rotate_transform(actor->transform, x*eng->sync, y*eng->sync, z*eng->sync);

	gfx_get_transform_orientation(actor->transform, orient);

	if(actor->object) elf_set_physics_object_orientation(actor->object, orient[0], orient[1], orient[2], orient[3]);
	if(actor->dobject) elf_set_physics_object_orientation(actor->dobject, orient[0], orient[1], orient[2], orient[3]);

	actor->moved = ELF_TRUE;

	gfx_set_qua_rotation(x*eng->sync, y*eng->sync, z*eng->sync, orient);
	elf_get_actor_position_(actor, position);

	for(cactor = (elf_actor*)elf_begin_list(actor->children); cactor;
		cactor = (elf_actor*)elf_next_in_list(actor->children))
	{
		if(!cactor->object || elf_get_physics_object_mass(cactor->object) < 0.00001)
		{
			elf_recursive_reference_rotation(cactor, position, orient);
		}
	}

	if(actor->type == ELF_LIGHT) elf_rotate_actor((elf_actor*)((elf_light*)actor)->shadow_camera, x, y, z);
}

void elf_rotate_actor_local(elf_actor *actor, float x, float y, float z)
{
	float orient[4];
	float new_orient[4];
	float temp_qua1[4];
	float diff_qua[4];
	float position[3];
	elf_actor *cactor;

	elf_get_actor_position_(actor, position);
	elf_get_actor_orientation_(actor, orient);

	memcpy(new_orient, orient, sizeof(float)*4);
	gfx_rotate_qua_local(x*eng->sync, y*eng->sync, z*eng->sync, new_orient);

	gfx_qua_get_inverse(orient, temp_qua1);
	gfx_mul_qua_qua(new_orient, temp_qua1, diff_qua);

	gfx_set_transform_orientation(actor->transform, new_orient[0], new_orient[1], new_orient[2], new_orient[3]);

	if(actor->object) elf_set_physics_object_orientation(actor->object, new_orient[0], new_orient[1], new_orient[2], new_orient[3]);
	if(actor->dobject) elf_set_physics_object_orientation(actor->dobject, new_orient[0], new_orient[1], new_orient[2], new_orient[3]);

	actor->moved = ELF_TRUE;

	for(cactor = (elf_actor*)elf_begin_list(actor->children); cactor;
		cactor = (elf_actor*)elf_next_in_list(actor->children))
	{
		if(!cactor->object || elf_get_physics_object_mass(cactor->object) < 0.00001)
		{
			elf_recursive_reference_rotation(cactor, position, diff_qua);
		}
	}

	if(actor->type == ELF_LIGHT) elf_rotate_actor_local((elf_actor*)((elf_light*)actor)->shadow_camera, x, y, z);
}

void elf_move_actor(elf_actor *actor, float x, float y, float z)
{
	elf_actor *cactor;
	float position[3];

	actor->moved = ELF_TRUE;

	gfx_move_transform(actor->transform, x*eng->sync, y*eng->sync, z*eng->sync);
	gfx_get_transform_position(actor->transform, position);

	if(actor->object) elf_set_physics_object_position(actor->object, position[0], position[1], position[2]);
	if(actor->dobject) elf_set_physics_object_position(actor->dobject, position[0], position[1], position[2]);

	for(cactor = (elf_actor*)elf_begin_list(actor->children); cactor;
		cactor = (elf_actor*)elf_next_in_list(actor->children))
	{
		if(!cactor->object || elf_get_physics_object_mass(cactor->object) < 0.00001)
		{
			elf_move_actor(cactor, x, y, z);
		}
	}

	if(actor->type == ELF_LIGHT) elf_move_actor((elf_actor*)((elf_light*)actor)->shadow_camera, x, y, z);
}

void elf_move_actor_local(elf_actor *actor, float x, float y, float z)
{
	float orient[4];
	float temp_vec1[3];
	float temp_vec2[3];
	elf_actor *cactor;

	elf_get_actor_orientation_(actor, orient);

	temp_vec1[0] = x;
	temp_vec1[1] = y;
	temp_vec1[2] = z;

	gfx_mul_qua_vec(orient, temp_vec1, temp_vec2);

	elf_get_actor_position_(actor, temp_vec1);

	temp_vec1[0] = temp_vec1[0]+temp_vec2[0]*eng->sync;
	temp_vec1[1] = temp_vec1[1]+temp_vec2[1]*eng->sync;
	temp_vec1[2] = temp_vec1[2]+temp_vec2[2]*eng->sync;

	gfx_set_transform_position(actor->transform, temp_vec1[0], temp_vec1[1], temp_vec1[2]);

	if(actor->object) elf_set_physics_object_position(actor->object, temp_vec1[0], temp_vec1[1], temp_vec1[2]);
	if(actor->dobject) elf_set_physics_object_position(actor->dobject, temp_vec1[0], temp_vec1[1], temp_vec1[2]);

	actor->moved = ELF_TRUE;

	for(cactor = (elf_actor*)elf_begin_list(actor->children); cactor;
		cactor = (elf_actor*)elf_next_in_list(actor->children))
	{
		if(!cactor->object || elf_get_physics_object_mass(cactor->object) < 0.00001)
		{
			elf_move_actor(cactor, temp_vec2[0], temp_vec2[1], temp_vec2[2]);
		}
	}

	if(actor->type == ELF_LIGHT) elf_move_actor_local((elf_actor*)((elf_light*)actor)->shadow_camera, x, y, z);
}

elf_vec3f elf_get_actor_position(elf_actor *actor)
{
	elf_vec3f pos;
	gfx_get_transform_position(actor->transform, &pos.x);
	return pos;
}

elf_vec3f elf_get_actor_rotation(elf_actor *actor)
{
	elf_vec3f rot;
	gfx_get_transform_rotation(actor->transform, &rot.x);
	return rot;
}

elf_vec4f elf_get_actor_orientation(elf_actor *actor)
{
	elf_vec4f orient;
	gfx_get_transform_orientation(actor->transform, &orient.x);
	return orient;
}

void elf_get_actor_position_(elf_actor *actor, float *params)
{
	gfx_get_transform_position(actor->transform, params);
}

void elf_get_actor_rotation_(elf_actor *actor, float *params)
{
	gfx_get_transform_rotation(actor->transform, params);
}

void elf_get_actor_orientation_(elf_actor *actor, float *params)
{
	gfx_get_transform_orientation(actor->transform, params);
}

void elf_set_actor_anisotropic_friction(elf_actor *actor, float x, float y, float z)
{
	if(actor->object) elf_set_physics_object_anisotropic_friction(actor->object, x, y, z);
}

void elf_set_actor_damping(elf_actor *actor, float lin_damp, float ang_damp)
{
	if(actor->object) elf_set_physics_object_damping(actor->object, lin_damp, ang_damp);
}

void elf_set_actor_sleep_thresholds(elf_actor *actor, float lin_thrs, float ang_thrs)
{
	if(actor->object) elf_set_physics_object_sleep_thresholds(actor->object, lin_thrs, ang_thrs);
}

void elf_set_actor_restitution(elf_actor *actor, float restitution)
{
	if(actor->object) elf_set_physics_object_restitution(actor->object, restitution);
}

void elf_add_force_to_actor(elf_actor *actor, float x, float y, float z)
{
	if(actor->object) elf_add_force_to_physics_object(actor->object, x, y, z);
}

void elf_add_torque_to_actor(elf_actor *actor, float x, float y, float z)
{
	if(actor->object) elf_add_torque_to_physics_object(actor->object, x, y, z);
}

void elf_set_actor_linear_velocity(elf_actor *actor, float x, float y, float z)
{
	if(actor->object) elf_set_physics_object_linear_velocity(actor->object, x, y, z);
}

void elf_set_actor_angular_velocity(elf_actor *actor, float x, float y, float z)
{
	if(actor->object) elf_set_physics_object_angular_velocity(actor->object, x, y, z);
}

void elf_set_actor_linear_factor(elf_actor *actor, float x, float y, float z)
{
	if(actor->object) elf_set_physics_object_linear_factor(actor->object, x, y, z);
}

void elf_set_actor_angular_factor(elf_actor *actor, float x, float y, float z)
{
	if(actor->object) elf_set_physics_object_angular_factor(actor->object, x, y, z);
}

elf_vec3f elf_get_actor_linear_velocity(elf_actor *actor)
{
	elf_vec3f result;
	memset(&result, 0x0, sizeof(elf_vec3f));

	if(actor->object) elf_get_physics_object_linear_velocity(actor->object, &result.x);

	return result;
}

elf_vec3f elf_get_actor_angular_velocity(elf_actor *actor)
{
	elf_vec3f result;
	memset(&result, 0x0, sizeof(elf_vec3f));

	if(actor->object) elf_get_physics_object_angular_velocity(actor->object, &result.x);

	return result;
}

elf_vec3f elf_get_actor_linear_factor(elf_actor *actor)
{
	elf_vec3f result;
	memset(&result, 0x0, sizeof(elf_vec3f));

	if(actor->object) elf_get_physics_object_linear_factor(actor->object, &result.x);

	return result;
}

elf_vec3f elf_get_actor_angular_factor(elf_actor *actor)
{
	elf_vec3f result;
	memset(&result, 0x0, sizeof(elf_vec3f));

	if(actor->object) elf_get_physics_object_angular_factor(actor->object, &result.x);

	return result;
}

elf_vec3f elf_get_actor_anisotropic_friction(elf_actor *actor)
{
	elf_vec3f result;
	memset(&result, 0x0, sizeof(elf_vec3f));

	if(actor->object) elf_get_physics_object_anisotropic_friction(actor->object, &result.x);

	return result;
}

float elf_get_actor_linear_damping(elf_actor *actor)
{
	if(actor->object) return elf_get_physics_object_linear_damping(actor->object);
	return 0.0;
}

float elf_get_actor_angular_damping(elf_actor *actor)
{
	if(actor->object) return elf_get_physics_object_angular_damping(actor->object);
	return 0.0;
}

float elf_get_actor_linear_sleep_threshold(elf_actor *actor)
{
	if(actor->object) return elf_get_physics_object_linear_sleep_threshold(actor->object);
	return 0.0;
}

float elf_get_actor_angular_sleep_threshold(elf_actor *actor)
{
	if(actor->object) return elf_get_physics_object_angular_sleep_threshold(actor->object);
	return 0.0;
}

float elf_get_actor_restitution(elf_actor *actor)
{
	if(actor->object) return elf_get_physics_object_restitution(actor->object);
	return 0.0;
}

elf_joint* elf_add_hinge_joint_to_actor(elf_actor *actor, elf_actor *actor2, const char *name, float px, float py, float pz, float ax, float ay, float az)
{
	elf_joint *joint;

	if(!actor->object || !actor2->object) return NULL;

	joint = elf_create_hinge_joint(actor, actor2, name, px, py, pz, ax, ay, az);

	elf_append_to_list(actor->joints, (elf_object*)joint);
	elf_append_to_list(actor2->joints, (elf_object*)joint);

	return joint;
}

elf_joint* elf_add_ball_joint_to_actor(elf_actor *actor, elf_actor *actor2, const char *name, float px, float py, float pz)
{
	elf_joint *joint;

	if(!actor->object || !actor2->object) return NULL;

	joint = elf_create_ball_joint(actor, actor2, name, px, py, pz);

	elf_append_to_list(actor->joints, (elf_object*)joint);
	elf_append_to_list(actor2->joints, (elf_object*)joint);

	return joint;
}

elf_joint* elf_add_cone_twist_joint_to_actor(elf_actor *actor, elf_actor *actor2, const char *name, float px, float py, float pz, float ax, float ay, float az)
{
	elf_joint *joint;

	if(!actor->object || !actor2->object) return NULL;

	joint = elf_create_cone_twist_joint(actor, actor2, name, px, py, pz, ax, ay, az);

	elf_append_to_list(actor->joints, (elf_object*)joint);
	elf_append_to_list(actor2->joints, (elf_object*)joint);

	return joint;
}

elf_joint* elf_get_actor_joint_by_name(elf_actor *actor, const char *name)
{
	elf_joint *joint;

	for(joint = (elf_joint*)elf_begin_list(actor->joints); joint;
		joint = (elf_joint*)elf_next_in_list(actor->joints))
	{
		if(!strcmp(elf_get_joint_name(joint), name)) return joint;
	}

	return NULL;
}

elf_joint* elf_get_actor_joint_by_index(elf_actor *actor, int idx)
{
	return (elf_joint*)elf_get_item_from_list(actor->joints, idx);
}

unsigned char elf_remove_actor_joint_by_name(elf_actor *actor, const char *name)
{
	elf_joint *joint;

	for(joint = (elf_joint*)elf_begin_list(actor->joints); joint;
		joint = (elf_joint*)elf_next_in_list(actor->joints))
	{
		if(!strcmp(elf_get_joint_name(joint), name))
		{
			if(elf_get_joint_actor_a(joint) == actor)
				elf_remove_from_list(elf_get_joint_actor_b(joint)->joints, (elf_object*)joint);
			else elf_remove_from_list(elf_get_joint_actor_a(joint)->joints, (elf_object*)joint);
			elf_clear_joint(joint);
			return elf_remove_from_list(actor->joints, (elf_object*)joint);
		}
	}

	return ELF_FALSE;
}

unsigned char elf_remove_actor_joint_by_index(elf_actor *actor, int idx)
{
	elf_joint *joint;
	int i;

	for(i = 0, joint = (elf_joint*)elf_begin_list(actor->joints); joint;
		joint = (elf_joint*)elf_next_in_list(actor->joints), i++)
	{
		if(i == idx)
		{
			if(elf_get_joint_actor_a(joint) == actor)
				elf_remove_from_list(elf_get_joint_actor_b(joint)->joints, (elf_object*)joint);
			else elf_remove_from_list(elf_get_joint_actor_a(joint)->joints, (elf_object*)joint);
			elf_clear_joint(joint);
			return elf_remove_from_list(actor->joints, (elf_object*)joint);
		}
	}

	return ELF_FALSE;
}

unsigned char elf_remove_actor_joint_by_object(elf_actor *actor, elf_joint *joint)
{
	if(elf_get_joint_actor_a(joint) == actor)
		elf_remove_from_list(elf_get_joint_actor_b(joint)->joints, (elf_object*)joint);
	else elf_remove_from_list(elf_get_joint_actor_a(joint)->joints, (elf_object*)joint);
	elf_clear_joint(joint);
	return elf_remove_from_list(actor->joints, (elf_object*)joint);
}

void elf_set_actor_ipo_frame(elf_actor *actor, float frame)
{
	elf_set_frame_player_frame(actor->ipo_player, frame);
}

void elf_set_actor_hierarchy_ipo_frame(elf_actor *actor, float frame)
{
	elf_actor *cactor;

	elf_set_frame_player_frame(actor->ipo_player, frame);

	for(cactor = (elf_actor*)elf_begin_list(actor->children); cactor;
		cactor = (elf_actor*)elf_next_in_list(actor->children))
	{
		elf_set_actor_hierarchy_ipo_frame(cactor, frame);
	}
}

void elf_play_actor_ipo(elf_actor *actor, float start, float end, float speed)
{
	elf_play_frame_player(actor->ipo_player, start, end, speed);
}

void elf_loop_actor_ipo(elf_actor *actor, float start, float end, float speed)
{
	elf_loop_frame_player(actor->ipo_player, start, end, speed);
}

void elf_stop_actor_ipo(elf_actor *actor)
{
	elf_stop_frame_player(actor->ipo_player);
}

void elf_pause_actor_ipo(elf_actor *actor)
{
	elf_stop_frame_player(actor->ipo_player);
}

void elf_resume_actor_ipo(elf_actor *actor)
{
	elf_stop_frame_player(actor->ipo_player);
}

float elf_get_actor_ipo_start(elf_actor *actor)
{
	return elf_get_frame_player_start(actor->ipo_player);
}

float elf_get_actor_ipo_end(elf_actor *actor)
{
	return elf_get_frame_player_end(actor->ipo_player);
}

float elf_get_actor_ipo_speed(elf_actor *actor)
{
	return elf_get_frame_player_speed(actor->ipo_player);
}

float elf_get_actor_ipo_frame(elf_actor *actor)
{
	return elf_get_frame_player_frame(actor->ipo_player);
}

unsigned char elf_is_actor_ipo_playing(elf_actor *actor)
{
	return elf_is_frame_player_playing(actor->ipo_player);
}

unsigned char elf_is_actor_ipo_paused(elf_actor *actor)
{
	return elf_is_frame_player_paused(actor->ipo_player);
}

int elf_get_actor_collision_count(elf_actor *actor)
{
	if(actor->object) return elf_get_physics_object_collision_count(actor->object);
	return 0;
}

elf_collision* elf_get_actor_collision(elf_actor *actor, int idx)
{
	return elf_get_physics_object_collision(actor->object, idx);
}

int elf_get_actor_property_count(elf_actor *actor)
{
	return elf_get_list_length(actor->properties);
}

void elf_add_property_to_actor(elf_actor *actor, elf_property *property)
{
	elf_append_to_list(actor->properties, (elf_object*)property);
}

elf_property* elf_get_actor_property_by_name(elf_actor *actor, const char *name)
{
	elf_property *prop;

	if(!name || strlen(name) < 1) return ELF_FALSE;

	for(prop = (elf_property*)elf_begin_list(actor->properties); prop;
		prop = (elf_property*)elf_next_in_list(actor->properties))
	{
		if(!strcmp(prop->name, name))
		{
			return prop;
		}
	}

	return NULL;
}

elf_property* elf_get_actor_property_by_index(elf_actor *actor, int idx)
{
	return (elf_property*)elf_get_item_from_list(actor->properties, idx);
}

unsigned char elf_remove_actor_property_by_name(elf_actor *actor, const char *name)
{
	elf_property *prop;

	if(!name || strlen(name) < 1) return ELF_FALSE;

	for(prop = (elf_property*)elf_begin_list(actor->properties); prop;
		prop = (elf_property*)elf_next_in_list(actor->properties))
	{
		if(!strcmp(prop->name, name))
		{
			elf_remove_from_list(actor->properties, (elf_object*)prop);
			return ELF_TRUE;
		}
	}

	return ELF_FALSE;
}

unsigned char elf_remove_actor_property_by_index(elf_actor *actor, int idx)
{
	elf_property *prop;
	int i;

	if(idx < 0 && idx >= elf_get_list_length(actor->properties)) return ELF_FALSE;

	for(i = 0, prop = (elf_property*)elf_begin_list(actor->properties); prop;
		prop = (elf_property*)elf_next_in_list(actor->properties), i++)
	{
		if(i == idx)
		{
			elf_remove_from_list(actor->properties, (elf_object*)prop);
			return ELF_TRUE;
		}
	}

	return ELF_FALSE;
}

unsigned char elf_remove_actor_property_by_object(elf_actor *actor, elf_property *property)
{
	return elf_remove_from_list(actor->properties, (elf_object*)property);
}

void elf_remove_actor_properties(elf_actor *actor)
{
	elf_dec_ref((elf_object*)actor->properties);
	actor->properties = elf_create_list();
	elf_inc_ref((elf_object*)actor->properties);
}

void elf_set_actor_selected(elf_actor *actor, unsigned char selected)
{
	actor->selected = !selected == ELF_FALSE;
}

unsigned char elf_get_actor_selected(elf_actor *actor)
{
	return actor->selected;
}

