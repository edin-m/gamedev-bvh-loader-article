#version 130

smooth in vec4 vColor;
smooth in vec2 vTexCoord;

uniform sampler2D textureMap;

void main()
{
    vec4 col = texture2D(textureMap, vTexCoord);

    gl_FragColor = vColor;

    if( gl_FrontFacing )
        gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    else
        gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);

    gl_FragColor = vec4(col.xyzw);

}
