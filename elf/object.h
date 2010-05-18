
void elf_init_objects()
{
	memset(global_ref_count_table, 0x0, sizeof(int)*ELF_OBJECT_TYPE_COUNT);
}

void elf_deinit_objects()
{
	if(elf_get_global_ref_count() > 0)
	{
		elf_write_to_log("error: possible memory leak in ELF, [%d] references not dereferenced\n",
			elf_get_global_ref_count());
		elf_log_ref_count_table();
	}

	if(elf_get_global_ref_count() < 0)
	{
		elf_write_to_log("error: possible double free in ELF, [%d] negative reference count\n",
			elf_get_global_ref_count());
		elf_log_ref_count_table();
	}

	if(elf_get_global_obj_count() > 0)
		elf_write_to_log("error: possible memory leak in ELF, [%d] objects not destroyed\n",
			elf_get_global_obj_count());

	if(elf_get_global_obj_count() < 0)
		elf_write_to_log("error: possible double free in ELF, [%d] negative object count\n",
			elf_get_global_obj_count());
}

void elf_log_ref_count_table()
{
	int i;

	elf_write_to_log("---------- REF COUNT TABLE ----------\n");

	for(i = 0; i < ELF_OBJECT_TYPE_COUNT; i++)
	{
		elf_write_to_log("%d : %d\n", i, global_ref_count_table[i]);
	}

	elf_write_to_log("-------------------------------------\n");
}

void elf_inc_ref(elf_object *obj)
{
	global_ref_count++;
	global_ref_count_table[obj->type]++;
	obj->ref_count++;
}

void elf_dec_ref(elf_object *obj)
{
	global_ref_count--;
	global_ref_count_table[obj->type]--;
	obj->ref_count--;

	if(obj->ref_count < 1)
	{
		switch(obj->type)
		{
			case ELF_KEY_EVENT: elf_destroy_key_event((elf_key_event*)obj); break;
			case ELF_CHAR_EVENT: elf_destroy_char_event((elf_char_event*)obj); break;
			case ELF_CONTEXT: elf_destroy_context((elf_context*)obj); break;
			case ELF_ENGINE: elf_destroy_engine((elf_engine*)obj); break;
			case ELF_TIMER: elf_destroy_timer((elf_timer*)obj); break;
			case ELF_IMAGE: elf_destroy_image((elf_image*)obj); break;
			case ELF_TEXTURE: elf_destroy_texture((elf_texture*)obj); break;
			case ELF_MATERIAL: elf_destroy_material((elf_material*)obj); break;
			case ELF_CAMERA: elf_destroy_camera((elf_camera*)obj); break;
			case ELF_MODEL: elf_destroy_model((elf_model*)obj); break;
			case ELF_ENTITY: elf_destroy_entity((elf_entity*)obj); break;
			case ELF_LIGHT: elf_destroy_light((elf_light*)obj); break;
			case ELF_SCENE: elf_destroy_scene((elf_scene*)obj); break;
			case ELF_PAK_INDEX: elf_destroy_pak_index((elf_pak_index*)obj); break;
			case ELF_PAK: elf_destroy_pak((elf_pak*)obj); break;
			case ELF_POST_PROCESS: elf_destroy_post_process((elf_post_process*)obj); break;
			case ELF_SCRIPT: elf_destroy_script((elf_script*)obj); break;
			case ELF_AUDIO_DEVICE: elf_destroy_audio_device((elf_audio_device*)obj); break;
			case ELF_AUDIO_SOURCE: elf_destroy_audio_source((elf_audio_source*)obj); break;
			case ELF_SOUND: elf_destroy_sound((elf_sound*)obj); break;
			case ELF_BONE: elf_destroy_bone((elf_bone*)obj); break;
			case ELF_ARMATURE: elf_destroy_armature((elf_armature*)obj); break;
			case ELF_STRING: elf_destroy_string_object((elf_string*)obj); break;
			case ELF_FONT: elf_destroy_font((elf_font*)obj); break;
			case ELF_LABEL: elf_destroy_label((elf_label*)obj); break;
			case ELF_BUTTON: elf_destroy_button((elf_button*)obj); break;
			case ELF_PICTURE: elf_destroy_picture((elf_picture*)obj); break;
			case ELF_TEXT_FIELD: elf_destroy_text_field((elf_text_field*)obj); break;
			case ELF_SLIDER: elf_destroy_slider((elf_slider*)obj); break;
			case ELF_SCREEN: elf_destroy_screen((elf_screen*)obj); break;
			case ELF_TEXT_LIST: elf_destroy_text_list((elf_text_list*)obj); break;
			case ELF_CHECK_BOX: elf_destroy_check_box((elf_check_box*)obj); break;
			case ELF_GUI: elf_destroy_gui((elf_gui*)obj); break;
			case ELF_DIRECTORY: elf_destroy_directory((elf_directory*)obj); break;
			case ELF_DIRECTORY_ITEM: elf_destroy_directory_item((elf_directory_item*)obj); break;
			case ELF_COLLISION: elf_destroy_collision((elf_collision*)obj); break;
			case ELF_PHYSICS_OBJECT: elf_destroy_physics_object((elf_physics_object*)obj); break;
			case ELF_PHYSICS_WORLD: elf_destroy_physics_world((elf_physics_world*)obj); break;
			case ELF_JOINT: elf_destroy_joint((elf_joint*)obj); break;
			case ELF_RESOURCES: elf_destroy_resources((elf_resources*)obj); break;
			case ELF_PARTICLE: elf_destroy_particle((elf_particle*)obj); break;
			case ELF_PARTICLES: elf_destroy_particles((elf_particles*)obj); break;
			case ELF_BEZIER_POINT: elf_destroy_bezier_point((elf_bezier_point*)obj); break;
			case ELF_BEZIER_CURVE: elf_destroy_bezier_curve((elf_bezier_curve*)obj); break;
			case ELF_IPO: elf_destroy_ipo((elf_ipo*)obj); break;
			case ELF_FRAME_PLAYER: elf_destroy_frame_player((elf_frame_player*)obj); break;
			case ELF_PROPERTY: elf_destroy_property((elf_property*)obj); break;
			case ELF_SCRIPTING: elf_destroy_scripting((elf_scripting*)obj); break;
			case ELF_PHYSICS_TRI_MESH: elf_destroy_physics_tri_mesh((elf_physics_tri_mesh*)obj); break;
			case ELF_LIST: elf_destroy_list((elf_list*)obj); break;
			default: elf_set_error(ELF_UNKNOWN_TYPE, "error: can not destroy unknown type\n"); break;
		}
	}
}

int elf_get_object_type(elf_object *obj)
{
	return obj->type;
}

int elf_get_object_ref_count(elf_object *obj)
{
	return obj->ref_count;
}

int elf_get_global_ref_count()
{
	return global_ref_count;
}

int elf_get_global_obj_count()
{
	return global_obj_count;
}

unsigned char elf_is_actor(elf_object *obj)
{
	if(obj->type == ELF_CAMERA || obj->type == ELF_ENTITY ||
		obj->type == ELF_LIGHT || obj->type == ELF_ARMATURE ||
		obj->type == ELF_BONE || obj->type == ELF_PARTICLES) return ELF_TRUE;
	return ELF_FALSE;
}

unsigned char elf_is_gui_object(elf_object *obj)
{
	if(obj->type == ELF_GUI || obj->type == ELF_LABEL || obj->type == ELF_BUTTON ||
		obj->type == ELF_PICTURE || obj->type == ELF_TEXT_FIELD ||
		obj->type == ELF_SLIDER || obj->type == ELF_SCREEN ||
		obj->type == ELF_TEXT_LIST || obj->type == ELF_CHECK_BOX) return ELF_TRUE;
	return ELF_FALSE;
}

