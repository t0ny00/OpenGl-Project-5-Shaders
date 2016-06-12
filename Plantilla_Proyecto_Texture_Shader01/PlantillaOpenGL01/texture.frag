#define textureWidth 2048.0
#define textureHeight 2048.0	
#define texel_size_x 1.0 / textureWidth
#define texel_size_y 1.0 / textureHeight

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

uniform float _bilinearFilterEnabled;

vec4 texture2D_bilinear( sampler2D tex, vec2 uv ) {
	vec2 f;

	f.x	= fract( uv.x * textureWidth );
	f.y	= fract( uv.y * textureHeight );

	vec4 t00 = texture2D( tex, uv + vec2( 0.0, 0.0 ));
	vec4 t10 = texture2D( tex, uv + vec2( texel_size_x, 0.0 ));
	vec4 tA = mix( t00, t10, f.x);

	vec4 t01 = texture2D( tex, uv + vec2( 0.0, texel_size_y ) );
	vec4 t11 = texture2D( tex, uv + vec2( texel_size_x, texel_size_y ) );
	vec4 tB = mix( t01, t11, f.x );

	return mix( tA, tB, f.y );
}

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
	cCheck = texture2D(tex_check,gl_TexCoord[0].st);
	cFlat = mix(cFlat,cCheck,_mix);

	if(_bilinearFilterEnabled == 1.0){
		cFinal = cFlat * iAmbFlat 
				+ texture2D_bilinear(tex_fill01,gl_TexCoord[0].st) * cFlat * cFill01 * iAmbFill01
				+ texture2D_bilinear(tex_fill02,gl_TexCoord[0].st) * cFlat * iAmbFill02 * cFill02
				+ texture2D_bilinear(tex_key,gl_TexCoord[0].st) * cFlat * iAmbKey * cKey;
	} else {
		cFinal = cFlat * iAmbFlat 
				+ texture2D(tex_fill01,gl_TexCoord[0].st) * cFlat * cFill01 * iAmbFill01
				+ texture2D(tex_fill02,gl_TexCoord[0].st) * cFlat * iAmbFill02 * cFill02
				+ texture2D(tex_key,gl_TexCoord[0].st) * cFlat * iAmbKey * cKey;
	}

	gl_FragColor = cFinal;
}