#ifndef DAC__H__
#define DAC__H__
#include "spi.h"
#include <math.h>       // Trig functions

#define PI 3.14159

unsigned short get_sine(int idx);
void dac_write(char channel, unsigned short value);

#endif // DAC__H__
