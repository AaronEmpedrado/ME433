#ifndef WS2812B_H__
#define WS2812B_H__

#include<xc.h> // processor SFR definitions
#define LEDS 4  // Macro so you dont have to change it every time

// link three 8bit colors together
typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} wsColor; 

void ws2812b_setup();
void ws2812b_setColor(wsColor*,int);
void ws2812b_initColor(wsColor*, int, unsigned char, unsigned char, unsigned char); //Init specific LED in array to values
wsColor HSBtoRGB(float hue, float sat, float brightness);

#endif
