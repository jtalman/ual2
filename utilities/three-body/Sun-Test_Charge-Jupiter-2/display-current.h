  if(WhichObject==3){
//fprintf(stderr, "enter displayCB(): WhichObject %d\n", WhichObject);
//fprintf(stderr, "current location\n");
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
   p[0] = (float) (SX[C] / 1e10 + SUN_X0);
   p[1] = (float) (SY[C] / 1e10 + SUN_Y0);
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
   p[0] = (float) (JX[C] / 1e11 + JUPITER_X0);
   p[1] = (float) (JY[C] / 1e11 + JUPITER_Y0);
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
