precision mediump float;

varying vec4 vColor;
varying vec2 vUV;
varying vec3 vNormal;

void main() {
    float NdotL = dot(normalize(vNormal), vec3(1.0, 0.0, 0.0)) * 0.5 + 0.5;
    gl_FragColor = vec4(vColor.rgb * NdotL, 1.0);
}
