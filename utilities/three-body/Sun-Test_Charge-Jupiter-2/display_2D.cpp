void display_2D(void){
 fprintf(stderr, "enter void display_2D(void)\n");
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(-50, 50, -50, 50, -1, 1);

 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();

 // draw
 glColor3ub( 255, 255, 255 );
 glEnableClientState( GL_VERTEX_ARRAY );
// glEnableClientState( GL_COLOR_ARRAY );
 glVertexPointer( 2, GL_FLOAT, sizeof(Point), &points[0].x );
// glColorPointer( 4, GL_UNSIGNED_BYTE, sizeof(Point), &points[0].r );
 glPointSize( 3.0 );
 glDrawArrays( GL_POINTS, 0, points.size() );
 glDisableClientState( GL_VERTEX_ARRAY );
// glDisableClientState( GL_COLOR_ARRAY );

 glFlush();
 glutSwapBuffers();
 fprintf(stderr, "leave void display_2D(void)\n");
}
