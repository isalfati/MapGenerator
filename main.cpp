#include <iostream>
#include <noise/noise.h>
#include "noiseutils.cpp"
#include <ctime>
#include <random>
#include <sstream>

using namespace std;
using namespace noise;
using namespace noise::utils;

// To String With Precision
string tswp(double value, int n = 3)
{
	ostringstream out;
	out.precision(n);
	out << fixed << value;
	return out.str();
}

struct Point{
	int x, y;
};

int main()
{
	// This ensures a new seed on each execution
	srand(time(0));
	int seed = rand() % INT32_MAX;
	
	cout << "This is the Seed for this map: " << seed << endl;
	
	cout << "Please, introduce the size of the image (width, height), and xLow, xHigh, yLow, yHigh." << endl;

	int width, height;
    cin >> width >> height;
	double xLow, xHigh, yLow, yHigh;
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

	// Create your own gradient: http://www.colorzilla.com/gradient-editor/ and add as many points as you want.
	renderer.ClearGradient ();
	renderer.AddGradientPoint (-1.0000, utils::Color (  0,   0, 128, 255)); // deeps
	renderer.AddGradientPoint (-0.2500, utils::Color (  0,   0, 255, 255)); // shallow
	renderer.AddGradientPoint (-0.0625, utils::Color (  0, 128, 255, 255)); // shore
	renderer.AddGradientPoint ( 0.0000, utils::Color (240, 240,  64, 255)); // sand
	renderer.AddGradientPoint ( 0.1250, utils::Color ( 32, 160,   0, 255)); // grass
	renderer.AddGradientPoint ( 0.3750, utils::Color (224, 224,   0, 255)); // dirt
	renderer.AddGradientPoint ( 0.7500, utils::Color (128, 128, 128, 255)); // rock
	renderer.AddGradientPoint ( 1.0000, utils::Color (255, 255, 255, 255)); // snow

	renderer.EnableLight();
	renderer.SetLightContrast(1.0);
	renderer.SetLightBrightness(2.0);
	renderer.Render();

	//Test to check if I can add POI to my map.
	int max_poi = rand() % (width/10);
	cout << "Added " << max_poi << " Points of Interest to the map." << endl;
	vector<Point> POI(max_poi);
	Point p;
	
	for(int i = 0; i < POI.size(); i++){
		p.x = rand() % width + 1;
		p.y = rand() % height + 1;
		POI[i] = p;
	}
	
	utils::Color  sea(255, 235,  0, 255);
	utils::Color land(  0,   0,  0, 255);
	utils::Color type;
	
	
	//cout << "POI IN: " << xx << ", " << yy << endl;
	for(int k = 0; k < POI.size(); k++){
		Point p = POI[k];
		//cout << "Valor consultado: " << heightMap.GetValue(p.x,p.y) << endl;
		if(heightMap.GetValue(p.x,p.y) < 0.0000){
			type = sea;
		}else type = land;
		for(int i = -1; i < 2; i++){
			for(int j = -1; j < 2; j++){
				image.SetValue(p.x + i, p.y + j, type);
			}
		}
	}

	utils::WriterBMP writer;
	writer.SetSourceImage(image);

	// Naming file with the input parameters.
	auto str = std::to_string(seed);
	auto parameters = tswp(xLow) + "_" + tswp(xHigh) + "_" + 
					  tswp(yLow) + "_" + tswp(yHigh) + "_" + 
					  std::to_string(width) + "x" + std::to_string(height);
	auto filename = str + "_" + parameters;

	writer.SetDestFilename(filename + ".bmp");
	//writer.SetDestFilename("./img/" + filename + ".bmp");
	writer.WriteDestFile();

	cout << "Check the directory. It contains the generated noise image." << endl; 
}
