//
// Created by Javier Peralta on 10/7/17.
//

#include <stdlib.h>
#include "pgm.h"

PGM readImage(const char* imageName){
  FILE *f = fopen(imageName, "r");
  PGM img;
  fscanf(f, "%s %d %d %d", img.fileType, &(img.w), &(img.h), &(img.maxGreyVal));
  //init values
  img.x0 = 0; img.y0 = 0; img.yn=img.h; img.xn = img.w;
  //images are ~400k we can still have'em in an simple arr
  img.data    = (byte**)malloc(sizeof(byte*) * img.h);
  img.data[0] =  (byte*)malloc(sizeof(byte) * img.h * img.w);
  for (int i = 1; i < img.h; i++){
    img.data[i] = img.data[i-1] + img.w;
  }
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