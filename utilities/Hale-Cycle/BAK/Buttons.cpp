void Buttons(int id){
 fprintf(stderr, "enter void Buttons( int id ) %d\n", id);
 switch(id){
  case 0:
   fprintf(stderr, "PX: 0\n");
   PR = sqrt(PX*PX+PY*PY+PZ*PZ);
   Pr=sqrt(PX*PX+PY*PY);
   Pt = atan(PY/PX);
  break;

  case 1:
   fprintf(stderr, "PY: 1\n");
   PR=sqrt(PX*PX+PY*PY+PZ*PZ);
   Pr=sqrt(PX*PX+PY*PY);
   Pt = atan(PY/PX);
  break;

  case 2:
   fprintf(stderr, "PZ: 2\n");
   PR=sqrt(PX*PX+PY*PY+PZ*PZ);
   Pr=sqrt(PX*PX+PY*PY);
   Pt = atan(PY/PX);
  break;

  case 3:
   fprintf(stderr, "Pr: 3\n");
   PX=Pr*cos(Pt);
   PY=Pr*sin(Pt);
   PR = sqrt(PX*PX+PY*PY+PZ*PZ);
  break;

  case 4:
   fprintf(stderr, "Pt: 4\n");
   PX=Pr*cos(Pt);
   PY=Pr*sin(Pt);
   PR = sqrt(PX*PX+PY*PY+PZ*PZ);
  break;

  case 5:
   fprintf(stderr, "5: RESET\n");
   Reset();
  break;

  case 6:
   fprintf(stderr, "6: HOVER\n");
  break;

  case 7:
   fprintf(stderr, "7: CLICK\n");
  break;

  case 8:
   fprintf(stderr, "8: Step\n");
  break;

  case 9:
   fprintf(stderr, "9: Grid\n");
 if(WhichGrid==0){
   fprintf(stderr, "9: Grid:: NO GRID!!\n");
 }
 if(WhichGrid==1){
   fprintf(stderr, "9: Grid:: RECTANGULAR!!\n");
 }
 if(WhichGrid==2){
   fprintf(stderr, "9: Grid:: CYLINDRICAL!!\n");
 }
   Grid();
  break;

  case 101:
   fprintf(stderr, "101: PR is read only\n");
  break;

  default:
   fprintf( stderr, "Don't know what to do with Button ID %d\n", id );
 }

 Glui->sync_live();
 glutSetWindow( handle );
 glutPostRedisplay();
 fprintf(stderr, "leave void Buttons( int id ) %d\n", id);
}
