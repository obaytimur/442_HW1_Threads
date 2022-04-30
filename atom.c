//
// Created by ogi on 29.04.2022.
//
#include "atom.h"

void setID(atom* Atom, int id){
    Atom->atomID = id;
}

int getID(atom Atom){
    return Atom.atomID;
}

void setValue(atom* Atom, int value){
    Atom->typeValue = value;
}

int getValue(atom* Atom){
    return Atom->typeValue;
}

void setType(atom* Atom, char type){
    Atom->atomTYPE = type;
}

char getType(atom Atom){
    return Atom.atomTYPE;
}

void setGenerationTime(atom* Atom, double lambda){
    double var;
    var = rand() / (RAND_MAX + 1.0);
    Atom->atomGenarationRate = -(log(1-var) / lambda);
}
double getGenerationTime(atom Atom){
    return Atom.atomGenarationRate;
}