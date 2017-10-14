//
// Created by Javier Peralta on 10/7/17.
//

#ifndef PATIRATI_PGM_H
#define PATIRATI_PGM_H
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define defaultType "P2"
#define defaultMaxGrey 255

#ifndef BYTE_TYPE
#define BYTE_TYPE
typedef unsigned short byte;
#endif

typedef struct {
  char fileType[3];//2 chars of file type
  byte **data;
  int w, h; //columns (w), rows(h)
  int x0, y0, xn, yn; //start and end of image, (x, y) coordinates
  byte maxGreyVal;
} PGM;

typedef struct {
  int x, y;
  int xlen, ylen; //
  double slope;
  double angle; //arctan slope
  double length;
  int thickness;
} line;
byte** allocImgData(int w, int h);
line newLine(int x, int y, int xlen, int ylen, int thickness);
void setLengths(line *l, int xlen, int ylen);
PGM  readImage(const char* imageName);
void cropImage(PGM *image);
void freeImage(PGM *image);
void freeImageData(byte** imgData);
void printImage(PGM image, const char* name);
void printImageCrop(PGM image, const char* name);
void drawLine(PGM image, line l);
PGM newImage(int w, int h);
PGM newImageFromImg(PGM img);
void getLineLens(double slope, double l, int *xl, int *yl);
double compareImg(PGM image, PGM aprox);
#endif //PATIRATI_PGM_H
