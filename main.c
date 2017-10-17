#include "skeletization.h"
#include "ratipati.h"
#include "linepoint.h"

int main(int argc, const char** argv) {
  if(argc > 1){
    srand(0); //change to time(NULL)
    PGM img = readImage(argv[1]);
    cropImage(&img);
    PGM imgO = cloneImage(img);
    skeletize(&img);
    line* lines = rati(img, imgO);
    printf("-------- %s --------\n", argv[1]);
    for (int i = 0; i < 4; ++i) {
      printf("Linea %d, Angulo %lf, Longitud %lf\n", i, lines[i].angle, lines[i].length);
      lines[i].thickness = 15;
      drawLine(imgO, lines[i]);
    }
    free(lines);
    printImage(imgO, argv[2]);
    freeImage(&img);
    freeImage(&imgO);
  }
  return 0;
}