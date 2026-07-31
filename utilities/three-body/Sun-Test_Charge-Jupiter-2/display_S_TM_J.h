if(WhichObject==0){
    // clear buffer
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);   // display mode
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	GLsizei vx, vy, v;		// viewport dimensions
	GLint xl, yb;		// lower-left corner of viewport
	GLfloat scale2;		// real glui scale factor

    // save the initial ModelView matrix before modifying ModelView matrix
    glPushMatrix();

    // tramsform modelview matrix
    glTranslatef(0, 0, -cameraDistance);

    // set material
    float ambient[]  = {0.5f, 0.5f, 0.5f, 1};
    float diffuse[]  = {0.7f, 0.7f, 0.7f, 1};
    float specular[] = {1.0f, 1.0f, 1.0f, 1};
    float shininess  = 128;
    glMaterialfv(GL_FRONT, GL_AMBIENT,   ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

	// set the viewport to a square centered in the window:

	vx = glutGet( GLUT_WINDOW_WIDTH );
	vy = glutGet( GLUT_WINDOW_HEIGHT );
//	vx = glutGet( SCREEN_WIDTH );
//	vy = glutGet( SCREEN_HEIGHT );
//	vx = SCREEN_WIDTH;
//	vy = SCREEN_HEIGHT;
//	vx = (GLsizei) SCREEN_WIDTH;
//	vy = (GLsizei) SCREEN_HEIGHT;

/*
	v = vx < vy ? vx : vy;			// minimum dimension
	xl = ( vx - v ) / 2;
	yb = ( vy - v ) / 2;
	glViewport( xl, yb,  v, v );
*/

	glTranslatef( (GLfloat)TransXYZ[0], (GLfloat)TransXYZ[1], -(GLfloat)TransXYZ[2] );

	// uniformly scale the scene:

	glScalef( (GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale );
	scale2 = 1. + Scale2;		// because glui translation starts at 0.
	if( scale2 < MINSCALE )
		scale2 = MINSCALE;
	glScalef( (GLfloat)scale2, (GLfloat)scale2, (GLfloat)scale2 );

/*
	if( AxesOn ){
		fprintf( stderr, "Axes\n" );
		glColor3fv( AXES_COLOR );
		glLineWidth( AXES_WIDTH );
			Axes( 1.2 );
		glLineWidth( 1. );
        }
*/

    // line color
    float lineColor[] = {0.2f, 0.2f, 0.2f, 1};

    // draw left flat sphere with lines
    glPushMatrix();

    glTranslatef(S_x0, S_y0, S_z0);

    glScalef(1.5, 1.5, 1.5);

    glRotatef(cameraAngleX, 1, 0, 0);   // pitch
    glRotatef(cameraAngleY, 0, 1, 0);   // heading
    glBindTexture(GL_TEXTURE_2D, texId_Sun);

    sphere1.draw();
//  sphere1.draw(lineColor);
//  sphere1.drawWithLines(lineColor);

    //sphere1.drawLines(lineColor);
    glPopMatrix();

    // draw centre smooth sphere with line
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse); // reset diffuse
    glPushMatrix();

//  glTranslatef(0.0f, 0, 0);
    glTranslatef(M_x0, M_y0, M_z0);

    glScalef(0.3, 0.3, 0.3);

    glRotatef(cameraAngleX, 1, 0, 0);
    glRotatef(cameraAngleY, 0, 1, 0);
    glBindTexture(GL_TEXTURE_2D, texId_TC);
//  glBindTexture(GL_TEXTURE_2D, texId_Moon);
	if( AxesOn ){
//		fprintf( stderr, "Axes\n" );
		glColor3fv( AXES_COLOR );
		glLineWidth( AXES_WIDTH );
			Axes( 1.2 );
		glLineWidth( 1. );
        }

    sphere2.draw();
//  sphere2.drawWithLines(lineColor);

    glPopMatrix();

    // draw right sphere with texture
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse); // reset diffuse
    glPushMatrix();

//  glTranslatef(2.5f, 0, 0);
    glTranslatef(E_x0, E_y0, E_z0);

    glRotatef(cameraAngleX, 1, 0, 0);
    glRotatef(cameraAngleY, 0, 1, 0);
    glBindTexture(GL_TEXTURE_2D, texId_Jupiter);        //    texId_Earth;
    sphere2.draw();
    glPopMatrix();

    /*
    // using GLU quadric object
    GLUquadricObj* obj = gluNewQuadric();
    gluQuadricDrawStyle(obj, GLU_FILL); // GLU_FILL, GLU_LINE, GLU_SILHOUETTE, GLU_POINT
    gluQuadricNormals(obj, GL_SMOOTH);
    gluQuadricTexture(obj, GL_TRUE);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    gluSphere(obj, 2.0, 50, 50); // radius, slice, stack
    */

    glBindTexture(GL_TEXTURE_2D, 0);

//  showInfo();     // print max range of glDrawRangeElements

//  glPushMatrix();
//  glMatrixMode(GL_MODELVIEW);      // switch to modelview matrix
/*
	if( AxesOn ){
		fprintf( stderr, "Axes\n" );
		glColor3fv( AXES_COLOR );
		glLineWidth( AXES_WIDTH );
			Axes( 1.2 );
		glLineWidth( 1. );
        }
*/

    glPopMatrix();

if( WhichProjection ){
 toPerspective();
}else{
 toOrtho();
}

        if( DepthCueOn )
        {
                glEnable( GL_FOG );
                glFogi( GL_FOG_MODE, FOGMODE );
                glFogfv( GL_FOG_COLOR, FOGCOLOR );
                glFogf( GL_FOG_DENSITY, FOGDENSITY );
                glFogf( GL_FOG_START, FOGSTART );
                glFogf( GL_FOG_END, FOGEND );
                glEnable( GL_FOG );
        }
        else
        {
                glDisable( GL_FOG );
        }
//  glPopMatrix();

 glFlush();
    glutSwapBuffers();
}
