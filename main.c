#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "game.h"

int get_best_score(){
    FILE *fp;
    char buff[255];
    fp = fopen("best_score.txt", "r");
    fscanf(fp, "%s", buff);
    fclose(fp);
    return atoi(buff);
}

int get_best_score_IA(){
    FILE *fp;
    char buff[255];
    fp = fopen("best_score.txt", "r");
    //lis la première ligne
    fscanf(fp, "%s", buff);
    //lis le retour à la ligne
    fgets(buff, 255, (FILE*)fp);
    //lis la deuxième ligne
    fgets(buff, 255, (FILE*)fp);
    fclose(fp);
    return atoi(buff);
}

int main(int argc, char* argv[]){
    system("clear");
    //faire un menu pour choisir le mode
    
    int choice;
    choice = 2;
    //------------------------------MODE PLAYER---------------------------------
    if(choice==1){
        int best = get_best_score();
        player_mode(best);
    }
    //-------------------------------MODE IA------------------------------------
    if(choice==2){
        int best_IA = get_best_score_IA();
        IA_mode(best_IA);
    }
    //------------------------------QUITTER-------------------------------------
    if(choice==3){
        exit(0);
    }
    return 0;
}
