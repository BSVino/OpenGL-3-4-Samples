INCLUDE_DIRS = -I../External/include -I/usr/X11/include

NAME = GLUS

all: GLUS

GLUS:
	   gcc $(INCLUDE_DIRS) -c ../../src/$(NAME)/glus_mac.c ../../src/$(NAME)/glus_load.c ../../src/$(NAME)/glus_math.c ../../src/$(NAME)/glus_matrix.c ../../src/$(NAME)/glus_viewperspective.c ../../src/$(NAME)/glus_point.c ../../src/$(NAME)/glus_shaderprogram.c ../../src/$(NAME)/glus_shape.c ../../src/$(NAME)/glus_vector.c  ../../src/$(NAME)/glus_version.c
	   mv *.o lib/
	   ar -r lib/lib$(NAME).a lib/*.o

clean:
	   rm lib/*.o lib/lib$(NAME).a
