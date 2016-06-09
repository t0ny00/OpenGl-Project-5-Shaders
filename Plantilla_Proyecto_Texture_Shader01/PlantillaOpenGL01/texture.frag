uniform sampler2D stexflat;
uniform sampler2D tex_fill01;
uniform sampler2D tex_fill02;
uniform sampler2D tex_key;
uniform float _flat_ambient;
uniform float _fill01_ambient;
uniform float _fill02_ambient;
uniform float _key_ambient;
uniform float _fill01_r;
uniform float _fill01_g;
uniform float _fill01_b;

void main(void) {

	vec4 cFinal,cAmbFlat,cAmbFill01,cAmbFill02,cAmbKey,cFill01;

	cAmbFlat = vec4(_flat_ambient,_flat_ambient,_flat_ambient,0.1);
	cAmbFill01 = vec4(_fill01_ambient,_fill01_ambient,_fill01_ambient,1);
	cAmbFill02 = vec4(_fill02_ambient,_fill02_ambient,_fill02_ambient,1);
	cAmbKey = vec4(_key_ambient,_key_ambient,_key_ambient,1);

	cFill01 = vec4(_fill01_r,_fill01_g,_fill01_b,1);

	cFinal = texture2D(stexflat,gl_TexCoord[0].st) * cAmbFlat 
			+ texture2D(tex_fill01,gl_TexCoord[0].st)* cFill01 * cAmbFill01
			+ texture2D(tex_fill02,gl_TexCoord[0].st) * cAmbFill02
			+ texture2D(tex_key,gl_TexCoord[0].st) * cAmbKey;
	gl_FragColor = texture2D(stexflat,gl_TexCoord[0]);
}