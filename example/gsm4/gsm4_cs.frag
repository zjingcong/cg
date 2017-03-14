// gsm4_cs.frag ................................................

uniform sampler2D mytexture;
varying vec3 ec_vnormal, ec_vposition;
varying vec4 tcoords;

void main()
{
vec3 P, N, L, V, H;
vec4 pccoords;
vec4 diffuse_color = gl_FrontMaterial.diffuse; 
vec4 specular_color = gl_FrontMaterial.specular; 
float shininess = gl_FrontMaterial.shininess;
float depthsample;
vec4 clarity = vec4(1.0,1.0,1.0,0.0);

P = ec_vposition;
N = normalize(ec_vnormal);
L = normalize(gl_LightSource[0].position - P);
V = normalize(-P);				// eye position is (0,0,0)!
H = normalize(L+V);
		
pccoords = tcoords/tcoords.w ;
depthsample = texture2D(mytexture,pccoords.st);
if(depthsample < pccoords.z) clarity = vec4(0.5,0.3,0.3,0.0);
diffuse_color *= clarity*max(dot(N,L),0.0);
specular_color *= clarity*pow(max(dot(H,N),0.0),shininess);
gl_FragColor = 0.5*(diffuse_color + specular_color);
}

