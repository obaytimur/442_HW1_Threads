#include "atom.h"
#include "tube.h"
#include <getopt.h>
#include <stdio.h>
#include <stdbool.h>

// Hocam, I was getting multiple definition error on the mutexes that are created in the tube.h. Even though, I spent
// whole saturday to resolve it I could not. However, I believe that my algorithm, may not be a successfully working
// executable, works a pseudo algorithm that can be found in the zip file.


// function to get parameters from terminal
int argGetter(int argc, char **argv, int* carbonNumber, int* hidrogenNumber, int* oxigenNumber, int* nitrogenNumber, double* genTime);

// function that sleeps for a random time
void sleeper(double lambda);

int main(int argc, char **argv)
{

    bool cond;                      // boolean type initializations
    bool insertSuitable = false;
    bool isFound = false;

    char atomGenType[4] = {'C', 'H', 'O', 'N'};     // atom types
    int atomGenNumber[4] = {20,20,20,20};           // atom generation numbers, default 20
    int atomTypeValues[4] = {10, 100, 1000, 10000}; // value of each atom

    int possibleCarbon [] = {1000, 2000};       // possible values a tube can take for the respected atom
    int possibleHidro [] = {100, 200, 1000, 1100, 10000, 10100, 10200 };
    int possibleOxigen [] = {10, 100, 200, 1010, 10000, 11000};
    int possibleNitrogen [] = {100, 200, 300, 1000, 2000};

    char choice;            // other initializations used in the code
    int tubeNumber = 3;
    double generationTime = 100;
    int atomPerTube = 10;
    int mainThreadCheck = 0;
    int tubeThreadsCheck = 0;
    int totalAtomNumber = atomGenNumber[0]+atomGenNumber[1]+atomGenNumber[2]+atomGenNumber[3];
    int timeStamp;
    int insertInd;

    pthread_t mainThread;   // main thread
    pthread_t tubeThreads[tubeNumber]; // tube threads
    atom Atoms[atomGenNumber[0]+atomGenNumber[1]+atomGenNumber[2]+atomGenNumber[3]]; // array to hold all produced atoms
    tube Tubes[tubeNumber];         // array to hold tubes

    // call for the terminal option function
    int terminalOptions =argGetter(argc, argv, &atomGenNumber[0], &atomGenNumber[1], &atomGenNumber[2], &atomGenNumber[3], &generationTime);
    if(terminalOptions == -1){
        return -1;
    }

    //initialize tubes and their special parameters
    for(int i=0; i < tubeNumber; i++){
        Tubes[i].array = (atom*) malloc(atomPerTube);
        Tubes[i].front = 0;
        Tubes[i].totalAtoms = 0;
        Tubes[i].tubeID = i;
        Tubes[i].moleculeTYPE = 0;
        Tubes[i].value = 0;
        pthread_mutex_init(&(Tubes[i].tubeMutex),0);
        pthread_cond_init(&(Tubes[i].tubeCondition),0);
    }

    // initialize global tube parameters
    pthread_mutex_init(&qriticalMutex,0);
    pthread_mutex_init(&sleepMutex,0);
    pthread_mutex_init(&informationMutex,0);
    pthread_mutex_lock(&sleepMutex);

    // check main thread, if it cannot create, exits
    mainThreadCheck = pthread_create(&mainThread, NULL, infoPrint, NULL);
    if(mainThreadCheck < 0){
        printf("Main thread creation error!");
        exit(-1);
    }

    // check for tube threads, if it cannot create, exits
    for(int i=0; i<tubeNumber; i++){
        tubeThreadsCheck = pthread_create(&tubeThreads[i], NULL, removeMol, (void *) &Tubes[i]);
        if(tubeThreadsCheck < 0){
            printf("Tube thread %d creation error!", i);
            exit(-1);
        }
    }

    // atom generating and spilling loop
    for(int i=0; i < totalAtomNumber; i++){
        setID(&Atoms[i],i); //start by assigning an ID to an atom
        cond = true; // adjust the boolean used to select atom type
        while(cond){
            choice = rand() % 4; // choose a number between 0-3 resulting the atom type
            if(atomGenNumber[choice]>0){ // if that atom type can be created, it assigns its type and value
                setType(&Atoms[i], atomGenType[choice]);
                setValue(&Atoms[i], atomTypeValues[choice]);
                atomGenType[choice]--;
                cond = false; // changes condition to false so that exits while loop and continue with the splling
            }
        }
        sleeper(generationTime); // sleeps the main for a random amount dependent on generation rate
        printf("Atom %c with ID: %d is generated. \n", getType(Atoms[i]), getID(Atoms[i]));
        pthread_mutex_lock(&qriticalMutex); // enters CS to spill into tube

        // if all the tubes are empty, spills in the first one;
        if(Tubes[0].totalAtoms == 0 && Tubes[1].totalAtoms == 0 && Tubes[2].totalAtoms == 0){
            addAtom(&Tubes[0], Atoms[i]);
        } else { // for the rest takes this else
            insertSuitable = false;
            timeStamp = 1000;
            insertInd = 0;
            insertSuitable = false;
            isFound = false;
            switch (getType(Atoms[i])) { // look the atom type
                case 'C': // the logic in each type is the same, therefore not going to repeat comments for the rest cases

            // in these two loops, check for the possible tubes for the specified atom type can spill into that has atoms in it
            // While doing it, check for the timestamps of the first atom to select the smallest time
            // if it finds, change insertSuitable boolean take true to spill that atom
                    for(int tubeInd = 0; tubeInd < tubeNumber; tubeInd++){
                        for(int valueInd = 0; valueInd < 2; valueInd++){
                            if( (Tubes[tubeInd].value == possibleCarbon[valueInd]) && (Tubes[tubeInd].array[0].atomID << timeStamp) ){
                                timeStamp = Tubes[tubeInd].array[0].atomID;
                                insertInd = tubeInd;
                                insertSuitable = true;
                            }
                        }
                    }
                    if(insertSuitable){ // if a suitable tube to be spilled found, spill it
                        addAtom(&Tubes[insertInd], Atoms[i]);
                    }
                    else{ // check if there are empty tubes to spill the atom
                        for(int tubeEmptyInd=0; tubeEmptyInd < tubeNumber; tubeEmptyInd++){
                            if(Tubes[tubeEmptyInd].value == 0){ // if there is an empty tube insert atom
                                addAtom(&Tubes[tubeEmptyInd], Atoms[i]);
                                isFound = true;
                                break;
                            }
                        }
                        if(!isFound){ // if there is not, waste the atom
                            printf("Atom %c with ID: %d is wasted. \n", getType(Atoms[i]), getID(Atoms[i]));
                        }
                    }
                case 'H':
                    for(int tubeInd = 0; tubeInd < tubeNumber; tubeInd++){
                        for(int valueInd = 0; valueInd < 7; valueInd++){
                            if( (Tubes[tubeInd].value == possibleHidro[valueInd]) && (Tubes[tubeInd].array[0].atomID << timeStamp) ){
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
            // exits CS since atom is put in its place
            pthread_mutex_unlock(&qriticalMutex);
        }
    }
    // wait for the main thread
    if(pthread_join(mainThread, 0) < 0){
        perror("Error occurred while joining Main Thread!");
        exit(-1);
    }

    // destroy tube mutexes
    for(int i = 0; i < tubeNumber; i++){
        pthread_mutex_destroy(&(Tubes[i].tubeMutex));
    }
    // destroy global mutexes
    pthread_mutex_destroy(&qriticalMutex);
    pthread_mutex_destroy(&sleepMutex);
    pthread_mutex_destroy(&informationMutex);
    pthread_mutex_destroy(&sleepMutex);
}


int argGetter(int argc, char **argv, int* carbonNumber, int* hidrogenNumber, int* oxigenNumber, int* nitrogenNumber, double* genTime){
    int condition;
    int inputArg;
    while((condition = getopt (argc, argv, "chong")) != -1)
        switch (condition) // looks terminal options, if there are none run with default
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
    variable = rand() / (RAND_MAX + 1.0); // random number between 0-1, named as x in the pdf
    sleep(((-(log(1-variable)) / lambda))); // sleep as long as calculated
}







