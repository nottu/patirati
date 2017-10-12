//
// Created by Javier Peralta on 10/7/17.
//

#include <stdlib.h>
#include "pgm.h"

//private function...
byte** allocImgData(int w, int h){
  byte **data    = (byte**)malloc(sizeof(byte*) * h);
  data[0] =  (byte*)malloc(sizeof(byte) * h * w);
  for (int i = 1; i < h; i++){
    data[i] = data[i-1] + w;
  }
  return data;
}

PGM readImage(const char* imageName){
  FILE *f = fopen(imageName, "r");
  PGM img;
  if(f == NULL){
    img.data = NULL;
    return img;
  }
  fscanf(f, "%s %d %d %d", img.fileType, &(img.w), &(img.h), &(img.maxGreyVal));
  //init values
  img.x0 = 0; img.y0 = 0; img.yn=img.h; img.xn = img.w;
  //images are ~400k we can still have'em in an simple arr
  img.data = allocImgData(img.w, img.h);

  for (int i = 0; i < img.h; ++i) {
    for (int j = 0; j < img.w; ++j) {
      byte b;
      fscanf(f, "%hd",&b);
      img.data[i][j] = b;
    }
  }
  fclose(f);
  return img;
}
void cropImage(PGM *image){
  image->x0 = image->w; image->y0 = image->h; image->yn = 0; image->xn = 0;
  for (int i = 0; i < image->h; ++i) {
    for (int j = 0; j < image->w; ++j) {
      byte d = image->data[i][j];
      if(d != 0){
        image->x0 = j < image->x0 ? j : image->x0;
        image->y0 = i < image->y0 ? i : image->y0;
        image->xn = j > image->xn ? j : image->xn;
        image->yn = i > image->yn ? i : image->yn;
      }
    }
  }
}
void printImage(PGM image, char* name){
  FILE *f = fopen(name, "w");
  fprintf(f, "%s\n%d %d\n%d\n", image.fileType, image.w, image.h, image.maxGreyVal);
  for (int i = 0; i < image.h; ++i) {
    for (int j = 0; j < image.w; ++j) {
      fprintf(f, "%hd ", image.data[i][j]);
    }
  }
  fclose(f);
}
void printImageCrop(PGM image, char* name){
  FILE *f = fopen(name, "w");
  fprintf(f, "%s\n%d %d\n%d\n", image.fileType, image.xn - image.x0, image.yn - image.y0, image.maxGreyVal);
  for (int i = image.y0; i <image.yn; ++i) {
    for (int j = image.x0; j < image.xn; ++j) {
      fprintf(f, "%hd ", image.data[i][j]);
    }
  }
  fclose(f);
}
void freeImage(PGM *image){
  free(image->data[0]);
  free(image->data);
  image->w = 0;
  image->h = 0;
  image->maxGreyVal = 0;
  image->x0 = 0;
  image->y0 = 0;
  image->xn = 0;
  image->yn = 0;
}

//void fillSquare(PGM image, int x1, int y1, int x2, int y2){
//  for (int i = x1; i >=x2 ; --i) {
//    int j = y1;
//    while (){}
//  }
//}

void getLineLens(double slope, double l, int *xl, int *yl){
  double sl2 = slope * slope;
  double l2 = l * l;
  *xl = (int)sqrt(l2 / (1 + sl2));
  *yl = (int)sqrt(l2 / (1 + 1/sl2));
}

void drawLine(PGM image, line l){
  int dir = l.slope > 0 ? 1 : -1;
  if (fabs(l.slope) > 1){
    //thick recorrer en x
    for (int j = 0; j < l.thickness; ++j) {
      //recorrer en y
      for (int i = 0; i < l.ylen; ++i) {
        int ypos = l.y + i;
        int xpos = l.x + (int)(i/l.slope) + j;
        image.data[ypos][xpos] = defaultMaxGrey/2;
      }
    }
  } else {
    //thick recorrer en y
    for (int j = 0; j < l.thickness; ++j) {
      //recorrer en y
      for (int i = 0; i < l.xlen; ++i) {
        int xpos = l.x + dir *i;
        int ypos = l.y + (int)(dir *i * l.slope) + j;
        image.data[ypos][xpos] = defaultMaxGrey/2;
      }
    }
  }
}

PGM newImage(int w, int h){
  PGM img;
  img.w = w; img.h = h;
  img.x0 = 0; img.y0 = 0; img.yn=img.h; img.xn = img.w;
  img.maxGreyVal = defaultMaxGrey;
  strncpy(img.fileType, defaultType, 3);
  img.data = allocImgData(img.w, img.h);
  for (int i = 0; i < img.h; ++i) {
    for (int j = 0; j < img.w; ++j) {
      img.data[i][j] = 0;
    }
  }
  return img;
}
line newLine(int x, int y, int xlen, int ylen, int thickness){
  line l;
  l.x = x; l.y = y;
  l.xlen = abs(xlen); l.ylen = abs(ylen);
  l.slope = xlen ? (double)ylen/(double)xlen : 1000; //should use dbl max
  l.length = sqrt(xlen*xlen + ylen*ylen);
  l.angle = atan(l.slope);
  l.thickness = thickness;
  return l;
}
void setLengths(line *l, int xlen, int ylen){
  l->xlen = abs(xlen); l->ylen = abs(ylen);
  l->slope = xlen ? (double)ylen/(double)xlen : 1000; //should use dbl max
  l->angle = atan(l->slope);
  l->length = sqrt(xlen*xlen + ylen*ylen);
}
double compareImg(PGM image, PGM aprox){
  int TP = 0, FP = 0, P = 0;
  for (int i = image.y0; i <image.yn; ++i) {
    for (int j = image.x0; j < image.xn; ++j) {
      if(image.data[i][j]){
        P++;
        if(aprox.data[i][j]) TP++;
      } else if(aprox.data[i][j]) FP++;
    }
  }
  return (double)TP/(double)(P + FP); //penalize false positives
}