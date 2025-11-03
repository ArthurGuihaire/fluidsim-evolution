#version 330 core

in vec2 rectMin;
in vec2 rectSize;

in vec4 instanceColor;
in float radius;

out vec4 fragColor;

void main() {
    vec2 localPos = gl_FragCoord - rectMin;
    vec2 quadrantPos = abs(localPos - rectSize / 2);
    vec2 edgeDist = quadrantPos - (rectSize/2 - vec2(radius));
    vec2 edgeDistClamped = max(vec2(0.0f), edgeDist);
    float dist = length(edgeDistClamped);

    float alpha = 1.0 - smoothstep(radius-0.5, radius+0.5, dist);
    vec4 fragColor = vec4(instanceColor.rgb, alpha * instanceColor.a);
}
