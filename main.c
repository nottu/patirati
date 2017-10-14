#include "skeletization.h"

int main(int argc, const char** argv) {
  if(argc > 1){
    PGM img = readImage(argv[1]);
    cropImage(&img);
    printImageCrop(img, "test0.pgm");
    skeletize(&img);
    printImageCrop(img, "test1.pgm");
    freeImage(&img);
  }
  return 0;
}