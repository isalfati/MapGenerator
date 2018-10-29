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
	
	module::Cylinders water;
	water.SetFrequency(2);
	
	module::Perlin waterGrainNoise;
	waterGrainNoise.SetSeed(seed);
	waterGrainNoise.SetFrequency(48.0);
	waterGrainNoise.SetPersistence(0.5);
	waterGrainNoise.SetLacunarity(2.735858);
	waterGrainNoise.SetOctaveCount(3);
	waterGrainNoise.SetNoiseQuality(QUALITY_STD);
	
	module::ScalePoint scaledWaterGrain;
	scaledWaterGrain.SetSourceModule(0, waterGrainNoise);
	scaledWaterGrain.SetYScale(0.25);
	
	module::ScaleBias waterGrain;
	waterGrain.SetSourceModule(0, scaledWaterGrain);
	waterGrain.SetScale(0.25);
	waterGrain.SetBias(0.125);
	
	module::Add combinedWater;
	combinedWater.SetSourceModule(0, water);
	combinedWater.SetSourceModule(1, waterGrain);
	
	module::Turbulence perturbedWater;
	perturbedWater.SetSourceModule(0, combinedWater);
	perturbedWater.SetSeed(seed);
	perturbedWater.SetFrequency(4.0);
	perturbedWater.SetPower(1.0/256.0);
	perturbedWater.SetRoughness(4);
	
	module::TranslatePoint translatedWater;
	translatedWater.SetSourceModule(0, perturbedWater);
	translatedWater.SetXTranslation(2.0);
	
	
	module::Billow baseFlatTerrain;
	baseFlatTerrain.SetSeed(seed);
	baseFlatTerrain.SetFrequency(3.0);
	
	module::ScaleBias flatTerrain;
	flatTerrain.SetSourceModule(0, baseFlatTerrain);
	flatTerrain.SetScale(0.125);
	flatTerrain.SetBias(-0.5);
	
	// Type of Terrain
	module::Perlin terrainType;
	terrainType.SetSeed(seed);	
	terrainType.SetFrequency(0.5);
	terrainType.SetPersistence(0.25);
	
	// Selector of Terrain based on Perlin terrainType
	module::Select terrainSelector;
	terrainSelector.SetSourceModule(0, flatTerrain);
	//terrainSelector.SetSourceModule(1, water);
	terrainSelector.SetSourceModule(1, translatedWater);
	terrainSelector.SetControlModule(terrainType);
	terrainSelector.SetBounds(0.0, 1000.0);
	terrainSelector.SetEdgeFalloff(0.125);
	
	module::Turbulence finalTerrain;
	finalTerrain.SetSourceModule(0, terrainSelector);
	finalTerrain.SetFrequency(4.0);
	finalTerrain.SetPower(0.125);
	
	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(perturbedWater);
	//heightMapBuilder.SetSourceModule(baseFlatTerrain);
	//heightMapBuilder.SetSourceModule(flatTerrain);
	//heightMapBuilder.SetSourceModule(terrainSelector);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(width, height);
	heightMapBuilder.SetBounds(xLow, xHigh, yLow, yHigh);
	heightMapBuilder.Build();

	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap (heightMap);
	renderer.SetDestImage (image);
	renderer.ClearGradient ();
	
	//Bluish gradient
	renderer.AddGradientPoint (-1.0000, utils::Color( 30,  87, 153, 255));
	//renderer.AddGradientPoint (-0.6000, utils::Color( 41, 137, 216, 255));
	renderer.AddGradientPoint ( 1.000, utils::Color(125, 185, 232, 255));
	//renderer.AddGradientPoint ( 1.0000, utils::Color(255, 255, 255, 255));
	
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

/*

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
	heightMapBuilder.EnableSeamless(false);
	heightMapBuilder.Build();
	
	//Idea: Generar un map y utilizar un circular para intentar generar una isla

	// Común a todos los mapas.
	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap (heightMap);
	renderer.SetDestImage (image);
	renderer.ClearGradient ();
	renderer.AddGradientPoint (-1.0000 + SEA_LEVEL, utils::Color (  0,   0, 128, 255)); // deeps
	renderer.AddGradientPoint (-0.2500 + SEA_LEVEL, utils::Color (  0,   0, 255, 255)); // shallow
	renderer.AddGradientPoint ( 0.0000 + SEA_LEVEL, utils::Color (  0, 128, 255, 255)); // shore
	renderer.AddGradientPoint ( 0.0625 + SEA_LEVEL, utils::Color (240, 240,  64, 255)); // sand
	renderer.AddGradientPoint ( 0.1250 + SEA_LEVEL, utils::Color ( 32, 160,   0, 255)); // grass
	renderer.AddGradientPoint ( 0.8750, utils::Color (224, 224,   0, 255)); // dirt
	renderer.AddGradientPoint ( 1.0000, utils::Color (128, 128, 128, 255)); // rock
	//renderer.AddGradientPoint ( 0.3750, utils::Color (224, 224,   0, 255)); // dirt
	//renderer.AddGradientPoint ( 0.8000, utils::Color (128, 128, 128, 255)); // rock
	//renderer.AddGradientPoint ( 1.0000, utils::Color (255, 255, 255, 255)); // snow
	renderer.EnableLight ();
	renderer.SetLightContrast (3.0);
	renderer.SetLightBrightness (2.0);
	renderer.Render ();
	
	utils::WriterBMP Writer;
	Writer.SetSourceImage(image);
	
	Writer.SetDestFilename(tswp(seed) + ".bmp");
	Writer.WriteDestFile();

	cout << "Check the directory. It contains the generated map." << endl; 
}*/