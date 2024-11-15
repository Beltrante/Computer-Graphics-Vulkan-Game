#version 450

layout(location = 0) in vec2 fragUV;
layout(location = 0) out vec4 outColor;
void main() {
	outColor = vec4(texture(tex, fragUV).rgb, 1.0f);	// output color