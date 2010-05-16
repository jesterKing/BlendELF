/*
	BlendELF
	------------------------------------------
	File: 		default.h
	Author:		mbg
	Purpose:	Includes commonly used headers.
*/

#define ELF_OS_LINUX	1
#define ELF_OS_WINDOWS	2
#define ELF_OS_MACOSX	3

#if defined(__WIN32__) || defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__)
	#define ELF_BUILD_OS	ELF_OS_WINDOWS
#else
	#define ELF_BUILD_OS	ELF_OS_LINUX
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <malloc.h>
#include <sys/types.h>

#if ELF_BUILD_OS == ELF_OS_WINDOWS
	// mbg: prevents windows.h from including winsock.h, this is later handled by enet 
	#define _WINSOCKAPI_
	#include <windows.h>
	#include <strsafe.h>
#else
	#include <dirent.h>
#endif

#include <GL/glfw.h>
#include <GL/gl.h>

#include <enet/enet.h>

typedef unsigned char ELF_BOOL;

/* 
	End of File 
*/

