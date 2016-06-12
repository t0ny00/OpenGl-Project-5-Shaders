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
aiVector3D scene_min, scene_max, scene_center;

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)


using namespace std;

cwc::glShaderManager SM;
cwc::glShader *shader;


//Textures
static GLuint texflat;
unsigned char* imageflat = NULL;
static GLuint tex_fill01;
unsigned char* image_fill01 = NULL;
static GLuint tex_fill02;
unsigned char* image_fill02 = NULL;
static GLuint tex_key;
unsigned char* image_key = NULL;
static GLuint tex_check;
unsigned char* image_check = NULL;

int iheight, iwidth;

GLfloat flat_ambient;
GLfloat fill01_ambient;
GLfloat fill02_ambient;
GLfloat key_ambient;

GLfloat mix;
GLfloat mixColor_r;
GLfloat mixColor_g;
GLfloat mixColor_b;

GLfloat fill01_r;
GLfloat fill01_g;
GLfloat fill01_b;

GLfloat fill02_r;
GLfloat fill02_g;
GLfloat fill02_b;

GLfloat key_r;
GLfloat key_g;
GLfloat key_b;

GLfloat bilinearFilterEnabled = 0.0;

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

void init(){
	
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   
  
   // Cargando Textura
   glGenTextures(1, &texflat);
   glBindTexture(GL_TEXTURE_2D, texflat);
  
   
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   imageflat = glmReadPPM("baked_flat.ppm", &iwidth, &iheight);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidth, iheight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageflat);

   glGenTextures(1, &tex_fill01);
   glBindTexture(GL_TEXTURE_2D, tex_fill01);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   image_fill01 = glmReadPPM("baked_fill01.ppm", &iwidth, &iheight);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidth, iheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image_fill01);

   glGenTextures(1, &tex_fill02);
   glBindTexture(GL_TEXTURE_2D, tex_fill02);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   image_fill02 = glmReadPPM("baked_fill02.ppm", &iwidth, &iheight);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidth, iheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image_fill02);

   glGenTextures(1, &tex_key);
   glBindTexture(GL_TEXTURE_2D, tex_key);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   /*
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   */
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   image_key = glmReadPPM("baked_keyrabbit.ppm", &iwidth, &iheight);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidth, iheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image_key);

   glGenTextures(1, &tex_check);
   glBindTexture(GL_TEXTURE_2D, tex_check);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   image_check = glmReadPPM("baked_checker.ppm", &iwidth, &iheight);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iwidth, iheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image_check);

   shader = SM.loadfromFile("texture.vert","texture.frag"); // load (and compile, link) from file
  		  if (shader==0) 
			  std::cout << "Error Loading, compiling or linking shader\n";
   
	flat_ambient = 0;
	fill01_ambient = 0;
	fill02_ambient = 0;
	key_ambient = 0;
	fill01_r = 0.5;
	fill01_g = 0.5;
	fill01_b = 0.5;
	fill02_r = 0.5;
	fill02_g = 0.5;
	fill02_b = 0.5;
	key_r = 0.5;
	key_g = 0.5;
	key_b = 0.5;
	mix = 0;
	mixColor_r = 1;
	mixColor_g = 1;
	mixColor_b = 1;

}



void Keyboard(unsigned char key, int x, int y)
{


  switch (key)
  {
	case '1':
		flat_ambient += 0.05;
		break;
	case '2':
		if (flat_ambient >= 0.04) flat_ambient -= 0.05;
		break;
	case 'q':
		fill01_ambient += 0.05;
		break;
	case 'w':
		if (fill01_ambient >= 0.04) fill01_ambient -= 0.05;
		break;
	case 'a':
		fill02_ambient += 0.05;
		break;
	case 's':
		if (fill02_ambient >= 0.04) fill02_ambient -= 0.05;
		break;
	case 'z':
		key_ambient += 0.05;
		break;
	case 'x':
		if (key_ambient >= 0.04) key_ambient -= 0.05;
		break;
	case 'e':
		fill01_r += 0.05;
		break;
	case 'r':
		fill01_g += 0.05;
		break;
	case 't':
		fill01_b += 0.05;
		break;
	case 'y':
		if (fill01_r >= 0.04) fill01_r -= 0.05;
		break;
	case 'u':
		if (fill01_g >= 0.04) fill01_g -= 0.05;
		break;
	case 'i':
		if (fill01_b >= 0.04) fill01_b -= 0.05;
		break;
	case 'd':
		fill02_r += 0.05;
		break;
	case 'f':
		fill02_g += 0.05;
		break;
	case 'g':
		fill02_b += 0.05;
		break;
	case 'h':
		if (fill02_r >= 0.04) fill02_r -= 0.05;
		break;
	case 'j':
		if (fill02_g >= 0.04) fill02_g -= 0.05;
		break;
	case 'k':
		if (fill02_b >= 0.04) fill02_b -= 0.05;
		break;
	case 'c':
		key_r += 0.05;
		break;
	case 'v':
		key_g += 0.05;
		break;
	case 'b':
		key_b += 0.05;
		break;
	case 'n':
		if (key_r >= 0.04) key_r -= 0.05;
		break;
	case 'm':
		if (key_g >= 0.04) key_g -= 0.05;
		break;
	case ',':
		if (key_b >= 0.04) key_b -= 0.05;
		break;
	case '3':
		mix = 0.3;
		mixColor_r = 5;
		mixColor_g = 0;
		mixColor_b = 0;
		break;
	case '4':
		mix = 0.3;
		mixColor_r = 0;
		mixColor_g = 5;
		mixColor_b = 0;
		break;
	case '5':
		mix = 0.3;
		mixColor_r = 0;
		mixColor_g = 0;
		mixColor_b = 5;
		break;
	case '6':
		mix = 0.3;
		mixColor_r = 5;
		mixColor_g = 0;
		mixColor_b = 5;
		break;
	case 'o':
		bilinearFilterEnabled = 1.0;
		break;
	case 'p':
		bilinearFilterEnabled = 0.0;
		break;
	default:
		break;
  }
  
  glutPostRedisplay();
}

void recursive_render (const aiScene *sc, const aiNode* nd)
{
	unsigned int i;
	unsigned int n = 0, t;
	aiMatrix4x4 m = nd->mTransformation;

	// update transform
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);

	// draw all meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n) {
		
		const aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];

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
	for (n = 0; n < nd->mNumChildren; ++n) {		
		recursive_render(sc, nd->mChildren[n]);
	}

	glPopMatrix();
}

void render(){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat zExtent, xExtent, xLocal, zLocal;
    int loopX, loopZ;

	glLoadIdentity ();                       
	gluLookAt (0.0, 37.0, 98.0, 0.0, 30.0, 0.0, 0.0, 1.0, 0.0);
	
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_LINE_SMOOTH );	

			
	glPushMatrix();

	if (shader) shader->begin();

	
	shader->setTexture("stexflat", texflat,0);
	shader->setTexture("tex_fill01", tex_fill01,1);
	shader->setTexture("tex_fill02", tex_fill02,2);
	shader->setTexture("tex_key", tex_key,3);
	shader->setTexture("tex_check", tex_check,4);

	shader->setUniform1f("_flat_ambient" , flat_ambient);
	shader->setUniform1f("_fill01_ambient" , fill01_ambient);
	shader->setUniform1f("_fill02_ambient" , fill02_ambient);
	shader->setUniform1f("_key_ambient" , key_ambient);
	shader->setUniform1f("_fill01_r" , fill01_r);
	shader->setUniform1f("_fill01_g" , fill01_g);
	shader->setUniform1f("_fill01_b" , fill01_b);
	shader->setUniform1f("_fill02_r" , fill02_r);
	shader->setUniform1f("_fill02_g" , fill02_g);
	shader->setUniform1f("_fill02_b" , fill02_b);
	shader->setUniform1f("_key_r" , key_r);
	shader->setUniform1f("_key_g" , key_g);
	shader->setUniform1f("_key_b" , key_b);
	shader->setUniform1f("_mix" , mix);
	shader->setUniform1f("_mixColor_r" , mixColor_r);
	shader->setUniform1f("_mixColor_g" , mixColor_g);
	shader->setUniform1f("_mixColor_b" , mixColor_b);
	shader->setUniform1f("_bilinearFilterEnabled" , bilinearFilterEnabled);

	
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
	
	
	/*
	glPushMatrix();
	glLoadIdentity();	
	glTranslatef(1.1, 0.5, -3.0);
	glutSolidSphere(0.2f,30,30);
	glPopMatrix();*/
	 

	if (shader) shader->end();

	

	
	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);
	glutSwapBuffers();
}

void animacion(int value) {
	
	glutTimerFunc(10,animacion,1);
    glutPostRedisplay();
	
}

void get_bounding_box_for_node (const aiNode* nd, 	aiVector3D* min, 	aiVector3D* max, 	aiMatrix4x4* trafo){
	aiMatrix4x4 prev;
	unsigned int n = 0, t;

	prev = *trafo;
	aiMultiplyMatrix4(trafo,&nd->mTransformation);

	for (; n < nd->mNumMeshes; ++n) {
		const aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t) {

			aiVector3D tmp = mesh->mVertices[t];
			aiTransformVecByMatrix4(&tmp,trafo);

			min->x = aisgl_min(min->x,tmp.x);
			min->y = aisgl_min(min->y,tmp.y);
			min->z = aisgl_min(min->z,tmp.z);

			max->x = aisgl_max(max->x,tmp.x);
			max->y = aisgl_max(max->y,tmp.y);
			max->z = aisgl_max(max->z,tmp.z);
		}
	}

	for (n = 0; n < nd->mNumChildren; ++n) {
		get_bounding_box_for_node(nd->mChildren[n],min,max,trafo);
	}
	*trafo = prev;
}

void get_bounding_box (aiVector3D* min, aiVector3D* max){
	aiMatrix4x4 trafo;
	aiIdentityMatrix4(&trafo);
	
	min->x = min->y = min->z =  1e10f;
	max->x = max->y = max->z = -1e10f;
	get_bounding_box_for_node(scene->mRootNode,min,max,&trafo);
}

int loadasset (const char* path){
	// we are taking one of the postprocessing presets to avoid
	// spelling out 20+ single postprocessing flags here.
	scene = aiImportFile(path,aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene) {
		get_bounding_box(&scene_min,&scene_max);
		scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
		scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
		scene_center.z = (scene_min.z + scene_max.z) / 2.0f;
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
