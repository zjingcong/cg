// These are set by the .vert code, and they're interpolated.
varying vec3 ec_vnormal, ec_vposition;

# define PI 3.14159265


// calculate the color when using a spot light
// Blinn-Phong Lighting
vec4 CalSpotLight(in gl_LightSourceParameters spotlight, in gl_MaterialParameters material, vec3 fragPos, vec3 normal, vec3 viewDir)
{
	vec4 ambientColor, diffuseColor, specColor;
	vec3 P, N, L, V, H;

	P = fragPos;
	N = normal;
	V = viewDir;
	L = normalize(spotlight.position - P);
	H = normalize(L + V);	// halfway between L and V

	// diffuse shading
	float diff = max(dot(N, L), 0.0);
	// specular shading
	float spec = ((material.shininess + 2.0) / (8.0 * PI)) * pow(max(dot(H, N), 0.0), material.shininess);
	// attenuation
	float kc = spotlight.constantAttenuation;
	float kl = spotlight.linearAttenuation;
	float kq = spotlight.quadraticAttenuation;
	float d = length(spotlight.position - P);
	float attenuation = 1.0f / (kc + kl * d + kq * d * d);
	// calculate color
	ambientColor = spotlight.ambient * material.ambient;
	diffuseColor = spotlight.diffuse * material.diffuse * diff;
	specColor = spotlight.specular * material.specular * spec;
	ambientColor *= attenuation;
	diffuseColor *= attenuation;
	specColor *= attenuation;

	return (ambientColor + diffuseColor + specColor);
}


void main()
{
	vec3 fragPos, normal, viewDir;
	fragPos = ec_vposition;
	normal = normalize(ec_vnormal);
	viewDir = normalize(-fragPos);

	vec4 fragColor;
	// key light
	fragColor += CalSpotLight(gl_LightSource[0], gl_FrontMaterial, fragPos, normal, viewDir);
	// fill light
	fragColor += CalSpotLight(gl_LightSource[1], gl_FrontMaterial, fragPos, normal, viewDir);
	// back light
	fragColor += CalSpotLight(gl_LightSource[2], gl_FrontMaterial, fragPos, normal, viewDir);

	gl_FragColor = fragColor;
}

