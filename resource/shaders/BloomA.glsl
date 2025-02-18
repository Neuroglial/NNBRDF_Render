#vertex
#include "resource\shaders\b_post.vs"
#end

#fragment
#version 420 core
#include "resource\shaders\uniform\UB_Base_Info.inc"
out vec4 fragColor;
in vec2 texCoords;
uniform sampler2D iChannel0;
const int samples = 7;
float gaussian(float i,float sigma) {
    return exp( -.5*i*i/(sigma*sigma) ) / ( 2.57 * sigma );
}
vec4 blur(sampler2D sp, vec2 uv, float scale, int LOD) {
    vec4 output = vec4(0);

    float sigma = samples * .25f;
    int radius = samples/2;
    
    for(int j = 0; j <= LOD; ++j)
    {   
        float bias = pow(2,j);
        for ( int i = -radius; i <= radius; ++i )
        {
            output += gaussian(abs(i),sigma) * textureLod( sp, uv + vec2(0.0f,scale*i*bias), float(j) );
        }
    }

    return output / output.a;
}

void main() {
    fragColor = blur(iChannel0, texCoords, 1./iResolution.y, int(log2(max(iResolution.x,iResolution.y))));
}
#end