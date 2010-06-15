
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
	actor->sources = elf_create_list();
	actor->properties = elf_create_list();

	elf_inc_ref((elf_object*)actor->joints);
	elf_inc_ref((elf_object*)actor->sources);
	elf_inc_ref((elf_object*)actor->properties);

	actor->ipo = elf_create_ipo();
	elf_inc_ref((elf_object*)actor->ipo);

	actor->ipo_player = elf_create_frame_player();
	elf_inc_ref((elf_object*)actor->ipo_player);
	elf_set_frame_player_user_data(actor->ipo_player, actor);
	elf_set_frame_player_callback(actor->ipo_player, elf_actor_ipo_callback);

	actor->pbb_lengths.x = actor->pbb_lengths.y = actor->pbb_lengths.z = 1.0;
	actor->shape = ELF_NONE;
	actor->mass = 0.0;
	actor->lin_damp = 0.0;
	actor->ang_damp = 0.0;
	actor->lin_sleep = 0.8;
	actor->ang_sleep = 1.0;
	actor->restitution = 0.0;
	actor->anis_fric.x = actor->anis_fric.y = actor->anis_fric.z = 1.0;
	actor->lin_factor.x = actor->lin_factor.y = actor->lin_factor.z = 1.0;
	actor->ang_factor.x = actor->ang_factor.y = actor->ang_factor.z = 1.0;

	actor->moved = ELF_TRUE;
}

void elf_update_actor(elf_actor *actor)
{
	static float oposition[3];
	static float oorient[4];
	static float position[3];
	static float orient[4];
	static elf_audio_source *source;

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

	if(actor->script && actor->scene->run_scripts)
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

void elf_actor_pre_draw(elf_actor *actor)
{
}

void elf_actor_post_draw(elf_actor *actor)
{
	actor->moved = ELF_FALSE;
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

	for(joint = (elf_joint*)elf_begin_list(actor->joints); joint;
		joint = (elf_joint*)elf_next_in_list(actor->joints))
	{
		if(elf_get_joint_actor_a(joint) == actor)
			elf_remove_from_list(elf_get_joint_actor_b(joint)->joints, (elf_object*)joint);
		else elf_remove_from_list(elf_get_joint_actor_a(joint)->joints, (elf_object*)joint);
		elf_clear_joint(joint);
	}

	elf_dec_ref((elf_object*)actor->joints);
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
	actor->moved = ELF_TRUE;

	gfx_set_transform_position(actor->transform, x, y, z);

	if(actor->object) elf_set_physics_object_position(actor->object, x, y, z);
	if(actor->dobject) elf_set_physics_object_position(actor->dobject, x, y, z);

	if(actor->type == ELF_LIGHT) elf_set_actor_position((elf_actor*)((elf_light*)actor)->shadow_camera, x, y, z);
}

void elf_set_actor_rotation(elf_actor *actor, float x, float y, float z)
{
	float orient[4];

	actor->moved = ELF_TRUE;

	gfx_set_transform_rotation(actor->transform, x, y, z);
	gfx_get_transform_orientation(actor->transform, orient);

	if(actor->object) elf_set_physics_object_orientation(actor->object, orient[0], orient[1], orient[2], orient[3]);
	if(actor->dobject) elf_set_physics_object_orientation(actor->dobject, orient[0], orient[1], orient[2], orient[3]);

	if(actor->type == ELF_LIGHT) elf_set_actor_rotation((elf_actor*)((elf_light*)actor)->shadow_camera, x, y, z);
}

void elf_set_actor_orientation(elf_actor *actor, float x, float y, float z, float w)
{
	actor->moved = ELF_TRUE;

	gfx_set_transform_orientation(actor->transform, x, y, z, w);

	if(actor->object) elf_set_physics_object_orientation(actor->object, x, y, z, w);
	if(actor->dobject) elf_set_physics_object_orientation(actor->dobject, x, y, z, w);

	if(actor->type == ELF_LIGHT) elf_set_actor_orientation((elf_actor*)((elf_light*)actor)->shadow_camera, x, y, z, w);
}

void elf_rotate_actor(elf_actor *actor, float x, float y, float z)
{
	float orient[4];

	actor->moved = ELF_TRUE;

	gfx_rotate_transform(actor->transform, x*eng->sync, y*eng->sync, z*eng->sync);
	gfx_get_transform_orientation(actor->transform, orient);

	if(actor->object) elf_set_physics_object_orientation(actor->object, orient[0], orient[1], orient[2], orient[3]);
	if(actor->dobject) elf_set_physics_object_orientation(actor->dobject, orient[0], orient[1], orient[2], orient[3]);

	if(actor->type == ELF_LIGHT) elf_rotate_actor((elf_actor*)((elf_light*)actor)->shadow_camera, x, y, z);
}

void elf_rotate_actor_local(elf_actor *actor, float x, float y, float z)
{
	float orient[4];

	actor->moved = ELF_TRUE;

	gfx_rotate_transform_local(actor->transform, x*eng->sync, y*eng->sync, z*eng->sync);
	gfx_get_transform_orientation(actor->transform, orient);

	if(actor->object) elf_set_physics_object_orientation(actor->object, orient[0], orient[1], orient[2], orient[3]);
	if(actor->dobject) elf_set_physics_object_orientation(actor->dobject, orient[0], orient[1], orient[2], orient[3]);

	if(actor->type == ELF_LIGHT) elf_rotate_actor_local((elf_actor*)((elf_light*)actor)->shadow_camera, x, y, z);
}

void elf_move_actor(elf_actor *actor, float x, float y, float z)
{
	float position[3];

	actor->moved = ELF_TRUE;

	gfx_move_transform(actor->transform, x*eng->sync, y*eng->sync, z*eng->sync);
	gfx_get_transform_position(actor->transform, position);

	if(actor->object) elf_set_physics_object_position(actor->object, position[0], position[1], position[2]);
	if(actor->dobject) elf_set_physics_object_position(actor->dobject, position[0], position[1], position[2]);

	if(actor->type == ELF_LIGHT) elf_move_actor((elf_actor*)((elf_light*)actor)->shadow_camera, x, y, z);
}

void elf_move_actor_local(elf_actor *actor, float x, float y, float z)
{
	float position[3];

	actor->moved = ELF_TRUE;

	gfx_move_transform_local(actor->transform, x*eng->sync, y*eng->sync, z*eng->sync);
	gfx_get_transform_position(actor->transform, position);

	if(actor->object) elf_set_physics_object_position(actor->object, position[0], position[1], position[2]);
	if(actor->dobject) elf_set_physics_object_position(actor->dobject, position[0], position[1], position[2]);

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

void elf_set_actor_bounding_lengths(elf_actor *actor, float x, float y, float z)
{
	actor->pbb_lengths.x = x;
	actor->pbb_lengths.y = y;
	actor->pbb_lengths.z = z;

	if(actor->object)
	{
		elf_set_actor_physics(actor, elf_get_actor_shape(actor),
			elf_get_actor_mass(actor));
	}
}

void elf_set_actor_bounding_offset(elf_actor *actor, float x, float y, float z)
{
	actor->pbb_offset.x = x;
	actor->pbb_offset.y = y;
	actor->pbb_offset.z = z;

	if(actor->object)
	{
		elf_set_actor_physics(actor, elf_get_actor_shape(actor),
			elf_get_actor_mass(actor));
	}
}

void elf_set_actor_physics(elf_actor *actor, int shape, float mass)
{
	float position[3];
	float orient[4];
	float scale[3];
	float radius;
	elf_joint *joint;
	elf_entity *entity;

	elf_disable_actor_physics(actor);

	actor->shape = (unsigned char)shape;
	actor->mass = mass;
	actor->physics = ELF_TRUE;

	switch(shape)
	{
		case ELF_BOX:
		{
			actor->object = elf_create_physics_object_box(actor->pbb_lengths.x/2.0,
				actor->pbb_lengths.y/2.0, actor->pbb_lengths.z/2.0, mass,
				actor->pbb_offset.x, actor->pbb_offset.y, actor->pbb_offset.z);
			break;
		}
		case ELF_SPHERE:
		{
			if(actor->pbb_lengths.x > actor->pbb_lengths.y && actor->pbb_lengths.x > actor->pbb_lengths.z)
				radius = actor->pbb_lengths.x/2.0;
			else if(actor->pbb_lengths.y > actor->pbb_lengths.x && actor->pbb_lengths.y > actor->pbb_lengths.z)
				radius = actor->pbb_lengths.y/2.0;
			else  radius = actor->pbb_lengths.z/2.0;

			actor->object = elf_create_physics_object_sphere(radius, mass,
				actor->pbb_offset.x, actor->pbb_offset.y, actor->pbb_offset.z);
			break;
		}
		case ELF_MESH:
		{
			if(actor->type != ELF_ENTITY) return;
			entity = (elf_entity*)actor;
			if(!entity->model || !elf_get_model_indices(entity->model)) return;
			if(!entity->model->tri_mesh)
			{
				entity->model->tri_mesh = elf_create_physics_tri_mesh(
					elf_get_model_vertices(entity->model),
					elf_get_model_indices(entity->model),
					elf_get_model_indice_count(entity->model));
				elf_inc_ref((elf_object*)entity->model->tri_mesh);
			}
			actor->object = elf_create_physics_object_mesh(entity->model->tri_mesh, mass);
			break;
		}
		case ELF_CAPSULE:
		{
			if(actor->pbb_lengths.x > actor->pbb_lengths.y) radius = actor->pbb_lengths.x/2.0;
			else radius = actor->pbb_lengths.y/2.0;

			actor->object = elf_create_physics_object_capsule(actor->pbb_lengths.z, radius, mass,
				actor->pbb_offset.x, actor->pbb_offset.y, actor->pbb_offset.z);
			break;
		}
		default: return;
	}

	elf_set_physics_object_actor(actor->object, (elf_actor*)actor);
	elf_inc_ref((elf_object*)actor->object);

	gfx_get_transform_position(actor->transform, position);
	gfx_get_transform_orientation(actor->transform, orient);
	gfx_get_transform_scale(actor->transform, scale);

	elf_set_physics_object_position(actor->object, position[0], position[1], position[2]);
	elf_set_physics_object_orientation(actor->object, orient[0], orient[1], orient[2], orient[3]);
	elf_set_physics_object_scale(actor->object, scale[0], scale[1], scale[2]);

	elf_set_physics_object_damping(actor->object, actor->lin_damp, actor->ang_damp);
	elf_set_physics_object_sleep_thresholds(actor->object, actor->lin_sleep, actor->ang_sleep);
	elf_set_physics_object_restitution(actor->object, actor->restitution);
	elf_set_physics_object_anisotropic_friction(actor->object, actor->anis_fric.x, actor->anis_fric.y, actor->anis_fric.z);
	elf_set_physics_object_linear_factor(actor->object, actor->lin_factor.x, actor->lin_factor.y, actor->lin_factor.z);
	elf_set_physics_object_angular_factor(actor->object, actor->ang_factor.x, actor->ang_factor.y, actor->ang_factor.z);

	if(actor->scene) elf_set_physics_object_world(actor->object, actor->scene->world);

	// things are seriously going to blow up if we don't update the joints
	// when a new physics object has been created
	for(joint = (elf_joint*)elf_begin_list(actor->joints); joint;
		joint = (elf_joint*)elf_next_in_list(actor->joints))
	{
		elf_set_joint_world(joint, NULL);
		if(actor->scene) elf_set_joint_world(joint, actor->scene->world);
	}
}

unsigned char elf_is_actor_physics(elf_actor *actor)
{
	return actor->physics;
}

void elf_disable_actor_physics(elf_actor *actor)
{
	if(actor->object)
	{
		elf_set_physics_object_actor(actor->object, NULL);
		elf_set_physics_object_world(actor->object, NULL);
		elf_dec_ref((elf_object*)actor->object);
		actor->object = NULL;
	}

	actor->physics = ELF_FALSE;
}

void elf_set_actor_damping(elf_actor *actor, float lin_damp, float ang_damp)
{
	actor->lin_damp = lin_damp;
	actor->ang_damp = ang_damp;
	if(actor->object) elf_set_physics_object_damping(actor->object, lin_damp, ang_damp);
}

void elf_set_actor_sleep_thresholds(elf_actor *actor, float lin_thrs, float ang_thrs)
{
	actor->lin_sleep = lin_thrs;
	actor->ang_sleep = ang_thrs;
	if(actor->object) elf_set_physics_object_sleep_thresholds(actor->object, lin_thrs, ang_thrs);
}

void elf_set_actor_restitution(elf_actor *actor, float restitution)
{
	actor->restitution = restitution;
	if(actor->object) elf_set_physics_object_restitution(actor->object, restitution);
}

void elf_set_actor_anisotropic_friction(elf_actor *actor, float x, float y, float z)
{
	actor->anis_fric.x = x; actor->anis_fric.y = y; actor->anis_fric.z = z;
	if(actor->object) elf_set_physics_object_anisotropic_friction(actor->object, x, y, z);
}

void elf_set_actor_linear_factor(elf_actor *actor, float x, float y, float z)
{
	actor->lin_factor.x = x; actor->lin_factor.y = y; actor->lin_factor.z = z;
	if(actor->object) elf_set_physics_object_linear_factor(actor->object, x, y, z);
}

void elf_set_actor_angular_factor(elf_actor *actor, float x, float y, float z)
{
	actor->ang_factor.x = x; actor->ang_factor.y = y; actor->ang_factor.z = z;
	if(actor->object) elf_set_physics_object_angular_factor(actor->object, x, y, z);
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

elf_vec3f elf_get_actor_bounding_lengths(elf_actor *actor)
{
	return actor->pbb_lengths;
}

elf_vec3f elf_get_actor_bounding_offset(elf_actor *actor)
{
	return actor->pbb_offset;
}

int elf_get_actor_shape(elf_actor *actor)
{
	return actor->shape;
}

float elf_get_actor_mass(elf_actor *actor)
{
	return actor->mass;
}

float elf_get_actor_linear_damping(elf_actor *actor)
{
	return actor->lin_damp;
}

float elf_get_actor_angular_damping(elf_actor *actor)
{
	return actor->ang_damp;
}

float elf_get_actor_linear_sleep_threshold(elf_actor *actor)
{
	return actor->lin_sleep;
}

float elf_get_actor_angular_sleep_threshold(elf_actor *actor)
{
	return actor->ang_sleep;
}

float elf_get_actor_restitution(elf_actor *actor)
{
	return actor->restitution;
}

elf_vec3f elf_get_actor_anisotropic_friction(elf_actor *actor)
{
	return actor->anis_fric;
}

elf_vec3f elf_get_actor_linear_factor(elf_actor *actor)
{
	return actor->lin_factor;
}

elf_vec3f elf_get_actor_angular_factor(elf_actor *actor)
{
	return actor->ang_factor;
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

void elf_direct_actor_at(elf_actor *actor, elf_vec3f at, int axis)
{
}

void elf_set_actor_selected(elf_actor *actor, unsigned char selected)
{
	actor->selected = !selected == ELF_FALSE;
}

unsigned char elf_get_actor_selected(elf_actor *actor)
{
	return actor->selected;
}

