#Makefile

DEV_CFLAGS = -g -Wall -DELF_PLAYER
STA_CFLAGS = -Wall -O2 -DELF_PLAYER
SHR_CFLAGS = -fPIC -Wall -O2

INCS = -Igfx -Ielf -I/usr/include/lua5.1 -I/usr/include/freetype2

BLENDELF_LIBS = -lGL -lGLU -lGLEW -lglfw -lXxf86vm -lXrandr -lXrender -pthread \
	-lfreeimage -lvorbisfile -lvorbis -logg -lopenal -llua5.1 -lfreetype \
	-lBulletDynamics -lLinearMath -lBulletCollision

BLENDELF_STATIC_LIBS = -lGL -lGLU /usr/lib/libGLEW.a /usr/lib/libglfw.a \
	/usr/lib/libXxf86vm.a /usr/lib/libXrandr.a /usr/lib/libXrender.a \
	/usr/lib/libvorbisfile.a /usr/lib/libvorbis.a /usr/lib/libogg.a \
	/usr/lib/liblua5.1.a /usr/local/lib/libBulletDynamics.a \
	/usr/local/lib/libLinearMath.a /usr/local/lib/libBulletCollision.a \
	-lfreeimage -lfreetype -lopenal -pthread

all:
	python genwraps.py
	gcc -c elf/blendelf.c $(DEV_CFLAGS) $(INCS)
	gcc -c gfx/gfx.c $(DEV_CFLAGS) $(INCS)
	gcc -c elf/audio.c $(DEV_CFLAGS) $(INCS)
	g++ -c elf/physics.cpp $(DEV_CFLAGS) $(INCS)
	g++ -c elf/binds.cpp $(DEV_CFLAGS) $(INCS)
	g++ -c elf/blendelf_wrap.cxx $(DEV_CFLAGS) $(INCS)
	gcc -Wl,-rpath,linux_libraries -o blendelf *.o $(DEV_CFLAGS) $(BLENDELF_LIBS)
	rm *.o

static:
	python genwraps.py
	gcc -c elf/blendelf.c $(STA_CFLAGS) $(INCS)
	gcc -c gfx/gfx.c $(STA_CFLAGS) $(INCS)
	gcc -c elf/audio.c $(STA_CFLAGS) $(INCS)
	g++ -c elf/physics.cpp $(STA_CFLAGS) $(INCS)
	g++ -c elf/binds.cpp $(STA_CFLAGS) $(INCS)
	g++ -c elf/blendelf_wrap.cxx $(STA_CFLAGS) $(INCS)
	gcc -Wl,-rpath,linux_libraries -o blendelf *.o $(STA_CFLAGS) $(BLENDELF_STATIC_LIBS)
	rm *.o

shared:
	python genwraps.py
	gcc -c -fPIC elf/blendelf.c $(SHR_CFLAGS) $(INCS)
	gcc -c -fPIC gfx/gfx.c $(SHR_CFLAGS) $(INCS)
	gcc -c -fPIC elf/audio.c $(SHR_CFLAGS) $(INCS)
	g++ -c -fPIC elf/physics.cpp $(SHR_CFLAGS) $(INCS)
	g++ -c -fPIC elf/binds.cpp $(SHR_CFLAGS) $(INCS)
	g++ -c -fPIC elf/blendelf_wrap.cxx $(SHR_CFLAGS) $(INCS)
	gcc -Wl,-rpath,linux_libraries -shared -o libblendelf.so *.o $(SHR_CFLAGS) $(BLENDELF_STATIC_LIBS)
	rm *.o

