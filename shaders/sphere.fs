#version 330
uniform float time;
uniform vec2 resolution;
out vec4 fragColor;
float makePoint(float x,float y,float fx,float fy,float sx,float sy,float t, float s) {
	float xx=x+sin(t*fx)*sx;
	float yy=y+cos(t*fy)*sy;
	return ((s/3.0)/((0.007/s)+sqrt(xx*xx+yy*yy)));
}
vec3 gu(vec4 a,vec4 b,float f){ return mix(a.xyz,b.xyz,(f-a.w)*(1.0/(b.w-a.w))); }
float makePoint(float x, float y, float s) { return (s/3.0)/((0.007/s)+sqrt(x*x+y*y)); }
vec3 grad(float f){
	vec4 c01=vec4(0.0,0.0,0.0,0.00);
	vec4 c02=vec4(0.5,0.0,0.0,0.50);
	vec4 c03=vec4(1.0,0.0,0.0,0.55);
	vec4 c04=vec4(1.0,1.0,0.0,0.80);
	vec4 c05=vec4(1.0,1.0,1.0,1.00);
	return (f<c02.w)?gu(c01,c02,f):
	(f<c03.w)?gu(c02,c03,f):
	(f<c04.w)?gu(c03,c04,f):
	gu(c04,c05,f);
}
void main(void) {
	vec2 p = (gl_FragCoord.xy / resolution) - 0.5;
	p.x *= resolution.x / resolution.y;
	float a = makePoint(p.x, p.y, 55.0);
	vec3 a1 = grad(a / 183.0);
	fragColor = vec4(a1.x, a1.y, a1.z, 1.0);
}
