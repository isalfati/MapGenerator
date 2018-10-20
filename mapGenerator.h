#ifndef MAPGENERATOR_H_
#define MAPGENERATOR_H_

#include <iostream>
#include <vector>
#include <limits.h>
#include <math.h>

using namespace std;

struct Point{
    int x, y;
};

struct RGB{
    unsigned int R, G, B;
};

class mapGenerator
{

public:
    
    int height_, width_, nPoints_;
    vector<Point> myPoints_;
    vector<RGB> myColors_;
    
    void createPoints(int nPoints, int height, int width, vector<Point> & myPoints);
    void createColors(vector<RGB> & myColors);
    
    void createSites(vector<Point> & myPoints, vector<RGB> & myColors);
    void setSites();
    
    void printPoints();
    void printColors();

    int distance(Point & myPoint, int x, int y);

};

#endif // MAPGENERATOR_H_
