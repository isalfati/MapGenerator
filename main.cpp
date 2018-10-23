#include <iostream>
#include "bitmap_image.hpp"
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
	double seed = dRand(0.0, 1.0);
	cout << "dRand(0.0, 1.0): " << seed << endl;
	
	cout << "Please, introduce the size of the image: " << endl;
	int width, height;
    cin >> width >> height;
	double xLow, xHigh, yLow, yHigh;
	cout << "Please, introduce xLow, xHigh, yLow, yHigh values of the map: " << endl;
	cin >> xLow >> xHigh >> yLow >> yHigh;
	
	module::Perlin myPerlin;
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
	renderer.Render();
	
	utils::WriterBMP writer;
	writer.SetSourceImage(image);
	
	auto str = std::to_string(seed);
	writer.SetDestFilename(str + ".bmp");
	writer.WriteDestFile();
	
    cout << "Check the directory. It contains the generated noise image." << endl; 
}
