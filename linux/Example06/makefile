INCLUDE_DIRS = -I../../src/GLUS -I../External/include
LIBRARY_DIRS = -L../GLUS/lib -L../External/lib
LIBRARIES = -lGLUS -lGLEW -lGL -lm -lrt

NAME = Example06

all: main

main: 
	   gcc $(INCLUDE_DIRS) $(LIBRARY_DIRS) ../../src/$(NAME)/main.c $(LIBRARIES) -o../../Binaries/$(NAME)

clean:
	   rm ../../Binaries/$(NAME)
