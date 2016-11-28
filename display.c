#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ncurses.h>
#include "vehicule.h"
#include "couleur.h"
#include "list_car.h"

#define WIDTH 20
#define HEIGHT 7

int startx = 0;
int starty = 0;

char *choices[] = {
			"Mode manuel",
			"Mode automatique",
			"Quitter"
		  };
int n_choices = sizeof(choices) / sizeof(char *);


const int HAUTEUR_ROUTE = 35;
const int HAUTEUR_MENU = 8;
const int LARGEUR_MENU = 24;
const int LARGEUR_ROUTE = 8;
const int NB_VOIE_DEFAULT = 3;
const char * TAB = "                                            ";
//+2 for the cactuses
const int TAB_SIZE = 44+5+2;
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

int difficulty(int difficulty ){
    if(difficulty == 1 ){return 10;}
    else if(difficulty == 2){return 20;}
    else{return 30;}

}

void print_controls(){
    printf("\033[1;80H╔══════════════════════╗\n");
    printf("\033[2;80H║Accélerer    ▶ z ou ↑ ║\n");
    printf("\033[3;80H╠══════════════════════╣\n");
    printf("\033[4;80H║Ralentir     ▶ s ou ↓ ║\n");
    printf("\033[5;80H╠══════════════════════╣\n");
    printf("\033[6;80H║Gauche       ▶ q ou ← ║\n");
    printf("\033[7;80H╠══════════════════════╣\n");
    printf("\033[8;80H║Droite       ▶ d ou → ║\n");
    printf("\033[9;80H╠══════════════════════╣\n");
    printf("\033[10;80H║Klaxonner    ▶ k      ║\n");
    printf("\033[11;80H╠══════════════════════╣\n");
    printf("\033[12;80H║Quitter      ▶ l      ║\n");
    printf("\033[13;80H╚══════════════════════╝\n");
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
    print_controls();
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

void move_player(int new_pos, vehicule * player){
    clean_car(player);
    player->posx = new_pos;
    draw_car(player);
}

int move_cars(vehicule * carList, int nbCars, vehicule * player, vehicule ** road){
    int i,j;
    int car_removed = 0;
    vehicule * ghost =  malloc(sizeof(vehicule));
    *ghost = generGhost();
    for (i=0;i<nbCars;i++){
    	vehicule * v = &carList[i];

        /*for(j=0;j<nbCars;j++){
            if(v->posy+1 == carList[j].posy || v->posy-1 == carList[j].posy){v->vitesse = carList[j].vitesse;}
        }*/



        if ((v->posy)>=0 && (v->posy)<=HAUTEUR_ROUTE){ // supprime la voiture que si elle est visible et la remplace par un phantom dans la matrice
            clean_car(v);
            road[(v->posy)][v->posx] = *ghost;
        }
        
        /*
          compare la vitesse à celle du player
          et bouge la voiture en fonction
        */
        if(player->vitesse > v->vitesse){

          //on "descend" la voiture

            if((v->posy)>0 && (v->posy)<HAUTEUR_ROUTE){                 
                road[v->posy+1][v->posx] = *v;
            }
            v->posy=v->posy+1;
        }else if (player->vitesse < v->vitesse) {
            //on "monte" la voiture
            if((v->posy)>0 && (v->posy)<HAUTEUR_ROUTE){                
            road[v->posy-1][v->posx] = *v;
            }
        v->posy=v->posy-1;    
        }else{
            road[v->posy][v->posx] = *v;
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

void move_IA(vehicule * IA, vehicule ** road){          // Mouvement de l'IA
    
    int scan_height=1;
    int i;
    int cmpt_G=0,cmpt_D=0;


    vehicule ghost = generGhost();

    for(i=0;i<HAUTEUR_ROUTE;i++){
        if(road[i][0].ghost != ghost.ghost){cmpt_G+=1;}

        if(road[i][2].ghost != ghost.ghost){cmpt_D+=1;}
    }

    if(road[IA->posy-scan_height][IA->posx].ghost == 0) {
        
        if (road[IA->posy-(scan_height)][IA->posx].vitesse > 25){
            IA->vitesse=road[IA->posy-(scan_height)][IA->posx].vitesse;
        }

        if(IA->posx==0 && road[34][1].ghost == 1){
            road[IA->posy][IA->posy]= ghost;
            move_player(1,IA);
            road[IA->posy][IA->posx]=*IA;
        }

        else if(IA->posx==1){

            if(road[33][2].ghost == 1 && cmpt_D<cmpt_G){
                road[IA->posy][IA->posy]= ghost;
                move_player(2,IA);
                road[IA->posy][IA->posx]=*IA;
            }
            else if(road[34][0].ghost == 1 && road[33][0].ghost == 1){
                road[IA->posy][IA->posy]= ghost;
                move_player(IA->posx-1,IA);
                road[IA->posy][IA->posx]=*IA;
            }

        }
        else if(IA->posx == 2 && road[34][1].ghost == 1){
            road[IA->posy][IA->posy]= ghost;
            move_player(IA->posx-1,IA);
            road[IA->posy][IA->posx]=*IA;
        }
    }
    if(road[IA->posy-(scan_height)][IA->posx].ghost == 1 && road[IA->posy-(scan_height+1)][IA->posx].ghost == 1){
        IA->vitesse=150;
    }
    draw_car(IA);
}

void print_menu(WINDOW *menu_win, int highlight)
{   
	int x, y, i;

	x = 2;
	y = 2;
	box(menu_win, 0, 0);
	for(i = 0; i < n_choices; ++i)
	{	if(highlight == i + 1) /* High light the present choice */
		{	wattron(menu_win, A_REVERSE);
			mvwprintw(menu_win, y, x, "%s", choices[i]);
			wattroff(menu_win, A_REVERSE);
		}
		else
			mvwprintw(menu_win, y, x, "%s", choices[i]);
		++y;
	}
	wrefresh(menu_win);
}

int launch_menu() {
    WINDOW *menu_win;
	int highlight = 1;
	int choice = 0;
	int c;

	initscr();
	clear();
	noecho();
	cbreak();	/* Line buffering disabled. pass on everything */
    int row,col;
    getmaxyx(stdscr,row,col);
    //width of game = 76
	startx = (col/3) - (WIDTH/2);
	starty = (row/3) - (HEIGHT/2);

	menu_win = newwin(HEIGHT, WIDTH, starty, startx);
	keypad(menu_win, TRUE);
    mvprintw(3,0,"                     ______              ____ ____ _          ____                          \n                    /_  __/_____ ____ _ / __// __/(_)_____   / __ \\ ____ _ _____ ___   _____\n                     / /  / ___// __ `// /_ / /_ / // ___/  / /_/ // __ `// ___// _ \\ / ___/\n                    / /  / /   / /_/ // __// __// // /__   / _, _// /_/ // /__ /  __// /    \n                   /_/  /_/    \\__,_//_/  /_/  /_/ \\___/  /_/ |_| \\__,_/ \\___/ \\___//_/     \n                    Use arrow keys to go up and down, Press enter to select a choice");
	refresh();
	print_menu(menu_win, highlight);
	while(1)
	{	c = wgetch(menu_win);
		switch(c)
		{	case KEY_UP:
				if(highlight == 1)
					highlight = n_choices;
				else
					--highlight;
				break;
			case KEY_DOWN:
				if(highlight == n_choices)
					highlight = 1;
				else
					++highlight;
				break;
			case 10:
				choice = highlight;
				break;
			default:
				break;
		}
		print_menu(menu_win, highlight);
		if(choice != 0)	/* User did a choice come out of the infinite loop */
			break;
	}
    //mvprintw(23, 0, "You chose choice %d with choice string %s\n", choice, choices[choice - 1]);
	clrtoeol();
	refresh();
	endwin();
	return choice;
}
