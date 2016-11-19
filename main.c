#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include "vehicule.h"
#include "couleur.h"
#include "sound.h"
#include "display.h"
#include "list_car.h"


char * crash="./Sound/crash.mp3";
char * klaxon="./Sound/klaxon.mp3";
char * acceleration="./Sound/acceleration.mp3";
char * cow="./Sound/cow.mp3";
char * music=" ";
char * end=" ";
char * decompte="./Sound/decompte.mp3";
char * gameOver=" ";


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


vehicule** alloc_road(int nb_l, int nb_c){
    vehicule** road = (vehicule**)malloc(nb_l*sizeof(vehicule*));
    int i;
    for(i=0; i<nb_l; i++){
        road[i] = (vehicule*)malloc(nb_c*sizeof(vehicule));
    }
    return road;
}

int main(int argc, char* argv[]){
    system("clear");
    //printf("Bienvenue sur traffic racer\n");
    //création de la matrice de la route large de 3 et haute de 30
    //qui ne sert probablement à rien au final xD
    srand(time(NULL));
    vehicule** road = alloc_road(HAUTEUR_ROUTE, NB_VOIE_DEFAULT);
    vehicule* carList = malloc(NB_MAX_CAR*sizeof(vehicule));
    int nb_cars = 0, score=0, size_score = 1, best_score=0;
    vehicule player;
    player.posx = NB_VOIE_DEFAULT/2;
    player.posy = HAUTEUR_ROUTE;
    player.type = 'v';
    player.couleur = BLUE;
    player.custom = "🚘";
    player.vitesse = 100;
    road[player.posx][player.posy] = player;
    //road[HAUTEUR_ROUTE-1][NB_VOIE_DEFAULT/2] = player;
    print_road();
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
    road[v.posx][v.posy] = v;
    vehicule v2 = generVehicule(NB_VOIE_DEFAULT);
    v2.posy = 2;
    addCar(&v2, carList, nb_cars);
    nb_cars += 1;
    draw_car(&v2);
    road[v2.posx][v2.posy] = v2;
    vehicule v3 = generVehicule(NB_VOIE_DEFAULT);
    v3.posy = 7;
    addCar(&v3, carList, nb_cars);
    nb_cars += 1;
    draw_car(&v3);
    road[v3.posx][v3.posy] = v3;
    vehicule v4 = generVehicule(NB_VOIE_DEFAULT);
    v4.posy = 9;
    addCar(&v4, carList, nb_cars);
    nb_cars += 1;
    draw_car(&v4);
    road[v4.posx][v4.posy] = v4;
    int b = 1;
    playSound(decompte);
    sleep(3);
    killSound(decompte);
    unsigned int lastTime = 0, currentTime;
    while(b){
        char c = key_pressed();
        /*
        if(c=='m'){
            int car_removed = move_cars(carList, nb_cars);
            nb_cars -= car_removed;
        }
        */
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
            //tester si vitesse inf ou egale a 150km/h
            player.vitesse +=10;
            update_vitesse(&player);
        }
        if(c=='B'){
            //tester si vitesse sup ou egale a 50km/h
            player.vitesse -=10;
            update_vitesse(&player);
        }
        clean_cursor();
        currentTime = SDL_GetTicks();
        if (currentTime > lastTime + 399) {
          //printf("Report: %d secondes\n", currentTime/1000);
          int car_removed = move_cars(carList, nb_cars, &player, &road);
          score += car_removed*5;
          nb_cars -= car_removed;
          size_score = floor(log10(abs(score))) + 1;
          update_panel(&player, score, size_score, best_score);
          lastTime = currentTime;
        }
    }
    return 0;
}
