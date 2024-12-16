#version 420 core

out vec4 fragColor;
in vec2 texCoords;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;
uniform float depth;

void main()
{   
    vec3 color;
    if(depth>0.5f){
        color = vec3(texture(iChannel1, texCoords).r);
    }else
    {
        color = texture(iChannel0, texCoords).rgb;
    }
    fragColor = vec4(color,1);
}