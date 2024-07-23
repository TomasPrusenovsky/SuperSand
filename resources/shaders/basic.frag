#version 460 core

uniform sampler2D imgTexture;
out vec4 FragColor;
in vec2 texCoord;

void main() {
    FragColor = texture(imgTexture, texCoord);
//    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}