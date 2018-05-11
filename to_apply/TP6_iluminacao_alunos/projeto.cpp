#include <stdio.h>
#include "RgbImage.h"
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdio.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
//--------------------------------- Definir cores
#define AZUL     0.0, 0.0, 1.0, 1.0
#define VERMELHO 1.0, 0.0, 0.0, 1.0
#define AMARELO  1.0, 1.0, 0.0, 1.0
#define VERDE    0.0, 1.0, 0.0, 1.0
#define LARANJA  1.0, 0.5, 0.1, 1.0
#define WHITE    1.0, 1.0, 1.0, 1.0
#define BLACK    0.0, 0.0, 0.0, 1.0
#define GRAY     0.9, 0.92, 0.29, 1.0
#define PI       3.14159


GLfloat  cinzento[] = { 0.5 ,0.5 ,0.5 };
GLfloat luzAmbiente[4] = { 1,1,1,1 };
//
int w=0,s=0,d=0,a=0;

int pressed[6] = {0,0,0,0,0,0}; //moves

GLint        wScreen=800, hScreen=600;        //.. janela (pixeis)

//Coordenadas da lata
float canWalk[]={5,5,20};

GLUquadricObj *quadratic;


//Parametros de visao do observador
GLfloat  rVisao=3.0, aVisao=0.5*PI, incVisao=0.1, aVisaoY=0;

//Posição inicial do observador
GLfloat  obsPini[] ={10, 3, 2};
GLfloat  obsPfin[] ={obsPini[0]+rVisao*cos(aVisao), obsPini[1]+rVisao*sin(aVisaoY), obsPini[2]+rVisao*sin(aVisao)};

//Identificador das Texturas
RgbImage imag;
GLuint textures[8];

void drawEixos()
{
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo X
    glColor4f(VERMELHO);
    glBegin(GL_LINES);
    glVertex3i( 0, 0, 0);
    glVertex3i(10, 0, 0);
    glEnd();
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo Y
    glColor4f(VERDE);
    glBegin(GL_LINES);
    glVertex3i(0,  0, 0);
    glVertex3i(0, 10, 0);
    glEnd();
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo Z
    glColor4f(AZUL);
    glBegin(GL_LINES);
    glVertex3i( 0, 0, 0);
    glVertex3i( 0, 0,10);
    glEnd();
    
}
void draw_cylinder(GLfloat radius, GLfloat height, GLubyte R, GLubyte G, GLubyte B){  //Fui buscar à net
    GLfloat x              = 0.0;
    GLfloat y              = 0.0;
    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.1;
    
    /** Draw the tube */
    //glEnable(GL_TEXTURE_2D);
    //mglBindTexture(GL_TEXTURE_2D,textures[2]);
    glBegin(GL_QUAD_STRIP);
        angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y , height);
            glVertex3f(x, y , 0.0);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
        glVertex3f(radius, 0.0, 0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    
    
    /** Draw the circle on top of cylinder */
    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D,textures[2]);
    glBegin(GL_POLYGON);
    angle = 0.0;
    while( angle < 2*PI ) {
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y , height);
        angle = angle + angle_stepsize;
    }
    glVertex3f(radius, 0.0, height);
    glEnd();
    //glDisable(GL_TEXTURE_2D);
}

void drawCubo1(){
    glPushMatrix();
    
        glBegin(GL_QUADS);
            //UpperFace
            glNormal3d(0, 1, 0);
            glVertex3f( 0.5f,  0.5f, -0.5f);
            glVertex3f(-0.5f,  0.5f, -0.5f);
            glVertex3f(-0.5f,  0.5f,  0.5f);
            glVertex3f( 0.5f,  0.5f,  0.5f);
    
            //BottomFace
            glNormal3d(0, -1, 0);
            glVertex3f( 0.5f, -0.5f, -0.5f);
            glVertex3f( 0.5f, -0.5f,  0.5f);
            glVertex3f(-0.5f, -0.5f,  0.5f);
            glVertex3f(-0.5f, -0.5f, -0.5f);
    
            //FrontFace
            glNormal3d(-1, 0, 0);
            glVertex3f(-0.5f,  0.5f, -0.5f);
            glVertex3f(-0.5f, -0.5f, -0.5f);
            glVertex3f(-0.5f, -0.5f,  0.5f);
            glVertex3f(-0.5f,  0.5f,  0.5f);
    
            //BackFace
            glNormal3d(1, 0, 0);
            glVertex3f( 0.5f,  0.5f,  0.0f);
            glVertex3f( 0.5f,  0.0f,  0.0f);
            glVertex3f( 0.5f,  0.0f,  0.5f);
            glVertex3f( 0.5f,  0.5f,  0.5f);
    
            //RightFace
            glNormal3d(0, 0, 1);
            glVertex3f(-0.5f,  0.5f,  0.5f);
            glVertex3f(-0.5f, -0.5f,  0.5f);
            glVertex3f( 0.5f, -0.5f,  0.5f);
            glVertex3f( 0.5f,  0.5f,  0.5f);
    
            //LeftFace
            glNormal3d(0, 0, -1);
            glVertex3f(-0.5f,  0.5f, -0.5f);
            glVertex3f(-0.5f, -0.5f, -0.5f);
            glVertex3f( 0.5f, -0.5f, -0.5f);
            glVertex3f( 0.5f,  0.5f, -0.5f);
        glEnd();
    glPopMatrix();
}
void criaDefineTexturas(void){
    //----------------------------------------- Chao z=0
    glGenTextures(1, &textures[0]);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    imag.LoadBmpFile("chao.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());
    
    glGenTextures(1, &textures[1]);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    imag.LoadBmpFile("ola.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());
    
    glGenTextures(1, &textures[2]);
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    imag.LoadBmpFile("up.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());
    
    glGenTextures(1, &textures[3]);
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    imag.LoadBmpFile("lt.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());
    
    glGenTextures(1, &textures[4]);
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    imag.LoadBmpFile("ft.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());
    
    glGenTextures(1, &textures[5]);
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    imag.LoadBmpFile("rt.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());
    
    glGenTextures(1, &textures[6]);
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    imag.LoadBmpFile("bk.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());
    
    glGenTextures(1, &textures[7]);
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    imag.LoadBmpFile("dn.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());
}
void init(void){
    glClearColor(WHITE);
    glEnable(GL_DEPTH_TEST);  //activa o zbuffer para ter em conta a profundidade
    glShadeModel(GL_SMOOTH);
    criaDefineTexturas();
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_TEXTURE_2D);
    
    
    
}
void drawFloor(GLfloat comp, GLfloat larg){
    float scale = comp/2;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,textures[0]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3i( 0, 0, 0);
        glTexCoord2f(0.0f, scale); glVertex3i(0, 0, larg);
        glTexCoord2f(scale, scale); glVertex3i(comp, 0, larg);
        glTexCoord2f(scale, 0.0f); glVertex3i(comp, 0, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
void drawCanEngine(){
    glPushMatrix();
        if(canWalk[0]>20)
            canWalk[0]=5;
        glTranslatef(5,1,canWalk[0]);
        canWalk[0]+=0.02;
        glRotatef(-90, 1,0,0);
        draw_cylinder(0.3, 1.0, 255, 160, 100);
    glPopMatrix();
    
    glPushMatrix();
        if(canWalk[1]>20)
            canWalk[1]=5;
        glTranslatef(canWalk[1],1,20);
        canWalk[1]+=0.02;
        glRotatef(-90, 1,0,0);
        draw_cylinder(0.3, 1.0, 255, 160, 100);
    glPopMatrix();
    
    glPushMatrix();
        if(canWalk[2]<5)
            canWalk[2]=20;
        glTranslatef(20,1,canWalk[2]);
        canWalk[2]-=0.02;
        glRotatef(-90, 1,0,0);
        draw_cylinder(0.3, 1.0, 255, 160, 100);
    glPopMatrix();
    
    glutPostRedisplay();
}

void drawScene(void){
    //Desenha chão
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    
    drawFloor(30,30);
    
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT, cinzento);
        drawCanEngine();
    glPopMatrix();
    
    drawEixos();
}
void updateKeys(){
	if(pressed[0]==1){
		obsPini[0]=(obsPini[0]-incVisao*cos(aVisao));
        obsPini[2]=(obsPini[2]+incVisao*sin(aVisao));
       
        obsPfin[0]=(obsPini[0]-rVisao*cos(aVisao));
        obsPfin[2]=(obsPini[2]+rVisao*sin(aVisao));
	}
	if(pressed[1]==1){
		obsPini[0]=(obsPini[0]+incVisao*cos(aVisao));
		obsPini[2]=(obsPini[2]-incVisao*sin(aVisao));
            
		obsPfin[0]=(obsPini[0]-rVisao*cos(aVisao));
		obsPfin[2]=(obsPini[2]+rVisao*sin(aVisao));
	}
	if(pressed[2]==1){
		aVisao=aVisao+0.05;
        obsPfin[0]=obsPini[0]-rVisao*cos(aVisao);
        obsPfin[2]=obsPini[2]+rVisao*sin(aVisao);
	}
	if(pressed[3]==1){
		aVisao=aVisao-0.05;
        obsPfin[0]=obsPini[0]-rVisao*cos(aVisao);
        obsPfin[2]=obsPini[2]+rVisao*sin(aVisao);
	}
	if(pressed[4]==1){
		if(aVisaoY<20){
			aVisaoY+=0.05;
			obsPfin[1]=obsPini[1]+aVisaoY;
		}
	}
	if(pressed[5]==1){
		if(aVisaoY>-20){
			aVisaoY-=0.05;
			obsPfin[1]=obsPini[1]+aVisaoY;
		}
	}
	glutPostRedisplay();
}
void drawSkybox(float size){
    
    size = size/2.0f;
    
    glEnable(GL_TEXTURE_2D);
    //Left
    glBindTexture(GL_TEXTURE_2D,textures[1]);
    glPushMatrix();
    
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,0.0f); glVertex3i(obsPini[0]-size,obsPini[1]-size,obsPini[2]-size);
    glTexCoord2f(1.0f,0.0f); glVertex3i(obsPini[0]+size,obsPini[1]-size,obsPini[2]-size);
    glTexCoord2f(1.0f,1.0f); glVertex3i(obsPini[0]+size,obsPini[1]+size,obsPini[2]-size);
    glTexCoord2f(0.0f,1.0f); glVertex3i(obsPini[0]-size,obsPini[1]+size,obsPini[2]-size);
    glEnd();
    
    glPopMatrix();
    
    //Front
    glBindTexture(GL_TEXTURE_2D,textures[3]);
    glPushMatrix();
    
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,0.0f); glVertex3i(obsPini[0]+size,obsPini[1]-size,obsPini[2]-size);
    glTexCoord2f(1.0f,0.0f); glVertex3i(obsPini[0]+size,obsPini[1]-size,obsPini[2]+size);
    glTexCoord2f(1.0f,1.0f); glVertex3i(obsPini[0]+size,obsPini[1]+size,obsPini[2]+size);
    glTexCoord2f(0.0f,1.0f); glVertex3i(obsPini[0]+size,obsPini[1]+size,obsPini[2]-size);
    glEnd();
    
    glPopMatrix();
    
    //Right
    glBindTexture(GL_TEXTURE_2D,textures[0]);
    glPushMatrix();
    
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,0.0f); glVertex3i(obsPini[0]+size,obsPini[1]-size,obsPini[2]+size);
    glTexCoord2f(1.0f,0.0f); glVertex3i(obsPini[0]-size,obsPini[1]-size,obsPini[2]+size);
    glTexCoord2f(1.0f,1.0f); glVertex3i(obsPini[0]-size,obsPini[1]+size,obsPini[2]+size);
    glTexCoord2f(0.0f,1.0f); glVertex3i(obsPini[0]+size,obsPini[1]+size,obsPini[2]+size);
    glEnd();
    
    glPopMatrix();
    
    //Back
    glBindTexture(GL_TEXTURE_2D,textures[6]);
    glPushMatrix();
    
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,0.0f); glVertex3i(obsPini[0]-size,obsPini[1]-size,obsPini[2]+size);
    glTexCoord2f(1.0f,0.0f); glVertex3i(obsPini[0]-size,obsPini[1]-size,obsPini[2]-size);
    glTexCoord2f(1.0f,1.0f); glVertex3i(obsPini[0]-size,obsPini[1]+size,obsPini[2]-size);
    glTexCoord2f(0.0f,1.0f); glVertex3i(obsPini[0]-size,obsPini[1]+size,obsPini[2]+size);
    glEnd();
    
    glPopMatrix();
    
    //Top
    glBindTexture(GL_TEXTURE_2D,textures[7]);
    glPushMatrix();
    
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,0.0f); glVertex3i(obsPini[0]+size,obsPini[1]+size,obsPini[2]-size);
    glTexCoord2f(1.0f,0.0f); glVertex3i(obsPini[0]+size,obsPini[1]+size,obsPini[2]+size);
    glTexCoord2f(1.0f,1.0f); glVertex3i(obsPini[0]-size,obsPini[1]+size,obsPini[2]+size);
    glTexCoord2f(0.0f,1.0f); glVertex3i(obsPini[0]-size,obsPini[1]+size,obsPini[2]-size);
    glEnd();
    
    glPopMatrix();
    
}

void display(void){
    //Limpa buffers
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
    glEnable(GL_LIGHTING);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //glEnable(GL_LIGHTING);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(99.0, wScreen/hScreen, 0.1, 100.0);
    
    
   
    
    
    // Camara + Observador
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    updateKeys();

    //Posição do observador
    gluLookAt(obsPini[0], obsPini[1], obsPini[2], obsPfin[0], obsPfin[1], obsPfin[2], 0, 1, 0);
    
    
    //Desenha "cena"
    drawScene();
    drawSkybox(100.0f);
    //Atualiza
    glutSwapBuffers();
}


void keyboardUp(unsigned char key, int x, int y){
    
    switch (key) {
        case 'w':
        case 'W':
        	pressed[0]=0;
            break;
        case 's':
        case 'S':
        	pressed[1]=0;
            break;
        case 'a':
        case 'A':
        	pressed[2]=0;
            break;
        case 'd':
        case 'D':
        	pressed[3]=0;
            break;
        case 'o':
        case 'O':
        	pressed[4]=0;
        	break;
        case 'l':
        case 'L':
        	pressed[5]=0;
        	break;
        case 27:
            exit(0);
            break;
    }
}
void keyboardDown(unsigned char key, int x, int y){
    
    switch (key) {
        case 'w':
        case 'W':
        	pressed[0]=1;
            break;
        case 's':
        case 'S':
        	pressed[1]=1;
            break;
        case 'a':
        case 'A':
        	pressed[2]=1;
            break;
        case 'd':
        case 'D':
        	pressed[3]=1;
            break;
        case 'o':
        case 'O':
        	pressed[4]=1;
        	break;
        case 'l':
        case 'L':
        	pressed[5]=1;
        	break;
        case 27:
            exit(0);
            break;
    }
}



int main(int argc, char** argv){
    
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize (wScreen, hScreen);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Projeto Final CG 2017/2018 ");
    //glutFullScreen();
    init();
    
    //glutReshapeFunc(resize);
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    glutDisplayFunc(display);
    
    
    
    glutMainLoop();
    
    return 0;
}
