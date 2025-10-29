#version 330 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D screenTexture;
uniform vec2 resolution; // Pass screen resolution here (e.g., 1920x1080)

void main()
{
	vec2 texelSize = 1.0 / resolution;

	vec3 col = texture(screenTexture, texCoord).rgb;

	// Sample surrounding pixels
	vec3 north = texture(screenTexture, texCoord + vec2(0.0, texelSize.y)).rgb;
	vec3 south = texture(screenTexture, texCoord - vec2(0.0, texelSize.y)).rgb;
	vec3 east  = texture(screenTexture, texCoord + vec2(texelSize.x, 0.0)).rgb;
	vec3 west  = texture(screenTexture, texCoord - vec2(texelSize.x, 0.0)).rgb;

	// Compute local contrast
	float contrast = length(north + south + east + west - 4.0 * col);

	// Smoothing factor based on contrast
	float strength = clamp(contrast * 5.0, 0.0, 1.0);

	// Average the surrounding pixels
	vec3 avg = (north + south + east + west) / 4.0;

	// Mix original color with blurred one
	vec3 result = mix(col, avg, strength);

	FragColor = vec4(result, 1.0);
}
