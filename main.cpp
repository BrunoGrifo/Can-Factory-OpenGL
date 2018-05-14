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


GLfloat  cinzento[] = { 0.5 ,0.5 ,0.5 }, preto[] = { 0 ,0 ,0 }, azul [] = {0, 0, 1},vermelho [] = {1, 0, 0} ;
GLfloat luzAmbiente[4] = { 1,1,1,1 };
//
int w=0,s=0,d=0,a=0;

int pressed[6] = {0,0,0,0,0,0}; //moves

GLint        wScreen=1000, hScreen=900;        //.. janela (pixeis)

//Coordenadas da lata
float canWalk[]={4,5,20};
float floorWalk[]={3.7,3.7,20};

GLUquadricObj *quadratic;


//Parametros de visao do observador
GLfloat  rVisao=3.0, aVisao=0.5*PI, incVisao=0.1, aVisaoY=0;

//Posição inicial do observador
GLfloat  obsPini[] ={10, 3.5, 2};
GLfloat  obsPfin[] ={obsPini[0]+rVisao*cos(aVisao), obsPini[1]+rVisao*sin(aVisaoY), obsPini[2]+rVisao*sin(aVisao)};

//Identificador das Texturas
RgbImage imag;
GLuint textures[20];

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
   
}


//Verificar texturas
void drawBlock(float comp, float alt, float larg, int textID){
	glPushMatrix();
		//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        if(textID!=-1){
            glEnable(GL_TEXTURE_2D);	              // Select Our Texture
            glBindTexture(GL_TEXTURE_2D, textures[textID]);
        }
		glBegin(GL_QUADS);
		    // Front Face
		    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.0f,  0.0f,  larg);  // Bottom Left Of The Texture and Quad
		    glTexCoord2f(1.0f, 0.0f); glVertex3f( comp,  0.0f,  larg);  // Bottom Right Of The Texture and Quad
		    glTexCoord2f(1.0f, 1.0f); glVertex3f( comp,  alt,   larg);  // Top Right Of The Texture and Quad
		    glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f,  alt,   larg);  // Top Left Of The Texture and Quad
		    // Back Face
		    glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.0f,  0.0f,  0.0f);  // Bottom Right Of The Texture and Quad
		    glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.0f,   alt,  0.0f);  // Top Right Of The Texture and Quad
		    glTexCoord2f(0.0f, 1.0f); glVertex3f( comp,   alt,  0.0f);  // Top Left Of The Texture and Quad
		    glTexCoord2f(0.0f, 0.0f); glVertex3f( comp,  0.0f,  0.0f);  // Bottom Left Of The Texture and Quad
		    // Top Face
		    glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f,   alt,  0.0f);  // Top Left Of The Texture and Quad
		    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.0f,   alt,  larg);  // Bottom Left Of The Texture and Quad
		    glTexCoord2f(1.0f, 0.0f); glVertex3f( comp,   alt,  larg);  // Bottom Right Of The Texture and Quad
		    glTexCoord2f(1.0f, 1.0f); glVertex3f( comp,   alt,  0.0f);  // Top Right Of The Texture and Quad
		    // Bottom Face
		    glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.0f,  0.0f,  0.0f);  // Top Right Of The Texture and Quad
		    glTexCoord2f(0.0f, 1.0f); glVertex3f( comp,  0.0f,  0.0f);  // Top Left Of The Texture and Quad
		    glTexCoord2f(0.0f, 0.0f); glVertex3f( comp,  0.0f,  larg);  // Bottom Left Of The Texture and Quad
		    glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.0f,  0.0f,  larg);  // Bottom Right Of The Texture and Quad
		    // Right face
		    glTexCoord2f(1.0f, 0.0f); glVertex3f( comp,  0.0f,  0.0f);  // Bottom Right Of The Texture and Quad
		    glTexCoord2f(1.0f, 1.0f); glVertex3f( comp,   alt,  0.0f);  // Top Right Of The Texture and Quad
		    glTexCoord2f(0.0f, 1.0f); glVertex3f( comp,   alt,  larg);  // Top Left Of The Texture and Quad
		    glTexCoord2f(0.0f, 0.0f); glVertex3f( comp,  0.0f,  larg);  // Bottom Left Of The Texture and Quad
		    // Left Face
		    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.0f,  0.0f,  larg);  // Bottom Left Of The Texture and Quad
		    glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.0f,   alt,  larg);  // Bottom Right Of The Texture and Quad
		    glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.0f,   alt,  0.0f);  // Top Right Of The Texture and Quad
		    glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f,  0.0f,  0.0f);  // Top Left Of The Texture and Quad
		glEnd();
        if(textID!=-1)
		    glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void drawWall(float comp, float alt, float larg, int d,int index){
	float i,j;
	int dAlt = roundf(d/(comp/alt));

	for(j=0.0f;j<dAlt;j++){
		for(i=0.0f;i<d;i++){
			glPushMatrix();
				glTranslatef((comp/d)*i,(alt/dAlt)*j,0.0f);
				drawBlock(comp/d,alt/dAlt,larg,index);
			glPopMatrix();
		}
	}
}
void drawHouse(){
    glPushMatrix();
		drawWall(25,1.5,0.2,10,11);
	glPopMatrix();
    glPushMatrix();
        glTranslatef(0,0,0.2);
		drawWall(0.8,10,0.8,1,11);
	glPopMatrix();


    glPushMatrix();
		glRotatef(-90,0,1,0);
		drawWall(25,1.5,0.2,10,11);
	glPopMatrix();
    glPushMatrix();
		glTranslatef(24,0,0.2);
		drawWall(0.8,10,0.8,1,11);
	glPopMatrix();

    glPushMatrix();
		glTranslatef(0,0,25);
		drawWall(25,1.5,0.2,10,11);
	glPopMatrix();
    glPushMatrix();
		glTranslatef(0,0,24.2);
		drawWall(0.8,10,0.8,1,11);
	glPopMatrix();

    glPushMatrix();
		glTranslatef(25,0,0);
        glRotatef(-90,0,1,0);
		drawWall(25.2,1.5,0.2,10,11);
	glPopMatrix();
    glPushMatrix();
		glTranslatef(24,0,24.2);
		drawWall(0.8,10,0.8,1,11);
	glPopMatrix();


    //roof
    glPushMatrix();
        glTranslatef(0,10,25);
        glRotatef(-90,1,0,0);
		drawWall(25,25,0.2,20,7);
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
    imag.LoadBmpFile("wall3.bmp");
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
    imag.LoadBmpFile("lt.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());
    
    glGenTextures(1, &textures[3]);
    glBindTexture(GL_TEXTURE_2D, textures[3]);
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
    
    glGenTextures(1, &textures[4]);
    glBindTexture(GL_TEXTURE_2D, textures[4]);
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
    glBindTexture(GL_TEXTURE_2D, textures[5]);
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
    
    glGenTextures(1, &textures[6]);
    glBindTexture(GL_TEXTURE_2D, textures[6]);
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
    
    glGenTextures(1, &textures[7]);
    glBindTexture(GL_TEXTURE_2D, textures[7]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    imag.LoadBmpFile("top1.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());

    glGenTextures(1, &textures[8]);
    glBindTexture(GL_TEXTURE_2D, textures[8]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    imag.LoadBmpFile("coca.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());

    glGenTextures(1, &textures[9]);
    glBindTexture(GL_TEXTURE_2D, textures[9]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    imag.LoadBmpFile("topcan.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());
    
    glGenTextures(1, &textures[10]);
    glBindTexture(GL_TEXTURE_2D, textures[10]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    imag.LoadBmpFile("metal3.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());

    glGenTextures(1, &textures[11]);
    glBindTexture(GL_TEXTURE_2D, textures[11]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    imag.LoadBmpFile("tijolo.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());

    glGenTextures(1, &textures[12]);
    glBindTexture(GL_TEXTURE_2D, textures[12]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    imag.LoadBmpFile("canfloor.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());

    glGenTextures(1, &textures[13]);
    glBindTexture(GL_TEXTURE_2D, textures[13]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    imag.LoadBmpFile("capa.bmp");
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
    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, textures[8]);
    for(int i=0;i<15;i++){
        glPushMatrix();
            glTranslatef(5.1,1.3,canWalk[0]+i);
            glRotatef(-90, 1,0,0);
            draw_cylinder(0.2,0.8, 255, 160, 100);
        glPopMatrix();
    }
    canWalk[0]+=0.02;
    if(canWalk[0]>6)
        canWalk[0]=5;

    //glDisable(GL_TEXTURE_2D);
    for(int i=0;i<15;i++){
        glPushMatrix();
            glTranslatef(canWalk[1]+i,1.3,20.1);
            glRotatef(-90, 1,0,0);
            draw_cylinder(0.2,0.8, 255, 160, 100);
        glPopMatrix();
    }
    canWalk[1]+=0.02;
    if(canWalk[1]>6)
        canWalk[1]=5;
    
    for(int i=0;i<15;i++){
        glPushMatrix();
            glTranslatef(20.1,1.3,canWalk[2]-i);
            glRotatef(-90, 1,0,0);
            draw_cylinder(0.2,0.8, 255, 160, 100);
        glPopMatrix();
    }
    canWalk[2]-=0.02;
    if(canWalk[2]<19)
        canWalk[2]=20;
    
    glutPostRedisplay();
}
void drawCanFloor(){
    glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,textures[12]);
        glPushMatrix();
            glBegin(GL_QUADS);
                glTexCoord2f(0.0f,0.0f); glVertex3i(  1,  0, 0);
                glTexCoord2f(1.0f,0.0f); glVertex3i(     0,  0, 0);
                glTexCoord2f(1.0f,1.0f); glVertex3i(     0, 1, 0);
                glTexCoord2f(0.0f,1.0f); glVertex3i(  1, 1, 0);
            glEnd();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
}
void drawCanFloorEngine(){
    for(int i=0;i<16;i++){
        glPushMatrix();
            glTranslatef(4.6,1.25,floorWalk[0]+i);
            glRotatef(90,1,0,0);
            drawCanFloor();
        glPopMatrix();
    }

    for(int i=0;i<15;i++){
        glPushMatrix();
            glTranslatef(floorWalk[0]+i+1,1.25,19.6);
            glRotatef(90,1,0,0);
            drawCanFloor();
        glPopMatrix();
    }

    for(int i=0;i<16;i++){
        glPushMatrix();
            glTranslatef(19.6,1.25,23.4-(floorWalk[0]+i));
            glRotatef(90,1,0,0);
            drawCanFloor();
        glPopMatrix();
    }
    floorWalk[0]+=0.02;

    if(floorWalk[0]>=4.7){
        floorWalk[0]=3.7;
    }

    glutPostRedisplay();
}

void drawScene(void){
    //Desenha chão
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    
    drawFloor(25,25);
    
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT, cinzento);
        drawCanEngine();
        drawCanFloorEngine();
    glPopMatrix();
    
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

    glBindTexture(GL_TEXTURE_2D,textures[5]);
    glPushMatrix();
    
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,0.0f); glVertex3i(obsPini[0]-size,obsPini[1]-size,obsPini[2]-size);
    glTexCoord2f(1.0f,0.0f); glVertex3i(obsPini[0]+size,obsPini[1]-size,obsPini[2]-size);
    glTexCoord2f(1.0f,1.0f); glVertex3i(obsPini[0]+size,obsPini[1]+size,obsPini[2]-size);
    glTexCoord2f(0.0f,1.0f); glVertex3i(obsPini[0]-size,obsPini[1]+size,obsPini[2]-size);
    glEnd();
    
    glPopMatrix();
    

    glBindTexture(GL_TEXTURE_2D,textures[3]);
    glPushMatrix();
    
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,0.0f); glVertex3i(obsPini[0]+size,obsPini[1]-size,obsPini[2]-size);
    glTexCoord2f(1.0f,0.0f); glVertex3i(obsPini[0]+size,obsPini[1]-size,obsPini[2]+size);
    glTexCoord2f(1.0f,1.0f); glVertex3i(obsPini[0]+size,obsPini[1]+size,obsPini[2]+size);
    glTexCoord2f(0.0f,1.0f); glVertex3i(obsPini[0]+size,obsPini[1]+size,obsPini[2]-size);
    glEnd();
    
    glPopMatrix();
    

    glBindTexture(GL_TEXTURE_2D,textures[4]);
    glPushMatrix();
    
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,0.0f); glVertex3i(obsPini[0]+size,obsPini[1]-size,obsPini[2]+size);
    glTexCoord2f(1.0f,0.0f); glVertex3i(obsPini[0]-size,obsPini[1]-size,obsPini[2]+size);
    glTexCoord2f(1.0f,1.0f); glVertex3i(obsPini[0]-size,obsPini[1]+size,obsPini[2]+size);
    glTexCoord2f(0.0f,1.0f); glVertex3i(obsPini[0]+size,obsPini[1]+size,obsPini[2]+size);
    glEnd();
    
    glPopMatrix();
    

    glBindTexture(GL_TEXTURE_2D,textures[2]);
    glPushMatrix();
    
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,0.0f); glVertex3i(obsPini[0]-size,obsPini[1]-size,obsPini[2]+size);
    glTexCoord2f(1.0f,0.0f); glVertex3i(obsPini[0]-size,obsPini[1]-size,obsPini[2]-size);
    glTexCoord2f(1.0f,1.0f); glVertex3i(obsPini[0]-size,obsPini[1]+size,obsPini[2]-size);
    glTexCoord2f(0.0f,1.0f); glVertex3i(obsPini[0]-size,obsPini[1]+size,obsPini[2]+size);
    glEnd();
    
    glPopMatrix();
    
    glBindTexture(GL_TEXTURE_2D,textures[6]);
    glPushMatrix();
    
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,0.0f); glVertex3i(obsPini[0]+size,obsPini[1]+size,obsPini[2]-size);
    glTexCoord2f(1.0f,0.0f); glVertex3i(obsPini[0]+size,obsPini[1]+size,obsPini[2]+size);
    glTexCoord2f(1.0f,1.0f); glVertex3i(obsPini[0]-size,obsPini[1]+size,obsPini[2]+size);
    glTexCoord2f(0.0f,1.0f); glVertex3i(obsPini[0]-size,obsPini[1]+size,obsPini[2]-size);
    glEnd();
    
    glPopMatrix();

    //Bottom
    glBindTexture(GL_TEXTURE_2D,textures[1]);
    glPushMatrix();
    
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,0.0f); glVertex3i(obsPini[0]-size,obsPini[1]-size,obsPini[2]-size);
    glTexCoord2f(1.0f,0.0f); glVertex3i(obsPini[0]-size,obsPini[1]-size,obsPini[2]-size);
    glTexCoord2f(1.0f,1.0f); glVertex3i(obsPini[0]+size,obsPini[1]+size,obsPini[2]-size);
    glTexCoord2f(0.0f,1.0f); glVertex3i(obsPini[0]+size,obsPini[1]+size,obsPini[2]-size);
    glEnd();
    
    glPopMatrix();
    
}
void drawConveyor(){
    
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT, azul);
        glTranslatef(4.1,0,0.2);
        drawBlock(2,3,4.5,-1);
    glPopMatrix();

    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT, azul);
        glTranslatef(19.1,0,0.2);
        drawBlock(2,3,4.5,-1);
    glPopMatrix();


    //Secções de transformação
    //Primeira secção
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT, preto);
        glTranslatef(4.5,1.25,19.5);
        drawBlock(1.2,2,1.2,-1);
    glPopMatrix();
    //capa de vermelho
    
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT, vermelho);
        glTranslatef(4.3,2.3,19.3);
        drawBlock(1.6,2,1.6,13);
    glPopMatrix();
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT, vermelho);
        glTranslatef(4.3,1.25,19.3);
        drawBlock(0.4,1.1,0.2,13);
    glPopMatrix();
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT, vermelho);
        glTranslatef(5.5,1.25,19.3);
        drawBlock(0.4,1.1,0.4,13);
    glPopMatrix();

    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT, vermelho);
        glTranslatef(5.7,1.25,20.5);
        drawBlock(0.2,1.1,0.4,13);
    glPopMatrix();



    //Asegunda secção
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT, preto);
        glTranslatef(19.5,1.25,19.5);
        drawBlock(1.2,2,1.2,-1);
    glPopMatrix();

    //capa de vermelho
    
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT, vermelho);
        glTranslatef(19.3,2.3,19.3);
        drawBlock(1.6,2,1.6,13);
    glPopMatrix();
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT, vermelho);
        glTranslatef(20.5,1.25,19.3);
        drawBlock(0.4,1.1,0.2,13);
    glPopMatrix();
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT, vermelho);
        glTranslatef(19.3,1.25,19.3);
        drawBlock(0.4,1.1,0.4,13);
    glPopMatrix();

    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT, vermelho);
        glTranslatef(19.3,1.25,20.5);
        drawBlock(0.2,1.1,0.4,13);
    glPopMatrix();

    // -----------------------PRIMEIRA LATA --------------------- PATH
    //lado esquerdo
    glPushMatrix();
        glTranslatef(5.5,0,5);
        drawBlock(0.2,1,0.2,10);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(5.5,0,10);
        drawBlock(0.2,1,0.2,10);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(5.5,0,15);
        drawBlock(0.2,1,0.2,10);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(5.5,0,19.4);
        drawBlock(0.2,1,0.2,10);
    glPopMatrix();
    //lado direito
    glPushMatrix();
        glTranslatef(4.5,0,5);
        drawBlock(0.2,1,0.2,10);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(4.5,0,10);
        drawBlock(0.2,1,0.2,10);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(4.5,0,15);
        drawBlock(0.2,1,0.2,10);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(4.5,0,20.5);
        drawBlock(0.2,1,0.2,10);
    glPopMatrix();
    //topo
    glPushMatrix();
        glTranslatef(4.5,1,4.5);
        drawBlock(1.2,0.2,16.2,10);
    glPopMatrix();



    // -----------------------SEGUNDA LATA --------------------- PATH
    //lado esquerdo
    glPushMatrix();
        glTranslatef(10,0,20.5);
        drawBlock(0.2,1,0.2,10);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(15,0,20.5);
        drawBlock(0.2,1,0.2,10);
    glPopMatrix();
    //lado direito

    glPushMatrix();
        glTranslatef(10,0,19.5);
        drawBlock(0.2,1,0.2,10);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(15,0,19.5);
        drawBlock(0.2,1,0.2,10);
    glPopMatrix();
    
    //topo
    glPushMatrix();
        glTranslatef(5,1,20.7);
        glRotatef(90,0,1,0);
        drawBlock(1.2,0.2,15.6,10);
    glPopMatrix();

    // -----------------------TERCEIRA LATA --------------------- PATH
    //lado esquerdo
    glPushMatrix();
        glTranslatef(19.5,0,5);
        drawBlock(0.2,1,0.2,10);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(19.5,0,10);
        drawBlock(0.2,1,0.2,10);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(19.5,0,15);
        drawBlock(0.2,1,0.2,10);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(19.5,0,19.4);
        drawBlock(0.2,1,0.2,10);
    glPopMatrix();
    //lado direito
    glPushMatrix();
        glTranslatef(20.5,0,5);
        drawBlock(0.2,1,0.2,10);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(20.5,0,10);
        drawBlock(0.2,1,0.2,10);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(20.5,0,15);
        drawBlock(0.2,1,0.2,10);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(20.5,0,20.5);
        drawBlock(0.2,1,0.2,10);
    glPopMatrix();
    //topo
    glPushMatrix();
        glTranslatef(19.5,1,4.5);
        drawBlock(1.2,0.2,16.2,10);
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
    gluPerspective(99.0, wScreen/hScreen, 0.3, 100.0);
    
    
   
    
    
    // Camara + Observador
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    updateKeys();

    //Posição do observador
    gluLookAt(obsPini[0], obsPini[1], obsPini[2], obsPfin[0], obsPfin[1], obsPfin[2], 0, 1, 0);
    
    
    //Desenha "cena"
    drawScene();
    drawConveyor();
    
    drawHouse();
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
