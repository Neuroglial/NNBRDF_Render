#version 420 core

out vec4 fragColor;
in vec2 texCoords;
uniform sampler2D texture1;

float Luminance(vec3 Linear)
{
	return dot(Linear, vec3(0.3, 0.59, 0.11));
}

void main()
{
    vec3 color = texture(texture1, texCoords).rgb;
    fragColor = vec4(vec3(Luminance(color)),1);
}