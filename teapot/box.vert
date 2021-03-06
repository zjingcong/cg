varying vec4 tcoords;
varying vec3 ec_vnormal, ec_vposition;

void main()
{
	ec_vnormal = gl_NormalMatrix * gl_Normal;
	ec_vposition = gl_ModelViewMatrix * gl_Vertex;

	gl_FrontColor = gl_Color;
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;

	tcoords = gl_TextureMatrix[7]*gl_Vertex;
}

