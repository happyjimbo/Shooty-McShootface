#version 120

uniform sampler2D texture;
uniform vec4 flashColor;
uniform float time;

vec4 applyFlash(vec4 texColor, float time)
{
    float intensity = abs(sin(time * 6.28318));
    return mix(texColor, flashColor, intensity);
}

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    vec4 texColor = texture2D(texture, uv);

    if (texColor.a > 0.1)
    {
        gl_FragColor = applyFlash(texColor, time);
    }
    else
    {
        gl_FragColor = texColor;
    }
}