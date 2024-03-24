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

in vec2 fragmentTexCoord;

uniform sampler2D colorBuffer;

out vec4 color;

//void main()
//{
//    vec4 pixel = texture(colorBuffer, fragmentTexCoord);
//    color = vec4(pixel.r, pixel.g, pixel.b, 1.0);
//}

void main()
{
    float Pi = 6.28318530718; // Pi*2
    
    // GAUSSIAN BLUR SETTINGS {{{
    float Directions = 16.0; // BLUR DIRECTIONS (Default 16.0 - More is better but slower)
    float Quality = 3.0; // BLUR QUALITY (Default 4.0 - More is better but slower)
    float Size = 1.0; // BLUR SIZE (Radius)
    // GAUSSIAN BLUR SETTINGS }}}
   
    vec2 Radius = Size/vec2(1280, 720);
    
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragmentTexCoord;
    // Pixel colour
    vec4 Color = texture(colorBuffer, uv);
    
    // Blur calculations
    for( float d=0.0; d<Pi; d+=Pi/Directions)
    {
		for(float i=1.0/Quality; i<=1.0; i+=1.0/Quality)
        {
			Color += texture( colorBuffer, uv+vec2(cos(d),sin(d))*Radius*i);
        }
    }
    
    // Output to screen
    Color /= Quality * Directions - 15.0;
    color =  Color;
}