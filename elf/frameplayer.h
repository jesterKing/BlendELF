
elf_frame_player *elf_create_frame_player()
{
	elf_frame_player *player;

	player = (elf_frame_player*)malloc(sizeof(elf_frame_player));
	memset(player, 0x0, sizeof(elf_frame_player));
	player->type = ELF_FRAME_PLAYER;

	player->cur_frame = 1.0;

	global_obj_count++;
	
	return player;
}

void elf_destroy_frame_player(elf_frame_player *player)
{
	free(player);

	global_obj_count--;
}

void elf_update_frame_player(elf_frame_player *player)
{
	if(player->end > 1.0 && !player->pause)
	{
		player->cur_frame += player->speed*elf_get_sync();
		if(player->loop)
		{
			if(player->cur_frame > player->end+1) elf_set_frame_player_frame(player, player->start);
			else elf_set_frame_player_frame(player, player->cur_frame);
		}
		else
		{
			if(player->cur_frame > player->end)
			{
				elf_set_frame_player_frame(player, player->cur_frame);
				elf_stop_frame_player(player);
			}
			else elf_set_frame_player_frame(player, player->cur_frame);
		}
	}
}

void elf_set_frame_player_frame(elf_frame_player *player, float frame)
{
	player->cur_frame = frame;
	if(player->cur_frame < 1.0) player->cur_frame = 1.0;

	if(elf_is_frame_player_playing(player) || elf_is_frame_player_paused(player))
	{
		if(player->cur_frame < player->start) player->cur_frame = player->start;
		if(player->loop)
		{
			if(player->cur_frame > player->end+1) player->cur_frame = player->start;
		}
		else
		{
			if(player->cur_frame > player->end) player->cur_frame = player->end;
		}
	}

	if(player->callback) player->callback(player);
}

void elf_play_frame_player(elf_frame_player *player, float start, float end, float speed)
{
	player->start = start;
	player->end = end;
	if(player->start < 1.0) player->start = 1.0;
	if(player->end < player->start) player->end = player->start;
	player->cur_frame = player->start;
	player->speed = speed;
	player->loop = ELF_FALSE;
	player->pause = ELF_FALSE;

	elf_set_frame_player_frame(player, player->cur_frame);
}

void elf_loop_frame_player(elf_frame_player *player, float start, float end, float speed)
{
	player->start = start;
	player->end = end;
	player->cur_frame = start;
	player->speed = speed;
	player->pause = ELF_FALSE;
	player->loop = ELF_TRUE;

	elf_set_frame_player_frame(player, player->cur_frame);
}

void elf_stop_frame_player(elf_frame_player *player)
{
	player->start = 0.0f;
	player->end = 0.0f;
	player->speed = 0.0f;
	player->pause = ELF_FALSE;
	player->loop = ELF_FALSE;
}

void elf_pause_frame_player(elf_frame_player *player)
{
	if(player->end > 0.99999) player->pause = ELF_TRUE;
}

void elf_resume_frame_player(elf_frame_player *player)
{
	if(player->end > 0.99999) player->pause = ELF_FALSE;
}

float elf_get_frame_player_start(elf_frame_player *player)
{
	return player->start;
}

float elf_get_frame_player_end(elf_frame_player *player)
{
	return player->end;
}

float elf_get_frame_player_speed(elf_frame_player *player)
{
	return player->speed;
}

float elf_get_frame_player_frame(elf_frame_player *player)
{
	return player->cur_frame;
}

unsigned char elf_is_frame_player_playing(elf_frame_player *player)
{
	if(player->end > 0.99999 && !player->pause) return ELF_TRUE;
	return ELF_FALSE;
}

unsigned char elf_is_frame_player_paused(elf_frame_player *player)
{
	if(player->pause) return ELF_TRUE;
	return ELF_FALSE;
}

void elf_set_frame_player_user_data(elf_frame_player *player, void *user_data)
{
	player->user_data = user_data;
}

void* elf_get_frame_player_user_data(elf_frame_player *player)
{
	return player->user_data;
}

void elf_set_frame_player_callback(elf_frame_player *player, void (*callback)(elf_frame_player*))
{
	player->callback = callback;
}

