#version 450 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D textTexture;
uniform vec4 textColor;

void main() {
    vec4 sampled = texture(textTexture, TexCoord);
    FragColor = textColor * sampled.rgba;
}