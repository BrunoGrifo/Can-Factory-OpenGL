#include <stdio.h>
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

GLint        wScreen=800, hScreen=600;        //.. janela (pixeis)

//Identificador das Texturas
RgbImage imag;
GLuint textures[1];

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
    
    drawFloor(50,50);
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
    
    
    //Desenha "cena"
    //iluminacao();
    drawScene();
    
    //Atualiza
    glutSwapBuffers();
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
    //glutSpecialFunc(teclasNotAscii);
    //glutKeyboardFunc(keyboardDown);
    //glutKeyboardUpFunc(keyboardUp);
    glutDisplayFunc(display);
    glutTimerFunc(msec, Timer, 1);
    
    glutMainLoop();
    
    return 0;
}
