#include <iostream>
#include <noise/noise.h>
#include "noiseutils.cpp"
#include <ctime>
#include <random>

using namespace std;
using namespace noise;
using namespace noise::utils;

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
	//double seed = dRand(0.0, 1.0);
	int seed = rand() % INT32_MAX;
	cout << "dRand(0.0, 1.0): " << seed << endl;
	
	cout << "Please, introduce the size of the image: " << endl;
	int width, height;
    cin >> width >> height;
	double xLow, xHigh, yLow, yHigh;
	cout << "Please, introduce xLow, xHigh, yLow, yHigh values of the map: " << endl;
	cin >> xLow >> xHigh >> yLow >> yHigh;
	
	module::Perlin myPerlin;
	
	//A new Map each time:
	myPerlin.SetSeed(seed);
	
	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule (myPerlin);
	heightMapBuilder.SetDestNoiseMap (heightMap);
	heightMapBuilder.SetDestSize(width, height);
	heightMapBuilder.SetBounds(xLow, xHigh, yLow, yHigh);
	heightMapBuilder.Build();
    
	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap(heightMap);
	renderer.SetDestImage(image);
	
	// Create your own gradient: http://www.colorzilla.com/gradient-editor/
	renderer.ClearGradient ();
	renderer.AddGradientPoint (-1.0000, utils::Color (  0,   0, 128, 255)); // deeps
	renderer.AddGradientPoint (-0.2500, utils::Color (  0,   0, 255, 255)); // shallow
	renderer.AddGradientPoint (-0.0625, utils::Color (  0, 128, 255, 255)); // shore
	renderer.AddGradientPoint ( 0.0000, utils::Color (240, 240,  64, 255)); // sand
	renderer.AddGradientPoint ( 0.1250, utils::Color ( 32, 160,   0, 255)); // grass
	renderer.AddGradientPoint ( 0.3750, utils::Color (224, 224,   0, 255)); // dirt
	renderer.AddGradientPoint ( 0.7500, utils::Color (128, 128, 128, 255)); // rock
	renderer.AddGradientPoint ( 1.0000, utils::Color (255, 255, 255, 255)); // snow	
	
	renderer.Render();
	
	utils::WriterBMP writer;
	writer.SetSourceImage(image);
	
	auto str = std::to_string(seed);
	writer.SetDestFilename(str + ".bmp");
	writer.WriteDestFile();

    cout << "Check the directory. It contains the generated noise image." << endl; 
}
