attribute vec3 position;
attribute vec2 uv;
attribute vec3 normal;
attribute vec4 color;

uniform mat4 transform;
uniform mat4 projection;

varying vec4 vColor;
varying vec2 vUV;
varying vec3 vNormal;

void main() {
    vColor = color;
    vUV = uv;
    vNormal = (transform * vec4(normal, 0.0)).xyz;
    gl_Position = projection * transform * vec4(position, 1.0);
}
