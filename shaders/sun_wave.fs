#version 330
uniform float time;
uniform vec2 resolution;
out vec4 fragColor;
#define power 1.9
#define zoomOut 1.0
#define rot 1.0
#define iter 10.0
#define huePower 5.0
#define glow 0.9
#define distortScale 0.5
#define distortPower 1.0
#define Speed 0.08
#define WaveSpeed 0.9
#define Brightness 0.3
void main() {
	vec2 uv = gl_FragCoord.xy / resolution.xy;
	vec2 XYScale = vec2(1.,1.);
	vec2 XYMove = vec2(0.0,0.0);
	uv *= zoomOut;
	uv.xy = uv.xy * XYScale;
	uv.xy = uv.xy + XYMove;
	vec3 finalCol = vec3(0,0,0);
	float halfDistort = distortScale / 0.5;
	float distortsc2 = distortScale / distortScale + halfDistort;
	for(float i = 1.0; i < iter; i++) {
		uv.x += distortPower / i * sin(i * distortScale * uv.y - time * Speed);
		uv.y += distortPower / i * sin(i * distortsc2 * uv.x + time * Speed);
	}
	vec3 col = vec3(vec3(glow,glow,glow)/sin(time * WaveSpeed - length(uv.yx) - uv.y));
	finalCol = vec3(col * col);
	vec3 Color = vec3(1.0, 0.2, 0.1) * Brightness;
	fragColor = vec4(finalCol.rgb * Color, 1) * power;
}
