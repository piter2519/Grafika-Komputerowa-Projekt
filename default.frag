// === default.frag (multi-light with attenuation) ===
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in mat3 TBN;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D roughnessMap;

uniform vec3 viewPos;
#define NUM_LIGHTS 15
uniform vec3 lightPositions[NUM_LIGHTS];

void main()
{
    vec3 albedo = texture(albedoMap, TexCoords).rgb;
    float roughness = clamp(texture(roughnessMap, TexCoords).r, 0.04, 1.0);

    vec3 normal = texture(normalMap, TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(TBN * normal);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0);

    float lightPower = 100.0;
    vec3 lightColor = vec3(1.0);

    for (int i = 0; i < NUM_LIGHTS; ++i)
    {
        vec3 lightDir = normalize(lightPositions[i] - FragPos);
        float diff = max(dot(normal, lightDir), 0.0);

        vec3 halfway = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normal, halfway), 0.0), mix(4.0, 64.0, 1.0 - roughness));

        float distance = length(lightPositions[i] - FragPos);
        float attenuation = lightPower / (1.0 + distance * distance);

        vec3 diffuse = diff * albedo * attenuation;
        vec3 specular = lightColor * spec * (1.0 - roughness) * attenuation;

        result += diffuse + specular;
    }

    vec3 ambient = 0.1 * albedo;
    vec3 finalColor = ambient + result;
    FragColor = vec4(min(finalColor, vec3(1.0)), 1.0);
}
