void
Reset( void )
{
 fprintf(stderr, "enter void Reset( void )\n");
 ActiveButton = 0;
 AxesOn = true;
 Debug = true;
 DepthCueOn = false;
 LeftButton = ROTATE;
 Scale  = 1.0;
 Scale2 = 0.0;		// because we add 1. to it in Display()
 WhichColor = RED;
 WhichObject = SPHERE;
 WhichProjection = PERSP;
 Xrot = Yrot = 0.;
 TransXYZ[0] = TransXYZ[1] = TransXYZ[2] = 0.;

                   RotMatrix[0][1] = RotMatrix[0][2] = RotMatrix[0][3] = 0.;
 RotMatrix[1][0]                   = RotMatrix[1][2] = RotMatrix[1][3] = 0.;
 RotMatrix[2][0] = RotMatrix[2][1]                   = RotMatrix[2][3] = 0.;
 RotMatrix[3][0] = RotMatrix[3][1] = RotMatrix[3][3]                   = 0.;
 RotMatrix[0][0] = RotMatrix[1][1] = RotMatrix[2][2] = RotMatrix[3][3] = 1.;

 mouseX=0, mouseY=0;
 cameraAngleX=0;
 cameraAngleY=0;

 S_x0=-2.5;
 S_y0=+0.0;
 S_z0=+0.0;

 M_x0=+0.0;
 M_y0=+0.0;
 M_z0=+0.0;

 E_x0=+2.5;
 E_y0=+0.0;
 E_z0=+0.0;

 Glui->sync_live();
 glutSetWindow( handle );
//glutPostRedisplay();
//reshapeCB(SCREEN_WIDTH,SCREEN_HEIGHT);
 displayCB();
 fprintf(stderr, "leave void Reset( void )\n");
}
