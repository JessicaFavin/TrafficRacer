#include <stdio.h>
#include "keys.h"

const char* VOITURE_PLAYER  = "\e[31m🚘\e[39m";
const char* VOITURE  = "\e[93m🚖\e[39m";
const char* COLLISION = "\e[31m💥\e[39m";
const char* PALMIER = " 🌴  ";
const int HAUTEUR_ROUTE = 30;
const int HAUTEUR_MENU = 9;
const int LARGEUR_ROUTE = 8;

void print_player(int pos){
    if(pos%3==0)
        printf("        ║\e[100m   %s   \e[93m|\e[39m        \e[93m|\e[39m       \e[49m║\n", VOITURE_PLAYER);
    if(pos%3==1)
        printf("        ║\e[100m       \e[93m|\e[39m   %s    \e[93m|\e[39m       \e[49m║\n", VOITURE_PLAYER);
    if(pos%3==2)
        printf("        ║\e[100m       \e[93m|\e[39m        \e[93m|\e[39m    %s  \e[49m║\n", VOITURE_PLAYER);

}

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

void move_player(int old_pos, int new_pos){
    printf("\033[%d;%dH\e[100m \e[49m", HAUTEUR_ROUTE+HAUTEUR_MENU, old_pos+8+5);
    printf("\033[%d;%dH\e[100m%s \e[49m", HAUTEUR_ROUTE+HAUTEUR_MENU, (new_pos*LARGEUR_ROUTE)+8+5, VOITURE_PLAYER);
}

void clean_cursor(){
    printf("\033[%d;1H ",HAUTEUR_ROUTE+HAUTEUR_MENU+1);
}

void reposition_cursor(){
    printf("\033[%d;1H",HAUTEUR_ROUTE+HAUTEUR_MENU+1);
}

int main(int argc, char* argv[]){
    printf("Bienvenue sur traffic racer\n");
    print_game(0);
    int pos_player = 0;

    int b = 1;
    while(b){
        char c = key_pressed();
        clean_cursor();
        if(c=='d' && pos_player<2){
            pos_player += 1;
            move_player(pos_player-1, pos_player);
        }
        if(c=='q' && pos_player>0){
            pos_player-=1;
            move_player(pos_player+1, pos_player);
        }
        if(c=='z'){
            b = 0;
        }
        reposition_cursor();
    }

    return 0;
}
