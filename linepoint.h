//
// Created by jperalta on 10/16/17.
// Basic line and point definitions

#ifndef PATIRATI_LINEPOINT_H
#define PATIRATI_LINEPOINT_H

#ifndef BYTE_TYPE
#define BYTE_TYPE
typedef unsigned short byte;
#endif

typedef struct {
  int x, y;
} ipoint; //integer point

typedef struct {
  int x, y;
  int xlen, ylen; //
  double slope;
  double angle; //arctan slope
  double length;
  byte thickness;
} line;

ipoint newPoint(int x, int y);

#endif //PATIRATI_LINEPOINT_H
