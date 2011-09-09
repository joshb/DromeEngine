const vec3 AMBIENT = vec3(0.1, 0.1, 0.1);
const float MAX_DIST = 12.5;
const float MAX_DIST_SQUARED = MAX_DIST * MAX_DIST;

uniform sampler2D texture;
uniform sampler2D normalmap;
uniform sampler2DShadow shadowmap[3];
uniform vec3 lightColor[3];

varying vec3 lightDirection[3];
varying vec3 lightPositionOut[3];
varying vec3 lightHalfAngle[3];
varying vec4 shadowCoords[3];

void
main()
{
	// initialize diffuse/specular lighting
	vec3 diffuse = vec3(0.0, 0.0, 0.0);
	vec3 specular = vec3(0.0, 0.0, 0.0);

	// get fragment normal
	vec3 fragmentNormal = (texture2D(normalmap, gl_TexCoord[0].xy).rgb * 2.0) - 1.0;
	fragmentNormal.z *= 0.75;
	fragmentNormal = normalize(fragmentNormal);
	fragmentNormal = clamp(fragmentNormal, 0.0, 1.0);

	// loop through each light
	for(int i = 0; i < 3; ++i) {
		// get fragment/shadow map depth
		float fragmentDepth = shadowCoords[i].z / shadowCoords[i].w;

		// workaround for not being able to use
		// variables to index sampler arrays
		float shadowDepth;
		if(i == 0)
			shadowDepth = shadow2DProj(shadowmap[0], shadowCoords[0]).z;
		else if(i == 1)
			shadowDepth = shadow2DProj(shadowmap[1], shadowCoords[1]).z;
		else
			shadowDepth = shadow2DProj(shadowmap[2], shadowCoords[2]).z;

		if(shadowCoords[i].w < 0.0 || shadowDepth > fragmentDepth) {
			float dist = dot(lightPositionOut[i], lightPositionOut[i]);
			if(dist > MAX_DIST_SQUARED)
				dist = MAX_DIST_SQUARED;
			dist /= MAX_DIST_SQUARED;

			// diffuse
			vec3 color = lightColor[i] * (1.0 - dist);
			diffuse += color * pow(clamp(dot(fragmentNormal, normalize(lightDirection[i])), 0.0, 1.0), 1.0);

			// specular
			float s = pow(clamp(dot(fragmentNormal, normalize(lightHalfAngle[i])), 0.0, 1.0), 8.0) * (1.0 - dist);
			specular += (color + 0.75) * s;
		}
	}

	vec4 sample = texture2D(texture, gl_TexCoord[0].xy);
	gl_FragColor = vec4(clamp(sample.rgb * (diffuse + AMBIENT) + specular, 0.0, 1.0), sample.a);
}
