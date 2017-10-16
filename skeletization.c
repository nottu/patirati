//
// Created by jperalta on 10/13/17.
//

#include "skeletization.h"

pointList* newNode(ipoint point){
  pointList *p = (pointList*)malloc(sizeof(pointList));
  p->point = point; p->next = NULL;
  return p;
}
pointList* newNodeC(int x, int y){
  return newNode(newPoint(x, y));
}
// Calculates number of transitions (white to black) and neighbours, returns white nodes
pointList* countTransitionsAndNeighbours(PGM img, ipoint point, byte *white, byte *transitions){
  pointList *head = newNode(point);
  pointList *tail = head;
  byte n = 8; //should take into account border pixels
  int indexes[8] = {0,1,1,1,0,-1,-1,-1};
  byte lastPix = img.data[point.y - 1][point.x - 1];
  *white = 0; *transitions = 0;
  for (byte i = 0; i < n; ++i) {
    int yidx = point.y + indexes[(i + 6) % 8]; //vales := -1,-1,0,1,1,1,0,-1
    int xidx = point.x + indexes[i];
    if(img.data[yidx][xidx]){
      if(img.data[yidx][xidx] == img.maxGreyVal){
        tail->next = newNodeC(xidx, yidx);
        tail = tail->next;
      }
      img.data[yidx][xidx] = (byte)(img.maxGreyVal - 1);
      *white += 1;
      if(!lastPix) {
        *transitions += 1;
        lastPix = 1;
      }
    } else lastPix = 0;
  }
  tail = head;
  head = head->next;
  free(tail);
  return head;
}

byte shouldKeepFirstPass(PGM img, ipoint p, byte white, byte transitions){
  byte b = 0;
  b += white >= 2 && white <= 6;
  b += transitions == 1;
  b += !img.data[p.y - 1][p.x] || !img.data[p.y][p.x + 1] || !img.data[p.y + 1][p.x];
  b += !img.data[p.y][p.x - 1] || !img.data[p.y][p.x + 1] || !img.data[p.y + 1][p.x];
  return (byte)(b != 4);
}
byte shouldKeepSecondPass(PGM img, ipoint p, byte white, byte transitions){
  byte b = 0;
  b += white >= 2 && white <= 6;
  b += transitions == 1;
  b += !img.data[p.y - 1][p.x] || !img.data[p.y][p.x + 1] || !img.data[p.y][p.x - 1];
  b += !img.data[p.y - 1][p.x] || !img.data[p.y + 1][p.x] || !img.data[p.y][p.x - 1];
  return (byte)(b != 4);
}

int filterPass(PGM *img, shouldKeepFunc func){
  int x = 0, y = 0;
  findFirstWhite(*img, &x, &y);
  pointList *head = newNodeC(x, y);
  pointList *tail = head; // easier insert at end
  byte white = 0, transitions = 0;
  pointList *counter = head;
  do {
    pointList *newPoints = countTransitionsAndNeighbours(*img, counter->point, &white, &transitions);
    tail->next = newPoints;
    while(newPoints && tail->next) tail = tail->next;
    counter->val = func(*img, counter->point, white, transitions);
    counter = counter->next;
  } while(counter);
  tail = head;
  byte** imgData = allocImgData(img->w, img->h);
  memcpy(imgData[0], img->data[0], sizeof(byte)* img->w * img->h);
  int px = 0;
  while (tail->next){
    if(!tail->val){
      imgData[tail->point.y][tail->point.x] = 0;
      px++;
    }
    else imgData[tail->point.y][tail->point.x] = img->maxGreyVal;
    pointList *tmp = tail;
    tail = tail->next;
    free(tmp);
  }
  free(tail);
  freeImageData(img->data);
  img->data = imgData;
  return px;
}

void skeletize(PGM *image){
  int chng = 0;
  int iter = 0;
  do {
    iter++;
    chng = 0;
    chng += filterPass(image, shouldKeepFirstPass);
    chng += filterPass(image, shouldKeepSecondPass);
  } while(chng && iter < SKELETIZATION_MAX_ITER);
}