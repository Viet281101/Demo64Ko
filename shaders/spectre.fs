#version 330
in float height;
uniform float maxHeight;
out vec4 fragColor;
void main() {
	float normHeight = height / maxHeight;
	vec3 bottomColor = vec3(1.0, 0.0, 0.0);
	vec3 topColor = vec3(1.0, 0.65, 0.0);
	vec3 color = mix(bottomColor, topColor, normHeight);
	fragColor = vec4(color, 1.0);
}