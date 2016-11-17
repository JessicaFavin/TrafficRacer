#include "sound.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void playSound(char * name ){ // Lance le son en paramétre
	char * res= malloc(100*sizeof(char));
	char * play= "play -q ";
	char * esperluette="&";

	strcat(res,play);
	strcat(res,name);
	strcat(res,esperluette);

	system(res);
}

void killSound(char * name){ // Ferme le processus sox d'un sons en paramétre
	char * res = malloc(100*sizeof(char));
	char * comPart1="./kill.sh";
	

	strcat(res,comPart1);
	strcat(res,name);

	system(res);
}
