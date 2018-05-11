
// jorge henriques / pedro martins - 2017/18

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include "RgbImage.h"
#include <GL\glut.h>


//--------------------------------- Definir cores
#define AZUL     0.0, 0.0, 1.0, 1.0
#define VERMELHO 1.0, 0.0, 0.0, 1.0
#define AMARELO  1.0, 1.0, 0.0, 1.0
#define VERDE    0.0, 1.0, 0.0, 1.0
#define LARANJA  1.0, 0.5, 0.1, 1.0
#define WHITE    1.0, 1.0, 1.0, 1.0
#define BLACK    0.0, 0.0, 0.0, 1.0
#define GRAY     0.9, 0.92, 0.29, 1.0
#define PI		 3.14159

//………………………………………………………………………………………………………………………………………………………………………………………… A variar no programa
bool 		Focos[]	= {1,1};		//.. Dois Focos ligados ou desligados
bool		light=TRUE;				//.. Luz global
GLfloat		anguloFoco= 10.0;		//.. angulo inicial do foco
GLfloat		anguloINC = 3.0;		//.. incremento
GLfloat		anguloMIN = 3.0;		//.. minimo
GLfloat		anguloMAX = 70.0;		//.. maximo

	
//………………………………………………………………………………………………………………………………… Textura
RgbImage  imag;
GLuint    tex;
GLuint    texture[1];

GLfloat Pos1[]= {-1.0f, 0.0f,  1.0f, 1.0f };
GLfloat Pos2[]= { 1.0f, 0.0f,  1.0f, 1.0f };

//…………………………………………………………………………………………………………………………………
GLint	  dim=64;   //numero divisoes da grelha


void defineTexturas()
{   
	//……………………………………………………………………………………………………………………………  Textura
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	imag.LoadBmpFile("caracol.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
}



void defineLuzes()
{
	GLfloat ambient[]		= { 1.0f, 1.0f,  1.0f, 1.0f};	//……… branco
	
	GLfloat Foco_direccao[]	= { 0, 0, -1   };	//……… -Z
	GLfloat Foco1_cor[]		= { 0, 1,  0, 1};	//……… Cor da luz 1
	GLfloat Foco2_cor[]		= { 1, 0,  0, 1};	//……… Cor da luz 2
	GLfloat Foco_ak			= 1.0;
	GLfloat Foco_al			= 0.05f;
	GLfloat Foco_aq			= 0.0f;
	GLfloat Foco_Expon		= 2.0;		// Foco, SPOT_Exponent
	
	//……………………………………………………………………………………………………………………………Ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambient);
	
	//……………………………………………………………………………………………………………………………Foco Esquerda
	glLightfv(GL_LIGHT0,GL_POSITION,				Pos1);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,					Foco1_cor);
	 glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,	Foco_ak);
	 glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,		Foco_al);
	 glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,	Foco_aq);
	 glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,				anguloFoco);
	glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,			Foco_direccao);
	 glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,			Foco_Expon);
	
	 //……………………………………………………………………………………………………………………………Foco Direita
	glLightfv(GL_LIGHT1,GL_POSITION,				Pos2);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,					Foco2_cor);
	 glLightf(GL_LIGHT1,GL_CONSTANT_ATTENUATION,	Foco_ak);
	 glLightf(GL_LIGHT1,GL_LINEAR_ATTENUATION,		Foco_al);
	 glLightf(GL_LIGHT1,GL_QUADRATIC_ATTENUATION,	Foco_aq);
	 glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,				anguloFoco);
	glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,			Foco_direccao);
	 glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,			Foco_Expon);

}



void init(void)
{
	//……………………………………………………………………………………………………………………………Apagar
	glClearColor(BLACK);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);
	
	//……………………………………………………………………………………………………………………………Texturas
	defineTexturas( );
	glEnable(GL_TEXTURE_2D);

	//……………………………………………………………………………………………………………………………Luzes
	defineLuzes( );
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
}




void desenha()
{
	int				i,j;
	float			med_dim=(float)dim/2;


	//===========================================================
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0,0,550,550);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(20.0f,1.0f,0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt( 0, 0, 10, 0,0,0, 0, 1, 0);


	//============================================Esferasverde e vermelha
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	
	//…………………………………………………………………………………Verde
	if (Focos[0]) {
	glPushMatrix();
		glColor3f(0.0f,1.0f,0.0f);
		glTranslatef(Pos1[0],Pos1[1],Pos1[2]);			
		glutSolidSphere(0.1f, 100, 100);
	glPopMatrix();
	}
	//…………………………………………………………………………………Vermelha
	if (Focos[1]) {
	glPushMatrix();
		glColor3f(1.0f,0.0f,0.0f);
		glTranslatef(Pos2[0],Pos2[1],Pos2[2]);
		glutSolidSphere(0.1f, 100, 100);
	glPopMatrix();
	}
		
	
	//============================================ Grelha de polionos (dim*dim)
	//============================================ O ponto minimo   e (0,0), o maximo (2,2)
	//============================================ A textura minimo e (0,0), o maximo (1,1)
	if (light){
		defineLuzes( );
		glEnable(GL_LIGHTING);
	}
	
		
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	glColor3f(1.0f,1.0f,1.0f);
	glPushMatrix();
		glTranslatef(-1.0,-1.0,0);  // meio do poligono 
		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			for (i=0;i<dim;i++)
				for (j=0;j<dim;j++)
				{
					//glNormal(0,1,0);
					glTexCoord2f((float)j/dim,(float)i/dim);
					glVertex3d((float)j/med_dim,(float)i/med_dim,0);
					glTexCoord2f((float)(j+1)/dim,(float)i/dim);
					glVertex3d((float)(j+1)/med_dim,(float)i/med_dim,0);
					glTexCoord2f((float)(j+1)/dim,(float)(i+1)/dim);
					glVertex3d((float)(j+1)/med_dim,(float)(i+1)/med_dim,0);
					glTexCoord2f((float)j/dim,(float)(i+1)/dim);
					glVertex3d((float)j/med_dim,(float)(i+1)/med_dim,0);
				}
		glEnd();
	glPopMatrix();
	
		
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Actualizacao
	glutSwapBuffers();

}




//======================================================= EVENTOS
void keyboard(unsigned char key, int x, int y){
	
	switch (key) {

		case 'G':
		case 'g':
			Focos[0]=!Focos[0];
			if (Focos[0]==0)
				glDisable(GL_LIGHT0);
			else
				glEnable(GL_LIGHT0);
			glutPostRedisplay();
			break;
		case 'R':
		case 'r':
			Focos[1]=!Focos[1];
			if (Focos[1]==0)
				glDisable(GL_LIGHT1);
			else
				glEnable(GL_LIGHT1);
			glutPostRedisplay();
			break;		

		case 'a':
		case 'A':
			dim=2*dim;
			if (dim>256) dim=256;
			glutPostRedisplay();
		break;
		case 'z':
		case 'Z':
			dim=0.5*dim;
			if (dim<1) dim=1;
			glutPostRedisplay();
		break;
	case 'l':
	case 'L':
		light=!light;
		glutPostRedisplay();
		break;
	
	//--------------------------- Escape
	case 27:
		exit(0);
		break;	
  }

}


void teclasNotAscii(int key, int x, int y){

	if(key == GLUT_KEY_UP) {
		anguloFoco=anguloFoco+anguloINC;
		if (anguloFoco>anguloMAX)
			anguloFoco=anguloMAX;
	}
	if(key == GLUT_KEY_DOWN) {
		anguloFoco=anguloFoco-anguloINC;
		if (anguloFoco<anguloMIN)
			anguloFoco=anguloMIN;	
	}
	glutPostRedisplay();	
}



//======================================================= MAIN
int main(int argc, char** argv){

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize (550, 550); 
	glutInitWindowPosition (300, 100); 
	glutCreateWindow ("{jh,pjmm, jpa}  'G/R'-Ligar  'A:Z'-Grelha  'Dow:Up'-Angulo   'L'-Ambiente");
  
	
	init();	

	glutSpecialFunc(teclasNotAscii); 
	glutDisplayFunc(desenha); 
	glutKeyboardFunc(keyboard);
	
	glutMainLoop();

	return 0;
}
