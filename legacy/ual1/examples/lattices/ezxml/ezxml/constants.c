#include"ezxml.h"
void main(int argc,char*argv[]){
 ezxml_t f1 = ezxml_parse_file("constants.xml"), constant;
 char *name;
 char *value_attr;
 
 for (constant = ezxml_child(f1, "constant"); constant; constant = constant->next) {
  name = ezxml_attr(constant, "name");
  value_attr = ezxml_attr(constant, "value");
  printf("%s %s\n", name,value_attr);
 }
 ezxml_free(f1); 
}
