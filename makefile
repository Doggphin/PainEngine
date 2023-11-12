
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall -DUSEGLEW
LIBS=-lfreeglut -lglew32 -lglu32 -lopengl32 -lm
CLEAN=rm -f *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
RES=$(shell uname -r|sed -E 's/(.).*/\1/'|tr 12 21)
CFLG=-O3 -Wall -Wno-deprecated-declarations -DRES=$(RES)
LIBS=-framework GLUT -framework OpenGL
#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall
LIBS=-lglut -lGLU -lGL -lm
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f $(EXE) *.o *.a
endif

# Which compiler to use
CC=gcc
# Which flags to use
CFLAGS =-Wall
# Which libraries to use
LDFLAGS = 
# Which object files are part of the final program
OBJFILES= vector3.o Quaternion.o physics.o geometry.o comp_collider.o vector2.o window.o macros.h includeallcomps.h comp_cameracontroller.o comp_mesh.o comp_camera.o comp_entityrotator.o comp_playercontroller.o input.o ecs.o uthash.h project.o
# What to name the final prog
EXE = project

all: $(EXE)

fatal.o: fatal.c CSCIx229.h
errcheck.o: errcheck.c CSCIx229.h
print.o: print.c CSCIx229.h
loadtexbmp.o: loadtexbmp.c CSCIx229.h
loadobj.o: loadobj.c CSCIx229.h
projection.o: projection.c CSCIx229.h
project.o: project.c CSCIx229.h

CSCIx229.a: projection.o loadobj.o loadtexbmp.o print.o errcheck.o fatal.o
	ar -rcs $@ $^

.c.o:
	gcc -c $(CFLG)  $<

$(EXE): $(OBJFILES) CSCIx229.a
	$(CC) $(CLFG) $(CFLAGS) -o $(EXE) $(OBJFILES) CSCIx229.a $(LIBS) $(LDFLAGS)

clean:
	$(CLEAN)