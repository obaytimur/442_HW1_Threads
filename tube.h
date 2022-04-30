//
// Created by ogi on 29.04.2022.
//

#ifndef HW1_TUBE_H
#define HW1_TUBE_H
#include "atom.h"
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// The functions of the tube struct are described in the source codes

typedef struct tube {
    atom* array;
    int front;
    int value; // total value of the tube, i.e. sum of its atom values
    int totalAtoms;
    int tubeID;
    int tubeTS; // time stamp (ID of the atom spilled first)
    int moleculeTYPE; // 1:H2O, 2:CO2, 3: NO2, 4: NH3
    pthread_mutex_t tubeMutex;
    pthread_cond_t tubeCondition;
}tube;

void addAtom(tube* Tube, atom Atom);
void* removeMol(void* Tube);
void* infoPrint(int molType, int tubeNumber);

pthread_mutex_t qriticalMutex;
pthread_mutex_t sleepMutex;
pthread_mutex_t informationMutex;

typedef struct Information {
    int tubeID;
    tube Tube;
}information;

information infoVariable;

#endif //HW1_TUBE_H
