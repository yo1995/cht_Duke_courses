#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _string_pair_t {
char * str1;
char * str2;
};
typedef struct _string_pair_t string_pair_t;

int main(void) {
char * line = NULL;
size_t sz;
string_pair_t ** array = NULL;
int count = 0;
while (getline(&line, &sz, stdin) >= 0) {
	array = realloc(array, (count+1) * sizeof(*array));
	array[count] = malloc(sizeof(array[count]));
	array[count]->str1 = strdup(line);
	char * p = strchr(array[count]->str1, '=');
	if (p != NULL) {
		*p = '\0';
		array[count]->str2 = strdup(strchr(line, '=') + 1);
	}
	else {
		array[count]->str2 = NULL;
	}
	count++;
}
for (int i = 0; i < count; i++) {
printf("%s = %s\n", array[i]->str1, array[i]->str2);
free(array[i]->str1);
free(array[i]->str2);
}
free(array);
return EXIT_SUCCESS;
}