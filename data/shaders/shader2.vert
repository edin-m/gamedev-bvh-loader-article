#version 130

in vec4 position;
in vec4 color;
in vec2 vUV;

smooth out vec4 vColor;
smooth out vec2 vTexCoord;

uniform mat4 MVP;
uniform mat4 MVP2;
uniform int ort;

void main()
{
    vColor = color;
    vTexCoord = vUV;

//    gl_Position = MVP * position;
//    vec4 v =  position;
    vec4 v = MVP * position;

    if( ort == 1 )
    {
        v.xy = v.xy + 15;
    }
    else if ( ort == 2 )
    {
        v.xy = v.xy - 15;
    }
    else if ( ort == 3 )
    {
        v.xy = vec2(v.x - 15, v.y + 15);
    }
    else if ( ort == 4 )
    {
        v = MVP2 * position;
        v.xy = vec2(v.x + 15, v.y - 15);
    }

    if( ort > 0 )
    {
        if( ort == 4 )
            v = vec4(v.xyz * 0.05, v.w);
        else
            v = vec4(v.xyz * 0.05, 1.0);
    }

    gl_Position = v;


}
