#version 410

smooth in vec3 vWorldPos;
out float outputColor;

uniform vec3 vCameraPos;

void main() {
    vec3 dist = - vCameraPos - vWorldPos;
    outputColor = length(dist);
}
