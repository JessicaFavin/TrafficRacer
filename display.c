#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "vehicule.h"
#include "couleur.h"
#include "list_car.h"

const int HAUTEUR_ROUTE = 35;
const int HAUTEUR_MENU = 8;
const int LARGEUR_MENU = 24;
const int LARGEUR_ROUTE = 8;
const int NB_VOIE_DEFAULT = 3;
const char * TAB = "                                            ";
//+2 for the cactuses
const int TAB_SIZE = 44+5+2;
const int NB_MAX_CAR = 50;
const int VIT_MAX_IA = 150;
const int VIT_MIN_IA = 50;


void clean_cursor(){
    printf("\033[%d;1H \n",HAUTEUR_ROUTE+2);
}

void reposition_cursor(){
    printf("\033[%d;1H",HAUTEUR_ROUTE+2);
}

int get_size_int(int entier){
    if(entier==0){
        return 1;
    }
    return floor(log10(abs(entier))) + 1;
}

void print_road(int best){
    int i, j;
    for(i=0; i<HAUTEUR_ROUTE; i++){
        printf("%s", TAB);
        if(i%3==0){
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
        if(i%3==1){
            printf("🌵 \n");
        } else {
            printf("  \n");
        }

    }
    if(best>9999999){
        best = 9999999;
    }
    int size_best = get_size_int(best);
    printf("\033[1;1H");
    printf("╔══════════════════════╗\n");
    printf("║SPEED (KM/S) ▶ 100    ║\n");
    printf("╠══════════════════════╣\n");
    printf("║SCORE        ▶ 0      ║\n");
    printf("╠══════════════════════╣\n");
    printf("║BEST SCORE   ▶ %d",best);
    i = 0;
    for(i=0; i<7-size_best; i++){
        printf(" ");
    }
    printf("║\n");
    printf("╚══════════════════════╝\n");
}

void update_score(int score, int size_score){
    printf("\033[4;17H");
    printf("%d",score);
    int i;
    for(i=0; i<7-size_score; i++){
        printf(" ");
    }
    printf("║");
}

void update_best_score(int best, int size_best){
    printf("\033[6;17H");
    printf("%d",best);
    int i;
    for(i=0; i<7-size_best; i++){
        printf(" ");
    }
    printf("║");
}

void update_vitesse(vehicule * player){
    printf("\033[2;17H");
    printf("%d",player->vitesse);
    int i;
    int size_vitesse = get_size_int(player->vitesse);
    for(i=0; i<7-size_vitesse; i++){
        printf(" ");
    }
}

void update_panel(vehicule * player, int score, int size_score, int best_score){
    update_score(score, size_score);
    if(score>best_score){
        clean_cursor();
        update_best_score(score, size_score);
    }
    int i;
    for(i = 0; i<7-size_score; i++){
        printf(" ");
    }
}

void draw_car(vehicule * v){
    /*
        dessine une voiture sur la route
    */
    int new_pos=v->posy;
    printf("\033[%d;%dH\e[100m%s \e[49m",new_pos,((v->posx*LARGEUR_ROUTE)+TAB_SIZE),v->custom);// ATTENTION!!!! AJUSTER LES POSITION!
    //v->posy=new_pos;

}

void clean_car(vehicule * v){
    /*
        efface la voiture donnée de la route
    */
    int old_pos= v->posy;
    printf("\033[%d;%dH\e[100m  \e[49m",old_pos,((v->posx*LARGEUR_ROUTE)+TAB_SIZE)); // ATTENTION!!!! AJUSTER LES POSITION!

}

void move_player(int old_pos, int new_pos, vehicule * player){
    clean_car(player);
    player->posx = new_pos;
    draw_car(player);
}

int move_cars(vehicule * carList, int nbCars, vehicule * player, vehicule *** road_pointer){
    int i=0;
    vehicule ** road = *road_pointer;
    int car_removed = 0;
    vehicule g;
    vehicule * ghost=&g;
    ghost->ghost=1;
    ghost->posx = 1;
    ghost->posy = 0;
    ghost->type = 'v';
    ghost->couleur = BLUE;
    ghost->custom = "🚘";
    ghost->vitesse = 0; // vehicule phantom obligatoire pour le move_IA)
    for (i=0;i<nbCars;i++){
    	vehicule * v = &carList[i];
        if ((v->posy)>=0 && (v->posy)<=HAUTEUR_ROUTE){ // supprime la voiture que si elle est visible
            clean_car(v);
        }
        /*
          compare la vitesse à celle du player
          et bouge la voiture en fonction
        */
        if(player->vitesse>v->vitesse){
          //on "descend" la voiture
            road[v->posx][(v->posy)] = *ghost;   
            v->posy=v->posy+1;
            if((v->posy)>=0 && (v->posy)<=HAUTEUR_ROUTE){                 // ATTENTION ERREUR SEGMENTATION SI IL Y A PAS CETTE CONDITION
                road[v->posx][v->posy] = *v;
            }
        }else if (player->vitesse<v->vitesse) {
          //on "monte" la voiture
          road[v->posx][(v->posy)] = *ghost;
          v->posy=v->posy-1;
          if((v->posy)>=0 && (v->posy)<=HAUTEUR_ROUTE){                // ATTENTION ERREUR SEGMENTATION SI IL Y A PAS CETTE CONDITION
            road[v->posx][v->posy] = *v;
            }
        }
        if((v->posy)>HAUTEUR_ROUTE){
            v->etat=0;
            removeCar(carList, nbCars);
            car_removed += 1;
        } else if((v->posy)>0 && (v->posy)<=HAUTEUR_ROUTE){
            draw_car(v);

        }
    }
    return car_removed;

}

void move_IA(vehicule * IA, vehicule *** road_pointer){          // ATTENTION ERREUR SEGMENTATION + move_player (ne fonctionne pas)
    int scan_height=1;

    vehicule ** road = *road_pointer;
    vehicule ghost;
    ghost.posx = 1;
    ghost.posy = 0;
    ghost.type = 'v';
    ghost.couleur = BLUE;
    ghost.custom = "🚘";
    ghost.vitesse = 0;
    ghost.ghost=1;


    if(road[IA->posx][(IA->posy)-scan_height].ghost != ghost.ghost){
        IA->vitesse=VIT_MAX_IA;    
    }
    else{
        IA->vitesse=road[IA->posx][(IA->posy)-scan_height].vitesse;
    }
    /*if(road[IA->posx][(IA->posy)-(scan_height+1)].ghost != ghost.ghost) {
        if(IA->posx==1){
            IA->posx=(IA->posx)+1;
            //move_player((IA->posx)-1,IA->posx,IA);
        }
        if(IA->posx==2){
            if(road[(IA->posx)+1][(IA->posy)-(scan_height+1)].ghost == ghost.ghost){
                IA->posx=(IA->posx)+1;
                //move_player((IA->posx)-1,IA->posx,IA);
            }
            else if(road[(IA->posx)-1][(IA->posy)-(scan_height+1)].ghost == ghost.ghost){
                IA->posx=(IA->posx)-1;
                //move_player((IA->posx)+1,IA->posx,IA);
            }

        }
        if(IA->posx==3){
            IA->posx=(IA->posx)-1;
            //move_player((IA->posx)+1,IA->posx,IA);



        }
    }*/
    
}
