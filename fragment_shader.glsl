#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 color1;

void main()
{
    vec4 color2 = texture(texture1, TexCoord);
    vec4 color3 = texture(texture2, TexCoord);
    FragColor = vec4(mix(color2.rgb,color3.rgb , color3.a*0.25f) + color1 ,1.0f);
}