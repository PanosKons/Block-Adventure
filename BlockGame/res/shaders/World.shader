#shader vertex
#version 330 core

layout(location = 0) in vec4 i_position;
layout(location = 1) in vec4 i_color;
layout(location = 2) in vec2 i_texCoord;
layout(location = 3) in float i_texId;
layout(location = 4) in vec3 i_normalv;

out vec4 v_color;
out vec2 v_TexCoord;
out float v_tex;
out vec3 v_normal;
out vec4 v_pos;


uniform mat4 u_V;

void main()
{
	v_pos = i_position;
	v_TexCoord = i_texCoord;
	v_color = i_color;
	v_tex = i_texId;
	v_normal = i_normalv;

	gl_Position = i_position * u_V;
};

#shader fragment
#version 330 core

layout (location = 0) out vec4 gPosition;//3
layout (location = 1) out vec4 gNormal;//3
layout (location = 2) out vec4 gAlbedoSpec;


in vec2 v_TexCoord;
in vec4 v_color;
in float v_tex;
in vec3 v_normal;
in vec4 v_pos;

uniform sampler2D u_texture[32];

void main()
{
	gPosition = v_pos;
	
	int index = int(v_tex + 0.5);
	if (v_tex >= 0) {
		vec4 texColor = texture(u_texture[index], v_TexCoord);
		if (texColor.a == 0.0) discard;
		gAlbedoSpec = texColor * v_color;
	}
	else {
		gAlbedoSpec = v_color;
	}
	gNormal = vec4(v_normal, 1);
};