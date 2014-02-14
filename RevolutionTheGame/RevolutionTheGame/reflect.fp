#version 330
smooth in vec3 vVaryingTexCoords;

uniform samplerCube colorMap;

out vec4 vFragColor;

void main(void)
{
	vFragColor = texture(colorMap, vVaryingTexCoords.stp);
}
