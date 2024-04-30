#version 330
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
out float height;
uniform mat4 proj, modelView;
void main() {
	gl_Position = proj * modelView * vec4(pos.xy, 0.0, 1.0);
	height = pos.z;
}