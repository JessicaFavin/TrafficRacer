#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "vehicule.h"
#include "couleur.h"

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

int choixVitesse(){
    //retourne une vitesse comprise entre 50 et 120 km/h
    int vitesse = ((rand()%8)+5)*10;
    return vitesse;
}

vehicule generVehicule(int nbVoie){
    int posX=(rand()%nbVoie);
    vehicule v;
    v.posy = 0;
    v.posx = posX;
    v.type = choixType();
    v.couleur = choixCouleur();
    v.custom = choixCustom(v.type ,v.couleur);
    v.etat = 1;
    v.vitesse = choixVitesse();
    v.ghost = 0;
    return v;
}

vehicule generGhost(){
    vehicule v;
    v.posy = -1;
    v.posx = -1;
    v.type = choixType();
    v.couleur = choixCouleur();
    v.custom = choixCustom(v.type ,v.couleur);
    v.etat = -1;
    v.vitesse = -1;
    v.ghost = 1;
    return v;
}
