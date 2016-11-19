#ifndef DISPLAY_H
#define DISPLAY_H

const int HAUTEUR_ROUTE;
const int NB_VOIE_DEFAULT;
const int NB_MAX_CAR;


void print_road();
void update_score(int score, int size_score);
void update_best_score(int best, int size_best);
void update_vitesse(vehicule * player);
void update_panel(vehicule * player, int score, int size_score, int best_score);
void move_player(int old_pos, int new_pos, vehicule * player);
void draw_car(vehicule * v);
void clean_car(vehicule * v);
int move_cars(vehicule * carList, int nbCars, vehicule * player, vehicule *** road_pointer);
void clean_cursor();
void reposition_cursor();

#endif
