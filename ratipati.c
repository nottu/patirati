//
// Created by Javier Peralta on 10/9/17.
//

#include "ratipati.h"
#include "pgm.h"

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
  if(y == *yl) { //look downards
    *xl -= 1;
    while(image.data[*yl += 1][*xl] == 0);

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

line getFirstJointInfo(PGM image){
  int x, y; //coords for first white
  findFirstWhite(image, &x, &y);
  if( !x ){
    printf("ERROR");
    exit(1);
  }
  int xl, yl;
  findLeftPoint(image, x, y, &xl, &yl);
  int xr, yr;
  findRightPoint(image, x, y, &xr, &yr);
  //center line start a bit
  x = (xr+x)/2;
  y = (yr + y)/2;
  line l2 = newLine(x, y, xl-x, yl-y, 1);
  return l2;
}

line* fastBadApproach(PGM image){
  line* badLines = (line*)malloc(sizeof(line) * 4);
  badLines[0] = getFirstJointInfo(image);
  int nx = 0, ny = 0, endx, endy;
  for (int i = 1; i < 4; ++i) {
    int slopeDir = badLines[i - 1].slope > 0 ? 1 : -1;
    endx = badLines[i - 1].x + slopeDir*badLines[i - 1].xlen;
    endy = badLines[i - 1].y + badLines[i - 1].ylen; //always down
    nx = endx; ny = endy;
    //recorre ny hasta encontrar dif a 0
    while (image.data[++ny][nx]);
    ny--;
    while (image.data[ny][nx -= slopeDir]);
    nx += slopeDir;
    int xl, yl;
    slopeDir < 0 ? findRightPoint(image, nx, ny, &xl, &yl)
                 : findLeftPoint(image, nx, ny, &xl, &yl);

    endx -= (endx - nx)/2;
    endy -= (endy - ny)/2;
    nx = endx; ny = endy;
    //center line end a bit
    setLengths(&badLines[i - 1], endx - badLines[i - 1].x, endy - badLines[i - 1].y);
    //center line start a bit
    line l = newLine(nx, ny, xl-nx, yl-ny, 1);
    badLines[i] = l;
  }
  return badLines;
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
  freedom.thMin = 1; freedom.thMax = 15;
  return freedom;
}
void genLinesWithVariance(line orig, lineFreedom freedom,  line* lines,  int nLines){
  for (int i = 0; i < nLines; ++i) {
    int startX = freedom.vxMin + rand()%(freedom.vxMax - freedom.vxMin);
    startX += orig.x;
    int startY = freedom.vyMin + rand()%(freedom.vyMax - freedom.vyMin);
    startY += orig.y;
    int endX = freedom.vxEMin + rand()%(freedom.vxEMax - freedom.vxEMin);
    endX += orig.xlen;
    endX *= orig.slope > 0 ? 1 : -1;
    int endY = freedom.vyEMin + rand()%(freedom.vyEMax - freedom.vyEMin);
    endY += orig.ylen;
    int th = freedom.thMin + rand()%(freedom.thMax - freedom.thMin);
    line nl = newLine(startX, startY, endX, endY, th);
    lines[i] = nl;
  }
}
line bestLine(PGM image, line l, int ngen){
  //use less arbitrary values
  #warning use less arbitrary values
  lineFreedom freedom = newLineFreedom(10, 10, 10, 10);
  line *lines = (line*)malloc(sizeof(line) * ngen);
  genLinesWithVariance(l, freedom, lines, ngen);
}
void rati(const char *imgName, const char* outName){
#warning use time(NULL) as seed
  srand(0);
  PGM img = readImage(imgName);
  if(img.data == NULL){
    printf("ERROR al leer %s", imgName);
    return;
  }
//  int ngen = 20;
  cropImage(&img);
//  line *lines = fastBadApproach(img);
////  for (int i = 0; i < 1; ++i) { // i < 4
////    bestLine(img, lines[i], ngen);
////  }
//  PGM img2 = newImage(img.w, img.h);
//  for (int j = 0; j < 4; ++j) {
//    drawLine(img2, lines[j]);
//  }
//  printImage(img2, outName);
//  freeImage(&img2);
//  free(lines);
  printImageCrop(img, outName);
  freeImage(&img);
}

