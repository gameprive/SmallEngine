#version 330 core

in vec2 TexCoord;
in vec3 Normal;

uniform mat4 uView;

uniform sampler2D uDiffuseMap;
uniform sampler2D uNormalMap;
uniform sampler2D uSpecularMap;
uniform sampler2D uEmissionMap;

uniform vec3 uMaterialDiffuse;
uniform vec3 uMaterialSpecular;
uniform float uSpecularIntensity;
uniform float uSpecularHardness;
uniform float uNormalMapStrength;

out vec4 outColor;

void main()
{
	float minBrightness = texture(uEmissionMap, TexCoord.st).r;
	vec3 p = vec3((texture(uNormalMap, TexCoord).rg) - vec2(0.5, 0.5), 0.0);
	vec3 texColor = texture(uDiffuseMap, TexCoord.st).rgb;

	vec3 sun = normalize(vec3(0.5, 0.5, 0.5));
	vec3 normDelta = normalize(Normal + (p * uNormalMapStrength));
	float diffuse = max(dot(sun, normDelta), 0.1);

	vec3 reflectDir = reflect(normDelta, sun);
	float spec = max(dot(normalize(uView[2].xyz), reflectDir), 0.0);
	spec = pow(spec, uSpecularHardness);
	spec *= uSpecularIntensity;

	vec3 litColor = texColor * uMaterialDiffuse * diffuse;
	litColor = litColor + (litColor * spec * uMaterialSpecular * texture(uSpecularMap, TexCoord.st).r);
	litColor = mix(litColor, texColor, minBrightness);

	outColor = vec4(litColor, 1.0);
}