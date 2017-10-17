//
// Created by Javier Peralta on 10/9/17.
//
#include "ratipati.h"
#include "linepoint.h"

//linefreedom
pointFreedom newPointFreedom(byte x, byte y, byte th){
  pointFreedom freedom;
  freedom.xVar = x; freedom.yVar = y; freedom.thVar = th;
  return freedom;
}
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
void drawLine(PGM image, line l){
  int dir = l.slope > 0 ? 1 : -1;
  if (fabs(l.slope) > 1){
    //thick recorrer en x
    for (int j = 0; j < l.thickness; ++j) {
      //recorrer en y
      for (int i = 0; i < l.ylen + j *dir/(2*l.slope); ++i) {
        int ypos = l.y + i;
        int xpos = l.x - l.thickness/2 + (int)(i/l.slope) + j;
        if(ypos < image.y0 || ypos > image.yn || xpos < image.x0 || xpos > image.xn) continue;
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
        if(ypos < image.y0 || ypos > image.yn || xpos < image.x0 || xpos > image.xn) continue;
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
ipoint* findInflectionPoints(PGM img, byte nPoints){
  ipoint *points = (ipoint*)malloc(sizeof(ipoint) * nPoints);
  int x, y;
  findFirstWhite(img, &x, &y);
  points[0] = newPoint(x, y);
  char lastSlope = 0;
  int x1 = findPointInRow(&img, img.y0), x2;
  int found = 1;
  for (int i = y + 8; i < img.yn; i++) {
    x2 = findPointInRow(&img, i);
    if(x2 != 0 && x1 != 0){
      if(x1 > x2){
        if(lastSlope == -1 && found < nPoints -1){//inflection point
          points[found++] = newPoint(x1, i);
        }
        lastSlope = 1;
      } else if(x1 < x2){
        if(lastSlope == 1 && found < nPoints -1){//inflection point
          points[found++] = newPoint(x1, i);
        }
        lastSlope = -1;
      }
      if(abs(x1 - x2) > 25) {
        int xl, yl;
        findLeftPoint(img, x2, i, &xl, &yl);
        x2 = xl;
        i = yl;
        points[found++] = newPoint(x2, i);
      }
    }
    x1 = x2;
  }
  findLastWhite(img, &x, &y);
  points[nPoints - 1] = newPoint(x, y); //found should equal nPoints-1
  return points;
}
//UMDA like algorithm
int cmpGradeFunc(const void *a, const void *b){
  pointsGrade *pa = (pointsGrade*)a;
  pointsGrade *pb = (pointsGrade*)b;
  int res = pa->grade < pb->grade ? 1 : -1;
  return res;
}
double gradePoints(PGM *img, ipoint *points, byte thVar, byte thickness, int n){
  double grade = 0;
  int nlines = n - 1;
  int nTests = 1;
  line* lines = (line*)malloc(sizeof(line) * nlines);
  for (int i = 0; i < nlines; ++i) {
    lines[i] = newLineFromPoints(points[i], points[i + 1]);
  }
  PGM img2 = cleanImageClone(*img);
  //byte bestTh = *thickness;
  for (int i = 0; i < nTests; ++i) {
    byte thD = (byte)(15 + rand()%(10 * 2) - 10);
    for (int j = 0; j < nlines; ++j) {
      lines[j].thickness = (byte)(j == 0 ? 50 : 20);
      drawLine(img2, lines[j]);
    }
    double g = compareImg(*img, img2);
    if( g > grade){
      grade = g;
     // bestTh = thD;
    }
  }
  //*thickness = bestTh;
  freeImage(&img2);
  free(lines);
  return grade;
}
ipoint genPointWithVariance(ipoint p, int dx, int dy) {
  dy = dy ? dy : dy+1;
  dx = dx ? dx : dx+1;
  int xD = rand()%(2 * dx) - dx; //should gen values in [-dx, dx]
  int yD = rand()%(2 * dy) - dy;
  return newPoint(p.x + xD, p.y + yD);
}
void genPointPopulation(ipoint *p, pointFreedom *var, int n, int size, ipoint** pop){
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < n; ++j) {
      pop[i][j] = genPointWithVariance(p[j], var[j].xVar, var[j].yVar);
    }
  }
}
double testAndSortPointPop(PGM *img, ipoint** pop, byte* thicknesses, pointFreedom *var, int n, int size){
  pointsGrade *grades = (pointsGrade*)malloc(sizeof(pointsGrade) * size);
  for (int i = 0; i < size; ++i) {
    grades[i].points = pop[i];
    grades[i].thickness = 15;
    grades[i].grade = gradePoints(img, pop[i], 0, 15, n);
  }
  //sort
  qsort(grades, (size_t )size, sizeof(pointsGrade), cmpGradeFunc);
//  for (int i = 0; i < n - 1; ++i) {
//    thicknesses[i] = grades[0]
//  }
  for (int i = 0; i < size; ++i) {
    pop[i] = grades[i].points;
//    printf("Sorted Grades := %lf\n", grades[i].grade);
  }
//  printf("Grade := %lf", grades[0].grade);
  double best = grades[0].grade;
  free(grades);
  return best;
}
void updatePointAndVariance(ipoint *p, pointFreedom *var, ipoint **pop, int n, int size){
  byte maxVar = 10;
  for (int i = 0; i < n; ++i){
    double px = 0, py = 0;
    int minX = p[i].x, maxX = p[i].x, minY = p[i].y, maxY = p[i].y;
    for (int j = 0; j < size; ++j) {
      px += pop[j][i].x;
      py += pop[j][i].y;
      if(pop[j][i].x < minX) minX = pop[j][i].x;
      else if(pop[j][i].x > maxX) maxX = pop[j][i].x;
      if(pop[j][i].y < minY) minY = pop[j][i].y;
      else if(pop[j][i].y > maxY) maxY = pop[j][i].y;
    }
    //p[i].x = (int)(px/size);
    var[i].xVar = (byte)((p[i].x - minX) > (maxX - p[i].x) ? (p[i].x - minX) : (maxX - p[i].x));
    var[i].xVar++;
    if(var[i].xVar > maxVar) var[i].xVar = maxVar;
    var[i].yVar = (byte)((p[i].y - minY) > (maxX - p[i].y) ? (p[i].y - minX) : (maxX - p[i].y));
    var[i].yVar++;
    if(var[i].yVar > maxVar) var[i].yVar = maxVar;
  }
}
line* findBestLines(PGM *img, ipoint* points, byte n){
  byte nlines = (byte)(n - 1); //always n > 0
  line* lines = (line*)malloc(sizeof(line) * nlines);
  byte* thicknesses = (byte*)malloc(sizeof(byte) * nlines);
  //use array of ipoint struct to save variance in x and y
  pointFreedom* variance = (pointFreedom*)malloc(sizeof(pointFreedom) * n);
  byte maxFree = 10;
  for (int i = 0; i < nlines; ++i) {
    thicknesses[i] = 15;
  }
  for (int i = 0; i < n; ++i) {
    variance[i] = newPointFreedom(maxFree, maxFree, maxFree);
  }
  byte genSize = 100; // max of 256?
  ipoint** pop = (ipoint**)malloc(sizeof(ipoint*) * genSize);
  for (int i = 0; i < genSize; ++i) pop[i] = (ipoint*)malloc(sizeof(ipoint) * n);
  //loop in a while variance > epsilon
  int maxIter = 100;
  ipoint *bestPop = (ipoint*)malloc(sizeof(ipoint) * n);
  byte hasBest = 0;
  double best = 1;
  int iterSinceBest = 0;
  while(maxIter--) {
//    printf("Iter %d : ", 60 - maxIter);
    genPointPopulation(points, variance, n, genSize, pop);
    if (hasBest)
      memcpy(pop[0], bestPop, sizeof(ipoint) * n);
    double nB = testAndSortPointPop(img, pop, thicknesses, variance, n, genSize);
    if (fabs(best - nB) < 0.00001){
      iterSinceBest++;
      if (iterSinceBest > 10) break;
    } else{ iterSinceBest = 0;}
    best = nB;
    hasBest = 1;
    memcpy(bestPop, pop[0], sizeof(ipoint) * n);
    updatePointAndVariance(points, variance, pop, n, (genSize + 1)/2);
    memcpy(points, pop[0], sizeof(ipoint) * n);
//    printf("\n");
  }
  free(bestPop);
  for (int i = 0; i < nlines; ++i) {
    lines[i] = newLineFromPoints(pop[0][i], pop[0][i + 1]);
    lines[i].thickness = 1;
  }
  for (int i = 0; i < genSize; ++i) free(pop[i]);
  free(pop);
  free(variance);
  free(thicknesses);
  return lines;
}

// simple call to do everything
line* rati(PGM img, PGM img0){
  byte npoints = 5;
  ipoint* points = findInflectionPoints(img, npoints);
  line* lines = findBestLines(&img0, points, npoints);
  free(points);
  return lines;
}
