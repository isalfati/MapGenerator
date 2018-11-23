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

void printMap(Perlin map, int w, int h)
{
  for(int i = 0; i < w; i++){
    for(int j = 0; j < h; j++){
      double x = map.GetValue(i, j, 0);
      if(x >  1.0) x =  1.0;
      if(x < -1.0) x = -1,0;
      //cout << x << " ";
    }
    //cout << endl;
  }
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
    // We need to ensure that the seed is always random
    srand(time(0));
    int seed = rand() % INT32_MAX;

    cout << "This is the seed for the generated map: " << seed << endl;

    int width, height;
    cin >> width >> height;
    double xLow, xHigh, yLow, yHigh;
    cin >> xLow >> xHigh >> yLow >> yHigh;
    double contrast, brightness;
    cin >> contrast >> brightness;
    char shape, size;
    cin >> shape >> size;


    module::Perlin basicMap;
    //basicMap.SetFrequency(1.0);
    basicMap.SetFrequency(1.25);
    basicMap.SetLacunarity(2.5);
    basicMap.SetNoiseQuality(QUALITY_BEST);
    basicMap.SetOctaveCount(16);
    basicMap.SetPersistence(0.5);
    basicMap.SetSeed(seed);

    printMap(basicMap, width, height);
    /*utils::NoiseMap heightMapBasic;
    utils::NoiseMapBuilderPlane heightMapBasicBuilder;
    heightMapBasicBuilder.SetSourceModule(basicMap);
    heightMapBasicBuilder.SetDestNoiseMap(heightMapBasic);
    heightMapBasicBuilder.SetDestSize(width, height);
    heightMapBasicBuilder.SetBounds(xLow, xHigh, yLow, yHigh);
    heightMapBasicBuilder.Build();*/

    // --------------------------------------------------------- \\

    int seedMountain = rand() % INT32_MAX;
    cout << "This is the seed for the mountain terrain: " << seedMountain << endl;

    module::Perlin mountainTerrain;
    mountainTerrain.SetFrequency(2.5);
    mountainTerrain.SetLacunarity(2.5);
    mountainTerrain.SetNoiseQuality(QUALITY_BEST);
    mountainTerrain.SetOctaveCount(16);
    mountainTerrain.SetPersistence(0.5);
    mountainTerrain.SetSeed(seedMountain);

    printMap(mountainTerrain, width, height);
    /*module::ScaleBias bigHills;
    bigHills.SetSourceModule(0, mountainTerrain);
    bigHills.SetScale(0.25);
    bigHills.SetBias(0.25);// */

    // --------------------------------------------------------- \\

    int seedTerrain = rand() % INT32_MAX;
    cout << "This is the seed for the terrain selector: " << seedTerrain << endl;

    module::Perlin terrainType1;
    terrainType1.SetFrequency(1);
    terrainType1.SetLacunarity(2.5);
    terrainType1.SetNoiseQuality(QUALITY_BEST);
    terrainType1.SetOctaveCount(16);
    terrainType1.SetPersistence(0.5);
    terrainType1.SetSeed(seedTerrain);

    printMap(terrainType1, width, height);
    // --------------------------------------------------------- \\

    module::Select terrainPart1;
    terrainPart1.SetSourceModule(0, basicMap);
    //terrainPart1.SetSourceModule(1, bigHills);
    terrainPart1.SetSourceModule(1, mountainTerrain);
    terrainPart1.SetControlModule(terrainType1);
    terrainPart1.SetBounds(0.0, 1000.0);
    terrainPart1.SetEdgeFalloff(0.25);

    utils::NoiseMap heightMapTerrainPart1;
    utils::NoiseMapBuilderPlane heightMapTerrainPart1Builder;
    heightMapTerrainPart1Builder.SetSourceModule(terrainPart1);
    heightMapTerrainPart1Builder.SetDestNoiseMap(heightMapTerrainPart1);
    heightMapTerrainPart1Builder.SetDestSize(height, width);
    heightMapTerrainPart1Builder.SetBounds(xLow, xHigh, yLow, yHigh);
    heightMapTerrainPart1Builder.Build();

    // --------------------------------------------------------- \\

    /* for(int i = width/2 - 50; i < width/2 + 50; i++){
      for(int j = height/2 - 50; j < height/2 + 50; j++){
          heightMapTerrainPart1.SetValue(i,j, -1.0);
      }
    }// */

    // --------------------------------------------------------- \\

    // Normalise between 0 and 1, how to normalize between -0.5 and 1?
    double nh, maxnh, minnh;
    maxnh = -10.0;
    minnh =  10.0;
    for(int i = 0; i < width; i++){
      for(int j = 0; j < height; j++){
        nh = heightMapTerrainPart1.GetValue(i, j);

        if(nh > maxnh) maxnh = nh;
        else if(nh < minnh) minnh = nh;

        heightMapTerrainPart1.SetValue(i, j, nh);
      }
    }
    //cout << "Final values: " << endl;
    //cout << "minnh: " << minnh << ", maxnh: " << maxnh << endl;

    for(int i = 0; i < width; i++){
      for(int j = 0; j < height; j++){
          heightMapTerrainPart1.SetValue(i, j, (heightMapTerrainPart1.GetValue(i, j) - minnh) / ( maxnh - minnh));
      }
    }

    // Circle filter
  	double dx, dy, d, maxw, delta, gradient;
  	for(int x = 0; x < width; x++){
  		for(int y = 0; y < height; y++){
  			dx = abs(x - width*0.5);
  			dy = abs(y - width*0.5);
        if(shape == 'I'){
            // Island
  			    d = sqrt(dx*dx + dy*dy);
        }else if(shape == 'C'){
  			    // More like a
            d = max(dx, dy);
        }
  			//maxw = height*0.66 - width*0.1;// * 0.5 -100.0; // + 10.0;
        //maxw = height*0.5;
        if(size == 'P'){
            maxw = height * 0.35;
        }else if(size == 'M'){
            maxw = height * 0.40;
        }else if(size == 'G'){
            maxw = height * 0.45;
        }else if(size == 'E'){
            maxw = height * 0.55;
        }

  			delta = d / maxw;
  			gradient = delta * delta;

  			double value = max (0.0, 1.0 - gradient);
  			double vmap = heightMapTerrainPart1.GetValue(x, y);

  			//cout << "value: " << value << ", vmap: " << vmap << " - ";
  			heightMapTerrainPart1.SetValue(x, y, (vmap * value) + 0.1);
  		}
  		//cout << endl;
  	}//*/

    // --------------------------------------------------------- \\

    utils::RendererImage renderer;
    utils::Image image;
    renderer.SetSourceNoiseMap (heightMapTerrainPart1);
    renderer.SetDestImage (image);
    renderer.ClearGradient ();

    /*renderer.AddGradientPoint (-1.0000, utils::Color (  0,   0, 255, 255));
    renderer.AddGradientPoint (-0.5000, utils::Color (240, 240, 128, 255));
    renderer.AddGradientPoint ( 0.0000, utils::Color ( 32, 200,   0, 255));
    renderer.AddGradientPoint ( 0.6500, utils::Color (223, 191, 159, 255));
    renderer.AddGradientPoint ( 0.8500, utils::Color (122, 102,  82, 255));
    renderer.AddGradientPoint ( 1.0000, utils::Color (255, 255, 255, 255)); //*/

    //Render para mapas normalizados
    renderer.AddGradientPoint (-1.0000, utils::Color (  0,   0,  64, 255));
    renderer.AddGradientPoint ( 0.2000, utils::Color (  0,   0, 255, 255)); // shallow
    renderer.AddGradientPoint ( 0.3250, utils::Color (  0, 128, 255, 255)); // shore
    renderer.AddGradientPoint ( 0.3500, utils::Color (240, 240, 128, 255)); // sand
    renderer.AddGradientPoint ( 0.4000, utils::Color ( 32, 200,   0, 255)); // grass
    renderer.AddGradientPoint ( 0.6500, utils::Color (223, 191, 159, 255)); // dirt
    renderer.AddGradientPoint ( 0.7500, utils::Color (122, 102,  82, 255)); // rock
    renderer.AddGradientPoint ( 1.0000, utils::Color (255, 255, 255, 255)); // Snow */

    renderer.EnableLight ();
    renderer.SetLightContrast (contrast); //3.
    renderer.SetLightBrightness (brightness); //2.0
    renderer.Render();
    utils::WriterBMP Writer;
    Writer.SetSourceImage(image);

    Writer.SetDestFilename(std::to_string(seed) + ".bmp");
    Writer.WriteDestFile();

    cout << "Check the directory. It contains the generated map." << endl;
}
