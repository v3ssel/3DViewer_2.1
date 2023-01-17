#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

flat out  vec3 NormalFlat;
smooth out  vec3 FragPos;
smooth out  vec3 NormalSmooth;
out  vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    NormalFlat = NormalSmooth = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);

    gl_Position = projection * view * vec4(FragPos, 1.0);
}
