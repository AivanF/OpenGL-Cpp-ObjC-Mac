#version 410

uniform struct Matrices {
    mat4 modelViewMatrix;
} matrices;

uniform float screenWidth;
uniform float screenHeight;

layout (location = 0) in vec3 inCoord;
layout (location = 1) in vec2 inPosition;

out vec3 texCoord;

void main() {
    texCoord = inCoord;
    vec4 pre = matrices.modelViewMatrix * vec4(inPosition, 0.0, 1.0);
    pre.x = 2 * pre.x / screenWidth - 1;
    pre.y = 2 * pre.y / screenHeight - 1;
    gl_Position = pre;
}
