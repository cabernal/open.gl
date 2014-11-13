SDL_INCLUDE_PATH = /usr/include/SDL2
GLEW_INCLUDE_PATH = /usr/include/GL
GL_LINK = -lGL
LIBS = -L /usr/lib -lSDL2 -lGL -lGLEW
all: context

context: context.cpp
	g++ -Wall -I $(SDL_INCLUDE_PATH) -I $(GLEW_INCLUDE_PATH) context.cpp $(LIBS) -o context

clean:
	rm -f context
