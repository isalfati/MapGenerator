#ifndef POINTGENERATOR_H_
#define POINTGENERATOR_H_

struct Point{
    int x, y;
};

struct RGB{
    unsigned int R, G, B;
};

void createPoints(int nPoints, int height, int width, struct Point * myPoints);
void createColors(struct RGB * myColors);
void createSites();
void setSites();

void printPoints();
void printColors();

int distance(struct Point * point, int x, int y);



#endif // POINTGENERATOR_H_
