#include <stdio.h>
#include <stdlib.h>
#include "keys.h"
#include "vehicule.h"
#include "couleur.h"


const char* VOITURE_PLAYER  = "\e[31m🚘\e[39m";
const char* VOITURE  = "\e[93m🚖\e[39m";
const char* COLLISION = "\e[31m💥\e[39m";
const char* PALMIER = " 🌴  ";
const int HAUTEUR_ROUTE = 30;
const int HAUTEUR_MENU = 9;
const int LARGEUR_ROUTE = 8;
const int NB_VOIE_DEFAULT = 3;
const char * TAB = "        ";
const int NB_MAX_CAR = 50;

void print_player(int pos){
    if(pos%3==0)
        printf("        ║\e[100m   %s   \e[93m|\e[39m        \e[93m|\e[39m       \e[49m║\n", VOITURE_PLAYER);
    if(pos%3==1)
        printf("        ║\e[100m       \e[93m|\e[39m   %s    \e[93m|\e[39m       \e[49m║\n", VOITURE_PLAYER);
    if(pos%3==2)
        printf("        ║\e[100m       \e[93m|\e[39m        \e[93m|\e[39m    %s  \e[49m║\n", VOITURE_PLAYER);

}

/*
void print_game(int pos){
    printf("╔══════════════════════╗\n");
    printf("║SPEED (KM/S) ▶     200║\n");
    printf("╠══════════════════════╣\n");
    printf("║SCORE        ▶    1560║\n");
    printf("╠══════════════════════╣\n");
    printf("║BEST SCORE   ▶   10840║\n");
    printf("╚══════════════════════╝\n");
    int i;
    for(i=0; i<HAUTEUR_ROUTE-1; i++){
        if(i%6==0)
            printf("    %s\e[39m║\e[100m       \e[93m|\e[39m        \e[93m|\e[39m       \e[39m\e[49m║%s\n", PALMIER, PALMIER);
        else
            printf("        \e[39m║\e[100m       \e[93m|\e[39m        \e[93m|\e[39m       \e[39m\e[49m║\n");
    }
    print_player(pos);
    printf("\n");
}
*/

void print_game(vehicule** road){
    printf("╔══════════════════════╗\n");
    printf("║SPEED (KM/S) ▶     200║\n");
    printf("╠══════════════════════╣\n");
    printf("║SCORE        ▶    1560║\n");
    printf("╠══════════════════════╣\n");
    printf("║BEST SCORE   ▶   10840║\n");
    printf("╚══════════════════════╝\n");
    int i, j;
    vehicule v;
    for(i=0; i<HAUTEUR_ROUTE; i++){
        printf("%s", TAB);
        printf("║%s", BACKROAD);
        for(j=0; j<NB_VOIE_DEFAULT; j++){
            if((road[i][j].couleur==NULL)){
                printf("       ");
            } else {
                v = road[i][j];
                printf("%s   %s   %s",v.couleur,v.custom, DEFAULT);
            }
            if(!(j==NB_VOIE_DEFAULT-1)){
                printf("%s║%s", BACKDEFAULT, BACKROAD);
            }
        }
        printf("%s║\n", BACKDEFAULT);
    }
}

void move_player(int old_pos, int new_pos, vehicule** road, vehicule * player){
    printf("\033[%d;%dH\e[100m  \e[49m",
    HAUTEUR_ROUTE+HAUTEUR_MENU+1, (old_pos*LARGEUR_ROUTE)+8+5);
    printf("\033[%d;%dH\e[100m%s \e[49m",
    HAUTEUR_ROUTE+HAUTEUR_MENU+1, (new_pos*LARGEUR_ROUTE)+8+5, VOITURE_PLAYER);
    road[HAUTEUR_ROUTE-1][old_pos].couleur = NULL;
    player->posx = new_pos;
    road[HAUTEUR_ROUTE-1][new_pos] = (*player);
}

void clean_cursor(){
    printf("\033[%d;1H ",HAUTEUR_ROUTE+HAUTEUR_MENU+2);
}

void reposition_cursor(){
    printf("\033[%d;1H",HAUTEUR_ROUTE+HAUTEUR_MENU+2);
}

vehicule** alloc_road(int nb_l, int nb_c){
    vehicule** road = (vehicule**)malloc(nb_l*sizeof(vehicule*));
    int i;
    for(i=0; i<nb_l; i++){
        road[i] = (vehicule*)malloc(nb_c*sizeof(vehicule));
    }
    return road;
}


void addCar(vehicule * v, vehicule * carList, int nbCars){
    //ajoute la voiture donnée en argument à la liste des voitures
    carList[nbCars] = (*v);
}

void removeCar(vehicule * carList, int nbCars){
    /*
    retire la voiture donnée en argument à la liste des voitures
    je suis pas completement sûre que cette fonction marche
    je vérifierais plus tard ou j'ajouterais une libraire
    pour avoir une vraie liste
    */
    int i;
    int decale = 0;
    for(i=0; i<nbCars-1; i++){
        if(carList[i].etat==0){
            decale+=1;
        }
        if(decale){
            carList[i]=carList[i+decale];
        }
    }

}

void draw_car(vehicule * v){
    /*
        dessine une voiture sur la route
        utilise les attributs de la voiture v
        pour les coordonnées regarde la fonction move_player
        (et ajuste si ça fait de la merde)
    */

}

void clean_car(vehicule * v){
    /*
        efface la voiture donnée de la route
        pareil que pour draw_car sauf qu'au lieu de custom tu ecris un espace
        pour effecer le caractère qui est déjà sur la position
    */

}


void move_cars(vehicule * carList, int nbCars){
    /*
        bouge les voitures de la carList du "haut" vers les "bas"
        utilise draw_car et clean_car que tu auras définies avant
        en changeant les coordonnées de la voiture entre les deux
        ah et fait gaffe à arreter d'incrémenter les position de la voiture
        une fois qu'elle a atteint la limite de la route
        je sais pas encore comment les retirer de la liste du coup
        change l'état à 0
        et du coup avant de bouger les voitures fait un if(voiture.etat==1)
        pour être sûr qu'elle est bien sur la route
    */

}


int main(int argc, char* argv[]){
    printf("Bienvenue sur traffic racer\n");
    //création de la matrice de la route large de 3 et haute de 30
    //qui ne sert probablement à rien au final xD
    vehicule** road = alloc_road(HAUTEUR_ROUTE, NB_VOIE_DEFAULT);
    vehicule* carList = malloc(NB_MAX_CAR*sizeof(vehicule));
    int nb_cars = 0;
    vehicule player;
    player.posx = 1;
    player.posy = 30;
    player.type = 'v';
    player.couleur = RED;
    player.custom = "🚘";
    road[HAUTEUR_ROUTE-1][NB_VOIE_DEFAULT/2] = player;
    print_game(road);
    int pos_player = NB_VOIE_DEFAULT/2;
    /*
        ------------ A DECOMMENTER POUR TESTER --------------
        créé 2 voitures tests et les dessine sur la route
        j'ai l'impression que generVehicule ne change pas le rand
        a chaque appel il faudra qu'on revoit ça
    */
    /*
    vehicule v = generVehicule(NB_VOIE_DEFAULT);
    v.posx = 0;
    v.couleur = BLUE;
    addCar(&v, carList, nb_cars);
    nb_cars +=1;
    draw_car(&v);
    vehicule v2 = generVehicule(NB_VOIE_DEFAULT);
    v2.posx = 2;
    v2.posy = 12;
    v2.couleur = GREEN;
    addCar(&v2, carList, nb_cars);
    nb_cars += 1;
    draw_car(&v2);
    */
    int b = 1;
    while(b){
        char c = key_pressed();
        /*
            ---------------- A DECOMMENTER POUR TESTER -----------------
            attention la boucle while risque de faire des trucs bizarres
            genre executer ta fonction à l'infini
        */

        //move_cars(carList, nb_cars);
        if(c=='d' && pos_player<NB_VOIE_DEFAULT-1){
            pos_player += 1;
            move_player(pos_player-1, pos_player, road, &player);
        }
        if(c=='q' && pos_player>0){
            pos_player-=1;
            move_player(pos_player+1, pos_player, road, &player);
        }
        if(c=='z'){
            b = 0;
        }
        reposition_cursor();
    }

    return 0;
}
