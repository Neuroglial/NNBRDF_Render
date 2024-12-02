#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    vec4 color1 = texture(texture1, TexCoord);
    vec4 color2 = texture(texture2, TexCoord);
    FragColor = vec4(mix(color1.rgb,color2.rgb , color2.a*0.25f),1.0f);
}