uniform sampler2D stexflat;

void main(void) {

	vec4 cFinal;
	cFinal = texture2D(stexflat,gl_TexCoord[0].st);
	
	gl_FragColor = cFinal;
}