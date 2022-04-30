//
// Created by ogi on 29.04.2022.
//
#include "tube.h"

void addAtom(tube* Tube, atom Atom) { // atom adder function
    pthread_mutex_lock(&(Tube->tubeMutex)); // lock specific tube to spill atom
    Tube->array[Tube->front + Tube->totalAtoms] = Atom;
    pthread_mutex_lock(&qriticalMutex); // lock all tubes entering CS since a decision related to all will be given
    Tube->totalAtoms++; // increase atom number of tube
    Tube->value= Tube->value + Atom.typeValue; // add the value of the atom

    // if the tube's value is equal to any of the molecules, it produces that molecule
    if(Tube->value == 1200 || Tube->value == 2010 || Tube->value == 12000 || Tube->value == 10300){
        switch (Tube->value) {
            case 1200:
                infoPrint(0, Tube->tubeID); // prints the mol
                removeMol(Tube); // resets the tube
            case 2010:
                infoPrint(1, Tube->tubeID);
                removeMol(Tube);
            case 12000:
                infoPrint(2, Tube->tubeID);
                removeMol(Tube);
            case 10300:
                infoPrint(3, Tube->tubeID);
                removeMol(Tube);
        }
    }
    pthread_mutex_unlock(&qriticalMutex); // exit CS
    if (pthread_cond_signal(&Tube->tubeCondition) != 0){
        printf("Condition Signal could not be generated!");
    }
    pthread_mutex_unlock(&Tube->tubeMutex); // unlock tube's mutex
}

void* removeMol(void* Tube){ // reset the tube by resetting its parameters
    for(int i=0; i < ((tube*)Tube)->totalAtoms; i++){
        ((tube*)Tube)->array = (atom*) malloc(10);
    }
    ((tube*)Tube)->value = 0;
    ((tube*)Tube)->front = 0;
    ((tube*)Tube)->totalAtoms = 0;
}

void* infoPrint(int molType, int tubeNumber){ // printing function
    const char *molArray[4];
    molArray[0] = "H2O";
    molArray[1] = "CO2";
    molArray[2] = "NO2";
    molArray[3] = "NH3";
    printf("%s is created in tube %d. \n", molArray[molType], tubeNumber);
}



