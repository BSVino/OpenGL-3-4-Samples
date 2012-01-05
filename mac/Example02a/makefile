INCLUDE_DIRS = -I../../src/GLUS -I../External/include -I/usr/X11/include
LIBRARY_DIRS = -L../GLUS/lib -L../External/lib -L/usr/X11/lib
LIBRARIES = -lglfw -lGLUS -lm -framework CoreFoundation -framework OpenGL -framework Cocoa

NAME = Example02a

all: main

main: 
	   gcc $(INCLUDE_DIRS) $(LIBRARY_DIRS) ../../src/$(NAME)/main.c $(LIBRARIES) -o ../../Binaries/$(NAME)

clean:
	   rm ../../Binaries/$(NAME)
