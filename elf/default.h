/*
	BlendELF
	------------------------------------------
	File: 		default.h
	Author:		mbg
	Purpose:	Includes commonly used headers.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <malloc.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef ELF_WINDOWS
	// mbg: prevents windows.h from including winsock.h, this is later handled by enet 
	#define _WINSOCKAPI_
	#include <windows.h>
	#include <strsafe.h>
#else
	#include <dirent.h>
#endif

#include <GL/glfw.h>
#ifdef ELF_MACOSX
	#include <OpenGL/gl.h>
#else
	#include <GL/gl.h>
#endif

#ifdef ELF_MACOSX
	#include <OpenAL/alc.h>
	#include <OpenAL/al.h>
#else
	#include <AL/alc.h>
	#include <AL/al.h>
#endif

#include <vorbis/vorbisfile.h>

#include <FreeImage.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <enet/enet.h>

/* 
	End of File 
*/

