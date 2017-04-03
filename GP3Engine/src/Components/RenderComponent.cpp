#include "Components\RenderComponent.h"
#include "GameObject.h"

Renderer::Renderer()
{
	type = "render component";
}

Renderer::Renderer(GameObject *tempOwner)
{
	type = "render component";
	owner = tempOwner;
}

Renderer::~Renderer()
{
}

void Renderer::init()
{

}

void Renderer::update(mat4 MVPMat)
{
	MVP = MVPMat;
}

void Renderer::render()
{
	//if (owner->getCurScene()->getActiveShader() != owner->getShader()->getShader())
	//{
		CHECK_GL_ERROR();
		//glUseProgram(owner->getShader()->getShader());
		owner->getCurScene()->setActiveShader(owner->getShader()->getShader());
		CHECK_GL_ERROR();
		//TODO fix invalid operation
	//}

	if (!owner->getTexture()->getTex2D())
	{
		glDepthMask(GL_FALSE);
		CHECK_GL_ERROR();
	}

	//get the uniform loaction for the MVP
	GLint MVPLocation = glGetUniformLocation(owner->getShader()->getShader(), "MVP");
	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, value_ptr(MVP));
	CHECK_GL_ERROR();

	if (owner->getTexture()->getTex2D())
	{

		GLuint modelLocation = glGetUniformLocation(owner->getShader()->getShader(), "model");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(owner->getModelMatrix()));
		CHECK_GL_ERROR();
		//get the uniform for the texture coords

		GLint texture0Location = glGetUniformLocation(owner->getShader()->getShader(), "mat.text");//inside material strut of texture spec FS
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, owner->getTexture()->getTexture());
		glUniform1i(texture0Location, 0);
		CHECK_GL_ERROR();

	}
	else
	{
		GLint cubeTexLocation = glGetUniformLocation(owner->getShader()->getShader(), "cubeTexture");
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, owner->getTexture()->getTexture());
		glUniform1i(cubeTexLocation, 1);
		CHECK_GL_ERROR();
	}

	/*GLint toonShadeLocation = glGetUniformLocation(owner->getShader()->getShader(), "toonShade");
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_1D, owner->getTexture()->getTexture());
	glUniform1i(toonShadeLocation, 1);*/

	glBindVertexArray(owner->getModel()->getVAO());
	CHECK_GL_ERROR();
	//begin drawing triangle 
	glDrawElements(GL_TRIANGLES, owner->getModel()->getMesh()->getNumIndices(), GL_UNSIGNED_INT, 0);
	CHECK_GL_ERROR();

	if (!owner->getTexture()->getTex2D())
	{
		glDepthMask(GL_TRUE);
		CHECK_GL_ERROR();
	}

}

string Renderer::getType()
{
	return type;
}