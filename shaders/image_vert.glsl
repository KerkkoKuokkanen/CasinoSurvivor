
#version 330 core

layout (location = 0) in vec2 aPos;      // final position from CPU
layout (location = 1) in vec2 aTexCoord; // final texture coordinate from CPU
layout (location = 2) in vec4 aColor;    // final vertex color from CPU
layout (location = 3) in float aTexIndex; // texture index from CPU

out vec2 texCoord;
out vec4 vertexColor;
out float texIndex;

void main()
{
	vec2 correctedPos = vec2(aPos.x, aPos.y * (16.0 / 9.0));
	gl_Position = vec4(correctedPos, 0.0, 1.0);
	texCoord = aTexCoord;
	vertexColor = aColor;
	texIndex = aTexIndex;
}
