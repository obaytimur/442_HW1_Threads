//
// Created by ogi on 29.04.2022.
//

#ifndef HW1_TUBE_H
#define HW1_TUBE_H
#include "atom.h"
#include <pthread.h>
#include <unistd.h>


typedef struct tube {
    atom* array;
    int front;
    int value;
    int totalAtoms;
    int tubeID;
    int tubeTS; // time stamp (ID of the atom spilled first)
    int moleculeTYPE; // 1:H2O, 2:CO2, 3: NO2, 4: NH3
    pthread_mutex_t tubeMutex;
    pthread_mutex_t carbonMutex;
    pthread_mutex_t hidroMutex;
    pthread_mutex_t oxiMutex;
    pthread_mutex_t nitroMutex;
    pthread_cond_t tubeCondition;

}tube;

typedef struct Information {
    int tubeID;
    tube Tube;
}information;

void addAtom(tube* Tube, atom Atom);
int getTimeStamp(tube Tube);
void* removeMol(void* Tube);
void* infoPrint();
void setMolType(int type);
int getMolType(tube* Tube);
int removedMol(tube* Tube);
pthread_mutex_t qriticalMutex;
pthread_mutex_t sleepMutex;
pthread_mutex_t informationMutex;
pthread_mutex_t removalMutex;
information infoVariable;

#endif //HW1_TUBE_H
