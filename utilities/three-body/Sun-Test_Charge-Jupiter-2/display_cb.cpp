void
display_cb(int id){
fprintf(stderr, "enter void display_cb( int id ) %d\n", id);
fprintf(stderr, "RADIOBUTTON_ID %d\n", RADIOBUTTON_ID);
fprintf(stderr, "WhichObject %d\n", WhichObject);
        switch( WhichObject )
        {
                case SPHERE:
//                      Reset();
                        Glui->sync_live();

                        glutSetWindow( handle );
//                      glutSetWindow( MainWindow );

//                      glutDisplayFunc(displayCB);
                        displayCB();
                        glutPostRedisplay();
                        break;

                case ICONIC:
                        Glui->sync_live();

                        glutSetWindow( handle );
//                      glutDisplayFunc(display_2D);
//                      display_2D();
                        glutPostRedisplay();
                        break;

                case STEP:
                        Glui->sync_live();

                        glutSetWindow( handle );
//                      glutDisplayFunc(display_2D);
//                      display_2D();
                        glutPostRedisplay();
                        break;

//              default:
//                      fprintf( stderr, "Don't know what to do with Button ID %d\n", id );
        }

fprintf(stderr, "leave void display_cb( int id ) %d\n", id);
}
