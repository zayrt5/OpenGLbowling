/*
 *  Set projection
 */
#include "CSCIx229.h"

void Project(double fov,double asp,double dim, int fpm, int mode)
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if(fpm) {
      gluPerspective(fov,asp,dim/4,4*dim);
   }
   else {
      if (mode)
         gluPerspective(fov,asp,dim/4,4*dim);
      //  Orthogonal projection
      else
         glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   } 

   ErrCheck("in Project function");
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

