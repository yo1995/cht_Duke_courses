#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "outname.h"

char * computeOutputFileName(const char * inputName) {
  //WRITE ME
  //.counts = 7 \0 =1 sum = 8
	char * outFileName = malloc((strlen(inputName) + 8) * sizeof(*outFileName));
	stpcpy(outFileName, inputName);
	strcat(outFileName, ".counts");
	return outFileName;
}
