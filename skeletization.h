//
// Created by jperalta on 10/13/17.
// Implemetation of Zhang-Suen_thinning algorithm
// Only "public functions" are here

#ifndef PATIRATI_SKELETIZATION_H
#define PATIRATI_SKELETIZATION_H

#include "pgm.h"
#include "linepoint.h"

#define SKELETIZATION_MAX_ITER 100
typedef struct pntNode{
  ipoint point;
  struct pntNode *next;
  byte val;
} pointList;
typedef byte (*shouldKeepFunc)(PGM, ipoint, byte, byte);
//node point stuff
pointList* newNode(ipoint point);
pointList* newNodeC(int x, int y);
//skeletize helper functions
//A(P1) and B(P1)
pointList* countTransitionsAndNeighbours(PGM img, ipoint point, byte *white, byte *transitions);
byte shouldKeepFirstPass(PGM img, ipoint p, byte white, byte transitions);
byte shouldKeepSecondPass(PGM img, ipoint p, byte white, byte transitions);
int filterPass(PGM *img, shouldKeepFunc func);//single iteration of Zhan-Suen
//
void skeletize(PGM *image);

#endif //PATIRATI_SKELETIZATION_H
