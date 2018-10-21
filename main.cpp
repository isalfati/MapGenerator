#include <iostream>
#include "mapGenerator.h"
#include "bitmap_image.hpp"

int main()
{
    bitmap_image image(5000, 5000);
    
    image.set_all_channels(255, 255, 255);
    
    image_drawer draw(image);
    
    for(int i = 0; i < 5000; i++)
        for(int j = 0; j < 5000; j++)
            if(i==j) image.set_pixel(i,j, 255, 0,0);
    
    image.save_image("bmp.output");
    
    
    
    
    
    int h, w, nP;
    cout << "Please, introduce the number of points, and the size of the beard." << endl;
    cin >> nP >> h >> w;
    
    
    mapGenerator myBoard(w, h, nP);
    
    myBoard.createPoints();
    myBoard.createColors();
    myBoard.createSites();
    myBoard.setSites();
    
    
    
    mapGenerator uno;
    uno = mapGenerator(1,2,3);
    

    
    /*int nPoints, height, width;
    printf("Please, introduce the numer of points, and the size of the board.\n");
    scanf("%d %d %d", &nPoints, &height, &width);
    
    struct Point myPoints[20];
    createPoints(20, 5, 5, myPoints);
    struct RGB myColors[20];
    createColors(myColors);
    
    unsigned char bmp[height][width][3];
    createSites(myPoints, myColors, bmp);*/
}
