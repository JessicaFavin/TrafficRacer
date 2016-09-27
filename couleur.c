#include "couleur.h"


char *choixCouleur(){
    srand();
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
