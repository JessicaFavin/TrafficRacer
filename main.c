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

void save_best(int best){
    int best_IA = get_best_score_IA();
    char* best_c = malloc(sizeof(char)*100);
    sprintf(best_c, "%d", best);
    char* best_IA_c = malloc(sizeof(char)*100);
    sprintf(best_IA_c, "%d", best_IA);
    FILE* fichier = NULL;
    fichier = fopen("best_score.txt", "w");
    if (fichier != NULL){
        fputs(best_c, fichier);
        fputs("\n", fichier);
        fputs(best_IA_c, fichier);
        fclose(fichier);
    }
    printf("Score saved!");
}

void save_best_IA(int best_IA){
    int best = get_best_score();
    char* best_c = malloc(sizeof(char)*100);
    sprintf(best_c, "%d", best);
    char* best_IA_c = malloc(sizeof(char)*100);
    sprintf(best_IA_c, "%d", best_IA);
    FILE* fichier = NULL;
    fichier = fopen("best_score.txt", "w");
    if (fichier != NULL){
        fputs(best_c, fichier);
        fputs("\n", fichier);
        fputs(best_IA_c, fichier);
        fclose(fichier);
    }
    printf("Score IA saved!");
}

int main(int argc, char* argv[]){
    //faire un menu pour choisir le mode

    int choice = 0, best = 0, score = 0, quit = 1;
    while(quit){
        system("clear");
        printf("\033[1;1H");
        choice = launch_menu();
        //------------------------------MODE PLAYER---------------------------------
        //printf("Choix = %d\n", choice);

        if(choice==1){
            best = get_best_score();
            decompte_display();
            score = player_mode(best);
            if(score>best){
                save_best(score);
            }
        }
        //-------------------------------MODE IA------------------------------------
        if(choice==2){
            best = get_best_score_IA();
            //decompte_display();
            score = IA_mode(best);
            if(score>best){
                save_best_IA(score);
            }
        }

        //------------------------------QUITTER-------------------------------------
        if(choice==3){
            exit(0);
        }
    }
    return 0;
}
