#include "mapGenerator.h"

void mapGenerator::createPoints(int nPoints, int height, int width, vector<Point> & myPoints)
{
    int rx, ry;
    height_ = height;
    width_ = width;
    nPoints_ = nPoints;
    
    struct Point myP;
    for(int i = 0; i < nPoints; i++){
        rx = rand() % height_;
        ry = rand() % width_;
        
        myP.x = rx;
        myP.y = ry;
        
        myPoints[0] = myP;
        
        printf("Generated Point: [X][Y] = (%d, %d)\n", rx, ry);
    }
}

void mapGenerator::createColors(vector<RGB> & myColors)
{
    unsigned int r,g,b;
    struct RGB myColor;
    for(int i = 0; i < nPoints_; i++){
        r = rand() % 256; g = rand() % 256; b = rand() % 256; 
        myColor.R = r; myColor.G = g; myColor.B = b;
        
        printf("INDEX: %d has RGB:(%u, %u, %u)\n", i, r, g, b);
    }
}

void mapGenerator::createSites(vector<Point> & myPoints, vector<RGB> & myColors)
{
    int d;
    for(int hh = 0; hh < height_; hh++){
        for(int ww = 0; ww < width_; ww++){
            int ind = -1, dist = INT_MAX;
            for(int it = 0; it < nPoints_; it++){
                struct Point p = myPoints[it];
                d = distance(p, hh, ww);
                if (d < dist)
                {
                    dist = d;
                    ind = it;
                }else{

                }
            }
        }
    }    
}

void mapGenerator::setSites()
{
}

int mapGenerator::distance(Point & myPoint, int x, int y)
{
    int xd = x - myPoint.x;
    int yd = y - myPoint.y;
    int result = (xd*xd) + (yd*yd);
    return (int)sqrt(result);
}
