#version 410

uniform sampler2D gSampler;
uniform vec3 vCameraPos;
uniform float dstmin;
uniform float dstmax;

smooth in vec2 vTexCoord;
flat in vec4 vColorPart;

out vec4 FragColor;


void main() {
    vec4 vTexColor = texture(gSampler, vTexCoord);
    
    float dist;
    
    dist = gl_FragCoord.z;
//    dist = distance(vCameraPos, gl_FragCoord.xyz);
    
    if (dist > dstmin) {
        if (dist < dstmax) {
            dist = 1.0f - (dist - dstmin) / (dstmax - dstmin);
        } else {
            dist = 0.0f;
        }
//        dist = 0.0f;
    } else {
        dist = 1.0f;
    }
    
    FragColor = vec4(vTexColor.xyz, dist) * vColorPart;
}
