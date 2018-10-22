# == Future C++ Map Generator ==

Personal project based on: [Polygonal Map Generation for Games](www-cs-students.stanford.edu/~amitp/game-programming/polygon-map-generation).

I'm using 3rd party libraries: [C++ Bitmap Library](https://github.com/ArashPartow/bitmap) to generate an image of the map.

# == Actual C++ Map Generator ==

I'm creating a map generator based on Perlin Noise.

First, we need to install the [Libnoise Library] (https://github.com/qknight/libnoise) as follows

```
git clone https://github.com/qknight/libnoise
cd libnoise
mkdir build
cd build
cmake ..
make
sudo make install
```

Once you've done this, you can erase the libnoise folder.

Now, its time to include it to our program:

```c++
#include <noise/noise.h>

using namespace noise;
```

Makefile as an example to show how to include the library while compiling.

```
CFLAGS = -o
CC = g++

MG: main.o 
	$(CC) main.o $(CFLAGS) MG.out -I ./Noise/ -L -/Noise -lnoise

main.o:
	$(CC) -c main.cpp

clean:
	rm -f core *.o
```

Once you've compiled, try to execute the binary. It is possible that you get the following error:
```
error while loading shared libraries: libnoise.so: cannot open shared object file: No such file or directory
```

It it is the case, do the following:
```
sudo find / -name libnoise.so
echo $LD_LIBRARY_PATH
```
if there is nothing to be displayed, add the default path value
```
LD_LIBRARY_PATH=/usr/local/lib
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib/libnoise.so
export LD_LIBRARY_PATH
```

Then you should be able to run your application.
