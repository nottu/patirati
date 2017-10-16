//
// Created by Javier Peralta on 10/9/17.
//

#ifndef PATIRATI_RATIPATI_H
#define PATIRATI_RATIPATI_H

#include "pgm.h"

#ifndef IPOINT
#define IPOINT
typedef struct {
  int x, y;
} ipoint; //integer point
#endif

typedef struct {
  int x, y;
  int xlen, ylen; //
  double slope;
  double angle; //arctan slope
  double length;
  int thickness;
} line;

line newLine(int x, int y, int xlen, int ylen, int thickness);
line newLineFromPoints(ipoint p1, ipoint p2);
void setLengths(line *l, int xlen, int ylen);
void drawLine(PGM image, line l);

int findPointInRow(PGM *img, int y);
ipoint __newPoint(int x, int y);//same as in skeletization.h
ipoint* findInflectionPoints(PGM img, int nPoints);

line* rati(PGM img);

#endif //PATIRATI_RATIPATI_H
