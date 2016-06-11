uniform sampler2D stexflat;
uniform sampler2D stexflat2;
uniform sampler2D stexflat3;
uniform sampler2D stexflat4;
uniform sampler2D stexflat5;
uniform float _amb;
uniform float _bunny;
uniform float _fill1;
uniform float _fill2;
uniform vec4 BColors1;
uniform vec4 BColors2;
uniform vec4 BunnyColors;

void main(void) {

	vec4 cFinal; 
	vec4 CT;
	vec4 CT2;
	vec4 CT3;
	vec4 CT4;
	vec4 CT5;
	vec4 Ambiente;
	vec4 Conejo;
	vec4 Relleno1;
	vec4 Relleno2;
	CT = texture2D(stexflat,gl_TexCoord[0].st);
	CT2 = texture2D(stexflat2,gl_TexCoord[0].st);
	CT3 = texture2D(stexflat3,gl_TexCoord[0].st);
	CT4 = texture2D(stexflat4,gl_TexCoord[0].st);
	CT5 = texture2D(stexflat5,gl_TexCoord[0].st);
	Ambiente = CT*_amb;
	Conejo = BunnyColors*CT2*_bunny;
	Relleno1 = BColors1*CT3*_fill1;
	Relleno2 = BColors2*CT4*_fill2;

	cFinal =  Ambiente + Conejo + Relleno1 + Relleno2;
	
	gl_FragColor = cFinal;
}