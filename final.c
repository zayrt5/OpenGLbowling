/*
 *  Textures and Lighting
 *
 *  Demonstrates using lighting and textures.
 *
 *  Key bindings:
 *  l          Toggle lighting on/off
 *  t          Change textures
 *  m          Toggles texture mode modulate/replace
 *  a/A        decrease/increase ambient light
 *  d/D        decrease/increase diffuse light
 *  s/S        decrease/increase specular light
 *  e/E        decrease/increase emitted light
 *  n/N        Decrease/increase shininess
 *  []         Lower/rise light
 *  x          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx229.h"
int lightmode=0;
int mode=0;       //  spective
int ntex=0;       //  Cube faces
int axes=1;       //  Display axes
int fp=1;
int fov=60;
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=3.4;   //  Size of world
int fps=30;
int dt=50;     // Timer period (ms)
// Light values
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;    // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light
unsigned int texture[7]; // Texture names
int rot = 0.0; 





/* Camera looking coords 
 * Don't need a y coord because up-down view cannot be altered in fp 
 */
double Cx = 0; 
double Cz = 0; 

double Ex = 0;
double Ey = 0;
double Ez = 5;

int throwing = 0;

float xVal = 0;
float zVal = 0;

float xVec = 1;
float zVec = 1;


 static void Vertex(double th,double ph)
{
   double x = Cos(th)*Cos(ph);
   double y = Sin(th)*Cos(ph);
   double z =         Sin(ph);
   glNormal3d(x,y,z);
   glTexCoord2d(th/360.0,ph/180.0+0.5);

   glVertex3d(x,y,z);
}



static void drawBall(double x,double y,double z,double r, int texnum)
{

   
   int th,ph;

   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);


   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, texture[texnum]);
   

   glColor3f(1,1,1);
   //  Latitude bands

   for (ph=-90;ph<90;ph+=5)
   {
      glBegin(GL_QUAD_STRIP);
      
      for (th=0;th<=360;th+=5)
      {
         
         Vertex(th,ph);
         Vertex(th,ph+5);
      }
      
      glEnd();
   }


   glDisable(GL_TEXTURE_2D);

   glPopMatrix();


   //  Undo transformations
}



void timer(int v) {
   //couldnt quite get the math right on the throwing normal to the camera. works sometimes, im assuming negatives mess my logic up
   //regardless, time permits this state of function
   if(!throwing){
   xVal = Ex + (Cx*.5);
   zVal = Ez + (Cz*.5);
   xVec = (Cx*Ex);
   zVec = (Cz*Ez);
   }
   
   if(throwing){
    xVal += xVec/10;
    zVal += zVec/10;
    if (xVal >30 || xVal < -30 || zVal >30 || zVal < -30 ) {
      throwing = 0;
      xVal = Ex + Cx/4;
      zVal = Ez + Cz/4;
      glutPostRedisplay();
    }
    glutPostRedisplay();
   }
    

   glutTimerFunc(100, timer, v);
  
}





static void laneArrow(double x, double y, double z,
		      double dx, double dy, double dz,
		      double th){

   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[1]);


   glBegin(GL_TRIANGLE_STRIP);
   glNormal3f(0,.01,0);
    glVertex3f(-1,0,0 );
    glVertex3f(0,0,2 );
    glVertex3f(0,0,1 );

    glVertex3f(1,0,0 );

   glEnd();


   //  Undo transformations and textures
   glPopMatrix();
   glDisable(GL_TEXTURE_2D); 



}

static void drawCyl(double x, double y, double z,
		      double dx, double dy, double dz,  double th){


   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.7,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,1,0,0);
   glScaled(dx,dy,dz);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[4]);

    float x1              = 0.0;
    float y1              = 0.0;
    float angle          = 0.0;
    float angle_stepsize = 0.1;

    /** Draw the tube */
    glColor3f(1,1,1);
    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
        while( angle < 2*PI ) {
            x1 = 1 * cos(angle);
            y1 = 1 * sin(angle);
            glNormal3f(x1, y1, 0);
            glTexCoord2f(angle,5); glVertex3f(x1, y1 , 1);
            glTexCoord2f(angle,0); glVertex3f(x1, y1 , 0.0);
            angle = angle + angle_stepsize;
        }
        glVertex3f(1, 0.0, 1);
        glVertex3f(1, 0.0, 0.0);
    glEnd();

    /** Draw the circle on top of cylinder */
    glBegin(GL_POLYGON);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = 1 * cos(angle);
            y = 1 * sin(angle);
            glNormal3f(0, 0, 1);
            glVertex3f(x, y , 1);
            angle = angle + angle_stepsize;
        }
        glVertex3f(1, 0.0, 1);
    glEnd();
   //  Undo transformations and textures
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

}

static void halfCyl(double x, double y, double z,
		      double dx, double dy, double dz,  double th){


   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.7,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,1,0,0);
   glScaled(dx,dy,dz);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[3]);

    float x1              = 0.0;
    float y1              = 0.0;
    float angle          = 0.0;
    float angle_stepsize = 0.1;

    /** Draw the tube */
    glColor3f(1,1,1);
    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
        while( angle < PI ) {
            x1 = 1 * cos(angle);
            y1 = 1 * sin(angle);
            glNormal3f(-1*x1, -1*y1, 0);
            glTexCoord2f(angle,10);
            glVertex3f(x1, y1 , 1);
            glTexCoord2f(angle,0);
            glVertex3f(x1, y1 , 0.0);
            angle = angle + angle_stepsize;
        }

    glEnd();

    /** Draw the circle on top of cylinder */
    glBegin(GL_POLYGON);
    angle = 0.0;
      
        while( angle < PI ) {
            x = 1 * cos(angle);
            y = 1 * sin(angle);
            glNormal3f(0, 0, -1);
            glTexCoord2f(angle,1);
            glVertex3f(x, y , 1);
            angle = angle + angle_stepsize;
        }
        glVertex3f(1, 0.0, 1);
    glEnd();
   //  Undo transformations and textures
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

}

static void drawCone(double x, double y, double z,
		      double dx, double dy, double dz,
		      int th, int dtex){
   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.7,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,1,0,0);
   glScaled(dx,dy,dz);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[dtex]);

   glBegin(GL_TRIANGLE_FAN);
         glTexCoord2f(0.5,0.5); glVertex3f(0,0,1);
         for (int k=0;k<=360;k+=10)
         {
            glNormal3f(Sin(k),Cos(k),0);
            glTexCoord2f(1/2*Cos(k)+0.5,1/2*Sin(k)+0.5);
            glVertex2f(Sin(k),Cos(k));
         }
         glEnd();
   
   //  Undo transformations and textures
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

}



static void drawPin(double x, double y, double z,
		      double dx, double dy, double dz){


   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(dx,dy,dz);
   //  Enable textures
   drawCone(0,-.2,0,     .2,.2,.2,   90,    4);
   drawCone(0,0,0,     .2,.2,.2,   -90,    4);
   drawCyl(0,0,0,     .2,.2,.2,       90);

   drawCone(0,.23,0,     .13,.13,.13,   90,    5);
   drawCone(0,.35,0,     .13,.13,.13,   -90,    4);
   drawCyl(0,.35,0,     .13,.13,.13,       90);
   //  Undo transformations and textures
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);


}

static void ballReturn(double x, double y, double z,
		      double dx, double dy, double dz,
		      double th,  int numBall){

               //set these values when i was a larger dummy than i am now
  float front = -5;
   float back = 5;
   float top = 1.5;
   float midY = 1;
   float bottom = .01;
   float left = -2;
   float right = .8;
   float midX = -.6;
   float reachL = -3.4;
   float reachR = 2.2;

  // Set specular color to white
  float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,.8);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[3]);

   glBegin(GL_QUADS);
  //splitter
  glColor3f(1, 1, 1);
  glNormal3f( 0, top, 0);
   glTexCoord2f(0,0); glVertex3f(-.4, top, front);
   glTexCoord2f(.5,0); glVertex3f(-.8, top, front);
   glTexCoord2f(5,5); glVertex3f(-.4, top, back);
   glTexCoord2f(0,5); glVertex3f(-.8, top, back);

   glTexCoord2f(0,0); glVertex3f(reachR+.1, top, front);
   glTexCoord2f(.5,0); glVertex3f(reachR-.1, top, front);
   glTexCoord2f(5,5); glVertex3f(reachR+.1, top, back);
   glTexCoord2f(0,5); glVertex3f(reachR-.1, top, back);

   glTexCoord2f(0,0); glVertex3f(reachL+.1, top, front);
   glTexCoord2f(.5,0); glVertex3f(reachL-.1, top, front);
   glTexCoord2f(5,5); glVertex3f(reachL+.1, top, back);
   glTexCoord2f(0,5); glVertex3f(reachL-.1, top, back);


   
   //mid supports
   glNormal3f( 0, midY, 0);
   glTexCoord2f(0,0); glVertex3f(right+.15, midY, front);
   glTexCoord2f(.5,0); glVertex3f(right, midY, front);
   glTexCoord2f(5,5); glVertex3f(right+.15, midY, back);
   glTexCoord2f(0,5); glVertex3f(right, midY, back);

   glTexCoord2f(0,0); glVertex3f(left-.15, midY, front);
   glTexCoord2f(.5,0); glVertex3f(left, midY, front);
   glTexCoord2f(5,5); glVertex3f(left-.15, midY, back);
   glTexCoord2f(0,5); glVertex3f(left, midY, back);

   //inner return walls
   glNormal3f( left, 0, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(right, bottom, front);
   glTexCoord2f(0.0,1.0); glVertex3f(right, midY, front);
   glTexCoord2f(1.0,1.0); glVertex3f(right, midY, back);
   glTexCoord2f(1.0,0.0); glVertex3f(right, bottom, back);
   

   glNormal3f( right, 0, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(left, bottom, front);
   glTexCoord2f(0.0,1.0); glVertex3f(left, midY, front);
   glTexCoord2f(1.0,1.0); glVertex3f(left, midY, back);
   glTexCoord2f(1.0,0.0); glVertex3f(left, bottom, back);

   //connecting supports

   glNormal3f( right, top, 0);
   glTexCoord2f(0,0); glVertex3f(right, midY, front);
   glTexCoord2f(.5,0); glVertex3f(right, midY, front+.2);
   glTexCoord2f(5,5); glVertex3f(midX, top, front);
   glTexCoord2f(0,5);glVertex3f(midX, top, front +.2);

   
   glTexCoord2f(0,0); glVertex3f(right, midY, back);
   glTexCoord2f(.5,0); glVertex3f(right, midY, back+.2);
   glTexCoord2f(5,5); glVertex3f(midX, top, back);
   glTexCoord2f(0,5);glVertex3f(midX, top, back +.2);

   

   

   glNormal3f( left, top, 0);
   glTexCoord2f(0,0); glVertex3f(right, midY, back);
   glTexCoord2f(.5,0); glVertex3f(right, midY, back+.2);
   glTexCoord2f(5,5); glVertex3f(reachR, top, back);
   glTexCoord2f(0,5); glVertex3f(reachR, top, back +.2);

   glTexCoord2f(0,0); glVertex3f(right, midY, front);
   glTexCoord2f(.5,0); glVertex3f(right, midY, front+.2);
   glTexCoord2f(5,5); glVertex3f(reachR, top, front);
   glTexCoord2f(0,5); glVertex3f(reachR, top, front +.2);

   

   glNormal3f( right, top, 0);
   glTexCoord2f(0,0); glVertex3f(left, midY, front);
   glTexCoord2f(.5,0); glVertex3f(left, midY, front+.2);
   glTexCoord2f(5,5); glVertex3f(reachL, top, front);
   glTexCoord2f(0,5); glVertex3f(reachL, top, front +.2);

   glTexCoord2f(0,0); glVertex3f(left, midY, back);
   glTexCoord2f(.5,0); glVertex3f(left, midY, back+.2);
   glTexCoord2f(5,5); glVertex3f(reachL, top, back);
   glTexCoord2f(0,5); glVertex3f(reachL, top, back +.2);
   


   glNormal3f( left, top, 0);
   glTexCoord2f(0,0); glVertex3f(left, midY, front);
   glTexCoord2f(.5,0); glVertex3f(left, midY, front+.2);
   glTexCoord2f(5,5); glVertex3f(midX, top, front);
   glTexCoord2f(0,5); glVertex3f(midX, top, front +.2);

   glTexCoord2f(0,0); glVertex3f(left, midY, back);
   glTexCoord2f(.5,0); glVertex3f(left, midY, back+.2);
   glTexCoord2f(5,5); glVertex3f(midX, top, back);
   glTexCoord2f(0,5); glVertex3f(midX, top, back +.2);




   glEnd();

   //outer return walls
   glBindTexture(GL_TEXTURE_2D,texture[2]);

   glBegin(GL_QUADS);
   glNormal3f( right+.15, 0, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(right+.15, bottom, front);
   glTexCoord2f(0.0,1.0); glVertex3f(right+.15, midY, front);
   glTexCoord2f(1.0,1.0); glVertex3f(right+.15, midY, back);
   glTexCoord2f(1.0,0.0); glVertex3f(right+.15, bottom, back);

   


   glNormal3f( left-.15, 0, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(left-.15, bottom, front);
   glTexCoord2f(0.0,1.0); glVertex3f(left-.15, midY, front);
   glTexCoord2f(1.0,1.0); glVertex3f(left-.15, midY, back);
   glTexCoord2f(1.0,0.0); glVertex3f(left-.15, bottom, back);

   glEnd();

   glBindTexture(GL_TEXTURE_2D,texture[3]);


   //front
   glBegin(GL_TRIANGLE_FAN);


   glNormal3f( 0, 0, front);
   glTexCoord2f(1.0,0.5); glVertex3f(right,midY , front);
   glTexCoord2f(0.5,1.5); glVertex3f(midX, 1.7, front);
   glTexCoord2f(0.0,0.5); glVertex3f(left, midY, front);


   glTexCoord2f(1.0,0.0); glVertex3f(right, bottom, front);
   glTexCoord2f(0.0,0.0); glVertex3f(left, bottom, front);
   glTexCoord2f(0.0,1.0); glVertex3f(left, midY, front);

   glTexCoord2f(1.0,0.0); glVertex3f(right, bottom, front);
   glTexCoord2f(0.0,1.0); glVertex3f(left, midY, front);
   glTexCoord2f(1.0,1.0); glVertex3f(right, midY, front);

   glEnd();


   //actual ball return part

   glBegin(GL_POLYGON);
   glColor3ub(150, 62, 62);

   glNormal3f( right, 0, 0);
   glVertex3f(right, bottom, back);
   glVertex3f(right, bottom, back+2.5);
   glVertex3f(right, midY, back + 2.5);
   glVertex3f(right, top + 1, back + 2);
   glVertex3f(right, top + 1.5, back + 1.5);
   glVertex3f(right, top + 2, back + 1);
   glVertex3f(right, top + 2.5, back);
   glEnd();

   glBegin(GL_POLYGON);
   glColor3ub(150, 62, 62);

   glNormal3f( left, 0, 0);
   glVertex3f(left, bottom, back);
   glVertex3f(left, bottom, back+2.5);
   glVertex3f(left, midY, back + 2.5);
   glVertex3f(left, top + 1, back + 2);
   glVertex3f(left, top + 1.5, back + 1.5);
   glVertex3f(left, top + 2, back + 1);
   glVertex3f(left, top + 2.5, back);
   glEnd();

   glBegin(GL_QUAD_STRIP);
   glColor3ub(97, 48, 16);
   glNormal3f( 0, 0, back+2.5);
   glVertex3f(left ,bottom , back+2.5 );
   glVertex3f(right ,bottom , back+2.5 );

   glVertex3f(left ,midY ,back + 2.5 );
   glVertex3f(right ,midY ,back + 2.5 );

   glNormal3f( 0, top, back+2);
   glVertex3f(left ,top + 1, back + 2 );
   glVertex3f(right ,top + 1, back + 2 );

   glNormal3f( 0, top+1.5, back+1.5);
   glVertex3f(left ,top + 1.5, back + 1.5 );
   glVertex3f(right ,top + 1.5, back + 1.5 );

   glNormal3f( 0, top+2, back+1);
   glVertex3f(left ,top + 2, back + 1 );
   glVertex3f(right ,top + 2, back + 1 );

   glNormal3f( 0, top+2.5, back);
   glVertex3f(left ,top + 2.5, back );
   glVertex3f(right ,top + 2.5, back );


   glEnd();

   //ball hole
   
   glBegin(GL_QUADS);
   
   glColor3f(0,0,0);
   glNormal3f( 0, bottom+.01, 0 );
   glTexCoord2f(0.0,1.0); glVertex3f(left ,bottom+.01 , back+2.5 );
   glTexCoord2f(0.0,0.0); glVertex3f(left, bottom+.01, back);
   glTexCoord2f(1.0,0.0); glVertex3f(right, bottom+.01, back);
   glTexCoord2f(1.0,1.0); glVertex3f(right ,bottom+.01 , back+2.5 );
   
   glEnd();





   switch(numBall){
      //no breaks on this train
      case 5:
         drawBall(-2, 2.5, -1.6,    1.3,    1  );
      case 4:
         drawBall(+.8,+2.5,.8,    1.3,     0 );
      case 3:
         drawBall(+.8,+2.5,-1.6,    1.3,    1 );
      case 2:
         drawBall(.8,+2.5,-4,    1.3,    0 );
      case 1:
         drawBall(-2,+2.5,-4,   1.3,    1 );

   }


   
   //  Undo transformations and textures
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

}




static void drawLane(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{

   //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,1);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   //  Front
   glColor3f(1,1,1);
   glBegin(GL_QUADS);
   glNormal3f( 0, .01, 0);
   glTexCoord2f(0,0); glVertex3f(-2,0, 10);
   glTexCoord2f(5,0); glVertex3f(+2,0, 10);
   glTexCoord2f(5,5); glVertex3f(+2,0, -10);
   glTexCoord2f(0,5); glVertex3f(-2,0, -10);

  
   glColor3f(.5,.1,.1);
   glTexCoord2f(0,0); glVertex3f(-2.6,0, 10);
   glTexCoord2f(5,0); glVertex3f(-5.3,0, 10);
   glTexCoord2f(5,5); glVertex3f(-5.3,0, -10);
   glTexCoord2f(0,5); glVertex3f(-2.6,0, -10);
   glEnd();
   //the worst/best part of learning new things is just seeing how silly you were at the start.
   //or even yesterday. and today will be looked at just the same
   //4 / 6 = 2/3
   //-2 + n*2/3
   laneArrow(-1.333, .02, 0 ,     .1,1,.1,    0);
   laneArrow(-.666, .02, .5,       .1,1,.1,    0 );
   laneArrow(0, .02, 1,        .1,1,.1,    0);
   laneArrow(.666, .02, .5,        .1,1,.1,    0);
   laneArrow(1.333, .02, 0,        .1,1,.1,    0);

   laneArrow(-1.333, .02, -7 ,     .1,1,.1,    0);
   laneArrow(-.666, .02, -6.5,       .1,1,.1,    0 );
   laneArrow(0, .02, -6,        .1,1,.1,    0);
   laneArrow(.666, .02, -6.5,        .1,1,.1,    0);
   laneArrow(1.333, .02, -7,        .1,1,.1,    0);



   drawPin(-1,.15,9,    .5,.5,.5);
   drawPin(-.333,.15,9,    .5,.5,.5);
   drawPin(.333,.15,9,    .5,.5,.5);
   drawPin(1,.15,9,    .5,.5,.5);

   drawPin(-.666,.15,8.5,    .5,.5,.5);
   drawPin(0,.15,8.5,    .5,.5,.5);
   drawPin(.666,.15,8.5,    .5,.5,.5);

   drawPin(-.333,.15,8,    .5,.5,.5);
   drawPin(.333,.15,8,    .5,.5,.5);

   drawPin(0,.15,7.5,    .5,.5,.5);

   halfCyl(2.3,0,10,  .3,.2,20,    180);
   halfCyl(-2.3,0,10,  .3,.2,20,    180);

   drawCyl(-3.95, 4, 5,    .5,.5,4,     90);
   drawCyl(-3.95, 4, -5,    .5,.5,4,     90);

   
      

   //  Undo transformations and textures
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);


}

static void endWall(){

   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.0,0.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,.1);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate

   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[6]);


   glBegin(GL_QUADS);
   glNormal3f(0,0,-1);
   glTexCoord2f(0,0); glVertex3f(8,-1,26);
   glTexCoord2f(0,5); glVertex3f(8,3,26);
   glTexCoord2f(5,5); glVertex3f(-9,3,26);
   glTexCoord2f(5,0); glVertex3f(-9,-1,26);
   



   

   glEnd();


   //  Undo transformations and textures
   glPopMatrix();
   glDisable(GL_TEXTURE_2D); 



}

static void endWall2(){

   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.0,0.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,.1);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate

   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[3]);


   glBegin(GL_QUADS);
   glNormal3f(0,0,-1);
   glTexCoord2f(0,0); glVertex3f(8,-1,-10);
   glTexCoord2f(0,5); glVertex3f(8,3,-10);
   glTexCoord2f(5,5); glVertex3f(-9,3,-10);
   glTexCoord2f(5,0); glVertex3f(-9,-1,-10);
   



   

   glEnd();


   //  Undo transformations and textures
   glPopMatrix();
   glDisable(GL_TEXTURE_2D); 



}

static void sideWalls(){

   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.0,0.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,.1);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate

   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[4]);


   glBegin(GL_QUADS);
   glNormal3f(1,0,0);
   glTexCoord2f(0,0); glVertex3f(-9,-1,26);
   glTexCoord2f(0,2); glVertex3f(-9,3,26);
   glTexCoord2f(2,2); glVertex3f(-9,3,-10);
   glTexCoord2f(2,0); glVertex3f(-9,-1,-10);
   
   glNormal3f(-1,0,0);
   glTexCoord2f(0,0); glVertex3f(8,-1,26);
   glTexCoord2f(0,2); glVertex3f(8,3,26);
   glTexCoord2f(2,2); glVertex3f(8,3,-10);
   glTexCoord2f(2,0); glVertex3f(8,-1,-10);



   

   glEnd();


   //  Undo transformations and textures
   glPopMatrix();
   glDisable(GL_TEXTURE_2D); 



} 

static void roof(){

   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.0,0.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,.1);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate

   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[4]);


   glBegin(GL_QUADS);
   glNormal3f(0,-1,0);
   glTexCoord2f(0,0); glVertex3f(-10,3, 28);
   glTexCoord2f(10,0); glVertex3f(+10,3, 28);
   glTexCoord2f(10,10); glVertex3f(+10,3, -10);
   glTexCoord2f(0,10); glVertex3f(-10,3, -10);



   

   glEnd();


   //  Undo transformations and textures
   glPopMatrix();
   glDisable(GL_TEXTURE_2D); 



}

/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void drawFloor(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,1,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   //  Front
   glColor3f(1,1,1);
   glBegin(GL_QUADS);
   glNormal3f( 0, 1, 0);
   glTexCoord2f(0,0); glVertex3f(-10,-1, 5);
   glTexCoord2f(50,0); glVertex3f(+10,-1, 5);
   glTexCoord2f(50,50); glVertex3f(+10,-1, -10);
   glTexCoord2f(0,50); glVertex3f(-10,-1, -10);
   glEnd();

   ballReturn(0,-1,0 , 0.15,0.15,0.15    ,0,    5);

   ballReturn(-3,-1,0 , 0.15,0.15,0.15    ,0,    4);

   ballReturn(3,-1,0 , 0.15,0.15,0.15    ,0,    3);

   ballReturn(6,-1,0 , 0.15,0.15,0.15    ,0,    2);

   ballReturn(-6,-1,0 , 0.15,0.15,0.15    ,0,    1);

   drawLane(2,-1,15 , .5,1,1    ,0);
   drawLane(-2,-1,15 , .5,1,1    ,0);
   drawLane(6,-1,15 , .5,1,1    ,0);
   drawLane(-6,-1,15 , .5,1,1    ,0);

   endWall();
   sideWalls();
   endWall2();
   roof();


   drawBall(xVal, -.5, zVal,   .5,1);

   

   //  Undo transformations and textures
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius r
 */
static void ball(double x,double y,double z,double r)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glutSolidSphere(1.0,16,16);
   //  Undo transofrmations
   glPopMatrix();
}





/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   //  Eye position

   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Set perspective
   glLoadIdentity();


   if (fp) {
      Cx = +2*dim*Sin(rot); //Ajust the camera vector based on rot
      Cz = -2*dim*Cos(rot);

      gluLookAt(Ex,Ey,Ez, Cx+Ex,Ey,Cz+Ez, 0,1,0); //  Use gluLookAt, y is the up-axis

   }
   else {
      //  Perspective - set eye position
      if (mode)
      {
         double px = -2*dim*Sin(th)*Cos(ph);
         double py = +2*dim        *Sin(ph);
         double pz = +2*dim*Cos(th)*Cos(ph);
         gluLookAt(px,py,pz , 0,0,0 , 0,Cos(ph),0);
      }
      //  Orthogonal - set world orientation
      else
      {
         glRotatef(ph,1,0,0);
         glRotatef(th,0,1,0);
      }
   }
   

   //  Light switch
   if (light)
   {
      //  Translate intensity to color vectors
      float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
      //  Light direction
      float Position[]  = {5*Cos(zh),ylight,5*Sin(zh),1};
      //  Draw light position as ball (still no lighting here)
      glColor3f(1,1,1);
      ball(Position[0],Position[1],Position[2] , 0.1);
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
      glDisable(GL_LIGHTING);
   //  Draw scene
   drawFloor(0,0,0 , 0.5,0.5,0.5 , 0);

   
   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
   if (axes)
   {
      const double len=2.0; //  Length of axes
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   //  Display parameters
   glWindowPos2i(5,5);
   

   if(throwing){
   Print("Throw not ready. Retrieve throw: ENTER!");
   }
   else{
      Print("Throw ready: SPACEBAR");
   }




   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //if(!fp) {
      //  Right arrow key - increase angle by 5 degrees
      if (key == GLUT_KEY_RIGHT)
         th += 5;
      //  Left arrow key - decrease angle by 5 degrees
      else if (key == GLUT_KEY_LEFT)
         th -= 5;
      //  Up arrow key - increase elevation by 5 degrees
      else if (key == GLUT_KEY_UP)
         ph += 5;
      //  Down arrow key - decrease elevation by 5 degrees
      else if (key == GLUT_KEY_DOWN)
         ph -= 5;
      //  PageUp key - increase dim
      else if (key == GLUT_KEY_PAGE_UP && dim>1)
         dim -= 0.1;
      //  PageDown key - decrease dim
      else if (key == GLUT_KEY_PAGE_DOWN)
         dim += 0.1;
      //  Keep angles to +/-360 degrees
      th %= 360;
      ph %= 360;
   //}

   ErrCheck("special project");
   //  Update projection
   Project(fov,asp,dim,fp,mode);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Toggle axes
   else if (ch == '9')
      axes = 1-axes;
   //  Toggle first person
   else if (ch == 'f' || ch == 'F')
   {
      fp = 1-fp;
   }

   if (fp) {
      double dt = 0.05;
      if (ch == 'w' || ch == 'W'){
         Ex += Cx*dt; //Update the eye vector based on the camera vector
         Ez += Cz*dt;
      }
      else if (ch == 'a' || ch == 'A'){
         rot -= 3;
      }
      else if (ch == 's' || ch == 'S'){
         Ex -= Cx*dt;
         Ez -= Cz*dt;
      }
      else if (ch == 'd' || ch == 'D'){
         rot += 3;
      }

      else if(ch ==32){
         throwing = 1;
      }
      else if(ch == 13){
         throwing = 0;

      }

      else if (ch == '1')
         fov--;
      else if (ch == '2')
         fov++;

      

      //  Keep angles to +/-360 degrees
      rot %= 360;
   }


   if (ch == 'y' || ch == 'Y')
      lightmode = 1-lightmode;
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Toggle textures mode
   else if (ch == 't')
      ntex = 1-ntex;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Ambient level
   else if (ch=='b' && ambient>0)
      ambient -= 5;
   else if (ch=='B' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='i' && diffuse>0)
      diffuse -= 5;
   else if (ch=='I' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='p' && specular>0)
      specular -= 5;
   else if (ch=='P' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;

   //  Translate shininess power to value (-1 => 0)
   shiny = shininess<0 ? 0 : pow(2.0,shininess);


   ErrCheck("key project");


   //  Reproject
   Project(fov,asp,dim,fp,mode);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, RES*width,RES*height);
   //  Set projection
   Project(fov,asp,dim,fp,mode);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("final: Isaiah Thomas");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
#endif
   //  Set callbacks
   glutDisplayFunc(display);
   glutTimerFunc(100, timer, 0);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Load textures
   texture[0] = LoadTexBMP("bowlball.bmp");
   texture[1] = LoadTexBMP("bowlball2.bmp");
   texture[2] = LoadTexBMP("lane.bmp");
   texture[3] = LoadTexBMP("plate.bmp");
   texture[4] = LoadTexBMP("pinbody.bmp");
   texture[5] = LoadTexBMP("redpart.bmp");
   texture[6] = LoadTexBMP("backwall.bmp");

   
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
