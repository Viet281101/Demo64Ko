#version 330
uniform float time;
uniform vec2 resolution;
out vec4 fragColor;
void main( void ) {
	vec2 p = (gl_FragCoord.xy - 0.5 * resolution) / min(resolution.x, resolution.y);
	vec3 c = vec3(0.005, 0.0021, 0.009);
	vec3 lightCol = vec3(0.1 + 0.09 * tan (time * 1.), 0.3, 0.6 + 0.04 * tan (time * 3.));
	for(int i = 0; i < 50; ++i) {
		float t = 0.8 * 3. * float(i) / 4. * fract(time * .1);
		float x = cos(t);
		float y = sin(t);
		vec2 o = .5 * vec2(x  * fract(time * .1) - sin (time * 0.4) * y, y);
		c += 0.02 / (length(p - o)) * lightCol * 0.6;
	}
	fragColor = vec4(c,1.);
}