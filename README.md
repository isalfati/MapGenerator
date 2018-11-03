# == C++ Map Generator ==

I'm creating a map generator based on Perlin Noise.

First, we need to install the Libnoise Library (https://github.com/qknight/libnoise) as follows

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
STD = -std=c++11
CC = g++

MG: main.o 
	$(CC) main.o $(CFLAGS) MG.out -I/usr/local/include -L/usr/local/lib -lnoise

main.o:
	$(CC) $(STD) -c main.cpp

clean:
	rm -f core *.o *.out
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

NOTE: If you are using noiseutils library (http://libnoise.sourceforge.net/downloads/noiseutils.zip), do not forget to include noiseutils.cpp into the main file.

#### Find below some examples
![Map1](https://github.com/isalfati/MapGenerator/blob/master/img/map_1.bmp)
![Map2](https://github.com/isalfati/MapGenerator/blob/master/img/map_2.bmp)
![Map3](https://github.com/isalfati/MapGenerator/blob/master/img/map_3.bmp)
![Map4](https://github.com/isalfati/MapGenerator/blob/master/img/map_4.bmp)

Here are some mono-color examples

![Map5](https://github.com/isalfati/MapGenerator/blob/master/img/map_5.bmp)
![Map6](https://github.com/isalfati/MapGenerator/blob/master/img/map_6.bmp)
![Map7](https://github.com/isalfati/MapGenerator/blob/master/img/map_7.bmp)

Also, an aplication of this library could be a blood moon generator:

![BM1](https://github.com/isalfati/MapGenerator/blob/master/img/BM_1.bmp) 
![BM2](https://github.com/isalfati/MapGenerator/blob/master/img/BM_2.bmp)
![BM3](https://github.com/isalfati/MapGenerator/blob/master/img/BM_3.bmp) 
![BM4](https://github.com/isalfati/MapGenerator/blob/master/img/BM_4.bmp)
