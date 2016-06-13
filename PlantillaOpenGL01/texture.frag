// Nabil J. Márquez   -  11-10683
// Marisela del Valle -  11-10267
// Proyecto V - Computacion Grafica I
// Profesor: Eduardo Roa
// Ultima modificacion:  11/06/16

// Inicializamos variables
#define textureWidth 4096.0
#define textureHeight 4096.0	
#define texel_size_x 1.0 / textureWidth
#define texel_size_y 1.0 / textureHeight

uniform sampler2D stexflat, stexflat2, stexflat3, stexflat4, stexflat5;
uniform float _amb, _bunny, _fill1, _fill2;
uniform vec4 cFill1, cFill2, cBunny, cFloor, cAmb;
uniform bool billOn;

// Funcion para filtro bilinear
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

	vec4 wholePaint, cAUX, cAUX2, cAUX3, cAUX4, cAUX5, amb, bunny, fill1, fill2, floor; 
	// Verificamos condicion para ver si se activa el filtro o no
	if (billOn) {
		cAUX = texture2D_bilinear(stexflat,gl_TexCoord[0].st);
		cAUX2 = texture2D_bilinear(stexflat2,gl_TexCoord[0].st);
		cAUX3 = texture2D_bilinear(stexflat3,gl_TexCoord[0].st);
		cAUX4 = texture2D_bilinear(stexflat4,gl_TexCoord[0].st);
		cAUX5 = texture2D_bilinear(stexflat5,gl_TexCoord[0].st);
	}

	else {
		cAUX = texture2D(stexflat,gl_TexCoord[0].st);
		cAUX2 = texture2D(stexflat2,gl_TexCoord[0].st);
		cAUX3 = texture2D(stexflat3,gl_TexCoord[0].st);
		cAUX4 = texture2D(stexflat4,gl_TexCoord[0].st);
		cAUX5 = texture2D(stexflat5,gl_TexCoord[0].st);
	}
	// Valores correspondientes para el piso, ambiente, conejo y arboles
	floor = mix(cAUX,cFloor,cAUX5);
	amb = cAmb*_amb;
	bunny = cBunny*cAUX2*_bunny;
	fill1 = cFill1*cAUX3*_fill1;
	fill2 = cFill2*cAUX4*_fill2;

	wholePaint =  floor*amb*(bunny + fill1 + fill2);

	gl_FragColor = wholePaint;
}