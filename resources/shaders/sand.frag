#version 460 core

out vec4 FragColor;

void main() {
    vec2 t = gl_FragCoord.xy;
    FragColor = vec4(t.x / 800.0 ,t.y / 800.0, 1.0, 1.0);
}