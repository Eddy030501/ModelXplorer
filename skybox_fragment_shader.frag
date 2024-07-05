#version 410 core
#extension GL_ARB_separate_shader_objects : enable

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture_diffuse;

void main() {
    FragColor = texture(texture_diffuse, TexCoord);
}
