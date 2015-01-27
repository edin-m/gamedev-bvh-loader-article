#version 130

in vec4 position;
in vec4 color;
uniform mat4 MVP;

smooth out vec4 vColor;

void main()
{
    gl_Position = MVP * position;
    vColor = color;
}
