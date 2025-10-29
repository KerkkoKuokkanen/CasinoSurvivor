
#version 330 core

layout (location = 0) in vec2 iPosition;
layout (location = 1) in vec4 iUVRegion;
layout (location = 2) in vec2 iScale;
layout (location = 3) in float iAngle;
layout (location = 4) in vec4 iColor;

uniform vec2 uUniformScale;

out vec2 texCoord;
out vec4 fColor;

void main() {
	float cosAngle = cos(iAngle);
	float sinAngle = sin(iAngle);
	mat2 rotation = mat2(cosAngle, -sinAngle,
						 sinAngle, cosAngle);

	vec2 quadVertices[4] = vec2[](
		vec2(-0.5, -0.5),
		vec2( 0.5, -0.5),
		vec2( 0.5,  0.5),
		vec2(-0.5,  0.5)
	);
	vec2 localPos = quadVertices[gl_VertexID];

	localPos *= uUniformScale;
	vec2 scaledPos = localPos * iScale;
	vec2 rotatedPos = scaledPos * rotation;

	vec2 finalPos = rotatedPos + iPosition;

	gl_Position = vec4(finalPos, 0.0, 1.0);

	vec2 texCoords[4] = vec2[](
		vec2(iUVRegion.x, iUVRegion.w),
		vec2(iUVRegion.z, iUVRegion.w),
		vec2(iUVRegion.z, iUVRegion.y),
		vec2(iUVRegion.x, iUVRegion.y)
	);
	texCoord = texCoords[gl_VertexID];
	fColor = iColor;
}
