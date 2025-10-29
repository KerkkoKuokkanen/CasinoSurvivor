
#version 330 core

out vec4 FragColor;
in vec2 texCoord;
in float texIndex;
in vec4 vertexColor;

uniform sampler2D uTextures[4];

void main()
{
	vec4 texColor;

	// Choose the correct texture based on texIndex
	if (texIndex < 0.5)
		texColor = texture(uTextures[0], texCoord);
	else if (texIndex < 1.5)
		texColor = texture(uTextures[1], texCoord);
	else if (texIndex < 2.5)
		texColor = texture(uTextures[2], texCoord);
	else
		texColor = texture(uTextures[3], texCoord);

	FragColor = texColor * vertexColor;
}
