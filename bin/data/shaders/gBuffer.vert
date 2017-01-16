#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool reverse_normals;

in vec3 position;
in vec3 normal;
in vec2 texcoord;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;
out vec3 toLightVector;

void main()
{
    Position = (view * model * vec4(position, 1.0)).xyz;
    if(reverse_normals)
    {
        Normal = transpose(inverse(mat3(view * model))) * normal * -1.0f;
    }
    else
    {
        Normal = transpose(inverse(mat3(view * model))) * normal;
    }
    TexCoord = texcoord;
    
    gl_Position = projection * view * model * vec4(position, 1.0);
}
