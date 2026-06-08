#version 460 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedo;

uniform sampler2D textureAlbedo;

in vec4 fragPos;
in vec4 fragNormal;
in vec2 fragUV;

void main() {
    // store the fragment position vector in the first gbuffer texture
    gPosition = fragPos;
    // also store the per-fragment normals into the gbuffer
    gNormal = vec4(normalize(fragNormal.rgb), 0.0f);
    // and the diffuse per-fragment color
    gAlbedo = texture(textureAlbedo, fragUV);
}
