#shader vertex
#version 330

noperspective out vec2 TexCoord;

void main(void)
{
    TexCoord.x = (gl_VertexID == 2)? 2.0: 0.0;
    TexCoord.y = (gl_VertexID == 1)? 2.0: 0.0;
    
    gl_Position = vec4(2.0 * TexCoord - 1.0, 0.0, 1.0);
}

#shader fragment
#version 330

uniform sampler2D buffer;
noperspective in vec2 TexCoord;

out vec3 outColor;

void main(void)
{
    vec2 SunCoord;
    SunCoord.x = 0.99;
    SunCoord.y = 0.99;

    outColor = texture(buffer, TexCoord).rgb;
    outColor.g *= TexCoord.y;

    vec2 dis = TexCoord.xy - SunCoord.xy;
    float distance = 0.75*inversesqrt(dot(dis, dis));
    outColor.rgb *= distance;

}
