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
	double contrast, brightness;
	cin >> contrast >> brightness;
	
	module::Perlin basicMap;
	basicMap.SetFrequency(1.0); //Altough I think it is already defaulted at 1.
	//basicMap.SetFrequency(1.5);
	basicMap.SetLacunarity(2);
	basicMap.SetNoiseQuality(QUALITY_BEST);
	basicMap.SetOctaveCount(16);
	basicMap.SetPersistence(0.5);
	basicMap.SetSeed(seed);
	
	module::RidgedMulti ridgedMap;
	ridgedMap.SetFrequency(1.0);
	ridgedMap.SetLacunarity(2);
	ridgedMap.SetNoiseQuality(QUALITY_BEST);
	ridgedMap.SetOctaveCount(1);
	ridgedMap.SetSeed(seed/2);
	
	utils::NoiseMap heightMap, riverMap;
	utils::NoiseMapBuilderPlane heightMapBuilder, riverMapBuilder;
	//heightMapBuilder.SetSourceModule(/* NAME FINAL TERRAIN */);
	heightMapBuilder.SetSourceModule(basicMap);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(width, height);
	heightMapBuilder.SetBounds(xLow, xHigh, yLow, yHigh);
	heightMapBuilder.Build();

	riverMapBuilder.SetSourceModule(ridgedMap);
	riverMapBuilder.SetDestNoiseMap(riverMap);
	riverMapBuilder.SetDestSize(width, height);
	riverMapBuilder.SetBounds(xLow, xHigh, yLow, yHigh);
	riverMapBuilder.Build();

	utils::NoiseMap mapNorm;
	mapNorm.SetSize(width, height);
	
	double nh, maxnh, minnh;
	maxnh = -10.0;
	minnh =  10.0;
	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			nh = heightMap.GetValue(i, j);
			
			if(nh > maxnh) maxnh = nh;
			else if(nh < minnh) minnh = nh;
			
			heightMap.SetValue(i, j, nh);
		}
	}
	
	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			heightMap.SetValue(i, j, (heightMap.GetValue(i, j) - minnh ) / ( maxnh - minnh ));
		}
	}
	
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
			
			//cout << "value: " << value << ", vmap: " << vmap << " - ";
			heightMap.SetValue(x, y, vmap * value);
		}
		//cout << endl;
	}

	for( int x = 0; x < width; x++){
		for(int y = 0; y < height; y++){
			double v = riverMap.GetValue(x, y);
			if( v > 0.2000){
				//cout << v << " " << endl;
				
				heightMap.SetValue(x, y, -0.99);
			}
		}
	}

	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap (heightMap);
	//renderer.SetSourceNoiseMap (riverMap);
	renderer.SetDestImage (image);
	renderer.ClearGradient ();
	
	/*renderer.AddGradientPoint (-1.0000, utils::Color (255,   0,   0, 255));
	renderer.AddGradientPoint ( 0.0000, utils::Color (255, 255, 255, 255));
	renderer.AddGradientPoint ( 1.0000, utils::Color (  0,   0,   0, 255));*/
	
	renderer.AddGradientPoint (-1.0000, utils::Color (  0,   0,  64, 255));
	renderer.AddGradientPoint ( 0.0000, utils::Color (  0,   0, 128, 255)); //deeps
	renderer.AddGradientPoint ( 0.2000, utils::Color (  0,   0, 255, 255)); //shallow
	renderer.AddGradientPoint ( 0.3000, utils::Color (  0, 128, 255, 255)); //shore
	renderer.AddGradientPoint ( 0.3500, utils::Color (240, 240, 128, 255)); //sand
	//renderer.AddGradientPoint ( 0.5500, utils::Color (224, 224,   0, 255)); //dirt
	renderer.AddGradientPoint ( 0.5500, utils::Color (223, 191, 159, 255)); //dirt
	renderer.AddGradientPoint ( 0.4500, utils::Color ( 32, 200,   0, 255)); //grass 
	renderer.AddGradientPoint ( 0.7500, utils::Color (128, 128, 128, 255)); //rock
	renderer.AddGradientPoint ( 0.9000, utils::Color (207,  16,  32, 255)); // Lava
	renderer.AddGradientPoint ( 1.0000, utils::Color (255,   0,   0, 255)); //Pure RED*/
	
	renderer.EnableLight ();
	renderer.SetLightContrast (contrast); //3.0
	renderer.SetLightBrightness (brightness); //2.0
	renderer.Render ();
	
	utils::WriterBMP Writer;
	Writer.SetSourceImage(image);
	
	Writer.SetDestFilename(std::to_string(seed) + ".bmp");
	Writer.WriteDestFile();
	
	cout << "Check the directory. It contains the generated map." << endl;
}