#include <iostream>
#include "bitmap_image.hpp"
#include "Noise/noise.h"

using namespace std;

struct RGB
{
    int r, g, b;
};


int main()
{
    int width, height;
    
    cout << "Please, introduce the size of the image: " << endl;
    cin >> width >> height;
    
    cout << "Now, introduce the frequency, the octaves and a seed value: " << endl;
    
    int freq, oct, seed;
    cin >> freq >> oct >> seed;
    
    //freq = std::clamp(freq, 0.1, 64.0);
    //oct  = std::clamp(oct, 1, 16);
    
    
    double fx = width  / freq; 
    double fy = height / freq;
    
    bitmap_image image(width, height);
    
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            
            //const RGB rgb(perlin.octaveNoise0_1(x / fx, y / fy, oct));
            RGB rgb;
            rgb.r = 255;
            rgb.g = 255;
            rgb.b = 255;
            
            image.set_pixel(x, y, rgb.r, rgb.b, rgb.g);
        }
    }
    
    image.save_image("Noise.bmp");
    cout << "Check the directory. It contains the generated noise image." << endl;
}
