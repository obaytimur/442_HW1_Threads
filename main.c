#include "atom.h"
#include "tube.h"
#include <getopt.h>
#include <stdio.h>
#include <stdbool.h>

int argGetter(int argc, char **argv, int* carbonNumber, int* hidrogenNumber, int* oxigenNumber, int* nitrogenNumber, double* genTime);

void sleeper(double lambda);

int main(int argc, char **argv)
{

    bool cond;
    bool insertSuitable = false;
    bool isFound = false;
    char atomGenType[4] = {'C', 'H', 'O', 'N'};
    int atomGenNumber[4] = {20,20,20,20};
    int atomTypeValues[4] = {10, 100, 1000, 10000};

    int possibleCarbon [] = {1000, 2000};
    int possibleHidro [] = {100, 200, 1000, 1100, 10000, 10100, 10200 };
    int possibleOxigen [] = {10, 100, 200, 1010, 10000, 11000};
    int possibleNitrogen [] = {100, 200, 300, 1000, 2000};
    int* possibleValues[4]= {possibleCarbon, possibleHidro, possibleOxigen, possibleNitrogen} ;

    char choice;
    int tubeNumber = 3;
    int atomPerTube = 10;
    int molNumbers[4] = {1,2,3,4};
    double generationTime = 100;
    int mainThreadCheck = 0;
    int tubeThreadsCheck = 0;
    int totalAtomNumber = atomGenNumber[0]+atomGenNumber[1]+atomGenNumber[2]+atomGenNumber[3];
    int timeStamp;
    int insertInd;

    pthread_t mainThread;
    pthread_t tubeThreads[tubeNumber];
    atom Atoms[atomGenNumber[0]+atomGenNumber[1]+atomGenNumber[2]+atomGenNumber[3]];
    tube Tubes[tubeNumber];

    int terminalOptions =argGetter(argc, argv, &atomGenNumber[0], &atomGenNumber[1], &atomGenNumber[2], &atomGenNumber[3], &generationTime);
    if(terminalOptions == -1){
        return -1;
    }

    for(int i=0; i < tubeNumber; i++){
        Tubes[i].array = (atom*) malloc(sizeof (atomPerTube));
        Tubes[i].front = 0;
        Tubes[i].totalAtoms = 0;
        Tubes[i].tubeID = i+1;
        Tubes[i].moleculeTYPE = 0;
        Tubes[i].value = 0;
        pthread_mutex_init(&(Tubes[i].tubeMutex),0);
        pthread_mutex_init(&(Tubes[i].carbonMutex),0);
        pthread_mutex_init(&(Tubes[i].hidroMutex),0);
        pthread_mutex_init(&(Tubes[i].oxiMutex),0);
        pthread_mutex_init(&(Tubes[i].nitroMutex),0);
        pthread_cond_init(&(Tubes[i].tubeCondition),0);
    }

    pthread_mutex_init(&qriticalMutex,0);
    pthread_mutex_init(&sleepMutex,0);
    pthread_mutex_init(&informationMutex,0);
    pthread_mutex_lock(&sleepMutex);

    mainThreadCheck = pthread_create(&mainThread, NULL, infoPrint, NULL);
    if(mainThreadCheck < 0){
        printf("Main thread creation error!");
        exit(-1);
    }
    for(int i=0; i<tubeNumber; i++){
        tubeThreadsCheck = pthread_create(&tubeThreads[i], NULL, removeMol, (void *) &Tubes[i]);
        if(tubeThreadsCheck < 0){
            printf("Tube thread %d creation error!", i);
            exit(-1);
        }
    }
    for(int i=0; i < totalAtomNumber; i++){
        setID(&Atoms[i],i);
        cond = true;
        while(cond){
            choice = rand() % 4;
            if(atomGenNumber[choice]>0){
                setType(&Atoms[i], atomGenType[choice]);
                setValue(&Atoms[i], atomTypeValues[choice]);
                atomGenType[choice]--;
                cond = false;
            }
        }
        sleeper(generationTime);
        printf("Atom %c with ID: %d is generated. \n", getType(Atoms[i]), getID(Atoms[i]));
        pthread_mutex_lock(&qriticalMutex);
        if(Tubes[0].totalAtoms == 0 && Tubes[1].totalAtoms == 0 && Tubes[2].totalAtoms == 0){
            addAtom(&Tubes[0], Atoms[i]);
        } else {
            insertSuitable = false;
            timeStamp = 1000;
            insertInd = 0;
            insertSuitable = false;
            isFound = false;
            switch (getType(Atoms[i])) {
                case 'C':
                    for(int tubeInd = 0; tubeInd < tubeNumber; tubeInd++){
                        for(int valueInd = 0; valueInd < 2; valueInd++){
                            if( (Tubes[tubeInd].value == possibleCarbon[valueInd]) && (Tubes[tubeInd].array[0].atomID << timeStamp) ){
                                timeStamp = Tubes[tubeInd].array[0].atomID;
                                insertInd = tubeInd;
                                insertSuitable = true;
                            }
                        }
                    }
                    if(insertSuitable){
                        addAtom(&Tubes[insertInd], Atoms[i]);
                    }
                    else{
                        for(int tubeEmptyInd=0; tubeEmptyInd < tubeNumber; tubeEmptyInd++){
                            if(Tubes[tubeEmptyInd].value == 0){
                                addAtom(&Tubes[tubeEmptyInd], Atoms[i]);
                                isFound = true;
                                break;
                            }
                        }
                        if(!isFound){
                            printf("Atom %c with ID: %d is wasted. \n", getType(Atoms[i]), getID(Atoms[i]));
                        }
                    }
                case 'H':
                    for(int tubeInd = 0; tubeInd < tubeNumber; tubeInd++){
                        for(int valueInd = 0; valueInd < 7; valueInd++){
                            if( (Tubes[tubeInd].value == possibleNitrogen[valueInd]) && (Tubes[tubeInd].array[0].atomID << timeStamp) ){
                                timeStamp = Tubes[tubeInd].array[0].atomID;
                                insertInd = tubeInd;
                                insertSuitable = true;
                            }
                        }
                    }
                    if(insertSuitable){
                        addAtom(&Tubes[insertInd], Atoms[i]);
                    }
                    else{
                        for(int tubeEmptyInd=0; tubeEmptyInd < tubeNumber; tubeEmptyInd++){
                            if(Tubes[tubeEmptyInd].value == 0){
                                addAtom(&Tubes[tubeEmptyInd], Atoms[i]);
                                isFound = true;
                                break;
                            }
                        }
                        if(!isFound){
                            printf("Atom %c with ID: %d is wasted. \n", getType(Atoms[i]), getID(Atoms[i]));
                        }
                    }
                case 'O':
                    for(int tubeInd = 0; tubeInd < tubeNumber; tubeInd++){
                        for(int valueInd = 0; valueInd < 6; valueInd++){
                            if( (Tubes[tubeInd].value == possibleOxigen[valueInd]) && (Tubes[tubeInd].array[0].atomID << timeStamp) ){
                                timeStamp = Tubes[tubeInd].array[0].atomID;
                                insertInd = tubeInd;
                                insertSuitable = true;
                            }
                        }
                    }
                    if(insertSuitable){
                        addAtom(&Tubes[insertInd], Atoms[i]);
                    }
                    else{
                        for(int tubeEmptyInd=0; tubeEmptyInd < tubeNumber; tubeEmptyInd++){
                            if(Tubes[tubeEmptyInd].value == 0){
                                addAtom(&Tubes[tubeEmptyInd], Atoms[i]);
                                isFound = true;
                                break;
                            }
                        }
                        if(!isFound){
                            printf("Atom %c with ID: %d is wasted. \n", getType(Atoms[i]), getID(Atoms[i]));
                        }
                    }
                case 'N':
                    for(int tubeInd = 0; tubeInd < tubeNumber; tubeInd++){
                        for(int valueInd = 0; valueInd < 5; valueInd++){
                            if( (Tubes[tubeInd].value == possibleNitrogen[valueInd]) && (Tubes[tubeInd].array[0].atomID << timeStamp) ){
                                timeStamp = Tubes[tubeInd].array[0].atomID;
                                insertInd = tubeInd;
                                insertSuitable = true;
                            }
                        }
                    }
                    if(insertSuitable){
                        addAtom(&Tubes[insertInd], Atoms[i]);
                    }
                    else{
                        for(int tubeEmptyInd=0; tubeEmptyInd < tubeNumber; tubeEmptyInd++){
                            if(Tubes[tubeEmptyInd].value == 0){
                                addAtom(&Tubes[tubeEmptyInd], Atoms[i]);
                                isFound = true;
                                break;
                            }
                        }
                        if(!isFound){
                            printf("Atom %c with ID: %d is wasted. \n", getType(Atoms[i]), getID(Atoms[i]));
                        }
                    }
            }
            pthread_mutex_unlock(&qriticalMutex);
        }
    }

    if(pthread_join(mainThread, 0) < 0){
        perror("Error occurred while joining Main Thread!");
        exit(-1);
    }
    for(int i = 0; i < tubeNumber; i++){
        pthread_mutex_destroy(&(Tubes[i].tubeMutex));
    }
    pthread_mutex_destroy(&qriticalMutex);
    pthread_mutex_destroy(&sleepMutex);
    pthread_mutex_destroy(&informationMutex);
    pthread_mutex_destroy(&sleepMutex);
}


int argGetter(int argc, char **argv, int* carbonNumber, int* hidrogenNumber, int* oxigenNumber, int* nitrogenNumber, double* genTime){
    int condition;
    int inputArg;
    while((condition = getopt (argc, argv, "chong")) != -1)
        switch (condition)
        {
            case 'c':
                if(argv[inputArg] != NULL){
                    *carbonNumber = atof(argv[inputArg]);
                    break;
                }
                else{
                    break;
                }
            case 'h':
                if(argv[inputArg] != NULL){
                    *hidrogenNumber = atof(argv[inputArg]);
                    break;
                }
                else{
                    break;
                }
            case 'o':
                if(argv[inputArg] != NULL){
                    *oxigenNumber = atof(argv[inputArg]);
                    break;
                }
                else{
                    break;
                }
            case 'n':
                if(argv[inputArg] != NULL){
                    *nitrogenNumber = atof(argv[inputArg]);
                    break;
                }
                else{
                    break;
                }
            case 'g':
                if(argv[inputArg] != NULL){
                    *genTime = atof(argv[inputArg]);
                    break;
                }
                else{
                    break;
                }
            case 'y':
                printf("Available Commands Are: \n c: number of carbons \n h: number of hidrogens \n o: number of oxigens \n n: number of nitrogens \n");
                return -1;
        }
    printf("Reaction Simulator will run with the given parameters: \n Total number of generated carbons will be: %d \n "
           "Total number of generated hidrogens will be: %d \n Total number of generated oxigens will be: %d \n "
           "Total number of generated nitrogens will be: %d \n Generation time will be: %f", *carbonNumber, *hidrogenNumber, *oxigenNumber, *nitrogenNumber, *genTime);
    return 0;
}

void sleeper(double lambda){
    double variable;
    variable = rand() / (RAND_MAX + 1.0);
    sleep(((-(log(1-variable)) / lambda)));
}







