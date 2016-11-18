#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "keys.h"
#include "vehicule.h"
#include "couleur.h"
#include "sound.h"


const char* VOITURE_PLAYER  = "\e[31m🚘\e[39m";
const char* VOITURE  = "\e[93m🚖\e[39m";
const char* COLLISION = "\e[31m💥\e[39m";
const char* PALMIER = " 🌴  ";
const int HAUTEUR_ROUTE = 35;
const int HAUTEUR_MENU = 8;
const int LARGEUR_MENU = 24;
const int LARGEUR_ROUTE = 8;
const int NB_VOIE_DEFAULT = 3;
const char * TAB = "                                            ";
//+2 for the cactuses
const int TAB_SIZE = 44+5+2;
const int NB_MAX_CAR = 50;

void print_player(int pos){
    if(pos%3==0)
        printf("        ║\e[100m   %s   \e[93m|\e[39m        \e[93m|\e[39m       \e[49m║\n", VOITURE_PLAYER);
    if(pos%3==1)
        printf("        ║\e[100m       \e[93m|\e[39m   %s    \e[93m|\e[39m       \e[49m║\n", VOITURE_PLAYER);
    if(pos%3==2)
        printf("        ║\e[100m       \e[93m|\e[39m        \e[93m|\e[39m    %s  \e[49m║\n", VOITURE_PLAYER);

}

void print_game(vehicule** road){
    int i, j;
    for(i=0; i<HAUTEUR_ROUTE; i++){
        printf("%s", TAB);
        if(i%2==0){
            printf("🌵 ");
        } else {
            printf("  ");
        }
        printf("║%s", BACKROAD);
        for(j=0; j<NB_VOIE_DEFAULT; j++){
            printf("       ");
            if(!(j==NB_VOIE_DEFAULT-1) && i%2==0){
                printf("|");
            } else {
                printf(" ");
            }
        }
        printf("%s║", BACKDEFAULT);
        if(i%2==1){
            printf("🌵 \n");
        } else {
            printf("  \n");
        }

    }
    printf("\033[1;1H");
    printf("╔══════════════════════╗\n");
    printf("║SPEED (KM/S) ▶     200║\n");
    printf("╠══════════════════════╣\n");
    printf("║SCORE        ▶    1560║\n");
    printf("╠══════════════════════╣\n");
    printf("║BEST SCORE   ▶   10840║\n");
    printf("╚══════════════════════╝\n");
}

void move_player(int old_pos, int new_pos, vehicule * player){
    printf("\033[%d;%dH\e[100m   \e[49m",
    player->posy, (old_pos*LARGEUR_ROUTE)+TAB_SIZE);
    printf("\033[%d;%dH\e[100m%s  \e[49m",
    player->posy, (new_pos*LARGEUR_ROUTE)+TAB_SIZE, VOITURE_PLAYER);
    player->posx = new_pos;
}

void clean_cursor(){
    printf("\033[%d;1H \n",HAUTEUR_ROUTE+2);
}

void reposition_cursor(){
    printf("\033[%d;1H",HAUTEUR_ROUTE+2);
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
    if(nbCars<50){
        carList[nbCars] = (*v);
    }
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
    int new_pos=v->posy;
    printf("\033[%d;%dH\e[100m%s \e[49m",new_pos,((v->posx*LARGEUR_ROUTE)+TAB_SIZE),v->custom);// ATTENTION!!!! AJUSTER LES POSITION!
    //v->posy=new_pos;
}

void clean_car(vehicule * v){
    /*
        efface la voiture donnée de la route
        pareil que pour draw_car sauf qu'au lieu de custom tu ecris un espace
        pour effecer le caractère qui est déjà sur la position
    */
    int old_pos= v->posy;
    printf("\033[%d;%dH\e[100m  \e[49m",old_pos,((v->posx*LARGEUR_ROUTE)+TAB_SIZE)); // ATTENTION!!!! AJUSTER LES POSITION!

}


int move_cars(vehicule * carList, int nbCars){
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
    int i=0;
    int car_removed = 0;
    for (i=0;i<nbCars;i++){
    	vehicule * v= &carList[i];
        clean_car(v);
        v->posy=v->posy+1;
        if((v->posy)==HAUTEUR_ROUTE+1){
            removeCar(carList, nbCars);
            car_removed += 1;
        } else {
            draw_car(v);
        }
    }
    return car_removed;

}


int main(int argc, char* argv[]){
    system("clear");
    //printf("Bienvenue sur traffic racer\n");
    //création de la matrice de la route large de 3 et haute de 30
    //qui ne sert probablement à rien au final xD
    srand(time(NULL));
    vehicule** road = alloc_road(HAUTEUR_ROUTE, NB_VOIE_DEFAULT);
    vehicule* carList = malloc(NB_MAX_CAR*sizeof(vehicule));
    int nb_cars = 0;
    vehicule player;
    player.posx = 1;
    player.posy = HAUTEUR_ROUTE;
    player.type = 'v';
    player.couleur = RED;
    player.custom = "🚘";
    player.vitesse = 90;
    //road[HAUTEUR_ROUTE-1][NB_VOIE_DEFAULT/2] = player;
    print_game(road);
    move_player(0, player.posx, &player);
    int pos_player = NB_VOIE_DEFAULT/2;
    /*
        ------------ A DECOMMENTER POUR TESTER --------------
        créé 2 voitures tests et les dessine sur la route
        j'ai l'impression que generVehicule ne change pas le rand
        a chaque appel il faudra qu'on revoit ça
    */

    vehicule v = generVehicule(NB_VOIE_DEFAULT);
    addCar(&v, carList, nb_cars);
    nb_cars += 1;
    draw_car(&v);
    vehicule v2 = generVehicule(NB_VOIE_DEFAULT);
    v2.posy = 2;
    addCar(&v2, carList, nb_cars);
    nb_cars += 1;
    draw_car(&v2);
    vehicule v3 = generVehicule(NB_VOIE_DEFAULT);
    v3.posy = 7;
    addCar(&v3, carList, nb_cars);
    nb_cars += 1;
    draw_car(&v3);
    vehicule v4 = generVehicule(NB_VOIE_DEFAULT);
    v4.posy = 9;
    addCar(&v4, carList, nb_cars);
    nb_cars += 1;
    draw_car(&v4);
    int b = 1;
    while(b){
        char c = key_pressed();
        /*
            ---------------- A DECOMMENTER POUR TESTER -----------------
            attention la boucle while risque de faire des trucs bizarres
            genre executer ta fonction à l'infini
        */
        if(c=='m'){
            int car_removed = move_cars(carList, nb_cars);
            nb_cars -= car_removed;
        }
        if(c=='d' && pos_player<NB_VOIE_DEFAULT-1){
            pos_player += 1;
            move_player(pos_player-1, pos_player, &player);
        }
        if(c=='q' && pos_player>0){
            pos_player-=1;
            move_player(pos_player+1, pos_player, &player);
        }
        if(c=='z'){
            b = 0;
        }
        clean_cursor();
    }

    return 0;
}
