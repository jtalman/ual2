void Grid(void){
 fprintf(stderr, "enter void Grid(void)\n");

/*
 PX=1;
 PY=1;
 PZ=1;

 PT=0;
 PR=1;

 CPX=5;//0;
 CPY=4;//0;
 CPZ=8;//CAMERA_DISTANCE;

 CTX=0;
 CTY=0;
 CTZ=0;

 CUX=0;
 CUY=1;
 CUZ=0;

 HClk=0;
 S=0;

 zoom=0.8;
//WhichGrid=1;
*/

 if(WhichGrid==0){
    spinner_PX->disable();
    spinner_PY->disable();
    spinner_PZ->disable();
    spinner_PT->disable();
    spinner_PR->disable();
 }

 if(WhichGrid==1){
    spinner_PX->enable();
    spinner_PY->enable();
    spinner_PZ->enable();
    spinner_PT->disable();
    spinner_PR->disable();
 }

 if(WhichGrid==2){
    spinner_PX->disable();
    spinner_PY->disable();
    spinner_PZ->enable();
    spinner_PT->enable();
    spinner_PR->enable();
 }

// resetCameraPTU(CPX, CPY, CPZ, CTX, CTY, CTZ, CUX, CUY, CUZ);
 Glui->sync_live();
 glutSetWindow( handle );
 glutPostRedisplay();
 fprintf(stderr, "leave void Grid(void)\n");
}
