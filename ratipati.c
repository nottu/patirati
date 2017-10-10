//
// Created by Javier Peralta on 10/9/17.
//

#include "ratipati.h"

void findLeftPoint(PGM image, int x, int y, int* xl, int* yl){
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
}
void findRightPoint(PGM image, int x, int y, int* xr, int* yr){
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
byte getFirstJointInfo(PatiRati *rati, PGM image){
  int x, y; //coords for first white
  findFirstWhite(image, &x, &y);
  if( !x ){
    printf("ERROR");
    return 1;
  }
  int xl, yl;
  findLeftPoint(image, x, y, &xl, &yl);
  int xr, yr;
  findRightPoint(image, x, y, &xr, &yr);

  return 0;
}