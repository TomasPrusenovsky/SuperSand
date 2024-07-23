#version 460 core
layout (location = 0) in vec2 aPos;

float[8] texCoords = {
0.0, 1.0,
1.0, 1.0,
0.0, 0.0,
1.0, 0.0
};

out vec2 texCoord;

void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
    texCoord = vec2(texCoords[gl_VertexID * 2], texCoords[gl_VertexID * 2 + 1]);
}