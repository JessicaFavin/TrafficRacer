#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sound.h"



void playSound(const char * name ){ // Lance le son en paramétre
	char * res= malloc(200*sizeof(char));
	char * play= "play -q -v 0.99 ";
	char * esperluette=" &";

	strcat(res,play);
	strcat(res,name);
	strcat(res,esperluette);

	system(res);
}

void killSound(const char * name){ // Ferme le processus sox d'un sons en paramétre
	char * res = malloc(200*sizeof(char));
	char * comPart1="./killSound.sh ";

	strcat(res,comPart1);
	strcat(res,name);

	system(res);
}
