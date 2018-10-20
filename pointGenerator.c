#include "pointGenerator.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int height_, width_, nPoints_;


void createPoints(int nPoints, int height, int width, struct Point * myPoints)
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

void createColors(struct RGB * myColors)
{
    unsigned int r,g,b;
    struct RGB myColor;
    for(int i = 0; i < nPoints_; i++){
        r = rand() % 256; g = rand() % 256; b = rand() % 256; 
        myColor.R = r; myColor.G = g; myColor.B = b;
        
        printf("INDEX: %d has RGB:(%u, %u, %u)\n", i, r, g, b);
    }
}

// main: struct Point myPoints[nPoints];
