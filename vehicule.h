#ifndef VEHICULE_H
#define VEHICULE_H

typedef struct vehicule vehicule;
struct vehicule{
    int posx;
    int posy;
    char type;
    char * couleur;
    char * custom;
    int etat;
    int vitesse;
    int ghost;
};

char choixType();
char * choixCustom(char type, char * couleur);
int choixVitesse();
vehicule generVehicule(int nbVoie);
vehicule generGhost();
vehicule generPlayer();

#endif
