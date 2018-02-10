#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverse(char * str) {
  //WRITE ME!
  int count = 0;
  char temp;
  if(str==NULL){
	return;
  }
  while ( str[count]!='\0'){
  	count++;
  }
  char * end = str+count-1;
  //int len = strlen(str); could use strlen function
  while(str < end){
	temp = *str;     
    *str = *end;     
    *end = temp; 
    end--;
    str++;
  }
}

int main(void) {
  char str0[] = "";
  char str1[] = "123";
  char str2[] = "abcd";
  char str3[] = "Captain's log, Stardate 42523.7";
  char str4[] = "Hello, my name is Inigo Montoya.";
  char str5[] = "You can be my wingman anyday!";
  char str6[] = "Executor Selendis! Unleash the full power of your forces! There may be no tomorrow!";
  char * array[] = {str0, str1, str2, str3, str4, str5, str6};
  int i = 0;
  for (i = 0; i < 7; i++) {
    reverse(array[i]);
    printf("%s\n", array[i]);
  }
  return EXIT_SUCCESS;
}
