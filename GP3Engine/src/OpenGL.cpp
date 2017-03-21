#include "OpenGL.h"
#include "Common.h"

void checkError(const char *file, int line)
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		printf("%s:%d - GL Error: %s\n", file, line, gluErrorString(err));
}

//function to initalise OpenGL
void initOpenGL()
{
	glewExperimental = GL_TRUE;
	CHECK_GL_ERROR();

	//smopth shading
	glShadeModel(GL_SMOOTH);	
	CHECK_GL_ERROR();

	//clear the background to black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	CHECK_GL_ERROR();

	//clear the depth buffer to 1.0
	glClearDepth(1.0f);
	CHECK_GL_ERROR();

	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	CHECK_GL_ERROR();

	//the depth test to use
	glDepthFunc(GL_LEQUAL);
	CHECK_GL_ERROR();

	//turn on best perspective correction
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//Need to normalize the normal vectors for lighting calculations
	//glEnable(GL_RESCALE_NORMAL);
	CHECK_GL_ERROR();
	//TODO fix this invalue enum

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/*Problem: glewInit failed, something is SERIOUSLY wrong*/
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
	}
}

//function to set/reset viewport
void setViewport(int width, int height)
{
	//set up viewport
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

 