//
// Created by Javier Peralta on 10/9/17.
//

#ifndef PATIRATI_RATIPATI_H
#define PATIRATI_RATIPATI_H

#include "pgm.h"

typedef struct {
  int x, y; //start point
  int lens[8]; //min and max len
  double angles[8]; //min and max angle
  double slopes[8]; //same info, easier to use
} PatiRati;

//returns suggestions for first joint (x, y, len, angle, slope)
byte getFirstJointInfo(PatiRati *rati, PGM image);

#endif //PATIRATI_RATIPATI_H
