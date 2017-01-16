#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D scene;
uniform sampler2D bloomBlur;

bool bloom = true;
const float exposure = 2.0f;

void main()
{             
    const float gamma = 1.2;
    vec3 hdrColor = texture(scene, TexCoord).rgb;
    vec3 bloomColor = texture(bloomBlur, TexCoord).rgb;
    if(bloom)
        hdrColor += bloomColor; // additive blending
    // tone mapping
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // also gamma correct while we're at it       
    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, 1.0f);
}
