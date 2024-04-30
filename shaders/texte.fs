#version 330
in vec2 tcoord;
out vec4 fragColor;
uniform sampler2D tex;
void main() {
	vec4 textColor = texture(tex, tcoord);
	float blendFactor = tcoord.x;
	vec3 red = vec3(1.0, 0.0, 0.0);
	vec3 yellow = vec3(1.0, 1.0, 0.0);
	vec3 orange = vec3(1.0, 0.65, 0.0);
	vec3 gradientColor;
	if (blendFactor < 0.5) { gradientColor = mix(red, yellow, blendFactor * 2.0); }
	else { gradientColor = mix(yellow, orange, (blendFactor - 0.5) * 2.0); }
	fragColor = vec4(gradientColor * textColor.rgb, textColor.a);
}
