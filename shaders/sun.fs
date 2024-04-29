#version 330
uniform float time;
uniform vec2 resolution;
out vec4 fragColor;
#define ray_brightness 10.
#define gamma 2.25
#define ray_density 8.5
#define curvature 24.
#define red   2.95
#define green 1.0
#define blue  0.49 
#define SIZE 0.4
float rand(vec2 n) { return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453); }
float noise(vec2 p){
	vec2 ip = floor(p);
	vec2 u = fract(p);
	u = u*u*(3.0-2.0*u);
	float res = mix(mix(rand(ip),rand(ip+vec2(1.0,0.0)),u.x), mix(rand(ip+vec2(0.0,1.0)),rand(ip+vec2(1.0,1.0)),u.x),u.y);
	return res*res;
}
mat2 m2 = mat2( 0.80,  0.60, -0.60,  0.80 );
float fbm( in vec2 p ) {
	float z=8.;
	float rz = -0.085;
	p *= 0.325;
	for (int i= 1; i < 6; i++) {
		rz+= abs((noise(p)-0.5)*2.)/z;
		z = z*2.;
		p = p*2.*m2;
	}
	return rz;
}
void main() {
	float t = -time*.03; 
	vec2 uv = gl_FragCoord.xy / resolution.xy-0.5;
	uv.x *= resolution.x/resolution.y;
	uv*= curvature* SIZE;
	float r = sqrt(dot(uv,uv));
	float x = dot(normalize(uv), vec2(.5,0.))+t;
	float y = dot(normalize(uv), vec2(.0,.5))+t;
	float val=0.0;
	val = fbm(vec2(r+ y * ray_density, r+ x * ray_density));
	val = smoothstep(gamma*.02-.1,ray_brightness+(gamma*0.02-.1)+.001,val);
	val = sqrt(val);
	vec3 col =  val/ vec3(red,green,blue);
	col = 1.-col; 
	float rad= 125. ; 
	col = mix(col,vec3(1.), rad - 166.667 * r);
	vec4 cfinal =  mix(vec4(col,1.0),vec4(0.0,0.0,.0,1.0),0.01);
	fragColor = vec4(cfinal);
}