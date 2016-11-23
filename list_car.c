#include "vehicule.h"

void addCar(vehicule * v, vehicule * carList, int nbCars){
    //ajoute la voiture donnée en argument à la liste des voitures
    if(nbCars<50){
        carList[nbCars] = (*v);
    }
}

void removeCar(vehicule * carList, int nbCars){
    /*
    retire la voiture donnée en argument à la liste des voitures
    */
    int i;
    int decale = 0;
    for(i=0; i<nbCars-1; i++){
        if(carList[i].etat==0){
            decale+=1;
        }
        if(decale){
            carList[i]=carList[i+decale];
        }
    }
}
