//
// Created by Javier Peralta on 10/9/17.
//

#include <stdlib.h>
#include "ratipati.h"

//return slope
double findLeftPoint(PGM image, int x, int y, int* xl, int* yl){
  *xl = x, *yl = y;
  int maxYdif = 30, ydif = 0;
  for (int i = x - 1; i >= 0; --i) { //look left
    ydif = 0;
    for (int j = *yl; j < image.yn; ++j) {
      if(image.data[j][i]) {
        *xl = i; *yl = j;
        break;
      }
      if(++ydif >= maxYdif){
        i = -1; //break of outer loop
        break;
      }
    }
  }
  return (double)(y - *yl)/(double)(x - *xl);
}
//return slope
double findRightPoint(PGM image, int x, int y, int* xr, int* yr){
  *xr = x, *yr = y;
  int maxYdif = 30, ydif = 0;
  for (int i = x; i < image.xn; ++i) { //look right
    ydif = 0;
    for (int j = *yr; j < image.yn; ++j) {
      if(image.data[j][i]) {
        *xr = i; *yr = j;
        break;
      }
      if(++ydif >= maxYdif){
        i = image.xn; //break of outer loop
        break;
      }
    }
  }
  return (double)(y - *yr)/(double)(x - *xr);
}
void findFirstWhite(PGM image, int *x, int *y){
  *x = 0, *y = 0; //coords for first white
  for (int i = image.y0; i < image.yn; ++i) { //should loop only once
    for (int j = image.x0; j < image.xn; ++j) {
      if(image.data[i][j]) {
        *x = j; *y = i;
        i = image.yn; //break of outer loop
        break;
      }
    }
  }
}

line getFirstJointInfo(PatiRati *rati, PGM image){
  int x, y; //coords for first white
  findFirstWhite(image, &x, &y);
  if( !x ){
    printf("ERROR");
    exit(1);
  }
  int xl, yl;
  findLeftPoint(image, x, y, &xl, &yl);
  line l2 = newLine(x, y, xl-x, yl-y, 1);
  return l2;
}

void findFirstLine(PGM image, PGM aprox, line sug){
//  +- 10 en X y Y, slope +- 0.2
  int sizeGen = 10;
  lineFreedom freedom = newLineFreedom(10, 10, 10, 10);
  line *lines = (line*)malloc(sizeof(line) * sizeGen);
  genLinesWithVariance(sug, freedom, lines, sizeGen);
//  PGM img2 = newImage(image.w, image.h);
//  drawLine(img2, sug);
//  printImage(img2, "test.pgm");
}
lineFreedom newLineFreedom(int x, int y, int xE, int yE){
  lineFreedom freedom;
  freedom.vxMin = -x; freedom.vxMax = x;
  freedom.vyMin = -y; freedom.vyMax = y;
  freedom.vxEMin = -xE; freedom.vxEMax = xE;
  freedom.vyEMin = -yE; freedom.vyEMax = yE;
  return freedom;
}
void genLinesWithVariance(line orig, lineFreedom freedom,  line* lines,  int nLines){

}
void rati(){}