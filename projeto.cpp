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
#define PI         3.14159

//
int w=0,s=0,d=0,a=0;

GLint        wScreen=800, hScreen=600;        //.. janela (pixeis)

//Parametros de visao do observador
GLfloat  rVisao=3.0, aVisao=0.5*PI, incVisao=0.2;

//Posição inicial do observador
GLfloat  obsPini[] ={5, 2, 22.5};
GLfloat  obsPfin[] ={obsPini[0]-rVisao*cos(aVisao), obsPini[1], obsPini[2]-rVisao*sin(aVisao)};

//Identificador das Texturas
RgbImage imag;
GLuint textures[1];


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
}
void init(void){
    
    glClearColor(WHITE);
    glShadeModel(GL_SMOOTH);
    criaDefineTexturas( );
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    
}
void drawFloor(GLfloat comp, GLfloat larg){
    float scale = comp/2;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,textures[0]);
    glColor4f(WHITE);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3i( 0, 0, 0);
        glTexCoord2f(0.0f, scale); glVertex3i(0, 0, larg);
        glTexCoord2f(scale, scale); glVertex3i(comp, 0, larg);
        glTexCoord2f(scale, 0.0f); glVertex3i(comp, 0, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}


void drawScene(void){
    //Desenha chão
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    
    drawFloor(20,20);
}
void display(void){
    //Limpa buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    //Viewport2
    glEnable(GL_LIGHTING);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(99.0, wScreen/hScreen, 0.1, 100.0);
    
    // Camara + Observador
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //Posição do observador
    gluLookAt(obsPini[0], obsPini[1], obsPini[2], obsPfin[0], obsPfin[1], obsPfin[2], 0, 1, 0);
    
    
    //Desenha "cena"
    //iluminacao();
    drawScene();
    
    //Atualiza
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y){
    
    switch (key) {
        case 'w':
        case 'W':
            obsPini[0]=(obsPini[0]+incVisao*cos(aVisao));
            obsPini[2]=(obsPini[2]-incVisao*sin(aVisao));
            
            obsPfin[0]=(obsPini[0]+rVisao*cos(aVisao));
            obsPfin[2]=(obsPini[2]-rVisao*sin(aVisao));
            glutPostRedisplay();
            break;
        case 's':
        case 'S':
            obsPini[0]=(obsPini[0]-incVisao*cos(aVisao));
            obsPini[2]=(obsPini[2]+incVisao*sin(aVisao));
            
            obsPfin[0]=(obsPini[0]+rVisao*cos(aVisao));
            obsPfin[2]=(obsPini[2]-rVisao*sin(aVisao));
            glutPostRedisplay();
            break;
        case 'a':
        case 'A':
            aVisao=aVisao+0.05;
            obsPfin[0]=obsPini[0]+rVisao*cos(aVisao);
            obsPfin[2]=obsPini[2]-rVisao*sin(aVisao);
            glutPostRedisplay();
            break;
        case 'd':
        case 'D':
            aVisao=aVisao-0.05;
            obsPfin[0]=obsPini[0]+rVisao*cos(aVisao);
            obsPfin[2]=obsPini[2]-rVisao*sin(aVisao);
            glutPostRedisplay();
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
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    
    
    
    glutMainLoop();
    
    return 0;
}
