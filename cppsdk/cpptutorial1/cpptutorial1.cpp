#include "stdlib.h"
#include "stdio.h"
#include "blendelf.h"

int main()
{
	elfInitWithConfig("config.txt");
	elf_handle scn = elfLoadScene("level1.pak");
	elf_handle cam = elfGetCameraByName(scn, "Camera");
	elfHideMouse(true);
	while(elfRun())
	{
		if(elfGetKeyState(ELF_KEY_W) == ELF_DOWN) elfMoveActorLocal(cam, 0.0, 0.0, -12.0); 
		if(elfGetKeyState(ELF_KEY_S) == ELF_DOWN) elfMoveActorLocal(cam, 0.0, 0.0, 12.0);
		if(elfGetKeyState(ELF_KEY_A) == ELF_DOWN) elfMoveActorLocal(cam, -12.0, 0.0, 0.0);
		if(elfGetKeyState(ELF_KEY_D) == ELF_DOWN) elfMoveActorLocal(cam, 12.0, 0.0, 0.0);
		elf_vec2i mf = elfGetMouseForce();
		elfRotateActor(cam, 0.0, 0.0, -mf.x*10.0);
		elfRotateActorLocal(cam, -mf.y*10.0, 0.0, 0.0); 
		if(elfGetKeyState(ELF_KEY_ESC) == ELF_PRESSED) elfQuit();
	}
	return 0;
}
