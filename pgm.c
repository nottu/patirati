//
// Created by Javier Peralta on 10/7/17.
//

#include "pgm.h"

//private function...
byte** allocImgData(int w, int h){
  byte **data    = (byte**)malloc(sizeof(byte*) * h);
  data[0] =  (byte*)calloc(sizeof(byte), (size_t )h * w);
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
  fscanf(f, "%s %d %d %hd", img.fileType, &(img.w), &(img.h), &(img.maxGreyVal));
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
      if (d != 0) {
        image->x0 = j < image->x0 ? j-1 : image->x0;
        image->y0 = i < image->y0 ? i-1 : image->y0;
        image->xn = j > image->xn ? j : image->xn;
        image->yn = i > image->yn ? i : image->yn;
      }
    }
  }
  image->xn+=2; image->yn+=2; //add some padding
}
void printImage(PGM image, const char* name){
  FILE *f = fopen(name, "w");
  fprintf(f, "%s\n%d %d\n%d\n", image.fileType, image.w, image.h, image.maxGreyVal);
  for (int i = 0; i < image.h; ++i) {
    for (int j = 0; j < image.w; ++j) {
      fprintf(f, "%hd ", image.data[i][j]);
    }
  }
  fclose(f);
}
void printImageCrop(PGM image, const char* name){
  FILE *f = fopen(name, "w");
  fprintf(f, "%s\n%d %d\n%d\n", image.fileType, image.xn - image.x0, image.yn - image.y0, image.maxGreyVal);
  for (int i = image.y0; i <image.yn; ++i) {
    for (int j = image.x0; j < image.xn; ++j) {
      fprintf(f, "%hd ", image.data[i][j]);
    }
  }
  fclose(f);
}
//set all iamge to 0s
void cleanImageData(PGM image){
  //only need zeros in uncropped area
  for (int i = image.y0; i <image.yn; ++i) {
    for (int j = image.x0; j < image.xn; ++j) {
     image.data[i][j] = 0;
    }
  }
}
void freeImage(PGM *image){
  freeImageData(image->data);
  image->w = 0;
  image->h = 0;
  image->maxGreyVal = 0;
  image->x0 = 0;
  image->y0 = 0;
  image->xn = 0;
  image->yn = 0;
}
void freeImageData(byte** imgData){
  free(imgData[0]);
  free(imgData);
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
PGM cloneImage(PGM imgO){
  PGM img = cleanImageClone(imgO);
  memcpy(img.data[0], imgO.data[0], sizeof(byte) * img.w * img.h);
  return img;
}

PGM cleanImageClone(PGM imgO){
  PGM img;
  img.w = imgO.w; img.h = imgO.h;
  img.x0 = imgO.x0; img.y0 = imgO.y0; img.yn=imgO.yn; img.xn = imgO.xn;
  img.maxGreyVal = imgO.maxGreyVal;
  strncpy(img.fileType, imgO.fileType, 3);
  img.data = allocImgData(img.w, img.h);
  return img;
}
//last and first points
void findFirstWhite(PGM image, int *x, int *y){
  *x = 0, *y = 0; //coords for first white
  for (int i = image.y0; i < image.yn; ++i) { //should loop only once
    for (int j = image.x0; j < image.xn; ++j) {
      if(image.data[i][j]) {
        *x = j; *y = i;
        return;
      }
    }
  }
}
void findLastWhite(PGM image, int *x, int *y){
  *x = 0, *y = 0; //coords for first white
  for (int i = image.yn; i > image.y0; --i) { //should loop only once
    for (int j = image.xn; j > image.x0; --j) {
      if(image.data[i][j]) {
        *x = j; *y = i;
        return;
      }
    }
  }
}
//

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
  return (double)(TP)/(double)(P); //penalize false positives
}