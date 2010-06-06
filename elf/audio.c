
#include "default.h"

#include "gfx.h"
#include "blendelf.h"
#include "types.h"

#define ELF_AUDIO_STREAM_CHUNK_SIZE 4096*8

typedef struct elf_data_chunk			elf_data_chunk;
typedef struct elf_data_dump			elf_data_dump;

struct elf_data_chunk {
	elf_data_chunk *next;
	int length;
	void *data;
};

struct elf_data_dump {
	elf_data_chunk *first;
	int offset;
};

elf_data_chunk* elf_create_data_chunk()
{
	elf_data_chunk *chunk;

	chunk = (elf_data_chunk*)malloc(sizeof(elf_data_chunk));
	memset(chunk, 0x0, sizeof(elf_data_chunk));

	return chunk;
}

void elf_destroy_data_chunk(elf_data_chunk *chunk)
{
	if(chunk->next) elf_destroy_data_chunk(chunk->next);
	if(chunk->data) free(chunk->data);
	free(chunk);
}

elf_data_dump* elf_create_data_dump()
{
	elf_data_dump *dump;

	dump = (elf_data_dump*)malloc(sizeof(elf_data_dump));
	memset(dump, 0x0, sizeof(elf_data_dump));

	return dump;
}

void elf_destroy_data_dump(elf_data_dump *dump)
{
	if(dump->first) elf_destroy_data_chunk(dump->first);
	free(dump);
}

int elf_get_data_dump_length(elf_data_dump *dump)
{
	elf_data_chunk *chk;
	int length;

	length = 0;

	chk = dump->first;
	while(chk)
	{
		length += chk->length;
		chk = chk->next;
	}

	return length;
}

void elf_add_chunk_to_data_dump(elf_data_dump *dump, void *data, int length)
{
	elf_data_chunk *chunk;
	elf_data_chunk *chk;

	chunk = elf_create_data_chunk();
	chunk->length = length;
	chunk->data = malloc(length);
	memcpy(chunk->data, data, length);

	if(!dump->first)
	{
		dump->first = chunk;
	}
	else
	{
		chk = dump->first;
		while(chk->next) chk = chk->next;
		chk->next = chunk;
	}
}

void elf_data_dump_to_buffer(elf_data_dump *dump, void *buf)
{
	int offset;
	elf_data_chunk *chk;

	offset = 0;

	chk = dump->first;
	while(chk)
	{
		memcpy(&((char*)buf)[offset], chk->data, chk->length);
		offset += chk->length;
		chk = chk->next;
	}
}

struct elf_audio_device {
	ELF_OBJECT_HEADER;
	ALCdevice *device;
	ALCcontext *context;
	float volume;
	float rolloff;
	elf_list *sources;
};

struct elf_audio_source {
	ELF_OBJECT_HEADER;
	elf_sound *sound;
	ALuint source;
	unsigned char loop;
	unsigned char paused;
};

struct elf_sound {
	ELF_OBJECT_HEADER;
	char *file_path;
	unsigned char file_type;
	ALuint buffer[3];
	int freq;
	int format;

	unsigned char streamed;
	unsigned char streaming;
	unsigned char eof;
	OggVorbis_File ogg_file;
	FILE *file;
	int length;
	int position;
	unsigned char oldest_buffer;
};

elf_audio_device *audio_device = NULL;

elf_audio_device* elf_create_audio_device()
{
	elf_audio_device *device;

	device = (elf_audio_device*)malloc(sizeof(elf_audio_device));
	memset(device, 0x0, sizeof(elf_audio_device));
	device->type = ELF_AUDIO_DEVICE;

	device->volume = 1.0;
	device->rolloff = 1.0;

	device->sources = elf_create_list();
	elf_inc_ref((elf_object*)device->sources);

	return device;
}

void elf_destroy_audio_device(elf_audio_device *device)
{
	if(device->sources) elf_dec_ref((elf_object*)device->sources);

	if(device->device)
	{
		alcMakeContextCurrent(NULL);
		alcDestroyContext(device->context);
		alcCloseDevice(device->device);
	}

	free(device);
}

unsigned char elf_init_audio()
{
	if(audio_device)
	{
		elf_write_to_log("warning: cannot initialize audio twice\n");
		return ELF_TRUE;
	}

	audio_device = elf_create_audio_device();
	elf_inc_ref((elf_object*)audio_device);

	audio_device->device = alcOpenDevice(NULL);
	if(!audio_device->device)
	{
		elf_write_to_log("warning: could not create audio device\n");
		elf_deinit_audio();
		return ELF_FALSE;
	}

	audio_device->context = alcCreateContext(audio_device->device, NULL);
	if(!audio_device->context)
	{
		elf_write_to_log("warning: could not create audio context\n");
		alcCloseDevice(audio_device->device);
		audio_device->device = NULL;
		elf_deinit_audio();
		return ELF_FALSE;
	}

	alcMakeContextCurrent(audio_device->context);

	alGetError();

	alListener3f(AL_POSITION, 0.0, 0.0, 0.0);
	alListener3f(AL_DIRECTION, 0.0, 0.0, -1.0);

	alListenerf(AL_GAIN, 1.0);

	return ELF_TRUE;
}

void elf_deinit_audio()
{
	if(!audio_device) return;

	elf_dec_ref((elf_object*)audio_device);

	audio_device = NULL;
}

void elf_update_audio()
{
	elf_audio_source *source;

	for(source = (elf_audio_source*)elf_begin_list(audio_device->sources); source;
		source = (elf_audio_source*)elf_next_in_list(audio_device->sources))
	{
		if(source->sound->streamed && !source->paused)
		{
			elf_stream_audio_source(source);
		}

		if(elf_get_object_ref_count((elf_object*)source) < 2 &&
			!elf_is_sound_playing(source) &&
			!elf_is_sound_paused(source))
		{
			if(source->sound->streamed)
			{
				if(source->sound->eof && !source->loop) elf_remove_from_list(audio_device->sources, (elf_object*)source);
			}
			else
			{
				elf_remove_from_list(audio_device->sources, (elf_object*)source);
			}
		}
	}
}

void elf_set_audio_volume(float volume)
{
	if(!audio_device) return;
	alListenerf(AL_GAIN, volume);
}

float elf_get_audio_volume()
{
	if(!audio_device) return 0.0;
	return audio_device->volume;
}

void elf_set_audio_rolloff(float rolloff)
{
	if(!audio_device) return;
	audio_device->rolloff = rolloff;
}

float elf_get_audio_rolloff()
{
	if(!audio_device) return 0.0;
	return audio_device->rolloff;
}

void elf_set_audio_listener_position(float x, float y, float z)
{
	if(!audio_device) return;
	alListener3f(AL_POSITION, x, y, z);
}

void elf_set_audio_listener_orientation(float *params)
{
	if(!audio_device) return;
	alListenerfv(AL_ORIENTATION, params);
}

elf_sound* elf_create_sound()
{
	elf_sound *sound;

	sound = (elf_sound*)malloc(sizeof(elf_sound));
	memset(sound, 0x0, sizeof(elf_sound));
	sound->type = ELF_SOUND;

	return sound;
}

void elf_destroy_sound(elf_sound *sound)
{
	if(sound->file_path) elf_destroy_string(sound->file_path);
	if(!sound->streamed)
	{
		if(sound->buffer[0]) alDeleteBuffers(1, &sound->buffer[0]);
	}
	else
	{
		alDeleteBuffers(3, sound->buffer);
		if(sound->file_type == ELF_OGG) ov_clear(&sound->ogg_file);
		if(sound->file_type == ELF_WAV) fclose(sound->file);
	}
	free(sound);
}

unsigned char elf_init_sound_with_ogg(elf_sound *snd, const char *file_path)
{
	vorbis_info *info = NULL;

	if(snd->streamed && snd->file_type == ELF_OGG)
	{
		ov_clear(&snd->ogg_file);
		snd->file = NULL;
	}
	if(snd->streamed && snd->file_type == ELF_WAV)
	{
		fclose(snd->file);
		snd->file = NULL;
	}

	snd->file_type = ELF_NONE;
	snd->length = 0;
	snd->position = 0;
	snd->eof = ELF_FALSE;

	snd->file = fopen(file_path, "rb");
	if(!snd->file)
	{
		elf_set_error(ELF_CANT_OPEN_FILE, "error: can't open file \"%s\"\n", file_path);
		return ELF_FALSE;
	}

	snd->file_type = ELF_OGG;

	ov_open(snd->file, &snd->ogg_file, NULL, 0);
	info = ov_info(&snd->ogg_file, -1);

	if(info->channels == 1) snd->format = AL_FORMAT_MONO16;
	else if(info->channels == 2) snd->format = AL_FORMAT_STEREO16;
	else
	{
		elf_set_error(ELF_CANT_OPEN_FILE, "error: invalid number of channels in \"%s\"\n", file_path);
		ov_clear(&snd->ogg_file);
		return ELF_FALSE;
	}

	snd->freq = info->rate;

	return ELF_TRUE;
}

unsigned char elf_init_sound_with_wav(elf_sound *snd, const char *file_path)
{
	char buf[ELF_AUDIO_STREAM_CHUNK_SIZE];

	char magic[5] = "\0\0\0\0";
	unsigned int chunk_length = 0;
	int junk_data = 0;
	unsigned short int audio_format = 0;
	unsigned short int channels = 0;
	unsigned int freq = 0;
	unsigned char fmt_found = ELF_FALSE;
	unsigned char data_found = ELF_FALSE;

	if(snd->streamed && snd->file_type == ELF_OGG)
	{
		ov_clear(&snd->ogg_file);
		snd->file = NULL;
	}
	if(snd->streamed && snd->file_type == ELF_WAV)
	{
		fclose(snd->file);
		snd->file = NULL;
	}

	snd->file_type = ELF_NONE;
	snd->length = 0;
	snd->position = 0;
	snd->eof = ELF_FALSE;

	snd->file = fopen(file_path, "rb");
	if(!snd->file)
	{
		elf_set_error(ELF_CANT_OPEN_FILE, "error: can't open file \"%s\"\n", file_path);
		return ELF_FALSE;
	}

	snd->file_type = ELF_WAV;

	fread(&magic, sizeof(int), 1, snd->file);
	if(strcmp((char*)&magic, "RIFF"))
	{
		elf_set_error(ELF_INVALID_FILE, "error: \"%s\" invalid wav file, RIFF not found\n", file_path);
		return ELF_FALSE;
	}

	fread(&chunk_length, sizeof(unsigned int), 1, snd->file);
	fread(&magic, sizeof(int), 1, snd->file);
	if(strcmp((char*)&magic, "WAVE"))
	{
		elf_set_error(ELF_INVALID_FILE, "error: \"%s\" invalid wav file, WAVE not found\n", file_path);
		return ELF_FALSE;
	}

	while(1)
	{
		if(fmt_found && data_found) break;

		if(feof(snd->file))
		{
			if(!fmt_found || !data_found)
			{
				elf_set_error(ELF_INVALID_FILE, "error: \"%s\" invalid wav file\n", file_path);
				return ELF_FALSE;
			}
			break;
		}

		fread(&magic, sizeof(int), 1, snd->file);
		fread(&chunk_length, sizeof(unsigned int), 1, snd->file);
		if(strcmp((char*)&magic, "fmt ") == 0)
		{
			fread(&audio_format, sizeof(unsigned short int), 1, snd->file);
			fread(&channels, sizeof(unsigned short int), 1, snd->file);
			fread(&freq, sizeof(unsigned int), 1, snd->file);
			fread(&junk_data, sizeof(unsigned int), 1, snd->file);
			fread(&junk_data, sizeof(unsigned short int), 1, snd->file);
			fread(&junk_data, sizeof(unsigned short int), 1, snd->file);

			if(chunk_length-16 > 0) fread(buf, 1, chunk_length-16, snd->file);

			if(audio_format != 1)
			{
				elf_set_error(ELF_INVALID_FILE, "error: \"%s\" invalid wav file, invalid audio format\n", file_path);
				return ELF_FALSE;
			}

			snd->freq = freq;

			if(channels == 1) snd->format = AL_FORMAT_MONO16;
			else if(channels == 2) snd->format = AL_FORMAT_STEREO16;
			else
			{
				elf_set_error(ELF_INVALID_FILE, "error: \"%s\" invalid wav file, invalid number of channels\n", file_path);
				return ELF_FALSE;
			}

			fmt_found = ELF_TRUE;
		}
		else if(strcmp((char*)&magic, "data") == 0)
		{
			snd->length = chunk_length;
			data_found = ELF_TRUE;
		}
	}

	return ELF_TRUE;
}

elf_sound* elf_load_sound(const char *file_path)
{
	elf_sound *snd = NULL;

	elf_data_dump *dump;
	char buf[ELF_AUDIO_STREAM_CHUNK_SIZE];
	int bytes_read = 0;
	char *data;

	int endian = 0;
	int bit_stream = 0;

	char *type = NULL;

	if(!audio_device) return NULL;

	snd = elf_create_sound();
	snd->file_path = elf_create_string(file_path);

	type = strrchr((char*)file_path, '.');

	if(strcmp(type, ".ogg") == 0)
	{
		if(!elf_init_sound_with_ogg(snd, snd->file_path))
		{
			elf_destroy_sound(snd);
			return NULL;
		}

		dump = elf_create_data_dump();
		do
		{
			bytes_read = ov_read(&snd->ogg_file, buf, ELF_AUDIO_STREAM_CHUNK_SIZE, endian, 2, 1, &bit_stream);
			elf_add_chunk_to_data_dump(dump, buf, bytes_read);
			snd->length += bytes_read;
		}
		while (bytes_read > 0);

		snd->length = elf_get_data_dump_length(dump);
		data = (char*)malloc(snd->length);
		elf_data_dump_to_buffer(dump, data);

		elf_destroy_data_dump(dump);
		ov_clear(&snd->ogg_file);
	}
	else if(strcmp(type, ".wav") == 0)
	{
		if(!elf_init_sound_with_wav(snd, snd->file_path))
		{
			elf_destroy_sound(snd);
			return NULL;
		}

		data = (char*)malloc(snd->length);
		fread(data, sizeof(char), snd->length, snd->file);

		fclose(snd->file);
		snd->file = NULL;
	}
	else
	{
		elf_set_error(ELF_UNKNOWN_FORMAT, "error: can't load \"%s\", unknown format\n", file_path);
		elf_destroy_sound(snd);
		return NULL;
	}

	alGenBuffers(1, &snd->buffer[0]);
	alBufferData(snd->buffer[0], snd->format, data, snd->length, snd->freq);

	free(data);

	return snd;
}

elf_sound* elf_load_streamed_sound(const char *file_path)
{
	elf_sound *snd = NULL;

	char *type = NULL;

	if(!audio_device) return NULL;

	snd = elf_create_sound();

	snd->file_path = elf_create_string(file_path);
	snd->streamed = ELF_TRUE;

	type = strrchr((char*)file_path, '.');

	if(strcmp(type, ".ogg") == 0)
	{
		if(!elf_init_sound_with_ogg(snd, snd->file_path))
		{
			elf_destroy_sound(snd);
			return NULL;
		}
	}
	else if(strcmp(type, ".wav") == 0)
	{
		if(!elf_init_sound_with_wav(snd, snd->file_path))
		{
			elf_destroy_sound(snd);
			return NULL;
		}
	}
	else
	{
		elf_set_error(ELF_UNKNOWN_FORMAT, "error: can't load \"%s\", unknown format\n", file_path);
		elf_destroy_sound(snd);
		return NULL;
	}

	alGenBuffers(3, snd->buffer);

	return snd;
}

int elf_get_sound_file_type(elf_sound *sound)
{
	return sound->file_type;
}

elf_audio_source* elf_play_sound(elf_sound *sound, float volume)
{
	elf_audio_source *source;

	if(!audio_device || sound->streaming) return NULL;

	source = elf_create_audio_source();

	// clear errors
	alGetError();

	source->sound = sound;
	elf_inc_ref((elf_object*)sound);

	alGenSources(1, &source->source);

	// check if everything went well
	if(alGetError() != AL_NO_ERROR)
	{
		elf_destroy_audio_source(source);
		return NULL;
	}

	if(!source->sound->streamed) alSourcei(source->source, AL_BUFFER, sound->buffer[0]);

	alSourcef(source->source, AL_GAIN, volume);
	alSourcef(source->source, AL_ROLLOFF_FACTOR, audio_device->rolloff);

	if(source->sound->streamed)
	{
		elf_stream_audio_source(source);
		source->sound->streaming = ELF_TRUE;
	}
	alSourcePlay(source->source);

	elf_append_to_list(audio_device->sources, (elf_object*)source);

	return source;
}

elf_audio_source* elf_play_entity_sound(elf_entity *entity, elf_sound *sound, float volume)
{
	elf_audio_source *source;
	float position[3];

	if(!audio_device || sound->streaming) return NULL;

	source = elf_create_audio_source();

	// clear errors
	alGetError();

	source->sound = sound;
	elf_inc_ref((elf_object*)sound);

	alGenSources(1, &source->source);

	// check if everything went well
	if(alGetError() != AL_NO_ERROR)
	{
		elf_destroy_audio_source(source);
		return NULL;
	}

	if(!source->sound->streamed) alSourcei(source->source, AL_BUFFER, sound->buffer[0]);

	alSourcef(source->source, AL_GAIN, volume);
	alSourcef(source->source, AL_ROLLOFF_FACTOR, audio_device->rolloff);

	elf_get_actor_position_((elf_actor*)entity, position);
	alSource3f(source->source, AL_POSITION, position[0], position[1], position[2]);

	if(source->sound->streamed)
	{
		elf_stream_audio_source(source);
		source->sound->streaming = ELF_TRUE;
	}
	alSourcePlay(source->source);

	elf_append_to_list(audio_device->sources, (elf_object*)source);
	elf_append_to_list(entity->sources, (elf_object*)source);

	return source;
}

elf_audio_source* elf_loop_sound(elf_sound *sound, float volume)
{
	elf_audio_source *source;

	if(!audio_device || sound->streaming) return NULL;

	source = elf_create_audio_source();

	// clear errors
	alGetError();

	source->loop = ELF_TRUE;
	source->sound = sound;
	elf_inc_ref((elf_object*)sound);

	alGenSources(1, &source->source);

	// check if everything went well
	if(alGetError() != AL_NO_ERROR)
	{
		elf_destroy_audio_source(source);
		return NULL;
	}

	if(!source->sound->streamed)
	{
		alSourcei(source->source, AL_BUFFER, sound->buffer[0]);
		alSourcei(source->source, AL_LOOPING, AL_TRUE);
	}

	alSourcef(source->source, AL_GAIN, volume);
	alSourcef(source->source, AL_ROLLOFF_FACTOR, audio_device->rolloff);

	if(source->sound->streamed)
	{
		elf_stream_audio_source(source);
		source->sound->streaming = ELF_TRUE;
	}
	alSourcePlay(source->source);

	elf_append_to_list(audio_device->sources, (elf_object*)source);

	return source;
}

elf_audio_source* elf_loop_entity_sound(elf_entity *entity, elf_sound *sound, float volume)
{
	elf_audio_source *source;
	float position[3];

	if(!audio_device || sound->streaming) return NULL;

	source = elf_create_audio_source();

	// clear errors
	alGetError();

	source->loop = ELF_TRUE;
	source->sound = sound;
	elf_inc_ref((elf_object*)sound);

	alGenSources(1, &source->source);

	// check if everything went well
	if(alGetError() != AL_NO_ERROR)
	{
		elf_destroy_audio_source(source);
		return NULL;
	}

	if(!source->sound->streamed)
	{
		alSourcei(source->source, AL_BUFFER, sound->buffer[0]);
		alSourcei(source->source, AL_LOOPING, AL_TRUE);
	}

	alSourcef(source->source, AL_GAIN, volume);
	alSourcef(source->source, AL_ROLLOFF_FACTOR, audio_device->rolloff);

	elf_get_actor_position_((elf_actor*)entity, position);
	alSource3f(source->source, AL_POSITION, position[0], position[1], position[2]);

	if(source->sound->streamed)
	{
		elf_stream_audio_source(source);
		source->sound->streaming = ELF_TRUE;
	}
	alSourcePlay(source->source);

	elf_append_to_list(audio_device->sources, (elf_object*)source);
	elf_append_to_list(entity->sources, (elf_object*)source);

	return source;
}

elf_audio_source* elf_create_audio_source()
{
	elf_audio_source *source;

	source = (elf_audio_source*)malloc(sizeof(elf_audio_source));
	memset(source, 0x0, sizeof(elf_audio_source));
	source->type = ELF_AUDIO_SOURCE;

	return source;
}

void elf_stream_audio_source(elf_audio_source *source)
{
	int queued;
	int processed;
	unsigned int buffer;

	char buf[ELF_AUDIO_STREAM_CHUNK_SIZE];
	int read;
	int bytes_read;
	int bytes_to_read;

	int endian = 0;
	int bit_stream = 0;

	if(!source->sound->eof)
	{
		if(!elf_is_sound_playing(source) && !elf_is_sound_paused(source))
		{
			elf_resume_sound(source);
		}
	}
	else
	{
		if(!source->loop)
		{
			return;
		}
	}

	alGetSourcei(source->source, AL_BUFFERS_QUEUED, &queued);
	alGetSourcei(source->source, AL_BUFFERS_PROCESSED, &processed);

	while(processed--) alSourceUnqueueBuffers(source->source, 1, &buffer);

	if(queued < 3)
	{
		bytes_read = 0;
		if(source->sound->file_type == ELF_OGG)
		{
			bytes_to_read = ELF_AUDIO_STREAM_CHUNK_SIZE;

			do
			{
				read = ov_read(&source->sound->ogg_file, &buf[bytes_read], ELF_AUDIO_STREAM_CHUNK_SIZE-bytes_read, endian, 2, 1, &bit_stream);
				if(read < 1)
				{
					if(source->loop) elf_init_sound_with_ogg(source->sound, source->sound->file_path);
					else source->sound->eof = ELF_TRUE;
					break;
				}
				bytes_read += read;
				bytes_to_read -= read;
			}
			while (bytes_to_read > 0);

			source->sound->position += bytes_read;
		}
		else if(source->sound->file_type == ELF_WAV)
		{
			if(source->sound->length-source->sound->position > ELF_AUDIO_STREAM_CHUNK_SIZE)
				bytes_to_read = ELF_AUDIO_STREAM_CHUNK_SIZE;
			else bytes_to_read = source->sound->length-source->sound->position;

			if(bytes_to_read > 0) fread(buf, sizeof(char), bytes_to_read, source->sound->file);
			else
			{
				if(source->loop) elf_init_sound_with_wav(source->sound, source->sound->file_path);
				else source->sound->eof = ELF_TRUE;
			}

			bytes_read = bytes_to_read;
			source->sound->position += bytes_read;
		}

		if(bytes_read > 0)
		{
			alBufferData(source->sound->buffer[source->sound->oldest_buffer], source->sound->format, buf, bytes_read, source->sound->freq);
			alSourceQueueBuffers(source->source, 1, &source->sound->buffer[source->sound->oldest_buffer]);
			source->sound->oldest_buffer++;
			if(source->sound->oldest_buffer > 2) source->sound->oldest_buffer = 0;
		}
	}
}

void elf_destroy_audio_source(elf_audio_source *source)
{
	if(source->sound)
	{
		source->sound->streaming = ELF_FALSE;
		elf_dec_ref((elf_object*)source->sound);
	}
	if(source->source) alDeleteSources(1, &source->source);

	free(source);
}

void elf_set_sound_volume(elf_audio_source *source, float volume)
{
	alSourcef(source->source, AL_GAIN, volume);
}

float elf_get_sound_volume(elf_audio_source *source)
{
	float volume = 0.0;
	alGetSourcef(source->source, AL_GAIN, &volume);
	return volume;
}

void elf_pause_sound(elf_audio_source *audio_source)
{
	alSourcePause(audio_source->source);
	audio_source->paused = ELF_TRUE;
}

void elf_resume_sound(elf_audio_source *source)
{
	if(elf_is_sound_playing(source)) return;

	if(source->sound->streamed)
	{
		if(source->sound->eof)
		{
			if(source->sound->file_type == ELF_OGG) elf_init_sound_with_ogg(source->sound, source->sound->file_path);
			else if(source->sound->file_type == ELF_WAV) elf_init_sound_with_wav(source->sound, source->sound->file_path);
		}
		else
		{
			alSourcePlay(source->source);
		}
	}
	else
	{
		alSourcePlay(source->source);
	}
	source->paused = ELF_FALSE;
}

void elf_stop_sound(elf_audio_source *source)
{
	int queued;
	unsigned int buffer;

	if(source->sound->streamed)
	{
		alSourceStop(source->source);
		alGetSourcei(source->source, AL_BUFFERS_QUEUED, &queued);
		while(queued--) alSourceUnqueueBuffers(source->source, 1, &buffer);
		source->sound->eof = ELF_TRUE;
	}
	else
	{
		alSourceStop(source->source);
		source->paused = ELF_FALSE;
	}
}

unsigned char elf_is_sound_playing(elf_audio_source *source)
{
	int state = 0;
	alGetSourcei(source->source, AL_SOURCE_STATE, &state);
	if(state == AL_PLAYING) return ELF_TRUE;
	return ELF_FALSE;
}

unsigned char elf_is_sound_paused(elf_audio_source *source)
{
	return source->paused;
}

void elf_set_sound_position(elf_audio_source *source, float x, float y, float z)
{
	alSource3f(source->source, AL_POSITION, x, y, z);
}

