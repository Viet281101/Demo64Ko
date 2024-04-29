#version 330
uniform float time;
uniform vec2 resolution;
out vec4 fragColor;
#define pi 3.14159265
#define R(p, a) p=cos(a)*p+sin(a)*vec2(p.y, -p.x)
#define hsv(h,s,v) mix(vec3(1.), clamp((abs(fract(h+vec3(3., 2., 1.)/3.)*6.-3.)-1.), 0., 1.), s)*v
float pn(vec3 p) {
	vec3 i = floor(p);
	vec4 a = dot(i, vec3(1., 57., 21.)) + vec4(0., 57., 21., 78.);
	vec3 f = cos((p-i)*pi)*(-.5) + .5;
	a = mix(sin(cos(a)*a), sin(cos(1.+a)*(1.+a)), f.x);
	a.xy = mix(a.xz, a.yw, f.y);
	return mix(a.x, a.y, f.z);
}
float fpn(vec3 p) { return pn(p*0.06125)*0.56 + pn(p*0.5)*0.25 + pn(p*0.25)*0.125; }
float sphere(vec3 p) { return length(p)-2.5; }
float f(vec3 p) {
	p.z += 6.;
	R(p.xy, 0.005*time);
	R(p.xz, 0.13*time);
	return sphere(p) + fpn(p*50.+time*15.) * 0.45;
}
vec3 g(vec3 p) {
	vec2 e = vec2(0.0001, 0.);
	return normalize(vec3(f(p+e.xyy) - f(p-e.xyy), f(p+e.yxy) - f(p-e.yxy), f(p+e.yyx) - f(p-e.yyx)));
}
void main(void) {
	vec3 p = vec3(0., 0., 2.);
	vec3 d = normalize(vec3((gl_FragCoord.xy / (0.5 * resolution) - 1.0) * vec2(resolution.x / resolution.y, 1.0), 0.) - p);
	float ld, td = 0., w;
	vec3 tc = vec3(0.);
	float r = 0.0, l = 1.0, b = 0.;
	bool hit = false;
	for (float i = 0.; i < 1.; i += 1. / 64.) {
		if (!(i < 1. && l >= 0.001 * r && r < 50. && td < .95))
			break;
		l = f(p) * 0.7;
		if (l < .01) {
			hit = true;
			ld = 0.01 - l;
			w = (1. - td) * ld;
			tc += w;
			td += w;
		}
		td += 1. / 200.;
		l = max(l, 0.3);
		p += l * d;
		r += l;
	}
	if (!hit) {fragColor = vec4(0.0, 0.0, 0.0, 1.0);}
	else {fragColor = vec4(tc.x + td * 2., ld * 3., 0, 1.0);}
}

