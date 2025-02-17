#vertex
#include "resource\shaders\b_post.vs"
#end

#fragment
#version 420 core

#include "resource\shaders\uniform\UB_Base_Info.inc"

out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D iChannel0;

vec3 makeBloom(float lod, vec2 offset, vec2 bCoord){
    
    vec2 pixelSize = 1.0 / vec2(iResolution.x, iResolution.y);

    offset += pixelSize;

    float lodFactor = exp2(lod);

    vec3 bloom = vec3(0.0);
    vec2 scale = lodFactor * pixelSize;

    vec2 coord = (bCoord.xy-offset)*lodFactor;
    float totalWeight = 0.0;

    if (any(greaterThanEqual(abs(coord - 0.5), scale + 0.5)))
        return vec3(0.0);

    for (int i = -5; i < 5; i++) {
        for (int j = -5; j < 5; j++) {

            float wg = pow(1.0-length(vec2(i,j)) * 0.125,6.0);

            bloom = pow(texture(iChannel0,vec2(i,j) * scale + lodFactor * pixelSize + coord, lod).rgb,vec3(2.2))*wg + bloom;
            totalWeight += wg;

        }
    }

    bloom /= totalWeight;

    return bloom;
}

void main()
{

	vec3 blur = vec3(0);

    blur += makeBloom(2.,vec2(0.0,0.0), texCoords);
	blur += makeBloom(3.,vec2(0.3,0.0), texCoords);
	blur += makeBloom(4.,vec2(0.0,0.3), texCoords);
	blur += makeBloom(5.,vec2(0.1,0.3), texCoords);
	blur += makeBloom(6.,vec2(0.2,0.3), texCoords);

    fragColor = vec4(pow(blur, vec3(1.0 / 1.8)),1.0);
}

#end