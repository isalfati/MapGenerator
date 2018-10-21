#include <iostream>
#include "mapGenerator.h"
#include "bitmap_image.hpp"

int main()
{
    int h, w, nP;
    cout << "Please, introduce the number of points, and the size of the beard." << endl;
    cin >> nP >> h >> w;
    
    mapGenerator myBoard(w, h, nP);
    
    myBoard.createPoints();
    myBoard.createColors();
    myBoard.createSites();
    myBoard.setSites();
    
}
