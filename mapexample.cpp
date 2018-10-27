// billow and rigged

#include <iostream>
#include <noise/noise.h>
#include "noiseutils.cpp"
#include <ctime>
#include <random>
#include <sstream>

using namespace std;
using namespace noise;
using namespace noise::utils;

string tswp(double value, int n = 3)
{
	ostringstream out;
	out.precision(n);
	out << fixed << value;
	return out.str();
}

int main()
{
	// This ensures a new seed on each execution
	srand(time(0));
	int seed = rand() % INT32_MAX;
	
	cout << "This is the Seed for this map: " << seed << endl;
	
	cout << "Please, introduce the size of the image (width, height), the bounds of a square (xLow, xHigh, yLow, yHigh), " << endl;
	cout << "the number of octaves (1-6, the bigger, the better results), the frequency (1-300), and the persistance (0-1, the bigger, the rougher terrains)" << endl;

	int width, height;
    cin >> width >> height;
	double xLow, xHigh, yLow, yHigh;
	cin >> xLow >> xHigh >> yLow >> yHigh;
	
	// Octaves     = Function with a certain frequency, each consecutive octave doubles the last frequency.
	// Frequency   = How many changes ocurr along a unit lenght. Increasing the frequency will increase the number of terrain features and also decreses the size of those features
	// Persistence = The persistence determines how quickly the amplitudes fall for each successive octave. More persistance will create rougher terrains.

	module::Perlin myModule;
	myModule.SetSeed(seed);
	myModule.SetOctaveCount(10);

	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(myModule);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(width, height); 
	heightMapBuilder.SetBounds(xLow, xHigh, yLow, yHigh);
	heightMapBuilder.Build();



//Idea: Generar un map y utilizar un circular para intentar generar una isla



	// Común a todos los mapas.
	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap (heightMap);
	renderer.SetDestImage (image);
	renderer.ClearGradient ();
	renderer.AddGradientPoint (-1.0000, utils::Color (  0,   0, 128, 255)); // deeps
	renderer.AddGradientPoint (-0.2500, utils::Color (  0,   0, 255, 255)); // shallow
	renderer.AddGradientPoint ( 0.0000, utils::Color (  0, 128, 255, 255)); // shore
	renderer.AddGradientPoint ( 0.0625, utils::Color (240, 240,  64, 255)); // sand
	renderer.AddGradientPoint ( 0.1250, utils::Color ( 32, 160,   0, 255)); // grass
	renderer.AddGradientPoint ( 0.3750, utils::Color (224, 224,   0, 255)); // dirt
	renderer.AddGradientPoint ( 0.7500, utils::Color (128, 128, 128, 255)); // rock
	renderer.AddGradientPoint ( 1.0000, utils::Color (255, 255, 255, 255)); // snow
	//renderer.AddGradientPoint ( 0.8700, utils::Color (96, 96, 96, 255));
	//renderer.AddGradientPoint ( 1.0000, utils::Color (255, 0, 0, 255));
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