#include "vehicule.h"
#include "couleur.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

char choixType(){
    srand(time(NULL));
    int type=(rand()%2);
    switch (type){
        case 0:
            return 'v';break;
        case 1:
            return 'c';break;
        default:
            return 'v';
    }
}

char * choixCustom(char type, char * couleur){
    if(type=='c'){
        return strcat(strcat(couleur,"🚍  ") ,DEFAULT);
    }
    return strcat(strcat(couleur,"🚘  " ),DEFAULT);
}

vehicule generVehicule(NB_VOIE_DEFAULT){
    srand(time(NULL));
    int posX=(rand()%NB_VOIE_DEFAULT);
    vehicule v;
    v.posy=0;
    v.posx=posX;
    v.type= choixType();
    v.couleur= choixCouleur();
    v.custom= choixCustom(v.type ,v.couleur);
    // v.etat=1; (inutile ?)

    return v;
}
