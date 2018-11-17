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
//	basicMap.SetFrequency(1.0); 
	basicMap.SetFrequency(1.25); 
	basicMap.SetLacunarity(2.5);
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
			heightMap.SetValue(i, j, (heightMap.GetValue(i, j) - minnh ) / ( maxnh - minnh )+0.05);
		}
	}
	
	// Circle filter
	double dx, dy, d, maxw, delta, gradient;
	for(int x = 0; x < width; x++){
		for(int y = 0; y < height; y++){
			dx = abs(x - width*0.5);
			dy = abs(y - width*0.5);
			// Island
			d = sqrt(dx*dx + dy*dy);
			// More like a continent
			//d = max(dx, dy);
			
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

	/*utils::NoiseMap POI;
	POI.SetSize(width, height);
	for(int i = 0; i < width; i++)
		for(int j = 0; j < height; j++)
			POI.SetValue(i, j, 0.0000);
			
	int nPoi = rand() % height;
	vector<Point> locationPoi (nPoi);
	
	for(int i = 0; i < nPoi; i++){
		locationPoi[i].x = rand() % width;
		locationPoi[i].y = rand() % height;
	}
	
	for(int i = 0; i < locationPoi.size(); i++)
		POI.SetValue(locationPoi[i].x, locationPoi[i].y, 1.0000);*/
			



	/*vector<double> rbiomes(8);
	for(int i = 0; i < rbiomes.size(); i++) rbiomes[i] = (double) (rand() % 256 / (256.0));
	int i1 = (rand() % rbiomes.size());
	int i2 = (rand() % rbiomes.size());
	double b1 = rbiomes[(rand() % rbiomes.size())]; 
	double b2 = rbiomes[(rand() % 7)];
	utils::NoiseMap biomes;
	biomes.SetSize(width, height);
	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			if((i>=j) and (i<(width/2))){
				biomes.SetValue(i, j, b1);
				//if(heightMap.GetValue(i, j) >= 0.5) heightMap.SetValue(i,j, -heightMap.GetValue(i,j));
			}else{
				biomes.SetValue(i,j, b2);
			}
//			biomes.SetValue(i, j, (double)i/width + (double)j/height);
		}
	}
	// */
	
	
	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap (heightMap);
	renderer.SetDestImage (image);
	renderer.ClearGradient ();
	
	//Render para mapas normalizados
	renderer.AddGradientPoint (-1.0000, utils::Color (  0,   0,  64, 255));
	renderer.AddGradientPoint ( 0.1000, utils::Color (  0,   0, 255, 255)); // shallow
	renderer.AddGradientPoint ( 0.3250, utils::Color (  0, 128, 255, 255)); // shore
	renderer.AddGradientPoint ( 0.3500, utils::Color (240, 240, 128, 255)); // sand
	renderer.AddGradientPoint ( 0.4000, utils::Color ( 32, 200,   0, 255)); // grass 
	renderer.AddGradientPoint ( 0.6500, utils::Color (223, 191, 159, 255)); // dirt
	renderer.AddGradientPoint ( 0.7500, utils::Color (122, 102,  82, 255)); // rock
	renderer.AddGradientPoint ( 1.0000, utils::Color (255, 255, 255, 255)); // Snow */
	
	renderer.EnableLight ();
	renderer.SetLightContrast (contrast); //3.0
	renderer.SetLightBrightness (brightness); //2.0
	renderer.Render ();
	
	utils::WriterBMP Writer;
	Writer.SetSourceImage(image);
	
	Writer.SetDestFilename(std::to_string(seed) + ".bmp");
	Writer.WriteDestFile();
	
	/*utils::RendererImage r;
	utils::Image i;
	//r.SetSourceNoiseMap (biomes);
	r.SetSourceNoiseMap(POI);
	r.SetDestImage (i);
	r.ClearGradient ();
	r.AddGradientPoint(0.0000, utils::Color(0,0,0,255));
	r.AddGradientPoint(1.0000, utils::Color(255,255,255,255));
	r.EnableLight();
	r.SetLightContrast(contrast);
	r.SetLightBrightness(brightness);
	r.Render();
	utils::WriterBMP w;
	w.SetSourceImage(i);
	w.SetDestFilename(std::to_string(seed) + "_biome.bmp");
	w.WriteDestFile();//*/
	
	
	
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