#include <proc/p32mx170f256b.h>
#include "dac.h"

// write the 12-bit data to the appropriate channel
void dac_write(char channel, unsigned short value) {
    unsigned char c;
    unsigned short v = value;
    unsigned short p;
    if (channel == 'a') {
        c = 0;
    } else if (channel == 'b') {
        c = 1;
    } else {
        return;     // invalid channel
    }
    // Set up the write value for the DAC
    p = (c << 15);          // Bring select bit to bit 15
    p = p | (0b111 << 12);   // buffered, 1x gain, active mode
    v = v & 0xfff;          // just ensure only bottom 12 bits are used
    p = p | v;              // add the lower 12 bits to the write value

    // The actual writing portion
    LATAbits.LATA0 = 0;     // bring CS low
    spi_io(p>>8);           // write the upper byte to output first
    spi_io(p);              // write the lower byte to output
    LATAbits.LATA0 = 1;     // bring CS high
}


// Helper function to help populate sine array
// Given an index into the 200 element array, return the appropriate unsigned short
// to use at that index scaled to the 12-bit value for the DAC
unsigned short get_sine(int idx) {
   double scaled_rad = (double)((2*PI / 50) * idx);     // Shift to start at a trough
   double temp_sine = sin(scaled_rad);
   unsigned short scaled_sine;
   scaled_sine = (unsigned short)(temp_sine * 2048) + 2048;
//   if (temp_sine >= 0) {
//       scaled_sine = (unsigned short)(temp_sine * 2048) + 2048;
//   } else {
//       scaled_sine = scaled_sine - (unsigned short)(temp_sine * -2048);
//   }
   return scaled_sine;
}




