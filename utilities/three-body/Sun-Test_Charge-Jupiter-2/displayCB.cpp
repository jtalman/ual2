void displayCB(){

#include"display-current.h"

//while(WhichObject==2){
  if(WhichObject==2){
//fprintf(stderr, "enter displayCB(): WhichObject %d\n", WhichObject);
//fprintf(stderr, "initial location\n");
//C=0;
 float p[2];
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT               );
//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GLUT_STENCIL);
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);

 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
  glOrtho(LEFT_ORTHO , RIGHT_ORTHO, BOTTOM_ORTHO, TOP_ORTHO, -1, 1);
//glOrtho(LEFT_ORTHO , RIGHT_ORTHO, -10, 10, -1, 1);
//glOrtho(-1 , 10, -10, 10, -1, 1);
//glOrtho(-5 , 5 , -5 , 5 , -1, 1);
//glOrtho(-50, 50, -50, 50, -1, 1);

 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();

 glColor3f ( 1.0, 1.0, 0.0 );
 glPointSize(5);
 glBegin(GL_POINTS);
   p[0] = (float) (SX[0] / 1e8 + SUN_X0);
   p[1] = (float) (SY[0] / 1e8 + SUN_Y0);
   glVertex2fv ( p ); 
 glEnd(); 

 glColor3f ( 1.0, 1.0, 1.0 );
 glPointSize(5);
 glBegin(GL_POINTS);
   p[0] = (float)TM_X[0] / 1e11;
   p[1] = (float)TM_Y[0] / 1e11;
   glVertex2fv ( p ); 
 glEnd(); 

 glColor3f ( 1.0, 0.0, 0.0 );
 glPointSize(5);
 glBegin(GL_POINTS);
   p[0] = (float) (JX[0] / 1e11 + JUPITER_X0);
   p[1] = (float) (JY[0] / 1e11 + JUPITER_Y0);
   glVertex2fv ( p ); 
 glEnd(); 

 glEnable(GL_LIGHTING);
 glEnable(GL_TEXTURE_2D);

glFlush();
//glutSwapBuffers();
//glutMainLoop();
glutPostRedisplay();
return;
}

if(WhichObject==1){
//while(WhichObject==1){
//fprintf(stderr, "enter displayCB(): WhichObject %d\n", WhichObject);
 float p[2];
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT               );
 glColor3f ( 1.0, 1.0, 0.0 );
//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GLUT_STENCIL);
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);

 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
//glOrtho(-50, 50, -50, 50, -1, 1);
//glOrtho(-1 , 10, -10, 10, -1, 1);
//glOrtho(LEFT_ORTHO , RIGHT_ORTHO, -10, 10, -1, 1);
  glOrtho(LEFT_ORTHO , RIGHT_ORTHO, BOTTOM_ORTHO, TOP_ORTHO, -1, 1);

 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();

 Cpoint[C].x = (float)(SX[C]/1e8);
 Cpoint[C].y = (float)(SY[C]/1e8);

 Cpoint_Sun[C].x = (float)(SX[C]/1e9 + SUN_X0);
 Cpoint_Sun[C].y = (float)(SY[C]/1e9 + SUN_Y0);

/*
 Cpoint_TM[C].x = (float)(TM_X[C]/1e11);
 Cpoint_TM[C].y = (float)(TM_Y[C]/1e11);
*/

 Cpoint_TM[C].x = (float)(TM_X[C]/5e10);
 Cpoint_TM[C].y = (float)(TM_Y[C]/5e10);

 Cpoint_Jupiter[C].x = (float) (JX[C]/1e11 + JUPITER_X0);
 Cpoint_Jupiter[C].y = (float) (JY[C]/1e11 + JUPITER_Y0);

/*
 glEnableClientState(GL_VERTEX_ARRAY);
 glVertexPointer(2, GL_FLOAT, sizeof(Point), Cpoint.data());
 glDrawArrays(GL_POINTS, 0, Cpoint.size());
 glDisableClientState(GL_VERTEX_ARRAY);
*/

/*
 glEnableClientState(GL_VERTEX_ARRAY);
 glVertexPointer(2, GL_FLOAT, sizeof(Point), Cpoint_Sun.data());
 glDrawArrays(GL_POINTS, 0, Cpoint_Sun.size());
 glDisableClientState(GL_VERTEX_ARRAY);
*/

 glColor3f ( 1.0, 1.0, 0.0 );
 glPointSize(5);
 glBegin(GL_POINTS);
// p[0] = (float)Cpoint_Sun[C].x / 1e8;
// p[1] = (float)Cpoint_Sun[C].y / 1e8;
   p[0] = (float)SX[C] / 1e10;
   p[1] = (float)SY[C] / 1e10;
   glVertex2fv ( p ); 
 glEnd(); 

 glColor3f ( 1.0, 1.0, 1.0 );
 glPointSize(5);
 glBegin(GL_POINTS);
   p[0] = (float)TM_X[C] / 1e11;
   p[1] = (float)TM_Y[C] / 1e11;
   glVertex2fv ( p ); 
 glEnd(); 

 glColor3f ( 1.0, 0.0, 0.0 );
 glPointSize(5);
 glBegin(GL_POINTS);
   p[0] = (float)JX[C] / 1e11;
   p[1] = (float)JY[C] / 1e11;
   glVertex2fv ( p ); 
 glEnd(); 

/*
 glPointSize(15);
 glColor3f ( 1.0, 1.0, 0.0 );
 glEnableClientState(GL_VERTEX_ARRAY);
 glVertexPointer(2, GL_FLOAT, sizeof(Point), Cpoint_Sun.data());
 glDrawArrays(GL_POINTS, 0, Cpoint_Sun.size());
 glDisableClientState(GL_VERTEX_ARRAY);

 glPointSize(5);
 glColor3f ( 1.0, 1.0, 1.0 );
 glEnableClientState(GL_VERTEX_ARRAY);
 glVertexPointer(2, GL_FLOAT, sizeof(Point), Cpoint_TM.data());
 glDrawArrays(GL_POINTS, 0, Cpoint_TM.size());
 glDisableClientState(GL_VERTEX_ARRAY);

 glPointSize(10);
 glColor3f ( 1.0, 0.0, 0.0 );
 glEnableClientState(GL_VERTEX_ARRAY);
 glVertexPointer(2, GL_FLOAT, sizeof(Point), Cpoint_Jupiter.data());
 glDrawArrays(GL_POINTS, 0, Cpoint_Jupiter.size());
 glDisableClientState(GL_VERTEX_ARRAY);
*/

 glEnable(GL_LIGHTING);
 glEnable(GL_TEXTURE_2D);

// glFlush();

 Cpoint[C].x = 0;
 Cpoint[C].y = 0;

 Cpoint_Sun[C].x = 0;
 Cpoint_Sun[C].y = 0;

 Cpoint_TM[C].x = 0;
 Cpoint_TM[C].y = 0;

 Cpoint_Jupiter[C].x = 0;
 Cpoint_Jupiter[C].y = 0;

 C++;
// C=C+100;

 C=C%STEPS;

glFlush();
//glutSwapBuffers();
//glutMainLoop();
glutPostRedisplay();

return;
}

#include"display_S_TM_J.h"

}
