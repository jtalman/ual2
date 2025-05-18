#include"ezxml.h"
void main(int argc,char*argv[]){
 ezxml_t f1 = ezxml_parse_file("elements.xml"), element, mtype;
 char *name;
 char *type_attr;
 char *l_attr;
 char *a_attr;
 char *b_attr;
 ezxml_t mfield = ezxml_child(element, "mfield");
 
 for (element = ezxml_child(f1, "element"); element; element = element->next) {
  type_attr = ezxml_attr(element, "type");
  name = ezxml_attr(element, "name");
  printf("%s ", type_attr);
  l_attr = ezxml_attr(element, "l");
//printf("%s = {.tag=\"%s\",.length=%s\n", name,name,l_attr);
//printf("%s ", l_attr);
  mfield = ezxml_child(element,"mfield");
  a_attr = ezxml_attr(mfield, "a");
  b_attr = ezxml_attr(mfield, "b");
//printf("%s ", a_attr);
//printf("%s \n", b_attr);
  printf("%s = {.tag=\"%s\",.length=%s,.strength=%s,.%s_track=&%s_track};\n", name,name,l_attr,b_attr,type_attr,type_attr);
 }
 ezxml_free(f1); 
}
