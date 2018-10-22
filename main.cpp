#include <iostream>
#include "bitmap_image.hpp"
#include <noise/noise.h>

using namespace std;
using namespace noise;

struct RGB
{
    int r, g, b;
};


int main()
{
    double width, height;
    
    cout << "Please, introduce the size of the image: " << endl;
    cin >> width >> height;
    
	module::Perlin myModule;
    
	bitmap_image image(width, height);
    
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            
			float value = myModule.GetValue(x, y, 0);
			cout << "Value for point: (" << x << ", " << y << "): " << value << endl;
			
			RGB rgb;
			if(value <= 0.5){
				rgb.r = 178;
				rgb.g = 178;
				rgb.b = 178;
			}else{
				rgb.r = 255;
				rgb.g = 0;
				rgb.b = 0;
			}
			
            image.set_pixel(x, y, rgb.r, rgb.b, rgb.g);
        }
    }
    
    image.save_image("Noise.bmp");
    cout << "Check the directory. It contains the generated noise image." << endl;
}
