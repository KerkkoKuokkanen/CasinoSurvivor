
#version 330 core

out vec4 FragColor;
in vec2 texCoord;
in float texIndex;
in vec4 vertexColor;

uniform sampler2D uTextures[8];

void main()
{
	vec4 texColor;

	int index = int(texIndex);
	texColor = texture(uTextures[index], texCoord);

	FragColor = texColor * vertexColor;
}
