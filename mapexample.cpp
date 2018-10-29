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
	
	cout << "This is the seed for this generated map: " << seed << endl;
	
	int width, height;
	cin >> width >> height;
	double xLow, xHigh, yLow, yHigh;
	cin >> xLow >> xHigh >> yLow >> yHigh;
	
	module::Perlin basicMap;
	basicMap.SetFrequency(1.0); //Altough I think it is already defaulted at 1.
	basicMap.SetLacunarity(2.0);
	basicMap.SetNoiseQuality(QUALITY_BEST);
	basicMap.SetOctaveCount(16);
	basicMap.SetPersistence(0.5);
	basicMap.SetSeed(seed);
	
	utils::NoiseMap heightMap, myMask;
	utils::NoiseMapBuilderPlane heightMapBuilder;
	//heightMapBuilder.SetSourceModule(/* NAME FINAL TERRAIN */);
	heightMapBuilder.SetSourceModule(basicMap);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(width, height);
	heightMapBuilder.SetBounds(xLow, xHigh, yLow, yHigh);
	heightMapBuilder.Build();

	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap (heightMap);
	renderer.SetDestImage (image);
	renderer.ClearGradient ();
	
	renderer.AddGradientPoint (-1.0000, utils::Color(  0,   0,   0, 255));
	renderer.AddGradientPoint ( 1.0000, utils::Color(255, 255, 255, 255));
	/*renderer.AddGradientPoint (-1.0000, utils::Color (  0,   0, 128, 255)); // deeps
	renderer.AddGradientPoint (-0.2500, utils::Color (  0,   0, 255, 255)); // shallow
	renderer.AddGradientPoint (-0.0625, utils::Color (  0, 128, 255, 255)); // shore
	renderer.AddGradientPoint ( 0.0000, utils::Color (240, 240,  64, 255)); // sand
	renderer.AddGradientPoint ( 0.1250, utils::Color ( 32, 160,   0, 255)); // grass
	renderer.AddGradientPoint ( 0.3750, utils::Color (224, 224,   0, 255)); // dirt
	renderer.AddGradientPoint ( 0.7500, utils::Color (128, 128, 128, 255)); // rock
	renderer.AddGradientPoint ( 1.0000, utils::Color (255, 255, 255, 255)); // snow*/
	
	renderer.EnableLight ();
	renderer.SetLightContrast (3.0);
	renderer.SetLightBrightness (2.0);
	renderer.Render ();	
	
	utils::WriterBMP Writer;
	Writer.SetSourceImage(image);
	
	Writer.SetDestFilename(tswp(seed) + ".bmp");
	Writer.WriteDestFile();
	
	cout << "Check the directory. It contains the generated map." << endl;
}