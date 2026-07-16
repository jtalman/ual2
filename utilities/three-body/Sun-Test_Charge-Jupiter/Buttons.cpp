void
Buttons( int id )
{
//fprintf(stderr, "enter void Buttons( int id ) %d\n", id);
        switch( id )
        {
                case AXES:
//fprintf(stderr, "AXES!!!!\n");
//AxesOn=!AxesOn;
displayCB();

/*
	if( AxesOn ){
		fprintf( stderr, "Axes\n" );
		glColor3fv( AXES_COLOR );
		glLineWidth( AXES_WIDTH );
			Axes( 1.2 );
		glLineWidth( 1. );
        }
*/
//                      Reset();
                        Glui->sync_live();

                        glutSetWindow( handle );
//                      glutSetWindow( MainWindow );

                        glutPostRedisplay();
                        break;

                case PROJ:
//fprintf(stderr, "PROJ!!!!\n");
//                      Reset();
displayCB();
                        Glui->sync_live();

                        glutSetWindow( handle );
//                      glutSetWindow( MainWindow );

                        glutPostRedisplay();
                        break;

                case DEPTH:
//fprintf(stderr, "DEPTH!!!!\n");
//DepthCueOn=!DepthCueOn;
//                      Reset();
displayCB();
                        Glui->sync_live();

                        glutSetWindow( handle );
//                      glutSetWindow( MainWindow );

                        glutPostRedisplay();
                        break;

                case SUN_POS:
//fprintf(stderr, "SUN_POS!!!!\n");
displayCB();
                        Glui->sync_live();

                        glutSetWindow( handle );
//                      glutSetWindow( MainWindow );

                        glutPostRedisplay();
                        break;

                case MOON_POS:
//fprintf(stderr, "MOON_POS!!!!\n");
displayCB();
                        Glui->sync_live();

                        glutSetWindow( handle );
//                      glutSetWindow( MainWindow );

                        glutPostRedisplay();
                        break;

                case EARTH_POS:
//fprintf(stderr, "EARTH_POS!!!!\n");
displayCB();
                        Glui->sync_live();

                        glutSetWindow( handle );
//                      glutSetWindow( MainWindow );

                        glutPostRedisplay();
                        break;

                case DBUG:
//fprintf(stderr, "DBUG!!!!\n");
//                      Reset();
                        Glui->sync_live();

                        glutSetWindow( handle );
//                      glutSetWindow( MainWindow );

                        glutPostRedisplay();
                        break;

                case RESET:
//fprintf(stderr, "RESET!!!!\n");
                        Reset();
displayCB();
                        Glui->sync_live();

                        glutSetWindow( handle );
//                      glutSetWindow( MainWindow );

                        glutPostRedisplay();
                        break;

                case QUIT:
//fprintf(stderr, "QUIT!!!!\n");
                        // gracefully close the glui window: 
                        // gracefully close out the graphics:
                        // gracefully close the graphics window: 
                        // gracefully exit the program:

                        Glui->close();
                        glutSetWindow( handle );
                        glFinish();
                        glutDestroyWindow( handle );
                        exit( 0 );
                        break;

//              default:
//                      fprintf( stderr, "Don't know what to do with Button ID %d\n", id );
        }

//fprintf(stderr, "leave void Buttons( int id ) %d\n", id);
}
