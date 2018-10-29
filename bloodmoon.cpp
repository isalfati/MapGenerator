#include <iostream>
#include <noise/noise.h>
#include "noiseutils.cpp"
#include <ctime>
#include <random>
#include <sstream>

using namespace std;
using namespace noise;
using namespace noise::utils;

#define SEA_LEVEL -0.0


string tswp(double value, int n = 3)
{
	ostringstream out;
	out.precision(n);
	out << fixed << value;
	return out.str();
}

int main()
{
	srand(time(0));
	int seed = rand() % INT32_MAX;
	
	cout << "This is the seed for the generated image: " << seed << endl;
	cout << endl;
	cout << "Please, introduce the width and the height of the moon: " << endl;
	
	int width, height;
	cin >> width >> height;
	
	cout << "Now, introduce the xLow, xHigh, yLow, yHigh. This 4 must form a square in a plain: " << endl;
	double xLow, xHigh, yLow, yHigh;
	cin >> xLow >> xHigh >> yLow >> yHigh;
	
	cout << "Nice. Now introduce the contrast and thr brightness of the image: " << endl;
	double contrast, brightness;
	cin >> contrast >> brightness;
	
	module::Perlin basicMap;
	basicMap.SetFrequency(1.0); //Altough I think the default value is 1.0
	basicMap.SetLacunarity(2.0);
	basicMap.SetNoiseQuality(QUALITY_BEST);
	basicMap.SetOctaveCount(16);
	basicMap.SetPersistence(0.5);
	basicMap.SetSeed(seed);
	
	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane heightMapBuilder;
	//heightMapBuilder.SetSourceModule(/* NAME FINAL TERRAIN */);
	heightMapBuilder.SetSourceModule(basicMap);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(width, height);
	heightMapBuilder.SetBounds(xLow, xHigh, yLow, yHigh);
	heightMapBuilder.Build();

	utils::NoiseMap myMask;
	myMask.SetSize(width, height);
	
	// Blood Moon Generator
	double dx, dy, d, maxw, delta, gradient;
	for(int x = 0; x < width; x++){
		for(int y = 0; y < height; y++){
			dx = abs(x - width*0.5);
			dy = abs(y - width*0.5);
			d = sqrt(dx*dx + dy*dy);
			
			maxw = height * 0.5 - 10.0;
			delta = d / maxw;
			gradient = delta * delta;
			
			double value = max (0.0, 1.0 - gradient);
			double vmap = heightMap.GetValue(x, y);
			myMask.SetValue(x, y, vmap*value);
		}
	}

	utils::RendererImage renderer;
	utils::Image image;
	//renderer.SetSourceNoiseMap (heightMap);
	renderer.SetSourceNoiseMap (myMask);
	renderer.SetDestImage (image);
	renderer.ClearGradient ();
	
	renderer.AddGradientPoint (-1.0000, utils::Color(255,   0,   0, 255));
	renderer.AddGradientPoint ( 0.0000, utils::Color(  0,   0,   0, 255));
	renderer.AddGradientPoint ( 1.0000, utils::Color(255,   0,   0, 255));
	renderer.EnableLight ();
	renderer.SetLightContrast (contrast); //3.0
	renderer.SetLightBrightness (brightness); //2.0
	renderer.Render ();
	
	utils::WriterBMP Writer;
	Writer.SetSourceImage(image);
	
	Writer.SetDestFilename(std::to_string(seed) + ".bmp");
	Writer.WriteDestFile();
	
	cout << "Check the directory. It contains the generated Blood Moon, it seams broken!" << endl;
}
