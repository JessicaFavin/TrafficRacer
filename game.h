#ifndef GAME_H
#define GAME_H

#include "vehicule.h"
#include "couleur.h"
#include "sound.h"
#include "list_car.h"
#include "display.h"

const char * cow;
const char * monkey;
const char * goat;
const char * eagle;
int radiofrq;
int nb_tour;
int player_mode(int best, int diff);
int IA_mode(int best);
char key_pressed();
vehicule** alloc_road(int nb_l, int nb_c);
void free_road(vehicule ** road, int nb_c, int nb_l);
int collision(int nbCars, vehicule* carList, vehicule * player);
int player_actions(char c, vehicule * player);
int IA_actions(char c);

#endif
