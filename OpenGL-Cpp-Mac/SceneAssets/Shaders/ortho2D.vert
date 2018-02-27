#version 410

uniform struct Matrices {
	mat4 modelViewMatrix;
} matrices;

uniform float screenWidth;
uniform float screenHeight;

layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec2 inCoord;

out vec2 texCoord;

void main() {
    texCoord = inCoord;
    vec4 pre = matrices.modelViewMatrix*vec4(inPosition, 0.0, 1.0);
    pre.x = pre.x / screenWidth - 1;
    pre.y = pre.y / screenHeight - 1;
    gl_Position = pre;
}
