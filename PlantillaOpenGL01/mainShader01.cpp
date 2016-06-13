﻿// Nabil J. Márquez   -  11-10683
// Marisela del Valle -  11-10267
// Proyecto V - Computacion Grafica I
// Profesor: Eduardo Roa
// Ultima modificacion:  11/06/16


// Cubica

#include <stdlib.h>
#include <conio.h>

#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include "glsl.h"
#include "glm.h"

// assimp include files. These three are usually needed.
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// the global Assimp scene object
const aiScene* scene = NULL;
GLuint scene_list = 0;
aiVector3D scene_minV, scene_maxV, scene_center;

#define aisgl_minV(x,y) (x<y?x:y)
#define aisgl_maxV(x,y) (y>x?y:x)

using namespace std;

cwc::glShaderManager SM;
cwc::glShader *shader;

//Textures
static GLuint texflat, texflat2, texflat3, texflat4, texflat5;
unsigned char* imageflat = NULL;
unsigned char* imageflat2 = NULL;
unsigned char* imageflat3 = NULL;
unsigned char* imageflat4 = NULL;
unsigned char* imageflat5 = NULL;

// Lights and colors
GLfloat amb, fill1, fill2, bunny;
float cAmb[4] = {1.0, 1.0, 1.0, 1.0};
float cFill1[4] = {1.0, 1.0, 1.0, 1.0};
float cFill2[4] = {1.0, 1.0, 1.0, 1.0};
float cFloor[4] = {1.0, 1.0, 1.0, 1.0};
float cBunny[4] = {1.0, 1.0, 1.0, 1.0};
// Bilinial filter
bool billOn = false;

int iheight, iwidth;

void ejesCoordenada() {
	
	glDisable(GL_LIGHTING);	
	glLineWidth(2.5);
	glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex2f(0,10);
		glVertex2f(0,-10);
		glColor3f(0.0,0.0,1.0);
		glVertex2f(10,0);
		glVertex2f(-10,0);
	glEnd();

	glLineWidth(1.5);
	int i;
	glColor3f(0.0,1.0,0.0);
	glBegin(GL_LINES);
		for(i = -10; i <=10; i++){
			if (i!=0) {		
				if ((i%2)==0){	
					glVertex2f(i,0.4);
					glVertex2f(i,-0.4);

					glVertex2f(0.4,i);
					glVertex2f(-0.4,i);
				}else{
					glVertex2f(i,0.2);
					glVertex2f(i,-0.2);

					glVertex2f(0.2,i);
					glVertex2f(-0.2,i);

				}
			}
		}
		
	glEnd();

	glEnable(GL_LIGHTING);

	glLineWidth(1.0);
}

void changeViewport(int w, int h) {
	
	float aspectratio;

	if (h==0)
		h=1;

   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(55, (GLfloat) w/(GLfloat) h, 1.0, 300.0);
   glMatrixMode (GL_MODELVIEW);

}

void Keyboard(unsigned char key, int x, int y)
{
  bool shallIPrint = true;
  switch (key)
  {
	default:
		shallIPrint = false;
		break;

	// 1: incrementa la intensidad de la luz ambiental en 0.05 (baked_flat)
	case '1':
		amb += 0.05;
		break;

	// 2: reduce la intensidad de la luz ambiental en 0.05 (baked_flat)
	case '2':
		if (amb>= 0)      //Raro el condicional, pero si lo pongo >= 0.05 se estanca en 0.05 y nunca llega a cero.
			amb -= 0.05;
		break;

	// q: incrementa la intensidad de la luz fill01 en 0.05 (baked_fill01)
	case 'Q':
	case 'q':
		fill1 += 0.05;
		break;

	// w: reduce la intensidad de la luz fill01 en 0.05 (baked_fill01)
	case 'W':
	case 'w':
		if (fill1>= 0)
			fill1 -= 0.05;
		break;

	// a: incrementa la intensidad de la luz fill02 en 0.05 (baked_fill01)
	case 'A': 
	case 'a': 
		fill2 += 0.05;
		break;

	// s: reduce la intensidad de la luz fill02 en 0.05 (baked_fill01)
	case 'S':
	case 's':
		if (fill2>= 0)
			fill2 -= 0.05;
		break;

	// z: incrementa la intensidad de la luz central del conejo en 0.05 (baked_keyrabbit)
	case 'Z':
	case 'z':
		bunny += 0.05;
		break;

	// x: reduce la intensidad de la luz central del conejo en 0.05 (baked_keyrabbit)
	case 'X':
	case 'x':
		if (bunny>= 0)
			bunny -= 0.05;
		break;

	// e: incremente R de RGB en 0.05 del color en la luz fill01
	case 'E':
	case 'e':
		cFill1[0] += 0.05;
		break;

	// r: incremente G de RGB en 0.05 del color en la luz fill01
	case 'R':
	case 'r':
		cFill1[1] += 0.05;
		break;

	// t: incremente B de RGB en 0.05 del color en la luz fill01
	case 'T':
	case 't':
		cFill1[2] += 0.05;
		break;

	// y: reduce R de RGB en 0.05 del color en la luz fill01
	case 'Y':
	case 'y':
		if (cFill1[0]>= 0)
			cFill1[0] -= 0.05;
		break;

	// u: reduce G de RGB en 0.05 del color en la luz fill01
	case 'U':
	case 'u':
		if (cFill1[1]>= 0)
			cFill1[1] -= 0.05;
		break;

	// i: reduce B de RGB en 0.05 del color en la luz fill01
	case 'I':
	case 'i':
		if (cFill1[2] >= 0)
			cFill1[2] -= 0.05;
		break;

	// d: incremente R de RGB en 0.05 del color en la luz fill02
	case 'D':
	case 'd':
		cFill2[0] += 0.05;
		break;

	// f: incremente G de RGB en 0.05 del color en la luz fill02
	case 'F':
	case 'f':
		cFill2[1] += 0.05;
		break;

	// g: incremente B de RGB en 0.05 del color en la luz fill02
	case 'G':
	case 'g':
		cFill2[2] += 0.05;
		break;

	// h: reduce R de RGB en 0.05 del color en la luz fill02
	case 'H':
	case 'h':
		if (cFill2[0]>= 0)
			cFill2[0] -= 0.05;
		break;

	// j: reduce G de RGB en 0.05 del color en la luz fill02
	case 'J':
	case 'j':
		if (cFill2[1]>= 0)
			cFill2[1] -= 0.05;
		break;
  
	// k: reduce B de RGB en 0.05 del color en la luz fill02
	case 'K':
	case 'k':
		if (cFill2[2]>= 0)
			cFill2[2] -= 0.05;
		break;

	// c: incremente R de RGB en 0.05 del color en la luz central conejo
	case 'C':
	case 'c':
		cBunny[0] += 0.05;
		break;

	// v: incremente G de RGB en 0.05 del color en la luz central conejo
	case 'V':
	case 'v':
		cBunny[1] += 0.05;
		break;

	// b: incremente B de RGB en 0.05 del color en la luz central conejo
	case 'B':
	case 'b':
		cBunny[2] += 0.05;
		break;

	// n: reduce R de RGB en 0.05 del color en la luz central conejo
	case 'N':
	case 'n':
		if (cBunny[0]>= 0)
			cBunny[0] -= 0.05;
		break;

	// m: reduce G de RGB en 0.05 del color en la luz central conejo
	case 'M':
	case 'm':
		if (cBunny[1]>= 0)
			cBunny[1] -= 0.05;
		break;

	// ,: reduce B de RGB en 0.05 del color en la luz central conejo
	case ',':
		if (cBunny[2]>= 0)
			cBunny[2] -= 0.05;
		break;

	// o: activa el filtro bilineal
	case 'O':
	case 'o':
		billOn=true;
		break;

	// p: desactiva el filtro bilineal
	case 'P':
	case 'p':
		billOn=false;
		break;

	// 1er color piso
	case '3':
		cFloor[0] = 1.0;
		cFloor[1] = 1.0;
		cFloor[2] = 0.5;
		cFloor[3] = 1.0;
		break;

	// 2do color piso
	case '4':
		cFloor[0] = 0.3;
		cFloor[1] = 0.3;
		cFloor[2] = 0.3;
		cFloor[3] = 1.0;
		break;

	// 3er color piso
	case '5':
		cFloor[0] = 0.5;
		cFloor[1] = 0.0;
		cFloor[2] = 1.0;
		cFloor[3] = 1.0;
		break;

	// 4to color piso
	case '6':
		cFloor[0] = 1.0;
		cFloor[1] = 0.0;
		cFloor[2] = 0.0;
		cFloor[3] = 1.0;
		break;
  }
  if (shallIPrint){
	printf("...I just press: %c\n",key);
  }
  cBunny[3]=1.0f;
  cBunny[3]=1.0f;
  glutPostRedisplay();
}


void init(){
	
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   
   //Seteamos las texturas para los distintos objetos
   glGenTextures(1, &texflat);
   glBindTexture(GL_TEXTURE_2D, texflat);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   // Textura ambiental
   imageflat = glmReadPPM("baked_flat.ppm", &iwidth, &iheight); 
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidth, iheight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageflat);

   glGenTextures(1, &texflat2);
   glBindTexture(GL_TEXTURE_2D, texflat2);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   // Textura para el conejo
   imageflat2 = glmReadPPM("baked_keyrabbit.ppm", &iwidth, &iheight);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidth, iheight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageflat2);

   glGenTextures(1, &texflat3);
   glBindTexture(GL_TEXTURE_2D, texflat3);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   // Texturas de relleno
   imageflat3 = glmReadPPM("baked_fill01.ppm", &iwidth, &iheight);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidth, iheight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageflat3);

   glGenTextures(1, &texflat4);
   glBindTexture(GL_TEXTURE_2D, texflat4);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   imageflat4 = glmReadPPM("baked_fill02.ppm", &iwidth, &iheight); 
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidth, iheight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageflat4);

   glGenTextures(1, &texflat5);
   glBindTexture(GL_TEXTURE_2D, texflat5);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   imageflat5 = glmReadPPM("baked_checker.ppm", &iwidth, &iheight); // Relleno 02
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidth, iheight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageflat5);

   shader = SM.loadfromFile("texture.vert","texture.frag"); // load (and compile, link) from file
  		  if (shader==0) 
			  std::cout << "Error Loading, compiling or linking shader\n";
	amb = 1.0f;
	bunny = 1.0f;
	fill1 = 0.0;
	fill2 = 0.0;
	for(int i = 0; i < 4; i++) {
		cFill1[i] = 1.0;
		cFill2[i] = 1.0;
		cBunny[i] = 1.0;
		cAmb[i] = 1.0;
	}
}

void recursive_render (const aiScene *sc, const aiNode* myNode)
{
	unsigned int i;
	unsigned int n = 0, t;
	aiMatrix4x4 m = myNode->mTransformation;

	// update transform
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);

	// draw all meshes assigned to this node
	for (; n < myNode->mNumMeshes; ++n) {
		
		const aiMesh* mesh = scene->mMeshes[myNode->mMeshes[n]];

		for (t = 0; t < mesh->mNumFaces; ++t) {
			const aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;

			switch(face->mNumIndices) {
				case 1: face_mode = GL_POINTS; break;
				case 2: face_mode = GL_LINES; break;
				case 3: face_mode = GL_TRIANGLES; break;
				default: face_mode = GL_POLYGON; break;
			}

			glBegin(face_mode);

			for(i = 0; i < face->mNumIndices; i++) {
				int index = face->mIndices[i];
				
				if(mesh->mColors[0] != NULL)
					glColor4fv((GLfloat*)&mesh->mColors[0][index]);
				
				if(mesh->mNormals != NULL) 
					glNormal3fv(&mesh->mNormals[index].x);
				
				if (mesh->HasTextureCoords(0)) 
					glTexCoord2f(mesh->mTextureCoords[0][index].x, 1-mesh->mTextureCoords[0][index].y);
				
				glVertex3fv(&mesh->mVertices[index].x);
			}

			glEnd();
		}

	}

	// draw all children
	for (n = 0; n < myNode->mNumChildren; ++n) {		
		recursive_render(sc, myNode->mChildren[n]);
	}

	glPopMatrix();
}

void render(){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat zExtent, xExtent, xLocal, zLocal;
    int loopX, loopZ;

	glLoadIdentity();                       
	gluLookAt (0.0, 37.0, 98.0, 0.0, 30.0, 0.0, 0.0, 1.0, 0.0);
	
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_LINE_SMOOTH );	

	glPushMatrix();
	// Iniciamos los shaders
	if (shader) shader->begin();
	// Pasamos las variables inicializadas al shader
	shader->setUniform1f("_amb", amb);
	shader->setUniform1f("_bunny", bunny);
	shader->setUniform1f("_fill1", fill1);
	shader->setUniform1f("_fill2", fill2);
	shader->setUniform1f("billOn", billOn);
	shader->setUniform4f("cFill1", cFill1[0], cFill1[1], cFill1[2], cFill1[3]);
	shader->setUniform4f("cFill2", cFill2[0], cFill2[1], cFill2[2], cFill2[3]);
	shader->setUniform4f("cBunny", cBunny[0], cBunny[1], cBunny[2], cBunny[3]);
	shader->setUniform4f("cAmb", cAmb[0], cAmb[1], cAmb[2], cAmb[3]);
	shader->setUniform4f("cFloor", cFloor[0], cFloor[1], cFloor[2], cFloor[3]);
	// Seteamos las texturas al shader
	shader->setTexture("stexflat", texflat, 0);
	shader->setTexture("stexflat2", texflat2, 1);
	shader->setTexture("stexflat3", texflat3, 2);
	shader->setTexture("stexflat4", texflat4, 3);
	shader->setTexture("stexflat5", texflat5, 4);

	// Codigo para el mesh	
	glEnable(GL_NORMALIZE);
	glTranslatef(0.0, -2.0, 0.0);
	glRotatef(0.0, 0.0, 1.0, 0.0);
	glScalef(1.0, 1.0, 1.0);
	if(scene_list == 0) {
	    scene_list = glGenLists(1);
	    glNewList(scene_list, GL_COMPILE);
            // now begin at the root node of the imported data and traverse
            // the scenegraph by multiplying subsequent local transforms
            // together on GL's matrix stack.
	    recursive_render(scene, scene->mRootNode);
	    glEndList();
	}
	glCallList(scene_list);
	
	glPopMatrix();
	 
	if (shader) shader->end();
	
	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);
	glutSwapBuffers();
}

void animacion(int value) {
	
	glutTimerFunc(10,animacion,1);
    glutPostRedisplay();
	
}

void get_bounding_box_for_node (const aiNode* myNode, 	aiVector3D* minV, 	aiVector3D* maxV, 	aiMatrix4x4* myMatrix){

	aiMatrix4x4 prev;
	unsigned int n = 0, t;

	prev = *myMatrix;
	aiMultiplyMatrix4(myMatrix,&myNode->mTransformation);

	for (; n < myNode->mNumMeshes; ++n) {
		const aiMesh* mesh = scene->mMeshes[myNode->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t) {
			aiVector3D tmp = mesh->mVertices[t];
			aiTransformVecByMatrix4(&tmp,myMatrix);
			minV->x = aisgl_minV(minV->x,tmp.x);
			minV->y = aisgl_minV(minV->y,tmp.y);
			minV->z = aisgl_minV(minV->z,tmp.z);
			maxV->x = aisgl_maxV(maxV->x,tmp.x);
			maxV->y = aisgl_maxV(maxV->y,tmp.y);
			maxV->z = aisgl_maxV(maxV->z,tmp.z);
		}
	}

	for (n = 0; n < myNode->mNumChildren; ++n) {
		get_bounding_box_for_node(myNode->mChildren[n],minV,maxV,myMatrix);
	}
	*myMatrix = prev;
}

void get_bounding_box (aiVector3D* minV, aiVector3D* maxV){

	aiMatrix4x4 myMatrix;
	aiIdentityMatrix4(&myMatrix);
	
	minV->x = minV->y = minV->z =  1e10f;
	maxV->x = maxV->y = maxV->z = -1e10f;
	get_bounding_box_for_node(scene->mRootNode,minV,maxV,&myMatrix);
}

int loadasset (const char* path){
	// we are taking one of the postprocessing presets to avoid
	// spelling out 20+ single postprocessing flags here.
	scene = aiImportFile(path,aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene) {
		get_bounding_box(&scene_minV,&scene_maxV);
		scene_center.x = (scene_minV.x + scene_maxV.x) / 2.0f;
		scene_center.y = (scene_minV.y + scene_maxV.y) / 2.0f;
		scene_center.z = (scene_minV.z + scene_maxV.z) / 2.0f;
		return 0;
	}
	return 1;
}

int main (int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(960,540);

	glutCreateWindow("Test Opengl");

	// Codigo para cargar la geometria usando ASSIMP

	aiLogStream stream;
	// get a handle to the predefined STDOUT log stream and attach
	// it to the logging system. It remains active for all further
	// calls to aiImportFile(Ex) and aiApplyPostProcessing.
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT,NULL);
	aiAttachLogStream(&stream);

	// ... same procedure, but this stream now writes the
	// log messages to assimp_log.txt
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE,"assimp_log.txt");
	aiAttachLogStream(&stream);

	// the model name can be specified on the command line. If none
	// is specified, we try to locate one of the more expressive test 
	// models from the repository (/models-nonbsd may be missing in 
	// some distributions so we need a fallback from /models!).
	
	loadasset( "escenario_proyecto01.obj");

	init ();

	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);
	glutKeyboardFunc (Keyboard);
	
	glutMainLoop();
	return 0;

}