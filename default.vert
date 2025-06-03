#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 T;
out vec3 B;
out vec3 N;

uniform mat4 model;
uniform mat4 camMatrix;

void main()
{
    mat3 normalMatrix = mat3(model); // zak³adamy brak skalowania
    T = normalize(normalMatrix * aTangent);
    B = normalize(normalMatrix * aBitangent);
    N = normalize(normalMatrix * aNormal);

    TexCoords = aTex;
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;
    gl_Position = camMatrix * worldPos;
}
