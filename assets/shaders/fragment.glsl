#version 330 core
out vec4 FragColor;
uniform float uTime;

void main()
{
    vec3 color = 0.5 + 0.5 * cos(6.28318 * (vec3(0.0, 0.33, 0.67) + uTime));
    FragColor = vec4(color * 3.0, 1.0);
}