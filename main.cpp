#include <stdio.h>
#include "RgbImage.h"
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdio.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <string.h>

#define frand()			((float)rand()/RAND_MAX)
#define MAX_PARTICULAS  25
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

#define GRAY1    0.2, 0.2, 0.2, 1.0
#define GRAY2    0.93, 0.93, 0.93, 1.0

#define frand()			((float)rand()/RAND_MAX)
#define MAX_PARTICULAS  25





//============================================================== Iluminacao


//---------------------------------------- Particle attributes
typedef struct {
	float   size;		// tamanho
	float	life;		// vida
	float	fade;		// fade
	float	r, g, b;    // color
	GLfloat x, y, z;    // posicao
	GLfloat vx, vy, vz; // velocidade 
    GLfloat ax, ay, az; // aceleracao
} Particle;

Particle  particula1[MAX_PARTICULAS];
GLint    milisec = 1000; 
int faisca = 0,alavanca =1 ,alavancaLigada=0;
float B_angulo=-45;

// -----------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------
// =============================================================================== Iluminacao

//����������������������������������������������������� Luz branca/cinzenta
GLfloat intLuzA=0.8;
GLfloat intLuzD=0.9;
GLfloat lightpos[]		= {    0.0,    15.0,     0.0,   1};
GLfloat lightambient[]  = { intLuzA, intLuzA, intLuzA    };
GLfloat lightdiffuse[]  = { intLuzD, intLuzD, intLuzD    };
GLfloat lightspecular[] = {	   1.0,     1.0,   1.0	     };
GLfloat dir[]={0,-1,0};
GLint   noite = 1;



//============================================================== Objectos
//����������������������������������������������������� Objectos Opacos

GLfloat  matBranco[]={ 1,1,1, 1};				// eixos

//............................................................ bule azul fixo
GLfloat  turquoiA []={ 0.1 ,0.18725 ,0.1745 };
GLfloat  turquoiD []={ 0.396 ,0.74151 ,0.69102, 1.0 };
GLfloat  turquoiS []={ 0.297254 ,0.30829 ,0.306678 };
GLint	 turquoiC = 0.1 *128;

//����������������������������������������������������� Objectos transparentes
GLfloat  alfa =0.8;

//............................................................ toroide as voltas
GLfloat  silverA []={ 0.19225, 0.19225, 0.19225      };  
GLfloat  silverD []={ 0.50754, 0.50754, 0.50754, alfa};    // na componente difusa
GLfloat  silverS []={ 0.50827, 0.50827, 0.50827      };
GLint    silverC = 0.4 *128;

//............................................................ bule dourado no centro
// +++++++++ A IMPLEMENTAR  +++++++++
GLfloat  goldA []={ 0.24725, 0.1995, 0.0745      };  
GLfloat  goldD []={ 0.75164, 0.60648, 0.22648, alfa};    // na componente difusa
GLfloat  goldS []={ 0.628281, 0.555802, 0.366065      };
GLint    goldC = 0.4 *128;


//����������������������������������������������������� Posicao toroide
GLfloat  rObj1  = 7.9;
GLfloat  angR   = -90;
GLfloat  incR   =   5;   // graus
GLfloat	 pObj1[] = { rObj1*cos(angR*PI/180.0), 0.5, rObj1*sin(angR*PI/180.0) };


//==============================================================  observador
GLfloat  raioOB = 12;
GLfloat  angOB  = 0.70;
GLfloat	 pO[]= { raioOB*cos(angOB), 1, raioOB*sin(angOB) };

//============================================================== Variaveis
GLint    parar =0;		// parar/iniciar movimento objecto1
GLint    text  =0;		// Com/sem textura 
GLint    decal =0;		// Tipo de mapeamento na textura


//======================================================================================
//======================================================================================
//======================================================================================


						//-------------------- TExturas
GLint   ligaLuz = 0;
GLint   ligaLuz1 = 0;
GLint   ligaLuz2 = 0;
GLint   tet = 0;
GLint trabalha = 1;


GLfloat contraste1[] = {0.1,0.1,0.1};
GLfloat contraste2[] = { 0.1,0.1,0.1 };

GLfloat luzGlobalCor[4] = { 1.0,1.0,1.0,1.0 };



//---------------------------------------------- Texturas
GLuint  texture[1];
char	 filename[1][12] = {"brilho0.bmp" };






GLfloat  cinzento[] = { 0.5 ,0.5 ,0.5 },castanho[] = { 0.5 ,0.3 ,0.2 }, preto[] = { 0.2 ,0.2 ,0.2 }, azul [] = {0, 0, 1},vermelho [] = {1, 0, 0} ,amarelo [] = {1, 1, 0.6} ,copo [] = {0.9,0.95,1},branco[] = {1,1,1}, verde[] = { 0,1,0 };
GLfloat luzAmbiente[4] = { 1,1,1,1 };
//
int w=0,s=0,d=0,a=0;

int pressed[12] = {0,0,0,0,0,0,0,0,0,0,0,0}; //moves

GLint        wScreen=1000, hScreen=900;        //.. janela (pixeis)

//Coordenadas da lata
float canWalk[]={4,5,20};
float floorWalk[]={3.7,3.7,20};

GLUquadricObj *quadratic,*lamp,*button;


//Parametros de visao do observador
GLfloat  rVisao=3.0, aVisao=0.5*PI, incVisao=0.1, aVisaoY=0;

//Posição inicial do observador
GLfloat  obsPini[] ={10, 3.5, 2};
GLfloat  obsPfin[] ={obsPini[0]+rVisao*cos(aVisao), obsPini[1]+rVisao*sin(aVisaoY), obsPini[2]+rVisao*sin(aVisao)};

//Identificador das Texturas
RgbImage imag;
GLuint textures[50];


void iniParticulas(Particle *particula)
{
 GLfloat v, theta, phi;
 int i;
 GLfloat px, py, pz;
 GLfloat ps;

	px = 7.7;
	py = 8.4;
	pz = 7.8;
	ps = 0.015;



 for(i=0; i<MAX_PARTICULAS; i++)  {

	//---------------------------------  
	v     = (1*frand()+0.02);
    theta = (2.0*frand()*M_PI);   // [0..2pi]
	phi   = (frand()*M_PI);		// [0.. pi]
    
    particula[i].size = ps ;		// tamanh de cada particula
    particula[i].x	  = px + 0.1*frand()*px;    // [-200 200]
    particula[i].y	  = py + 0.1*frand()*py;	// [-200 200]
    particula[i].z	  = pz + 0.1*frand()*pz;	// [-200 200]
        
	particula[i].vx = v * cos(theta) * sin(phi);	// esferico
    particula[i].vy = v * cos(phi);
    particula[i].vz = v * sin(theta) * sin(phi);
	particula[i].ax = 0.01f;
    particula[i].ay = -0.01f;
    particula[i].az = 0.015f;

	particula[i].r = 1.0f;
	particula[i].g = 1.0f;	
	particula[i].b = 1.0f;	
	particula[i].life = 0.2f;		                
	particula[i].fade = 0.001f;	// Em 100=1/0.01 iteracoes desaparece
	}
}
void draw_objecto0()  // bule transparente FIXO (origem)
{
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, goldA);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, goldD);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, goldS);//falta um push e pop por as texturas dentro dos pops e pushs
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, goldC);
    glBindTexture(GL_TEXTURE_2D, textures[40]);
    glTranslatef(10.5, 2.26, 22.5);
    glutSolidTeapot(0.3);
    glPopMatrix();
    //if (text == 1) glEnable(GL_TEXTURE_2D);

}

void showParticulas(Particle *particula, int sistema) {
 int i;
 int numero;

 numero=(int) (frand()*10.0);
 
 for (i=0; i<MAX_PARTICULAS; i++){
    glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,textures[1]);
        glPushMatrix();
            glColor4f(1,1,1, particula[i].life);
            glBegin(GL_QUADS);				        
                glTexCoord2d(0,0); glVertex3f(particula[i].x -particula[i].size, particula[i].y -particula[i].size, particula[i].z);      
                glTexCoord2d(1,0); glVertex3f(particula[i].x +particula[i].size, particula[i].y -particula[i].size, particula[i].z);        
                glTexCoord2d(1,1); glVertex3f(particula[i].x +particula[i].size, particula[i].y +particula[i].size, particula[i].z);            
                glTexCoord2d(0,1); glVertex3f(particula[i].x -particula[i].size, particula[i].y +particula[i].size, particula[i].z);       
            glEnd();	
            particula[i].x += particula[i].vx;
            particula[i].y += particula[i].vy;
            particula[i].z += particula[i].vz;
            particula[i].vx += particula[i].ax;
            particula[i].vy += particula[i].ay;
            particula[i].vz += particula[i].az;
            particula[i].life -= particula[i].fade;	
            }
        glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}


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
void draw_cylinder(int state_flag,int textura_index){  //Fui buscar à net
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
        glBindTexture(GL_TEXTURE_2D,textures[textura_index]);
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
void drawWindow(float comp, float alt, float larg, int textID){
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
		    
		glEnd();
        if(textID!=-1)
		    glDisable(GL_TEXTURE_2D);
	glPopMatrix();
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
    if(window==1){
        for(j=0.0f;j<dAlt;j++){
            for(i=0.0f;i<d;i++){
                    if((j==1 && i==3) || (j==1 && i==6) || (j==1 && i==7) || (j==1 && i==2)){
                        glDisable(GL_TEXTURE_2D);

                        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   silverA);
                        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   silverD);
                        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  silverS);
                        glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, silverC);
                        glPushMatrix();
                            glMaterialfv(GL_FRONT, GL_AMBIENT, copo);
                            glEnable(GL_BLEND);
                            glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
                            glTranslatef((comp/d)*i,(alt/dAlt)*j,0.0f);
                            drawWindow(comp/d,alt/dAlt,larg,-1);
                            glDisable(GL_BLEND);
                        glPopMatrix();	
                        glEnable(GL_TEXTURE_2D);
                    }else{
                        continue;
                    }
                
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
void defineTextura(int index,char* textura){
    glGenTextures(1, &textures[index]);
    glBindTexture(GL_TEXTURE_2D, textures[index]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    imag.LoadBmpFile(textura);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());

}
void criaDefineTexturas(void){	
    //----------------------------------------- Chao z=0
    defineTextura(0,"textures/chao.bmp");
    defineTextura(1,"textures/brilho.bmp");
    defineTextura(2,"textures/lt.bmp");
    defineTextura(3,"textures/rt.bmp");
    defineTextura(4,"textures/ft.bmp");
    defineTextura(5,"textures/bk.bmp");
    defineTextura(6,"textures/up.bmp");
    defineTextura(7,"textures/top1.bmp");
    defineTextura(8,"textures/coca1.bmp");
    defineTextura(9,"textures/topcan3.bmp");
    defineTextura(10,"textures/metal3.bmp");
    defineTextura(11,"textures/tijolo.bmp");
    defineTextura(12,"textures/canfloor.bmp");
    defineTextura(13,"textures/capa.bmp");
    defineTextura(14,"textures/par3.bmp");
    defineTextura(15,"textures/chaosujo.bmp");
    defineTextura(16,"textures/detail1.bmp");
    defineTextura(17,"textures/prateleira.bmp");
    defineTextura(18,"textures/metaltable.bmp");
    defineTextura(19,"textures/wood.bmp");
    defineTextura(20,"textures/calendar.bmp");
    defineTextura(21,"textures/candeiro.bmp");
    defineTextura(22,"textures/ini.bmp");
    defineTextura(23,"textures/paper2.bmp");
    defineTextura(24,"textures/paper3.bmp");
    defineTextura(25,"textures/paper4.bmp");
    defineTextura(26,"textures/coca2.bmp");
    defineTextura(27,"textures/coca3.bmp");
    defineTextura(28,"textures/coca4.bmp");
    defineTextura(29,"textures/wallstain.bmp");
    defineTextura(30, "textures/1.bmp");
	defineTextura(31, "textures/2.bmp");
	defineTextura(32, "textures/3.bmp");
	defineTextura(33, "textures/4.bmp");
	defineTextura(34, "textures/5.bmp");
	defineTextura(35, "textures/6.bmp");
    defineTextura(40,"textures/chaleira.bmp");
    defineTextura(41,"textures/factory.bmp");
    defineTextura(42,"textures/quadro1.bmp");
    defineTextura(43,"textures/quadro2.bmp");
    defineTextura(44,"textures/quadro3.bmp");
    defineTextura(45,"textures/quadro4.bmp");
    

    
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
    glDisable(GL_CULL_FACE);

	for (int i = 0; i<16; i++) {
		glPushMatrix();
		glTranslatef(5.1, 1.3, canWalk[0] + i - 0.5);
		glRotatef(-90, 1, 0, 0);
		draw_cylinder(1, 8);
		glPopMatrix();
		//printf("%.02f \n", canWalk[0]);
	}
	
	if (trabalha) {
		canWalk[0] += 0.02;
		if (canWalk[0] > 6)
			canWalk[0] = 5;
	}
	glEnable(GL_CULL_FACE);

	for (int i = 0; i<15; i++) {
		glPushMatrix();
		glTranslatef(canWalk[1] + i, 1.3, 20.1);
		glRotatef(-90, 1, 0, 0);
		draw_cylinder(2, 8);
		glPopMatrix();
	}
	if (trabalha) {
		canWalk[1] += 0.02;
		if (canWalk[1] > 6)
			canWalk[1] = 5;
	}

	for (int i = 0; i<16; i++) {
		glPushMatrix();
		glTranslatef(20.1, 1.3, canWalk[2] - i + 0.5);
		glRotatef(-90, 1, 0, 0);
		draw_cylinder(3, 8);
		glPopMatrix();
	}
	if (trabalha) {
		canWalk[2] -= 0.02;
		if (canWalk[2] < 19)
			canWalk[2] = 20;
	}
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
    if(trabalha)
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
    //parede direita
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
    glDisable(GL_CULL_FACE);
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);	              // Select Our Texture
        glBindTexture(GL_TEXTURE_2D, textures[21]);
            glMaterialfv(GL_FRONT, GL_AMBIENT, preto);
            glTranslatef(8,9,8);
            glRotatef(-90,1,0,0);
            gluQuadricDrawStyle ( lamp, GLU_FILL   );
            gluQuadricNormals   ( lamp, GLU_SMOOTH );
            gluQuadricTexture   ( lamp, GL_TRUE    );
            glDisable(GL_TEXTURE_2D);
            gluCylinder(lamp,0.02f,0.02f,3,15,10);
            glTranslatef(0,0,-1);
            glEnable(GL_TEXTURE_2D);	              // Select Our Texture
            glBindTexture(GL_TEXTURE_2D, textures[21]);
            gluCylinder(lamp,0.4f,0.02f,1,15,10);
            glDisable(GL_TEXTURE_2D);
            glMaterialfv(GL_FRONT, GL_AMBIENT, branco);
            glTranslatef(0,0,0.1);
            glutSolidSphere(0.25, 50, 50);
        
    glPopMatrix();
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);	              // Select Our Texture
        glBindTexture(GL_TEXTURE_2D, textures[21]);
            glMaterialfv(GL_FRONT, GL_AMBIENT, preto);
            glTranslatef(16,9,8);
            glRotatef(-90,1,0,0);
            gluQuadricDrawStyle ( lamp, GLU_FILL   );
            gluQuadricNormals   ( lamp, GLU_SMOOTH );
            gluQuadricTexture   ( lamp, GL_TRUE    );
            glDisable(GL_TEXTURE_2D);
            gluCylinder(lamp,0.02f,0.02f,3,15,10);
            glTranslatef(0,0,-1);
            glEnable(GL_TEXTURE_2D);	              // Select Our Texture
            glBindTexture(GL_TEXTURE_2D, textures[21]);
            gluCylinder(lamp,0.4f,0.02f,1,15,10);
            glDisable(GL_TEXTURE_2D);
            if (tet) {
                glMaterialfv(GL_FRONT, GL_AMBIENT, amarelo);
            }
            else {
                glMaterialfv(GL_FRONT, GL_AMBIENT, branco);
            }
            glTranslatef(0,0,0.1);
            glutSolidSphere(0.25, 50, 50);
    glPopMatrix();
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);	              // Select Our Texture
        glBindTexture(GL_TEXTURE_2D, textures[21]);
            glMaterialfv(GL_FRONT, GL_AMBIENT, preto);
            glTranslatef(8,9,16);
            glRotatef(-90,1,0,0);
            gluQuadricDrawStyle ( lamp, GLU_FILL   );
            gluQuadricNormals   ( lamp, GLU_SMOOTH );
            gluQuadricTexture   ( lamp, GL_TRUE    );
            glDisable(GL_TEXTURE_2D);
            gluCylinder(lamp,0.02f,0.02f,3,15,10);
            glTranslatef(0,0,-1);
            glEnable(GL_TEXTURE_2D);	              // Select Our Texture
        glBindTexture(GL_TEXTURE_2D, textures[21]);
            gluCylinder(lamp,0.4f,0.02f,1,15,10);
            glDisable(GL_TEXTURE_2D);
            glDisable(GL_TEXTURE_2D);
            if (tet) {
                glMaterialfv(GL_FRONT, GL_AMBIENT, amarelo);
            }
            else {
                glMaterialfv(GL_FRONT, GL_AMBIENT, branco);
            }
            glTranslatef(0,0,0.1);
            glutSolidSphere(0.25, 50, 50);
    glPopMatrix();
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);	              // Select Our Texture
        glBindTexture(GL_TEXTURE_2D, textures[21]);
            glMaterialfv(GL_FRONT, GL_AMBIENT, preto);
            glTranslatef(16,9,16);
            glRotatef(-90,1,0,0);
            gluQuadricDrawStyle ( lamp, GLU_FILL   );
            gluQuadricNormals   ( lamp, GLU_SMOOTH );
            gluQuadricTexture   ( lamp, GL_TRUE    );
            glDisable(GL_TEXTURE_2D);
            gluCylinder(lamp,0.02f,0.02f,3,15,10);
            glTranslatef(0,0,-1);
            glEnable(GL_TEXTURE_2D);	              // Select Our Texture
            glBindTexture(GL_TEXTURE_2D, textures[21]);
            gluCylinder(lamp,0.4f,0.02f,1,15,10);
            glDisable(GL_TEXTURE_2D);
            if (tet) {
                glMaterialfv(GL_FRONT, GL_AMBIENT, amarelo);
            }
            else {
                glMaterialfv(GL_FRONT, GL_AMBIENT, branco);
            }
            glTranslatef(0,0,0.1);
            glutSolidSphere(0.25, 50, 50);
    glPopMatrix();
    glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[21]);
	glMaterialfv(GL_FRONT, GL_AMBIENT, preto);
	glTranslatef(15, 5, 22.5);
	glRotatef(-90, 1, 0, 0);
	gluQuadricDrawStyle(lamp, GLU_FILL);
	gluQuadricNormals(lamp, GLU_SMOOTH);
	gluQuadricTexture(lamp, GL_TRUE);
	glDisable(GL_TEXTURE_2D);
	gluCylinder(lamp, 0.02f, 0.02f, 6, 15, 10);
	glTranslatef(0, 0, -1);
	glEnable(GL_TEXTURE_2D);	              // Select Our Texture
	glBindTexture(GL_TEXTURE_2D, textures[21]);
	gluCylinder(lamp, 0.4f, 0.02f, 1, 15, 10);
	glDisable(GL_TEXTURE_2D);
	if (ligaLuz2) {
		glMaterialfv(GL_FRONT, GL_AMBIENT, azul);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT, branco);
	}
	glTranslatef(0, 0, 0.1);
	glutSolidSphere(0.25, 50, 50);
	glPopMatrix();
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[21]);
	glMaterialfv(GL_FRONT, GL_AMBIENT, preto);
	glTranslatef(13.5, 5, 22.5);
	glRotatef(-90, 1, 0, 0);
	gluQuadricDrawStyle(lamp, GLU_FILL);
	gluQuadricNormals(lamp, GLU_SMOOTH);
	gluQuadricTexture(lamp, GL_TRUE);
	glDisable(GL_TEXTURE_2D);
	gluCylinder(lamp, 0.02f, 0.02f, 6, 15, 10);
	glTranslatef(0, 0, -1);
	glEnable(GL_TEXTURE_2D);	              // Select Our Texture
	glBindTexture(GL_TEXTURE_2D, textures[21]);
	gluCylinder(lamp, 0.4f, 0.02f, 1, 15, 10);
	glDisable(GL_TEXTURE_2D);
	if (ligaLuz) {
		glMaterialfv(GL_FRONT, GL_AMBIENT, verde);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT, branco);
	}
	glTranslatef(0, 0, 0.1);
	glutSolidSphere(0.25, 50, 50);
	glPopMatrix();
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[21]);
	glMaterialfv(GL_FRONT, GL_AMBIENT, preto);
	glTranslatef(12, 5, 22.5);
	glRotatef(-90, 1, 0, 0);
	gluQuadricDrawStyle(lamp, GLU_FILL);
	gluQuadricNormals(lamp, GLU_SMOOTH);
	gluQuadricTexture(lamp, GL_TRUE);
	glDisable(GL_TEXTURE_2D);
	gluCylinder(lamp, 0.02f, 0.02f, 6, 15, 10);
	glTranslatef(0, 0, -1);
	glEnable(GL_TEXTURE_2D);	              // Select Our Texture
	glBindTexture(GL_TEXTURE_2D, textures[21]);
	gluCylinder(lamp, 0.4f, 0.02f, 1, 15, 10);
	glDisable(GL_TEXTURE_2D);
	if (ligaLuz1) {
		glMaterialfv(GL_FRONT, GL_AMBIENT, vermelho);
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT, branco);
	}
	glTranslatef(0, 0, 0.1);
	glutSolidSphere(0.25, 50, 50);
	glPopMatrix();
    glEnable(GL_CULL_FACE);
}
void drawMesaDeatails(){
    drawTRBlock(9.5,1.8,1.8,90,0,1,0,1.2,0.2,6,19);
    drawTRBlock(9.5, 1.8, 23, 90, 0, 1, 0, 1.2, 0.2, 6, 19);
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);	              // Select Our Texture
        glBindTexture(GL_TEXTURE_2D, textures[18]);
            glTranslatef(12.5,1.3,1.2);
            glRotatef(-90,1,0,0);
            gluQuadricDrawStyle ( lamp, GLU_FILL   );
            gluQuadricNormals   ( lamp, GLU_SMOOTH );
            gluQuadricTexture   ( lamp, GL_TRUE    );
            gluCylinder(lamp,0.1f,0.4f,0.5,15,10);
            glTranslatef(0,0,-1.2);
            gluCylinder(lamp,0.8f,0.1f,1.2,30,10);
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
	glEnable(GL_TEXTURE_2D);	              // Select Our Texture
	glBindTexture(GL_TEXTURE_2D, textures[18]);
	glTranslatef(12.5, 1.3, 22.4);
	glRotatef(-90, 1, 0, 0);
	gluQuadricDrawStyle(lamp, GLU_FILL);
	gluQuadricNormals(lamp, GLU_SMOOTH);
	gluQuadricTexture(lamp, GL_TRUE);
	gluCylinder(lamp, 0.1f, 0.4f, 0.5, 15, 10);
	glTranslatef(0, 0, -1.2);
	gluCylinder(lamp, 0.8f, 0.1f, 1.2, 30, 10);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

    glPushMatrix();
        //glEnable(GL_TEXTURE_2D);
        glPushMatrix();
            //glBindTexture(GL_TEXTURE_2D, textures[18]);
            glTranslatef(10.5,2.26,1.2);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,textures[40]);
                glutSolidTeapot(0.3);
            glDisable(GL_TEXTURE_2D);
            glPushMatrix();
                //glBindTexture(GL_TEXTURE_2D, textures[18]);
                glMaterialfv(GL_FRONT, GL_AMBIENT, copo);
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
                glColor4f(0.0f, 0.0f, 1.0f, 0.2f);
                glTranslatef(1,-0.2,0.2);
                glRotatef(-90,1,0,0);
                glDisable(GL_CULL_FACE);
                gluCylinder(lamp,0.08f,0.1f,0.4,30,10);
                glTranslatef(-0.3,-0.2,0);
                gluCylinder(lamp,0.08f,0.1f,0.4,30,10);
                glEnable(GL_CULL_FACE);
                glDisable(GL_BLEND);
            glPopMatrix();
            
        glPopMatrix();
        //glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    draw_objecto0();

    //cadeira
    drawTRBlock(15,0,3,-90,1,0,0,0.1,0.1,2.3,19);
    drawTRBlock(14.2,0,3,-90,1,0,0,0.1,0.1,2.3,19);
    drawTRBlock(15,0,2.2,-90,1,0,0,0.1,0.1,1,19);
    drawTRBlock(14.2,0,2.2,-90,1,0,0,0.1,0.1,1,19);
    drawTRBlock(14.2,1,3,90,0,1,0,0.9,0.1,0.9,19);
    drawTRBlock(14.2,1.7,3,-90,1,0,0,0.9,0.1,0.4,19);

    glPushMatrix();
        glTranslatef(14.9,2.06,1.4);
        glScalef(0.4,0.8,0.6);
        glRotatef(180,0,1,0);
        glRotatef(90,1,0,0);
        glRotatef(-25,0,0,1);
        drawDetail(23);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(15,2.05,1.5);
        glScalef(0.4,0.8,0.6);
        glRotatef(180,0,1,0);
        glRotatef(90,1,0,0);
        drawDetail(24);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(14,2.05,1.5);
        glScalef(0.4,0.8,0.6);
        glRotatef(180,0,1,0);
        glRotatef(90,1,0,0);
        glRotatef(-15,0,0,1);
        drawDetail(25);
    glPopMatrix();

    glDisable(GL_CULL_FACE);
    glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT,cinzento);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cinzento);
	glMaterialfv(GL_FRONT, GL_SPECULAR, cinzento);
	

	glTranslatef(15, 2, 22.5);
	glRotatef(-90, 1, 0, 0);
	draw_cylinder(1,8);
	glPopMatrix();
	
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, cinzento);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cinzento);
	glMaterialfv(GL_FRONT, GL_SPECULAR, cinzento);
	glTranslatef(13.5, 2, 22.5);
	glRotatef(-90, 1, 0, 0);
	draw_cylinder(1,8);
	glPopMatrix();
	
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, cinzento);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cinzento);
	glMaterialfv(GL_FRONT, GL_SPECULAR, cinzento);
	glTranslatef(12, 2, 22.5);
	glRotatef(-90, 1, 0, 0);
	draw_cylinder(1,8);
	glPopMatrix();
	glEnable(GL_CULL_FACE);

}
void drawCansAllOverThePlace(){

        //primeira prateleira
        glPushMatrix();
            glTranslatef(0.25,5.2,5);
            glRotatef(-90, 1,0,0);
            glRotatef(-230, 0,0,1);
            draw_cylinder(3,27);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0,5.4,4.2);
            glRotatef(-90, 0,1,0);
            glRotatef(90, 0,0,1);
            glRotatef(135, 1,0,0);
            draw_cylinder(3,28);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.25,5.2,7);
            glRotatef(-90, 1,0,0);
            draw_cylinder(3,26);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.25,5.2,8);
            glRotatef(-90, 1,0,0);
            glRotatef(-180, 0,0,1);
            draw_cylinder(3,8);
        glPopMatrix();


        //segunda prateleira
        glPushMatrix();
            glTranslatef(0.25,2.4,9.5);
            glRotatef(-90, 1,0,0);
            glRotatef(-230, 0,0,1);
            draw_cylinder(3,28);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.25,2.4,14);
            glRotatef(-90, 1,0,0);
            glRotatef(-160, 0,0,1);
            draw_cylinder(3,26);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0,2.6,13);
            glRotatef(-90, 0,1,0);
            glRotatef(90, 0,0,1);
            glRotatef(135, 1,0,0);
            draw_cylinder(3,8);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.25,2.4,12);
            glRotatef(-90, 1,0,0);
            glRotatef(-180, 0,0,1);
            draw_cylinder(3,27);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.25,2.4,11.2);
            glRotatef(-90, 1,0,0);
            glRotatef(-180, 0,0,1);
            draw_cylinder(3,8);
        glPopMatrix();



        //terceira prateleira
        glPushMatrix();
            glTranslatef(0.25,5.2,17.5);
            glRotatef(-90, 1,0,0);
            glRotatef(-230, 0,0,1);
            draw_cylinder(3,27);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0,5.4,15.5);
            glRotatef(-90, 0,1,0);
            glRotatef(90, 0,0,1);
            glRotatef(135, 1,0,0);
            draw_cylinder(3,28);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0,5.4,16);
            glRotatef(-90, 0,1,0);
            glRotatef(90, 0,0,1);
            glRotatef(135, 1,0,0);
            draw_cylinder(3,26);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.25,5.2,18.5);
            glRotatef(-90, 1,0,0);
            glRotatef(-50, 0,0,1);
            draw_cylinder(3,26);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.25,5.2,20);
            glRotatef(-90, 1,0,0);
            glRotatef(-180, 0,0,1);
            draw_cylinder(3,8);
        glPopMatrix();

        //--------------------------------------------------------------------lado esquerdo
        //primeira prateleira
        glPushMatrix();
            glTranslatef(24.5,5.2,5);
            glRotatef(-90, 1,0,0);
            //glRotatef(-230, 0,0,1);
            draw_cylinder(3,27);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(24.75,5.4,4.2);
            glRotatef(-90, 0,1,0);
            glRotatef(90, 0,0,1);
            glRotatef(-45, 1,0,0);
            draw_cylinder(3,28);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(24.5,5.2,7);
            glRotatef(-90, 1,0,0);
            draw_cylinder(3,26);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(24.5,5.2,8);
            glRotatef(-90, 1,0,0);
            //glRotatef(-180, 0,0,1);
            draw_cylinder(3,8);
        glPopMatrix();


        //segunda prateleira
        glPushMatrix();
            glTranslatef(24.5,2.4,9.5);
            glRotatef(-90, 1,0,0);
            glRotatef(-230, 0,0,1);
            draw_cylinder(3,26);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(24.5,2.4,14);
            glRotatef(-90, 1,0,0);
            //glRotatef(-160, 0,0,1);
            draw_cylinder(3,28);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(24.75,2.6,13);
            glRotatef(-90, 0,1,0);
            glRotatef(90, 0,0,1);
            glRotatef(-45, 1,0,0);
            draw_cylinder(3,26);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(24.5,2.4,12);
            glRotatef(-90, 1,0,0);
            //glRotatef(-180, 0,0,1);
            draw_cylinder(3,8);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(24.5,2.4,11.2);
            glRotatef(-90, 1,0,0);
            //glRotatef(-180, 0,0,1);
            draw_cylinder(3,27);
        glPopMatrix();


         //terceira prateleira
        glPushMatrix();
            glTranslatef(24.5,5.2,17.5);
            glRotatef(-90, 1,0,0);
            //glRotatef(-230, 0,0,1);
            draw_cylinder(3,27);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(24.75,5.4,16);
            glRotatef(-90, 0,1,0);
            glRotatef(90, 0,0,1);
            glRotatef(-45, 1,0,0);
            draw_cylinder(3,28);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(24.75,5.4,16.5);
            glRotatef(-90, 0,1,0);
            glRotatef(90, 0,0,1);
            glRotatef(-45, 1,0,0);
            draw_cylinder(3,26);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(24.5,5.2,18.5);
            glRotatef(-90, 1,0,0);
            glRotatef(-50, 0,0,1);
            draw_cylinder(3,26);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(24.5,5.2,20);
            glRotatef(-90, 1,0,0);
            //glRotatef(-180, 0,0,1);
            draw_cylinder(3,8);
        glPopMatrix();


}
void drawWallStains(){
    int count=0;
    float amount=0.2;
    while(count!=11){
        glPushMatrix();
            glTranslatef(0.001,7.5,amount);
            glScalef(1,2.5,2.5);
            glRotatef(-90,0,1,0);
            drawDetail(29);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(24.799,7.5,amount+2.5);
            glScalef(1,2.5,2.5);
            glRotatef(90,0,1,0);
            drawDetail(29);
        glPopMatrix();


        glPushMatrix();
            glTranslatef(amount,7.5,0.201);
            glScalef(2.5,2.5,1);
            glRotatef(180,0,1,0);
            drawDetail(29);
        glPopMatrix();


        glPushMatrix();
            glTranslatef(amount-0.2,7.5,24.99);
            glScalef(2.5,2.5,1);
            drawDetail(29);
        glPopMatrix();
        
        amount+=2.5;
        count++;
    }
}
void printText(char *string, GLfloat x, GLfloat y, GLfloat z) {  
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT, preto);
        glRasterPos3f(x,y,z); 
        while(*string)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *string++); 
    glPopMatrix();
}
void quadros(){
    glPushMatrix();
        glTranslatef(7.6,6,0.21);
        glScalef(2.5,2.5,1);
        glRotatef(180,0,1,0);
        drawDetail(42);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(11.8,6,0.21);
        glScalef(2.5,2.5,1);
        glRotatef(180,0,1,0);
        drawDetail(43);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(16,6,0.21);
        glScalef(2.5,2.5,1);
        glRotatef(180,0,1,0);
        drawDetail(44);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(20,6,0.21);
        glScalef(2.5,2.5,1);
        glRotatef(180,0,1,0);
        drawDetail(45);
    glPopMatrix();
}
void drawDetails(){
    lamp = gluNewQuadric();
    //Candeiros
    drawLamps();

    //mancha de oleo
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
    drawPrateleiras();

    drawMesaDeatails();

    //calendario
    glPushMatrix();
        glTranslatef(12.3,2.8,0.21);
        glScalef(2,2,1);
        glRotatef(180,0,1,0);
        drawDetail(20);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(14.6,2.8,0.21);
        glScalef(2,2,1);
        glRotatef(180,0,1,0);
        drawDetail(41);
    glPopMatrix();

    //quadros
    quadros();

    //Latas espalhadas pela casa
    drawCansAllOverThePlace();

    //manchas parede
    drawWallStains();

    //quadros
    //printText("Teste numero 1",14,5,24.5);


}
void drawAlavanca(){
    button = gluNewQuadric();
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT, castanho);
        drawPathSupport(17.2,0,1.5,0.5,0.4,1,-1);
    glPopMatrix();
    if(alavanca==1){
        if(alavancaLigada==1){ //acabar
            glPushMatrix();
                glDisable(GL_CULL_FACE);
                glMaterialfv(GL_FRONT, GL_AMBIENT, vermelho);
                glTranslatef(17.45,0.3,2);
                glRotatef(-135,1,0,0);
                gluCylinder(button,0.04f,0.04f,2,15,10);
                glEnable(GL_CULL_FACE);
            glPopMatrix();
            glPushMatrix();
                glMaterialfv(GL_FRONT, GL_AMBIENT, cinzento);
                glTranslatef(17.45,0.3,2);
                glRotatef(-135,1,0,0);
                gluCylinder(button,0.0401f,0.0401f,1.9,15,10);
            glPopMatrix();
            /*glPushMatrix();
                glMaterialfv(GL_FRONT, GL_AMBIENT, cinzento);
                glTranslatef(17.2,3,1.5);
                glRotatef(90,1,0,0);
                gluCylinder(button,0.04f,0.04f,2,15,10);
            glPopMatrix();*/
        }else{
            glPushMatrix();
                glDisable(GL_CULL_FACE);
                glMaterialfv(GL_FRONT, GL_AMBIENT, vermelho);
                glTranslatef(17.45,0.3,2);
                glRotatef(-45,1,0,0);
                gluCylinder(button,0.04f,0.04f,2,15,10);
                glEnable(GL_CULL_FACE);
            glPopMatrix();
            glPushMatrix();
                glMaterialfv(GL_FRONT, GL_AMBIENT, cinzento);
                glTranslatef(17.45,0.3,2);
                glRotatef(-45,1,0,0);
                gluCylinder(button,0.0401f,0.0401f,1.9,15,10);
            glPopMatrix();
        }
    }else{
        
        if(alavancaLigada==1){  //acabar
            B_angulo+=0.8;
            if(B_angulo>(-45)){
                printf("Angulo: %.2f \n",B_angulo);
                B_angulo=-45;
                alavanca=1;
                alavancaLigada=0;
            }
            glPushMatrix();
                glDisable(GL_CULL_FACE);
                glMaterialfv(GL_FRONT, GL_AMBIENT, vermelho);
                glTranslatef(17.45,0.3,2);
                glRotatef(B_angulo,1,0,0);
                gluCylinder(button,0.04f,0.04f,2,15,10);
                glEnable(GL_CULL_FACE);
            glPopMatrix();
            glPushMatrix();
                glMaterialfv(GL_FRONT, GL_AMBIENT, cinzento);
                glTranslatef(17.45,0.3,2);
                glRotatef(B_angulo,1,0,0);
                gluCylinder(button,0.0401f,0.0401f,1.9,15,10);
            glPopMatrix();

            /*glPushMatrix();
                glMaterialfv(GL_FRONT, GL_AMBIENT, cinzento);
                glTranslatef(17.2,3,1.5);
                glRotatef(90,1,0,0);
                gluCylinder(button,0.04f,0.04f,2,15,10);
            glPopMatrix();*/
        }else{
            B_angulo-=0.8;
            if(B_angulo<(-135)){
                printf("Angulo: %.2f \n",B_angulo);
                B_angulo=-135;
                alavanca=1;
                alavancaLigada=1;
            }
            glPushMatrix();
                glDisable(GL_CULL_FACE);
                glMaterialfv(GL_FRONT, GL_AMBIENT, vermelho);
                glTranslatef(17.45,0.3,2);
                glRotatef(B_angulo,1,0,0);
                gluCylinder(button,0.04f,0.04f,2,15,10);
                glEnable(GL_CULL_FACE);
            glPopMatrix();
            glPushMatrix();
                glMaterialfv(GL_FRONT, GL_AMBIENT, cinzento);
                glTranslatef(17.45,0.3,2);
                glRotatef(B_angulo,1,0,0);
                gluCylinder(button,0.0401f,0.0401f,1.9,15,10);
            glPopMatrix();
        }

    }

}
void drawScene(void){
    //Desenha chão
    glDisable(GL_COLOR_MATERIAL);
    
    
    drawFloor(25,25);
    drawAlavanca();
    drawDetails();

    
    
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT, cinzento);
        drawCanEngine();
        drawCanFloorEngine();
    glPopMatrix();
    
}
void updateKeys(){
    
	if(pressed[0]==1){
        printf("Coordenadas: X-%.2f  Y-%.2f \n",obsPini[0],obsPfin[0]);
        obsPini[0]=(obsPini[0]-incVisao*cos(aVisao));
        obsPini[2]=(obsPini[2]+incVisao*sin(aVisao));

        obsPfin[0]=(obsPini[0]-rVisao*cos(aVisao));
        obsPfin[2]=(obsPini[2]+rVisao*sin(aVisao));
        
	}
	if(pressed[1]==1){
        printf("Coordenadas: X-%.2f  Y-%.2f \n",obsPini[0],obsPfin[0]);
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
		if(obsPfin[1]<6){
			aVisaoY+=0.05;
			obsPfin[1]=obsPini[1]+aVisaoY;
		}
	}
	if(pressed[5]==1){
		if(obsPfin[1]>1){
			aVisaoY-=0.05;
			obsPfin[1]=obsPini[1]+aVisaoY;
		}
	}
        if(obsPini[0]>19.1){
          obsPini[0]=19.09;
        }
        if(obsPini[0]<6.20){
            obsPini[0]=6.21;
        }
        if(obsPini[2]>19.1){
            obsPini[2]=19.09;
        }
        if(obsPini[2]<2){
            obsPini[2]=2.01;
        }

    lightambient[0] = intLuzA;
	lightambient[1] = intLuzA;
	lightambient[2] = intLuzA;
	lightdiffuse[0] = intLuzD;
	lightdiffuse[1] = intLuzD;
	lightdiffuse[2] = intLuzD;
	//glLightfv(GL_LIGHT6, GL_AMBIENT, lightambient );   
    //glLightfv(GL_LIGHT6, GL_DIFFUSE, lightdiffuse ); 
    
    if(pressed[8]==1){
        alfa=alfa+0.02;
        if (alfa>=1) alfa=1;
    }
     if(pressed[9]==1){
        alfa=alfa-0.02;
        if (alfa<=0) alfa=0;
    }

    goldD[3]	=alfa;
	silverD[3]  =alfa;

	glutPostRedisplay();
}
void drawSkybox(float size) {

	size = size / 2.0f;
	int aumento = 0;
	if (!noite) {
		aumento = 8;
	}

	glEnable(GL_TEXTURE_2D);

	if (noite) {
		glBindTexture(GL_TEXTURE_2D, textures[5]);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, textures[34]);

	}
	glPushMatrix();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3i(obsPini[0] - size - aumento, obsPini[1] - size, obsPini[2] - size);
	glTexCoord2f(1.0f, 0.0f); glVertex3i(obsPini[0] + size, obsPini[1] - size, obsPini[2] - size);
	glTexCoord2f(1.0f, 1.0f); glVertex3i(obsPini[0] + size, obsPini[1] + size, obsPini[2] - size);
	glTexCoord2f(0.0f, 1.0f); glVertex3i(obsPini[0] - size - aumento, obsPini[1] + size, obsPini[2] - size);
	glEnd();

	glPopMatrix();


	if (noite) {
		glBindTexture(GL_TEXTURE_2D, textures[3]);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, textures[30]);
	}
	glPushMatrix();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3i(obsPini[0] + size, obsPini[1] - size, obsPini[2] - size - aumento);
	glTexCoord2f(1.0f, 0.0f); glVertex3i(obsPini[0] + size, obsPini[1] - size, obsPini[2] + size);
	glTexCoord2f(1.0f, 1.0f); glVertex3i(obsPini[0] + size, obsPini[1] + size, obsPini[2] + size);
	glTexCoord2f(0.0f, 1.0f); glVertex3i(obsPini[0] + size, obsPini[1] + size, obsPini[2] - size - aumento);
	glEnd();

	glPopMatrix();

	int aumento2 = 0;
	if (noite) {
		glBindTexture(GL_TEXTURE_2D, textures[4]);
	}
	else {
		aumento2 = 5;
		glBindTexture(GL_TEXTURE_2D, textures[33]);
	}
	glPushMatrix();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3i(obsPini[0] + size + aumento2, obsPini[1] - size, obsPini[2] + size);
	glTexCoord2f(1.0f, 0.0f); glVertex3i(obsPini[0] - size, obsPini[1] - size, obsPini[2] + size);
	glTexCoord2f(1.0f, 1.0f); glVertex3i(obsPini[0] - size, obsPini[1] + size, obsPini[2] + size);
	glTexCoord2f(0.0f, 1.0f); glVertex3i(obsPini[0] + size + aumento2, obsPini[1] + size, obsPini[2] + size);
	glEnd();

	glPopMatrix();


	if (noite) {
		glBindTexture(GL_TEXTURE_2D, textures[2]);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, textures[32]);
	}
	glPushMatrix();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3i(obsPini[0] - size, obsPini[1] - size, obsPini[2] + size);
	glTexCoord2f(1.0f, 0.0f); glVertex3i(obsPini[0] - size, obsPini[1] - size, obsPini[2] - size);
	glTexCoord2f(1.0f, 1.0f); glVertex3i(obsPini[0] - size, obsPini[1] + size, obsPini[2] - size);
	glTexCoord2f(0.0f, 1.0f); glVertex3i(obsPini[0] - size, obsPini[1] + size, obsPini[2] + size);
	glEnd();

	glPopMatrix();

	if (noite) {
		glBindTexture(GL_TEXTURE_2D, textures[6]);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, textures[35]);
	}
	glPushMatrix();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3i(obsPini[0] + size, obsPini[1] + size, obsPini[2] - size);
	glTexCoord2f(1.0f, 0.0f); glVertex3i(obsPini[0] + size, obsPini[1] + size, obsPini[2] + size);
	glTexCoord2f(1.0f, 1.0f); glVertex3i(obsPini[0] - size, obsPini[1] + size, obsPini[2] + size);
	glTexCoord2f(0.0f, 1.0f); glVertex3i(obsPini[0] - size, obsPini[1] + size, obsPini[2] - size);
	glEnd();

	glPopMatrix();

	//Bottom
	if (noite) {
		glBindTexture(GL_TEXTURE_2D, textures[1]);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, textures[21]);
	}
	glPushMatrix();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3i(obsPini[0] - size, obsPini[1] - size, obsPini[2] - size);
	glTexCoord2f(1.0f, 0.0f); glVertex3i(obsPini[0] - size, obsPini[1] - size, obsPini[2] - size);
	glTexCoord2f(1.0f, 1.0f); glVertex3i(obsPini[0] + size, obsPini[1] + size, obsPini[2] - size);
	glTexCoord2f(0.0f, 1.0f); glVertex3i(obsPini[0] + size, obsPini[1] + size, obsPini[2] - size);
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
    drawPathParts(azul,4.1,0,0.2,2,3,4.5,22);
    drawPathParts(preto,4.5,1.2,4.61,1.2,1.2,0.1,-1);
    drawPathParts(azul,19.1,0,0.2,2,3,4.5,22);
    drawPathParts(preto,19.5,1.2,4.61,1.2,1.2,0.1,-1);

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
    drawPathSupport(19.5,1,4.5,1.2,0.2,16.2,10);
    
}

void display(void){
    //Limpa buffers
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCor);
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
    GLfloat localPos[4] = { 13.5,5,22.5 ,1 };
	GLfloat localPos1[4] = { 12.0,5,22.5 ,1.0 };
	GLfloat localPos2[4] = { 15.0,5.0,22.5 ,1.0 };
	GLfloat localPos3[4] = { 16, 9, 8 ,1.0 };
	GLfloat localPos4[4] = { 8, 9, 16 ,1.0 };
	GLfloat localPos5[4] = { 16, 9, 16 ,1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, localPos);
	glLightfv(GL_LIGHT1, GL_POSITION, localPos1);
	glLightfv(GL_LIGHT2, GL_POSITION, localPos2);
	glLightfv(GL_LIGHT3, GL_POSITION, localPos3);
	glLightfv(GL_LIGHT4, GL_POSITION, localPos4);
	glLightfv(GL_LIGHT5, GL_POSITION, localPos5);


	if (noite)
		glClearColor(GRAY1);
	else
		glClearColor(GRAY2);
	if (ligaLuz)
		glEnable(GL_LIGHT0);
	else
		glDisable(GL_LIGHT0);
	if (ligaLuz1)
		glEnable(GL_LIGHT1);
	else
		glDisable(GL_LIGHT1);
	if (ligaLuz2)
		glEnable(GL_LIGHT2);
	else
		glDisable(GL_LIGHT2);
	if (tet) {
		glEnable(GL_LIGHT3);
		glEnable(GL_LIGHT4);
		glEnable(GL_LIGHT5);
	}else {
		glDisable(GL_LIGHT3);
		glDisable(GL_LIGHT4);
		glDisable(GL_LIGHT5);
	}

    drawSkybox(100.0f);
    drawHouse();
    //Desenha "cena"
    drawScene();
    drawConveyor();
    
    
    
    glDisable(GL_CULL_FACE);
    if(faisca==1)
        showParticulas(particula1, 2);
    glEnable(GL_CULL_FACE);
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
        case 'e':
        case 'E':
        	faisca=0;
        	break;
        case 'i':
        case 'I':
            pressed[6]=0;
        	break;
        case 'k':
        case 'K':
        	pressed[7]=0;
        	break;
        case 'u':
        case 'U':
        	pressed[8]=0;
        	break;
        case 'j':
        case 'J':
        	pressed[9]=0;
        	break;
        case 'y':
        case 'Y':
        	pressed[10]=0;
        	break;
        case 'h':
        case 'H':
        	pressed[11]=0;
        	break;
        case 'N':
        case 'n':
            noite = !noite;
            if (noite)
            {
                luzGlobalCor[0] = 1.0;   luzGlobalCor[1] = 1.0;   luzGlobalCor[2] = 1.0;
            }
            else
            {
                luzGlobalCor[0] = 0.4;   luzGlobalCor[1] = 0.4;   luzGlobalCor[2] = 0.4;
            }
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCor);
            glutPostRedisplay();
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
        case 'i':
        case 'I':
            pressed[6]=1;
        	break;
        case 't':
        case 'T':
        if(obsPini[0]>16 && obsPini[2]<19.5){
            if(alavanca==0){
                if(alavancaLigada==0){
                    alavancaLigada=1;
                }else
                    alavancaLigada=0;
            }else
                alavanca=0; 
            trabalha = !trabalha;
            glutPostRedisplay();
        }
            break;
        case 'g':
        case 'G':
            ligaLuz = !ligaLuz;
            glutPostRedisplay();
            break;
        case 'r':
        case 'R':
            ligaLuz1 = !ligaLuz1;
            glutPostRedisplay();
            break;
        case 'b':
        case 'B':
            ligaLuz2 = !ligaLuz2;
            glutPostRedisplay();
            break;
        case 'e':
        case 'E':
            iniParticulas(particula1);
            faisca=1;
            break;
        case 'k':
        case 'K':
            tet = !tet;
            glutPostRedisplay();
            break;
        case 'u':
        case 'U':
        	pressed[8]=1;
        	break;
        case 'j':
        case 'J':
        	pressed[9]=1;
        	break;
        case 'y':
        case 'Y':
        	pressed[10]=1;
        	break;
        case 'h':
        case 'H':
        	pressed[11]=1;
        	break;
        case 27:
            exit(0);
            break;
    }
}
//==========================================  Functions
void idle(void)
{

 glutPostRedisplay();

}
void initLights(void) {
	//…………………………………………………………………………………………………………………………………………… Ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCor);

	GLfloat localCor[4] = { 0.0,1.0,0.0,1.0 };
	GLfloat localCorDif[4] = { 0.4, 0.4, 0.4, 1.0 };
	

	GLfloat localAttCon = 1.0;
	GLfloat localAttLin = 0.007;
	GLfloat localAttQua = 0.0;
	GLfloat dire[4] = { 0,-1,0,0 };
	GLfloat spec[4] = { 1,1,1,1 };
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, localCor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, localCorDif);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dire);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
	//glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 0.0);
	//glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, localAttCon);
	//glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, localAttLin);
	//glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, localAttQua);

	GLfloat localCor1[4] = { 1.0,0.0,0.0,1.0 };
	GLfloat localCorDif1[4] = { 0.4, 0.4, 0.4, 1.0 };
	GLfloat localPos1[4] = { 13.5,4.0,1.0 ,1.0 };
	GLfloat localAttCon1 = 1.0;
	GLfloat localAttLin1 = 0.007;
	GLfloat localAttQua1 = 0.0;
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
	glLightfv(GL_LIGHT1, GL_AMBIENT, localCor1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, localCorDif1);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dire);
	glLightfv(GL_LIGHT1, GL_SPECULAR, spec);


	GLfloat localCor2[4] = { 0.0,0.0,1.0,1.0 };
	GLfloat localCorDif2[4] = { 0.4, 0.4, 0.4, 1.0 };
	GLfloat localPos2[4] = { 15.0,4.0,1.0 ,1.0 };
	GLfloat localAttCon2 = 1.0;
	GLfloat localAttLin2 = 0.05;
	GLfloat localAttQua2 = 0.0;


	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30.0);
	glLightfv(GL_LIGHT2, GL_AMBIENT, localCor2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, localCorDif2);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, dire);
	glLightfv(GL_LIGHT2, GL_SPECULAR, spec);
	//glLightf(GL_LIGHT2, GL_SHININESS, 32.0);

	GLfloat localCor3[4] = { 1.0,1.0,0.0,1.0 };

	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 30.0);
	glLightfv(GL_LIGHT3, GL_AMBIENT, localCor3);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, localCorDif);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, dire);
	glLightfv(GL_LIGHT3, GL_SPECULAR, spec);

	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 30.0);
	glLightfv(GL_LIGHT4, GL_AMBIENT, localCor3);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, localCorDif);
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, dire);
	glLightfv(GL_LIGHT4, GL_SPECULAR, spec);

	glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 30.0);
	glLightfv(GL_LIGHT5, GL_AMBIENT, localCor3);
	glLightfv(GL_LIGHT5, GL_DIFFUSE, localCorDif);
	glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, dire);
	glLightfv(GL_LIGHT5, GL_SPECULAR, spec);




}

void Timer(int value) 
{
	iniParticulas(particula1);

	glutPostRedisplay();
	glutTimerFunc(milisec ,Timer, 1);   //.. Espera msecDelay milisegundos
}
void init(void){
    glClearColor(WHITE);
    glEnable(GL_DEPTH_TEST);  //activa o zbuffer para ter em conta a profundidade
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    criaDefineTexturas();
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_TEXTURE_2D);
    iniParticulas(particula1);
    glEnable(GL_LIGHTING);
    initLights();
    
    
    
}


int main(int argc, char** argv){
    
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
    glutInitWindowSize (wScreen, hScreen);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Projeto Final CG 2017/2018 ");
    //glutFullScreen();
    init();
    //glutReshapeFunc(resize);
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    glutDisplayFunc(display);
     glutIdleFunc(idle);
    glutTimerFunc(milisec , Timer, 1);     //.. fun��o callback

    glutMainLoop();
    
    return 0;
}