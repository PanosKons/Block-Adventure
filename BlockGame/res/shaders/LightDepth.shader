#shader vertex
#version 330 core

layout(location = 0) in vec4 i_position;
layout(location = 1) in vec4 i_color;
layout(location = 2) in vec2 i_texCoord;
layout(location = 3) in float i_texId;
layout(location = 4) in vec3 i_normalv;

uniform mat4 lightSpaceMatrix;

void main()
{
	gl_Position = i_position * lightSpaceMatrix;
};

#shader fragment
#version 330 core

void main()
{
};