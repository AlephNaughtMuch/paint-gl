#version 460 core
out vec4 fragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 cameraPos;

uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;
uniform float shininess;

in vec2 fragUV;

void main() {
    vec3 normal = texture(gNormal, fragUV).rgb;
    vec3 position = texture(gPosition, fragUV).rgb;
    vec4 color = texture(gAlbedo, fragUV);

    vec3 lightDir = normalize(lightPos - position);
    vec3 camDir =  normalize(cameraPos - position);

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

    fragColor = ambientColor + diffuseColor + specularColor;

}
