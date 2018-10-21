#include "mapGenerator.h"


//First width, then height
mapGenerator::mapGenerator()
{
}
      
mapGenerator::mapGenerator(int width, int height, int nPoints)
{
    width_      = width;
    height_     = height;
    nPoints_    = nPoints;
    
    myPoints_ = vector<Point>(nPoints);
    myColors_ = vector<RGB>  (nPoints);
    
    myImage_ = bitmap_image(width_, height_);
    //myImage_.set_all_channels(255, 255, 255);
    
    //image_drawer draw(myImage_);
}

void mapGenerator::createPoints()
{
    int rx, ry;
    
    struct Point myP;
    int w = width_  - 2;
    int h = height_ - 2;
    
    for(int i = 0; i < nPoints_; i++){
        rx = rand() % w + 1;
        ry = rand() % h + 1;
        
        myP.x = rx;
        myP.y = ry;
        
        myPoints_[i] = myP;
        
        //cout << "Generated Point: [X][Y] = (" << rx << "," << ry << ")" << endl;
    }
}

void mapGenerator::createColors()
{
    unsigned int r,g,b;
    struct RGB color;
    for(int i = 0; i < nPoints_; i++){
        r = rand() % 256; g = rand() % 256; b = rand() % 256; 
        color.R = r; color.G = g; color.B = b;
        myColors_[i] = color;
        
//         cout << "INDEX: " << i << " has RGB: (" << r << ", " << g << ", " << b << ")" << endl;
    }
}

void mapGenerator::createSites()
{
    int d = 0;
    for(int ww = 0; ww < width_; ww++){
        for(int hh = 0; hh < height_; hh++){
            int ind = -1, dist = INT_MAX;
            for(int it = 0; it < myPoints_.size(); it++){
                Point & p = myPoints_[it];
                d = distance(p, ww, hh);
                if(d < dist){
                    dist = d;
                    ind = it;
                }
                
            }
            if(ind > -1){
//                 cout << "INDEX RECORRIDO: " << ind << endl;
                // Pintamos
                RGB color = myColors_[ind];
//                 cout << " has the color: (RGB) : (" << color.R << "," << color.G << "," << color.B << ")" << endl;           
                myImage_.set_pixel(ww, hh, color.R, color.G, color.B);
            }else{
                // Nada
            }
        }
    }
}
    

void mapGenerator::setSites()
{
    for(const auto& point: myPoints_){
        for(int i = -1; i < 2; i++){
            for(int j = -1; j < 2; j++){
                int x = point.x;
                int y = point.y;
                myImage_.set_pixel(x + i, y + j, 0, 0, 0);
            }
        }
    }
    //image_drawer draw(myImage_);
    myImage_.save_image("primertest.bmp");
    cout << "Check the directory. It contains the generated map." << endl;
}

int mapGenerator::distance(Point & myPoint, int x, int y)
{
    int xd = x - myPoint.x;
    int yd = y - myPoint.y;
    int result = (xd*xd) + (yd*yd);
    //return (int)sqrt(result);
    return result;
}
