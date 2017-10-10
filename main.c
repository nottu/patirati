#include "ratipati.h"

int main(int argc, const char** argv) {
  PGM img = readImage(argv[1]);
  cropImage(&img);
//  printImageCrop(img, "crop.pgm");
  PatiRati p;
  getFirstJointInfo(&p, img);
  freeImage(&img);
  return 0;
}