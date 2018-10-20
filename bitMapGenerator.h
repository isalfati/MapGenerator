#ifndef BITMAPGENERATOR_H_
#define BITMAPGENERATOR_H_

void generateBitMapImage(unsigned char * image, int height, int width, char * imageFileName);
unsigned char * createBitMapFileHeader(int height, int width);
unsigned char * createBitMapInfoHeader(int height, int width);

#endif // BITMAPGENERATOR_H_
