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
out vec4 v_FragPosLightSpace;


uniform mat4 u_V;
uniform mat4 lightSpaceMatrix;

void main()
{
	v_pos = i_position;
	v_TexCoord = i_texCoord;
	v_color = i_color;
	v_tex = i_texId;
	v_normal = i_normalv;

	v_FragPosLightSpace = i_position * lightSpaceMatrix;

	gl_Position = i_position * u_V;
};

#shader fragment
#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;


in vec2 v_TexCoord;
in vec4 v_color;
in float v_tex;
in vec3 v_normal;
in vec4 v_pos;
in vec4 v_FragPosLightSpace;

uniform sampler2D u_texture[32];

float getShadow(){
	vec3 projCoords = v_FragPosLightSpace.xyz / v_FragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	vec2 coords = projCoords.xy;
    float depth = v_FragPosLightSpace.z / v_FragPosLightSpace.w;
    depth = (1.0 - 0.0) * 0.5 * depth + (1.0 + 0.0) * 0.5;
	float bias = 0.005;
	return depth - bias > texture(u_texture[7], coords).r  ? 1.0 : 0.0;
}

void main()
{
	gPosition = vec4(getShadow(),0,0,1); //SHOULD FIX BUFFER STUFF NOW IT JUST WORKS
	//gPosition = v_pos;
	gNormal = vec4(v_normal,1);
	
	int index = int(v_tex + 0.5);
	if (v_tex >= 0) {
		vec4 texColor = texture(u_texture[index], v_TexCoord);
		if (texColor.a == 0.0) discard;
		gAlbedoSpec = texColor * v_color;
		//gAlbedoSpec = vec4(vec3(gl_FragCoord.z), 1.0);
	}
	else {
		gAlbedoSpec = v_color;
	}
};