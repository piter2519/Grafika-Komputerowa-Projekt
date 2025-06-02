// === default.vert ===
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 TexCoords;
out vec3 FragPos;
out mat3 TBN;

uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
    TexCoords = aTex;
    vec3 T = normalize(mat3(model) * aTangent);
    vec3 B = normalize(mat3(model) * aBitangent);
    vec3 N = normalize(mat3(model) * aNormal);
    TBN = mat3(T, B, N);

    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;
    gl_Position = camMatrix * worldPos;
}