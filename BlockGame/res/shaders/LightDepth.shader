#shader vertex
#version 330 core

layout(location = 0) in vec4 i_position;
layout(location = 1) in vec4 i_color;
layout(location = 2) in vec2 i_texCoord;
layout(location = 3) in float i_texId;
layout(location = 4) in vec3 i_normalv;

uniform mat4 lightSpaceMatrix;
out vec2 v_TexCoord;
out float v_tex;


void main()
{
	v_TexCoord = i_texCoord;
	v_tex = i_texId;
	gl_Position = i_position * lightSpaceMatrix;
};

#shader fragment
#version 330 core

in vec2 v_TexCoord;
in float v_tex;

uniform sampler2D u_texture[32];

void main()
{
	int index = int(v_tex + 0.5);
	if(texture(u_texture[index], v_TexCoord).a == 0) discard;
};