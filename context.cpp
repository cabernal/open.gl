#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL.h>
#include <SDL_opengl.h>

/* 
	- Place holder shaders.
	- TODO: Put these in their own files
*/
const char *vertex_shader_src = 
"#version 150\n\
in vec2 position;\n\
void main()\n\
{\n\
	gl_Position = vec4(position, 0.0, 1.0);\n\
}";

const char *fragment_shader_src = 
"#version 150\n\
out vec4 outColor; \n\
void main() \n\
{ \n\
outColor = vec4(1.0, 1.0, 1.0, 1.0); \n\
}";

const char *uniform_fragment_shader_src = 
"#version 150\n\
uniform vec3 triangleColor; \n\
out vec4 outColour; \n\
void main() \n\
{ \n\
outColor = vec4(triangleColor, 1.0); \n\
}";

float vertices[] = {
     0.0f,  0.5f, // Vertex 1 (X, Y)
     0.5f, -0.5f, // Vertex 2 (X, Y)
    -0.5f, -0.5f  // Vertex 3 (X, Y)
};

GLint check_shader_compilation_status(GLint vertexShader)
{
	// Get shader compilation status
	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE)
	{
			char compile_log[512];
			glGetShaderInfoLog(vertexShader, sizeof(compile_log), NULL, compile_log);
			printf("Shader compilation failed.\n");
			printf("Shader compilation log: %s\n", compile_log);
	}

	return status;
}


int main(int atgc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    // Create window
    SDL_Window* window = SDL_CreateWindow("OpenGL", 100, 100, 800, 900, SDL_WINDOW_OPENGL);

    // Create OpenGL context
    SDL_GLContext context = SDL_GL_CreateContext(window);

    // Initialize GLEW
	glewExperimental = GL_TRUE;
    glewInit();
	
	// Create VAO, and start using  it
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

    // Create vertex buffer object
	GLuint vbo;
    glGenBuffers(1, &vbo);

	// Active vertex buffer object, and copy data into it
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Create and compile vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader,  1, &vertex_shader_src, NULL);
	glCompileShader(vertexShader);

	// Create and compile fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragment_shader_src, NULL);
	glCompileShader(fragmentShader);

	// Check for any shader compilation errors
	if (check_shader_compilation_status(vertexShader) != GL_TRUE ||
		check_shader_compilation_status(fragmentShader) != GL_TRUE)
	{
		return -1;
	}

	// Link the vertex and fragment shader into a shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// Specify the layout of the vertex data
	GLint posAttr = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttr);
	glVertexAttribPointer(posAttr, 2, GL_FLOAT, GL_FALSE, 0, 0);


	//** Start Drawing **//
	glDrawArrays(GL_TRIANGLES, 0, 3);



    printf("%u\n", vbo);


    // Event loop
    SDL_Event windowEvent;

    while(true)
    {
        if(SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT) break;
        }
        
        // Swap front and back buffers
        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(context);

    SDL_Quit();
    return 0;
}
