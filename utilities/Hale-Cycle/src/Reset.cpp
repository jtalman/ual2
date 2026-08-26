void Reset(void){
 fprintf(stderr, "enter void Reset(void)\n");

 PX=5.5;//1;
 PY=0;//1;
 PZ=0;

/*
 Pr = +sqrt(two);
 Pt = +atan(PY/PX);
 PR   = +sqrt(three);
*/

 Pr=sqrt(PX*PX+PY*PY);
 Pt = atan(PY/PX);
 PR = sqrt(PX*PX+PY*PY+PZ*PZ);

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

 year=2026;

 S=0;
 SClk = +0;

 zoom=0.8;
 WhichGrid=1;

 if(WhichGrid==0){
    spinner_PX->disable();
    spinner_PY->disable();
    spinner_PZ->disable();
    spinner_Pr->disable();
    spinner_Pt->disable();
    spinner_PR->disable();
 }

 if(WhichGrid==1){
    spinner_PX->enable();
    spinner_PY->enable();
    spinner_PZ->enable();
    spinner_Pr->disable();
    spinner_Pt->disable();
    spinner_PR->disable();
 }

 if(WhichGrid==2){
    spinner_PX->disable();
    spinner_PY->disable();
    spinner_PZ->enable();
    spinner_Pr->enable();
    spinner_Pt->enable();
    spinner_PR->disable();
 }

 resetCameraPTU(CPX, CPY, CPZ, CTX, CTY, CTZ, CUX, CUY, CUZ);
 Glui->sync_live();
 glutSetWindow( handle );
 glutPostRedisplay();
 fprintf(stderr, "leave void Reset(void)\n");
}
