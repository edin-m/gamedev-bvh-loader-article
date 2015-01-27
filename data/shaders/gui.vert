#version 130

//uniform vec4 screenParams;

in vec2 position;
in vec2 vUV;

smooth out vec2 vTexCoord;

void main()
{
    vec4 v = vec4(2*position.xy - 1, -1.0, 1.0);

    vTexCoord = vUV;

    gl_Position = v;
}
