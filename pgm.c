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

void drawLine(PGM image, line l){
  if(fabs(l.slope) > 1){
    int imax = l.ylen < 0 ? 0 : l.ylen;
    double xstart = l.x;
    double ystart = l.y;
    double ystart2 = - (l.thickness - 1) * 1/l.slope;
    double xstart2 = - (l.thickness - 1) * l.slope;
    int i = l.ylen < 0 ? l.ylen : 0;
    int ypos = 0, xpos = 0;
    for (; i < imax; ++i) {
      ypos = (int)ystart + i;
      xpos = (int)xstart + (int)(i/l.slope);
      image.data[ypos][xpos] = (byte)defaultMaxGrey;
      for (int j = 1; j < xstart2 - 1 ; ++j) {
//        image.data[ypos + (j)][(int)xstart + j] = (byte)defaultMaxGrey/2;
        image.data[ypos + (int)(j * (-0.99/l.slope))][xpos + j] = (byte)defaultMaxGrey/2;
      }
      ypos = (int)(ystart +ystart2 + i);
      xpos = (int)(xstart + xstart2 + (int)(i/l.slope));
      image.data[ypos][xpos] = (byte)defaultMaxGrey;
    }
    if(l.thickness > 1){
      line l2 = newLine(l.x, l.y, (int)xstart2, (int)ystart2, 1);
      drawLine(image, l2);
      l2 = newLine((int)xstart + (int)(i/l.slope) + 1, (int)ystart + i,(int)xstart2, (int)ystart2, 1);
      drawLine(image, l2);
    }
  }
  if(fabs(l.slope) < 1){
    int i = l.xlen < 0 ? l.xlen : 0;
    int imax = l.xlen < 0 ? 0 : l.xlen;
    for (; i < imax; ++i) {
      int xpos = l.x + i;
      int ypos = l.y + (int)(i*l.slope);
      for (int j = 0; j < l.thickness; ++j) {
        image.data[ypos+j][xpos] = (byte)defaultMaxGrey;
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
  l.x = x; l.y = y; l.xlen = xlen; l.ylen = ylen;
  l.slope = (double)ylen/(double)xlen;
  l.length = sqrt(xlen*xlen + ylen*ylen);
  l.angle = atan(l.slope);
  l.thickness = thickness;
  return l;
}