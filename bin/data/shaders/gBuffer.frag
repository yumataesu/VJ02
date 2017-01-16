#version 330 core

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;
in vec3 toLightVector;

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec4 gDepth;
//layout (location = 4) out vec4 gShadow;

uniform sampler2D tex;
uniform sampler2D normal;
uniform vec3 lightColor;
uniform bool frag_bloom;

uniform samplerCube depthCubemap;

uniform float near; // Projection matrix's near plane distance
uniform float far; // Projection matrix's far plane distance

const float lightfar = 500.0f;

//http://www.thetenthplanet.de/archives/1180
mat3 cotangent_frame(vec3 N, vec3 p, vec2 uv)
{
    // get edge vectors of the pixel triangle
    vec3 dp1 = dFdx( p );
    vec3 dp2 = dFdy( p );
    vec2 duv1 = dFdx( uv );
    vec2 duv2 = dFdy( uv );
    
    // solve the linear system
    vec3 dp2perp = cross( dp2, N );
    vec3 dp1perp = cross( N, dp1 );
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;
    
    // construct a scale-invariant frame
    float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
    return mat3( T * invmax, B * invmax, N );
}


float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    
//    vec3 fragToLight = vec3(0.0, 0.0, 0.0) - Position;
//    float closestDepth = texture(depthCubemap, fragToLight).r;
//    closestDepth *= lightfar;
//    gShadow = vec4(vec3(closestDepth / lightfar), 1.0);
    
    vec3 NormalTexel =  texture(normal, TexCoord).rgb;
    mat3 tanM = cotangent_frame(Normal, Position, TexCoord);
    vec3 Normal_ = normalize(tanM * Normal);
    
    float depth = LinearizeDepth(gl_FragCoord.z);
    
    gPosition = vec4(Position, 1.0);
    gNormal = vec4(normalize(Normal), 1.0);
    if(frag_bloom)
        gAlbedo = vec4(1.0, 0.9, 1.0, 1.0);
    if(!frag_bloom)
        gAlbedo = vec4(lightColor, 1.0);
    gDepth = vec4(depth, depth, depth, 1.0);
}
