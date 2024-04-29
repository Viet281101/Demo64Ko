#version 330
uniform float time;
uniform vec2 resolution;
out vec4 fragColor;
const float PI = 3.14159265;
float tex(vec2 pos){
    float upper = length(vec2(cos(PI/3.), sin(PI/11.))*length(pos)-pos);
    return min(pow(upper, .2), pow(1.-length(pos), .88));
}
void main( void ) {
    vec2 position = ( gl_FragCoord.xy / min(resolution.x, resolution.y) ) - vec2(1., 0.5);
    position *= 2.;
    fragColor = vec4(abs(position), abs(position));
    float color = 0.0;
    vec2 circleDistor = position;
    circleDistor /= (sqrt(5.) - length(circleDistor));
    float modab = 1.;
    float off = mod(time, 1.) * modab;
    circleDistor = normalize(circleDistor)*mod(length(circleDistor)-off, modab);
    float ang = asin(circleDistor.y/length(circleDistor));
    if(circleDistor.x < 0.){ ang = PI-ang; }
    ang = mod(ang, PI/3.);
    circleDistor = vec2(cos(ang), sin(ang))*length(circleDistor);
    vec4 col = mix(vec4(0.5), vec4(0.), tex(circleDistor));
    fragColor = col;
}