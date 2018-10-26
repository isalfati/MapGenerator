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

int main()
{
	// This ensures a new seed on each execution
	srand(time(0));
	int seed = rand() % INT32_MAX;
	
	cout << "This is the Seed for this map: " << seed << endl;
	
	// Octaves     = Function with a certain frequency, each consecutive octave doubles the last frequency.
	// Frequency   = How many changes ocurr along a unit lenght. Increasing the frequency will increase the number of terrain features and also decreses the size of those features
	// Persistence = The persistence determines how quickly the amplitudes fall for each successive octave. More persistance will create rougher terrains.
	// 
	cout << "Please, introduce the size of the image (width, height), the bounds of a square (xLow, xHigh, yLow, yHigh), " << endl;
	cout << "the number of octaves (1-6, the bigger, the better results), the frequency (1-300), and the persistance (0-1, the bigger, the rougher terrains)" << endl;

	int width, height;
    cin >> width >> height;
	double xLow, xHigh, yLow, yHigh;
	cin >> xLow >> xHigh >> yLow >> yHigh;
	
	// Base Flat Terrain
	module::Billow BaseFlatTerrain;
	
	// Base Mountain Terrain
	module::RidgedMulti MountainTerrain;
	
	// Noise Map as a Controller
	module::Perlin ControlMap;
	
	BaseFlatTerrain.SetSeed(seed);
	MountainTerrain.SetSeed(seed);
	
	ControlMap.SetSeed(seed);
	ControlMap.SetFrequency(0.4);
	ControlMap.SetPersistence(0.55);
	
	module::ScaleBias FlatTerrain;
	FlatTerrain.SetSourceModule(0, ControlMap);
	FlatTerrain.SetScale(0.125);
	FlatTerrain.SetBias(1);
	
	module::Select FinalTerrain;
	FinalTerrain.SetSourceModule(0, BaseFlatTerrain);
	FinalTerrain.SetSourceModule(1, MountainTerrain);
	FinalTerrain.SetControlModule(ControlMap);
	FinalTerrain.SetBounds(0.0, 1000.0);
	FinalTerrain.SetEdgeFalloff(0.25);
	
	utils::NoiseMap HeightMap;
	utils::NoiseMapBuilderPlane NoiseMapBuilderPlane;
	
	
	NoiseMapBuilderPlane.SetSourceModule(ControlMap);
	NoiseMapBuilderPlane.SetDestNoiseMap(HeightMap);
	NoiseMapBuilderPlane.SetDestSize(width, height);
	NoiseMapBuilderPlane.SetBounds(xLow, xHigh, yLow, yHigh);
	
	NoiseMapBuilderPlane.Build();
	
	utils::RendererImage Renderer;
	utils::Image Image;
	Renderer.SetSourceNoiseMap(HeightMap);
	Renderer.SetDestImage(Image);
	
	Renderer.ClearGradient();
	Renderer.AddGradientPoint (-1.0000, utils::Color (  0,   0, 128, 255)); // deeps
	Renderer.AddGradientPoint (-0.2500, utils::Color (  0,   0, 255, 255)); // shallow
	Renderer.AddGradientPoint (-0.0625, utils::Color (  0, 128, 255, 255)); // shore
	Renderer.AddGradientPoint ( 0.0000, utils::Color (240, 240,  64, 255)); // sand
	Renderer.AddGradientPoint ( 0.1250, utils::Color ( 32, 160,   0, 255)); // grass
	Renderer.AddGradientPoint ( 0.3750, utils::Color (224, 224,   0, 255)); // dirt
	Renderer.AddGradientPoint ( 0.7500, utils::Color (128, 128, 128, 255)); // rock
	Renderer.AddGradientPoint ( 1.0000, utils::Color (255,   0,   0, 255)); // Lava 
	Renderer.EnableLight();
	Renderer.SetLightContrast(1.0);
	Renderer.SetLightBrightness(2.0);
	Renderer.Render();	
		
	utils::WriterBMP Writer;
	Writer.SetSourceImage(Image);
	
	
	Writer.SetDestFilename("myExample.bmp");
	Writer.WriteDestFile();

	cout << "Check the directory. It contains the generated noise image." << endl; 
	
}