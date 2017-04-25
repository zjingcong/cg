// These are set by the .vert code, and they're interpolated.
varying vec3 ec_vnormal, ec_vposition;

# define PI 3.14159265

// calculate the color when using a spot light
vec4 CalSpotLight(int i)
{
	vec4 ambientColor, diffuseColor, specColor;
	vec3 P, N, L, V, H;

	P = ec_vposition;
	N = normalize(ec_vnormal);
	V = normalize(-P);
	L = normalize(gl_LightSource[i].position - P);
	H = normalize(L + V);	// halfway between L and V


	// diffuse shading
	float diff = max(dot(N, L), 0.0);
	// specular shading
	float spec = ((gl_FrontMaterial.shininess + 2.0) / (8.0 * PI)) * pow(max(dot(H, N), 0.0), gl_FrontMaterial.shininess);
	// attenuation
	float kc = 1.0;
	float kl = 0.2;
	float kq = 0.01;
	float d = length(gl_LightSource[i].position - P);
	float attenuation = 1.0f / (kc + kl * d + kq * d * d);

	float lightAngle = degrees(acos(max(dot((-L), normalize(gl_LightSource[i].spotDirection)), 0.0)));
	// cutoff
	float cutoff = gl_LightSource[i].spotCutoff;
	// spot falloff
	float spot;
	// float spot = pow(max(dot((-L), normalize(gl_LightSource[i].spotDirection)), 0.0), gl_LightSource[i].spotExponent);
	// make soft edge
	if (lightAngle > cutoff)
		{spot = clamp(pow(cos((lightAngle - cutoff) * PI / 180.0), gl_LightSource[i].spotExponent), 0.0, 1.0);}
	else	{spot = 1.0;}

	// calculate color
	ambientColor = gl_LightSource[i].ambient * gl_FrontMaterial.ambient;
	diffuseColor = gl_LightSource[i].diffuse * gl_FrontMaterial.diffuse * diff;
	specColor = gl_LightSource[i].specular * gl_FrontMaterial.specular * spec;
	diffuseColor *= (attenuation * spot);
	specColor *= (attenuation * spot);

	return (ambientColor + diffuseColor + specColor);
}


void main()
{
	vec4 fragColor;
	// scene ambient
	fragColor += gl_FrontLightModelProduct.sceneColor;	// GL_LIGHT_MODEL_AMBIENT
	// spotlight
	for (int i = 0; i < 4; ++i)
	{
		fragColor += CalSpotLight(i);
	}
	fragColor += CalSpotLight(7);
	gl_FragColor = fragColor;
}

