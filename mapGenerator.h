#ifndef MAPGENERATOR_H_
#define MAPGENERATOR_H_

#include <iostream>
#include <vector>
#include <limits.h>
#include <math.h>
#include "bitmap_image.hpp"

using namespace std;

struct Point{
    int x, y;
};

struct RGB{
    unsigned int R, G, B;
};

const int height = 200;
const int width  = 200;

class mapGenerator
{

public:
    
    int height_, width_, nPoints_;
    vector<Point> myPoints_;
    vector<RGB> myColors_;
    bitmap_image myImage_;
    
    mapGenerator();
    mapGenerator(int height, int width, int nPoints);
            
    void createPoints();
    void createColors();
    
    void createSites();
    void setSites();
    
    void printPoints();
    void printColors();

    int distance(Point & myPoint, int x, int y);

};

#endif // MAPGENERATOR_H_
