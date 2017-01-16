#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 position;
in vec3 normal;
in vec2 texcoord;

out vec2 v_texcoord;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    v_texcoord = texcoord;
}
