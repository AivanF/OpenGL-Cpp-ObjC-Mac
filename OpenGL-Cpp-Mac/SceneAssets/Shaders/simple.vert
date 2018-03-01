#version 410

uniform struct Matrices {
    mat4 projMatrix;
    mat4 viewMatrix;
    mat4 modelMatrix;
    mat4 normalMatrix;
} matrices;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;

smooth out vec3 vWorldPos;

void main() {
    gl_Position = matrices.projMatrix * matrices.viewMatrix * matrices.modelMatrix * vec4(inPosition, 1.0);
    vec4 vWorldPosVertex = matrices.modelMatrix*vec4(inPosition, 1.0);
    vWorldPos = vWorldPosVertex.xyz;
}
