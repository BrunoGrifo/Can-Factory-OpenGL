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
GLuint textures[25];


GLvoid draw_circle(const GLfloat radius,const GLuint num_vertex){
  GLfloat vertex[4]; 
  GLfloat texcoord[2];
 
  const GLfloat delta_angle = 2.0*M_PI/num_vertex;
 
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,textures[9]);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
  glBegin(GL_TRIANGLE_FAN);
 
  //draw the vertex at the center of the circle
  texcoord[0] = 0.5;
  texcoord[1] = 0.5;
  glTexCoord2fv(texcoord);
 
  vertex[0] = vertex[1] = vertex[2] = 0.0;
  vertex[3] = 1.0;        
  glVertex4fv(vertex);
 
  for(int i = 0; i < num_vertex ; i++)
  {
    texcoord[0] = (cos(delta_angle*i) + 1.0)*0.5;
    texcoord[1] = (sin(delta_angle*i) + 1.0)*0.5;
    glTexCoord2fv(texcoord);
 
    vertex[0] = cos(delta_angle*i) * radius;
    vertex[1] = sin(delta_angle*i) * radius;
    vertex[2] = 0.0;
    vertex[3] = 1.0;
    glVertex4fv(vertex);
  }
 
  texcoord[0] = (1.0 + 1.0)*0.5;
  texcoord[1] = (0.0 + 1.0)*0.5;
  glTexCoord2fv(texcoord);
 
  vertex[0] = 1.0 * radius;
  vertex[1] = 0.0 * radius;
  vertex[2] = 0.0;
  vertex[3] = 1.0;
  glVertex4fv(vertex);
  glEnd();
 
  glDisable(GL_TEXTURE_2D);
 
}
void draw_cylinder(int state_flag){  //Fui buscar à net
    quadratic = gluNewQuadric();
    if(state_flag==1){
        gluCylinder(quadratic,0.2f,0.2f,0.8,15,15);
    }
    if(state_flag==2){
        gluCylinder(quadratic,0.2f,0.2f,0.8,15,15);
        glPushMatrix();
            glTranslatef(0,0,0.8);
            draw_circle(0.2,30);
        glPopMatrix();
    }
    if(state_flag==3){

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,textures[8]);
        glPushMatrix();
            glRotatef(-90,0,0,1);
            gluQuadricDrawStyle ( quadratic, GLU_FILL   );
            gluQuadricNormals   ( quadratic, GLU_SMOOTH );
            gluQuadricTexture   ( quadratic, GL_TRUE    );
            gluCylinder(quadratic,0.2f,0.2f,0.8,10,10);
            glTranslatef(0,0,0.8);
            draw_circle(0.2,30); 
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }
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
void drawWall(float comp, float alt, float larg, int d,int index,int window){
	float i,j;
	int dAlt = roundf(d/(comp/alt));

	for(j=0.0f;j<dAlt;j++){
		for(i=0.0f;i<d;i++){
            if(window==1){
                if((j==1 && i==3) || (j==1 && i==6) || (j==1 && i==7) || (j==1 && i==2)){
                    continue;
                }else{
                    glPushMatrix();
                        glTranslatef((comp/d)*i,(alt/dAlt)*j,0.0f);
                        drawBlock(comp/d,alt/dAlt,larg,index);
                    glPopMatrix();
                }
            }else{
                glPushMatrix();
                    glTranslatef((comp/d)*i,(alt/dAlt)*j,0.0f);
                    drawBlock(comp/d,alt/dAlt,larg,index);
                glPopMatrix();
            }
		}
	}
}

void drawHouse(){
    glPushMatrix();
		drawWall(25,10,0.2,10,14,1);
	glPopMatrix();

    glPushMatrix();
		glRotatef(-90,0,1,0);
		drawWall(25,10,0.2,10,14,0);
	glPopMatrix();

    glPushMatrix();
		glTranslatef(0,0,25);
		drawWall(25,10,0.2,10,14,1);
	glPopMatrix();

    glPushMatrix();
		glTranslatef(25,0,0);
        glRotatef(-90,0,1,0);
		drawWall(25.2,10,0.2,10,14,0);
	glPopMatrix();

    //roof
    glPushMatrix();
        glTranslatef(0,10,25);
        glRotatef(-90,1,0,0);
		drawWall(25,25,0.2,20,7,0);
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
    imag.LoadBmpFile("textures/chao.bmp");
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
    imag.LoadBmpFile("textures/wall3.bmp");
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
    imag.LoadBmpFile("textures/lt.bmp");
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
    imag.LoadBmpFile("textures/rt.bmp");
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
    imag.LoadBmpFile("textures/ft.bmp");
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
    imag.LoadBmpFile("textures/bk.bmp");
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
    imag.LoadBmpFile("textures/up.bmp");
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
    imag.LoadBmpFile("textures/top1.bmp");
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
    imag.LoadBmpFile("textures/coca1.bmp");
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
    imag.LoadBmpFile("textures/topcan3.bmp");
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
    imag.LoadBmpFile("textures/metal3.bmp");
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
    imag.LoadBmpFile("textures/tijolo.bmp");
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
    imag.LoadBmpFile("textures/canfloor.bmp");
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
    imag.LoadBmpFile("textures/capa.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());

    glGenTextures(1, &textures[14]);
    glBindTexture(GL_TEXTURE_2D, textures[14]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    imag.LoadBmpFile("textures/par3.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());

    glGenTextures(1, &textures[15]);
    glBindTexture(GL_TEXTURE_2D, textures[15]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    imag.LoadBmpFile("textures/chaosujo.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());
    
    glGenTextures(1, &textures[16]);
    glBindTexture(GL_TEXTURE_2D, textures[16]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    imag.LoadBmpFile("textures/detail1.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());
    
    glGenTextures(1, &textures[17]);
    glBindTexture(GL_TEXTURE_2D, textures[17]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    imag.LoadBmpFile("textures/prateleira.bmp");
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
    for(int i=0;i<16;i++){
        glPushMatrix();
            glTranslatef(5.1,1.3,canWalk[0]+i-0.5);
            glRotatef(-90, 1,0,0);
            draw_cylinder(1);
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
            draw_cylinder(2);
        glPopMatrix();
    }
    canWalk[1]+=0.02;
    if(canWalk[1]>6)
        canWalk[1]=5;
    
    for(int i=0;i<16;i++){
        glPushMatrix();
            glTranslatef(20.1,1.3,canWalk[2]-i+0.5);
            glRotatef(-90, 1,0,0);
            draw_cylinder(3);
        glPopMatrix();
    }
    canWalk[2]-=0.02;
    if(canWalk[2]<19)
        canWalk[2]=20;
    
    glutPostRedisplay();
}

void drawDetail(int index){
    glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,textures[index]);
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
void drawPathSupport(double T1,double T2,double T3,double dB1,double dB2,double dB3,double dBFlag ){
    glPushMatrix();
        glTranslatef(T1,T2,T3);
        drawBlock(dB1,dB2,dB3,dBFlag);
    glPopMatrix();
}
void drawTRBlock(double T1,double T2,double T3,int R1,int R2,int R3,int R4,double dB1,double dB2,double dB3,double dBFlag){
    glPushMatrix();
        glTranslatef(T1,T2,T3);
        glRotatef(R1,R2,R3,R4);
        drawBlock(dB1,dB2,dB3,dBFlag);
    glPopMatrix();
}
void drawPrateleiras(){
    drawPathSupport(0.01,5,3.25,0.6,0.2,5,17);
        drawTRBlock(0,4.5,3.3,-45,0,0,1,0.06,0.8,0.06,10);
        drawTRBlock(0,4.5,8.1,-45,0,0,1,0.06,0.8,0.06,10);

    drawPathSupport(0.01,2.2,9.25,0.6,0.2,5,17);
        drawTRBlock(0,1.7,9.3,-45,0,0,1,0.06,0.8,0.06,10);
        drawTRBlock(0,1.7,14.1,-45,0,0,1,0.06,0.8,0.06,10);

    drawPathSupport(0.01,5,15.25,0.6,0.2,5,17);
        drawTRBlock(0,4.5,15.3,-45,0,0,1,0.06,0.8,0.06,10);
        drawTRBlock(0,4.5,20.1,-45,0,0,1,0.06,0.8,0.06,10);

    //parede esquerda
    drawPathSupport(24.2,5,3.25,0.6,0.2,5,17);
        drawTRBlock(24.8,4.5,3.3,45,0,0,1,0.06,0.8,0.06,10);
        drawTRBlock(24.8,4.5,8.1,45,0,0,1,0.06,0.8,0.06,10);

    drawPathSupport(24.2,2.2,9.25,0.6,0.2,5,17);
        drawTRBlock(24.8,1.7,9.3,45,0,0,1,0.06,0.8,0.06,10);
        drawTRBlock(24.8,1.7,14.1,45,0,0,1,0.06,0.8,0.06,10);

    drawPathSupport(24.2,5,15.25,0.6,0.2,5,17);    
        drawTRBlock(24.8,4.5,15.3,45,0,0,1,0.06,0.8,0.06,10);
        drawTRBlock(24.8,4.5,20.1,45,0,0,1,0.06,0.8,0.06,10);

}
void drawLamps(){
    //lamp1

}
void drawDetails(){
    //Candeiros
    drawLamps();

    //chao
    glPushMatrix();
        glTranslatef(7.995,0.001,19.62);
        glScalef(1.99,1,1.99);
        glRotatef(90,1,0,0);
        glRotatef(180,0,0,1);
        drawDetail(15);
    glPopMatrix();
    
    //Parede
    glPushMatrix();
        glTranslatef(0.01,2.19,3.25);
        glScalef(1,2.5,2.5);
        glRotatef(-90,0,1,0);
        drawDetail(16);
    glPopMatrix();

    //prateleiras
    //parede direita
    drawPrateleiras();



}

void drawScene(void){
    //Desenha chão
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    
    
    drawFloor(25,25);
    drawDetails();
    
    
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
        /*if(obsPini[0]>18.5){
            obsPini[0]=(obsPini[0]+incVisao*cos(aVisao));
        }
        if(obsPini[2]>18.5){
            obsPini[2]=(obsPini[2]-incVisao*sin(aVisao));
        }

        if(obsPini[0]<5){
            obsPini[0]=(obsPini[0]+incVisao*cos(aVisao));
        }
        if(obsPini[2]<2){
            obsPini[2]=(obsPini[2]-incVisao*sin(aVisao));
        }*/
       
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
void drawPathParts(GLfloat* cor,double T1,double T2,double T3,double dB1,double dB2,double dB3,double dBFlag){
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT, cor);
        glTranslatef(T1,T2,T3);
        drawBlock(dB1,dB2,dB3,dBFlag);
    glPopMatrix();

}

void drawConveyor(){
    drawPathParts(azul,4.1,0,0.2,2,3,4.5,-1);
    drawPathParts(azul,19.1,0,0.2,2,3,4.5,-1);

    //Secções de transformação
    //Primeira secção
    drawPathParts(preto,4.5,1.25,19.5,1.2,2,1.2,-1);
    //capa de vermelho
    drawPathParts(vermelho,4.3,2.3,19.3,1.6,2,1.6,13);
    drawPathParts(vermelho,4.3,1.25,19.3,0.4,1.1,0.2,13);
    drawPathParts(vermelho,5.5,1.25,19.3,0.4,1.1,0.4,13);
    drawPathParts(vermelho,5.7,1.25,20.5,0.2,1.1,0.4,13);

    //Segunda secção
    drawPathParts(preto,19.5,1.25,19.5,1.2,2,1.2,-1);
    //capa de vermelho
    drawPathParts(vermelho,19.3,2.3,19.3,1.6,2,1.6,13);
    drawPathParts(vermelho,20.5,1.25,19.3,0.4,1.1,0.2,13);
    drawPathParts(vermelho,19.3,1.25,19.3,0.4,1.1,0.4,13);
    drawPathParts(vermelho,19.3,1.25,20.5,0.2,1.1,0.4,13);

    // -----------------------PRIMEIRA LATA --------------------- PATH
    //lado esquerdo
    drawPathSupport(5.5,0,5,0.2,1,0.2,10);
    drawPathSupport(5.5,0,10,0.2,1,0.2,10);
    drawPathSupport(5.5,0,15,0.2,1,0.2,10);
    drawPathSupport(5.5,0,19.4,0.2,1,0.2,10);
     //lado direito
    drawPathSupport(4.5,0,5,0.2,1,0.2,10);
    drawPathSupport(4.5,0,10,0.2,1,0.2,10);
    drawPathSupport(4.5,0,15,0.2,1,0.2,10);
    drawPathSupport(4.5,0,20.5,0.2,1,0.2,10);
    //topo
    drawPathSupport(4.5,1,4.5,1.2,0.2,16.2,10);

    // -----------------------SEGUNDA LATA --------------------- PATH
    //lado esquerdo
    drawPathSupport(10,0,20.5,0.2,1,0.2,10);
    drawPathSupport(15,0,20.5,0.2,1,0.2,10);

    //lado direito
    drawPathSupport(10,0,19.5,0.2,1,0.2,10);
    drawPathSupport(15,0,19.5,0.2,1,0.2,10); 
    //topo
    glPushMatrix();
        glTranslatef(5,1,20.7);
        glRotatef(90,0,1,0);
        drawBlock(1.2,0.2,15.6,10);
    glPopMatrix();

    // -----------------------TERCEIRA LATA --------------------- PATH
    //lado esquerdo
    drawPathSupport(19.5,0,5,0.2,1,0.2,10);
    drawPathSupport(19.5,0,10,.2,1,0.2,10);
    drawPathSupport(19.5,0,15,0.2,1,0.2,10);
    drawPathSupport(19.5,0,19.4,0.2,1,0.2,10);
    //lado direito
    drawPathSupport(20.5,0,5,0.2,1,0.2,10);
    drawPathSupport(20.5,0,10,0.2,1,0.2,10);
    drawPathSupport(20.5,0,15,.2,1,0.2,10);
    drawPathSupport(20.5,0,20.5,0.2,1,0.2,10);
    //topo
    drawPathSupport(19.5,1,4.5,.2,0.2,16.2,10);
    
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
