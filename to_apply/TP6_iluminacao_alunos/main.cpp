
#include <stdlib.h>
#include <stdio.h>
#include "RgbImage.h"
#include <math.h>
#include <time.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>


//==================================================================== Definir cores
#define AZUL     0.0, 0.0, 1.0, 1.0
#define VERMELHO 1.0, 0.0, 0.0, 1.0
#define AMARELO  1.0, 1.0, 0.0, 1.0
#define VERDE    0.0, 1.0, 0.0, 1.0
#define LARANJA  0.8, 0.6, 0.1, 1.0
#define WHITE    1.0, 1.0, 1.0, 1.0
#define BLACK    0.0, 0.0, 0.0, 1.0
#define GRAY1    0.2, 0.2, 0.2, 1.0
#define GRAY2    0.93, 0.93, 0.93, 1.0

#include "materiais.h"

//================================================================================
//===========================================================Variaveis e constantes

//------------------------------------------------------------ Sistema Coordenadas
GLfloat  xC=16.0, zC=15.0;
GLint    wScreen=800, hScreen=600;
char     texto[30];
//------------------------------------------------------------ Observador
GLfloat  PI = 3.14159;
GLfloat  rVisao=3.0, aVisao=0.5*PI, incVisao=0.1;
GLfloat  obsPini[] ={1, 0.25, 0.5*xC};
GLfloat  obsPfin[] ={obsPini[0]-rVisao*cos(aVisao), obsPini[1], obsPini[2]-rVisao*sin(aVisao)};


//================================================================================
////�����������������������������������������������������     Iluminacao

//------------------------------------------------------------ Global (ambiente)
GLint   noite=1;
GLfloat luzGlobalCor[4]={1.0,1.0,1.0,1.0};   // 

//================================================================================
//����������������������������������������������������� Materiais: Esfera de jade 
GLint   ligaLuz=1;
GLfloat matAmbiente[] = { 0.135 ,0.2225 ,0.1575 };	  
GLfloat matDifusa[]   = {   0.54 ,0.89 ,0.63 };	  
GLfloat matEspecular[]= { 0.316228 ,0.316228 ,0.316228 }; 
GLint   especMaterial = 0.1 *128;



//************************************************** Poligno
//  Definir as coordenas do poligono
//************************************************** Poligno

//================================================================================
//=========================================================================== INIT
//================================================================================



//��������������������������������������������������������� Iluminacao
void initLights(void){
	//����������������������������������������������������� Ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCor);
	
   //����������������������������������������������������� Tecto
   	GLfloat localCor[4] ={0.4,0.4,0.0,1.0}; 
	GLfloat localCorDif[4] ={ 1, 1, 1, 1.0}; 
	GLfloat localPos[4] ={xC/2, 10.0, xC/2, 1.0};
	GLfloat localAttCon =1.0;
	GLfloat localAttLin =0.05;
	GLfloat localAttQua =0.0;
   
	glLightfv(GL_LIGHT0, GL_POSITION,      localPos );   
	glLightfv(GL_LIGHT0, GL_AMBIENT,       localCor );   
	glLightfv(GL_LIGHT0, GL_DIFFUSE,       localCorDif );   
	glLightf (GL_LIGHT0, GL_CONSTANT_ATTENUATION, localAttCon);
	glLightf (GL_LIGHT0, GL_LINEAR_ATTENUATION,   localAttLin) ;         
	glLightf (GL_LIGHT0, GL_QUADRATIC_ATTENUATION,localAttQua) ;          
	

}

void init(void)
{
	glClearColor(WHITE);
	glShadeModel(GL_SMOOTH);
	

	initLights();
	glEnable(GL_LIGHTING);  
	glEnable(GL_LIGHT0);  
	glEnable(GL_DEPTH_TEST);
}
	

//================================================================================
//======================================================================== DISPLAY
void desenhaTexto(char *string, GLfloat x, GLfloat y, GLfloat z) 
{  
	glRasterPos3f(x,y,z); 
	while(*string)
	  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *string++); 
}


void drawScene(){
	
	//============================================Eixos
	glBegin(GL_LINES);						
		glVertex3i(0,0,-xC); 
		glVertex3i(0,0, xC); 		
	glEnd();
	glBegin(GL_LINES);						
		glVertex3i(0,-xC,0); 
		glVertex3i(0,xC,0); 		
	glEnd();
	glBegin(GL_LINES);						
		glVertex3i(-xC,0,0); 
		glVertex3i( xC,0,0); 		
	glEnd();

	//************************************************** Poligono
	//  As cores dos objectos devem ser definidas:
	//		1. Cor usando o glColorMAterial
	//		2. Propriedades materiais (a implementar)
	//************************************************** Poligno
	

	//************************************************** Poligono
	//  Construir um poligono de uma dada cor
	//************************************************** Poligono
	

	glEnable(GL_LIGHTING);
	
	glPushMatrix();
		glTranslatef( 2, 0, 2);
		glColor4f(AMARELO);
		glutSolidSphere(1.2, 250, 250);
	glPopMatrix();
		
	
	glutPostRedisplay();
}


GLvoid resize(GLsizei width, GLsizei height)
{
	wScreen=width;
	hScreen=height;
	drawScene();	
}


void drawOrientacao()
{
	glPushMatrix();
		glColor4f(VERDE);
		glTranslatef (obsPini[0],obsPini[1],obsPini[2]);		
		glutSolidCube(0.75);
	glPopMatrix();
	glPushMatrix();
		glColor4f(LARANJA);
		glTranslatef (obsPfin[0],obsPfin[1],obsPfin[2]);		
		glutSolidCube(0.5);
	glPopMatrix();
	//****************************************************************************
	//  Direccao do FOCO=lanterna
	//****************************************************************************
}	

void display(void){
  
	if (noite)
		glClearColor(GRAY1);
	else
		glClearColor(GRAY2);
	if (ligaLuz)
		glEnable(GL_LIGHT0);
	else
		glDisable(GL_LIGHT0);
	
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	//================================================================= Viewport1
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glViewport (0, hScreen/4, wScreen/4, hScreen/4);
  	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho (-xC,xC, -xC,xC, -zC,zC);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
	gluLookAt( 0, 10,0, 0,0,0, 0, 0, -1);
	//--------------------- desenha objectos
	drawScene(); 
	drawOrientacao(); 
	//--------------------- Informacao
	glColor3f(1,0,0);
	sprintf(texto, "%d - Noite", noite);
	desenhaTexto(texto,-12,1,-6);
	sprintf(texto, "%d - Tecto", ligaLuz);
	desenhaTexto(texto,-12,1,-9);
	
	//================================================================= Viewport2
	glEnable(GL_LIGHTING);  
	glViewport (wScreen/4, hScreen/4, 0.75*wScreen, 0.75*hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(99.0, wScreen/hScreen, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(obsPini[0], obsPini[1], obsPini[2], obsPfin[0], obsPfin[1], obsPfin[2], 0, 1, 0);
	
	drawScene();
	
	
	glutSwapBuffers();
}





//======================================================= EVENTOS
void keyboard(unsigned char key, int x, int y){

	switch (key) {
	//--------------------------- Direccao da Lanterna
	//****************************************************************************
	//  A lanterna pode-se movimentar usando as teclas 'S', 'D', 'E', 'C'
	//****************************************************************************
	case 's':
	case 'S':
	break;
	case 'd':
	case 'D':
	break;
	case 'e':
	case 'E':
	break;
	case 'c':
	case 'C':
	break;
	//--------------------------- Dia/noite : definir intensidades da luz
	case 'n':
	case 'N':
		noite=!noite;
		if (noite)
			{ luzGlobalCor[0]=1.0;   luzGlobalCor[1]=1.0;   luzGlobalCor[2]=1.0; }
		else
			{ luzGlobalCor[0]=0.4;   luzGlobalCor[1]=0.4;   luzGlobalCor[2]=0.4; }
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCor);
		glutPostRedisplay();	
	break;
		//--------------------------- Iluminacaoda sala
	case 't':
	case 'T':
		ligaLuz=!ligaLuz;
		glutPostRedisplay();	
	break;
//--------------------------- Foco
	case 'f':
	case 'F':
	break;
	//--------------------------- ColorMaterial
	case 'm':
	case 'M':
	break;
	//--------------------------- Escape
	case 27:
		exit(0);
    break;
	
  }
}

void updateVisao(){
	obsPfin[0] =obsPini[0]+rVisao*cos(aVisao);
	obsPfin[2] =obsPini[2]-rVisao*sin(aVisao);
	glutPostRedisplay();	
}

void teclasNotAscii(int key, int x, int y)
{
    if(key == GLUT_KEY_UP) {
		obsPini[0]=obsPini[0]+incVisao*cos(aVisao); 
		obsPini[2]=obsPini[2]-incVisao*sin(aVisao); 
	}
	if(key == GLUT_KEY_DOWN) {
		obsPini[0]=obsPini[0]-incVisao*cos(aVisao); 
		obsPini[2]=obsPini[2]+incVisao*sin(aVisao); 
	} 
	if(key == GLUT_KEY_LEFT) {
		aVisao = (aVisao + 0.1) ;
	}
	if(key == GLUT_KEY_RIGHT) {
		aVisao = (aVisao - 0.1) ;
	}
	updateVisao();
}

//======================================================= MAIN
int main(int argc, char** argv){

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	glutInitWindowSize (wScreen, hScreen); 
	glutInitWindowPosition (400, 100); 
	glutCreateWindow ("{jh,pjmm}@dei.uc.pt-CG  (left,right,up,down) - (N,T) ");
  
	init();
	glutSpecialFunc(teclasNotAscii); 
	glutReshapeFunc(resize);
	glutDisplayFunc(display); 
	glutKeyboardFunc(keyboard);
	
	glutMainLoop();

	return 0;
}

