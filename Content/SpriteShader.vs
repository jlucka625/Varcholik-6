//This shader is the vertex shader for OpenGL. Made in GLSL
#version 330 core

//vec2 position + vec2 texCoords
layout (location = 0) in vec4 vertex;

out vec2 TexCoords;

//Single screen, no camera required
uniform mat4 model;
uniform mat4 projection;
uniform vec2 translation;

void main()
{
	TexCoords = vertex.zw;

	//Optional texture translation here
	TexCoords.x += translation.x;
	TexCoords.y += translation.y;

	gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}