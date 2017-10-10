//
// Created by Javier Peralta on 10/7/17.
//

#ifndef PATIRATI_PGM_H
#define PATIRATI_PGM_H
#include <stdio.h>

typedef unsigned short byte;

typedef struct {
  char fileType[3];//2 chars of file type
  byte **data;
  int w, h; //columns (w), rows(h)
  int x0, y0, xn, yn; //start and end of image, (x, y) coordinates
  int maxGreyVal;
} PGM;

PGM  readImage(const char* imageName);
void cropImage(PGM *image);
void freeImage(PGM *image);
void printImage(PGM image, char* name);
void printImageCrop(PGM image, char* name);
#endif //PATIRATI_PGM_H
