#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPosWorld;
layout(location = 2) in vec3 fragNormalWorld;

layout(set = 0, binding = 0) uniform GlobalUbo {
    mat4 projection;
    mat4 view;
    mat4 invView;
    vec4 ambientLightColor; // w is intensity
    vec4 directionalLightColor; // w is intensity
    vec3 directionalLightDirection;
} ubo;

layout(location = 0) out vec4 outColor;

void main() {
    vec3 ambientLight = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;
    vec3 surfaceNormal = normalize(fragNormalWorld);

    vec3 cameraPosWorld = ubo.invView[3].xyz;
    vec3 viewDirection = normalize(cameraPosWorld - fragPosWorld);

    vec3 directionToLight = -ubo.directionalLightDirection;
    float diffuseFactor = max(dot(surfaceNormal, directionToLight), 0); // Lambertian

    vec3 halfAngle = normalize(directionToLight + viewDirection);
    float blinnTerm = dot(surfaceNormal, halfAngle);
    blinnTerm = clamp(blinnTerm, 0, 1);
    blinnTerm = pow(blinnTerm, 512.0); // higher values -> sharper highlight
    vec3 specularLight = ubo.directionalLightColor.xyz * ubo.directionalLightColor.w * blinnTerm;

    vec3 directionalDifuse = ubo.directionalLightColor.xyz * ubo.directionalLightColor.w * diffuseFactor;

    outColor = vec4((directionalDifuse + ambientLight) * fragColor.rgb + specularLight * fragColor.rgb, 1.0);
}
