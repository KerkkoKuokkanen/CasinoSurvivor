
#version 330 core

out vec4 FragColor;

in vec2 texCoord;
in vec4 fColor;

uniform sampler2D tex0;

void main()
{
	vec4 texColor = texture(tex0, texCoord);
	FragColor = texColor * fColor;
}
