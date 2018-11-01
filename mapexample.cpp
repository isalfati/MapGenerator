#include <iostream>
#include <noise/noise.h>
#include "noiseutils.cpp"
#include <algorithm>
#include <ctime>
#include <random>
#include <sstream>

using namespace std;
using namespace noise;
using namespace noise::utils;

struct Point{
	int x, y;
	double value;
};

bool operator<(const Point& a, const Point& b)
{
	return a.value < b.value;
}

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
	basicMap.SetLacunarity(2.2);
	basicMap.SetNoiseQuality(QUALITY_BEST);
	basicMap.SetOctaveCount(16);
	basicMap.SetPersistence(0.5);
	//basicMap.SetSeed(2002235257);
	basicMap.SetSeed(283802548);
	
	module::Curve basicMapCurved;
	basicMapCurved.SetSourceModule(0, basicMap);
	/*basicMapCurved.AddControlPoint(-2.0000, -1.625);
	basicMapCurved.AddControlPoint(-1.0000, -1.375);
	basicMapCurved.AddControlPoint( 0.0000, -0.375);
	basicMapCurved.AddControlPoint( 0.0625,  0.125);
	basicMapCurved.AddControlPoint( 0.1250,  0.250);
	basicMapCurved.AddControlPoint( 0.2500,  1.000);
	basicMapCurved.AddControlPoint( 0.5000,  0.250);
	basicMapCurved.AddControlPoint( 0.7500,  0.250);
	basicMapCurved.AddControlPoint( 1.0000,  0.500);
	basicMapCurved.AddControlPoint( 2.0000,  0.500);//*/
	
	//Seguir desde linea 153
	basicMapCurved.AddControlPoint(-1.0000,  1.0000);
	basicMapCurved.AddControlPoint(-0.5000,  0.5000);
	basicMapCurved.AddControlPoint( 0.0000,  0.0000);
	basicMapCurved.AddControlPoint( 0.5000, -0.5000);
	basicMapCurved.AddControlPoint( 1.0000, -1.0000);
	
	
	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane heightMapBuilder;
	//heightMapBuilder.SetSourceModule(/* NAME FINAL TERRAIN */);
	//heightMapBuilder.SetSourceModule(basicMap);
	heightMapBuilder.SetSourceModule(basicMapCurved);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(width, height);
	heightMapBuilder.SetBounds(xLow, xHigh, yLow, yHigh);
	heightMapBuilder.Build();

	// Normalise
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
	
	// Circle filter
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
	}//*/
	
	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap (heightMap);
	renderer.SetDestImage (image);
	renderer.ClearGradient ();
	

	/*renderer.AddGradientPoint (-1.0000, utils::Color (255,   0,   0, 255));
	renderer.AddGradientPoint ( 0.0000, utils::Color (255, 255, 255, 255));
	renderer.AddGradientPoint ( 1.0000, utils::Color (  0,   0,   0, 255));//*/
	
	//Render standard
	/*renderer.AddGradientPoint (-1.0000, utils::Color (  0,   0, 128, 255)); // deeps
	renderer.AddGradientPoint (-0.2500, utils::Color (  0,   0, 255, 255)); // shallow
	renderer.AddGradientPoint (-0.0625, utils::Color (  0, 128, 255, 255)); // shore
	renderer.AddGradientPoint ( 0.0000, utils::Color (240, 240,  64, 255)); // sand
	renderer.AddGradientPoint ( 0.1250, utils::Color ( 32, 160,   0, 255)); // grass
	renderer.AddGradientPoint ( 0.3750, utils::Color (224, 224,   0, 255)); // dirt
	renderer.AddGradientPoint ( 0.7500, utils::Color (128, 128, 128, 255)); // rock
	renderer.AddGradientPoint ( 1.0000, utils::Color (255, 255, 255, 255)); // snow */
	
	
	//Render para mapas normalizados
	renderer.AddGradientPoint (-1.0000, utils::Color (  0,   0,  64, 255));
	renderer.AddGradientPoint ( 0.0000, utils::Color (  0,   0, 128, 255)); // deeps
	renderer.AddGradientPoint ( 0.1000, utils::Color (  0,   0, 255, 255)); // shallow
	renderer.AddGradientPoint ( 0.2000, utils::Color (  0, 128, 255, 255)); // shore
	renderer.AddGradientPoint ( 0.3500, utils::Color (240, 240, 128, 255)); // sand
	//renderer.AddGradientPoint ( 0.5500, utils::Color (224, 224,   0, 255)); // dirt
	renderer.AddGradientPoint ( 0.4500, utils::Color ( 32, 200,   0, 255)); // grass 
	renderer.AddGradientPoint ( 0.5500, utils::Color (223, 191, 159, 255)); // dirt
	renderer.AddGradientPoint ( 0.7500, utils::Color (128, 128, 128, 255)); // rock
	//renderer.AddGradientPoint ( 0.9000, utils::Color (207,  16,  32, 255)); // Lava
	//renderer.AddGradientPoint ( 1.0000, utils::Color (255,   0,   0, 255)); // Pure RED
	renderer.AddGradientPoint ( 0.9000, utils::Color (128, 128, 128, 255));
	renderer.AddGradientPoint ( 1.0000, utils::Color (255, 255, 255, 255)); // */
	
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

	/*	
	// This part searches and creates a path for rivers.
	int xmax, ymax;
	double minor = 0.0;
	for(int x = 0; x < width; x++){
		for(int y = 0; y < height; y++){
			double v = heightMap.GetValue(x, y);
			if( v > minor){
				minor = v;
				xmax = x; ymax = y;
			}
		}
	}
	bool shallow = false;
	Point local;
	local.x = xmax;
	local.y = ymax;
	local.value = heightMap.GetValue(local.x, local.y);
	int j = 0;
	vector<Point> neighbors (8);
	while(!shallow){
		int i = 0;
		for(int x = -1; x <= 1; x++){
			for(int y = -1; y <= 1; y++){
				if((x==0) and (y==0)) {
					// skip.
				}else{
					neighbors[i].x = local.x + x;
					neighbors[i].y = local.y + y;
					neighbors[i].value = heightMap.GetValue(neighbors[i].x, neighbors[i].y);
					i++;
				}
			}
		}
		// Select next point.
		cout << "Center: " << local.x << ", " << local.y << " with value: " << local.value << endl;
		cout << "Before sort" << endl;
		for(int j = 0; j < neighbors.size(); j++){
			cout << "P(" << neighbors[j].x << ", " << neighbors[j].y << ") has value: " << neighbors[j].value << endl;
		}
		std::sort(neighbors.begin(), neighbors.end());
		local = neighbors[0];
		cout << "New center: " << local.x << ", " << local.y << " with value: " << local.value << endl;
		cout << "After sort" << endl;
		for(int j = 0; j < neighbors.size(); j++){
			cout << "P(" << neighbors[j].x << ", " << neighbors[j].y << ") has value: " << neighbors[j].value << endl;
		}
		cout << "//" << endl;
		heightMap.SetValue(local.x, local.y, 1.0000);
		if(local.value <= 0.2000) shallow = true;
		//j++;
	}
	cout << "Position of the maximum value of the heightMap: [X,Y]: " << xmax <<  ", " << ymax << endl;*/