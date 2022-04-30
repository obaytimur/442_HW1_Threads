//
// Created by ogi on 29.04.2022.
//
#include "tube.h"

void addAtom(tube* Tube, atom Atom) {
    pthread_mutex_lock(&(Tube->tubeMutex));
    Tube->array[Tube->front + Tube->totalAtoms] = Atom;
    pthread_mutex_lock(&qriticalMutex);
    Tube->totalAtoms++;
    Tube->value= Tube->value + Atom.typeValue;
    if(Tube->value == 1200 || Tube->value == 2010 || Tube->value == 12000 || Tube->value == 10300){
        switch (Tube->value) {
            case 1200:
                printf("Molecule H2O is generated in Tube %d. \n", Tube->tubeID);
                removeMol(Tube);
            case 2010:
                printf("Molecule CO2 is generated in Tube %d. \n", Tube->tubeID);
                removeMol(Tube);
            case 12000:
                printf("Molecule NO2 is generated in Tube %d. \n", Tube->tubeID);
                removeMol(Tube);
            case 10300:
                printf("Molecule NH3 is generated in Tube %d. \n", Tube->tubeID);
                removeMol(Tube);
        }
    }
    pthread_mutex_unlock(&qriticalMutex);
    if (pthread_cond_signal(&Tube->tubeCondition) != 0){
        printf("Condition Signal could not be generated!");
    }
    pthread_mutex_unlock(&Tube->tubeMutex);
}

int getTimeStamp(tube Tube){
    return Tube.array[0].atomID;
}

void* removeMol(void* Tube){
    pthread_mutex_lock(&(((tube*)Tube)->tubeMutex));
    while(((tube*)Tube)->totalAtoms == 0){
        pthread_cond_wait(&(((tube*)Tube)->tubeCondition), &(((tube*)Tube)->tubeMutex));
    }
    pthread_mutex_unlock(&(((tube*)Tube)->tubeMutex));

    pthread_mutex_lock(&(((tube*)Tube)->tubeMutex));
    for(int i=0; i < ((tube*)Tube)->totalAtoms; i++){
        ((tube*)Tube)->array = (atom*) malloc(10);
    }
    ((tube*)Tube)->value = 0;
    ((tube*)Tube)->front = 0;
    ((tube*)Tube)->totalAtoms = 0;
    pthread_mutex_unlock(&(((tube*)Tube)->tubeMutex));
}

void* infoPrint(){
    while(1){
        pthread_mutex_lock(&sleepMutex);
        printf("%d is created in tube %d. \n", infoVariable.Tube.moleculeTYPE, infoVariable.tubeID);
        pthread_mutex_unlock(&informationMutex);
        pthread_mutex_lock(&removalMutex);

    }
}

void setMolType(int type){

}

int getMolType(tube* Tube){
    return Tube->moleculeTYPE;
}


