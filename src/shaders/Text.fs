#version 330 core

in vec2 texCoords;

uniform sampler2D text;
uniform vec3 textColor = vec3(1.0, 1.0, 1.0);

out vec4 fragColor;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, texCoords).r);
    fragColor = vec4(textColor, 1.0) * sampled;
}
