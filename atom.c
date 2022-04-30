//
// Created by ogi on 29.04.2022.
//
#include "atom.h"

void setID(atom* Atom, int id){ // set atom ID
    Atom->atomID = id;
}

int getID(atom Atom){ // get atom ID
    return Atom.atomID;
}

void setValue(atom* Atom, int value){ // set atom value
    Atom->typeValue = value;
}

int getValue(atom* Atom){ // get atom value
    return Atom->typeValue;
}

void setType(atom* Atom, char type){ // set atom type
    Atom->atomTYPE = type;
}

char getType(atom Atom){ // get atom type
    return Atom.atomTYPE;
}