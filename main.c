#include "skeletization.h"
#include "ratipati.h"

int main(int argc, const char** argv) {
  if(argc > 1){
    PGM img = readImage(argv[1]);
    cropImage(&img);
    PGM imgO = cloneImage(img);
    skeletize(&img);
    line* lines = rati(img);
    for (int i = 0; i < 4; ++i) {
      drawLine(imgO, lines[i]);
    }
    printImage(imgO, argv[2]);
    freeImage(&img);
    freeImage(&imgO);
  }
  return 0;
}