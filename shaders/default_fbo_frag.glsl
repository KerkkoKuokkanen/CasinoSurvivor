#version 330 core

out vec4 FragColor;
in vec2 texCoord;

uniform sampler2D screenTexture;
uniform vec2 resolution;

// Constants for tuning
#define FXAA_REDUCE_MIN   (1.0/128.0)
#define FXAA_REDUCE_MUL   (1.0/8.0)
#define FXAA_SPAN_MAX     8.0

void main()
{
    vec2 texelSize = 1.0 / resolution;

    // 1. Sample the center and 4 immediate neighbors
    vec3 rgbNW = texture(screenTexture, texCoord + vec2(-1.0, -1.0) * texelSize).rgb;
    vec3 rgbNE = texture(screenTexture, texCoord + vec2(1.0, -1.0) * texelSize).rgb;
    vec3 rgbSW = texture(screenTexture, texCoord + vec2(-1.0, 1.0) * texelSize).rgb;
    vec3 rgbSE = texture(screenTexture, texCoord + vec2(1.0, 1.0) * texelSize).rgb;
    vec3 rgbM  = texture(screenTexture, texCoord).rgb;

    // 2. Convert to Luma (Human eye is most sensitive to green)
    vec3 luma = vec3(0.299, 0.587, 0.114);
    float lumaNW = dot(rgbNW, luma);
    float lumaNE = dot(rgbNE, luma);
    float lumaSW = dot(rgbSW, luma);
    float lumaSE = dot(rgbSE, luma);
    float lumaM  = dot(rgbM,  luma);

    // 3. Find the direction of the edge
    vec2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));

    // 4. Calculate scaling factor for the blur direction
    float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);
    float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);

    dir = min(vec2(FXAA_SPAN_MAX,  FXAA_SPAN_MAX),
          max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),
          dir * rcpDirMin)) * texelSize;

    // 5. Sample along the detected edge direction
    vec3 rgbA = 0.5 * (
        texture(screenTexture, texCoord + dir * (1.0/3.0 - 0.5)).rgb +
        texture(screenTexture, texCoord + dir * (2.0/3.0 - 0.5)).rgb);
    
    vec3 rgbB = rgbA * 0.5 + 0.25 * (
        texture(screenTexture, texCoord + dir * (0.0/3.0 - 0.5)).rgb +
        texture(screenTexture, texCoord + dir * (3.0/3.0 - 0.5)).rgb);

    // 6. Check if we blurred too much (if luma is out of bounds, use rgbA)
    float lumaB = dot(rgbB, luma);
    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

    float alpha = texture(screenTexture, texCoord).a;

    if ((lumaB < lumaMin) || (lumaB > lumaMax)) {
        FragColor = vec4(rgbA, alpha);
    } else {
        FragColor = vec4(rgbB, alpha);
    }
}
