
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 color;


out vec4 frag_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    frag_color = vec4(color, 1.0);
}



    