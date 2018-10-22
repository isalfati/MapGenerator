#include <iostream>
#include "bitmap_image.hpp"
#include <noise/noise.h>
#include <ctime>
#include <random>

using namespace std;
using namespace noise;

struct RGB
{
    int r, g, b;
};

double dRand(double lower, double upper){
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> unif(lower, upper);
	
	double a_random_double = unif(gen);
	return a_random_double;
}

int main()
{
	srand(time(0));
    double width, height;
	
	cout << "dRand(0.0, 1.0): " << dRand(0.0, 1.0) << endl;
    
    cout << "Please, introduce the size of the image: " << endl;
    cin >> width >> height;
	cout << "Now, introduce the Frequency (1 - 100), the Lacunarity (1 - 5), the Octave count (int) and the Persistence: (0 - 2)" << endl;
	double freq, lac, pers;
	int oct;
	cin >> freq >> lac >> oct >> pers;
    
	module::Perlin myPerlin, myPerlin2, myPerlin3;
	int random = rand() % INT32_MAX;
	cout << "Random value seed: " << random << endl;
	myPerlin.SetSeed (random);
	myPerlin.SetFrequency (freq);
	myPerlin.SetLacunarity (lac);
	myPerlin.SetOctaveCount (oct);
	myPerlin.SetPersistence (pers);
	myPerlin.SetNoiseQuality (noise::QUALITY_BEST);
	
	myPerlin2.SetSeed (random);
	myPerlin2.SetFrequency (freq*2);
	myPerlin2.SetLacunarity (lac);
	myPerlin2.SetOctaveCount (oct);
	myPerlin2.SetPersistence (pers);
	myPerlin2.SetNoiseQuality (noise::QUALITY_STD);
	
	myPerlin3.SetSeed (random);
	myPerlin3.SetFrequency (freq/2);
	myPerlin3.SetLacunarity (lac);
	myPerlin3.SetOctaveCount (oct);
	myPerlin3.SetPersistence (pers);
	myPerlin3.SetNoiseQuality (noise::QUALITY_BEST);
	
	
	
    
	bitmap_image image(width, height);
    
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            
			//float value = myPerlin.GetValue(x, y, 0);
			//if(value < 0.0) value *= -1.0;
			
			float value1 = myPerlin.GetValue(x, y, 0);
			float value2 = dRand(0.0f, value1);
			//float value3 = dRand(value2, value1);
			
			//float value2 = myPerlin2.GetValue(x, y, 0);
			//float value3 = myPerlin3.GetValue(x, y, 0);
			
			if(value1 < 0) value1 *= -1;
			if(value2 < 0) value2  *= -1;
			//if(value3 < 0) value3 *= -1;
			
			float value = value1 + value2; // + value3;
			if(value > 1) value = 1.0;
			if(value < -1) value = -1.0;

			//cout << "Value for point: (" << x << ", " << y << "): " << value << endl;
			
			RGB rgb;
			if(value > -1 and value < -0.2){
				rgb.r = 255;
				rgb.g = 255; 
				rgb.b = 255;
			}else if(value >= -0.2 and value < -0.4){
				rgb.r = 230;
				rgb.g = 230; 
				rgb.b = 230;
			}else if(value >= -0.4 and value < -0.6){
				rgb.r = 205;
				rgb.g = 205; 
				rgb.b = 205;
			}else if(value <= -0.6 and value < -0.8){
				rgb.r = 180;
				rgb.g = 180; 
				rgb.b = 180;
			}else if(value >= -0.8 and value < 0){
				rgb.r = 155;
				rgb.g = 155; 
				rgb.b = 155;
			}else if(value >= 0 and value < 0.2){
				rgb.r = 130;
				rgb.g = 130; 
				rgb.b = 130;
			}else if(value >= 0.2 and value < 0.4){
				rgb.r = 105;
				rgb.g = 105; 
				rgb.b = 105;
			}else if(value >= 0.4 and value < 0.6){
				rgb.r = 80;
				rgb.g = 80; 
				rgb.b = 80;
			}else if(value >= 0.6 and value < 0.8){
				rgb.r = 55;
				rgb.g = 55; 
				rgb.b = 55;
			}else{
				rgb.r = 30;
				rgb.g = 30; 
				rgb.b = 30;
			}
			
			
            image.set_pixel(x, y, rgb.r, rgb.b, rgb.g);
        }
    }
    
    image.save_image("Noise.bmp");
    cout << "Check the directory. It contains the generated noise image." << endl;
}
