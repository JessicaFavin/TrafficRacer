
typedef struct vehicule vehicule;
struct vehicule{
    int posx;
    int posy;
    //char alignement;
    char type;
    const char * couleur;
    char * custom;
    //char etat;

};

char choixType();
char * choixCustom(char type, char * couleur);
vehicule generVehicule(NB_VOIE_DEFAULT);
