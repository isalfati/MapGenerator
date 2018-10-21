#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <string>
#include <climits>

using namespace std;

struct Point{
	int x, y;
};

struct RGB{
	unsigned int R, G, B;
};

typedef vector <vector <vector<unsigned char> > > Map;

const int fileHeaderSize = 14;
const int infoHeaderSize = 40;
const int bytesPerPixel = 3;

class Voronoi{

vector<Point> points_;
vector<RGB> colors_;
int size_;

public:


void createPoints(int count, int w, int h) // Por ahora daré un size input
{
	int x_, y_;
	size_ = w;
	Point myP_;
	for(int i = 0; i < count; i++){
		x_ = rand() % w;
		y_ = rand() % h;
		myP_.x = x_;
		myP_.y = y_;
		points_.push_back(myP_);
	}
}

void createColors() 
{
	unsigned int r_, g_, b_;
	RGB myColor_;
	for(int i = 0; i < points_.size(); i++){
		r_ = rand() % 256; g_ = rand() % 256; b_ = rand() % 256;
		myColor_.R = r_; myColor_.G = g_; myColor_.B = b_;
        cout << "INDEX: " << i << " has (RGB): (" << myColor_.R << ", " << myColor_.G << ", " << myColor_.B << ")" << endl;
		colors_.push_back(myColor_);
	}
}

static int DistanceSqrd(const Point & point, int x, int y)
{
    int xd = x - point.x;
    int yd = y - point.y;
    int result = (xd * xd) + (yd * yd);
    return sqrt(result);
    
}

void createSites(Map &myMap)
{
    //Distance to each point???
    int w, h, d;
    w = h = size_;
    for(int hh = 0; hh < h; hh++){
        for(int ww = 0; ww < w; ww++){
            int ind = -1, dist = INT_MAX;
            for(size_t it = 0; it < points_.size(); it++){
                const Point & p = points_[it];
                d = DistanceSqrd(p, ww, hh);
                if (d < dist){
                    dist = d;
                    ind = it;
                }
                
                cout << "Point : " << p.x << ", " << p.y;
                cout << " with distance: " << d;
                cout << " has the color: (RGB) : (" << colors_[ind].R << "," << colors_[ind].G << "," << colors_[ind].B << ")" << endl;
            }
            if( ind >= 0){
                // La idea sería pintar el pixel de un color concreto
                myMap[hh][ww][0] = colors_[ind].R;
                myMap[hh][ww][1] = colors_[ind].G;
                myMap[hh][ww][2] = colors_[ind].B;
            }else{
                
            }
        }
    }
}

void setSitesPoints(Map &myMap)
{
    // Sites has 3x3 black dot
    for(const auto & point : points_){
        int x = point.x, y = point.y;
        myMap[x][y][0] = 0;
        myMap[x][y][1] = 0;
        myMap[x][y][2] = 0;
        //for(int i = -1; i <= 1; i++){
        //    for(int j = -1; j <= 1; j++){
               // myMap[x+i][y+j][0] = 0;
               // myMap[x+i][y+j][1] = 0;
               // myMap[x+i][y+j][2] = 0;
        //    }
        //}       
    }
}

void printPoints()
{
	int cont = 0;
	for(int i = 0; i < points_.size(); i++){
		cont++;
		cout << points_[i].x << ":" << points_[i].y << ", ";
		if(cont % 5 == 0) cout << endl;
	}
	cout << endl;
}

void printColors()
{
	int cont = 0;
	for(int i = 0; i < colors_.size(); i++){
		cont++;
		cout << "(" << colors_[i].R << ", " << colors_[i].G << ", " << colors_[i].B << ") ";
		if(cont % 5 == 0) cout << endl;
	}
	cout << endl;
}

unsigned char* createBitmapInfoHeader(int height, int width){
    static unsigned char infoHeader[] = {
        0,0,0,0, /// header size
        0,0,0,0, /// image width
        0,0,0,0, /// image height
        0,0, /// number of color planes
        0,0, /// bits per pixel
        0,0,0,0, /// compression
        0,0,0,0, /// image size
        0,0,0,0, /// horizontal resolution
        0,0,0,0, /// vertical resolution
        0,0,0,0, /// colors in color table
        0,0,0,0, /// important color count
    };

    infoHeader[ 0] = (unsigned char)(infoHeaderSize);
    infoHeader[ 4] = (unsigned char)(width    );
    infoHeader[ 5] = (unsigned char)(width>> 8);
    infoHeader[ 6] = (unsigned char)(width>>16);
    infoHeader[ 7] = (unsigned char)(width>>24);
    infoHeader[ 8] = (unsigned char)(height    );
    infoHeader[ 9] = (unsigned char)(height>> 8);
    infoHeader[10] = (unsigned char)(height>>16);
    infoHeader[11] = (unsigned char)(height>>24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(bytesPerPixel*8);

    return infoHeader;
}

unsigned char* createBitmapFileHeader(int height, int width){
    int fileSize = fileHeaderSize + infoHeaderSize + bytesPerPixel*height*width;

    static unsigned char fileHeader[] = {
        0,0, /// signature
        0,0,0,0, /// image file size in bytes
        0,0,0,0, /// reserved
        0,0,0,0, /// start of pixel array
    };

    fileHeader[ 0] = (unsigned char)('B');
    fileHeader[ 1] = (unsigned char)('M');
    fileHeader[ 2] = (unsigned char)(fileSize    );
    fileHeader[ 3] = (unsigned char)(fileSize>> 8);
    fileHeader[ 4] = (unsigned char)(fileSize>>16);
    fileHeader[ 5] = (unsigned char)(fileSize>>24);
    fileHeader[10] = (unsigned char)(fileHeaderSize + infoHeaderSize);

    return fileHeader;
}

void generateImage(Map & myMap, string name)
{
    unsigned char* fileHeader = createBitmapFileHeader(size_, size_);
    unsigned char* infoHeader = createBitmapInfoHeader(size_, size_);
    unsigned char padding[3] = {0, 0, 0};
    int paddingSize = (4-(size_*bytesPerPixel)%4)%4;

    char n = name;
    FILE* imageFile = fopen("hello", "wb");

    fwrite(fileHeader, 1, fileHeaderSize, imageFile);
    fwrite(infoHeader, 1, infoHeaderSize, imageFile);

    int i;
    for(i=0; i<size_; i++){
        fwrite(myMap+(i*size_*bytesPerPixel), bytesPerPixel, size_, imageFile);
        fwrite(padding, 1, paddingSize, imageFile);
    }

    fclose(imageFile);
}

private:

};

int main(){
	int c, w, h;
	cout << "Please, introduce c, h, and w." << endl;
	cin >> c >> h >> w;
    string name;
    cout << "Now, introduce the name of the output file." << endl;
    cin >> name;

    Map myMap(h, vector<vector<unsigned char> > (w, vector<unsigned char>(c)));
    
	Voronoi V;
	V.createPoints(c, w, h);
	V.createColors();
        cout << "===================" << endl;
        V.createSites(myMap);
        V.setSitesPoints(myMap);
        cout << "===================" << endl;
	V.printPoints();
	V.printColors();
    
    V.generateImage(myMap, name);
}
