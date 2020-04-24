#include "font.h"
#include "ssd1306.h"

// can index into array with a char
void drawChar(unsigned char x, unsigned char y, char c) {
    char ascii_idx = c - 0x20;       //ASCII is the array
    unsigned char i;      // loop iterator for width
    unsigned char j;      // loop iterator for height

    // Loop from left to right
    // drawing character from left-> right, down-> up
    for (i=0; i<5; i++){
        char col = ASCII[(unsigned)ascii_idx][i];
        for (j=0; j<8; j++) {
            unsigned char mask = 0x01 << j;
            ssd1306_drawPixel(x+i, y+j, ((col & mask) >> j));
        }
    }
    ssd1306_update(); 
}
