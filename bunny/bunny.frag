// These are set by the .vert code, and they're interpolated.
varying vec3 ec_vnormal, ec_vposition;

# define PI 3.14159265

struct Material
{
	// material ambient, diffuse and specular reflectance
	vec4 diffuse;
	vec4 specular;
	vec4 ambient;
	float shininess;
};


// calculate the color when using a spot light
// Blinn-Phong Lighting
vec4 CalSpotLight(in gl_LightSourceParameters spotlight, Material material)
{
	vec4 diffuseColor, specColor;
	vec3 P, N, L, V, H;
	vec4 material_diff = material.diffuse; 
	vec4 material_spec = material.specular; 
	float material_shininess = material.shininess;

	P = ec_vposition;
	N = normalize(ec_vnormal);
	V = normalize(-P);

	L = normalize(spotlight.position - P);
	H = normalize(L + V);
		
	diffuseColor = material_diff * max(dot(N, L), 0.0);
	specColor = material_spec * ((material_shininess + 2.0) / (8.0 * PI)) * pow(max(dot(H, N), 0.0), material_shininess);

	return (diffuseColor + specColor);
}


void main()
{
	// set material
	Material myMaterial;
	myMaterial.diffuse = gl_FrontMaterial.diffuse;
	myMaterial.specular = gl_FrontMaterial.specular;
	myMaterial.shininess = gl_FrontMaterial.shininess;
	myMaterial.ambient = gl_FrontMaterial.ambient;

	vec4 fragColor;
	fragColor += CalSpotLight(gl_LightSource[0], myMaterial);

	gl_FragColor = fragColor;
}

