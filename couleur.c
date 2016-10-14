#include "couleur.h"
#include <stdlib.h>
#include <time.h>

char * DEFAULT="\e[39m";
char * BLUE="\e[34m";
char * GREEN="\e[32m";
char * RED = "\e[31m";
char * CYAN="\e[96m";
char * BACKROAD="\e[100m";
char * BACKDEFAULT="\e[49m";

char *choixCouleur(){
    int color=(rand()%4);
    switch(color){
        case 0:
            return BLUE;break;
        case 1:
            return CYAN;break;
        case 2:
            return DEFAULT;break;
        case 3:
            return GREEN;break;

        default:
            return BLUE;break;
    }
}
