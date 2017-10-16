//
// Created by jperalta on 10/16/17.
// Basic line and point definitions

#ifndef PATIRATI_LINEPOINT_H
#define PATIRATI_LINEPOINT_H

typedef struct {
  int x, y;
} ipoint; //integer point

typedef struct {
  int x, y;
  int xlen, ylen; //
  double slope;
  double angle; //arctan slope
  double length;
  int thickness;
} line;

ipoint newPoint(int x, int y);

#endif //PATIRATI_LINEPOINT_H
