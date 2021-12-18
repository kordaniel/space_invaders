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

uint hash(uint x)
{
    // Bob Jenkins' One-At-A-Time hashing algorithm
    x += (x << 10u);
    x ^= (x >> 6u);
    x += (x << 3u);
    x ^= (x >> 11u);
    x += (x << 15u);
    return x;
}

uint hash(uvec2 v) { return hash(v.x ^ hash(v.y)); }
uint hash(uvec3 v) { return hash(v.x ^ hash(v.y) ^ hash(v.z)); }
uint hash(uvec4 v) { return hash(v.x ^ hash(v.y) ^ hash(v.z) ^ hash(v.w)); }

float floatConstruct(uint m)
{
    // Construct a float with half-open range [0:1) using low 23 bits.
    const uint ieeeMantissa = 0x007FFFFFu; // binary32 mantissa bitmask
    const uint ieeeOne      = 0x3F800000u; // 1.0 in IEEE binary32

    m &= ieeeMantissa;                     // Keep only mantissa bits (fractional part)
    m |= ieeeOne;                          // Add fractional part to 1.0
                                           // uintBitsToFloat(m) is in range [1:2)
    return uintBitsToFloat(m) - 1.0;       // Range [0:1)
}

// Pseudo-random value in half-open range [0:1).
float random( float x ) { return floatConstruct(hash(floatBitsToUint(x))); }
float random( vec2  v ) { return floatConstruct(hash(floatBitsToUint(v))); }
float random( vec3  v ) { return floatConstruct(hash(floatBitsToUint(v))); }
float random( vec4  v ) { return floatConstruct(hash(floatBitsToUint(v))); }


void main(void)
{
    float randvalue = random(TexCoord);

    vec2 lightCoord, sunCoord;
    lightCoord.x = 0.3  + 0.7  * randvalue; // x in [0.30, 1)
    lightCoord.y = 0.92 + 0.08 * randvalue; // y in [0.92, 1)
    sunCoord.x   = 0.65;
    sunCoord.y   = 0.96;

    vec2 distanceVec    = (TexCoord.xy - lightCoord.xy);
    vec2 distanceSunVec = (TexCoord.xy - sunCoord.xy);
    float invDistGrad    = inversesqrt(dot(distanceVec, distanceVec));
    float invDistSunGrad = inversesqrt(dot(distanceSunVec, distanceSunVec));

    outColor = texture(buffer, TexCoord).rgb;
    outColor.g *= 0.6 * TexCoord.y;
    outColor.b *= 0.9 * TexCoord.y;

    if (outColor.r < 0.5) {
        outColor.rg  *=       randvalue;
        outColor.b   *= 3   * randvalue;
        outColor.rgb *= 0.6 * (0.1 + invDistGrad);
    } else {
        outColor.rgb *= invDistSunGrad;
    }

}
