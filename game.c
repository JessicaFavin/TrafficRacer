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
int NB_MAX_CAR;

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


vehicule **alloc_road(int nb_c, int nb_l){
    vehicule **road =malloc(nb_c*sizeof(vehicule));
    int i,j;
    vehicule ghost = generGhost();

    for(i=0; i<nb_l; i++){
        road[i] = malloc(nb_l*sizeof(vehicule));
    }
    for(i=0;i<nb_l;i++){
        for(j=0;j<nb_c;j++){
            road[i][j]=ghost;
        }
    }
    return road;
}

/*void free_road(vehicule ** road, int nb_c, int nb_l){
    int  i;

    for(i=0; i<=nb_l; i++){
            free(*(road+i));
        
    }
    free(road);
}*/

int more_cars( vehicule* carList, int nb_cars, vehicule ** road){
    int random = (rand()%(NB_VOIE_DEFAULT-1)), car_added = 0, i;
    vehicule v;
    for(i=0; i<random; i++){
        if(nb_cars+car_added<NB_MAX_CAR){
            v = generVehicule(NB_VOIE_DEFAULT);
            //doit vérifier que la case de la voiture est bien libre
            //sinon soit en génère une autre tant que sa case n'est pas libre
            //soit change le posx pour une case libre
            addCar(&v, carList, nb_cars);
            car_added += 1;
            draw_car(&v);
            road[v.posy][v.posx] = v;
        }
    }
    return car_added;
}

int scoring(int car_passed, int vitesse){
    int points = 0;
    points = car_passed*vitesse/30;
    return points;
}

int collision(int nbCars, vehicule* carList, vehicule * player){
    int i;
    for(i=0;i<nbCars;i++){
        if(carList[i].posx == player->posx && carList[i].posy == player->posy){

            playSound(crash);
            printf("\033[%d;%dH\e[100m\e[31m 💥 \e[39m\e[49m",player->posy,((player->posx*LARGEUR_ROUTE)+TAB_SIZE));

            return 0;
        }
    }
    return 1;

}



int player_actions(char c, vehicule * player){
    int b = 1;
    if((c=='d'||c=='C') && player->posx<NB_VOIE_DEFAULT-1){
        move_player(player->posx+1, player);
    }
    if((c=='q'||c=='D') && player->posx>0){
        move_player(player->posx-1, player);
    }
    if(c=='l'){
        b = 0;
    }
    if(c=='A' || c=='z'){
        //test si vitesse inf a VIT_MAX_PLAYER
        if (player->vitesse<VIT_MAX_PLAYER){
           player->vitesse +=10;
            update_vitesse(player);
        }
    }
    if(c=='B' || c=='s'){
        //test si vitesse sup  a VIT_MIN_PLAYER
        if (player->vitesse>VIT_MIN_PLAYER){
            player->vitesse -=10;
            update_vitesse(player);
        }
    }
    if(c=='k'){
        playSound(klaxon);
    }
    if(c == 'm'){
        playSound(cow);
    }
    return b;
}


int player_mode(int best){
    srand(time(NULL));
    vehicule** road = alloc_road(NB_VOIE_DEFAULT+1,HAUTEUR_ROUTE+1);
    int diff = 3;
    NB_MAX_CAR = difficulty (diff);
    vehicule* carList = malloc(NB_MAX_CAR*sizeof(vehicule));
    int nb_cars = 0, score=0, size_score = 1;
    vehicule player = generPlayer();
    road[player.posx][player.posy] = player;
    player.custom="\e[31m🚘  \e[39m";
    print_road(best);
    move_player(player.posx, &player);
    int b = 1;
    unsigned int lastTime = 0, currentTime;
    while(b){
        char c = key_pressed();
        b = player_actions(c, &player);
        clean_cursor();
        currentTime = SDL_GetTicks();
        if (currentTime > lastTime + 200) {
            int car_removed = move_cars(carList, nb_cars, &player, road);
            int car_added = more_cars(carList, nb_cars, road);
            c=collision(nb_cars,carList,&player);
            nb_cars += car_added;
            score += scoring(car_removed, player.vitesse);
            nb_cars -= car_removed;
            size_score = get_size_int(score);
            update_panel(&player, score, size_score, best);
            lastTime = currentTime;
            clean_cursor();
            if (c==0){sleep(3);b=0;}
        }
    }
    free(carList);
    return score;
}

int IA_mode(int best){

    srand(time(NULL));
    vehicule** road = alloc_road(NB_VOIE_DEFAULT+1, HAUTEUR_ROUTE+1);
    NB_MAX_CAR = 50;
    vehicule* carList = malloc(NB_MAX_CAR*sizeof(vehicule));
    vehicule * ghost =  malloc(sizeof(vehicule));
     *ghost = generGhost();
    int nb_cars = 0, score=0, size_score = 1;
    vehicule IA;
    IA.posx = 1;
    IA.posy = HAUTEUR_ROUTE;
    IA.type = 'v';
    IA.couleur = BLUE;
    IA.custom="\e[31m🚘  \e[39m";
    IA.vitesse = 150;
    IA.ghost=12;
    road[IA.posx][IA.posy] = IA;
    print_road(best);
    move_player(IA.posx, &IA);
    int b = 1;
    unsigned int lastTime = 0, currentTime;
    while(b){
        char c = key_pressed();
        if(c=='l'){
            b = 0;
        }
        if(c=='v'){
            update_vitesse(&IA);
        }
        clean_cursor();
        currentTime = SDL_GetTicks();
        if (currentTime > lastTime + 10) {
            int car_removed = move_cars(carList, nb_cars, &IA, road);
            int car_added = more_cars(carList, nb_cars, road);
            score += scoring(car_removed, IA.vitesse);
            nb_cars += car_added;
            nb_cars -= car_removed;
            size_score = get_size_int(score);
            update_panel(&IA, score, size_score, best);
            lastTime = currentTime;
            move_IA(&IA,road);
            update_vitesse(&IA);
            clean_cursor();    
        }
    }
    return score;
}
