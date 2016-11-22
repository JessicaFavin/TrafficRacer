#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include "game.h"
#include "display.h"

const char * crash="./Sound/crash.mp3";
const char * klaxon="./Sound/klaxon.mp3";
const char * acceleration="./Sound/acceleration.mp3";
const char * cow="./Sound/cow.mp3";
const char * music=" ";
const char * end=" ";
const char * decompte="./Sound/decompte.mp3";
const char * gameOver=" ";
const int VIT_MAX_PLAYER = 150;
const int VIT_MIN_PLAYER = 50;

char key_pressed() {
    struct termios oldterm, newterm;
    int oldfd;
    char c, result = 0;
    tcgetattr(STDIN_FILENO, &oldterm);
    newterm = oldterm;
    newterm.c_lflag &= ~ (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newterm);
    oldfd = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldfd | O_NONBLOCK);
    c = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldterm);
    fcntl(STDIN_FILENO, F_SETFL, oldfd);
    if(c!=EOF){
        ungetc(c, stdin);
        result = getchar();
    }
    return result;
}


vehicule** alloc_road(int nb_c, int nb_l){
    vehicule** road = (vehicule**)malloc((nb_c)*sizeof(vehicule*));
    int i,j;
    vehicule ghost = generGhost();
    for(i=0; i<nb_l; i++){
        road[i] = (vehicule*)malloc(nb_l*sizeof(vehicule));
    }
    for(i=0;i<nb_c;i++){
        for(j=0;j<nb_l;j++){
            road[i][j]=ghost;
        }
    }
    return road;
}

int more_cars( vehicule* carList, int nb_cars, vehicule ** road){
    int random = (rand()%(NB_VOIE_DEFAULT-1)), car_added = 0, i;
    vehicule v;
    for(i=0; i<random; i++){
        v = generVehicule(NB_VOIE_DEFAULT);
        addCar(&v, carList, nb_cars);
        car_added += 1;
        draw_car(&v);
        road[v.posx][v.posy] = v;
    }
    return car_added;
}


void player_mode(int best){
    srand(time(NULL));
    vehicule** road = alloc_road(NB_VOIE_DEFAULT, HAUTEUR_ROUTE);
    vehicule* carList = malloc(NB_MAX_CAR*sizeof(vehicule));
    int nb_cars = 0, score=0, size_score = 1;
    vehicule player = generPlayer();
    road[player.posx][player.posy] = player;
    print_road(best);
    move_player(0, player.posx, &player);
    int pos_player = NB_VOIE_DEFAULT/2;
    int b = 1;
    unsigned int lastTime = 0, currentTime;
    while(b){
        char c = key_pressed();
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
        if(c=='A'){
            //test si vitesse inf a VIT_MAX_PLAYER
            if (player.vitesse<VIT_MAX_PLAYER){
               player.vitesse +=10;
                update_vitesse(&player);
            }
        }
        if(c=='B'){
            //test si vitesse sup  a VIT_MIN_PLAYER
            if (player.vitesse>VIT_MIN_PLAYER){
                player.vitesse -=10;
                update_vitesse(&player);
            }
        }
        clean_cursor();
        currentTime = SDL_GetTicks();
        if (currentTime > lastTime + 199) {
            int car_removed = move_cars(carList, nb_cars, &player, &road);
            int car_added = more_cars(carList, nb_cars, road);
            nb_cars += car_added;
            score += car_removed*5;
            nb_cars -= car_removed;
            size_score = get_size_int(score);
            update_panel(&player, score, size_score, best);
            lastTime = currentTime;
            clean_cursor();
        }
    }
}

void IA_mode(int best){

    //printf("This mode is not implemented yet.\nCome back later. Bye!\n");
    //printf("best score for IA: %d\n",best);
    srand(time(NULL));
    vehicule** road = alloc_road(NB_VOIE_DEFAULT, HAUTEUR_ROUTE);
    vehicule* carList = malloc(NB_MAX_CAR*sizeof(vehicule));
    vehicule * ghost =  malloc(sizeof(vehicule));
     *ghost = generGhost();
    int nb_cars = 0, score=0, size_score = 1;
    vehicule IA;
    IA.posx = 1;
    IA.posy = HAUTEUR_ROUTE;
    IA.type = 'v';
    IA.couleur = BLUE;
    IA.custom = choixCustom('v', RED);
    IA.vitesse = 150;
    IA.ghost=12;
    road[IA.posx][IA.posy] = IA;
    print_road(best);
    move_player(0, IA.posx, &IA);
    int pos_IA = NB_VOIE_DEFAULT/2;
/*
    vehicule v = generVehicule(NB_VOIE_DEFAULT);
    addCar(&v, carList, nb_cars);
    nb_cars += 1;
    draw_car(&v);
    road[v.posx][v.posy] = v;
    vehicule v2 = generVehicule(NB_VOIE_DEFAULT);
    v2.posy = 12;
    v2.posx = 1;
    addCar(&v2, carList, nb_cars);
    nb_cars += 1;
    draw_car(&v2);
    road[v2.posx][v2.posy] = v2;
    vehicule v3 = generVehicule(NB_VOIE_DEFAULT);
    v3.posy = 7;
    v3.posx = 1;
    addCar(&v3, carList, nb_cars);
    nb_cars += 1;
    draw_car(&v3);
    road[v3.posx][v3.posy] = v3;
    vehicule v4 = generVehicule(NB_VOIE_DEFAULT);
    v4.posy = 8;
    v4.posx = 2;
    addCar(&v4, carList, nb_cars);
    nb_cars += 1;
    draw_car(&v4);
    road[v4.posx][v4.posy] = v4;
    vehicule v5 = generVehicule(NB_VOIE_DEFAULT);
    v5.posy = 9;
    v5.posx = 2;
    addCar(&v5, carList, nb_cars);
    nb_cars += 1;
    draw_car(&v5);
    road[v5.posx][v5.posy] = v4;
    vehicule v6 = generVehicule(NB_VOIE_DEFAULT);
    v6.posy = 5;
    v6.posx = 1;
    addCar(&v6, carList, nb_cars);
    nb_cars += 1;
    draw_car(&v6);
    road[v6.posx][v6.posy] = v4;
    vehicule v7 = generVehicule(NB_VOIE_DEFAULT);
    v7.posy = 2;
    v7.posx = 0;
    addCar(&v7, carList, nb_cars);
    nb_cars += 1;
    draw_car(&v7);
    road[v7.posx][v7.posy] = v4;
    road[0][35]=*ghost;
    road[1][35]=*ghost;
    road[2][35]=*ghost;
*/
    int b = 1;
    unsigned int lastTime = 0, currentTime;
    while(b){
        char c = key_pressed();
        if(c=='z'){
            b = 0;
        }
        if(c=='v'){
            update_vitesse(&IA);;
        }
        clean_cursor();
        currentTime = SDL_GetTicks();
        if (currentTime > lastTime + 200) {
            move_IA(&IA,&road);
            int car_removed = move_cars(carList, nb_cars, &IA, &road);
            int car_added = more_cars(carList, nb_cars, road);
            score += car_removed*5;
            nb_cars += car_added;
            nb_cars -= car_removed;
            size_score = get_size_int(score);
            update_panel(&IA, score, size_score, best);
            lastTime = currentTime;
            clean_cursor();
        }
    }
}
