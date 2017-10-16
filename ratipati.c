//
// Created by Javier Peralta on 10/9/17.
//

#include "ratipati.h"

//line stuff
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
line newLineFromPoints(ipoint p1, ipoint p2){
  return newLine(p1.x ,p1.y, p2.x - p1.x, p2.y - p1.y, 1);
}
void setLengths(line *l, int xlen, int ylen){
  l->xlen = abs(xlen); l->ylen = abs(ylen);
  l->slope = xlen ? (double)ylen/(double)xlen : 1000; //should use dbl max
  l->angle = atan(l->slope);
  l->length = sqrt(xlen*xlen + ylen*ylen);
}
void drawLine(PGM image, line l){
  int dir = l.slope > 0 ? 1 : -1;
  if (fabs(l.slope) > 1){
    //thick recorrer en x
    for (int j = 0; j < l.thickness; ++j) {
      //recorrer en y
      for (int i = 0; i < l.ylen + j *dir/(2*l.slope); ++i) {
        int ypos = l.y + i;
        int xpos = l.x - l.thickness/2 + (int)(i/l.slope) + j;
        image.data[ypos][xpos] = defaultMaxGrey/2;
      }
    }
  } else {
    //thick recorrer en y
    for (int j = 0; j < l.thickness; ++j) {
      //recorrer en y
      for (int i = 0; i < l.xlen - j*l.slope/2; ++i) {
        int xpos = l.x + dir *i;
        int ypos = l.y - l.thickness/2 + (int)(dir *i * l.slope) + j;
        image.data[ypos][xpos] = defaultMaxGrey/2;
      }
    }
  }
}
//

int findPointInRow(PGM *img, int y){
  for (int j = img->x0; j < img->xn; ++j) {
    if (img->data[y][j]) return j;
  }
  return 0;
}

ipoint __newPoint(int x, int y){
  ipoint p;
  p.x = x; p.y = y;
  return p;
}
void findLeftPoint(PGM image, int x, int y, int* xl, int* yl){
  *xl = x, *yl = y;
  int maxYdif = 30, ydif = 0, xdif = 0;
  for (int i = x - 1; i >= 0; --i) { //look left
    ydif = 0; xdif++;
    for (int j = *yl; j < image.yn; ++j) {
      if(image.data[j][i]) {
        if(j - *yl > xdif) return;
        *xl = i; *yl = j;
        xdif = 0;
        break;
      }
      if(++ydif >= maxYdif){
        return;
      }
    }
  }
}
//inflection points plus start and end
ipoint* findInflectionPoints(PGM img, int nPoints){
  ipoint *points = (ipoint*)malloc(sizeof(ipoint) * nPoints);
  int x, y;
  findFirstWhite(img, &x, &y);
  points[0] = __newPoint(x, y);
  char lastSlope = 0;
  int x1 = findPointInRow(&img, img.y0), x2;
  int found = 1;
  for (int i = y + 8; i < img.yn; i++) {
    x2 = findPointInRow(&img, i);
    if(x2 != 0 && x1 != 0){
      if(x1 > x2){
        if(lastSlope == -1){//inflection point
          points[found++] = __newPoint(x1, i);
        }
        lastSlope = 1;
      } else if(x1 < x2){
        if(lastSlope == 1){//inflection point
          points[found++] = __newPoint(x1, i);
        }
        lastSlope = -1;
      }
      if(abs(x1 - x2) > 25) {
        int xl, yl;
        findLeftPoint(img, x2, i, &xl, &yl);
        x2 = xl;
        i = yl;
        points[found++] = __newPoint(x2, i);
      }
    }
    x1 = x2;
  }
  findLastWhite(img, &x, &y);
  points[nPoints - 1] = __newPoint(x, y); //found should equal nPoints-1
  return points;
}

line* rati(PGM img){
  int npoints = 5;
  ipoint* points = findInflectionPoints(img, npoints);
  line* lines = (line*)malloc(sizeof(line) * npoints);
  for (int i = 0; i < 4; ++i) {
    lines[i] = newLineFromPoints(points[i], points[i+1]);
    drawLine(img, lines[i]);
  }
  return lines;
}