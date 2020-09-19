#ifndef _VIDEO_DRIVER_H_
#define _VIDEO_DRIVER_H_

void writePixel(int x, int y, double size, int color[3]);

void writeChar(char c, double size, int color[3]);

void writeWord(char * c,double size, int color[3]);

void newLine();

void clear();

void deleteChar();

void deleteLine();

void screenLine();

void changeScreen();

void scroll(double size);

#endif