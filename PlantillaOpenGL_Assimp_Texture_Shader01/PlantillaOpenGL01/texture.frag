uniform sampler2D tex01;
uniform sampler2D tex02;
uniform float _mixI;

void main(void) {

	vec4 cFinal;
	vec4 cT01;
	vec4 cT02;
	cT01 = texture2D(tex01,gl_TexCoord[0].st);
	cT02 = texture2D(tex02,gl_TexCoord[0].st);

	cFinal =  mix(cT01,cT02, max(_mixI,0.0));
	gl_FragColor = cFinal;
}