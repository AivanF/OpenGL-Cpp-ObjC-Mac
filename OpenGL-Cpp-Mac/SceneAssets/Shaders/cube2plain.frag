#version 410

in vec3 texCoord;
out vec4 outputColor;

uniform samplerCube gSamplerCube;

void main() {
    float clr = texture(gSamplerCube, texCoord).r;
    if (clr < 10000.0f) {
        clr = clamp(clr / 10.0f, 0, 0.95f);
    }
    outputColor = vec4(clr, clr, clr, 1.0f);
}

