#vertex
#include "resource\shaders\b_post.vs"
#end

#fragment
#version 420 core

#include "resource\shaders\uniform\UB_Base_Info.inc"

out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

const int samples = 3;
 
float gaussian(float i,float sigma) {
    return exp( -.5* dot(i/=sigma,i) ) / ( 6.28 * sigma*sigma );
}

vec4 blur(sampler2D sp, vec2 uv, vec2 scale, int LOD) {
    vec4 output = vec4(0);

    float sigma = samples * .25f;
    int radius = samples/2;
    
    for(int j = 0; j <= LOD; ++j)
    {   
        float bias = pow(2,j);
        for ( int i = -radius; i <= radius; ++i )
        {
            output += gaussian(i,sigma) * textureLod( sp, uv + vec2(0.0f,scale.y*i*bias), float(j) );
        }
    }

    return output / output.a;
}

void main() {
    vec4 color = blur(iChannel0, texCoords, 1./iResolution.xy, int(log2(max(iResolution.x,iResolution.y))));
    color += texture(iChannel1, texCoords);

    // HDR tonemapping
    color = color / (color + vec4(1.0));
    // gamma correctf
    color = pow(color, vec4(1.0/1.2f)); 

    fragColor = vec4(color.rgb,1.0f);
}
#end