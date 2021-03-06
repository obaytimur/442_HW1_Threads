//
// Created by ogi on 29.04.2022.
//

#ifndef HW1_ATOM_H
#define HW1_ATOM_H
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// The functions of the atom struct are described in the source codes

typedef struct atom {
    int atomID;
    char atomTYPE; // C, H, O or N
    int typeValue;
    double atomGenarationRate;
}atom;

void setID(atom* Atom, int id);
int getID(atom Atom);
void setValue(atom* Atom, int value);
int getValue(atom* Atom);
void setType(atom* Atom, char type);
char getType(atom Atom);
#endif //HW1_ATOM_H
