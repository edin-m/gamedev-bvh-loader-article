#version 130

smooth in vec2 vTexCoord;

uniform sampler2D textureMap;

void main()
{
    vec4 col = texture2D(textureMap, vTexCoord);

    gl_FragColor = col;
}
