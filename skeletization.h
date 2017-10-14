//
// Created by jperalta on 10/13/17.
// Implemetation of Zhang-Suen_thinning algorithm
// Only "public functions" are here

#ifndef PATIRATI_SKELETIZATION_H
#define PATIRATI_SKELETIZATION_H

#include "pgm.h"

typedef struct {
  int x, y;
} ipoint; //integer point

void skeletize(PGM *image);

#endif //PATIRATI_SKELETIZATION_H
