#version 460 core
out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 cameraPos;

uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;
uniform float shininess;

in vec3 fragPos;
in vec3 fragNormal;

void main() {
    vec3 normal = normalize(fragNormal);
    vec4 color = vec4(0.1f, 0.7f, 0.9f, 1.0f);
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 camDir =  normalize(cameraPos - fragPos);

    // Reflection
    float nDotL = dot(normal, lightDir);
    vec3 reflection = normalize((normal * 2.0f * nDotL) - lightDir);

    // Phong components
    float ambientFactor  = ambientStrength;
    float diffuseFactor  = diffuseStrength * max(nDotL, 0.0f);
    float specularFactor = specularStrength * pow(max(dot(reflection, camDir), 0.0f), shininess);

    vec4 ambientColor  = (0.3f * vec4(lightColor, 1.0f)) * color * ambientFactor;
    vec4 diffuseColor  = vec4(lightColor, 1.0f) * color * diffuseFactor;
    vec4 specularColor = vec4(lightColor, 1.0f) * specularFactor;

    FragColor = ambientColor + diffuseColor + specularColor;


}
