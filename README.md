# == Future C++ Map Generator ==

Personal project based on: [Polygonal Map Generation for Games](www-cs-students.stanford.edu/~amitp/game-programming/polygon-map-generation).

I'm using 3rd party libraries: [C++ Bitmap Library](https://github.com/ArashPartow/bitmap) to generate an image of the map.

# == Actual C++ Map Generator ==

I'm creating a map generator based on Perlin Noise

First, Follow this steps:

1) 
```git clone https://github.com/qknight/libnoise
cd libnoise
mkdir build
cd build
cmake ..
make
sudo make install```


2) If you get the following error once you've included the noise library:

"error while loading shared libraries: libnoise.so: cannot open shared object file: No such file or directory"

do the following:

sudo find / -name libnoise.so
echo $LD_LIBRARY_PATH

if there is nothing to be displayed, add the default path value
LD_LIBRARY_PATH=/usr/local/lib

Then:

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib/libnoise.so
export LD_LIBRARY_PATH

Then you should be able to run your application.
