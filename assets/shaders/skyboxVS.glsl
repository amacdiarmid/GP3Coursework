#version 150
in vec3 vertexPosition;

out vec3 vertexTexCoordsOut;

//uniform vec3 cameraPosition;
uniform mat4 MVP;

void main()
{
	vertexTexCoordsOut = vertexPosition;
	gl_Position = MVP * vec4(vertexPosition, 1.0);
}