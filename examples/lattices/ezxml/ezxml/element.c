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
  name = ezxml_attr(element, "name");
  if( !strcmp( argv[1], name)){
   printf("HIT\n");
   printf("%s:\n", name);
   type_attr = ezxml_attr(element, "type");
   printf("type %s:\n", type_attr);
   l_attr = ezxml_attr(element, "l");
   printf("l %s:\n", l_attr);
   mfield = ezxml_child(element,"mfield");
   a_attr = ezxml_attr(mfield, "a");
   b_attr = ezxml_attr(mfield, "b");
   printf("a %s:\n", a_attr);
   printf("b %s:\n", b_attr);
  }
//element_child = ezxml_child(element, "type");
 }
 ezxml_free(f1); 
}
