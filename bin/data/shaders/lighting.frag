#version 330 core

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 lposition;
};
const int LightNUM = 1;

uniform Light light[LightNUM];
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D ssao;
uniform samplerCube depthCubemap;

uniform vec3 viewPos;

uniform float constant;
uniform float linear;
uniform float quadratic;

in vec2 TexCoord;


layout (location = 0) out vec4 color;
layout (location = 1) out vec4 BrightColor;

const float far = 100.0f;


bool shadows = true;

// array of offset direction for sampling
vec3 gridSamplingDisk[20] = vec3[]
(
 vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
 vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
 vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
 vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
 vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
 );


float ShadowCalculation(vec3 fragPos)
{
    // Get vector between fragment position and light position
    vec3 fragToLight = fragPos - light[0].lposition;
    
    // Use the fragment to light vector to sample from the depth map
    float closestDepth = texture(depthCubemap, fragToLight).r;
    
    // It is currently in linear range between [0,1]. Let's re-transform it back to original depth value
    closestDepth *= far;
    
    // Now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    
    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / far)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(depthCubemap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far;   // Undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
    
    // Display closestDepth as debug (to visualize depth cubemap)
    vec4 FragColor = vec4(vec3(closestDepth / far), 1.0);
    
    // return shadow;
    return FragColor.r;
}


void main()
{
    
    vec3 position = texture(gPosition, TexCoord).xyz;
    vec3 normal = texture(gNormal, TexCoord).xyz;
    vec3 albedo = texture(gAlbedo, TexCoord).rgb;
    
    vec3 unitLightVector = normalize(light[0].lposition - position);
    vec3 uintVectorToCamera = normalize(viewPos - position);
    
    
    float AmbientOcculusion = texture(ssao, TexCoord).r;
    
    vec3 ambient = vec3(0.0, 0.0, 0.0);
    vec3 diffuse = vec3(0.0, 0.0, 0.0);
    vec3 specular = vec3(0.0, 0.0, 0.0);
    vec3 final = vec3(0.0, 0.0, 0.0);
    vec3 lighting = vec3(0.0, 0.0, 0.0);
    BrightColor = vec4(0.0, 0.0, 0.0, 0.0);
    
    //    for(int i = 0; i < LightNUM; i++)
    //    {
    //float distance = length(light[0].lpositionition - position);
    
    // Ambient
    ambient = albedo;
    
    // Diffuse
    vec3 lightDir = normalize(light[0].lposition - position);
    diffuse = max(dot(normal, lightDir), 0.0) * light[0].diffuse;
    
    // Specular
    vec3 viewDir  = normalize(viewPos - position);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
    specular = light[0].specular * spec;
    
    //Attenuation
    float dist = length(light[0].lposition - position) * 0.1;
    float attenuation = 0.0 + (0.0 * dist) + (0.1 * dist * dist);
    
    ambient /= attenuation;
    diffuse /= attenuation;
    specular /= attenuation;
    
    //float shadow = shadows ? ShadowCalculation(position) : 0.0;
    //final = (1.0 - shadow) * (diffuse + specular);
    
    lighting += ambient + diffuse + specular;
    
    // Check whether result is higher than some threshold, if so, output as bloom threshold color
    float brightness = 0.0;
    brightness = dot(lighting, vec3(0.2126, 0.2152, 0.2722));
    if(brightness > 1.0)
    {
        BrightColor = vec4(lighting, 1.0);
    }else{
        BrightColor = vec4(0.0, 0.0, 0.0, 0.0);
    }
    
    color = vec4(lighting, 1.0);
}




