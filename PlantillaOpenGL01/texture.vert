// Nabil J. Márquez   -  11-10683
// Marisela del Valle -  11-10267
// Proyecto V - Computacion Grafica I
// Profesor: Eduardo Roa
// Ultima modificacion:  11/06/16

void main(void){
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

}