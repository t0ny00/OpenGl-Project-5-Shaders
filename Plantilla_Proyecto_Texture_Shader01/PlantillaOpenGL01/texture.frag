uniform sampler2D stexflat;
uniform sampler2D tex_fill01;
uniform sampler2D tex_fill02;
uniform sampler2D tex_key;
uniform sampler2D tex_check;

uniform float _flat_ambient;
uniform float _fill01_ambient;
uniform float _fill02_ambient;
uniform float _key_ambient;
uniform float _fill01_r;
uniform float _fill01_g;
uniform float _fill01_b;
uniform float _fill02_r;
uniform float _fill02_g;
uniform float _fill02_b;
uniform float _key_r;
uniform float _key_g;
uniform float _key_b;
uniform float _mix;
uniform float _mixColor_r;
uniform float _mixColor_g;
uniform float _mixColor_b;

void main(void) {

	vec4 cFinal,iAmbFlat,iAmbFill01,iAmbFill02,iAmbKey,cFill01,cFill02,cKey,cFlat,cCheck;

	iAmbFlat = vec4(_flat_ambient,_flat_ambient,_flat_ambient,0.1);
	iAmbFill01 = vec4(_fill01_ambient,_fill01_ambient,_fill01_ambient,1);
	iAmbFill02 = vec4(_fill02_ambient,_fill02_ambient,_fill02_ambient,1);
	iAmbKey = vec4(_key_ambient,_key_ambient,_key_ambient,1);

	cFill01 = vec4(_fill01_r,_fill01_g,_fill01_b,1);
	cFill02 = vec4(_fill02_r,_fill02_g,_fill02_b,1);
	cKey= vec4(_key_r,_key_g,_key_b,1);

	cFlat = texture2D(stexflat,gl_TexCoord[0].st);
	cCheck = texture2D(tex_check,gl_TexCoord[0].st) * vec4(_mixColor_r,_mixColor_g,_mixColor_b,1);

	cFlat = mix(cFlat,cCheck,_mix);
	cFinal = cFlat * iAmbFlat 
			+ texture2D(tex_fill01,gl_TexCoord[0].st) * cFlat * cFill01 * iAmbFill01
			+ texture2D(tex_fill02,gl_TexCoord[0].st) * cFlat * iAmbFill02 * cFill02
			+ texture2D(tex_key,gl_TexCoord[0].st) * cFlat * iAmbKey * cKey;
	gl_FragColor = cFinal;
}