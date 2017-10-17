//
// Created by Javier Peralta on 10/9/17.
//

#ifndef PATIRATI_RATIPATI_H
#define PATIRATI_RATIPATI_H

#include "pgm.h"
#include "linepoint.h"

// simple struct to contain "freedom" of positions
typedef struct {
  byte xVar, yVar, thVar;
} pointFreedom;
//init line freedom with amount of freedom, i.e. from -value to value
pointFreedom newPointFreedom(byte x, byte y, byte th);
//
typedef struct {
  ipoint* points;
  byte thickness;
  double grade;
} pointsGrade;

line newLine(int x, int y, int xlen, int ylen, int thickness);
line newLineFromPoints(ipoint p1, ipoint p2);
void drawLine(PGM image, line l);

int findPointInRow(PGM *img, int y);
ipoint* findInflectionPoints(PGM img, byte nPoints);

//-+dx, +-dy
int cmpGradeFunc(const void *a, const void *b);
double gradePoints(PGM *img, ipoint *p, byte thVar, byte thickness, int n);
ipoint genPointWithVariance(ipoint p, int dx, int dy);
void genPointPopulation(ipoint *p, pointFreedom *var, int n, int size, ipoint** pop);
void testAndSortPointPop(PGM *img, ipoint** pop, byte* thicknesses, pointFreedom *var, int n, int size);
void updatePointAndVariance(ipoint *p, pointFreedom *var, ipoint **pop, int n, int size);
line* findBestLines(PGM *img, ipoint* points, byte n);

line* rati(PGM img, PGM img0);

#endif //PATIRATI_RATIPATI_H
