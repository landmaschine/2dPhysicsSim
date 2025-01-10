#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 instanceData;

out vec4 vertexColor;

uniform mat4 projection;

void main() {
    vec3 worldPos = vec3(aPos.xy * instanceData.z + instanceData.xy, 0.0);
    gl_Position = projection * vec4(worldPos, 1.0f);
    vertexColor = vec4(1.0, 1.0, 1.0, 1.0);
}