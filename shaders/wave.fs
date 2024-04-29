#version 330
uniform float time;
uniform vec2  resolution;
out vec4 fragColor;
void main(void){
	vec2 p = (gl_FragCoord.xy * 2.0 - resolution) / min(resolution.x, resolution.y);
	float lambda = time*2.0;
	float t =0.02/abs(tan(lambda) - length(p));
	float t2 = atan(p.y, p.x) + time;
	vec2 something = vec2(1., (sin(time)+ 1.)*0.5);
	float dotProduct = dot(vec2(t),something)/length(p);
	fragColor = vec4(vec3(dotProduct), 1.0);
}