#ifndef GAME_H
#define GAME_H

#include "vehicule.h"
#include "couleur.h"
#include "sound.h"
#include "list_car.h"
#include "display.h"

void player_mode(int best);
void IA_mode(int best);
char key_pressed();
vehicule** alloc_road(int nb_l, int nb_c);

#endif
