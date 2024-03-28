#shader vertex
#version 330 core

out vec2 fragmentTexCoord;

const vec2[6] positions = vec2[](
    vec2( -1.0, -1.0),
    vec2(-1.0, 1.0),
    vec2(1.0, 1.0),

    vec2(1.0, 1.0),
    vec2( 1.0, -1.0),
    vec2( -1.0,  -1.0)
);

void main()
{
    vec2 vertexPos = positions[gl_VertexID];
    gl_Position = vec4(vertexPos, 0.0, 1.0);
    fragmentTexCoord = 0.5 * (vertexPos + vec2(1.0, 1.0));
}

#shader fragment
#version 330 core
out vec4 FragColor;
  
in vec2 fragmentTexCoord;

uniform sampler2D u_texture[32];
uniform vec3 lightdir;

void main()
{             
    vec3 FragPos = texture(u_texture[4], fragmentTexCoord).rgb;
    vec3 Normal = texture(u_texture[5], fragmentTexCoord).rgb;
    vec3 Albedo = texture(u_texture[6], fragmentTexCoord).rgb;
    vec3 lighting = Albedo * 0.3;

    vec3 diffuse = 0.5 * max(dot(Normal, lightdir), 0.0) * Albedo * vec3(0.9,0.9,0.8);
    lighting += diffuse;
    
    FragColor = vec4(lighting, 1.0);
}  