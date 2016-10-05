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

/*
void addCar(vehicule * v, vehicule * carList){

}

void move_cars(vehicule * carList){

}
*/

int main(int argc, char* argv[]){
    printf("Bienvenue sur traffic racer\n");
    //création de la matrice de la route large de 3 et haute de 30
    vehicule** road = alloc_road(HAUTEUR_ROUTE, NB_VOIE_DEFAULT);
    //vehicule* carList = malloc(NB_MAX_CAR*sizeof(vehicule));
    //int nb_cars = 0;
    //vehicule player = {1, 30, 'v', RED, "🚘"};
    vehicule player;
    player.posx = 1;
    player.posy = 30;
    player.type = 'v';
    player.couleur = RED;
    player.custom = "🚘";
    road[HAUTEUR_ROUTE-1][NB_VOIE_DEFAULT/2] = player;
    print_game(road);
    int pos_player = NB_VOIE_DEFAULT/2;

    int b = 1;
    while(b){
        char c = key_pressed();
        //move_cars(carList, nb_cars);
        //vehicule v = generateCar();
        //addCar(&v, carList);
        //clean_cursor();
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
