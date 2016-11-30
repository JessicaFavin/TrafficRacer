#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <ncurses.h>
#include <SDL2/SDL.h>
#include "vehicule.h"
#include "couleur.h"
#include "list_car.h"
#include "sound.h"
#include "game.h"

#define WIDTH 20
#define HEIGHT 7

int startx = 0;
int starty = 0;

char *choices[] = {
	"Mode manuel",
	"Mode automatique",
	"Quitter"
};

char *difficulty_choices[] = {
	"Facile",
	"Normal",
	"Difficile"
};
int n_choices = sizeof(choices) / sizeof(char *);
int d_choices = sizeof(difficulty_choices) / sizeof(char *);

const int HAUTEUR_ROUTE = 35;
const int HAUTEUR_MENU = 8;
const int LARGEUR_MENU = 24;
const int LARGEUR_ROUTE = 8;
const int NB_VOIE_DEFAULT = 3;
const char * TAB = "                                            ";
//+2 for the decors
const int TAB_SIZE = 44+5+2;
const int VIT_MAX_IA = 150;
const int VIT_MIN_IA = 50;

int decor = 0;
int ligne_blanches = 0;



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

char * current_song(int frq){
	if(frq==2){
		return "Beparwa";
	}
	if(frq==3){
		return "Ballad";
	}
	if(frq==1){
		return "Gamey";
	}
	return "Musala";
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
	printf("\033[12;80H║Radio        ▶ r      ║\n");
	printf("\033[13;80H╠══════════════════════╣\n");
	printf("\033[14;80H║Quitter      ▶ l      ║\n");
	printf("\033[15;80H╚══════════════════════╝\n");
	printf("\033[16;80H           Radio");
	printf("\033[17;80HChanson en cours: %s     ",current_song(radiofrq));
}

void update_radio(){
	printf("\033[17;80HChanson en cours: %s     ",current_song(radiofrq));
}
void print_road(int best){
	int i, j;
	for(i=0; i<HAUTEUR_ROUTE; i++){
		printf("%s", TAB);
		if(i%3==0){
			printf("%s🌵%s ", GREEN, DEFAULT);
		} else {
			printf("  ");
		}
		printf("║%s", BACKROAD);
		for(j=0; j<NB_VOIE_DEFAULT; j++){
			printf("       ");
			if(!(j==NB_VOIE_DEFAULT-1) && i%3==0){
				printf("|");
			} else {
				printf(" ");
			}
		}
		printf("%s║", BACKDEFAULT);
		if(i%3==1){
			printf("%s🌵%s \n", GREEN, DEFAULT);
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

void move_road(){
	int i, j;
	for(i=0; i<HAUTEUR_ROUTE; i++){
		printf("\033[%d;%dH",i+1,TAB_SIZE-6);
		if(i%3==decor){
			printf("  ");
		}
		if(i%3==(decor+1)%3) {
			if(i==nb_tour%100){
				if(radiofrq==1){
					printf("🐄 ");
					if(nb_tour%36==0){
						playSound(cow);
					}
				} else if(radiofrq==2) {
					printf("🐐 ");
					if(nb_tour%36==0){
						playSound(goat);
					}
				} else if(radiofrq==3) {
					printf("🕊 ");
					if(nb_tour%36==0){
						playSound(eagle);
					}
				} else {
					printf("🐒 ");
					if(nb_tour%36==0){
						playSound(monkey);
					}
				}
			} else {
				if(radiofrq==1){
					printf("%s🌵%s ", GREEN, DEFAULT);
				} else if(radiofrq==2) {
					printf("%s🌴%s ", GREEN, DEFAULT);
				} else if(radiofrq==3) {
					printf("%s🌺%s ", YELLOW, DEFAULT);
				} else {
					printf("%s🌲%s ", GREEN, DEFAULT);
				}
			}
		}
		for(j=0; j<NB_VOIE_DEFAULT; j++){
			printf("\033[%d;%dH%s", i+1, 61+(j*8)-6, BACKROAD);
			if(!(j==NB_VOIE_DEFAULT-1) && i%3==ligne_blanches){
				printf(" ");
			}
			if(!(j==NB_VOIE_DEFAULT-1) && i%3==(ligne_blanches+1)%3){
				printf("|");
			}
		}
		printf("%s", BACKDEFAULT);
		printf("\033[%d;%dH",i+1,TAB_SIZE+25-3);
		if(i%3==(decor+1)%3){
			printf("  ");
		}
		if(i%3==(decor+2)%3) {
			if(i+7==nb_tour%100){
				if(radiofrq==1){
					printf("🐄 ");
					if(nb_tour%36==0){
						playSound(cow);
					}
				} else if(radiofrq==2) {
					printf("🐐 ");
					if(nb_tour%36==0){
						playSound(goat);
					}
				} else if(radiofrq==3) {
					printf("🕊 ");
					if(nb_tour%36==0){
						playSound(eagle);
					}
				} else {
					printf("🐒 ");
					if(nb_tour%36==0){
						playSound(monkey);
					}
				}
			} else {
				if(radiofrq==1){
					printf("%s🌵%s ", GREEN, DEFAULT);
				} else if(radiofrq==2) {
					printf("%s🌴%s ", GREEN, DEFAULT);
				} else if(radiofrq==3) {
					printf("%s🌺%s ", YELLOW, DEFAULT);
				} else {
					printf("%s🌲%s ", GREEN, DEFAULT);
				}
			}
		}
	}
	clean_cursor();
	decor = (decor+1)%3;
	ligne_blanches = (ligne_blanches+1)%3;
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
	int i;
	int car_removed = 0;
	vehicule * ghost =  malloc(sizeof(vehicule));
	*ghost = generGhost();
	for (i=0;i<nbCars;i++){
		vehicule * v = &carList[i];

		/*for(j=0;j<nbCars;j++){
		if(v->posy+1 == carList[j].posy){v->vitesse = carList[j].vitesse;}
	}*/



	if ((v->posy)>=0 && (v->posy)<=HAUTEUR_ROUTE && v->vitesse != player->vitesse){ // supprime la voiture que si elle est visible et la remplace par un phantom dans la matrice
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

void move_IA(vehicule * carList, int nbCars, vehicule * IA, vehicule ** road){          // Mouvement de l'IA

int scan_height=1;
int i,j;
int cmpt_G=0,cmpt_D=0;


vehicule ghost = generGhost();

    for(i=0;i<HAUTEUR_ROUTE;i++){
	   if(road[i][IA->posx-1].ghost != ghost.ghost){cmpt_G+=1;}

	   if(road[i][IA->posx+1].ghost != ghost.ghost){cmpt_D+=1;}
    }
    for(j=0;j<nbCars;j++){
        if(carList[j].posy == IA->posy-scan_height && carList[j].posx == IA->posx){
                IA->vitesse = carList[j].vitesse;
        }
        if(carList[j].posy == IA->posy && road[carList[j].posy][carList[j].posx].ghost != 1){
            carList[j].vitesse = road[carList[j].posy][carList[j].posx].vitesse;
            if(carList[j].vitesse == 50){
                carList[j].vitesse = 40;
            }
        }else if(carList[j].posy-scan_height == IA->posy && road[IA->posy-scan_height][carList[j].posx].ghost != 1){
            carList[j].vitesse = road[carList[j].posy-scan_height][carList[j].posx].vitesse;
        }

    }
    if(road[IA->posy-scan_height][IA->posx].ghost == 0) {

    	if(IA->posx==0 && road[IA->posy-scan_height][IA->posx+1].ghost == 1 && road[IA->posy][IA->posx+1].ghost != 0){
    		move_player(1,IA);
    		road[IA->posy][IA->posx]=*IA;
    	}

    	else if(IA->posx>0 && IA->posx<NB_VOIE_DEFAULT-1){

    		if(road[IA->posy-scan_height][IA->posx+1].ghost == 1 && cmpt_D<cmpt_G && road[IA->posy][IA->posx+1].ghost == 1){
    			move_player(2,IA);
    			road[IA->posy][IA->posx]=*IA;
    		}
    		else if(road[IA->posy-scan_height][IA->posx-1].ghost == 1 && road[IA->posy][IA->posx-1].ghost != 0){
    			move_player(IA->posx-1,IA);
    			road[IA->posy][IA->posx]=*IA;
    		}

    	}
    	else if(IA->posx == NB_VOIE_DEFAULT-1 && road[IA->posy-scan_height][IA->posx-1].ghost == 1 && road[IA->posy][IA->posx-1].ghost != 0){
    		move_player(IA->posx-1,IA);
    		road[IA->posy][IA->posx]=*IA;

    	}else if((road[IA->posy-(scan_height)][IA->posx+1].ghost == 0 || road[IA->posy-(scan_height)][IA->posx-1].ghost ==0) && road[IA->posy-scan_height][IA->posx].ghost == 0){
    		if(IA->posx == 2){
    		      move_player(IA->posx-1,IA);
    		      road[IA->posy][IA->posx]=*IA;
    	   }else if(IA->posx == 0){
    	           move_player(IA->posx+1,IA);
    	           road[IA->posy][IA->posx]=*IA;
            }
        }
    }
    if(road[IA->posy-(scan_height)][IA->posx].vitesse != IA->vitesse && nb_tour%5 == 0 ){
    	   IA->vitesse=150;
    }
    for(j=0;j<nbCars;j++){
        if(carList[j].posy == IA->posy-scan_height && carList[j].posx == IA->posx){
            IA->vitesse = carList[j].vitesse;
        }
    }
    if(IA->vitesse == 50 && nb_tour%5 == 0){
        IA->vitesse = 40;
    }
    draw_car(IA);


}

void print_menu(WINDOW *menu_win, int highlight){
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

void print_menu_difficulty(WINDOW *menu_win, int highlight){
	int x, y, i;

	x = 2;
	y = 2;
	box(menu_win, 0, 0);
	for(i = 0; i < d_choices; ++i)
	{   if(highlight == i + 1) /* High light the present choice */
		{   wattron(menu_win, A_REVERSE);
			mvwprintw(menu_win, y, x, "%s", difficulty_choices[i]);
			wattroff(menu_win, A_REVERSE);
		}
		else
		mvwprintw(menu_win, y, x, "%s", difficulty_choices[i]);
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
	mvprintw(3,0,"                     ______              ____ ____ _          ____                          \n                    /_  __/_____ ____ _ / __// __/(_)_____   / __ \\ ____ _ _____ ___   _____\n                     / /  / ___// __ `// /_ / /_ / // ___/  / /_/ // __ `// ___// _ \\ / ___/\n                    / /  / /   / /_/ // __// __// // /__   / _, _// /_/ // /__ /  __// /    \n                   /_/  /_/    \\__,_//_/  /_/  /_/ \\___/  /_/ |_| \\__,_/ \\___/ \\___//_/     \n              Utilisez les fleches Haut et Bas pour selectionner.Appuyer Entree pour selectionner. \n\n                                  ATTENTION au volume de votre son !!!!");
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

int launch_difficulty_menu() {
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
	mvprintw(3,0,"                     ______              ____ ____ _          ____                          \n                    /_  __/_____ ____ _ / __// __/(_)_____   / __ \\ ____ _ _____ ___   _____\n                     / /  / ___// __ `// /_ / /_ / // ___/  / /_/ // __ `// ___// _ \\ / ___/\n                    / /  / /   / /_/ // __// __// // /__   / _, _// /_/ // /__ /  __// /    \n                   /_/  /_/    \\__,_//_/  /_/  /_/ \\___/  /_/ |_| \\__,_/ \\___/ \\___//_/     \n              Utilisez les fleches Haut et Bas pour selectionner.Appuyer Entree pour selectionner. \n\n                                     Choisissez la difficulté : ");
	refresh();
	print_menu_difficulty(menu_win, highlight);
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
		print_menu_difficulty(menu_win, highlight);
		if(choice != 0)	/* User did a choice come out of the infinite loop */
		break;
	}
	clrtoeol();
	refresh();
	endwin();
	return choice;
}

int game_over(){
	system("clear");
	printf("\033[4;22H    ______                           ____");
	printf("\033[5;22H   / ____/____ _ ____ ___   ___     / __ \\ _   __ ___   _____");
	printf("\033[6;22H  / / __ / __ `// __ `__ \\ / _ \\   / / / /| | / // _ \\ / ___/");
	printf("\033[7;22H / /_/ // /_/ // / / / / //  __/  / /_/ / | |/ //  __// /");
	printf("\033[8;22H \\____/ \\__,_//_/ /_/ /_/ \\___/   \\____/  |___/ \\___//_/    \n");
	unsigned int currentTime = SDL_GetTicks();
	unsigned int beginTime = currentTime;
	while(currentTime -beginTime < 3500){currentTime = SDL_GetTicks();}
	system("clear");
	return 0;
}

void decompte_display(){
	unsigned int currentTime = SDL_GetTicks();
	unsigned int beginTime = currentTime;
	system("./killSound.sh ./Sound/LoadingLoop.wav repeat 9999");
	system("play -q -v 0.99 ./Sound/debutJeu.mp3 &");
	system("clear");
	printf("\033[4;43H  333333333333333   ");
	printf("\033[5;43H 3:::::::::::::::33 ");
	printf("\033[6;43H 3::::::33333::::::3");
	printf("\033[7;43H 3333333     3:::::3");
	printf("\033[8;43H             3:::::3");
	printf("\033[9;43H             3:::::3");
	printf("\033[10;43H    33333333:::::3 ");
	printf("\033[11;43H    3:::::::::::3  ");
	printf("\033[12;43H    33333333:::::3 ");
	printf("\033[13;43H             3:::::3");
	printf("\033[14;43H             3:::::3");
	printf("\033[15;43H             3:::::3");
	printf("\033[16;43H 3333333     3:::::3");
	printf("\033[17;43H 3::::::33333::::::3");
	printf("\033[18;43H 3:::::::::::::::33 ");
	printf("\033[19;43H  333333333333333   \n");
	while(currentTime -beginTime < 1012){currentTime = SDL_GetTicks();}
	system("clear");
	printf("\033[4;43H  222222222222222    ");
	printf("\033[5;43H 2:::::::::::::::22  ");
	printf("\033[6;43H 2::::::222222:::::2 ");
	printf("\033[7;43H 2222222     2:::::2 ");
	printf("\033[8;43H            2:::::2  ");
	printf("\033[9;43H            2:::::2  ");
	printf("\033[10;43H         2222::::2  ");
	printf("\033[11;43H    22222::::::22   ");
	printf("\033[12;43H  22::::::::222     ");
	printf("\033[13;43H 2:::::22222        ");
	printf("\033[14;43H2:::::2             ");
	printf("\033[15;43H2:::::2             ");
	printf("\033[16;43H2:::::2       222222");
	printf("\033[17;43H2::::::2222222:::::2");
	printf("\033[18;43H2::::::::::::::::::2");
	printf("\033[19;43H22222222222222222222\n");
	beginTime = currentTime;
	while(currentTime - beginTime < 1200){currentTime = SDL_GetTicks();}
	system("clear");
	printf("\033[4;43H  1111111   ");
	printf("\033[5;43H 1::::::1   ");
	printf("\033[6;43H1:::::::1   ");
	printf("\033[7;43H111:::::1   ");
	printf("\033[8;43H   1::::1   ");
	printf("\033[9;43H   1::::1   ");
	printf("\033[10;43H   1::::1   ");
	printf("\033[11;43H   1::::l   ");
	printf("\033[12;43H   1::::l   ");
	printf("\033[13;43H   1::::l   ");
	printf("\033[14;43H   1::::l   ");
	printf("\033[15;43H   1::::l   ");
	printf("\033[16;43H111::::::111");
	printf("\033[17;43H1::::::::::1");
	printf("\033[18;43H1::::::::::1");
	printf("\033[19;43H111111111111\n");
	beginTime = currentTime;
	while(currentTime - beginTime < 1200){currentTime = SDL_GetTicks();}
	system("clear");
	printf("\033[4;22H         GGGGGGGGGGGGG     OOOOOOOOO      !!! ");
	printf("\033[5;22H      GGG::::::::::::G   OO:::::::::OO   !!:!!");
	printf("\033[6;22H    GG:::::::::::::::G OO:::::::::::::OO !:::!");
	printf("\033[7;22H   G:::::GGGGGGGG::::GO:::::::OOO:::::::O!:::!");
	printf("\033[8;22H  G:::::G       GGGGGGO::::::O   O::::::O!:::!");
	printf("\033[9;22H G:::::G              O:::::O     O:::::O!:::!");
	printf("\033[10;22H G:::::G              O:::::O     O:::::O!:::!");
	printf("\033[11;22H G:::::G    GGGGGGGGGGO:::::O     O:::::O!:::!");
	printf("\033[12;22H G:::::G    G::::::::GO:::::O     O:::::O!:::!");
	printf("\033[13;22H G:::::G    GGGGG::::GO:::::O     O:::::O!:::!");
	printf("\033[14;22H G:::::G        G::::GO:::::O     O:::::O!!:!!");
	printf("\033[15;22H  G:::::G       G::::GO::::::O   O::::::O !!! ");
	printf("\033[16;22H   G:::::GGGGGGGG::::GO:::::::OOO:::::::O     ");
	printf("\033[17;22H    GG:::::::::::::::G OO:::::::::::::OO  !!! ");
	printf("\033[18;22H      GGG::::::GGG:::G   OO:::::::::OO   !!:!!");
	printf("\033[19;22H         GGGGGG   GGGG     OOOOOOOOO      !!! \n");
	beginTime = currentTime;
	while(currentTime - beginTime < 500){currentTime = SDL_GetTicks();}
	system("clear");
}
