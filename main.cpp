#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <climits>

using namespace std;

struct Point{
	int x, y;
};

struct RGB{
	unsigned int R, G, B;
};

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

void createSites()
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
            }else{
                
            }
        }
    }
}

void setSitesPoints()
{
    // Sites has 3x3 black dot
    for(const auto & point : points_){
        int x = point.x, y = point.y;
        for(int i = -1; i <= 1; i++){
            for(int j = -1; j <= 1; j++){
                
                // Pintamos sobre el bitmap
                
            }
        }       
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

private:

};

int main(){
	int c, w, h;
	cout << "Please, introduce c, w, and h." << endl;
	cin >> c >> w >> h;

	Voronoi V;
	V.createPoints(c, w, h);
	V.createColors();
        cout << "===================" << endl;
        V.createSites();
        V.setSitesPoints();
        cout << "===================" << endl;
	V.printPoints();
	V.printColors();
}
