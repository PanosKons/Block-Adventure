#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 colorr;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float texId;

out vec4 v_color;
out vec2 v_TexCoord;
out float v_tex;
uniform mat4 u_V;

void main()
{
	gl_Position = position * u_V;
	v_TexCoord = texCoord;
	v_color = colorr;
	v_tex = texId;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
uniform sampler2D u_texture[32];
in vec4 v_color;
in float v_tex;

void main()
{
	int index = int(v_tex + 0.5);
	if (v_tex >= 0) {
		vec4 texColor = texture(u_texture[index], v_TexCoord);
		color = texColor * v_color;
	}
	else {
		color = v_color;
	}
};