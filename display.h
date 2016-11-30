#ifndef DISPLAY_H
#define DISPLAY_H

const int HAUTEUR_ROUTE;
const int NB_VOIE_DEFAULT;
const int LARGEUR_ROUTE;
const int TAB_SIZE;



int get_size_int(int entier);
void print_road(int best, int mode);
void update_score(int score, int size_score);
void update_best_score(int best, int size_best);
void update_vitesse(vehicule * player);
void update_panel(vehicule * player, int score, int size_score, int best_score);
void move_player(int new_pos, vehicule * player);
void draw_car(vehicule * v);
void clean_car(vehicule * v);
int move_cars(vehicule * carList, int nbCars, vehicule * player, vehicule ** road);
void clean_cursor();
void reposition_cursor();
void move_IA(vehicule * carList, int nbCars, vehicule * IA, vehicule ** road);
int launch_menu();
int difficulty(int difficulty);
int game_over();
int game_over_menu();
void decompte_display();
int launch_difficulty_menu();
void move_road();
void update_radio();
void update_restricted_radio();
#endif
