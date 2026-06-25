int initGLUT(int argc, char **argv)
{
    // GLUT stuff for windowing
    // initialization openGL window.
    // it is called before any other GLUT routine
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);   // display mode

    glutInitWindowSize(screenWidth, screenHeight);  // window size

    glutInitWindowPosition(100, 100);               // window location

    // finally, create a window with openGL context
    // Window will not displayed until glutMainLoop() is called
    // it returns a unique ID
    handle = glutCreateWindow( WINDOWTITLE );
    MainWindow=handle;
//  int handle = glutCreateWindow( WINDOWTITLE );
//  int handle = glutCreateWindow(argv[0]);     // param is the title of window

    // register GLUT callback functions
    glutDisplayFunc(displayCB);
    glutTimerFunc(33, timerCB, 33);             // redraw only every given millisec
    glutReshapeFunc(reshapeCB);
    glutKeyboardFunc(keyboardCB);

    glutMouseFunc(mouseCB);
//  glutMouseFunc(MouseButton);

//  glutMotionFunc(MouseMotion);
    glutMotionFunc(mouseMotionCB);

    return handle;
}
