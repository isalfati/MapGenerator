#include <stdio.h>
#include "bitMapGenerator.h"
#include "pointGenerator.h"

int main()
{
    int nPoints, height, width;
    printf("Please, introduce the numer of points, and the size of the board.\n");
    scanf("%d %d %d", &nPoints, &height, &width);
    
    struct Point myPoints[20];
    createPoints(20, 5, 5, myPoints);
    struct RGB myColors[20];
    createColors(myColors);
}
