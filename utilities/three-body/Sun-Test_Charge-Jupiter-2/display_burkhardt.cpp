void display_burkhardt(){
while(WhichObject==1){
 step++;
// display_burkhardt();

//fprintf(stderr, "enter void display_burkhardt(void)\n");

  float p[2];
  float XX[2] = {0};
  int point;
//
//  Clear the window.
//
//glClear ( GL_COLOR_BUFFER_BIT );
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT               );
//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GLUT_STENCIL);
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);

 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(-50, 50, -50, 50, -1, 1);

 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
//
//  Draw the generator points in BLUE.
//  Note that OpenGL is using FLOAT's for real numbers, while we prefer DOUBLE's.
//
  glColor3f ( 0.0, 0.0, 1.0 );
  glPointSize(5);
  glBegin (GL_POINTS);
    p[0] = ( float ) JX[step] / 1e11;
    p[1] = ( float ) JY[step] / 1e11;
    glVertex2fv ( p ); 
  glEnd ( ); 

//for ( point = 0; point < point_num; point++ )
//for ( int i = 0; i < point_num; i++ )
//for (int i = 0; i < point_num; i++){
//for (int i = 0; i < step; i++){
//  glBegin (GL_POINTS);
/*
      p[0] = ( float ) xy[0+point*2];
      p[1] = ( float ) xy[1+point*2];
*/
/*
      p[0] = ( float ) JX[0+point*2] / 1e11;
      p[1] = ( float ) JY[1+point*2] / 1e11;
*/
/*
      p[0] = ( float ) SX[i] / 1e8;
      p[1] = ( float ) SY[i] / 1e8;
*/
/*
      p[0] = ( float ) TM_X[i] / 1e10;
      p[1] = ( float ) TM_Y[i] / 1e10;
*/
/*
      p[0] = ( float ) JX[i] / 1e11;
      p[1] = ( float ) JY[i] / 1e11;
*/

//    glVertex2fv ( p ); 

// glEnd ( ); 
//}

  glColor3f ( 1.0, 0.0, 0.0 );
  glPointSize(10);
  glBegin (GL_POINTS);
  glVertex2fv ( XX ); 
  glEnd ( ); 


//
//  Clear all the buffers.
//
  glFlush ( ); 
  glutSwapBuffers();

//glutMainLoopEvent(); // handle the main loop once
  glutMainLoop(); // handle the main loop once

//fprintf(stderr, "leave void display_burkhardt(void)\n");
//return;
}
}
