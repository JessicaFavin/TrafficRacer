
typedef struct vehicule vehicule;
struct vehicule{
    int posx;
    int posy;
    //char alignement;
    char type;
    char * couleur;
    char * custom;
    int etat;
    int vitesse;
};

char choixType();
char * choixCustom(char type, char * couleur);
vehicule generVehicule(int nbVoie);
