#include"ezxml.h"
void main(int argc,char*argv[]){
 ezxml_t f1 = ezxml_parse_file("sectors.xml"), sector, frame;
 char *sectorname;
 char *childname;
 
 for (sector = ezxml_child(f1, "sector"); sector; sector = sector->next) {
  sectorname = ezxml_attr(sector, "name");
  if( !strcmp( argv[1], sectorname)){
   printf("HIT\n");
//printf("%s:\n", sectorname);
   for (frame = ezxml_child(sector, "frame");frame;frame = frame->next){
    childname = ezxml_attr(frame, "ref");
    printf("%s:\n", childname);
   }
  }
// printf("\n\n\n", childname);
 }
 ezxml_free(f1); 
}
