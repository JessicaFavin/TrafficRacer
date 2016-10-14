#include "vehicule.h"
#include "couleur.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

char choixType(){
    int type = (rand()%2);
    switch (type){
        case 0:
            return 'v';
            break;
        case 1:
            return 'c';
            break;
        default:
            return 'v';
    }
}

char * choixCustom(char type, char * couleur){
    char * camion = "🚍  ";
    char * voiture = "🚘  ";
    char * res = malloc(100*sizeof(char));
    if(type=='c'){
        strcat(res, couleur);
        strcat(res, camion);
        strcat(res, DEFAULT);
        return res;
    }
    strcat(res, couleur);
    strcat(res, voiture);
    strcat(res, DEFAULT);
    return res;
}

vehicule generVehicule(int nbVoie){
    int posX=(rand()%nbVoie);
    vehicule v;
    v.posy = 0;
    v.posx = posX;
    v.type = choixType();
    v.couleur = choixCouleur();
    v.custom = choixCustom(v.type ,v.couleur);
    v.etat = 1; //(inutile ?)
    v.vitesse = 90;
    return v;
}
