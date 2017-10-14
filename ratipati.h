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

typedef struct {
  int vxMin, vxMax;
  int vyMin, vyMax;
  int vxEMin, vxEMax;
  int vyEMin, vyEMax;
  int thMin, thMax;
} lineFreedom;

//returns suggestions for first joint (x, y, len, angle, slope)
line getFirstJointInfo(PGM image);
void genLinesWithVariance(line orig, lineFreedom freedom,  line* lines,  int nLines);
void rati(const char *imgName, const char* outName);
void findFirstLine(PGM image, PGM aprox, line sug);
lineFreedom newLineFreedom(int x, int y, int xE, int yE);
line* fastBadApproach(PGM image);

#endif //PATIRATI_RATIPATI_H
