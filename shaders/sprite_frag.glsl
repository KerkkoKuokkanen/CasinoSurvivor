
#version 330 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D tex0;
uniform vec4 imageColor;

void main()
{
	vec4 texColor = texture(tex0, texCoord);
	FragColor = texColor * imageColor;
}
