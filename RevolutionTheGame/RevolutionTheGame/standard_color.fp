#version 400
smooth in vec3 vVaryingNormal;
smooth in vec3 vVaryingLightDir;
smooth in vec2 vVaryingTexCoords;
smooth in vec3 tangentWorld;
smooth in vec3 bitangentWorld;

uniform vec4 lightColor;
uniform vec4 ambientLightColor;
uniform sampler2D colorMap;
uniform sampler2D normalMap;

out vec4 vFragColor;

vec3 CalcBumpedNormal()
{
    vec3 Normal = normalize(vVaryingNormal);
    vec3 Tangent = normalize(tangentWorld);
    vec3 BumpMapNormal = texture(normalMap, vVaryingTexCoords).xyz;
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);
    mat3 TBN = mat3(Tangent, bitangentWorld, Normal);
    return normalize(TBN * BumpMapNormal);
}

void main(void)
{
	vec3 normalWorld = CalcBumpedNormal();
	//vec3 normalWorld = vVaryingNormal;
	float lightIntensity;

	lightIntensity = clamp(dot(normalWorld, vVaryingLightDir), 0.0f, 1.0f);

	vFragColor = ambientLightColor;

	vFragColor += clamp(lightColor * lightIntensity, 0.0, 1.0);

	vec3 vReflection = normalize(reflect(-vVaryingLightDir, normalWorld));
	
	float spec = clamp(dot(vReflection, normalWorld), 0.0, 1.0);

	if (lightIntensity != 0)
	{
		float powSpec = pow(spec, 128);
		vFragColor.rgb += vec3(powSpec, powSpec, powSpec);
	}

	vFragColor = vFragColor * texture(colorMap, vVaryingTexCoords.st);
}